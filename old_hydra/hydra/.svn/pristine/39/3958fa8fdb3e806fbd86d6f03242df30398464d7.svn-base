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
//#include "htofinodetector.h"
#include "hgeanttof.h"
#include "htofinoraw.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include <iostream.h>

//*-- Author : D.Vasiliev
//*-- Modified: 1/12/99 by V.Charriere
//*-- Modified: 8/11/99 by R.Holzmann
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
//  initParContainer();
  fLoc.set(3,0,0,0);
  iter=0;    

}

HTofinoDigitizer::HTofinoDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fGeantCat=0;
  fRawCat=0;
//  initParContainer();
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

/*  fRawCat = gHades->getCurrentEvent()->getCategory(catTofinoRaw);
  if (!fRawCat) {
    fRawCat = gHades->getSetup()->getDetector("Tofino")->buildCategory(catTofinoRaw);
    if (!fRawCat) 
       return kFALSE;
    else
       gHades->getCurrentEvent()->addCategory(catTofinoRaw,fRawCat,"Tofino");
  }
*/

  Int_t ind[3] = {6,4,1};
  ((HRecEvent*)gHades->getCurrentEvent())->getPartialEvent(catTofino)
          ->addCategory(catTofinoRaw, new HMatrixCategory("HTofinoRaw",3,ind,1.0));

  iter = (HIterator *)fGeantCat->MakeIterator();
  time(&curtime);
  gRandom->SetSeed(curtime);
  
  return kTRUE;
}


Int_t HTofinoDigitizer::execute(void) {

  HGeantTof* geant = 0;
  HTofinoRaw* raw = 0;

  Float_t geaTof;
  Float_t geaEner;
  Float_t geaX;
  Float_t geaY;  // used by Tofino only

  Float_t tdcTofino, adcTofino;
	
  Int_t timeCh, chargeCh;

  Float_t hl, vg, al, ar, trz, eq, yp;
  Float_t prevTimeL, prevTimeR, prevChargeL, prevChargeR;
  Float_t prevTdcTofino, prevAdcTofino;
	
  Float_t timeResol;
  Float_t relAmplResol = 0.1;
  Float_t amplResol;
  Float_t deg2rad = 0.017453293;
  Float_t chargeRef;
  Float_t minEnerRelease = 1.8;  // units ?
  Float_t channelTDC = 0.04;     // ns/ch

	Float_t aTofino = 1240.2 * channelTDC;  //  tof(y) = aTofino + bTofino*y 
	Float_t bTofino = 2.454 * channelTDC;
	Float_t deltaA = 12. * channelTDC;
	Float_t deltaB = 0.125 * channelTDC;
	
  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)

		hl = 690.; // in mm
	  vg = 16.0;
		al = 200.;
	  ar = 47.;
		trz = 0.1;
    eq = 0.24;  
    yp = 11100.0;

  while ((geant=(HGeantTof *)iter->Next())!=0) {

    fLoc[1] = geant->getModule();
    if(fLoc[1] <= 21) continue;    // this is a Tof, skip it please.
//    else fLoc[1] = 21 - fLoc[1];  // Tof modules in HGeant: (0->21) = (in->out)

    fLoc[0] = geant->getSector();
		fLoc[1] = fLoc[1] - 22;
//    fLoc[2] = geant->getCell();
		fLoc[2] = 0;
		printf("cell = %d\n",fLoc[2]);
		
    raw = (HTofinoRaw*) fRawCat->getObject(fLoc);   // test if cell already in use
    if(raw) {
       raw->incNHit();  // increment counter for additional hits
       prevTdcTofino = raw->getTdcT();
       prevAdcTofino = raw->getAdcT();
    }
    else {
			 prevTdcTofino = 100000.;	
			 prevAdcTofino = 0.;
       raw = (HTofinoRaw*) fRawCat->getSlot(fLoc);  // get new slot
       if(!raw) continue;
       raw = new(raw) HTofinoRaw;
       raw->setNHit(1);
    }

    Short_t nHits = raw->getNHit();  // number of hits in this cell up till now


    geant->getHit(geaEner,geaX,geaY,geaTof);  // geaX, geaY - in mm
	
    if(vg!=0.0 && al!=0.0) {

      tdcTofino = geaTof + (aTofino + bTofino*(hl-geaY)*0.1);
      timeResol = tdcTofino * sqrt(pow(deltaA/aTofino,2) + pow(deltaB/bTofino,2) );
      tdcTofino = gRandom->Gaus(tdcTofino,timeResol);
	
      timeCh = (Int_t) (tdcTofino/channelTDC);
      if (timeCh < 0) tdcTofino = 0.;
	    if (timeCh > 4095) tdcTofino = 4095.;
      tdcTofino = ( Float_t ) (timeCh);
      if(tdcTofino > prevTdcTofino) tdcTofino = prevTdcTofino;

	    adcTofino = geaEner*yp*eq*0.5*(1 - cos(ar*deg2rad))*exp(-(hl-geaY)/al);
    	amplResol = adcTofino*relAmplResol;
    	adcTofino = gRandom->Gaus(adcTofino,amplResol);
	
	    chargeRef = yp*eq*0.5*(1-cos(ar*deg2rad))*minEnerRelease*exp(-hl/al);
//	if(fLoc[1] <= 3) chargeRef *= 3.;
//	if(fLoc[1]>3 && fLoc[1]<=7) chargeRef *= 2.;

	    chargeCh = (Int_t) ((adcTofino/chargeRef)*1024. + prevChargeL);

      if (chargeCh < 0) chargeCh = 0;
    	if (chargeCh > 4095)  chargeCh = 4095;
	    adcTofino = (Float_t)chargeCh;

    } else {

      	tdcTofino = 4095.;
	      adcTofino = 4095.;
    }

    raw->setTdcT(tdcTofino);
    raw->setAdcT(adcTofino);
    raw->setSector((Char_t) fLoc[0]);
    raw->setModule((Char_t) fLoc[1]);
    raw->setCell((Char_t) fLoc[2]); 
  }
  return 0;

}

ClassImp(HTofinoDigitizer)
