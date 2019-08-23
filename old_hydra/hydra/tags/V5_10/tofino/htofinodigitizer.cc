#include <TRandom.h>
#include <time.h>
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
#include <iostream.h>
#include "htofinocalpar.h"

//*-- Author : D.Vasiliev
//*-- Modified: 09/03/2000 by R.Holzmann
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

void HTofinoDigitizer::initParContainer() {

pTofinoCalPar=
  (HTofinoCalPar *)gHades->getRuntimeDb()->getContainer("TofinoCalPar");
  if (!pTofinoCalPar) {
    pTofinoCalPar=new HTofinoCalPar;
    gHades->getRuntimeDb()->addContainer(pTofinoCalPar);
  }
}

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

  time_t curtime;

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
/*    HTofinoDetector* tofino=(HTofinoDetector*)(gHades->getSetup()
                                                     ->getDetector("Tofino"));
    fRawCat=tofino->buildMatrixCategory("HTofRawSim");
*/
    if (!fRawCat) 
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofinoRaw,fRawCat,"Tofino");
  }

  time(&curtime);
  gRandom->SetSeed(curtime);
  
  return kTRUE;
}


Int_t HTofinoDigitizer::execute(void) {

  HGeantTof* geant = 0;
  HTofinoRaw* raw = 0;

  Float_t geaTof, geaEner, geaX, geaY; // Geant output  
  Int_t numTrack; //number of current track 

  Float_t tdcTofino, adcTofino, prevTdcTofino, prevAdcTofino;
  Int_t chargeCh;
  Float_t timeTofino, timeResol;
/*
  Float_t hl, vg, al, ar, trz, eq, yp;
  Float_t relAmplResol = 0.1;
  Float_t amplResol;
  Float_t deg2rad = 0.017453293;
  Float_t chargeRef;
  Float_t minEnerRelease = 1.8;  // units ?

  vg = 16.0;
  al = 200.;
  ar = 47.;
  trz = 0.1;
  eq = 0.24;  
  yp = 11100.0;
*/
/*
  Float_t chTdc = 0.031;    // ns/ch

  Int_t chargeCh;
  hl = 690.; // in mm
  Float_t fSlope, fOffset;
  Float_t aTofino = 1240.2 * chTdc;  //  tof(y) = aTofino + bTofino*y 
  Float_t bTofino = 2.454 * chTdc;
  Float_t deltaA = 12. * chTdc;
  Float_t deltaB = 0.125 * chTdc;
*/	

  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)
  while ((geant=(HGeantTof *)iter->Next())!=0) {

    fLoc[1] = geant->getModule();
    if(fLoc[1] <= 21) continue;    // this is a Tof, skip it please.
    fLoc[1] = fLoc[1] - 22;

    fLoc[0] = geant->getSector();
    fLoc[2] = geant->getCell();
    //        printf("cell = %d\n",fLoc[2]);
    cout <<"Geant fLoc[0, 1, 2] = " <<fLoc[0] <<" " <<fLoc[1] <<" " <<fLoc[2] <<endl;
    geant->getHit(geaEner,geaX,geaY,geaTof);  // geaX, geaY - in mm
    numTrack = geant->getTrack(); 

// Translate Geant fLoc to HTofinoRaw fLoc
    fLoc[2] = fLoc[1]; fLoc[1] = 0;
    cout <<"raw fLoc[0, 1, 2] = " <<fLoc[0] <<" " <<fLoc[1] <<" " <<fLoc[2] <<endl;
    raw = (HTofinoRaw*) fRawCat->getObject(fLoc); //test if cell already in use
    if(raw) {
//       raw->incNHit();  // increment counter for additional hits
       prevTdcTofino = raw->getTime();
       prevAdcTofino = raw->getCharge();
    }
    else {
       prevTdcTofino = 100000.;	
       prevAdcTofino = 0.;
       raw = (HTofinoRaw*) fRawCat->getSlot(fLoc);  // get new slot
       if(!raw) continue;
       raw = new(raw) HTofinoRaw;
//       raw->setNHit(1);
    }

//  Short_t nHits = raw->getNHit();  // number of hits in this cell up till now

    cout <<"geaEner,geaX,geaY,geaTof,numTrack" <<endl;
    cout <<geaEner <<" " <<geaX <<" " <<geaY <<" " <<geaTof <<" " <<numTrack <<endl;

    timeTofino = ((HTofinoCalPar*)pTofinoCalPar)
                                ->calcLightPropTime(fLoc[0],fLoc[2],geaY);
    timeResol = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcDeltaLightPropTime(fLoc[0],fLoc[2],timeTofino);
    cout <<"time = " <<timeTofino <<" +/- " <<timeResol <<endl;


    //      tdcTofino = aTofino + bTofino*(y23242526[fLoc[2]]-geaY)*0.1;
    //  timeResol = tdcTofino * sqrt(pow(deltaA/aTofino,2) + pow(deltaB/bTofino,2));
      timeTofino = gRandom->Gaus(timeTofino,timeResol);
      timeTofino = timeTofino + geaTof;
      cout <<"time = " <<timeTofino <<endl;
      tdcTofino = ((HTofinoCalPar*)pTofinoCalPar)->
		   timeToChannel(fLoc[0],fLoc[2],timeTofino);
    cout <<"tdc = " <<tdcTofino <<endl;

      tdcTofino = (Int_t)(tdcTofino + 0.5);
      if (tdcTofino < 0) tdcTofino = 0;
      if (tdcTofino > 4095.) tdcTofino = 4095;
      tdcTofino = ( Float_t )(tdcTofino);
      if(tdcTofino > prevTdcTofino) tdcTofino = prevTdcTofino;
    cout <<"tdc = " <<tdcTofino <<endl;
/*
    if(vg!=0.0 && al!=0.0) {
      adcTofino = geaEner*yp*eq*0.5*(1 - cos(ar*deg2rad))*exp(-(hl-geaY)/al);
      amplResol = adcTofino*relAmplResol;
      adcTofino = gRandom->Gaus(adcTofino,amplResol);
	
      chargeRef = yp*eq*0.5*(1-cos(ar*deg2rad))*minEnerRelease*exp(-hl/al);
      chargeCh = (Int_t) ((adcTofino/chargeRef)*1024. + prevChargeL);
      if (chargeCh < 0) chargeCh = 0;
      if (chargeCh > 4095)  chargeCh = 4095;
      adcTofino = (Float_t)chargeCh;
    } 
    else 
      adcTofino = 4095.;
*/
      chargeCh = (Int_t)(geaEner+0.5);
      if (chargeCh < 0) chargeCh = 0;
      if (chargeCh > 4095)  chargeCh = 4095;
      adcTofino = (Float_t)chargeCh;

    raw->setTime(tdcTofino);
    raw->setCharge(adcTofino);
    raw->setSector((Char_t) fLoc[0]);
    raw->setModule((Char_t) fLoc[1]);
    raw->setCell((Char_t) fLoc[2]); 

    cout <<"sec,mod,cell =" <<raw->getSector() <<" " <<raw->getModule() 
<<" " <<raw->getCell() <<endl; 
    cout <<"tdc,adc =" <<raw->getTime() <<" " <<raw->getCharge() <<endl; 
    cout <<"------------------------------" <<endl;
  } // while ((geant=(HGeantTof *)iter->Next())!=0) 
  return 0;

} // htofinodigitizer

ClassImp(HTofinoDigitizer)





