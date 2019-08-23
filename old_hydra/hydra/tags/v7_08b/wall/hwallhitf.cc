using namespace std;
#include <TRandom.h>
#include <time.h>
#include <iostream> 
#include <iomanip>
#include "hwallhitf.h" //FK//
#include "walldef.h"   //FK//
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hwalldetector.h" //FK//
#include "hwallraw.h"   //FK// 
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "hwallcalpar.h" //FK//
#include "hwallhit.h"   //FK//
#include "hstartdef.h"
#include "hstarthit.h"

//*-- Created : 24/06/2000 by M.Golubeva
//*-- Modified : 27/04/2001 by T.Wojcik 
//*-- Modified: 11/8/2005 by F. Krizek
/////////////////////////////////////////////////////////////////////
//
//  HWallHitf calibrates raw Wall data and puts output 
//  values into cal data category
//
/////////////////////////////////////////////////////////////////////

HWallHitF::HWallHitF(void) {

  fHitCat=NULL;
  fRawCat=NULL;
  fStartHitCat=NULL;
  pWallCalPar = NULL;
  fLoc.set(1,-1);
  iter=NULL;    

}

HWallHitF::HWallHitF(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {

  fHitCat=NULL;
  fRawCat=NULL;
  fStartHitCat=NULL;
  pWallCalPar = NULL;
  fLoc.set(1,-1);
  iter=NULL;    
}

HWallHitF::~HWallHitF(){
    if(iter){delete iter; iter=NULL;}
}

void HWallHitF::fillHit(HWallHit *hit, HWallRaw *raw) {
}

Bool_t HWallHitF::init(void) { 

  initParContainer();

  HDetector *start = gHades->getSetup()->getDetector("Start");

  fRawCat = gHades->getCurrentEvent()->getCategory(catWallRaw);
  if (!fRawCat) {
    fRawCat = gHades->getSetup()->getDetector("Wall")
                                ->buildCategory(catWallRaw);
    if (!fRawCat) 
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catWallRaw,fRawCat,"Wall");
  }
  iter = (HIterator *)fRawCat->MakeIterator("native");

  fHitCat = gHades->getCurrentEvent()->getCategory(catWallHit);
  if (!fHitCat) {
    fHitCat = gHades->getSetup()->getDetector("Wall")
                                ->buildCategory(catWallHit);

    if (!fHitCat)
      return kFALSE;
    else
      gHades->getCurrentEvent()->addCategory(catWallHit,fHitCat,"Wall");
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

void HWallHitF::initParContainer() {

  pWallCalPar = (HWallCalPar *)gHades->getRuntimeDb()
                                         ->getContainer("WallCalPar");
}

Int_t HWallHitF::execute(void) {

  HLocation ffLoc;
  HWallRaw* raw = NULL;
  HWallHit* hit = NULL;
  Bool_t startFlag;
  Float_t startTime = 0, calTime = 0;
  Int_t startStrip, startModule;
  fLoc.set(1,-1);
  ffLoc.set(1,-1);

  if (fStartHitCat) {
	HStartHit *h=NULL;
	if((h=(HStartHit *)fStartHitCat->getObject(0))!=NULL){
		startTime = h->getTime();
		startFlag = h->getFlag();    
		startStrip = h->getStrip();    
		startModule = h->getModule();
	}    
	else{
	startTime =0.0;
        startFlag = kFALSE;
	}
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
  while ((raw=(HWallRaw *)iter->Next())!=0) {

    fLoc[0] =  raw->getCell();
    if(fLoc[0] == -1) continue;

    if(raw->getTime() > 0.) {
      fLoc[0] = raw->getCell();
      HWallCalParCell &pPar=(*pWallCalPar)[0][ fLoc[0]];
      calTime =  pPar.getTDC_Slope()*raw->getTime()-pPar.getTDC_Offset();

      printf("slope %f offset %f rawtime %f caltime %f \n",pPar.getTDC_Slope(),pPar.getTDC_Offset(),(raw->getTime()),calTime);

      if(startFlag) calTime -=  startTime;

      //   if(calTime > 0.) {
	hit = (HWallHit*) fHitCat->getSlot(fLoc);
	if(hit != NULL) {
	  hit = new(hit) HWallHit;
	  hit->setTime(calTime);
	  //  hit->setCharge(((HWallCalParCell*)pWallCalPar)->getADC_Slope()*(raw->getCharge())-((HWallCalParCell*)pWallCalPar)->getADC_Offset());
          hit->setCharge(pPar.getADC_Slope()*(raw->getCharge())-pPar.getADC_Offset());
         printf("slope %f offset %f rawcharge %f calcharge %f \n",pPar.getADC_Slope(),pPar.getADC_Offset(),raw->getCharge(),pPar.getADC_Slope()*(raw->getCharge())-pPar.getADC_Offset());  
	  // hit->setCharge(raw->getCharge());
	  hit->setAddress(fLoc[0]);
          fillHit(hit,raw);
	} //if(hit != NULL) 
	// } //if(calTime > 0.) 
    } //if(raw->getTime() > 0.) 

  } // while ((geant=(HGeantTof *)iter->Next())!=0) 
  return 0;

} // htofinocalibrater

ClassImp(HWallHitF)










