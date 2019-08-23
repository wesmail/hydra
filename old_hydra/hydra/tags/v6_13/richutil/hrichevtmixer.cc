// $Id: hrichevtmixer.cc,v 1.1 2003-05-22 17:36:53 eberl Exp $
// Last update by Thomas Eberl: 03/05/22 19:17:05
//
using namespace std;
#include "hrichevtmixer.h"
#include "hdihitmatch.h"
#include "hdihitmatchsim.h"
#include "hrichcut.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include <iostream> 
#include <iomanip>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatch.h"
#include "hhitmatchsim.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "hrichhistfac.h"
#include <TObjArray.h>
#include <TRandom.h>

ClassImp(HRichEvtMixer)

HRichEvtMixer::HRichEvtMixer(Text_t *name,Text_t *title, Char_t* world, Int_t evts, Int_t rndnr) :
  HReconstructor(name,title)
{
    evtrange = evts;
    nrMixObjs = rndnr;
    cout<<"Taking "<<evtrange<<" evts for mixing, "<<rndnr<<" tracks per evt randomly selected"<<endl;
    //set flag for simulation or real data 
    TString s(world);
    if (!s.CompareTo("real")) isSim=kFALSE;
    else if (!s.CompareTo("simulation")) isSim=kTRUE;
    else Error("","neither real nor simulation data selected");

}

HRichEvtMixer::HRichEvtMixer()
{

}


HRichEvtMixer::~HRichEvtMixer(void) {

}


Bool_t HRichEvtMixer::init() 
{
    //cout<<"now in the init of HRichEvtMixer"<<endl;
    if (gHades) 
    {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) 
	{
	    HDetector *rich = spec->getDetector("Rich");
	    if (rich) 
	    { 
		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) 
		{
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		}
	    }
	    
	    pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
	    
	    // Setup output category
	    pHitDiMatchCat=event->getCategory(catDiMatchHit);
	    //cout<<pHitDiMatchCat<<endl;
	    if (!pHitDiMatchCat) 
	    {
		pHitDiMatchCat=rich->buildCategory(catDiMatchHit);
		//cout<<"pair cat created"<<endl;
		if (!pHitDiMatchCat) 
		{
		    Error("init","No HIT DIMATCH category defined");
		    return kFALSE;
		}
		else event->addCategory(catDiMatchHit, pHitDiMatchCat, "Rich");
	    }
	    
	    pIterDiMatchHit = (HIterator*)getHitDiMatchCat()->MakeIterator("native");
	    
	}
    }
    
    
    evtset = new TObjArray(evtrange);
    evtsStoredInRange = 0;
    nCounterProcessedNbEvents=0; //total nb of evts seen by this task
    ran = new TRandom();
    return kTRUE;
}

Bool_t HRichEvtMixer::finalize() {

    return kTRUE;
}


Int_t HRichEvtMixer::execute()
{
    
    // filter events which do not contain a correlation object
    if (getHitMatchCat()->getEntries()<1) return kSkipEvent;
    Int_t kReturnValue=1;
    if (evtsStoredInRange==evtrange) 
    {
	evtsStoredInRange=0;
	doEventMixing();
	deleteStoredEvents();
	storeEvent(evtsStoredInRange++);
	kReturnValue = 0;// exec next task
    }
    else 
    {
	storeEvent(evtsStoredInRange++);
	kReturnValue = kSkipEvent;// skip next task
    }
    
    // -----------------------------------
    nCounterProcessedNbEvents++;
    //cout<<getHitDiMatchCat()->getEntries()<<" pairs in evt "<<nCounterProcessedNbEvents<<endl;
    return kReturnValue;
    
}

void HRichEvtMixer::storeEvent(Int_t n)
{
    //    cout<<"in storeEvent"<<endl;
    TObjArray *arr = new TObjArray(getHitMatchCat()->getEntries());
    TString nn = "evt_"; nn+=n; arr->SetName(nn.Data());
    evtset->Add(arr);
    //    cout<<arr->GetName()<<endl;
    HHitMatch *h=0;
    HHitMatchSim *hs=0;
    pIterMatchHit->Reset();
    if (isSim)
    {
	while(( hs= (HHitMatchSim *)pIterMatchHit->Next())) 
	{
	    HHitMatchSim *ht = new HHitMatchSim();
	    *ht = *hs; // copy object
	    arr->Add(ht);
	}
    }
    else
    {
	while(( h= (HHitMatch *)pIterMatchHit->Next())) 
	{
	    HHitMatch *ht = new HHitMatch();
	    *ht = *h; // copy object
	    arr->Add(ht);
	}
	
    }
}

void HRichEvtMixer::deleteStoredEvents()
{

    for (Int_t evtcounter=0;evtcounter<evtset->GetLast()+1;evtcounter++)
    {
	TObjArray* evtc = ((TObjArray*)(*evtset)[evtcounter]);
	for (Int_t trackcounter=0;trackcounter<evtc->GetLast()+1;trackcounter++)
	{
	    if (isSim) delete ((HHitMatchSim*)(*evtc)[trackcounter]);
	    else delete ((HHitMatch*)(*evtc)[trackcounter]);
	}
	delete evtc;
    }
    delete evtset;
    evtset = new TObjArray(evtrange);
}

