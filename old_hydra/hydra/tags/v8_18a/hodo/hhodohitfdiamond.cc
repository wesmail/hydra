//*--Author : B. Spruck
//
//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
//  HHodoHitFDiamond
//
//  Simple Hodo hitfinder for protons beam 2007.
//  This code is for diamond detector (and "old" pion start/veto?)
//  fiber should use other code
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhodohitfdiamond.h"
#include "hhododetector.h"
#include "hododef.h"
#include "hhodocal.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hhodorefhitpar.h"
#include "hades.h"
#include "heventheader.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hhodohit.h"
#include "hlocation.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>

HHodoHitFDiamond::HHodoHitFDiamond(void)
{
   clear();
}

HHodoHitFDiamond::HHodoHitFDiamond(Text_t *name,Text_t *title) :
               HReconstructor(name,title)
{
   clear();
}

HHodoHitFDiamond::~HHodoHitFDiamond(void)
{
   if (pIterCal) delete pIterCal;
}

Bool_t HHodoHitFDiamond::init(void)
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

Bool_t HHodoHitFDiamond::find_hit(void)
{
   Bool_t foundone;
   foundone=false;

   for(Int_t ii=0; ii<HODOHITF_DIAMOND_MAXCHN; ii++){
      if( hit[ii]){
         if( foundone){
         	// Means two differnt
				return(false);
			}

         hitStrip=ii;
         hitTime=time[ii];
         foundone=true;
//         break; // we take only the first one ... wrokaround
      }// else no hit, nothing to do
   }
   return(foundone);
}

void HHodoHitFDiamond::prepare_hitfinder(void)
{
   for(Int_t ii=0; ii<HODOHITF_DIAMOND_MAXCHN; ii++){
      hit[ii]=false;
      time[ii]=-1000000.0;
   }

   HHodoCal *pCal  = 0;


         HHodoRefHitParMod &pRefHit=(*refhit)[usemod];
         Float_t lower, upper;
         lower=pRefHit.getHitLow();// Misuse ... rename function and variable
         upper=pRefHit.getHitHigh();
	pIterCal->Reset();
   while ((pCal=(HHodoCal *)pIterCal->Next())!=0)
   {
      Int_t m;// Number of Cal Times per channel
      // only use module "usemod" for diamond startdetector
      if(pCal->getModule()!=usemod) continue;

      m=pCal->getNHits();
      if(m<=0) continue;// No Hits -> forget it

      Int_t nStrip;
      Float_t fTime;

      fTime=-1000000.0;
      nStrip = pCal->getStrip();
      // Spikes should have been killed by hodocalibrator
      // We just take the first hit here... later change this to check the right time window
		for(Int_t i=0; i<m; i++){
         Float_t t;
      	t = pCal->getTime(i+1); // getTof();
			if( lower>=upper || (t>lower && t<upper)){
           fTime=t;
           break;
         }
		}
      if(fTime<=-999999) continue;

      // in Apr07 only channel 0-xx is used anyhow
      if( nStrip>=0 && nStrip<HODOHITF_DIAMOND_MAXCHN){
         hit[nStrip]=true;
         time[nStrip]=fTime;
      }
   }
}

Int_t HHodoHitFDiamond::execute(void)
{
// There is only ONE hit defined
   Bool_t found;
   prepare_hitfinder();
   found=find_hit();

   if(found){
      HHodoHit *pHit  = 0;

      loc[0] = usemod;// nModule;
      loc[1] = hitStrip;
      pHit=(HHodoHit *)pHitCat->getNewSlot(loc);
      if(pHit){
         pHit = new (pHit) HHodoHit;
         pHit->setModule(usemod);// hardcoded
         pHit->setStrip(hitStrip);
         pHit->setTime(hitTime);
//       pHit->setHodoMult(nHodoMult);
         pHit->setFlag(kTRUE);
      }

   }
   // always return a success or the other hitfinders (and other tasks) will not run
   return EXIT_SUCCESS;
}

Bool_t HHodoHitFDiamond::finalize(void)
{
   return kTRUE;
}
void HHodoHitFDiamond::clear(void)
{
   pCalCat=0;
   pHitCat=0;
   pIterCal=0;
}

ClassImp(HHodoHitFDiamond);
