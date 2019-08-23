// : Author  J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HTofGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "htofgo4mon.h"
#include "htofhistfiller.h"
#include "htofhistbooker.h"

#include "tofinodef.h"
#include "htofinoraw.h"

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

#include <iostream> 
#include <iomanip>
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
  if (iterraw) delete iterraw;
  if (iterhit) delete iterhit;
  if (itertofinoraw) delete itertofinoraw;
  iterraw=0;
  iterhit=0;
  itertofinoraw=0;

}
void HTofGo4Mon::initVariables()
{
    // iterator+categories
    rawCat =0;
    hitCat =0;
    tofinorawCat =0;

    iterraw=0;
    iterhit=0;
    itertofinoraw=0;

}
void HTofGo4Mon::printStatus(){
    cout << "************* HTofGo4Mon *****************" << endl;
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

    hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofHit));
    if (!hitCat) {
	Warning("HTofGo4Mon:init()","CATEGORY HTOFHIT IS NOT FILLED!");
    }

    tofinorawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofinoRaw));
    if (!tofinorawCat) {
	Warning("HTofinoGo4Mon:init()","CATEGORY HTOFINORAW IS NOT FILLED!");
    }

    if (rawCat)
    {
	iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
    }
    if (hitCat)
    {
	iterhit =(HIterator *)((HCategory*)hitCat)->MakeIterator("native");
    }
    if (tofinorawCat)
    {
	itertofinoraw =(HIterator *)((HCategory*)tofinorawCat)->MakeIterator("native");
    }

    return kTRUE;
}
Int_t HTofGo4Mon::execute()
{
    //tofHistbooker->resetActiveList(2,gHades->getEventCounter());  // if feature of incremental update should be used
    HTofRaw* raw=0;
    HTofHit* hit=0;
    HTofinoRaw* tofinoraw=0;

    Int_t tofinorawmult=0;
    Int_t rawmult=0;

    if(tofinorawCat)
    {
	itertofinoraw->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((tofinoraw=(HTofinoRaw *)itertofinoraw->Next())!=0) {
            if(tofinoraw->getTime()>0 && tofinoraw->getTime()<4096)tofinorawmult++;
	}
    }
 
    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((raw=(HTofRaw *)iterraw->Next())!=0) {
	    tofHistbooker->getTofFiller()->fillRaw(raw);
            if(raw->getRightTime()>0)rawmult++;
            if(raw->getLeftTime() >0)rawmult++;
	}
	tofHistbooker->getTofFiller()->fillRawMult((Int_t)(rawmult/2),tofinorawmult);
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

