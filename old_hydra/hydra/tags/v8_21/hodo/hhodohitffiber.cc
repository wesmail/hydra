//*--Author : B. Spruck
//
//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
//  HHodoHitFFiber
//
//  Simple Hodo hitfinder for proton beam 2006.
//  This code is for fiber detector
//  Used is hodo detector module usemod (changeable by user)
//  "old" pion start/veto or diamond should use other code
//
//  This code has to do some nasty things:
//  Define what ONE hit is (overlapping fibers, crosstalk)
//  This should depend on position AND time of the hits
//  only one hit in a time window ... not done yet!
//  time window should be in oracle ... not done yet, well RefTimeHit but...!
//  Note:
//  In May06 we use ONLY 32 channels of 96, and this is hardcoded!
//  For pions another reconstructor is needed anyhow...
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhodohitffiber.h"
#include "hhododetector.h"
#include "hododef.h"
#include "hhodocal.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hades.h"
#include "heventheader.h"
#include "hevent.h"
#include "hhodorefhitpar.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hhodohitffiber.h"
#include "hhodohit.h"
#include "hlocation.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>

HHodoHitFFiber::HHodoHitFFiber(void)
{
   clear();
}

HHodoHitFFiber::HHodoHitFFiber(const Text_t *name,const Text_t *title) :
               HReconstructor(name,title)
{
   clear();
}

HHodoHitFFiber::~HHodoHitFFiber(void)
{
   if (pIterCal) delete pIterCal;
}

Bool_t HHodoHitFFiber::init(void)
{
   HHodoDetector *hodo;
   hodo=(HHodoDetector *)gHades->getSetup()->getDetector("Hodo");

   if(!hodo){
      Error("init","No Hodo Det. found.");
      return kFALSE;
   }

   // Categories
   pCalCat=hodo->buildCategory(catHodoCal);
   if (!pCalCat) return kFALSE;
   pHitCat=hodo->buildCategory(catHodoHit);
   if (!pHitCat) return kFALSE;

   // Iterators
   pIterCal=(HIterator *)pCalCat->MakeIterator();

	// RefHit should not be needed if the TRB Time windows are set correctly
	// But as Long_t as these windows are far to open, this cut is very useful(?)
   refhit=(HHodoRefHitPar*)gHades->getRuntimeDb()->getContainer("HodoRefHitPar");
   if (!refhit){
      Error("init","No HodoRefHit Parameters");
      return kFALSE;
   }

   loc.set(2,0,0);
   fActive=kTRUE;
   return kTRUE;
}

Bool_t HHodoHitFFiber::find_hit(void)
{
   // How to check for several clusters?

   // What about clusters with different/same time?

   // Now do some "kind of" hit detection
   Bool_t foundone;
   foundone=false;

   for(Int_t ii=0; ii<HODOHITF_FIBER_MAXCHN; ii++){
      if( hit[ii]){
         if( foundone){
            // Means two differnt hits OR >3
				return(false);
         }
         if( ii<HODOHITF_FIBER_MAXCHN-1 && hit[ii+1]){
            if( ii<HODOHITF_FIBER_MAXCHN-2 && hit[ii+2]){
               if( ii<HODOHITF_FIBER_MAXCHN-3 && hit[ii+3]){
                  // more than three fibers, ignore
						return(false);
               }else{// only three neighborings
                  hit[ii]=0;
                  hit[ii+2]=0;
                  hitStrip=ii+1;
                  hitTime=time[ii+1];

                  // take the fastest time, but the middle strip nr ... is this o.k.?
                  if(time[ii]<hitTime) hitTime=time[ii];
                  if(time[ii+2]<hitTime) hitTime=time[ii+2];

                  foundone=true;
               }
            }else{// only two
               hit[ii+1]=0;
               hitStrip=ii;
               if(time[ii]<time[ii+1]) hitTime=time[ii]; else hitTime=time[ii+1];
               foundone=true;
            }
         }else{// one fiber only, that is easy
            hitStrip=ii;
            hitTime=time[ii];
            foundone=true;
         }
      }// else no hit, nothing to do
   }

   return(foundone);
}

void HHodoHitFFiber::prepare_hitfinder(void)
{
   for(Int_t ii=0; ii<HODOHITF_FIBER_MAXCHN; ii++){
      hit[ii]=false;
      time[ii]=-1000000.0;
   }

   // Here we depend on Cal that should have cleaned already up to Mult 1 per fiber!!!
   // which is not the case!

   // What do we do?

   // Step 1:
   // Apply another time window (again)?
   // Step 2:
   // Take mean of the times
   // Step 3:
   // Reject what is of the mean
   // Step 4:
   // find hit

   HHodoCal *pCal  = 0;

         HHodoRefHitParMod &pRefHit=(*refhit)[usemod];
         Float_t lower, upper;
         lower=pRefHit.getHitLow();// Misuse ... rename function and variable
         upper=pRefHit.getHitHigh();

	pIterCal->Reset();
   while ((pCal=(HHodoCal *)pIterCal->Next())!=0)
   {
      Int_t m;// Number of Cal Times per channel
      // only use module "usemod" for fiber startdetector
      if(pCal->getModule()!=usemod) continue;

      m=pCal->getNHits();
      if(m<=0) continue;// No Hits -> forget it

      Int_t nStrip;
      Float_t fTime=-1000000.0;

      nStrip = pCal->getStrip();
      // Spikes should have been killed by hodocalibrator
      // We just taker the first hit here... later change this to check the right time window
		for(Int_t i=0; i<m; i++){
         Float_t t;
      	t = pCal->getTime(i+1); // getTof();
			if( lower>=upper || (t>lower && t<upper)){
           fTime=t;
           break;
         }
		}
      if(fTime<=-999999) continue;

      // in May06 only channel 0-31 is used anyhow
      if( nStrip>=0 && nStrip<HODOHITF_FIBER_MAXCHN){
         hit[nStrip]=true;
         time[nStrip]=fTime;
      }
   }
}

Int_t HHodoHitFFiber::execute(void)
{
// Find the one and only true hit
// There is only ONE hit defined
   Bool_t found;

   // First do some simplified hit search:
   // works for 1,2,3 hits in a row
   prepare_hitfinder();
   found=find_hit();

   if(!found){
      // Here we should try to do some other nasty things...
      // f.i. better time filtering, ADC filtering and so on.
      // afterwards  try again to find a hit
      // but this code can not be written as Long_t as I dont have example data

      // prepare_again_hitfinder();
      // found=find_again_hit();
   }

   if(found){
      HHodoHit *pHit  = 0;

      loc[0] = usemod;// nModule;
      loc[1] = hitStrip;
      pHit=(HHodoHit *)pHitCat->getNewSlot(loc);
      if(pHit){
         pHit = new (pHit) HHodoHit;
         pHit->setModule(usemod);
         pHit->setStrip(hitStrip);
         pHit->setTime(hitTime);
//       pHit->setHodoMult(nHodoMult);
         pHit->setFlag(kTRUE);
      }

   }
   // always return a success or the other hitfinders (and other tasks) will not run
   return EXIT_SUCCESS;
}

Bool_t HHodoHitFFiber::finalize(void)
{
   return kTRUE;
}
void HHodoHitFFiber::clear(void)
{
   pCalCat=0;
   pHitCat=0;
   pIterCal=0;
}

ClassImp(HHodoHitFFiber)
