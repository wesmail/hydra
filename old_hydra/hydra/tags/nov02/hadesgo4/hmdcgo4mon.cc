// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HMdcGo4Mon
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcgo4mon.h"
#include "hmdchistfiller.h"
#include "hmdchistbooker.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdchit.h"
#include "hmdccal1.h"
#include "hmdcraw.h"
#include "hmdcraweventheader.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"

#include <iostream.h>
#include <stdlib.h>
#include <TIterator.h>

ClassImp(HMdcGo4Mon)

HMdcHistBooker* HMdcGo4Mon::Histbooker=0;

HMdcGo4Mon::HMdcGo4Mon(void) {
  // Default constructor calls the function setParContainers().
  initVariables();
}

HMdcGo4Mon::HMdcGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments. It also calls the function setParContainers().
  initVariables();
}

HMdcGo4Mon::~HMdcGo4Mon(void) {
  // destructor deletes the iterator
  if (iterhit)  delete iterhit;
  if (itercal1) delete itercal1;
  if (iterraw)  delete iterraw;
  if (iterrawHeader)  delete iterrawHeader;
  iterhit=0;
  itercal1=0;
  iterraw=0;
  iterrawHeader=0;
}
void HMdcGo4Mon::initVariables()
{
    // iterator+categories
    hitCat=0;
    calCat=0;
    rawCat=0;
    rawCatHeader=0;
    iterhit=0;
    itercal1=0;
    iterraw=0;
    iterrawHeader=0;
}
void HMdcGo4Mon::printStatus(){
    cout << "************* HMdcGo4Mon *****************" << endl;
    cout << "******************************************" << endl;
}
void HMdcGo4Mon::setParContainers() {
    // creates the parameter containers MdcLookupGeom/MdcLookupRaw/MdcTimeCut if they do not
    // exist and adds them to the list of parameter containers in the runtime
    // database
    Histbooker = HMdcHistBooker::getExObject();
    if(!Histbooker)
    {
      Error("HMdcGo4Mon::setParContainers()","ZERO POINTER FOR HMDCHISTBOOKER RECEIVED!");
      exit(1);
    }
    HMdcLookupGeom* lookup=0;
    lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
    if(!lookup){Error("HMdcConnectedTdc::fill()","HMdcLookupGeom not initialized!");exit(1);}

    HMdcLookupRaw* lookupraw=0;
    lookupraw=(HMdcLookupRaw*)gHades->getRuntimeDb()->getContainer("MdcLookupRaw");
    if(!lookupraw){Error("HMdcConnectedTdc::fill()","HMdcLookupRaw not initialized!");exit(1);}

    HMdcRawStruct* rawstru=0;
    rawstru=(HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
    if(!rawstru){Error("HMdcConnectedTdc::fill()","HMdcRawStruct not initialized!");exit(1);}

    HMdcGeomStruct* geomstru=0;
    geomstru=(HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
    if(!geomstru){Error("HMdcConnectedTdc::fill()","HMdcGeomStruct not initialized!");exit(1);}

}
Bool_t HMdcGo4Mon::init(void) {
  // creates the MdcRaw  and MdcCal1 categories and adds them to the current
  // event
  // creates an iterator which loops over all fired cells
  // calls the function setParContainers()
  Bool_t test=kFALSE;
  setParContainers();
  test=getCategories();
  printStatus();
  
  fActive=kTRUE;
  return test;
}
Bool_t HMdcGo4Mon::reinit(void)
{
    Histbooker = HMdcHistBooker::getExObject();
    Histbooker->printStatus();
    Histbooker->getMdcFiller()->getMdcLookup()->fill();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HMdcGo4Mon::getCategories()
{
    rawCatHeader=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcRawEventHeader));
    if (!rawCatHeader) {
	Warning("HMdcGo4Mon:init()","CATEGORY HMDCRAWEVENTHEADER IS NOT FILLED!");
    }
    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcRaw));
    if (!rawCat) {
	Warning("HMdcGo4Mon:init()","CATEGORY HMDCRAW IS NOT FILLED!");
    }
    calCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1));
    if (!calCat) {
	Warning("HMdcGo4Mon:init()","CATEGORY HMDCCAL1 IS NOT FILLED!");
    }
    hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcHit));
    if (!hitCat) {
	Warning("HMdcGo4Mon:init()","CATEGORY HMDCHIT IS NOT FILLED!");
    }
    if (rawCatHeader)
    {
	iterrawHeader =(HIterator *)((HCategory*)rawCatHeader)->MakeIterator("native");
    }
    if (rawCat)
    {
	iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
    }
    if (calCat)
    {
	itercal1=(HIterator *)((HCategory*)calCat)->MakeIterator("native");
    }
    if (hitCat)
    {
	iterhit=(HIterator *)((HCategory*)hitCat)->MakeIterator("native");
    }
    return kTRUE;
}
Int_t HMdcGo4Mon::execute()
{
    Histbooker->resetActiveList(2,gHades->getEventCounter()-1);
    HMdcCal1* cal=0;
    HMdcRaw* raw=0;
    HMdcRawEventHeader* rawHeader=0;
    if(rawCatHeader)
    {
	iterrawHeader->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((rawHeader=(HMdcRawEventHeader *)iterrawHeader->Next())!=0) {
	    Histbooker->getMdcFiller()->fillRawHeader(rawHeader);
	}
    }

    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
	while ((raw=(HMdcRaw *)iterraw->Next())!=0) {
	    Histbooker->getMdcFiller()->fillRaw(raw);
	}
    }
    if(calCat)
    {
        itercal1->Reset();
	//######################### FIll CAL1 HISTS ##################################
	while ((cal=(HMdcCal1 *)itercal1->Next())!=0) {
	    Histbooker->getMdcFiller()->fillCal1(cal);
	}
	//############################################################################
    }
    if(hitCat)
    {
        iterhit->Reset();
	//######################### FIll HIT HISTS ##################################
	//while ((cal=(HMdcHit *)iterhit->Next())!=0) {
	    //Histbooker->getMdcFiller()->fillCal1(cal);
	//}
	//############################################################################
    }
    return 0;
}
Bool_t HMdcGo4Mon::finalize()
{
    return kTRUE;
}

