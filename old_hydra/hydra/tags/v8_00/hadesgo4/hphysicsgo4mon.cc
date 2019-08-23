// : Author  J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcGo4Mon
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hphysicsgo4mon.h"
#include "hphysicshistfiller.h"
#include "hphysicshistbooker.h"

#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "kickdef.h"
#include "tofdef.h"
#include "tofinodef.h"
#include "showertofinodef.h"
#include "showerdef.h"
#include "richdef.h"

#include "hcategory.h"

#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"

#include "hmetamatch.h"
#include "hsplinetrack.h"
#include "hmdctrkcand.h"
#include "hmdcseg.h"
#include "hkicktrack.h"
#include "htofhit.h"
#include "hshowerhittof.h"
#include "hrichhit.h"

#include <iostream> 
#include <iomanip>
#include <stdlib.h>
#include <TIterator.h>

ClassImp(HPhysicsGo4Mon)

HPhysicsHistBooker* HPhysicsGo4Mon::physicsHistbooker=0;

HPhysicsGo4Mon::HPhysicsGo4Mon(void) {
    // Default constructor .
    initVariables();
}

HPhysicsGo4Mon::HPhysicsGo4Mon(Text_t* name,Text_t* title)
:  HReconstructor(name,title) {
    // Constructor calls the constructor of class HReconstructor with the name
    // and the title as arguments.
    initVariables();

}

