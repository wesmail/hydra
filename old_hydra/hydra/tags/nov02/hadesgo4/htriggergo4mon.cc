// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HMdcGo4Mon
//
///////////////////////////////////////////////////////////////////////////////

#include "htriggergo4mon.h"
#include "htriggerhistfiller.h"
#include "htriggerhistbooker.h"

#include "triggerinfodef.h"
#include "hcategory.h"
#include "hmatchurich.h"
#include "hmatchutof.h"
#include "hmatchushower.h"
#include "hmuleptons.h"

#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "heventheader.h"

#include <iostream.h>
#include <stdlib.h>
#include <TIterator.h>

HTriggerGo4Mon* HTriggerGo4Mon::pointMe=0;

ClassImp(HTriggerGo4Mon)

HTriggerHistBooker* HTriggerGo4Mon::triggerHistbooker=0;

HTriggerGo4Mon::HTriggerGo4Mon(void) {
  // Default constructor .
  initVariables();
}

HTriggerGo4Mon::HTriggerGo4Mon(Text_t* name,Text_t* title)
:  HReconstructor(name,title) {
    // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initVariables();
}

HTriggerGo4Mon::~HTriggerGo4Mon(void) {
  // destructor deletes the iterator
  if (iterrich)   delete iterrich;
  if (itertof)    delete itertof;
  if (itershower) delete itershower;
  if (itermatchu) delete itermatchu;
  iterrich  =0;
  itertof   =0;
  itershower=0;
  itermatchu=0;
  pointMe   =0;
}
void HTriggerGo4Mon::initVariables()
{
    // iterator+categories
    richCat   =0;
    tofCat    =0;
    showerCat =0;
    iterrich  =0;
    itertof   =0;
    itershower=0;
    itermatchu=0;
    pointMe      =this;
    useDownscaled=kTRUE;
}
void HTriggerGo4Mon::printStatus(){
    cout << "************* HTriggerGo4Mon ***************" << endl;
    cout << "********************************************" << endl;
}
void HTriggerGo4Mon::setParContainers() {

    triggerHistbooker = HTriggerHistBooker::getExObject();
    if(!triggerHistbooker)
    {
      Error("HTriggerGo4Mon::setParContainers()","ZERO POINTER FOR HTRIGGERHISTBOOKER RECEIVED!");
      exit(1);
    }
}
Bool_t HTriggerGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HTriggerGo4Mon::reinit(void)
{
    triggerHistbooker = HTriggerHistBooker::getExObject();
    triggerHistbooker->printStatus();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HTriggerGo4Mon::getCategories()
{

  richCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchURich));
  if (!richCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGERRICH IS NOT FILLED!");
    }
  tofCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchUTof));
  if (!tofCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGERTOF IS NOT FILLED!");
    }
  showerCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchUShower));
  if (!showerCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGERSHOWER IS NOT FILLED!");
    }
  matchuCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchULeptons));
  if (!matchuCat) 
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGERMULEPTONS IS NOT FILLED!");
    }
  if (richCat)
    {
      iterrich =(HIterator *)((HCategory*)richCat)->MakeIterator("native");
    }
  if (tofCat)
    {
      itertof=(HIterator *)((HCategory*)tofCat)->MakeIterator("native");
    }
  if (showerCat)
    {
      itershower =(HIterator *)((HCategory*)showerCat)->MakeIterator("native");
    }
  if (matchuCat)
    {
      itermatchu =(HIterator *)((HCategory*)matchuCat)->MakeIterator("native");
    }
  return kTRUE;
}

Int_t HTriggerGo4Mon::execute()
{
  //triggerHistbooker->resetActiveList(2,gHades->getEventCounter()); // if feature of incremental update should be used

  //cout << "downscaled: " << useDownscaled << endl;
  if( (useDownscaled==kTRUE) && (gHades->getCurrentEvent()->getHeader()->getDownscalingFlag()==0) )
    {
      return 0;
    }

  if(richCat)
    {
      iterrich->Reset();
      //########################## Fill RICH HISTS ##################################
      triggerHistbooker->getTriggerFiller()->fillRich(iterrich);
    }
  if(tofCat)
    {
      itertof->Reset();
      //######################### FIll TOF HISTS ##################################
      triggerHistbooker->getTriggerFiller()->fillTof(itertof);
      //############################################################################
    }
  if(showerCat)
    {
      itershower->Reset();
      //########################## Fill SHOWER HISTS ##################################
      triggerHistbooker->getTriggerFiller()->fillShower(itershower);
    }
  if(matchuCat)
    {
      itermatchu->Reset();
      //########################## Fill SHOWER HISTS ##################################
      triggerHistbooker->getTriggerFiller()->fillMatchU(itermatchu);
    }
    
  return 0;
}
Bool_t HTriggerGo4Mon::finalize()
{
    return kTRUE;
}

HTriggerGo4Mon* HTriggerGo4Mon::getExObject()
{
  return(pointMe);
}
