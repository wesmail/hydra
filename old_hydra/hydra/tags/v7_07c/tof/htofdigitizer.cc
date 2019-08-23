using namespace std;
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
#include "htofrawsim.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "htofdigipar.h"
#include "htofrawsimfilter.h"
#include <iostream> 
#include <iomanip>

//*-- Author : D.Vasiliev
//*-- Modified: 27/10/2002 D.Zovinec
//*-- Modified: 13/02/2002 by D.Zovinec
//*-- Modified: 30/11/2000 by R.Holzmann
//*-- Modified: 16/12/99 by J.Bielcik
//*-- Modified: 9/12/99 by V.Charriere
//*-- Modified: 8/11/99 by R.Holzmann
//*-- Modified: 24/10/99 by D.Vasiliev

/////////////////////////////////////////////////////////////////////
//
//  HTofDigitizer digitizes data, puts output values into
//  raw data category
//
//  Need to implement quenching?
//
/////////////////////////////////////////////////////////////////////

void HTofDigitizer::initParContainer() {

 fDigiPar=(HTofDigiPar *)gHades->getRuntimeDb()->getContainer("TofDigiPar");
}

HTofDigitizer::HTofDigitizer(void) {

  fGeantCat=0;
  fRawCat=0;
  fDigiPar=0;
  fLoc.set(3,0,0,0);
  iterGeant=0;
  iterTofRaw=0;
}