HPhysicsGo4Mon::~HPhysicsGo4Mon(void) {
    // destructor deletes the iterator

    if(iterMdcTrkCand)  {delete iterMdcTrkCand;  iterMdcTrkCand  =0;}
    if(iterMdcSeg)      {delete iterMdcSeg;      iterMdcSeg      =0;}
    if(iterTofHit)      {delete iterTofHit;      iterTofHit      =0;}
    if(iterShowerTofHit){delete iterShowerTofHit;iterShowerTofHit=0;}
    if(iterKickTrack)   {delete iterKickTrack;   iterKickTrack   =0;}
    if(iterSplineTrack) {delete iterSplineTrack; iterSplineTrack =0;}
    if(iterMetaMatch)   {delete iterMetaMatch;   iterMetaMatch   =0;}

}
void HPhysicsGo4Mon::initVariables()
{
    // iterator+categories
    MdcTrkCandCat   =0;
    MdcSegCat       =0;
    TofHitCat       =0;
    ShowerTofHitCat =0;
    KickTrackCat    =0;
    SplineTrackCat  =0;
    MetaMatchCat    =0;
    RichHitCat      =0;
    iterMdcTrkCand  =0;
    iterMdcSeg      =0;
    iterTofHit      =0;
    iterShowerTofHit=0;
    iterKickTrack   =0;
    iterSplineTrack =0;
    iterMetaMatch   =0;
    iterRichHit     =0;
}
void HPhysicsGo4Mon::printStatus(){
    cout << "************* HPhysicsGo4Mon *************" << endl;
    cout << "******************************************" << endl;
}
void HPhysicsGo4Mon::setParContainers() {

    physicsHistbooker = HPhysicsHistBooker::getExObject();
    if(!physicsHistbooker)
    {
	Error("HPhysicsGo4Mon::setParContainers()","ZERO POINTER FOR HPHYSICSHISTBOOKER RECEIVED!");
	exit(1);
    }
}
Bool_t HPhysicsGo4Mon::init(void) {

    Bool_t test=kFALSE;
    setParContainers();
    test=getCategories();
    printStatus();

    fActive=kTRUE;
    return test;
}
Bool_t HPhysicsGo4Mon::reinit(void)
{
    physicsHistbooker = HPhysicsHistBooker::getExObject();
    physicsHistbooker->printStatus();
    fActive=kTRUE;
    return kTRUE;
}
Bool_t HPhysicsGo4Mon::getCategories()
{

    MdcTrkCandCat   =(HCategory*)gHades->getCurrentEvent()->getCategory(catMdcTrkCand);
    if(!catMdcTrkCand)
    {
      Error("getCategories()","NO catMdcTrkCand in input!");
      return kFALSE;
    }
    else
    {
	iterMdcTrkCand=(HIterator*)MdcTrkCandCat->MakeIterator("native");
    }
    MdcSegCat       =(HCategory*)gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if(!MdcSegCat)
    {
      Error("getCategories()","NO catMdcSeg in input!");
      return kFALSE;
    }
    else
    {
	iterMdcSeg=(HIterator*)MdcSegCat->MakeIterator("native");
    }
    TofHitCat       =(HCategory*)gHades->getCurrentEvent()->getCategory(catTofHit);
    if(!TofHitCat)
    {
      Error("getCategories()","NO catTofHit in input!");
      return kFALSE;
    }
    else
    {
	iterTofHit=(HIterator*)TofHitCat->MakeIterator("native");
    }
    ShowerTofHitCat =(HCategory*)gHades->getCurrentEvent()->getCategory(catShowerHitTof);
    if(!ShowerTofHitCat)
    {
      Error("getCategories()","NO catShowerHitTof in input!");
      return kFALSE;
    }
    else
    {
	iterShowerTofHit=(HIterator*)ShowerTofHitCat->MakeIterator("native");
    }

    KickTrackCat =(HCategory*)gHades->getCurrentEvent()->getCategory(catKickTrack);
    if(!KickTrackCat)
    {
      Warning("getCategories()","NO catShowerHitTof in input!");
    }
    else
    {
	iterKickTrack=(HIterator*)KickTrackCat->MakeIterator("native");
    }

    SplineTrackCat =(HCategory*)gHades->getCurrentEvent()->getCategory(catSplineTrack);
    if(!SplineTrackCat)
    {
      Warning("getCategories()","NO catShowerHitTof in input!");
    }
    else
    {
	iterSplineTrack=(HIterator*)SplineTrackCat->MakeIterator("native");
    }
    MetaMatchCat =(HCategory*)gHades->getCurrentEvent()->getCategory(catMetaMatch);
    if(!MetaMatchCat)
    {
      Warning("getCategories()","NO catMetaMatch in input!");
    }
    else
    {
	iterMetaMatch=(HIterator*)MetaMatchCat->MakeIterator("native");
    }
    RichHitCat =(HCategory*)gHades->getCurrentEvent()->getCategory(catRichHit);
    if(!RichHitCat)
    {
      Warning("getCategories()","NO catRichHit in input!");
    }
    else
    {
	iterRichHit=(HIterator*)RichHitCat->MakeIterator("native");
    }

    return kTRUE;
}
Int_t HPhysicsGo4Mon::execute()
{
    //physicsHistbooker->resetActiveList(2,gHades->getEventCounter()); // if feature of incremental update should be used

    HSplineTrack* spline;
    HMetaMatch* metamatch;
    HKickTrack*   kick;
    HMdcTrkCand*  trk;
    HMdcSeg*  seg;
    HTofHit*  tofhit;
    HShowerHitTof*  showertofhit;
    HRichHit*  richhit;
    spline      =0;
    metamatch   =0;
    trk         =0;
    kick        =0;
    seg         =0;
    tofhit      =0;
    showertofhit=0;
    richhit     =0;
    //############################ MDC DEDX ##############################################
    if(MetaMatchCat&&SplineTrackCat)
    {
	iterMetaMatch->Reset();
	while ((metamatch=(HMetaMatch *)iterMetaMatch->Next())!=0)
	{
	    Int_t trkindex   =metamatch->getTrkCandInd();
	    trk              =(HMdcTrkCand*)MdcTrkCandCat->getObject(trkindex);
            Int_t splineindex=metamatch->getSplineInd();
	    if(splineindex!=-1)
	    {   // if spline track exists
		spline    =(HSplineTrack*)SplineTrackCat->getObject(splineindex);
		if(spline) physicsHistbooker->getPhysicsFiller()->fillMdcDeDx(spline,trk);
	    }
	}
    }
    //############################ TOF DEDX ##############################################
    if(MetaMatchCat&&SplineTrackCat&&TofHitCat)
    {
	iterMetaMatch->Reset();
	while ((metamatch=(HMetaMatch *)iterMetaMatch->Next())!=0)
	{
	    Int_t trkindex   =metamatch->getTrkCandInd();
	    trk              =(HMdcTrkCand*)MdcTrkCandCat->getObject(trkindex);
            Int_t splineindex=metamatch->getSplineInd();
	    if(splineindex!=-1)
	    {   // if spline track exists
		spline      =(HSplineTrack*)SplineTrackCat->getObject(splineindex);
		if(spline)
		{
		    Int_t indtof=spline->getTofHitInd();
		    if(indtof!=-1)
		    {
                        tofhit=(HTofHit*)TofHitCat->getObject(indtof);
			if(tofhit)physicsHistbooker->getPhysicsFiller()->fillTofDeDx(spline,tofhit);
		    }
		}
	    }
	}
    }
    //############################ TOFINO DEDX ##########################################
    if(MetaMatchCat&&SplineTrackCat&&ShowerTofHitCat)
    {
	iterMetaMatch->Reset();
	while ((metamatch=(HMetaMatch *)iterMetaMatch->Next())!=0)
	{
	    Int_t trkindex   =metamatch->getTrkCandInd();
	    trk              =(HMdcTrkCand*)MdcTrkCandCat->getObject(trkindex);
            Int_t splineindex=metamatch->getSplineInd();
	    if(splineindex!=-1)
	    {   // if spline track exists
		spline      =(HSplineTrack*)SplineTrackCat->getObject(splineindex);
		if(spline)
		{
		    Int_t indtof=spline->getShowerHitInd();
		    if(indtof!=-1)
		    {
                        showertofhit=(HShowerHitTof*)ShowerTofHitCat->getObject(indtof);
			if(showertofhit)physicsHistbooker->getPhysicsFiller()->fillTofinoDeDx(spline,showertofhit);
		    }
		}
	    }
	}
    }
    //############################ CORR PHI MDC TOF #######################################
    if(MdcSegCat&&TofHitCat)
    {
	iterMdcSeg->Reset();
	while ((seg=(HMdcSeg *)iterMdcSeg->Next())!=0)
	{
	    iterTofHit->Reset();
	    while ((tofhit=(HTofHit *)iterTofHit->Next())!=0)
	    {
		physicsHistbooker->getPhysicsFiller()->fillMdcTofPhi(seg,tofhit);
	    }
	}
    }
    //############################ CORR PHI MDC SHOWERTOFINO ##############################
    if(MdcSegCat&&ShowerTofHitCat)
    {
	iterMdcSeg->Reset();
	while ((seg=(HMdcSeg *)iterMdcSeg->Next())!=0)
	{
	    iterShowerTofHit->Reset();
	    while ((showertofhit=(HShowerHitTof *)iterShowerTofHit->Next())!=0)
	    {
		physicsHistbooker->getPhysicsFiller()->fillMdcTofinoPhi(seg,showertofhit);
	    }
	}
    }
    //############################ CORR PHI MDC RICH #######################################
    if(MdcSegCat&&RichHitCat)
    {
	iterMdcSeg->Reset();
	while ((seg=(HMdcSeg *)iterMdcSeg->Next())!=0)
	{
	    iterRichHit->Reset();
	    while ((richhit=(HRichHit *)iterRichHit->Next())!=0)
	    {
		physicsHistbooker->getPhysicsFiller()->fillMdcRichPhi(seg,richhit);
	    }
	}
    }
    return 0;
}
Bool_t HPhysicsGo4Mon::finalize()
{
    return kTRUE;
}

