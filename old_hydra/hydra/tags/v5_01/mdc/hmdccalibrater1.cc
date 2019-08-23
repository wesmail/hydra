//*-- AUTHOR : Ilse Koenig
//*-- Modified : 24/08/99 by R. Holzmann
//*-- Modified : 29/07/99 by Ilse Koenig
//*-- Modified : 17/02/2000 by J. Kempter

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCalibrater1
//
// This transformation class calculates the drift time information in all fired
// cells using parameters from the parameter containers MdcCalParRaw for the
// calibration parameters and from MdcLookupGeom to map the layer number and
// cell number on Cal1 level to the motherboard number and TDC channel number
// on Raw level.
// The drift times are randomized within one bin of TDC resolution.
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdccalibrater1.h"
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
#include "hmdcraw.h"
#include "hmdccal1.h"
#include "hmdccalparraw.h"
#include "hmdclookupgeom.h"
#include <iostream.h>

ClassImp(HMdcCalibrater1)

HMdcCalibrater1::HMdcCalibrater1(void) {
  // Default constructor calls the function setParContainers().
  rawCat=0;
  calCat=0;
  iter=0;
  calparraw=0;
  lookup=0;
}

HMdcCalibrater1::HMdcCalibrater1(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments. It also calls the function setParContainers().
  rawCat=0;
  calCat=0;
  iter=0;
  calparraw=0;
  lookup=0;
}


HMdcCalibrater1::~HMdcCalibrater1(void) {
  // destructor deletes the iterator
  if (iter) delete iter;
  iter=0;
}


void HMdcCalibrater1::setParContainers() {
  // creates the parameter containers MdcCalParRaw and MdcLookupGeom if they do not
  // exist and adds them to the list of parameter containers in the rumtime
  // database
  calparraw=(HMdcCalParRaw*)gHades->getRuntimeDb()
                                  ->getContainer("MdcCalParRaw");
  if (!calparraw) {
    calparraw=new HMdcCalParRaw;
    gHades->getRuntimeDb()->addContainer(calparraw);
  }
  lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()
                                ->getContainer("MdcLookupGeom");
  if (!lookup) {
    lookup=new HMdcLookupGeom;
    gHades->getRuntimeDb()->addContainer(lookup);
  }
}


Bool_t HMdcCalibrater1::init(void) {
  // creates the MdcRaw  and MdcCal1 categories and adds them to the current
  // event
  // creates an iterator which loops over all fired cells
  // calls the function setParContainers()
  setParContainers();
  rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
  }
  calCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal1);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal1,calCat,"Mdc");
  }
  iter=(HIterator *)rawCat->MakeIterator("native");
  loc.set(4,0,0,0,0);
  fActive=kTRUE;
  return kTRUE;
}


Int_t HMdcCalibrater1::execute(void) {
  // calibrates all fired cells
  HMdcRaw* raw=0;
  HMdcCal1* cal=0;
  Int_t mbo, ch;
  iter->Reset();
  while ((raw=(HMdcRaw *)iter->Next())!=0) {
    raw->getAddress(loc[0],loc[1],mbo,ch);
    //printf("s=%2i  m=%2i  mbo=%2i  ch=%2i \n",loc[0],loc[1],mbo,ch);
    if (loc[0]<0 || loc[1]<0) continue;
    HMdcLookupChan& chan=(*lookup)[loc[0]][loc[1]][mbo][ch];
    loc[2]=chan.getNLayer();
    loc[3]=chan.getNCell();
    if (loc[3]>=0) {
      cal=(HMdcCal1 *)calCat->getSlot(loc);
      if (cal) {
        cal=new(cal) HMdcCal1;
        const Int_t nHits=raw->getNHits();
        cal->setNHits(nHits);
        cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
        HMdcCalParTdc& tdc=(*calparraw)[loc[0]][loc[1]][mbo][ch];
        if (tdc.getSlope() && abs(raw->getNHits())<=2){
          cal->setTime1(tdc.getOffset() -
                         ((Float_t)(raw->getTime(1)) + gRandom->Rndm() - 0.5F)
                            * tdc.getSlope());
          if (nHits==-2 || nHits==+2){
             cal->setTime2(tdc.getOffset() -
                            ((Float_t)(raw->getTime(2)) + gRandom->Rndm() - 0.5F)
                               * tdc.getSlope());
          }
        }
      }
    } else {
      printf("lookup s=%i m=%i mbo=%2i chan=%2i not connected\n",
             loc[0],loc[1],mbo,ch);
    }
  }
  return 0;
}
