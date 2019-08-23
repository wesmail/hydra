// : Author  B.Spruck

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HHodoGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hhodogo4mon.h"
#include "hhodohistfiller.h"
#include "hhodohistbooker.h"

#include "hododef.h"
#include "hcategory.h"
#include "hhodoraw.h"

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
#include <TIterator.h>

ClassImp(HHodoGo4Mon)

HHodoHistBooker* HHodoGo4Mon::hodoHistbooker=0;

HHodoGo4Mon::HHodoGo4Mon(void) {
	// Default constructor .
	initVariables();
}

HHodoGo4Mon::HHodoGo4Mon(Text_t* name,Text_t* title)
:  HReconstructor(name,title) {
	// Constructor calls the constructor of class HReconstructor with the name
	// and the title as arguments.
	initVariables();
}

HHodoGo4Mon::~HHodoGo4Mon(void) {
	// destructor deletes the iterator
	if (iterraw) delete iterraw;
	iterraw=0;
}
void HHodoGo4Mon::initVariables()
{
	// iterator+categories
	rawCat =0;
	iterraw=0;	
}
void HHodoGo4Mon::printStatus(){
	cout << "************* HHodoGo4Mon **************" << endl;
	cout << "******************************************" << endl;
}
void HHodoGo4Mon::setParContainers() {
	
	hodoHistbooker = HHodoHistBooker::getExObject();
	if(!hodoHistbooker)
	{
      Error("HHodoGo4Mon::setParContainers()","ZERO POINTER FOR HHODOTHISTBOOKER RECEIVED!");
      exit(1);
	}
}
Bool_t HHodoGo4Mon::init(void) {
	
	Bool_t test=kFALSE;
	setParContainers();
	test=getCategories();
	printStatus();
	
	fActive=kTRUE;
	return test;
}
Bool_t HHodoGo4Mon::reinit(void)
{
	hodoHistbooker = HHodoHistBooker::getExObject();
	hodoHistbooker->printStatus();
	fActive=kTRUE;
	return kTRUE;
}
Bool_t HHodoGo4Mon::getCategories()
{
	
	rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catHodoRaw));
	if (!rawCat) {
		Warning("HHodoGo4Mon:init()","CATEGORY HHODORAW IS NOT FILLED!");
	}
	
	if (rawCat)
	{
		iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
	}
	
	calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catHodoCal));
	if (!calCat) {
		Warning("HHodoGo4Mon:init()","CATEGORY HHODOCAL IS NOT FILLED!");
	}
	
	if (calCat)
	{
		itercal =(HIterator *)((HCategory*)calCat)->MakeIterator("native");
	}
	
	hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catHodoHit));
	if (!hitCat) {
		Warning("HHodoGo4Mon:init()","CATEGORY HHODOHIT IS NOT FILLED!");
	}
	
	if (hitCat)
	{
		iterhit =(HIterator *)((HCategory*)hitCat)->MakeIterator("native");
	}
	return kTRUE;
}
Int_t HHodoGo4Mon::execute()
{
	//hodoHistbooker->resetActiveList(2,gHades->getEventCounter());  // if feature of incremental update should be used
	HHodoRaw* raw=0;
	HHodoCal* cal=0;
	HHodoHit* hit=0;
	
	if(rawCat)
	{
		hodoHistbooker->getHodoFiller()->fillRawCat(iterraw);
		
		iterraw->Reset();
		//########################## Fill RAW HISTS ##################################
		while ((raw=(HHodoRaw *)iterraw->Next())!=0) {
			hodoHistbooker->getHodoFiller()->fillRaw(raw);
		}
	}else{
//	cout << "------ No HodoRaw	 Cat!" << endl;
	}
	if(calCat)
	{
		itercal->Reset();
		//########################## Fill RAW HISTS ##################################
		while ((cal=(HHodoCal *)itercal->Next())!=0) {
			hodoHistbooker->getHodoFiller()->fillCal(cal);
		}
	}else{
//	cout << "------ No HodoCal	 Cat!" << endl;
	}
	if(hitCat)
	{
		iterhit->Reset();
		//########################## Fill RAW HISTS ##################################
		while ((hit=(HHodoHit *)iterhit->Next())!=0) {
			hodoHistbooker->getHodoFiller()->fillHit(hit);
		}
	}else{
//	cout << "------ No HodoHit	 Cat!" << endl;
	}
	return 0;
}
Bool_t HHodoGo4Mon::finalize()
{
	return kTRUE;
}

