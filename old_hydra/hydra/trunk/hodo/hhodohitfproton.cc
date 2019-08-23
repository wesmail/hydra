//*--Author : B. Spruck
//
//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
//  HHodoHitFProton
//
//  Simple Hodo hitfinder for protons beam 2006.
//  The name is misleading, should be named HodoHitFFiber
//  Used is hodo detector module 0 (todo!)
//  This code is for fiber detector
//  "old" pion start/veto should use other code
//
//  This code has to do some nasty things:
//  Define what ONE hit is (overlapping fibers, crosstalk)
//  This should depend on position AND time of the hits
//  only one hit in a time window
//  time window should be in oracle
//  Note:
//  In May06 we use ONLY 32 channels of 96, and this is hardcoded!
//  For pions another reconstructor is needed anyhow...
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhodohitfproton.h"
#include "hhododetector.h"
#include "hododef.h"
#include "hhodocal.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hades.h"
#include "heventheader.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hhodohitfproton.h"
#include "hhodohit.h"
#include "hlocation.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>

HHodoHitFProton::HHodoHitFProton(void)
{
   clear();
}

HHodoHitFProton::HHodoHitFProton(const Text_t *name,const Text_t *title) :
               HReconstructor(name,title)
{
   clear();
}

HHodoHitFProton::~HHodoHitFProton(void)
{
   if (pIterCal) delete pIterCal;
}

Bool_t HHodoHitFProton::init(void)
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

   loc.set(1,0);
   fActive=kTRUE;
   return kTRUE;
}

Int_t HHodoHitFProton::find_hit(void)
{
   // How to check for several clusters?

   // What about clusters with different/same time?

   // Now do some "kind of" hit detection
   Bool_t skipthis;
   Bool_t foundone;
   skipthis=false;
   foundone=false;

   for(Int_t ii=0; ii<HODOHITF_PROTON_MAXCHN; ii++){
      if( hit[ii]){
         if( foundone){
            // Means two differnt hits OR >3
            skipthis=true;
            break;
         }
         if( ii<HODOHITF_PROTON_MAXCHN-1 && hit[ii+1]){
            if( ii<HODOHITF_PROTON_MAXCHN-2 && hit[ii+2]){
               if( ii<HODOHITF_PROTON_MAXCHN-3 && hit[ii+3]){
                  // more than three fibers, ignore
                  skipthis=true;
                  break;
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
      if( skipthis) break;
   }

   if( skipthis) return(0);

   if(foundone) return(1);

   return(0);
}

void HHodoHitFProton::prepare_hitfinder(void)
{
   for(Int_t ii=0; ii<HODOHITF_PROTON_MAXCHN; ii++){
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

   pIterCal->Reset();
   while ((pCal=(HHodoCal *)pIterCal->Next())!=0)
   {
      Int_t m;// Number of Cal Times per channel
      // only use module 0 for proton startdetector
      if(pCal->getModule()!=0) continue;

      m=pCal->getNHits();
      if(m<=0) continue;// No Hits -> forget it

      Int_t nStrip;
      Float_t fTime;

      nStrip = pCal->getStrip();
      // Spikes should have been killed by hodocalibrator
      // We just taker the first hit here... later change this to check the right time window
      fTime = pCal->getTime(1); // getTof();
      if(fTime<=0) continue;

      // in May06 only channel 0-31 is used anyhow
      if( nStrip>=0 && nStrip<HODOHITF_PROTON_MAXCHN){
         hit[nStrip]=true;
         time[nStrip]=fTime;
      }
   }
}

Int_t HHodoHitFProton::execute(void)
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

      loc[0] = 0;// nModule;
      loc[1] = hitStrip;
      pHit=(HHodoHit *)pHitCat->getNewSlot(loc);
      if(pHit){
         pHit = new (pHit) HHodoHit;
         pHit->setModule(0);// hardcoded
         pHit->setStrip(hitStrip);
         pHit->setTime(hitTime);
//       pHit->setHodoMult(nHodoMult);
         pHit->setFlag(kTRUE);
      }

      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}

Bool_t HHodoHitFProton::finalize(void)
{
   return kTRUE;
}
void HHodoHitFProton::clear(void)
{
   pCalCat=0;
   pHitCat=0;
   pIterCal=0;
}

ClassImp(HHodoHitFProton)
