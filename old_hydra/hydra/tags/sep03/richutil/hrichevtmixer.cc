// $Id: hrichevtmixer.cc,v 1.2 2003-07-03 12:21:37 eberl Exp $
// Last update by Thomas Eberl: 03/06/21 18:33:06
//
using namespace std;
#include "hrichevtmixer.h"
#include "hdihitmatch.h"
#include "hdihitmatchsim.h"
#include "hrichcut.h"
#include "hrichcuttracklet.h"
#include "hrichcuttrackletsim.h"
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

HRichEvtMixer::HRichEvtMixer(Text_t *name,Text_t *title, Char_t* world, Int_t evts, Int_t rndnr,Char_t *cuts) :
  HReconstructor(name,title)
{
    
    evtrange = evts; // number of events to store before mixing within these
    nrMixObjs = rndnr; // number of tracks per evt used for mixing
                       // if 0 then all tracks are used
    if (nrMixObjs) cout<<"Taking "<<evtrange<<" evts for mixing, "<<rndnr<<" tracks per evt randomly selected"<<endl;
    else cout<<"Taking "<<evtrange<<" evts for mixing, ALL tracks per evt used"<<endl;
    //set flag for simulation or real data 
    TString s(world);
    if (!s.CompareTo("real")) isSim=kFALSE;
    else if (!s.CompareTo("simulation")) isSim=kTRUE;
    else Error("","neither real nor simulation data selected");


    // Prepare cut state identifier for object selection in store events
    TObjArray cutT(5);
    TString sCuts=cuts;

    Int_t len = sCuts.Length();
    if (len>1)
    {
	TString tmp;
	tmp="";
	for (int ii=0; ii < len; ii++)
	{ 
	    TString s(sCuts[ii]);
	    if (!s.CompareTo(","))
	    {
		cutT.Add(new TObjString(tmp.Data()));
		tmp="";
	    }
	    else
	    {
		tmp.Append(s);
	    }
	    if (ii==len-1) cutT.Add(new TObjString(tmp.Data()));
	}
    }


    if (cutT.GetLast()==0) {cutStateId = new TString(((TObjString*)(cutT[cutT.GetLast()]))->GetString());}
    else{
	TString *a = new TString("T");
	for (Int_t i=0;i<cutT.GetLast()+1;i++)
	{
	    TString tmp = ((TObjString*)(cutT[i]))->GetString();
	    if (i>0 && !tmp.Contains("nocut")) {a->Append(".");a->Append("T");}
	    if (!tmp.Contains("nocut")) a->Append(((TObjString*)(cutT[i]))->GetString());
	}

	cutStateId = a;
	if (!a->Contains(".")) cutStateId->Remove(0,1);
    }
    cout<<cutStateId->Data()<<endl;



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
    nCntMixedEvts = 0; // counter for number of mixed events
    nCounterProcessedNbEvents=0; //total nb of evts seen by this task
    ran = new TRandom();

    cuts = new TObjArray(1);
    if (isSim)
    {
	HRichCutTrackletSim *trackcut = new HRichCutTrackletSim("trackcut","default");
	cuts->Add(trackcut);
    }
    else
    {
	HRichCutTracklet *trackcut = new HRichCutTracklet("trackcut","default");
	cuts->Add(trackcut);
    }
    Warning("initCuts","default cuts created");
    
    return kTRUE;
}

Bool_t HRichEvtMixer::finalize() {
    cout<<"************** EVT Mixer TASK ************************************"<<endl;
    if (nrMixObjs) cout<<"Taking "<<evtrange<<" evts for mixing, "<<nrMixObjs<<" tracks per evt randomly selected"<<endl;
    else cout<<"Taking "<<evtrange<<" evts for mixing, ALL tracks per evt used"<<endl;
    cout<<" !!!! "<<nCntMixedEvts<<" event pairs used for mixing "<<endl;
    cout<<"******************************************************************"<<endl;
    return kTRUE;
}


