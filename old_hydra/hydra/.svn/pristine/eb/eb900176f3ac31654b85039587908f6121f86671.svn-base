// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HTofGo4Mon
//
///////////////////////////////////////////////////////////////////////////////

#include "htofgo4mon.h"
#include "htofhistfiller.h"
#include "htofhistbooker.h"

#include "tofdef.h"
#include "hcategory.h"
#include "htofcal.h"
#include "htofraw.h"
#include "htofhit.h"

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

ClassImp(HTofGo4Mon)

HTofHistBooker* HTofGo4Mon::tofHistbooker=0;

HTofGo4Mon::HTofGo4Mon(void) {
  // Default constructor .
  initVariables();
}

HTofGo4Mon::HTofGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initVariables();
}

HTofGo4Mon::~HTofGo4Mon(void) {
  // destructor deletes the iterator
  if (itercal) delete itercal;
  if (iterraw) delete iterraw;
  if (iterhit) delete iterhit;
  itercal=0;
  iterraw=0;
  iterhit=0;
}
void HTofGo4Mon::initVariables()
{
    // iterator+categories
    calCat =0;
    rawCat =0;
    hitCat =0;
    itercal=0;
    iterraw=0;
    iterhit=0;

}
void HTofGo4Mon::printStatus(){
    cout << "************* HTofGo4Mon ***************" << endl;
    cout << "******************************************" << endl;
}
void HTofGo4Mon::setParContainers() {

    tofHistbooker = HTofHistBooker::getExObject();
    if(!tofHistbooker)
    {
      Error("HTofGo4Mon::setParContainers()","ZERO POINTER FOR HTOFTHISTBOOKER RECEIVED!");
      exit(1);
    }
}
Bool_t HTofGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HTofGo4Mon::reinit(void)
{
    tofHistbooker = HTofHistBooker::getExObject();
    tofHistbooker->printStatus();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HTofGo4Mon::getCategories()
{

    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofRaw));
    if (!rawCat) {
	Warning("HTofGo4Mon:init()","CATEGORY HTOFRAW IS NOT FILLED!");
    }

    calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofCal));
    if (!calCat) {
	Warning("HTofGo4Mon:init()","CATEGORY HTOFCAL IS NOT FILLED!");
    }
    hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofHit));
    if (!hitCat) {
	Warning("HTofGo4Mon:init()","CATEGORY HTOFHIT IS NOT FILLED!");
        cout<<"NO HEADER"<<endl;
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
Int_t HTofGo4Mon::execute()
{
    HTofRaw* raw=0;
    HTofCal* cal=0;
    HTofHit* hit=0;
 
    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((raw=(HTofRaw *)iterraw->Next())!=0) {
	    tofHistbooker->getTofFiller()->fillRaw(raw);
	}
    }
    if(calCat)
    {
        itercal->Reset();
	//######################### FIll CAL HISTS ##################################
	while ((cal=(HTofCal *)itercal->Next())!=0) {
	    tofHistbooker->getTofFiller()->fillCal(cal);
	}
	//############################################################################
    }
    if(hitCat)
    {
	iterhit->Reset();
	//########################## Fill HIT HISTS ##################################
	while ((hit=(HTofHit *)iterhit->Next())!=0) {
	    tofHistbooker->getTofFiller()->fillHit(hit);
	}
    }
    return 0;
}
Bool_t HTofGo4Mon::finalize()
{
    return kTRUE;
}

