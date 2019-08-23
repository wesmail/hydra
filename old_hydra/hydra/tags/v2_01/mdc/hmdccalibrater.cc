#include "hmdccalibrater.h"
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
#include "hmdccal.h"
#include "hmdccalpar.h"
#include "hmdclookupgeom.h"
#include <iostream.h>

////////////////////////////////
//HMdcCalibrater
//
////////////////////////////////

ClassImp(HMdcCalibrater)

HMdcCalibrater::HMdcCalibrater(void) {
  rawCat=0;
  calCat=0;
  iter=0;
  calpar=0;
  lookup=0;
  setParContainers();
}

HMdcCalibrater::HMdcCalibrater(Text_t* name,Text_t* title) :
  HReconstructor(name,title)
{
  rawCat=0;
  calCat=0;
  iter=0;
  calpar=0;
  lookup=0;
  setParContainers();
}


HMdcCalibrater::~HMdcCalibrater(void) {
  if (calpar) gHades->getRuntimeDb()->removeContainer("MdcCalPar");
  if (lookup) gHades->getRuntimeDb()->removeContainer("MdcLookup");
}

void HMdcCalibrater::setParContainers() {
  calpar=(HMdcCalPar*)gHades->getRuntimeDb()->getContainer("MdcCalPar");
  if (!calpar) {
    calpar=new HMdcCalPar;
    gHades->getRuntimeDb()->addContainer(calpar);
  }
  lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
  if (!lookup) {
    lookup=new HMdcLookupGeom;
    gHades->getRuntimeDb()->addContainer(lookup);
  }
}

Bool_t HMdcCalibrater::init(void) {
  rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
  }
  calCat=gHades->getCurrentEvent()->getCategory(catMdcCal);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal,calCat,"Mdc");
  }
  iter=(HIterator *)rawCat->MakeIterator();
  loc.set(4,0,0,0,0);
  fActive=kTRUE;
  return kTRUE;
}

Int_t HMdcCalibrater::execute(void) {
  HMdcRaw* raw=0;
  HMdcCal* cal=0;
  Int_t mbo, ch;
  iter->Reset();
  while ((raw=(HMdcRaw *)iter->Next())!=0) {
    loc[0]=raw->getSector();
    loc[1]=raw->getModule();
    mbo=raw->getMbo();
    ch=raw->getTdc();
    HMdcLookupChan& chan=(*lookup)[loc[0]][loc[1]][mbo][ch];
    loc[2]=chan.getNLayer();
    loc[3]=chan.getNCell();
    cal=(HMdcCal *)calCat->getSlot(loc);
    if (cal) {
      cal=new(cal) HMdcCal;
      HMdcCalParCell& cell=(*calpar)[loc[0]][loc[1]][loc[2]][loc[3]];
//      if (raw->getNrOfHits()>0) {
        cal->setTime((Float_t)(raw->getTime(1))*(cell.getSlope()) +
                          (cell.getOffset()));
        if (raw->getNrOfHits()>1)
           cal->setTime((Float_t)(raw->getTime(2))*(cell.getSlope()) +
                          (cell.getOffset()));
//      }
      cal->setSector((Short_t)raw->getSector());
      cal->setModule((Short_t)raw->getModule());
      cal->setLayer((Short_t)loc[2]);
      cal->setCell((Short_t)loc[3]);
    }
  }
  return 0;
}

