//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCalibrater2Sim
//
// This transformation class is used for simulated data and is derived from
// the transformation class HMdcCalibrater2 which calculates the distances to
// the wires from the Cal1 drift times.
// The additional information for simulated data (track numbers, cell status,
// tracklist, statuslist)
// is copied in the overloaded calibrate function.
///////////////////////////////////////////////////////////////////////////////

#include "hmdccalibrater2sim.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hlocation.h"
#include "hmdccal1sim.h"
#include "hmdccal2sim.h"
#include "hmdccal2par.h"

#include <iostream.h>

ClassImp(HMdcCalibrater2Sim)

Bool_t HMdcCalibrater2Sim::init(void) {
  // creates the MdcCal1Sim input and MdcCal2Sim output matrix categories if
  // they do not exist and adds them to the current event;
  // checks if already existing categories are suitable for simulated data;
  // creates an iterator which loops over all fired cells;
    setParContainers();
    cal1Cat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1));
    if (!cal1Cat) {
	HMdcDetector* mdc=(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
	cal1Cat=(HCategory*)(((HMdcDetector*)mdc)->buildMatrixCategory("HMdcCal1Sim",0.5F));
	if (!cal1Cat) return kFALSE;
	else ((HEvent*)(gHades->getCurrentEvent()))->addCategory(catMdcCal1,cal1Cat,"Mdc");

    } else {
	if (cal1Cat->getClass()!=HMdcCal1Sim::Class()) {
	    Error("HMdcCalibrater2Sim::init()","Misconfigurated input category");
	    return kFALSE;
	}
    }

    cal2Cat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal2));
    if (!cal2Cat) {
	HMdcDetector* mdc=(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
	cal2Cat=(HCategory*)(((HMdcDetector*)mdc)->buildMatrixCategory("HMdcCal2Sim",0.5F));
	if (!cal2Cat) return kFALSE;
	else ((HEvent*)(gHades->getCurrentEvent()))->addCategory(catMdcCal2,cal2Cat,"Mdc");
    } else {
	if (cal2Cat->getClass()!=HMdcCal2Sim::Class()) {
	    Error("HMdcCalibrater2Sim::init()","Misconfigurated output category");
	    return kFALSE;
	}
    }

    iter=(HIterator *)((HCategory*)cal1Cat)->MakeIterator("native");
    loc.set(4,0,0,0,0);
    fActive=kTRUE;
    return kTRUE;
}
void HMdcCalibrater2Sim::getNewObject(HMdcCal2* p) {
  // puts pointer p to a new object of type HMdcCal2Sim
  new((HMdcCal2Sim*)p) HMdcCal2Sim;
}
void HMdcCalibrater2Sim::calibrate(HMdcCal1* cal1,HMdcCal2* cal2) {
  // copies the track numbers and the status number from input to output
    ((HMdcCal2Sim*)cal2)->setAddress(loc[0],loc[1],loc[2],loc[3]);
    Int_t nHits=((HMdcCal1Sim*)cal1)->getNHits();

    ((HMdcCal2Sim*)cal2)->setNHits(nHits);

    Double_t dummy=85;
    Double_t dist   =-99;
    Double_t disterr=-99;

    cal2par->calcDistance(loc[0],loc[1],dummy,cal1->getTime1(),&dist,&disterr);

    ((HMdcCal2Sim*)cal2)->setDist1((Float_t)dist,(Float_t)disterr);

    ((HMdcCal2Sim*)cal2)->setNTrack1(((HMdcCal1Sim*)cal1)->getNTrack1());
    ((HMdcCal2Sim*)cal2)->setStatus1(((HMdcCal1Sim*)cal1)->getStatus1());
  
    ((HMdcCal2Sim*)cal2)->setTrackList ( ((HMdcCal1Sim*)cal1)->getTrackList()  );
    ((HMdcCal2Sim*)cal2)->setStatusList( ((HMdcCal1Sim*)cal1)->getStatusList() );

    if( nHits<0)// if 2 leading edges hits
    {
        cal2par->calcDistance(loc[0],loc[1],dummy,cal1->getTime2(),&dist,&disterr);
	((HMdcCal2Sim*)cal2)->setDist2((Float_t)dist,(Float_t)disterr);
	((HMdcCal2Sim*)cal2)->setNTrack2(((HMdcCal1Sim*)cal1)->getNTrack2());
        ((HMdcCal2Sim*)cal2)->setStatus2(((HMdcCal1Sim*)cal1)->getStatus2());
    }
    else // leading and trailing edge
    {
       ((HMdcCal2Sim*)cal2)->setDist2((Float_t)dist,(Float_t)disterr);
       ((HMdcCal2Sim*)cal2)->setNTrack2(((HMdcCal1Sim*)cal1)->getNTrack2());
       ((HMdcCal2Sim*)cal2)->setStatus2(((HMdcCal1Sim*)cal1)->getStatus2());
    }
}