HTofDigitizer::HTofDigitizer(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {

  fGeantCat=0;
  fRawCat=0;
  fDigiPar=0;
  fLoc.set(3,0,0,0);
  iterGeant=0;
  iterTofRaw=0;
}

HTofDigitizer::~HTofDigitizer(void) {
}

Bool_t HTofDigitizer::init(void) {

  time_t curtime;
  initParContainer();
  fGeantCat = gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
  if (!fGeantCat) {
    fGeantCat = gHades->getSetup()->getDetector("Tof")->buildCategory(catTofGeantRaw);
    if (!fGeantCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofGeantRaw,fGeantCat,"Tof");
  }

  fRawCat = gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
     HTofDetector* tof=(HTofDetector*)(gHades->getSetup()->getDetector("Tof"));
     fRawCat=tof->buildMatrixCategory("HTofRawSim",0.5F);
     if (!fRawCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  }

  iterGeant = (HIterator *)fGeantCat->MakeIterator("native");
  iterTofRaw = (HIterator *)fRawCat->MakeIterator("native");

  time(&curtime);

  return kTRUE;
}

Int_t HTofDigitizer::execute(void) {

  const Float_t deg2rad = 0.017453293; // deg to rad conversion.
  const Float_t quantEff = 0.24;       // PMT quantum efficiency.
  const Float_t photYield = 11100.0;   // photon yield in scintillator (phot/MeV).
  const Float_t timeResZero = 0.18;     // time resolution in the centre of the strip [ns].
  const Float_t relAmplResol = 0.08;   // sigma of Gaus distribution.
  const Float_t minEnerRelease = 1.8;  // minimum energy release (MeV/cm)

  HGeantTof* geant = 0;
  HTofRawSim* raw = 0;
  HTofRawSimFilter fRawFilter;

  Float_t hl, al, ar, vg, slopeTDCl, slopeTDCr;
  Int_t thrCFDl, thrCFDr, thrADCl, thrADCr;

  Int_t   numTrack, numTrack1 = -1, numTrack2 = -1;      //track numbers
  Float_t trackLen;
  Float_t timeL, timeR, chargeL, chargeR;
  Int_t timeCh, chargeCh;
  Float_t prevTimeL, prevTimeR, prevChargeL, prevChargeR;
  Float_t timeResol, amplResol, chargeRef;

  Float_t geaTof = 0.;
  Float_t geaTof1[6][8][8];
  Float_t geaTof2[6][8][8];
  Float_t geaEner = 0.;
  Float_t geaX = 0.;
  Float_t geaY = 0.;     // used by Tofino only
  Float_t geaMom = 0.;

  iterGeant->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)

  while ((geant=(HGeantTof *)iterGeant->Next())!=0) {

    fLoc[1] = geant->getModule();
    if (fLoc[1] > 21) continue;   // this is a Tofino, skip it!
    fLoc[1] = 21 - fLoc[1];       // Tof modules in HGeant: (0->21) = (in->out)

    fLoc[0] = geant->getSector();
    fLoc[2] = geant->getCell();
    fLoc[2] = 7 - fLoc[2];        // reverse also order of rods in Tof module


    raw = (HTofRawSim*) fRawCat->getObject(fLoc);   // test if cell in use
    if(raw) {
       raw->incNHit();  // increment counter for additional hits
       numTrack1 = raw->getNTrack1();
       numTrack2 = raw->getNTrack2();
       prevTimeL = raw->getLeftTime();
       prevTimeR = raw->getRightTime();
       prevChargeL = raw->getLeftCharge();
       prevChargeR = raw->getRightCharge();
    }
    else {
       prevTimeL = prevTimeR = 100000.;
       prevChargeL = prevChargeR = 0.;
       raw = (HTofRawSim*) fRawCat->getNewSlot(fLoc);  // get new slot
       if(!raw) continue;
       raw = new(raw) HTofRawSim;
       raw->setNHit(1);
    }

    HTofDigiParCell& cell=(*fDigiPar)[fLoc[0]][fLoc[1]][fLoc[2]];
    hl = cell.getHalfLen();
    al = cell.getAttenLen();
    ar = cell.getAngleRef();
    vg = cell.getGroupVel();
    slopeTDCl = cell.getLeftTDCSlope();
    slopeTDCr = cell.getRightTDCSlope();
    thrCFDl = cell.getLeftCFDThreshold();
    thrCFDr = cell.getRightCFDThreshold();
    thrADCl = cell.getLeftADCThreshold();
    thrADCr = cell.getRightADCThreshold();

    geant->getHit(geaEner,geaX,geaY,geaTof,geaMom,trackLen);

    numTrack=geant->getTrack();

    if((vg!=0.0) && (al!=0.0))  {
	timeL = geaTof + (hl - geaX)/vg;
	timeResol = timeResZero*exp((hl - geaX)/(2.*al));
	timeL = gRandom->Gaus(timeL,timeResol);
	timeR = geaTof + (hl + geaX)/vg;
	timeResol = timeResZero*exp((hl + geaX)/(2.*al));
	timeR = gRandom->Gaus(timeR,timeResol);

        timeCh = (Int_t) (timeL/slopeTDCl);
        if (timeCh < 0) timeCh = 0;
	if (timeCh > 4095) timeCh = 4095;
	timeL = ( Float_t ) (timeCh);

        timeCh = (Int_t) (timeR/slopeTDCr);
        if (timeCh < 0) timeCh = 0;
	if (timeCh > 4095) timeCh = 4095;
        timeR = ( Float_t ) (timeCh);

        if(raw->getNHit()>1){
          if(geaTof < geaTof1[fLoc[0]][fLoc[1]][fLoc[2]]){
            numTrack2=numTrack1;
            numTrack1=numTrack;
            geaTof2[fLoc[0]][fLoc[1]][fLoc[2]]=geaTof1[fLoc[0]][fLoc[1]][fLoc[2]];
            geaTof1[fLoc[0]][fLoc[1]][fLoc[2]]=geaTof;
          } else {
            if(geaTof < geaTof2[fLoc[0]][fLoc[1]][fLoc[2]]){
              numTrack2=numTrack;
              geaTof2[fLoc[0]][fLoc[1]][fLoc[2]]=geaTof;
            }
          }
        }

        if(timeL > prevTimeL) timeL = prevTimeL;
        if(timeR > prevTimeR) timeR = prevTimeR;

	chargeL = geaEner*photYield*quantEff*0.5*(1 - cos(ar*deg2rad))*exp(-(hl-geaX)/al);
	amplResol = chargeL*relAmplResol;
	chargeL = gRandom->Gaus(chargeL,amplResol);
	chargeR = geaEner*photYield*quantEff*0.5*(1 - cos(ar*deg2rad))*exp(-(hl+geaX)/al);
	amplResol = chargeR*relAmplResol;
	chargeR = gRandom->Gaus(chargeR,amplResol);

	chargeRef = photYield*quantEff*0.5*(1-cos(ar*deg2rad))*minEnerRelease*exp(-hl/al);
	if(fLoc[1] <= 3) chargeRef *= 3.;
	if(fLoc[1]>3 && fLoc[1]<=7) chargeRef *= 2.;

	chargeCh = (Int_t) ((chargeL/chargeRef)*256. + prevChargeL);
        if (chargeCh < 0) chargeCh = 0;
	if (chargeCh > 4095)  chargeCh = 4095;
	chargeL = (Float_t)chargeCh;

	chargeCh = (Int_t) ((chargeR/chargeRef)*256. + prevChargeR);
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
    if(raw->getNHit()>1){
      raw->setNTrack1(numTrack1);
      raw->setNTrack2(numTrack2);
    } else {
      raw->setNTrack1(numTrack);
      raw->setNTrack2(-1);
      geaTof1[fLoc[0]][fLoc[1]][fLoc[2]]=geaTof;
      geaTof2[fLoc[0]][fLoc[1]][fLoc[2]]=100000.;
    }
  }

  // Exclusion of hits with charge less than ADC threshold.
  // Time of hits with charge less than CFD threshold is set
  // to zero. These hits are excluded later in the hitfinder.

  iterTofRaw->Reset();
  while ( (raw=(HTofRawSim *)iterTofRaw->Next())!=NULL) {
    fLoc[0] = raw->getSector();
    fLoc[1] = raw->getModule();
    fLoc[2] = raw->getCell();
    timeL=raw->getLeftTime();
    timeR=raw->getRightTime();
    chargeL=raw->getLeftCharge();
    chargeR=raw->getRightCharge();

    // overflow suppression
    if(((Int_t)timeL)>=4095) raw->setLeftTime(0.0);
    if(((Int_t)timeR)>=4095) raw->setRightTime(0.0);

    HTofDigiParCell& cell=(*fDigiPar)[fLoc[0]][fLoc[1]][fLoc[2]];
    thrCFDl = cell.getLeftCFDThreshold();
    thrCFDr = cell.getRightCFDThreshold();
    thrADCl = cell.getLeftADCThreshold();
    thrADCr = cell.getRightADCThreshold();

    // CFD and ADC thresholds
    if(((Int_t)chargeL)<thrCFDl){
      raw->setLeftTime(0.0);
      if(((Int_t)chargeL)<thrADCl){
        raw->setLeftCharge(0.0);
      }

    }
    if(((Int_t) chargeR)<thrCFDr){
      raw->setRightTime(0.0);
      if(((Int_t) chargeR)<thrADCr){
        raw->setRightCharge(0.0);
      }
    }
  }

  fRawCat->filter(fRawFilter);

  return 0;
}

ClassImp(HTofDigitizer)
