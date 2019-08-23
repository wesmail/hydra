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


void HMdcCalibrater2Sim::calibrate(HMdcCal1* cal1,HMdcCal2* cal2) {
  // copies the track numbers and the status number from input to output
  HMdcCalibrater2::calibrate(cal1,cal2);
  ((HMdcCal2Sim*)cal2)->setStatus(((HMdcCal1Sim*)cal1)->getStatus());
  ((HMdcCal2Sim*)cal2)->setNTrack1(((HMdcCal1Sim*)cal1)->getNTrack1());
  if (cal1->getNHits()>1)
    ((HMdcCal2Sim*)cal2)->setNTrack2(((HMdcCal1Sim*)cal1)->getNTrack2());
}

