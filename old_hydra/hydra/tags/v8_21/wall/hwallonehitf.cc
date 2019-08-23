//*-- AUTHOR Bjoern Spruck
//*-- created : 24.03.06
// Modified for Wall by M.Golubeva 01.11.2006

using namespace std;
#include "hwallonehitf.h"
#include "hwallonehit.h"
#include "hwallrefwinpar.h"
#include "hwalldetector.h"
#include "hwallcalibrater.h"
#include "walldef.h"
#include "hwallraw.h"
#include "hwallcal.h"
#include "hwallcalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include <iostream>
#include <iomanip>

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
//
//  HWallHitFinder: select one hit from 4 cal hits
//
////////////////////////////////////////////////////////////////


void HWallOneHitF::initParContainer() {

  //pWallCalPar = (HWallCalPar *)gHades->getRuntimeDb()
  //->getContainer("WallCalPar");
  pWallRefWinPar = (HWallRefWinPar *)gHades->getRuntimeDb()
                                         ->getContainer("WallRefWinPar");
}

HWallOneHitF::HWallOneHitF(void)
{
  //fRawCat=0;
   fCalCat=0;
   fOneHitCat=0;
   pWallRefWinPar=0;
   iter=0;
   //pWallCalPar=0;
   fLoc.set(1,-1);
}

HWallOneHitF::HWallOneHitF(const Text_t *name,const Text_t *title) :
   HReconstructor(name,title)
{
  //fRawCat=0;
   fCalCat=0;
   fOneHitCat=0;
   pWallRefWinPar=0;
   //pWallCalPar=0;
   iter=0;
   fLoc.set(1,-1);
}

HWallOneHitF::~HWallOneHitF(void)
{
   if (iter) delete iter;
   iter=0;
}

//HWallRaw *raw should be changed to HWallCal *cal
void HWallOneHitF::fillHit(HWallOneHit *hit, HWallCal *cal) {
}

Bool_t HWallOneHitF::init(void)
{
   initParContainer();

   HWallDetector *wall;
   wall=(HWallDetector *)gHades->getSetup()->getDetector("Wall");

   if(!wall){
      Error("init","No Wall Det. found.");
      return kFALSE;
   }

   // Categories
   //fRawCat=gHades->getCurrentEvent()->getCategory(catWallRaw);  
   //if (!fRawCat) return kFALSE;
   
   fCalCat=gHades->getCurrentEvent()->getCategory(catWallCal);  
   if (!fCalCat) return kFALSE;
   
   fOneHitCat=wall->buildCategory(catWallOneHit);
   if (!fOneHitCat) return kFALSE;
   else
   gHades->getCurrentEvent()->addCategory(catWallOneHit,fOneHitCat,"Wall");

   // Parameters
   //calpar=(HWallCalPar*)gHades->getRuntimeDb()->getContainer("WallCalPar");
   //if (!calpar) return kFALSE;

   pWallRefWinPar=(HWallRefWinPar*)gHades->getRuntimeDb()->getContainer("WallRefWinPar");
   if (!pWallRefWinPar) return kFALSE;

   iter=(HIterator *)fCalCat->MakeIterator(); //change to cal when it will be created
   //fActive=kTRUE;

   return kTRUE;
}

Int_t HWallOneHitF::execute(void)
{
  //cout <<"MARINA HWallOneHitF execute ------------ " <<endl; 
  //HWallRaw *raw=0;
   HWallCal *cal=0;
   HWallOneHit* hit = NULL;
   //Int_t cell=-1;
   fLoc.set(1,-1);
   //Int_t mod=0;

   Float_t calTime,hitTime=-400;
   Float_t calADC,hitAdc=-400;

   // Do the calibration here

   // Multiplicity checks i would postphone to hitfinder...

   // But how do we handle cases with tdc channel mult>4?
   // same question applies to what to do if hits were rejected by TDC itself -> see unpacker
   // maybe this should go to the raw category...

   //Fill cal category
   iter->Reset();
   //while ((raw=(HWallRaw *)iter->Next())!=0) {
   while ((cal=(HWallCal *)iter->Next())!=0) {
      Int_t m;// channel hit multiplicty

      m=cal->getNHits();
      //cout <<"marina HWallOneHitF execute ------------- " <<m <<endl;
      if(m<=0) continue;// No Hits -> forget it
      if(m>cal->getMaxMult()) m=cal->getMaxMult();

      fLoc[0] =  cal->getCell();
      if(fLoc[0] == -1) continue;

      //HWallRefWinParMod &pRefWin=(*pWallRefWinPar)[mod];
         Float_t lower, upper;
         lower=pWallRefWinPar->getRefWinLow();
         upper=pWallRefWinPar->getRefWinHigh();
	 //cout <<"lower,upper " <<lower <<" " <<upper <<endl;

         for(Int_t i=0; i<m; i++){
	   hitTime=-400; hitAdc=-400;
            calTime  = cal->getTime(i+1);
            calADC  = cal->getAdc(i+1);

            if(calADC>=0.0){
	      if(calTime>=lower && calTime<=upper) {
		hitTime=calTime; hitAdc=calADC;
		goto hit;
	      }  
	    }

	 }//for(Int_t i=0; i<m; i++)

   hit:

	hit = (HWallOneHit*) fOneHitCat->getSlot(fLoc);
	if(hit != NULL) {
	  hit = new(hit) HWallOneHit;
	  hit->setTime(hitTime); hit->setCharge(hitAdc);	 
	  hit->setAddress(fLoc[0]);
          fillHit(hit,cal);
	}//if(hit != NULL)

    }//while ((raw=(HWallRaw *)iter->Next())!=0)

   //return EXIT_SUCCESS;
  return 0;
}

ClassImp(HWallOneHitF)
