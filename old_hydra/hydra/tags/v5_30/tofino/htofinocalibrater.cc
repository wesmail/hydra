#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include "htofinocalibrater.h"
#include "tofinodef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "htofinodetector.h"
#include "htofinoraw.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "htofinocalpar.h"
#include "htofinocal.h"

//*-- Created: 24/06/2000 by M.Golubeva

/////////////////////////////////////////////////////////////////////
//
//  HTofinoCalibrater calibrates raw Tofino data and puts output 
//  values into cal data category
//
/////////////////////////////////////////////////////////////////////

HTofinoCalibrater::HTofinoCalibrater(void) {

  fCalCat=0;
  fRawCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,-1,-1,-1);
  iter=0;    

}

HTofinoCalibrater::HTofinoCalibrater(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fCalCat=0;
  fRawCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,-1,-1,-1);
  iter=0;    
}

Bool_t HTofinoCalibrater::init(void) {

  initParContainer();

  fRawCat = gHades->getCurrentEvent()->getCategory(catTofinoRaw);
  if (!fRawCat) {
    fRawCat = gHades->getSetup()->getDetector("Tofino")
                                ->buildCategory(catTofinoRaw);
    if (!fRawCat) 
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofinoRaw,fRawCat,"Tofino");
  }
  iter = (HIterator *)fRawCat->MakeIterator("native");

  fCalCat = gHades->getCurrentEvent()->getCategory(catTofinoCal);
  if (!fCalCat) {
    fCalCat = gHades->getSetup()->getDetector("Tofino")
                                ->buildCategory(catTofinoCal);

    if (!fCalCat)
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catTofinoCal,fCalCat,"Tofino");
  }  

  return kTRUE;
}

void HTofinoCalibrater::initParContainer() {

  pTofinoCalPar = (HTofinoCalPar *)gHades->getRuntimeDb()
                                         ->getContainer("TofinoCalPar");
  if (!pTofinoCalPar) {
    pTofinoCalPar=new HTofinoCalPar;
    gHades->getRuntimeDb()->addContainer(pTofinoCalPar);
  }
}

Int_t HTofinoCalibrater::execute(void) {

  HTofinoRaw* raw = 0;
  HTofinoCal* cal = 0;

  fLoc.set(3,-1,-1,-1);
  
  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)

  while ((raw=(HTofinoRaw *)iter->Next())!=0) {

    fLoc[0] = raw->getSector();
    fLoc[1] = raw->getModule();
    fLoc[2] = raw->getCell();
    if(fLoc[0] == -1 && fLoc[1] == -1 && fLoc[2] == -1) continue;

    cal = (HTofinoCal*) fCalCat->getSlot(fLoc);

    //    if(cal != 0) {
    //       cal->incNHit();  // increment counter for additional hits
    //    }

    cal = new(cal) HTofinoCal;
    cal->setTime(((HTofinoCalPar*)pTofinoCalPar)->
	         channelToTime(fLoc[0],fLoc[2],raw->getTime()));
    cal->setCharge(raw->getCharge());
    cal->setAddress(fLoc[0],fLoc[1],fLoc[2]);

  } // while ((geant=(HGeantTof *)iter->Next())!=0) 
  return 0;

} // htofinocalibrater

ClassImp(HTofinoCalibrater)










