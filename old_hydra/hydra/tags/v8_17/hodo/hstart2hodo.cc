//*-- Created 30.3.2006 by B. Spruck
//*-- Modified 27.3.2007 by B. Spruck

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////
//
//  HStart2Hodo:
//
//  Copies Start raw data to Hodo raw data category
//  Modules are set with set_mod(start_mod, hodo_mod);
//  Intention 1: Test libHodo with real data (start detektor)
//  Intention 2: Analyse data from previous pion test beam
//  times with new analysis software based on libHodo
//  Do not use unless you know what you are doing!
//
////////////////////////////////////////////////////////////////


using namespace std;
#include "hstart2hodo.h"
#include "hododef.h"
#include "hhodoraw.h"
#include "hhododetector.h"
#include "hstartdef.h"
#include "hstartraw.h"
#include "hstartdetector.h"
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


HStart2Hodo::HStart2Hodo(void)
{
  rawHodoCat=0;
  rawStartCat=0;
  iter=0;
}

HStart2Hodo::HStart2Hodo(Text_t *name,Text_t *title) : HReconstructor(name,title)
{
  rawHodoCat=0;
  rawStartCat=0;
  iter=0;
}

HStart2Hodo::~HStart2Hodo(void)
{
}

Bool_t HStart2Hodo::init(void)
{
   HStartDetector *start;
   start=(HStartDetector *)gHades->getSetup()->getDetector("Start");

   if(!start){
      Error("init","No Start Det. found.");
      return kFALSE;
   }

   // Categories
   rawStartCat=start->buildCategory(catStartRaw);
   if (!rawStartCat) return kFALSE;

  // creates the raw category and gets the pointer to the TRB lookup table
  rawHodoCat = gHades->getCurrentEvent ()->getCategory (catHodoRaw);
  if (!rawHodoCat) {
    rawHodoCat = gHades->getSetup ()->getDetector ("Hodo")->buildCategory (catHodoRaw);
    if (!rawHodoCat){
			Error("init","rawHodoCat = new HLinearCategory(\"HHodoRaw\"); failed!");
			return kFALSE;
		}else{
			gHades->getCurrentEvent ()->addCategory (catHodoRaw, rawHodoCat, "Hodo");
		}
  }
  loc.set (2,0,0);

  iter=(HIterator *)rawStartCat->MakeIterator();

  fActive=kTRUE;

  return kTRUE;
}

Int_t HStart2Hodo::execute(void)
{// What will be done here
 // Copy all Raw Start to Raw Hodo
 // compatibility with old data where hodo are in start cat
 // Do not use unless you know what you are doing!

	HHodoRaw *pHodoRaw=0;
	HStartRaw *pStartRaw=0;

	iter->Reset();

	while ((pStartRaw=(HStartRaw *)iter->Next())!=0) {
		Int_t st, mod;
		pStartRaw->getAddress(mod,st);
		if (mod==smod && st>=0){
	    loc[0] = hmod;
      loc[1] = st;
			pHodoRaw = (HHodoRaw *) rawHodoCat->getObject (loc);
			if (!pHodoRaw) {
					pHodoRaw = (HHodoRaw *) rawHodoCat->getSlot (loc);
					if (pHodoRaw) {
						pHodoRaw = new (pHodoRaw) HHodoRaw;
						pHodoRaw->setAddress(loc[0], loc[1]);
					}
					else {
						Error ("execute()", "Can't get slot");
						cerr << "execute(): Can't get slot  Mod: "<<loc[0] << " Slot " << loc[1] << endl;
						return EXIT_FAILURE;
					}
			} else {
					Error ("execute()", "Slot already exists! This should not happen!");
					cerr << "execute(): Slot already exists! Mod: "<<loc[0] << " Slot " << loc[1]  << endl;
					return EXIT_FAILURE;
			}

			// pHodoRaw->setModule(hmod);
			// pHodoRaw->setFiber(st);
			pHodoRaw->fill(pStartRaw->getTime(),1);// Width=1
		}
	}

	return EXIT_SUCCESS;
}

ClassImp(HStart2Hodo)
