// : Author  B.Spruck

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HWallGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hwallgo4mon.h"
#include "hwallhistfiller.h"
#include "hwallhistbooker.h"

#include "walldef.h"
#include "hcategory.h"
#include "hwallraw.h"

#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"

#include <iostream> 
#include <iomanip>
#include <stdlib.h>
#include "TIterator.h"

ClassImp(HWallGo4Mon)

HWallHistBooker* HWallGo4Mon::wallHistbooker=0;

HWallGo4Mon::HWallGo4Mon(void) {
	// Default constructor .
	initVariables();
}

HWallGo4Mon::HWallGo4Mon(Text_t* name,Text_t* title)
:  HReconstructor(name,title) {
	// Constructor calls the constructor of class HReconstructor with the name
	// and the title as arguments.
	initVariables();
}

HWallGo4Mon::~HWallGo4Mon(void) {
	// destructor deletes the iterator
	if (iterraw) delete iterraw;
	iterraw=0;
	if (iterhit) delete iterhit;
	iterhit=0;
}
void HWallGo4Mon::initVariables()
{
	// iterator+categories
	rawCat =0;
	iterraw=0;	
	hitCat =0;
	iterhit=0;	
}
void HWallGo4Mon::printStatus(){
	cout << "************* HWallGo4Mon **************" << endl;
	cout << "******************************************" << endl;
}
void HWallGo4Mon::setParContainers() {
	
	wallHistbooker = HWallHistBooker::getExObject();
	if(!wallHistbooker)
	{
      Error("HWallGo4Mon::setParContainers()","ZERO POINTER FOR HWALLTHISTBOOKER RECEIVED!");
      exit(1);
	}
}
Bool_t HWallGo4Mon::init(void) {
	
	Bool_t test=kFALSE;
	setParContainers();
	test=getCategories();
	printStatus();
	
	fActive=kTRUE;
	return test;
}
Bool_t HWallGo4Mon::reinit(void)
{
	wallHistbooker = HWallHistBooker::getExObject();
	wallHistbooker->printStatus();
	fActive=kTRUE;
	return kTRUE;
}
Bool_t HWallGo4Mon::getCategories()
{
	
	rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catWallRaw));
	if (!rawCat) {
		Warning("HWallGo4Mon:init()","CATEGORY HWALLRAW IS NOT FILLED!");
	}
	
	if (rawCat)
	{
		iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
	}
	
	hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catWallHit));
	if (!hitCat) {
		Warning("HWallGo4Mon:init()","CATEGORY HWALLHIT IS NOT FILLED!");
	}
	
	if (hitCat)
	{
		iterhit =(HIterator *)((HCategory*)hitCat)->MakeIterator("native");
	}
	return kTRUE;
}
Int_t HWallGo4Mon::execute()
{
	//wallHistbooker->resetActiveList(2,gHades->getEventCounter());  // if feature of incremental update should be used
	HWallRaw* raw=0;
	HWallHit* hit=0;

	if(rawCat)
	{
		wallHistbooker->getWallFiller()->fillRawCat(iterraw);
		
		iterraw->Reset();
		//########################## Fill RAW HISTS ##################################
		while ((raw=(HWallRaw *)iterraw->Next())!=0) {
			wallHistbooker->getWallFiller()->fillRaw(raw);
		}
	}else{
	cout << "------- No WallRaw Cat!"<<endl;
	}
    if(hitCat)
    {
		iterhit->Reset();
		//########################## Fill HIT HISTS ################################
		while ((hit=(HWallHit *)iterhit->Next())!=0) {
			wallHistbooker->getWallFiller()->fillHit(hit);
		}
    }
	return 0;
}
Bool_t HWallGo4Mon::finalize()
{
	return kTRUE;
}

