// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HTofinoGo4Mon
//
///////////////////////////////////////////////////////////////////////////////

#include "htofinogo4mon.h"
#include "htofinohistfiller.h"
#include "htofinohistbooker.h"

#include "tofinodef.h"
#include "hcategory.h"
#include "htofinocal.h"
#include "htofinoraw.h"

#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"

#include <iostream.h>
#include <stdlib.h>
#include <TIterator.h>

ClassImp(HTofinoGo4Mon)

HTofinoHistBooker* HTofinoGo4Mon::tofHistbooker=0;

HTofinoGo4Mon::HTofinoGo4Mon(void) {
  // Default constructor .
  initVariables();
}

HTofinoGo4Mon::HTofinoGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initVariables();
}

HTofinoGo4Mon::~HTofinoGo4Mon(void) {
  // destructor deletes the iterator
  if (itercal) delete itercal;
  if (iterraw) delete iterraw;
  itercal=0;
  iterraw=0;
}
void HTofinoGo4Mon::initVariables()
{
    // iterator+categories
    calCat =0;
    rawCat =0;
    itercal=0;
    iterraw=0;

}
void HTofinoGo4Mon::printStatus(){
    cout << "************* HTofinoGo4Mon **************" << endl;
    cout << "******************************************" << endl;
}
void HTofinoGo4Mon::setParContainers() {

    tofHistbooker = HTofinoHistBooker::getExObject();
    if(!tofHistbooker)
    {
      Error("HTofinoGo4Mon::setParContainers()","ZERO POINTER FOR HTOFINOTHISTBOOKER RECEIVED!");
      exit(1);
    }
}
Bool_t HTofinoGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HTofinoGo4Mon::reinit(void)
{
    tofHistbooker = HTofinoHistBooker::getExObject();
    tofHistbooker->printStatus();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HTofinoGo4Mon::getCategories()
{

    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofinoRaw));
    if (!rawCat) {
	Warning("HTofinoGo4Mon:init()","CATEGORY HTOFINORAW IS NOT FILLED!");
    }

    calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofinoCal));
    if (!calCat) {
	Warning("HTofinoGo4Mon:init()","CATEGORY HTOFINOCAL IS NOT FILLED!");
    }
    if (rawCat)
    {
	iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
    }
    if (calCat)
    {
	itercal=(HIterator *)((HCategory*)calCat)->MakeIterator("native");
    }
    return kTRUE;
}
Int_t HTofinoGo4Mon::execute()
{
    HTofinoRaw* raw=0;
    HTofinoCal* cal=0;
 
    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((raw=(HTofinoRaw *)iterraw->Next())!=0) {
	    tofHistbooker->getTofinoFiller()->fillRaw(raw);
	}
    }
    if(calCat)
    {
        itercal->Reset();
	//######################### FIll CAL HISTS ##################################
	while ((cal=(HTofinoCal *)itercal->Next())!=0) {
	    tofHistbooker->getTofinoFiller()->fillCal(cal);
	}
	//############################################################################
    }
    return 0;
}
Bool_t HTofinoGo4Mon::finalize()
{
    return kTRUE;
}

