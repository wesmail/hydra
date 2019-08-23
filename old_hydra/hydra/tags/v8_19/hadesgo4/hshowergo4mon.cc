// : Author  J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HShowerGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hshowergo4mon.h"
#include "hshowerhistfiller.h"
#include "hshowerhistbooker.h"

#include "showerdef.h"
#include "hcategory.h"
#include "hshowercal.h"
#include "hshowerraw.h"
#include "hshowerhit.h"

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

ClassImp(HShowerGo4Mon)

HShowerHistBooker* HShowerGo4Mon::showerHistbooker=0;

HShowerGo4Mon::HShowerGo4Mon(void) {
  // Default constructor .
  initVariables();
}

HShowerGo4Mon::HShowerGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initVariables();
}

HShowerGo4Mon::~HShowerGo4Mon(void) {
  // destructor deletes the iterator
  if (itercal) delete itercal;
  if (iterraw) delete iterraw;
  if (iterhit) delete iterhit;
  itercal=0;
  iterraw=0;
  iterhit=0;
}
void HShowerGo4Mon::initVariables()
{
    // iterator+categories
    calCat =0;
    rawCat =0;
    hitCat =0;
    itercal=0;
    iterraw=0;
    iterhit=0;

}
void HShowerGo4Mon::printStatus(){
    cout << "************* HShowerGo4Mon ***************" << endl;
    cout << "******************************************" << endl;
}
void HShowerGo4Mon::setParContainers() {

    showerHistbooker = HShowerHistBooker::getExObject();
    if(!showerHistbooker)
    {
      Error("HShowerGo4Mon::setParContainers()","ZERO POINTER FOR HSHOWERHISTBOOKER RECEIVED!");
      exit(1);
    }
}
Bool_t HShowerGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HShowerGo4Mon::reinit(void)
{
    showerHistbooker = HShowerHistBooker::getExObject();
    showerHistbooker->printStatus();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HShowerGo4Mon::getCategories()
{

    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catShowerRaw));
    if (!rawCat) {
	Warning("HShowerGo4Mon:init()","CATEGORY HSHOWERRAW IS NOT FILLED!");
    }

    calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catShowerCal));
    if (!calCat) {
	Warning("HShowerGo4Mon:init()","CATEGORY HSHOWERCAL IS NOT FILLED!");
    }
    hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catShowerHit));
    if (!hitCat) {
	Warning("HShowerGo4Mon:init()","CATEGORY HSHOWERHIT IS NOT FILLED!");
    }

    if (rawCat)
    {
	iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
    }
    if (calCat)
    {
	itercal=(HIterator *)((HCategory*)calCat)->MakeIterator("native");
    }
    if (hitCat)
    {
	iterhit =(HIterator *)((HCategory*)hitCat)->MakeIterator("native");
    }
    return kTRUE;
}
Int_t HShowerGo4Mon::execute()
{
    //showerHistbooker->resetActiveList(2,gHades->getEventCounter());  // if feature of incremental update should be used
    HShowerRaw* raw=0;
    HShowerCal* cal=0;
    HShowerHit* hit=0;
 
    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((raw=(HShowerRaw *)iterraw->Next())!=0) {
	    showerHistbooker->getShowerFiller()->fillRaw(raw);
	}
    }
    if(calCat)
    {
        itercal->Reset();
	//######################### FIll CAL HISTS ##################################
	while ((cal=(HShowerCal *)itercal->Next())!=0) {
	    showerHistbooker->getShowerFiller()->fillCal(cal);
	}
	//############################################################################
    }
    if(hitCat)
    {
	iterhit->Reset();
	//########################## Fill HIT HISTS ##################################
	while ((hit=(HShowerHit *)iterhit->Next())!=0) {
	    showerHistbooker->getShowerFiller()->fillHit(hit);
	}
    }
    return 0;
}
Bool_t HShowerGo4Mon::finalize()
{
    return kTRUE;
}

