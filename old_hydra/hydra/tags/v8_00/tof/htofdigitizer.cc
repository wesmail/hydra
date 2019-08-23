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
//*-- Modified: 05/06/2006 P. Tlusty
//*-- Modified: 27/10/2002 D.Zovinec
//*-- Modified: 13/02/2002 by D.Zovinec
//*-- Modified: 30/11/2000 by R.Holzmann
//*-- Modified: 16/12/99 by J.Bielcik
//*-- Modified: 9/12/99 by V.Charriere
//*-- Modified: 8/11/99 by R.Holzmann
//*-- Modified: 24/10/99 by D.Vasiliev

//_HADES_CLASS_DESCRIPTION 
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
  const Int_t embeddingmode=gHades->getEmbeddingMode(); // flag embedding 1=realistic 2:keep geant

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
  Float_t geaEner = 0.;
  Float_t geaX = 0.;
  Float_t geaY = 0.;     // used by Tofino only
  Float_t geaMom = 0.;



  //-------------------------------------------------
  // loop over raw sim category and set the default values
  // for embedding mode the real data
  // have -1 as track id -> set to standard
  // track number for embedding and increment the the
  // hit counter
  if(embeddingmode>0){
      iterTofRaw->Reset();
      while ((raw=(HTofRawSim *)iterTofRaw ->Next())!=0) {
          raw->setNHit(1); // increment counter for additional hits
	  raw->setLeftNTrack(gHades->getEmbeddingRealTrackId());
	  raw->setRightNTrack(gHades->getEmbeddingRealTrackId());
      }
  }
  //-------------------------------------------------



  iterGeant->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)

  //-------------------------------------------------
  // loop over geant category
  while ((geant=(HGeantTof *)iterGeant->Next())!=0) {

    fLoc[1] = geant->getModule();
    if (fLoc[1] > 21) continue;   // this is a Tofino, skip it!
    fLoc[1] = 21 - fLoc[1];       // Tof modules in HGeant: (0->21) = (in->out)

    fLoc[0] = geant->getSector();
    fLoc[2] = geant->getCell();
    fLoc[2] = 7 - fLoc[2];        // reverse also order of rods in Tof module


    //-------------------------------------------------
    // get TofRawSim object
    // if the same adress object exists already sorting has
    // to be done to find the earliest hit
    raw = (HTofRawSim*) fRawCat->getObject(fLoc);   // test if cell in use
    if(raw) {
       // if cell already exists
       raw->incNHit();  // increment counter for additional hits
       numTrack1 = raw->getLeftNTrack();
       numTrack2 = raw->getRightNTrack();

       prevTimeL = raw->getLeftTime();
       prevTimeR = raw->getRightTime();
       prevChargeL = raw->getLeftCharge();
       prevChargeR = raw->getRightCharge();
    }
    else {
       prevTimeL = prevTimeR = 100000.;
       prevChargeL = prevChargeR = 0.;
       raw = (HTofRawSim*) fRawCat->getNewSlot(fLoc);  // get new slot
       if(!raw) {
           Error("execute()","Could not retrieve new Slot in Category catTofRaw!");
	   continue;
       }
       raw = new(raw) HTofRawSim;
       raw->setNHit(1);
    }
    //-------------------------------------------------

    //-------------------------------------------------
    // get simulation paramters for the cell
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
    //-------------------------------------------------

    //-------------------------------------------------
    // get GEANT values of the hit
    geant->getHit(geaEner,geaX,geaY,geaTof,geaMom,trackLen);
    numTrack=geant->getTrack();

    if(raw->getNHit()==1){
	// for a new hit left and right
	// track number have to be set
	numTrack2=numTrack;
	numTrack1=numTrack;
    }
    //-------------------------------------------------

    if((vg!=0.0) && (al!=0.0))  {

	//-------------------------------------------------
	// calculation of left / right times
	timeL = geaTof + (hl - geaX)/vg;
	timeR = geaTof + (hl + geaX)/vg;

        timeCh = (Int_t) (timeL/slopeTDCl);
        if (timeCh < 0) timeCh = 0;
	if (timeCh > 4095) timeCh = 4095;
	timeL = ( Float_t ) (timeCh) + 0.5;

        timeCh = (Int_t) (timeR/slopeTDCr);
        if (timeCh < 0) timeCh = 0;
	if (timeCh > 4095) timeCh = 4095;
        timeR = ( Float_t ) (timeCh) + 0.5;
	//-------------------------------------------------

	//-------------------------------------------------
	// comparing the measured left/right tofs to the previous
	// existing times (=100000 if there was no hit before)
        // The shorter times wins.
	//-------------------------------------------------

	if(raw->getNHit()>1){
	    // for a double hit the track numbers and measured
	    // times have to be sorted. real hits should not be
	    // taken into account in embeddingmode=2.

	    //-------------------------------------------------
	    // for embeddingmode=2 the GEANT hits should be kept anyway
	    if(embeddingmode==2 ){

		if(numTrack1==gHades->getEmbeddingRealTrackId()) {
		    // keep GEANT hit -> ignore real hits in sorting
                    numTrack1=numTrack;
		}

		if(numTrack2==gHades->getEmbeddingRealTrackId()) {
		    // keep GEANT hit -> ignore real hits in sorting
                    numTrack2=numTrack;
		}
	    }
 	    //-------------------------------------------------

	    if(timeL < prevTimeL){
	      // sort track number by left time
		numTrack1=numTrack;
	    }

	    if(timeR < prevTimeR){
		// sort track number by right time
		numTrack2=numTrack;
	    }
	}
	// if times are larger than prevTimes keep
        // the previous times
	if(timeL > prevTimeL) {timeL = prevTimeL;}
	if(timeR > prevTimeR) {timeR = prevTimeR;}
	//-------------------------------------------------



	//-------------------------------------------------
	// calculation of charges
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
 	//-------------------------------------------------

    } else {

	//-------------------------------------------------
	// setting default values for times and charges
	timeL = 4095.;
	timeR = 4095.;
	chargeL = 4095.;
	chargeR = 4095.;
	//-------------------------------------------------
    }

    //-------------------------------------------------
    // filling the hit object
    raw->setLeftTime(timeL);
    raw->setRightTime(timeR);
    raw->setLeftCharge(chargeL);
    raw->setRightCharge(chargeR);
    raw->setSector((Char_t) fLoc[0]);
    raw->setModule((Char_t) fLoc[1]);
    raw->setCell((Char_t) fLoc[2]);
    raw->setLeftNTrack(numTrack1);
    raw->setRightNTrack(numTrack2);
    //-------------------------------------------------

  } // end loop over geant category

  //-------------------------------------------------
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

    HTofDigiParCell& cell=(*fDigiPar)[fLoc[0]][fLoc[1]][fLoc[2]];
    hl = cell.getHalfLen();
    al = cell.getAttenLen();
    thrCFDl = cell.getLeftCFDThreshold();
    thrCFDr = cell.getRightCFDThreshold();
    thrADCl = cell.getLeftADCThreshold();
    thrADCr = cell.getRightADCThreshold();
    slopeTDCl = cell.getLeftTDCSlope();
    slopeTDCr = cell.getRightTDCSlope();

    timeResol = timeResZero*exp(hl/(2.*al))/slopeTDCl;
    raw->setLeftTime(gRandom->Gaus(timeL,timeResol));
    timeResol = timeResZero*exp(hl/(2.*al))/slopeTDCr;
    raw->setRightTime(gRandom->Gaus(timeR,timeResol));

    if(((Int_t)timeL)<0) raw->setLeftTime(0.0);
    if(((Int_t)timeR)<0) raw->setRightTime(0.0);
    // overflow suppression
    if(((Int_t)timeL)>=4095) raw->setLeftTime(0.0);
    if(((Int_t)timeR)>=4095) raw->setRightTime(0.0);


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
