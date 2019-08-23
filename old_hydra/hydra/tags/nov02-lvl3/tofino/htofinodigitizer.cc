using namespace std;
#include <TRandom.h>
#include <time.h>
#include <iostream> 
#include <iomanip>
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
//  
/////////////////////////////////////////////////////////////////////

HTofinoDigitizer::HTofinoDigitizer(void) {

  fGeantCat=0;
  fCalCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,0,0,0);
  iter=0; iterCal=0;     
}

HTofinoDigitizer::HTofinoDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fGeantCat=0;
  fCalCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,0,0,0);
  iter=0; iterCal=0;    
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

  fCalCat = gHades->getCurrentEvent()->getCategory(catTofinoCal);
  if (!fCalCat) {
    fCalCat = ((HTofinoDetector*)(gHades->getSetup()->getDetector("Tofino")))
                                ->buildMatrixCategory("HTofinoCalSim");
    if (!fCalCat) 
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofinoCal,fCalCat,"Tofino");
  }
  iter = (HIterator *)fGeantCat->MakeIterator("native");
  iterCal = (HIterator *)fCalCat->MakeIterator("native");

  return kTRUE;
}

void HTofinoDigitizer::initParContainer() {

  pTofinoCalPar = (HTofinoCalPar *)gHades->getRuntimeDb()
                                         ->getContainer("TofinoCalPar");
}

Int_t HTofinoDigitizer::execute(void) {

  HLocation ffLoc;
  HGeantTof* pGeant = 0;
  HTofinoCalSim* pCal = 0;
  //HTofinoCalSim* cal = 0; // unused

  Float_t geaTof, geaEner, geaX, geaY, geaMom, trackLen;  // Geant output
  Int_t numTrack;
  Float_t tdcTofino = 0, adcTofino = 0, prevTdcTofino, prevAdcTofino;
  Float_t timeTofino, timeResol, thrCFD;

  prevTdcTofino = 100000.;	
  prevAdcTofino = 0.;

  ffLoc.set(2,-1,-1);

  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)
  //int nHit = 0; // unused
  while ((pGeant=(HGeantTof *)iter->Next())!=0) {  // loop over HGeant hits

    fLoc[1] = pGeant->getModule();
    if (fLoc[1]<22) continue;    // this is a Tof, skip it
    fLoc[0] = pGeant->getSector();
    fLoc[2] = pGeant->getCell();
    pGeant->getHit(geaEner,geaX,geaY,geaTof,geaMom,trackLen);
    numTrack = pGeant->getTrack(); 
    fLoc[2] = fLoc[1] - 22;
    fLoc[1] = 0;

    pCal = (HTofinoCalSim*) fCalCat->getObject(fLoc);

    if (pCal) {   // test if this cell has already seen a hit
       pCal->incNHit();  // increment hit counter
       prevTdcTofino = pCal->getTime();
       prevAdcTofino = pCal->getCharge();
    }
	/*
    else {        // this cell has not yet been hit, ask for a new slot in cat
       prevTdcTofino = 100000.;	
       prevAdcTofino = 0.;
       pCal = (HTofinoCalSim*)fCalCat->getNewSlot(fLoc);  // get new slot
       if(!pCal) continue;
       pCal = new(pCal) HTofinoCalSim();
       pCal->setNHit(1);
    }
	*/

    timeTofino = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcLightPropTime(fLoc[0],fLoc[2],geaY);
    timeResol = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcDeltaLightPropTime(fLoc[0],fLoc[2],geaY);

      timeTofino = gRandom->Gaus(timeTofino,timeResol);
      tdcTofino = timeTofino + geaTof;    // time in ns
      if (tdcTofino > prevTdcTofino) tdcTofino = prevTdcTofino;

      adcTofino = geaEner + prevAdcTofino;                    // energy in MeV
      ffLoc[0] =  fLoc[0] ;
      ffLoc[1] =  fLoc[2] ;
	  
      thrCFD = ((HTofinoCalPar*)pTofinoCalPar)->getCFDThreshold(ffLoc);
      if(adcTofino > thrCFD) { 
	     if(!pCal) {
	      pCal = (HTofinoCalSim*)fCalCat->getNewSlot(fLoc);  // get new slot
          if(!pCal) continue;
          pCal = new(pCal) HTofinoCalSim();
          pCal->setNHit(1);
		}
        pCal->setTime(tdcTofino);
        pCal->setCharge(adcTofino);
        pCal->setSector(fLoc[0]);
        pCal->setModule(fLoc[1]);
        pCal->setCell(fLoc[2]);
        pCal->setTrack(numTrack);
        pCal->setGeaTof(geaTof);
	  }
  } // while ((pGeant=(HGeantTof *)iter->Next())!=0) 
/*
  // Put tdcTofino and adcTofino equal to zero 
  // for hits with charge less then CFD Thresholds.
    iterCal->Reset();   // this works only in split mode=2
    while ((cal=(HTofinoCalSim *)iterCal->Next())!=0) { // loop over TofinoCalSim hits

      ffLoc[0] = cal->getSector();
      ffLoc[1] = cal->getCell();

      thrCFD = ((HTofinoCalPar*)pTofinoCalPar)->getCFDThreshold(ffLoc);
     
      if(cal->getCharge() > thrCFD) {
        pCal->setTime(cal->getTime());
        pCal->setCharge(cal->getCharge());
        pCal->setSector(cal->getSector());
        pCal->setModule(cal->getModule());
        pCal->setCell(cal->getCell());
        pCal->setTrack(cal->getTrack());
        pCal->setGeaTof(cal->getGeaTof());
      }
*/
/*
      if(adcTofino < thrCFD) {
	   cal->setTime(0.);
	   cal->setCharge(0.);
       cal->setAddress(fLoc[0],fLoc[1],fLoc[2]);
	   cal->setTrack(numTrack);
	   cal->setGeaTof(geaTof);
      }

    } // while ((cal=(HTofinoCalSim *)iterCal->Next())!=0)
*/
    /*    
    iterCal->Reset();   // this works only in split mode=2
    while ((pCal=(HTofinoCalSim *)iterCal->Next())!=0) { // loop over HGeant hits
      fLoc[0] = pCal->getSector();
      fLoc[1] = pCal->getModule();
      fLoc[2] = pCal->getCell();
      tdcTofino = pCal->getTime();
      adcTofino = pCal->getCharge();
      numTrack = pCal->getTrack();
      geaTof = pCal->getGeaTof();
    }
    */
//  printf("Entries on stop = %d\n",fCalCat->getEntries());
  return 0;

} // htofinodigitizer

ClassImp(HTofinoDigitizer)