void HRichEvtMixer::doEventMixing()
{
    // use nrMixObjs random objs each from the rest of evtrange-1 evts to 
    // make combinatorics with nrMixObjs random objs of this evt

    // clear HitMatch category
    getHitMatchCat()->Clear();
    // clear DiHitMatch category
    getHitDiMatchCat()->Clear();

    // memorize already stored HHitMatch objects
    TObjArray alreadystored(5);
    TObjArray alreadycopied(5);

    //    cout<<getHitMatchCat()->getEntries()<<" tracks in evt after clearing"<<endl;
    
    //cout<<"*** new mixing ***"<<endl;
    Int_t nrMixObjsTmp=nrMixObjs;//make a copy of the maximum nr of tracks per event

    TObjArray* evt0 = ((TObjArray*)(*evtset)[0]);//get first stored evt
    if (!evt0) {Error("doEventMixing","no first event"); return;}
    Int_t nEvt0NewIndCounter=0;//counter for unique track indexes randomly chosen
    Int_t evt0max = evt0->GetLast()+1; // nr of tracks in first evt
    
    // if nr of tracks in evt is smaller than selected nr of evts
    if ( nrMixObjsTmp > evt0max )  nrMixObjsTmp=evt0max;

    Int_t *ievt0 = new Int_t[nrMixObjsTmp]; 
    for (Int_t ii=0;ii<nrMixObjsTmp;ii++) ievt0[ii]=-2;
    for (Int_t trkcntevt0=0;trkcntevt0<nrMixObjsTmp;trkcntevt0++)
    {// draw a track from the evt0

	Int_t randtrackindex = (Int_t) ran->Integer(evt0max);
	while (nEvt0NewIndCounter<nrMixObjsTmp && !HRichCut::isNewIndex(randtrackindex,ievt0,nrMixObjsTmp)) 
	{
	    randtrackindex = (Int_t) ran->Integer(evt0max);
	    //cout<<randtrackindex<<"  "<<evt0max<<" "<<nrMixObjsTmp<<endl;
	}
	nEvt0NewIndCounter++;

	//	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	for (Int_t evtcounter=1;evtcounter<evtset->GetLast()+1;evtcounter++)
	{
	    //get next evt from evt set
	    TObjArray* evtc = ((TObjArray*)(*evtset)[evtcounter]);
	    if (!evtc) {Error("doEventMixing","no next event"); return;}
	    Int_t nrMixObjsTmpC=nrMixObjs;//make a copy of the maximum nr of tracks per event
	    Int_t nEvtCNewIndCounter=0;//counter for unique track indexes randomly chosen
	    Int_t evtcmax = evtc->GetLast()+1; // nr of tracks in current evt
    
	    // if nr of tracks in evt is smaller than selected nr of evts
	    if ( nrMixObjsTmpC > evtcmax )  nrMixObjsTmpC=evtcmax;

	    Int_t *ievtc = new Int_t[nrMixObjsTmpC]; 
	    for (Int_t ii=0;ii<nrMixObjsTmpC;ii++) ievtc[ii]=-2;
	    //choose nrMixObjsTmp from current evt and make pairs with current track from first event. 

	    Int_t *nTrack2Tuple=new Int_t[nrMixObjsTmpC];
	    for (Int_t iii=0;iii<nrMixObjsTmpC;iii++) nTrack2Tuple[iii]=-2;
	    for (Int_t trkcntevtc=0;trkcntevtc<nrMixObjsTmpC;trkcntevtc++)
	    {

		Int_t randtrackindexc = (Int_t) ran->Integer(evtcmax);
		while (nEvtCNewIndCounter<nrMixObjsTmpC && !HRichCut::isNewIndex(randtrackindexc,ievtc,nrMixObjsTmpC)) 
		{
		    randtrackindexc = (Int_t) ran->Integer(evtcmax);
		}
		nEvtCNewIndCounter++;

		// combine to pair
// 		cout<<"--------------------------------------------------------------"<<endl;
// 		cout<<"Evt0: index:"<<randtrackindex<<"  ("<<trkcntevt0<<","<<nrMixObjsTmp<<")"<<endl;
// 		cout<<"EvtC: index:"<<randtrackindexc<<"  ("<<trkcntevtc<<","<<nrMixObjsTmpC<<")  "<<evtc->GetName()<<endl;
		Bool_t isNewPair = HRichCut::isNew2Tuple(randtrackindex,randtrackindexc,nTrack2Tuple,nrMixObjsTmpC);
		if (isNewPair)
		{
		    if (isSim)
		    {
			if (!createHitDiMatch((HHitMatchSim*)(*evt0)[randtrackindex],
					      (HHitMatchSim*)(*evtc)[randtrackindexc],alreadystored,alreadycopied))
			    Error("doEventMixing","error in pair creation");
		    }
		    else
		    {
			if (!createHitDiMatch((HHitMatch*)(*evt0)[randtrackindex],
					      (HHitMatch*)(*evtc)[randtrackindexc],alreadystored,alreadycopied))
			    Error("doEventMixing","error in pair creation");
		    }
		}
	    }
	    delete [] ievtc; 
	    delete [] nTrack2Tuple;
	}
    }
    delete [] ievt0; 
}

