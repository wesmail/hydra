//*-- Created 30.3.2006 by B. Spruck
//*-- Modified 27.3.2007 by B. Spruck

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////
//
//  HHodoHit2Start:
//
//  Copies Hodo hit data to Start hit data category
//  (for backward compatibility with analysis software)
//  Module is set with set_mod(hodo_mod);
//  Start module is fixed to 0
//
////////////////////////////////////////////////////////////////


using namespace std;
#include "hhodohit2start.h"
#include "hododef.h"
#include "hhodohit.h"
#include "hhododetector.h"
#include "hstartdef.h"
#include "hstarthit.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include <iostream>
#include <iomanip>


HHodoHit2Start::HHodoHit2Start(void)
{
  hitHodoCat=0;
  hitStartCat=0;
  iter=0;
}

HHodoHit2Start::HHodoHit2Start(Text_t *name,Text_t *title) : HReconstructor(name,title)
{
  hitHodoCat=0;
  hitStartCat=0;
  iter=0;
}

HHodoHit2Start::~HHodoHit2Start(void)
{
}

Bool_t HHodoHit2Start::init(void)
{
   HHodoDetector *hodo;
   hodo=(HHodoDetector *)gHades->getSetup()->getDetector("Hodo");

   if(!hodo){
      Error("init","No Hodo Det. found.");
      return kFALSE;
   }

   // Categories
   hitHodoCat=hodo->buildCategory(catHodoHit);
   if (!hitHodoCat) return kFALSE;

   // Start is not having nice function for that
   hitStartCat=gHades->getCurrentEvent()->getCategory(catStartHit);
   if (!hitStartCat) {
      hitStartCat = new HLinearCategory("HStartHit");
      if (!hitStartCat){
         Error("init","hitStartCat = new HLinearCategory(\"HStartHit\"); failed!");
         return kFALSE;
      }else{
         gHades->getCurrentEvent()->addCategory(catStartHit,hitStartCat,"Start");
      }
  }

  iter=(HIterator *)hitHodoCat->MakeIterator();

  loc.set(1,0);// this is fixed for StartHit
  fActive=kTRUE;

  return kTRUE;
}

Int_t HHodoHit2Start::execute(void)
{// What will be done here
 // Copy all Hit data from Hodo module "usemod" to Start module 0
 // Used for may06 where hodo is used as a start det.
 // Do not use unless you know what you are doing!

   HHodoHit *pHodoHit=0;
   HStartHit *pStartHit=0;

   iter->Reset();

   //  Hit finding & storing.
   pStartHit=(HStartHit *)hitStartCat->getNewSlot(loc);
   if (pStartHit) {
      Int_t cnt=0;
      Int_t mod=-1, strip=-1;
      Int_t mult=-1;
      Float_t time=-1000000.0;
      Bool_t flag=kFALSE;

      pStartHit = new(pStartHit) HStartHit;
      pStartHit->setModule(-1);
      pStartHit->setStrip(-1);
      pStartHit->setTime(time);
      pStartHit->setStartMult(-1);
      pStartHit->setFlag(kFALSE);

      cnt=0;
      while ((pHodoHit=(HHodoHit *)iter->Next())!=0) {
         Int_t st;
         pHodoHit->getAddress(mod,st);
         if (mod==usemod && st>=0){
            // Check Hodo mod==0
            // and set Start mod=0
            strip=st;
            time=pHodoHit->getTime();
            mult=pHodoHit->getHodoMult();// not filled yet
            flag=pHodoHit->getFlag();// not filled yet
            cnt++;
         }
      }

      if( cnt==1){
         pStartHit->setModule(0);// =0 Is fixed für Start Hits
         pStartHit->setStrip(strip);
         pStartHit->setTime(time);
         pStartHit->setFlag(kTRUE);// flag  not filled yet
         pStartHit->setStartMult(1);// mult  not filled yet
      }
   } else {
      Error("execute()",
      "Can't get slot\n");
      return EXIT_FAILURE;
   }

	return EXIT_SUCCESS;
}

ClassImp(HHodoHit2Start)
