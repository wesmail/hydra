#include <TRandom.h>
#include <time.h>
#include "htofdigitizer.h"
#include "tofdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htofdetector.h"
#include "hgeanttof.h"
//#include "htofraw.h"
#include "htofrawsim.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "htofdigitpar.h"
#include <iostream.h>

//*-- Author : D.Vasiliev
//*-- Modified: 16/12/99 by J.Bielcik
//*-- Modified: 9/12/99 by V.Charriere
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


void HTofDigitizer::initParContainer() {

 fDigitPar=(HTofDigitPar *)gHades->getRuntimeDb()->getContainer("TofDigitPar");
  if (!fDigitPar) {
    fDigitPar=new HTofDigitPar;
    gHades->getRuntimeDb()->addContainer(fDigitPar);
  }

}



HTofDigitizer::HTofDigitizer(void) {

  fGeantCat=0;
  fRawCat=0;
  fDigitPar=0;
//  initParContainer();
  fLoc.set(3,0,0,0);
  iter=0;    

}

HTofDigitizer::HTofDigitizer(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fGeantCat=0;
  fRawCat=0;
  fDigitPar=0;
//  initParContainer();
  fLoc.set(3,0,0,0);
  iter=0;    
}

HTofDigitizer::~HTofDigitizer(void) {
}


Bool_t HTofDigitizer::init(void) {

  time_t curtime;

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

  fRawCat = gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
    //    fRawCat = gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
      HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
      fRawCat=tof->buildMatrixCategory("HTofRawSim",0.5F);

   if (!fRawCat) 
       return kFALSE;
    else
       gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  }

  iter = (HIterator *)fGeantCat->MakeIterator();
  time(&curtime);
  gRandom->SetSeed(curtime);
  
  return kTRUE;
}


Int_t HTofDigitizer::execute(void) {

  HGeantTof* geant = 0;
  HTofRawSim* raw = 0;
  HTofDigitParCell *cell = 0;

  Float_t geaTof;
  Float_t geaEner;
  Float_t geaX;
  Float_t geaY;  // used by Tofino only
  Int_t   numTrack; //number of current track
  Float_t timeL;
  Float_t timeR;
  Float_t chargeL;
  Float_t chargeR;

  Int_t timeCh, chargeCh;

  Float_t hl, vg, al, ar, trz, eq, yp;
  Float_t prevTimeL, prevTimeR, prevChargeL, prevChargeR;

  Float_t timeResol;
  Float_t relAmplResol = 0.1;
  Float_t amplResol;
  Float_t deg2rad = 0.017453293;
  Float_t chargeRef;
  Float_t minEnerRelease = 1.8;  // units ?
  Float_t channelTDC = 0.05;     // ns/ch

  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)

  while ((geant=(HGeantTof *)iter->Next())!=0) {

    fLoc[1] = geant->getModule();
    if (fLoc[1] > 21) continue;   // this is a Tofino, skip it!
    fLoc[1] = 21 - fLoc[1];       // Tof modules in HGeant: (0->21) = (in->out)

    fLoc[0] = geant->getSector();
    fLoc[2] = geant->getCell();
    fLoc[2] = 7 - fLoc[2];        // reverse also order of rods in Tof module

    raw = (HTofRawSim*) fRawCat->getObject(fLoc);   // test if cell already in use
    if(raw) {
       raw->incNHit();  // increment counter for additional hits
       prevTimeL = raw->getLeftTime();
       prevTimeR = raw->getRightTime();
       prevChargeL = raw->getLeftCharge();
       prevChargeR = raw->getRightCharge();
    }
    else {
       prevTimeL = prevTimeR = 100000.;
       prevChargeL = prevChargeR = 0.;
       raw = (HTofRawSim*) fRawCat->getSlot(fLoc);  // get new slot
       if(!raw) continue;
       raw = new(raw) HTofRawSim;
       raw->setNHit(1);
    }

    Short_t nHits = raw->getNHit();  // number of hits in this cell up till now

    cell = fDigitPar->getCell(fLoc);

    hl = cell->getHalfLen();
    vg = cell->getVelGroup();
    al = cell->getAttenLen();
    ar = cell->getAngleRef();
    trz = cell->getTimeResZero();
    eq = cell->getEffiQuant();
    yp = cell->getYieldPhot();

    geant->getHit(geaEner,geaX,geaY,geaTof);
    numTrack=geant->getTrack(); 


    if(vg!=0.0 && al!=0.0) {

	timeL = geaTof + (hl - geaX)/vg;
	timeResol = trz * exp((hl - geaX)/(2.*al));
	timeL = gRandom->Gaus(timeL,timeResol);
	timeR = geaTof + (hl + geaX)/vg;
	timeResol = trz * exp((hl + geaX)/(2.*al));
	timeR = gRandom->Gaus(timeR,timeResol);
	
        timeCh = (Int_t) (timeL/channelTDC);
        if (timeCh < 0) timeL = 0.;
	if (timeCh > 4095) timeL = 4095.;
	timeL = ( Float_t ) (timeCh);
        if(timeL > prevTimeL) timeL = prevTimeL;

        timeCh = (Int_t) (timeR/channelTDC);
        if (timeCh < 0) timeR = 0.;
	if (timeCh > 4095) timeR = 4095.;
        timeR = ( Float_t ) (timeCh);
        if(timeR > prevTimeR) timeR = prevTimeR;

	chargeL = geaEner*yp*eq*0.5*(1 - cos(ar*deg2rad))*exp(-(hl-geaX)/al);
	amplResol = chargeL*relAmplResol;
	chargeL = gRandom->Gaus(chargeL,amplResol);
	chargeR = geaEner*yp*eq*0.5*(1 - cos(ar*deg2rad))*exp(-(hl+geaX)/al);
	amplResol = chargeR*relAmplResol;
	chargeR = gRandom->Gaus(chargeR,amplResol);
	
	chargeRef = yp*eq*0.5*(1-cos(ar*deg2rad))*minEnerRelease*exp(-hl/al);
	if(fLoc[1] <= 3) chargeRef *= 3.;
	if(fLoc[1]>3 && fLoc[1]<=7) chargeRef *= 2.;

	chargeCh = (Int_t) ((chargeL/chargeRef)*1024. + prevChargeL);
        if (chargeCh < 0) chargeCh = 0;
	if (chargeCh > 4095)  chargeCh = 4095;
	chargeL = (Float_t)chargeCh;

	chargeCh = (Int_t) ((chargeR/chargeRef)*1024. + prevChargeR);
        if (chargeCh < 0) chargeCh = 0;
	if (chargeCh > 4095)  chargeCh = 4095;
	chargeR = (Float_t)chargeCh;

    } else {

	timeL = 4095.;
	timeR = 4095.;
	chargeL = 4095.;
	chargeR = 4095.;
    }

    raw->setLeftTime(timeL);
    raw->setRightTime(timeR);
    raw->setLeftCharge(chargeL);
    raw->setRightCharge(chargeR);
    raw->setSector((Char_t) fLoc[0]);
    raw->setModule((Char_t) fLoc[1]);
    raw->setCell((Char_t) fLoc[2]);
    raw->setNTrack(numTrack); //set number of current track to raw
  }
  return 0;

}

ClassImp(HTofDigitizer)







