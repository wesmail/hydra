using namespace std;
#include "hstartcalibrater.h"
#include "hstartdef.h"
#include "hstartcal.h"
#include "hstartselfcopar.h"
#include "htofcalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "heventheader.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hiterator.h"
#include "hlinearcategory.h"
#include "hstarthitfproton.h"
#include "hstarthit.h"
#include "tofdef.h"
#include "htofraw.h"
#include "htofinocalibrater.h"
#include "tofinodef.h"
#include "htofinoraw.h"
#include "htofinocalpar.h"
#include "hlocation.h"
#include "TMath.h"
#include <iostream> 
#include <iomanip>
#include "TH1.h"

//*--Author : T.Wojcik
//
// Simple Start hitfinder for protons beam 2003.
// As a start detector is used third module (pstart)
// Hit is defined when multiplicity of module 3 is equal 1
// and timeR and timeL or timeM is set, respectively.
//
///////////////////////////////////////////////////////////////////////////////


HStartHitFProton::HStartHitFProton(void) {
	clear();
}

HStartHitFProton::HStartHitFProton(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {
	clear();
}

HStartHitFProton::~HStartHitFProton(void) {
  if (pIterCal) delete pIterCal;
}

Bool_t HStartHitFProton::init(void) {
	pCalCat=gHades->getCurrentEvent()->getCategory(catStartCal);
	if (!pCalCat) {
		pCalCat=gHades->getSetup()->getDetector("Start")->
								buildCategory(catStartCal);
	if (!pCalCat) 
		return kFALSE;
	else 
		gHades->getCurrentEvent()->addCategory(catStartCal,pCalCat,"Start");
	}
	pIterCal=(HIterator *)pCalCat->MakeIterator();
	pHitCat=
	   (HLinearCategory *)(gHades->getCurrentEvent()->getCategory(catStartHit));
	if (!pHitCat) {
		pHitCat = new HLinearCategory("HStartHit");
	    if (!pHitCat) 
			return kFALSE;
    	else 
			gHades->getCurrentEvent()->addCategory(catStartHit,pHitCat,"Start");
  	}
	loc.set(1,0);
	fActive=kTRUE;
	return kTRUE;
}


Int_t HStartHitFProton::execute(void) {

	HLocation tmpLoc; 
	HStartCal *pCal=0;
	HStartHit *pHit=0;
	Int_t nStrip=-1;
	Int_t nModule=-1;
	Int_t nStartMult = 0;
	Int_t nType = -1;
	Float_t fTime = -1;

	tmpLoc.set(2,-1,-1);
	pIterCal->Reset();
	while ((pCal=(HStartCal *)pIterCal->Next())!=0) {
		nStartMult = pCal->getMult(3);
		nModule	   = pCal->getModule();
		nStrip     = pCal->getStrip();	
		nType      = pCal->getType();
		if( nStartMult>1 ) return 1;
    	if(nModule == 3) {
			loc[0] = nModule;
			loc[1] = nStrip;
			pHit=(HStartHit *)pHitCat->getNewSlot(loc);
			if(nType==TYPE_LEFT || nType==TYPE_RIGHT || nType==TYPE_MEANTIMER){
				fTime = pCal->getTof();	
			}
			else{
				fTime = pCal->getTime();
			}
			if(pHit){
				pHit = new (pHit) HStartHit;		
				pHit->setModule(nModule);
				pHit->setStrip(nStrip);
				pHit->setTime(fTime);	
				pHit->setStartMult(nStartMult);	
			}
		}
	}
	return 1;
}

Bool_t HStartHitFProton::finalize(void) {
  return kTRUE;
}
void HStartHitFProton::clear(){
	pCalCat=0;
	pHitCat=0;
	pIterCal=0;
}

ClassImp(HStartHitFProton)
