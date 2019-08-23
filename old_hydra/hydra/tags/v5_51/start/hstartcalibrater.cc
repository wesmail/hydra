//*-- Modified : 18/12/2001 by I.Koenig
//*-- Modified : 01/12/2000 by I.Koenig
#include "hstartcalibrater.h"
#include "hstartdef.h"
#include "hstartraw.h"
#include "hstartcal.h"
#include "hstartcalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include <iostream.h>

////////////////////////////////////////////////////////////////
//
//  HStartCalibrater: perform global correction on the data
//  
////////////////////////////////////////////////////////////////


HStartCalibrater::HStartCalibrater(void) {
  rawCat=0;
  calCat=0;
  iter=0;
  calpar=0;
}

HStartCalibrater::HStartCalibrater(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {
  rawCat=0;
  calCat=0;
  iter=0;
  calpar=0;
}

HStartCalibrater::~HStartCalibrater(void) {
  if (iter) delete iter;
  iter=0;
}

Bool_t HStartCalibrater::init(void) {
  calpar=(HStartCalPar*)gHades->getRuntimeDb()->getContainer("StartCalPar");
  if (!calpar) return kFALSE;
  rawCat=gHades->getCurrentEvent()->getCategory(catStartRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartRaw,rawCat,"Start");
  }
  calCat=gHades->getCurrentEvent()->getCategory(catStartCal);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartCal);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartCal,calCat,"Start");
  }
  iter=(HIterator *)rawCat->MakeIterator();
  loc.set(2,0,0);
  fActive=kTRUE;

  return kTRUE;
}

Int_t HStartCalibrater::execute(void) {
  HStartRaw *raw=0;
  HStartCal *cal=0;
  Int_t mod=0, strip=0;
  Float_t val=0.F;
  iter->Reset();
  while ((raw=(HStartRaw *)iter->Next())!=0) {
    raw->getAddress(mod,strip);
    loc[0] = mod; loc[1] = strip;
    cal=(HStartCal*)calCat->getSlot(loc);
    if (cal) {
      cal=new (cal) HStartCal;
      if (!cal) return -1;
      HStartCalParChan &pPar=(*calpar)[mod][strip];
      val=pPar.getSlope()*((Float_t)raw->getTime())+pPar.getOffset();
      cal->fill(val,mod,strip);
    }
  }
 
  return 0;
}

ClassImp(HStartCalibrater)















