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
#include "htofinoraw.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "htofinocalpar.h"

//*-- Author : D.Vasiliev
//*-- Modified: 08/06/2000 by R.Holzmann
//*-- Modified: 02/06/2000 by M.Golubeva
//*-- Modified: 1/12/99 by V.Charriere
//*-- Modified: 24/10/99 by D.Vasiliev

/////////////////////////////////////////////////////////////////////
//
//  HTofDigitizer digitizes data, puts output values into 
//  raw data category
//
//  Need to implement CFD thresholds, overflow handling, quenching ?
//  
/////////////////////////////////////////////////////////////////////

HTofinoDigitizer::HTofinoDigitizer(void) {

  fGeantCat=0;
  fRawCat=0;
  pTofinoCalPar = NULL;
  initParContainer();
  fLoc.set(3,0,0,0);
  iter=0;    

}

HTofinoDigitizer::HTofinoDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fGeantCat=0;
  fRawCat=0;
  pTofinoCalPar = NULL;
  initParContainer();
  fLoc.set(3,0,0,0);
  iter=0;    
}

HTofinoDigitizer::~HTofinoDigitizer(void) {
}

Bool_t HTofinoDigitizer::init(void) {

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

  fRawCat = gHades->getCurrentEvent()->getCategory(catTofinoRaw);
  if (!fRawCat) {
    fRawCat = gHades->getSetup()->getDetector("Tofino")
                                ->buildCategory(catTofinoRaw);
    if (!fRawCat) 
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofinoRaw,fRawCat,"Tofino");
  }

  return kTRUE;
}

void HTofinoDigitizer::initParContainer() {

pTofinoCalPar=
  (HTofinoCalPar *)gHades->getRuntimeDb()->getContainer("TofinoCalPar");
  if (!pTofinoCalPar) {
    pTofinoCalPar=new HTofinoCalPar;
    gHades->getRuntimeDb()->addContainer(pTofinoCalPar);
  }
}


Int_t HTofinoDigitizer::execute(void) {

  HGeantTof* geant = 0;
  HTofinoRaw* raw = 0;

  Float_t geaTof, geaEner, geaX, geaY;  // Geant output  
  Int_t numTrack;                       //number of current track 

  Float_t tdcTofino, adcTofino, prevTdcTofino, prevAdcTofino;
  Int_t chargeCh;
  Float_t timeTofino, timeResol;

  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)
  while ((geant=(HGeantTof *)iter->Next())!=0) {

    fLoc[1] = geant->getModule();
    if(fLoc[1] <= 21) continue;    // this is a Tof, skip it
    fLoc[1] = fLoc[1] - 22;

    fLoc[0] = geant->getSector();
    fLoc[2] = geant->getCell();

    geant->getHit(geaEner,geaX,geaY,geaTof);  // geaX, geaY - in mm
    numTrack = geant->getTrack(); 

// Translate Geant fLoc to HTofinoRaw fLoc
    fLoc[2] = fLoc[1]; fLoc[1] = 0;
    raw = (HTofinoRaw*) fRawCat->getObject(fLoc); //test if cell already in use
    if(raw) {
//       raw->incNHit();  // increment counter for additional hits
       prevTdcTofino = raw->getTime();
       prevAdcTofino = raw->getCharge();
    }
    else {
       prevTdcTofino = 100000.;	
       prevAdcTofino = 0.;
       raw = (HTofinoRaw*)fRawCat->getNewSlot(fLoc);  // get new slot
       if(!raw) continue;
       raw = new(raw) HTofinoRaw;
//       raw->setNHit(1);
    }

//  Short_t nHits = raw->getNHit();  // number of hits in this cell up till now

    timeTofino = ((HTofinoCalPar*)pTofinoCalPar)
                                ->calcLightPropTime(fLoc[0],fLoc[2],geaY);
    timeResol = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcDeltaLightPropTime(fLoc[0],fLoc[2],timeTofino);

    timeTofino = gRandom->Gaus(timeTofino,timeResol);
    timeTofino = timeTofino + geaTof;
    tdcTofino = ((HTofinoCalPar*)pTofinoCalPar)->
		   timeToChannel(fLoc[0],fLoc[2],timeTofino);

    tdcTofino = (Int_t)(tdcTofino + 0.5);
    if (tdcTofino < 0) tdcTofino = 0;
    if (tdcTofino > 4095.) tdcTofino = 4095;
    if(tdcTofino > prevTdcTofino) tdcTofino = prevTdcTofino;

    chargeCh = (Int_t)(100.*geaEner+0.5) + prevAdcTofino; // needs energy calib
    if (chargeCh < 0) chargeCh = 0;
    if (chargeCh > 4095)  chargeCh = 4095;
    adcTofino = (Float_t)chargeCh;

    raw->setTime((Short_t)tdcTofino);
    raw->setCharge((Short_t)adcTofino);
    raw->setSector(fLoc[0]);
    raw->setModule(fLoc[1]);
    raw->setCell(fLoc[2]); 

  } // while ((geant=(HGeantTof *)iter->Next())!=0) 
  return 0;

} // htofinodigitizer

ClassImp(HTofinoDigitizer)





