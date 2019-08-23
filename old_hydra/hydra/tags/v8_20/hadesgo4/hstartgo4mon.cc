// : Author  J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hstartgo4mon.h"
#include "hstarthistfiller.h"
#include "hstarthistbooker.h"

#include "hstartdef.h"
#include "hcategory.h"
#include "hstartcal.h"
#include "hstartraw.h"
#include "hstarthit.h"

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

ClassImp(HStartGo4Mon)

HStartHistBooker* HStartGo4Mon::startHistbooker=0;

HStartGo4Mon::HStartGo4Mon(void) {
  // Default constructor .
  initVariables();
}

HStartGo4Mon::HStartGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initVariables();

}

HStartGo4Mon::~HStartGo4Mon(void) {
  // destructor deletes the iterator
  if (itercal) delete itercal;
  if (iterraw) delete iterraw;
  if (iterhit) delete iterhit;
  itercal=0;
  iterraw=0;
  iterhit=0;
}
void HStartGo4Mon::initVariables()
{
    // iterator+categories
    calCat =0;
    rawCat =0;
    hitCat =0;
    itercal=0;
    iterraw=0;
    iterhit=0;

}
void HStartGo4Mon::printStatus(){
    cout << "************* HStartGo4Mon ***************" << endl;
    cout << "******************************************" << endl;
}
void HStartGo4Mon::setParContainers() {

    startHistbooker = HStartHistBooker::getExObject();
    if(!startHistbooker)
    {
      Error("HStartGo4Mon::setParContainers()","ZERO POINTER FOR HSTARTHISTBOOKER RECEIVED!");
      exit(1);
    }
}
Bool_t HStartGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HStartGo4Mon::reinit(void)
{
    startHistbooker = HStartHistBooker::getExObject();
    startHistbooker->printStatus();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HStartGo4Mon::getCategories()
{

    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catStartRaw));
    if (!rawCat) {
	Warning("HStartGo4Mon:init()","CATEGORY HSTARTRAW IS NOT FILLED!");
    }

    calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catStartCal));
    if (!calCat) {
	Warning("HStartGo4Mon:init()","CATEGORY HSTARTCAL IS NOT FILLED!");
    }
    hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catStartHit));
    if (!hitCat) {
	Warning("HStartGo4Mon:init()","CATEGORY HSTARTHIT IS NOT FILLED!");
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
Int_t HStartGo4Mon::execute()
{
    //startHistbooker->resetActiveList(2,gHades->getEventCounter()); // if feature of incremental update should be used

    startHistbooker->getStartFiller()->fillLatch();
    startHistbooker->getStartFiller()->fillVertex();

    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
        startHistbooker->getStartFiller()->fillRaw(iterraw);
    }
    if(calCat)
    {
        itercal->Reset();
	//######################### FIll CAL HISTS ##################################
	startHistbooker->getStartFiller()->fillCal(itercal);
	//############################################################################
    }
    if(hitCat)
    {
	iterhit->Reset();
	//########################## Fill HIT HISTS ##################################
	startHistbooker->getStartFiller()->fillHit(iterhit);
    }
    return 0;
}
Bool_t HStartGo4Mon::finalize()
{
    return kTRUE;
}

