// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HRichGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hrichgo4mon.h"
#include "hrichhistfiller.h"
#include "hrichhistbooker.h"

#include "richdef.h"
#include "hcategory.h"
#include "hrichcal.h"
#include "hrichraw.h"
#include "hrichhit.h"
#include "hrichgeometrypar.h"
#include "hrichfancy.h"

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

ClassImp(HRichGo4Mon)

HRichHistBooker* HRichGo4Mon::richHistbooker=0;

HRichGo4Mon::HRichGo4Mon(void) {
  // Default constructor .
  initVariables();
}

HRichGo4Mon::HRichGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initVariables();
}

HRichGo4Mon::~HRichGo4Mon(void) {
  // destructor deletes the iterator
  if (itercal) delete itercal;
  if (iterraw) delete iterraw;
  if (iterhit) delete iterhit;
  itercal=0;
  iterraw=0;
  iterhit=0;
}
void HRichGo4Mon::initVariables()
{
    // iterator+categories
    calCat =0;
    rawCat =0;
    hitCat =0;
    itercal=0;
    iterraw=0;
    iterhit=0;

}
void HRichGo4Mon::printStatus(){
    cout << "************* HRichGo4Mon ***************" << endl;
    cout << "******************************************" << endl;
}
void HRichGo4Mon::setParContainers() {

    richHistbooker = HRichHistBooker::getExObject();
    HRichGeometryPar* fRichGeomPar=0;
    fRichGeomPar=(HRichGeometryPar*)(gHades->getRuntimeDb()->getContainer("RichGeometryParameters"));
    if(!richHistbooker)
    {
      Error("HRichGo4Mon::setParContainers()","ZERO POINTER FOR HSTARTHISTBOOKER RECEIVED!");
      exit(1);
    }
}
Bool_t HRichGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HRichGo4Mon::reinit(void)
{
    richHistbooker = HRichHistBooker::getExObject();
    richHistbooker->printStatus();
    HRichFancy* richfancy = richHistbooker->getRichFiller()->getRichFancy();
    if(richfancy->getKnown()==kFALSE)
      {richfancy->init();}
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HRichGo4Mon::getCategories()
{

    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catRichRaw));
    if (!rawCat) {
	Warning("HRichGo4Mon:init()","CATEGORY HRICHRAW IS NOT FILLED!");
    }

    calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catRichCal));
    if (!calCat) {
	Warning("HRichGo4Mon:init()","CATEGORY HRICHCAL IS NOT FILLED!");
    }
    hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catRichHit));
    if (!hitCat) {
	Warning("HRichGo4Mon:init()","CATEGORY HRICHHIT IS NOT FILLED!");
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
Int_t HRichGo4Mon::execute()
{
    //richHistbooker->resetActiveList(2,gHades->getEventCounter()); // if feature of incremental update should be used
    HRichRaw* raw=0;
    HRichCal* cal=0;
    HRichHit* hit=0;
 
    if(rawCat)
    {
      //Fill every histogram depending on the raw category;
      iterraw->Reset();
      richHistbooker->getRichFiller()->fillRawCategory(iterraw);
      iterraw->Reset();
      //########################## Fill RAW HISTS ##################################
      while ((raw=(HRichRaw *)iterraw->Next())!=0) {
	richHistbooker->getRichFiller()->fillRaw(raw);
      }
    }
    if(calCat)
    {
      itercal->Reset();
      richHistbooker->getRichFiller()->fillCalCategory(itercal);
      
      itercal->Reset();
      //######################### FIll CAL HISTS ##################################
      while ((cal=(HRichCal *)itercal->Next())!=0) {
	richHistbooker->getRichFiller()->fillCal(cal);
      }
      //############################################################################
    }
    if(hitCat)
    {
	iterhit->Reset();
	//########################## Fill HIT HISTS ##################################
	while ((hit=(HRichHit *)iterhit->Next())!=0) {
	    richHistbooker->getRichFiller()->fillHit(hit);
	}
    }
    return 0;
}
Bool_t HRichGo4Mon::finalize()
{
    return kTRUE;
}

