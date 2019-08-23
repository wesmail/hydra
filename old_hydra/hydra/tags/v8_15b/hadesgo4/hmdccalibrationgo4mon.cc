// : Author  J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcCalibrationGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdccalibrationgo4mon.h"
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
#include "hmdcraw.h"
#include "hmdcraweventheader.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"

#include <iostream> 
#include <iomanip>
#include <stdlib.h>
#include <TIterator.h>

ClassImp(HMdcCalibrationGo4Mon)

HMdcHistBooker* HMdcCalibrationGo4Mon::Histbooker=0;

HMdcCalibrationGo4Mon::HMdcCalibrationGo4Mon(void) {
  // Default constructor calls the function setParContainers().
  initVariables();
}

HMdcCalibrationGo4Mon::HMdcCalibrationGo4Mon(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments. It also calls the function setParContainers().
  initVariables();
}

HMdcCalibrationGo4Mon::~HMdcCalibrationGo4Mon(void) {
  // destructor deletes the iterator
  if (iterraw)  delete iterraw;
  if (iterrawHeader)  delete iterrawHeader;
  iterraw=0;
  iterrawHeader=0;
}
void HMdcCalibrationGo4Mon::initVariables()
{
    // iterator+categories
    rawCat=0;
    rawCatHeader=0;
    iterraw=0;
    iterrawHeader=0;

}
void HMdcCalibrationGo4Mon::printStatus(){
    cout << "************* HMdcCalibrationGo4Mon ******" << endl;
    cout << "******************************************" << endl;
}
void HMdcCalibrationGo4Mon::setParContainers() {
    // creates the parameter containers MdcLookupGeom/MdcLookupRaw/MdcTimeCut if they do not
    // exist and adds them to the list of parameter containers in the runtime
    // database
    Histbooker = HMdcHistBooker::getExObject();
    if(!Histbooker)
    {
      Error("HMdcCalibrationGo4Mon::setParContainers()","ZERO POINTER FOR HMDCHISTBOOKER RECEIVED!");
      exit(1);
    }

}
Bool_t HMdcCalibrationGo4Mon::init(void) {
  // creates the MdcRaw  category and adds them to the current
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
Bool_t HMdcCalibrationGo4Mon::reinit(void)
{
    Histbooker = HMdcHistBooker::getExObject();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HMdcCalibrationGo4Mon::getCategories()
{
    /*
    rawCatHeader=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcRawEventHeader));
    if (!rawCatHeader) {
	Warning("HMdcCalibrationGo4Mon:init()","CATEGORY HMDCRAWEVENTHEADER IS NOT FILLED!");
    }*/
    rawCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcRaw));
    if (!rawCat) {
	Warning("HMdcCalibrationGo4Mon:init()","CATEGORY HMDCRAW IS NOT FILLED!");
    }
    if (rawCatHeader)
    {
	iterrawHeader =(HIterator *)((HCategory*)rawCatHeader)->MakeIterator("native");
    }
    if (rawCat)
    {
	iterraw =(HIterator *)((HCategory*)rawCat)->MakeIterator("native");
    }
    return kTRUE;
}
Int_t HMdcCalibrationGo4Mon::execute()
{
    if(rawCat)
    {
	iterraw->Reset();
	//########################## Fill RAW HISTS ##################################
        Histbooker->getMdcFiller()->fillCalibration(iterraw);
    }
    return 0;
}
Bool_t HMdcCalibrationGo4Mon::finalize()
{
    return kTRUE;
}