Int_t HRichEvtMixer::execute()
{
    //    cout<<"new event **************************************************************"<<endl;
    //    cout<<"exec: hit match cat: "<<getHitMatchCat()->getEntries()<<endl;
    // filter events which do not contain a correlation object
    if (getHitMatchCat()->getEntries()<1) return kSkipEvent;
    Int_t kReturnValue=1;
    if (evtsStoredInRange == evtrange) 
    {
	//	cout<<"now in exec mixer: "<<evtsStoredInRange<<endl;
	evtsStoredInRange=0;
	if (nrMixObjs>0) doEventMixingTrackSubset();
	else doEventMixingAllTracks();
	deleteStoredEvents();
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
    //    cout<<"storeEvent hit match cat: "<<getHitMatchCat()->getEntries()<<endl;
    TObjArray *arr = new TObjArray(getHitMatchCat()->getEntries());
    TString nn = "evt_"; nn+=n; arr->SetName(nn.Data());
    evtset->Add(arr);
    //cout<<arr->GetName()<<endl;

    HRichCutTracklet *trackcut = 0;
    HRichCutTrackletSim *trackcutsim = 0;
    
    if (isSim) 
    {
	trackcutsim = (HRichCutTrackletSim*)(cuts->FindObject("trackcut"));
    }
    else 
    {
	trackcut = (HRichCutTracklet*)(cuts->FindObject("trackcut"));
    }
    Bool_t isTrackletCut = kFALSE;
    if (trackcutsim && isSim) isTrackletCut=trackcutsim->switchTo(cutStateId->Data());
    else if(trackcut && !isSim) isTrackletCut=trackcut->switchTo(cutStateId->Data());
    else Error("selectTracklets","cut not found");
    if (!isTrackletCut) return;

    HHitMatch *h=0;
    HHitMatchSim *hs=0;
    pIterMatchHit->Reset();
    if (isSim)
    {
	while(( hs= (HHitMatchSim *)pIterMatchHit->Next())) 
	{
	    if (trackcutsim->check(hs))
	    {
		HHitMatchSim *ht = new HHitMatchSim();
		*ht = *hs; // copy object
		arr->Add(ht);
	    }
	}
    }
    else
    {
	while(( h= (HHitMatch *)pIterMatchHit->Next())) 
	{
	    if (trackcut->check(h))
	    {
		HHitMatch *ht = new HHitMatch();
		*ht = *h; // copy object
		arr->Add(ht);
	    }
	}
	
    }
    //    cout<<"evts in "<<arr->GetName()<<" after store evts: "<<arr->GetLast()+1<<endl;
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

void HRichEvtMixer::doEventMixingTrackSubset()
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
    if (!evt0) {Error("doEventMixingTrackSubset","no first event"); return;}
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
	    nCntMixedEvts++; // count number of mixed events
	    //get next evt from evt set
	    TObjArray* evtc = ((TObjArray*)(*evtset)[evtcounter]);
	    if (!evtc) {Error("doEventMixingTrackSubset","no next event"); return;}
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
			    Error("doEventMixingTrackSubset","error in pair creation");
		    }
		    else
		    {
			if (!createHitDiMatch((HHitMatch*)(*evt0)[randtrackindex],
					      (HHitMatch*)(*evtc)[randtrackindexc],alreadystored,alreadycopied))
			    Error("doEventMixingTrackSubset","error in pair creation");
		    }
		}
	    }
	    delete [] ievtc; 
	    delete [] nTrack2Tuple;
	}
    }
    delete [] ievt0; 
}

