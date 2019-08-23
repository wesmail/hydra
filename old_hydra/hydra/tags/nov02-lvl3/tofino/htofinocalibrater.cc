using namespace std;
#include <TRandom.h>
#include <time.h>
#include <iostream> 
#include <iomanip>
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
#include "hstartdef.h"
#include "hstarthit.h"

//*-- Created : 24/06/2000 by M.Golubeva
//*-- Modified : 27/04/2001 by T.Wojcik 
/////////////////////////////////////////////////////////////////////
//
//  HTofinoCalibrater calibrates raw Tofino data and puts output 
//  values into cal data category
//
/////////////////////////////////////////////////////////////////////

HTofinoCalibrater::HTofinoCalibrater(void) {

  fCalCat=0;
  fRawCat=0;
  fStartHitCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,-1,-1,-1);
  iter=0;    

}

HTofinoCalibrater::HTofinoCalibrater(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fCalCat=0;
  fRawCat=0;
  fStartHitCat=0;
  pTofinoCalPar = NULL;
  fLoc.set(3,-1,-1,-1);
  iter=0;    
}

Bool_t HTofinoCalibrater::init(void) {

  initParContainer();

  HDetector *start = gHades->getSetup()->getDetector("Start");

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
  
     // Get Start Hit category. If StartHit is not defined or the start detector
    // is not in the setup a Warning is displayed and fStartHitCat is set to 0
 
   if (start) {
      fStartHitCat =gHades->getCurrentEvent()->getCategory(catStartHit);
      if (!fStartHitCat)
        Warning("init","Start Hit level not defining: setting start time to 0");
    } else {
      Warning("init","Start detector not found. Setting start time to 0");
      fStartHitCat = 0;
    }


  return kTRUE;
}

void HTofinoCalibrater::initParContainer() {

  pTofinoCalPar = (HTofinoCalPar *)gHades->getRuntimeDb()
                                         ->getContainer("TofinoCalPar");
}

Int_t HTofinoCalibrater::execute(void) {

  HLocation ffLoc;
  HTofinoRaw* raw = 0;
  HTofinoCal* cal = 0;
  Bool_t startFlag;
  Float_t startTime = 0, calTime = 0;
  Int_t startStrip, startModule;
  fLoc.set(3,-1,-1,-1);
  ffLoc.set(2,-1,-1);

  if (fStartHitCat) {
    startTime = ((HStartHit *)fStartHitCat->getObject(0))->getTime();
    startFlag = ((HStartHit *)fStartHitCat->getObject(0))->getFlag();    
    startStrip = ((HStartHit *)fStartHitCat->getObject(0))->getStrip();    
    startModule = ((HStartHit *)fStartHitCat->getObject(0))->getModule();    
    /*
cout <<"startTime,strip,module = " <<startTime <<" " <<startStrip <<" " <<startModule <<endl;
    if(startFlag)
cout <<"startFlag = kTRUE"  <<endl;
    if(!startFlag)
cout <<"startFlag = kFALSE"  <<endl;    
*/   
  } else {
    startTime = 0.0;
    startFlag = kFALSE;
  }

  
  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)
  while ((raw=(HTofinoRaw *)iter->Next())!=0) {

    fLoc[0] = raw->getSector();
    fLoc[1] = raw->getModule();
    fLoc[2] = raw->getCell();
    if(fLoc[0] == -1 && fLoc[1] == -1 && fLoc[2] == -1) continue;

    if(raw->getTime() > 0.) {
      ffLoc[0] = raw->getSector();
      ffLoc[1] = raw->getCell();
      calTime = ((HTofinoCalPar*)pTofinoCalPar)->
		 channelToTime(fLoc[0],fLoc[2],raw->getTime());
      calTime -= ((HTofinoCalPar*)pTofinoCalPar)->getOffset(ffLoc);
      if(startFlag) calTime -=  startTime;

      if(calTime > 0.) {
	cal = (HTofinoCal*) fCalCat->getSlot(fLoc);
	if(cal != NULL) {
	  cal = new(cal) HTofinoCal;
	  cal->setTime(calTime);
	  cal->setCharge(raw->getCharge());
	  cal->setAddress(fLoc[0],fLoc[1],fLoc[2]);
	} //if(cal != NULL) 
      } //if(calTime > 0.) 
    } //if(raw->getTime() > 0.) 

  } // while ((geant=(HGeantTof *)iter->Next())!=0) 
  return 0;

} // htofinocalibrater

ClassImp(HTofinoCalibrater)










