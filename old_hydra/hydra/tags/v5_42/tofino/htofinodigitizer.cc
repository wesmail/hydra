#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include "htofinodigitizer.h"
#include "tofinodef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "htofinodetector.h"
#include "hgeanttof.h"
#include "htofinocalsim.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "htofinocalpar.h"

//*-- Author : D.Vasiliev
//*-- Modified: 26/10/2000 by M.Golubeva
//*-- Modified: 28/06/2000 by R.Holzmann
//*-- Modified: 24/06/2000 by M.Golubeva
//*-- Modified: 1/12/99 by V.Charriere
//*-- Modified: 24/10/99 by D.Vasiliev

/////////////////////////////////////////////////////////////////////
//
//  HTofinofDigitizer digitizes HGeant data, puts output values into 
//  cal data category
//
//  Need to implement CFD thresholds, overflow handling, quenching ?
//  
/////////////////////////////////////////////////////////////////////

HTofinoDigitizer::HTofinoDigitizer(void) {

  fGeantCat=0;
  fCalCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,0,0,0);
  iter=0;    
}

HTofinoDigitizer::HTofinoDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fGeantCat=0;
  fCalCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,0,0,0);
  iter=0;    
}

HTofinoDigitizer::~HTofinoDigitizer(void) {
}

Bool_t HTofinoDigitizer::init(void) {

  initParContainer();

  fGeantCat = gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
  if (!fGeantCat) {
    fGeantCat = gHades->getSetup()->getDetector("Tof")
                                  ->buildCategory(catTofGeantRaw);
    if (!fGeantCat)
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofGeantRaw,fGeantCat,"Tof");
  }
  iter = (HIterator *)fGeantCat->MakeIterator("native");

  fCalCat = gHades->getCurrentEvent()->getCategory(catTofinoCal);
  if (!fCalCat) {
    fCalCat = ((HTofinoDetector*)(gHades->getSetup()->getDetector("Tofino")))
                                ->buildMatrixCategory("HTofinoCalSim");
    if (!fCalCat) 
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofinoCal,fCalCat,"Tofino");
  }

  return kTRUE;
}

void HTofinoDigitizer::initParContainer() {

  pTofinoCalPar = (HTofinoCalPar *)gHades->getRuntimeDb()
                                         ->getContainer("TofinoCalPar");
  if (!pTofinoCalPar) {
    pTofinoCalPar=new HTofinoCalPar;
    gHades->getRuntimeDb()->addContainer(pTofinoCalPar);
  }
}

Int_t HTofinoDigitizer::execute(void) {

  HGeantTof* pGeant = 0;
  HTofinoCalSim* pCal = 0;

  Float_t geaTof, geaEner, geaX, geaY, geaMom, trackLen;  // Geant output
  Int_t numTrack;                       // number of current track 

  Float_t tdcTofino, adcTofino, prevTdcTofino, prevAdcTofino;
  Float_t timeTofino, timeResol;

  prevTdcTofino = 100000.;	
  prevAdcTofino = 0.;

  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)
//  printf("Entries on start = %d\n",fCalCat->getEntries());

  while ((pGeant=(HGeantTof *)iter->Next())!=0) {  // loop over HGeant hits

    fLoc[1] = pGeant->getModule();
    if (fLoc[1]<22) continue;    // this is a Tof, skip it
    fLoc[0] = pGeant->getSector();
    fLoc[2] = pGeant->getCell();
    pGeant->getHit(geaEner,geaX,geaY,geaTof,geaMom,trackLen);
            // geaX, geaY, trackLen in mm, geaTof in ns, geaMom in MeV/c
    numTrack = pGeant->getTrack(); 

// Translate Geant fLoc to HTofinoCal fLoc
    fLoc[2] = fLoc[1] - 22;
    fLoc[1] = 0;

    pCal = (HTofinoCalSim*) fCalCat->getObject(fLoc);

    if (pCal) {   // test if this cell has already seen a hit
       pCal->incNHit();  // increment hit counter
       prevTdcTofino = pCal->getTime();
       prevAdcTofino = pCal->getCharge();
    }
    else {        // this cell has not yet been hit, ask for a new slot in cat
       prevTdcTofino = 100000.;	
       prevAdcTofino = 0.;
       pCal = (HTofinoCalSim*)fCalCat->getNewSlot(fLoc);  // get new slot
       if(!pCal) continue;
       pCal = new(pCal) HTofinoCalSim();
       pCal->setNHit(1);
    }

    timeTofino = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcLightPropTime(fLoc[0],fLoc[2],geaY);
    timeResol = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcDeltaLightPropTime(fLoc[0],fLoc[2],geaY);

//    cout <<"sec,cell,geaY = " <<fLoc[0] <<" " <<fLoc[2] <<" " <<geaY <<endl;
//    cout <<"timeTofino,timeResol = " <<timeTofino <<" " <<timeResol <<endl;
    timeTofino = gRandom->Gaus(timeTofino,timeResol);
//    cout <<"geaTof,timeTofino(Gaus) = " <<geaTof <<" " <<timeTofino <<endl;
    timeTofino = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcLightPropTimeFull(fLoc[0],fLoc[2],timeTofino);
//    cout <<"timeTofino(Full) = " <<timeTofino <<endl;
    tdcTofino = timeTofino + geaTof;                        // time in ns
// cout <<"tdcTofino,prevTdcTofino = " <<tdcTofino <<" " <<prevTdcTofino <<endl;
    if (tdcTofino > prevTdcTofino) tdcTofino = prevTdcTofino;
    adcTofino = geaEner + prevAdcTofino;                    // energy in MeV
// cout <<"tdcTofino = " <<tdcTofino <<endl;

    pCal->setTime(tdcTofino);
    pCal->setCharge(adcTofino);
    pCal->setSector(fLoc[0]);
    pCal->setModule(fLoc[1]);
    pCal->setCell(fLoc[2]);
    pCal->setTrack(numTrack);
    pCal->setGeaTof(geaTof);
/*
    printf("n=%d: ",nHits);
    for(Int_t l=0;l<nHits;l++) printf(" %f %d",pCal->getGeaTof(l),pCal->getTrack(l));
    printf("\n");
*/
  } // while ((pGeant=(HGeantTof *)iter->Next())!=0) 

//  printf("Entries on stop = %d\n",fCalCat->getEntries());
  return 0;

} // htofinodigitizer

ClassImp(HTofinoDigitizer)







