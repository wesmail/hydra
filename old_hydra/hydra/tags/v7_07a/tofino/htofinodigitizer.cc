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
#include "htofinocalsimfilter.h"

//*-- Author : D.Vasiliev
//*-- Modified: 05/04/2003 by J.Otwinowski 
// offset subtraction added
//*-- Modified: 01/03/2003 by J.Otwinowski && D.Zovinec
// ADC and TDC parts were modified to get ADC and TDC in channels
//
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

ClassImp(HTofinoDigitizer)

HTofinoDigitizer::HTofinoDigitizer(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {

  fGeantCat=NULL;
  fCalCat=NULL;
  pTofinoCalPar = NULL;
  fLoc.set(3,0,0,0);
  iterGeant=NULL; iterCal=NULL;
}

HTofinoDigitizer::HTofinoDigitizer(void) {

  fGeantCat=NULL;
  fCalCat=NULL;
  pTofinoCalPar = NULL;
  fLoc.set(3,0,0,0);
  iterGeant=NULL; iterCal=NULL;
}

HTofinoDigitizer::~HTofinoDigitizer(void) {

	if(iterGeant) delete iterGeant;
	if(iterCal) delete iterCal;
}

Bool_t HTofinoDigitizer::init(void) {

  time_t curtime;
  initParContainer();

  fGeantCat = gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
  if (!fGeantCat) 
  {
 	fGeantCat = gHades->getSetup()->getDetector("Tof")->buildCategory(catTofGeantRaw);

    if (!fGeantCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofGeantRaw,fGeantCat,"Tof");
  }

  fCalCat = gHades->getCurrentEvent()->getCategory(catTofinoCal);
  if (!fCalCat) 
  {
    fCalCat = ((HTofinoDetector*)(gHades->getSetup()->getDetector("Tofino")))
                                ->buildMatrixCategory("HTofinoCalSim");
    if (!fCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofinoCal,fCalCat,"Tofino");
  }

  iterGeant = (HIterator *)fGeantCat->MakeIterator("native");
  iterCal = (HIterator *)fCalCat->MakeIterator("native");

  time(&curtime);
  gRandom->SetSeed(curtime);

  return kTRUE;
}

void HTofinoDigitizer::initParContainer() {

  pTofinoCalPar = (HTofinoCalPar *)gHades->getRuntimeDb()
                                         ->getContainer("TofinoCalPar");
  if(pTofinoCalPar == NULL) 
  {
  	Error("Error in HTofinoDigitizer::initParContainer()","Container TofinoCalPar not initialized");
  }
}

Int_t HTofinoDigitizer::execute(void) 
{
  const Float_t relAmplResol = 0.08;   // sigma of Gaus distribution.
  const Float_t minEnerRelease = 1.8;  // minimum energy release (MeV/cm)
  //const Float_t minEnerRelease = 2.0;  // minimum energy release (MeV/cm)
  const Float_t al = 2400.0;           // attenuation length

  Int_t thrCFD = 0 ; // cfd threshold
  Int_t thrADC = 0; // adc  threshold

  HLocation ffLoc;
  HGeantTof* pGeant = NULL;
  HTofinoCalSim* pCal = NULL;
  HTofinoCalSimFilter fCalFilter;

  Float_t geaTof, geaEner, geaX, geaY, geaMom, trackLen;  // Geant output
  Int_t   numTrack, adcTofinoCh;
  Float_t tdcTofino, tdcTofinoCh, adcTofino, prevTdcTofino, prevAdcTofino;
  Float_t timeTofino, timeResol, amplResol;
  Float_t distPMT, slope;

  geaTof = 0.0;
  geaEner = 0.0;
  geaX = 0.0;
  geaY = 0.0;
  geaMom = 0.0;
  numTrack = 0;
  trackLen = 0.0;
  tdcTofino = 0.0;
  tdcTofinoCh = 0;
  adcTofino = 0;
  adcTofinoCh = 0;
  distPMT = 0;
  slope = 0;

  prevTdcTofino = 100000.;
  prevAdcTofino = 0.;

  ffLoc.set(2,-1,-1);

  iterGeant->Reset();  
  while ( (pGeant=(HGeantTof *)iterGeant->Next()) != NULL) 
  {  // loop over HGeant hits

    fLoc[1] = pGeant->getModule();
    if(fLoc[1]<22) continue; 

    fLoc[0] = pGeant->getSector();
    fLoc[2] = pGeant->getCell();
    fLoc[2] = fLoc[1] - 22; // to start counting from 0

    fLoc[1] = 0;

    pGeant->getHit(geaEner,geaX,geaY,geaTof,geaMom,trackLen);
    numTrack = pGeant->getTrack();

    pCal = (HTofinoCalSim*) fCalCat->getObject(fLoc);
    if (pCal) {   // test if this cell has already seen a hit
       pCal->incNHit();  // increment hit counter
       prevTdcTofino = pCal->getTime();
       prevAdcTofino = pCal->getCharge();
    } else {        // this cell has not yet been hit, ask for a new slot in cat
       prevTdcTofino = 100000.;
       prevAdcTofino = 0.;
       pCal = (HTofinoCalSim*) fCalCat->getSlot(fLoc);  // get new slot
       if(!pCal) continue;
       pCal = new(pCal) HTofinoCalSim();
       pCal->setNHit(1);
    }

    timeTofino = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcLightPropTime(fLoc[0],fLoc[2],geaY);
    timeResol = ((HTofinoCalPar*)pTofinoCalPar)
                        ->calcDeltaLightPropTime(fLoc[0],fLoc[2],geaY);
    
    ffLoc[0] =  fLoc[0] ;
    ffLoc[1] =  fLoc[2] ;

	// calculate distance to the PMT  
    slope=((HTofinoCalPar*)pTofinoCalPar)->getSlope(ffLoc);
    if(slope != 0.0) distPMT=(timeTofino/slope);
    else Error("execute","slope equal zero, cannot calculate distance to PMT");

    // TDC information
    timeTofino = gRandom->Gaus(timeTofino,timeResol); // prop. time
    tdcTofino = timeTofino + geaTof;    // time in ns
    
    tdcTofinoCh = (Int_t) ((HTofinoCalPar*)pTofinoCalPar)->
					timeToChannel(fLoc[0],fLoc[2],tdcTofino);

    if(tdcTofinoCh<0) tdcTofinoCh = 0;
    if(tdcTofinoCh>4095) tdcTofinoCh = 4095;
	tdcTofino = (Float_t) tdcTofinoCh;

    if (tdcTofino > prevTdcTofino) tdcTofino = prevTdcTofino;
	
    // ADC information
    adcTofino = geaEner*exp(-distPMT/al);
    amplResol = adcTofino*relAmplResol;

    adcTofino = gRandom->Gaus(adcTofino,amplResol); 

    adcTofinoCh = (Int_t) ((adcTofino/minEnerRelease)*512. + prevAdcTofino);

    if (adcTofinoCh < 0) adcTofinoCh = 0;
    if (adcTofinoCh > 4095) adcTofinoCh = 4095;
    adcTofino = (Float_t) adcTofinoCh;

    pCal->setTime(tdcTofino);
    pCal->setCharge(adcTofino);
    pCal->setSector(fLoc[0]);
    pCal->setModule(fLoc[1]);
    pCal->setCell(fLoc[2]);
   	pCal->setTrack(numTrack);
   	pCal->setGeaTof(geaTof);
  }

  // filter overflow hits && recalibrate again tdc(ch) --> tdc(ns)
  iterCal->Reset();
  while ((pCal=(HTofinoCalSim *)iterCal->Next())!=0)
  {
    fLoc[0] = pCal->getSector();
    fLoc[1] = pCal->getModule();
    fLoc[2] = pCal->getCell();
    tdcTofino = pCal->getTime();
    adcTofino = pCal->getCharge();

    // TDC overflow suppression
    if(Int_t(tdcTofino) >= 4095) pCal->setTime(0.0);

    ffLoc[0]=fLoc[0];
    ffLoc[1]=fLoc[2];

    // CFD threshold
    thrCFD = (Int_t)((HTofinoCalPar*)pTofinoCalPar)->getCFDThreshold(ffLoc);
    thrADC = (Int_t)((HTofinoCalPar*)pTofinoCalPar)->getADCThreshold(ffLoc);
	
    if(((Int_t)adcTofino)<thrCFD) pCal->setTime(0.0);
    if(((Int_t)adcTofino)<thrADC) pCal->setCharge(0.0);

	// recalibrate tdc (ch) --> tdc (ns)
	tdcTofino = ((HTofinoCalPar*)pTofinoCalPar)->
                  channelToTime(fLoc[0],fLoc[2],pCal->getTime()); // in ns

	tdcTofino -= ((HTofinoCalPar*)pTofinoCalPar)->getOffset(ffLoc);

	if(tdcTofino > 0) pCal->setTime(tdcTofino);
	else   pCal->setTime(0.0);
  }
  fCalCat->filter(fCalFilter);

  return 0;
} // htofinodigitizer

Bool_t HTofinoDigitizer::finalize(void) 
{
return kTRUE;
}

