//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCalibrater2
//
// This transformation class should calculate the distances to the wires from
// the Cal1 drift times.
// The calibrate function is at the moment only a dummy function and the
// parameter container needed does not exist.
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdccalibrater2.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdccal1.h"
#include "hmdccal2.h"
#include "hmdccal2par.h"
#include <iostream.h>
#include <math.h>
#include <stdlib.h>

ClassImp(HMdcCalibrater2)

HMdcCalibrater2::HMdcCalibrater2(void) {
  // default constructor
  cal1Cat=0;
  cal2Cat=0;
  iter=0;
  cal2par=0;

 
}


HMdcCalibrater2::HMdcCalibrater2(Text_t* name,Text_t* title)
                 : HReconstructor(name,title)
  // constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments
{
  cal1Cat=0;
  cal2Cat=0;
  iter=0;
  cal2par=0;

  
}


HMdcCalibrater2::~HMdcCalibrater2(void) {
  // destructor deletes the iterator
  if (iter) delete iter;
  iter=0;
}


void HMdcCalibrater2::setParContainers() {
  // creates the parameter containers MdcCal2Par if it does not
  // exist and adds them to the list of parameter containers in the runtime
  // database

    cal2par=(HMdcCal2Par*)gHades->getRuntimeDb()->getContainer("MdcCal2Par");
}




Bool_t HMdcCalibrater2::init(void) {
  // creates the MdcCal1 input and MdcCal2 output categories if they do not
  // exist and adds them to the current event
  // creates an iterator which loops over all fired cells

    setParContainers();
    cal1Cat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!cal1Cat) {
    cal1Cat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal1);
    if (!cal1Cat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal1,cal1Cat,"Mdc");
  }

  cal2Cat=gHades->getCurrentEvent()->getCategory(catMdcCal2);
  if (!cal2Cat) {
    cal2Cat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal2);
    if (!cal2Cat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal2,cal2Cat,"Mdc");
  }

  iter=(HIterator *)cal1Cat->MakeIterator("native");
  loc.set(4,0,0,0,0);
  fActive=kTRUE;
  return kTRUE;
}


Int_t HMdcCalibrater2::execute(void) {
  // loops over all fired cells, gets the corresponding slot in the output
  // categorie HMdcCal2 and calls the function calibrate(...)
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


void HMdcCalibrater2::getNewObject(HMdcCal2* p) {
  // puts pointer p to a new object of type HMdcCal2
  p=new(p) HMdcCal2;
}

void HMdcCalibrater2::calibrate(HMdcCal1* cal1,HMdcCal2* cal2) {
    //
    //
    cal2->setAddress(loc[0],loc[1],loc[2],loc[3]);
    Int_t nHits=cal1->getNHits();
    Double_t dummy=85;
    Double_t dist   =-99;
    Double_t disterr=-99;

    cal2->setNHits(nHits);

    cal2par->calcDistance(loc[0],loc[1],dummy,cal1->getTime1(),&dist,&disterr);
    cal2->setDist1((Float_t)dist,(Float_t)disterr);

    if (nHits==-2)
    {   // two times leading edge
	cal2par->calcDistance(loc[0],loc[1],dummy,cal1->getTime2(),&dist,&disterr);
	cal2->setDist2((Float_t)dist,(Float_t)disterr);
    }
    else if(nHits==2)
    {   // leading and trailing edge
        cal2->setDist2((Float_t)dist,(Float_t)disterr);
    }
}




