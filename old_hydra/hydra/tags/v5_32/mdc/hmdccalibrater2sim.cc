//*-- AUTHOR : Ilse Koenig
//*-- Modified : 18/06/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCalibrater2Sim
//
// This transformation class is used for simulated data and is derived from
// the transformation class HMdcCalibrater2 which calculates the distances to
// the wires from the Cal1 drift times.
// The additional information for simulated data (track numbers, cell status)
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
#include <iostream.h>
ClassImp(HMdcCalibrater2Sim)

HMdcCalibrater2Sim::~HMdcCalibrater2Sim(void) {
  // destructor deletes the iterator
  if (iter) delete iter;
  iter=0;
}

Bool_t HMdcCalibrater2Sim::init(void) {
  // creates the MdcCal1Sim input and MdcCal2Sim output matrix categories if
  // they do not exist and adds them to the current event;
  // checks if already existing categories are suitable for simulated data;
  // creates an iterator which loops over all fired cells;
    setParContainers();
    cal1Cat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!cal1Cat) {
    HMdcDetector* mdc=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
    cal1Cat=mdc->buildMatrixCategory("HMdcCal1Sim",0.5F);
    if (!cal1Cat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal1,cal1Cat,"Mdc");
  } else {
    if (cal1Cat->getClass()!=HMdcCal1Sim::Class()) {
      Error("HMdcCalibrater2Sim::init()","Misconfigurated input category");
      return kFALSE;
    }
  }

  cal2Cat=gHades->getCurrentEvent()->getCategory(catMdcCal2);
  if (!cal2Cat) {
    HMdcDetector* mdc=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
    cal2Cat=mdc->buildMatrixCategory("HMdcCal2Sim",0.5F);
    if (!cal2Cat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal2,cal2Cat,"Mdc");
  } else {
    if (cal2Cat->getClass()!=HMdcCal2Sim::Class()) {
      Error("HMdcCalibrater2Sim::init()","Misconfigurated output category");
      return kFALSE;
    }
  }

  iter=(HIterator *)cal1Cat->MakeIterator("native");
  loc.set(4,0,0,0,0);
  fActive=kTRUE;
  return kTRUE;
}


void HMdcCalibrater2Sim::getNewObject(HMdcCal2* p) {
  // puts pointer p to a new object of type HMdcCal2Sim
  new((HMdcCal2Sim*)p) HMdcCal2Sim;
}

Int_t HMdcCalibrater2Sim::execute(void) {
  // loops over all fired cells, gets the corresponding slot in the output
  // categorie HMdcCal2Sim and calls the function calibrate(...)
  HMdcCal1* cal1=0;
  HMdcCal2* cal2=0;
  iter->Reset();
  while ((cal1=(HMdcCal1 *)iter->Next())!=0) {
    cal1->getAddress(loc[0],loc[1],loc[2],loc[3]);
    cal2=(HMdcCal2 *)cal2Cat->getSlot(loc);
    if (cal2) {
      getNewObject(cal2);
      calibrate(cal1,cal2);
    }
  }
  return 0;


}
void HMdcCalibrater2Sim::calibrate(HMdcCal1* cal1,HMdcCal2* cal2) {
  // copies the track numbers and the status number from input to output
    cal2->setAddress(loc[0],loc[1],loc[2],loc[3]);
    Int_t nHits=cal1->getNHits();
    cal2->setNHits(nHits);

    // for the moment only constant driftvelocity is used

    cal2->setDist1(42.*.001*(cal1->getTime1()),0.F);
    if (nHits==-2 || nHits==+2) cal2->setDist2(42.*.001*(cal1->getTime2()),0.F);
    ((HMdcCal2Sim*)cal2)->setStatus(((HMdcCal1Sim*)cal1)->getStatus());
    ((HMdcCal2Sim*)cal2)->setNTrack1(((HMdcCal1Sim*)cal1)->getNTrack1());
    if (cal1->getNHits()>1)
	((HMdcCal2Sim*)cal2)->setNTrack2(((HMdcCal1Sim*)cal1)->getNTrack2());
    //HMdcCalibrater2::calibrate(cal1,cal2);
}

