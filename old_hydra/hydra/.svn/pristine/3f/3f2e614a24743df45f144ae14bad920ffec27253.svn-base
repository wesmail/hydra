// : Author  J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "htriggergo4mon.h"
#include "htriggerhistfiller.h"
#include "htriggerhistbooker.h"

#include "triggerinfodef.h"
#include "hcategory.h"
#include "hmatchurich.h"
#include "hmatchutof.h"
#include "hmatchushower.h"
#include "hmuleptons.h"
#include "hrichhitIPU.h"

#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "heventheader.h"
#include "showerdef.h"
#include "tofdef.h"
#include "richdef.h"
#include "htriggerparshowermap.h"

#include <iostream> 

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
  if (itermuemu) delete itermuemu;
  if (iterrichhit) delete iterrichhit;
  if (itertofhit) delete itertofhit;
  if (itershowerhit) delete itershowerhit;
  iterrich  =0;
  itertof   =0;
  itershower=0;
  itermatchu=0;
  itermuemu=0;
  iterrichhit=0;
  itertofhit=0;
  itershowerhit=0;
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
    itermuemu=0;
    iterrichhit=0;
    itertofhit=0;
    itershowerhit=0;
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
    if((showerMap=(HTriggerParShowerMap*)gHades->getRuntimeDb()->getContainer("TriggerParShowerMap"))==NULL)
    {
      Error("setParContainers()","HTriggerParShowerMap container not found!");
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
  richCatHit=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catHardRichHit));
  if (!richCatHit)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HHARDRICHHIT IS NOT FILLED!");
    }
  tofCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchUTof));
  if (!tofCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGERTOF IS NOT FILLED!");
    }
  tofCatHit = (HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catTofHit));
  if(!tofCatHit)
  {
    Warning("HTriggerGo4Mon:init()","CATEGORY HTOFHIT IS NOT FILLED!");
  }
  showerCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchUShower));
  if (!showerCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGERSHOWER IS NOT FILLED!");
    }
  showerCatHit=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catShowerHitIPU));
  if (!showerCatHit)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HSHOWERHIT IS NOT FILLED!");
    }
  matchuCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMatchULeptons));
  if (!matchuCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGER MULEPTONS IS NOT FILLED!");
    }
  muemuCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMUEMULeptons));
  if (!muemuCat)
    {
      Warning("HTriggerGo4Mon:init()","CATEGORY HTRIGGER MUEMULeptons IS NOT FILLED!");
    }
  if (richCat)
    {
      iterrich =(HIterator *)((HCategory*)richCat)->MakeIterator("native");
    }
  if(richCatHit)
  {
    iterrichhit =(HIterator *)((HCategory*)richCatHit)->MakeIterator("native");
  }
  if (tofCat)
    {
      itertof=(HIterator *)((HCategory*)tofCat)->MakeIterator("native");
    }
  if(tofCatHit)
  {
    itertofhit =(HIterator *)((HCategory*)tofCatHit)->MakeIterator("native");
  }
  if (showerCat)
    {
      itershower =(HIterator *)((HCategory*)showerCat)->MakeIterator("native");
    }
  if (showerCatHit)
    {
      itershowerhit =(HIterator *)((HCategory*)showerCatHit)->MakeIterator("native");
    }
  if (matchuCat)
    {
      itermatchu =(HIterator *)((HCategory*)matchuCat)->MakeIterator("native");
    }
  if (muemuCat)
    {
      itermuemu =(HIterator *)((HCategory*)muemuCat)->MakeIterator("native");
    }
  return kTRUE;
}

Int_t HTriggerGo4Mon::execute()
{
  //triggerHistbooker->resetActiveList(2,gHades->getEventCounter()); // if feature of incremental update should be used
  UInt_t daqTime     = ( gHades->getCurrentEvent()->getHeader()->getTime() );
  UInt_t daqHour     = ( (daqTime & 0x00ff0000) >> 16 );
  UInt_t daqMin      = ( (daqTime & 0x0000ff00) >>  8 );
  UInt_t daqSec      =    daqTime & 0x000000ff;
  UInt_t currentTime = 3600*daqHour + 60*daqMin + daqSec;

  //cout << "downscaled: " << useDownscaled << endl;
  if( (useDownscaled!=0) )
    {
      if( (useDownscaled==1) && (gHades->getCurrentEvent()->getHeader()->getDownscalingFlag()==kFALSE) )
				{
					return(0);
				}
      else if( (useDownscaled==2) && (gHades->getCurrentEvent()->getHeader()->getDownscalingFlag()==kTRUE) )
				{
					return(0);
				}
    }

  if(1) // maybe i use this later...
    {
      triggerHistbooker->getTriggerFiller()->fillTime( currentTime );
    }
  if(richCat && iterrich)
    {
      //########################## Fill RICH HISTS ##################################
      iterrich->Reset();
      triggerHistbooker->getTriggerFiller()->fillRich(iterrich);
      //########################## Fill RICH Time HISTS #############################
      iterrich->Reset();
      triggerHistbooker->getTriggerFiller()->fillRichTime(iterrich, currentTime);

      if( iterrichhit)
      {
        //######################### Fill Rich correlation ################################
        iterrich->Reset();
        iterrichhit->Reset();
        triggerHistbooker->getTriggerFiller()->fillRichCorrelation(iterrich,iterrichhit);
      }
    }
  if(tofCat && itertof)
    {
      //######################### FIll TOF HISTS ##################################
      itertof->Reset();
      triggerHistbooker->getTriggerFiller()->fillTof(itertof);
      //########################## Fill TOF Time HISTS #############################
      itertof->Reset();
      triggerHistbooker->getTriggerFiller()->fillTofTime(itertof, currentTime);
      if( itertofhit)
      {
        //######################### Fill TOF correlation ################################
        itertof->Reset();
        itertofhit->Reset();
        triggerHistbooker->getTriggerFiller()->fillTofCorrelation(itertof,itertofhit);
      }
    }
  if(showerCat && itershower)
    {
      //########################## Fill SHOWER HISTS ##################################
      itershower->Reset();
      triggerHistbooker->getTriggerFiller()->fillShower(itershower);
      //########################## Fill Shower Time HISTS #############################
      itershower->Reset();
      triggerHistbooker->getTriggerFiller()->fillShowerTime(itershower, currentTime);
      if(itershowerhit)
      {
        // ########################## Fill SHOWER correlation ##############################
        itershower->Reset();
        itershowerhit->Reset();
        triggerHistbooker->getTriggerFiller()->fillShowerCorrelation(itershower,itershowerhit,showerMap);
      }
    }
  if(matchuCat && itermatchu)
    {
      //########################## Fill MatchU HISTS ##################################
      itermatchu->Reset();
      triggerHistbooker->getTriggerFiller()->fillMatchU(itermatchu);
      //########################## Fill MatchU Time HISTS #############################
      itermatchu->Reset();
      triggerHistbooker->getTriggerFiller()->fillMatchUTime(itermatchu, currentTime);

       if(iterrich && itertof && itershower && itermuemu)
       {
        // ########################## Fill SHOWER correlation ##############################
	iterrich->Reset();
	itertof->Reset();
	itershower->Reset();
	itermatchu->Reset();
	itermuemu->Reset();

        triggerHistbooker->getTriggerFiller()->fillMatchUCorrelation(
         iterrich,itertof,itershower,
         itermatchu,itermuemu
         );
       }
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