Bool_t HRichEvtMixer::createHitDiMatch(HHitMatch *h1, HHitMatch *h2,TObjArray &stored,TObjArray &storedc)
{

    //create a new HDiHitMatch object, input sector
    HLocation loc;
    loc.set(1,0);//dummy ?!!
    HDiHitMatch *hdi = (HDiHitMatch*)((HLinearCategory*)
					getHitDiMatchCat())->getNewSlot(loc);

    if (hdi!=NULL) hdi = new(hdi) HDiHitMatch(h1,h2);
    // store the HHitMatch objs;
    HHitMatch *h1stored = 0;
    HHitMatch *h2stored = 0;
    if (stored.IndexOf(h1)<0) {h1stored = copyHitMatch(h1);stored.Add(h1);storedc.Add(h1stored);}
    else {h1stored = (HHitMatch*)storedc[stored.IndexOf(h1)];}
    if (stored.IndexOf(h2)<0) {h2stored = copyHitMatch(h2);stored.Add(h2);storedc.Add(h2stored);}
    else {h2stored = (HHitMatch*)storedc[stored.IndexOf(h2)];}
    if ( !h1stored || !h2stored )
    {
	Error("createHitDiMatch","could not copy track to category");
	return kFALSE;
    }
    //    cout<<getHitMatchCat()->getEntries()<<" tracks in evt after copy"<<endl;
    Int_t ind1 = getHitMatchCat()->getIndex(h1stored);
    Int_t ind2 = getHitMatchCat()->getIndex(h2stored);
    hdi->setIndTrk1(ind1);
    hdi->setIndTrk2(ind2);
    if (hdi) return kTRUE;
    else return kFALSE;;
    
}
Bool_t HRichEvtMixer::createHitDiMatch(HHitMatchSim *h1, HHitMatchSim *h2,TObjArray &stored,TObjArray &storedc)
{

    //    h1->dumpToStdoutSim();
    
    //create a new HDiHitMatch object, input sector
    HLocation loc;
    loc.set(1,0);//dummy ?!!
    // sim pairs are not yet implemented
    HDiHitMatch *hdi = (HDiHitMatch*)((HLinearCategory*)
					getHitDiMatchCat())->getNewSlot(loc);

    if (hdi!=NULL) hdi = new(hdi) HDiHitMatch(h1,h2);
    // store the HHitMatch objs;
    HHitMatchSim *h1stored = 0;
    HHitMatchSim *h2stored = 0;
    if (stored.IndexOf(h1)<0) {h1stored = copyHitMatch(h1);stored.Add(h1);storedc.Add(h1stored);}
    else {h1stored = (HHitMatchSim*)storedc[stored.IndexOf(h1)];}
    if (stored.IndexOf(h2)<0) {h2stored = copyHitMatch(h2);stored.Add(h2);storedc.Add(h2stored);}
    else {h2stored = (HHitMatchSim*)storedc[stored.IndexOf(h2)];}
    if ( !h1stored || !h2stored )
    {
	Error("createHitDiMatch","could not copy track to category");
	return kFALSE;
    }
    //    cout<<getHitMatchCat()->getEntries()<<" tracks in evt after copy"<<endl;
    Int_t ind1 = getHitMatchCat()->getIndex(h1stored);
    Int_t ind2 = getHitMatchCat()->getIndex(h2stored);
    hdi->setIndTrk1(ind1);
    hdi->setIndTrk2(ind2);
    if (hdi) return kTRUE;
    else return kFALSE;;
    
}

HHitMatch* HRichEvtMixer::copyHitMatch(HHitMatch *h)
{
    //create a new HHitMatch object, input sector
    HLocation loc;
    loc.set(1,0);
    HHitMatch *pHitMatch = (HHitMatch*)((HLinearCategory*) getHitMatchCat())->getNewSlot(loc);

    if (pHitMatch!=NULL) pHitMatch = new(pHitMatch) HHitMatch;
    if (pHitMatch) *pHitMatch = *h;

    return pHitMatch;

}
HHitMatchSim* HRichEvtMixer::copyHitMatch(HHitMatchSim *h)
{
    //create a new HHitMatch object, input sector
    HLocation loc;
    loc.set(1,0);
    HHitMatchSim *pHitMatch = (HHitMatchSim*)((HLinearCategory*) getHitMatchCat())->getNewSlot(loc);

    if (pHitMatch!=NULL) pHitMatch = new(pHitMatch) HHitMatchSim;
    if (pHitMatch) *pHitMatch = *h;

    return pHitMatch;

}