void HRichEvtMixer::doEventMixingAllTracks()
{
    // should cuts be applied to select better leptons ?

    //    cout<<"new mixing ***************************************************************************"<<endl;
    // use all objs from the rest of evtrange-1 evts to 
    // make combinatorics with all objs of this evt

    // clear HitMatch category
    getHitMatchCat()->Clear();
    // clear DiHitMatch category
    getHitDiMatchCat()->Clear();

    // memorize already stored HHitMatch objects
    TObjArray alreadystored(5);
    TObjArray alreadycopied(5);
    Int_t maxNbEvts = evtset->GetLast()+1;
    //    cout<<"nb of stored evts for mixing: "<<maxNbEvts<<endl;

    for (Int_t evtcountero=0;evtcountero<maxNbEvts-1;evtcountero++)
    {// outer event loop
	
	//	cout<<"outer event loop, index: "<<evtcountero<<endl;
	TObjArray* evt0 = ((TObjArray*)(*evtset)[evtcountero]);//get evt
	if (!evt0) {Error("doEventMixingAllTracks","no first event"); return;}
	Int_t evt0max = evt0->GetLast()+1; // nr of tracks in evt
	//	cout<<"outer evt has entries: "<<evt0max<<endl;
	
	
	for (Int_t evtcounter=evtcountero+1;evtcounter<maxNbEvts;evtcounter++)
	{// loop over the rest of evts
	    nCntMixedEvts++; // count number of mixed events
	    
	    //		cout<<"inner event loop, index: "<<evtcounter<<endl;
	    // get next evt from evt set
	    TObjArray* evtc = ((TObjArray*)(*evtset)[evtcounter]);
	    if (!evtc) {Error("doEventMixingAllTracks","no next event"); return;}
	    Int_t evtcmax = evtc->GetLast()+1; // nr of tracks in current evt
	    //		cout<<"inner evt has entries: "<<evtcmax<<endl;
	    
	    // init array used to check whether index tuple in pair is unique
	    Int_t *nTrack2Tuple=new Int_t[evtcmax*evt0max];
	    for (Int_t iii=0;iii<evtcmax*evt0max;iii++) nTrack2Tuple[iii]=-2;
	    
	    for (Int_t trkcntevt0=0;trkcntevt0<evt0max;trkcntevt0++)
	    {// loop over track objs in evt
		//	cout<<"nb of tracks in outerevt "<<evtcountero<<": "<<evt0max<<" "<<trkcntevt0<<endl;
		//cout<<"outer evt track index: "<<trkcntevt0<<endl;
		
		
		for (Int_t trkcntevtc=0;trkcntevtc<evtcmax;trkcntevtc++)
		{// loop over tracks in this evt
		    // create pair with each track from first evt
		    //cout<<"nb of tracks in evt "<<evtcounter<<": "<<evtcmax<<" "<<trkcntevtc<<endl;
		    //cout<<"inner evt track index: "<<trkcntevtc<<endl;
		    
		    Bool_t isNewPair = HRichCut::isNew2Tuple(trkcntevt0,trkcntevtc,nTrack2Tuple,evtcmax*evt0max);
		    if (isNewPair)
		    {
			if (isSim)
			{
			    if (!createHitDiMatch((HHitMatchSim*)(*evt0)[trkcntevt0],
						  (HHitMatchSim*)(*evtc)[trkcntevtc],alreadystored,alreadycopied))
				Error("doEventMixingAllTracks","error in pair creation");
			}
			else
			{
			    //			    cout<<"pair: "<<evtcountero<<" "<<evtcounter<<" "<<trkcntevt0<<" "<<trkcntevtc<<endl;
			    if (!createHitDiMatch((HHitMatch*)(*evt0)[trkcntevt0],
						  (HHitMatch*)(*evtc)[trkcntevtc],alreadystored,alreadycopied))
				Error("doEventMixingAllTracks","error in pair creation");
			}
		    }
		} // end inner track loop
		
		//		cout<<"inner evt track loop end"<<endl;
	    }
	    delete [] nTrack2Tuple;

	}
    }
}

Bool_t HRichEvtMixer::createHitDiMatch(HHitMatch *h1, HHitMatch *h2,TObjArray &stored,TObjArray &storedc)
{

    // make sure that only tracks that were used in the mixed pair creation are kept
    // in the category.
    // this is necessary as the tracks will be used to fill histograms to control the properties
    // of the mixed event track content

    // in case of the simple method of using all tracks from an event package
    // this is not necessary, but still one has to make sure not to copy the same track
    // more than once into the category

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
    //cout<<getHitMatchCat()->getEntries()<<" tracks in evt after copy"<<endl;
    Int_t ind1 = getHitMatchCat()->getIndex(h1stored);
    Int_t ind2 = getHitMatchCat()->getIndex(h2stored);
    hdi->setIndTrk1(ind1);
    hdi->setIndTrk2(ind2);
    if (hdi) return kTRUE;
    else return kFALSE;;
    
}
Bool_t HRichEvtMixer::createHitDiMatch(HHitMatchSim *h1, HHitMatchSim *h2,TObjArray &stored,TObjArray &storedc)
{
    // make sure that only tracks that were used in the mixed pair creation are kept
    // in the category.
    // this is necessary as the tracks will be used to fill histograms to control the properties
    // of the mixed event track content


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
