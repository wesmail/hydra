// $Id: hrichhistofiller.cc,v 1.17 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 04/04/15 14:42:54
using namespace std;
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TTree.h"

#include "hades.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hdetector.h"
#include "hdihitmatch.h"
#include "hdihitmatchsim.h"
#include "hevent.h"
#include "hgeantparticleinfo.h"
#include "hhitmatch.h"
#include "hhitmatchsim.h"
#include "hhitmatchsim.h"
#include "hiterator.h"
#include "hlinearcategory.h"
#include "hmatrixcatiter.h"
#include "hrichcut.h"
#include "hrichcutpair.h"
#include "hrichcutpairsim.h"
#include "hrichcuttracklet.h"
#include "hrichcuttrackletsim.h"
#include "hrichhistfac.h"
#include "hrichhistofiller.h"
#include "hrichhit.h"
#include "hrichutilfunc.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htrackinfo.h"
#include "htrackinfo.h"
#include "richdef.h"

#include <iomanip>
#include <iostream> 
#include <string.h>

ClassImp(HRichHistoFiller)

    HRichHistoFiller::HRichHistoFiller(const Text_t *name,const Text_t *title,const Char_t* filename,const Char_t* cutfilein,const Char_t* world) :
	HReconstructor(name,title)
{
    cout<<"starrt of ctor " <<endl;
    pFileName  = filename;// output filename for histos
    pInFileName = cutfilein;// file with cut objects 
    if (!strcmp(pInFileName,"")) pInFileName="";
    //set flag for simulation or real data 
    TString s(world);
    if (!s.CompareTo("real")) isSim=kFALSE;
    else if (!s.CompareTo("simulation")) isSim=kTRUE;
    else Error("","neither real nor simulation data selected");

    // set several switches to their default value
    // see declaration for comments
    userDefTrackletCutLevelDepth=0;
    userDefPairCutLevelDepth=0;
    userDefCutCombinatorics = kFALSE;
    userDefDataLvlRecDepth = 0;

    kMakeSave = kFALSE;

    kDetHPairs=kFALSE;
    kTrackHPairs=kFALSE;
    kDetHTracks=kFALSE;

    kCombiCutT=kTRUE;
    kCombiCutP=kTRUE;

    kSplitSector=kFALSE;

    kAntiHistos = kFALSE;

    cutIdArrT  = new TObjArray(5);
    cutIdArrP  = new TObjArray(5);

    trackletcut =  paircut = 0;

    kStrictPairCutP = kFALSE;
    kStrictPairCutT = kFALSE;
    cout << "end of ctor " <<endl;
}

HRichHistoFiller::HRichHistoFiller()
{

}


HRichHistoFiller::~HRichHistoFiller(void) 
{

}


Bool_t HRichHistoFiller::init() {
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
		    Error("init","No HIT MATCH category defined, check if it is swt off in calling macro !");
		    return kFALSE;
		}
		else  pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");

		pHitDiMatchCat=event->getCategory(catDiMatchHit);
		if (!pHitDiMatchCat) 
		{
		    Error("init","No HIT DIMATCH category defined, check if it is swt off in calling macro !");
		    return kFALSE;
		}
		else pIterDiMatchHit = (HIterator*)getHitDiMatchCat()->MakeIterator("native");
	    }
	}
    }


    pHistArray = new TObjArray(10); // auto-expanding
    pFileOut = new TFile(pFileName.Data(),"RECREATE");
    if (pInFileName.CompareTo("") != 0 ) pFileCutIn = new TFile(pInFileName.Data());
    else pFileCutIn=0;
    initCuts();
    nCounterProcessedNbEvents=0; //total nb of evts seen by this task
    Int_t split = 3;
    Int_t bsize = 64000;
    testhit = 0;
    testhitsim = 0;
    tree = new TTree("testbaum","testbaum");

    // Create one branch. If splitlevel is set, event is a superbranch
    // creating a sub branch for each data member of the Event object.
    if (isSim) tree->Branch("HHitMatchSim", "HHitMatchSim", &testhitsim, bsize,split);
    else tree->Branch("HHitMatch", "HHitMatch", &testhit, bsize,split);

    cout<<"end of init " << endl;
    return kTRUE;
}

void HRichHistoFiller::initCuts()
{

    if (userDefDataLvlRecDepth!=0)
    {
	switch (userDefDataLvlRecDepth)
	{
	case 1:
	    kTrackHPairs=kTRUE;
	    break;
	case 2:
	    kTrackHPairs=kTRUE;
	    kDetHTracks=kTRUE;
	    break;
	case 3:
	    kTrackHPairs=kTRUE;
	    kDetHTracks=kTRUE;
	    kDetHPairs=kTRUE;
	    break;
	default:
	    kDetHPairs=kFALSE;
	    kTrackHPairs=kFALSE;
	    kDetHTracks=kFALSE;
	    break;
	}
    }
    // print out cutstate identifier set by user in macro
    cout<<endl;
    cout<<"***user defined cuts for HistoFiller************************"<<endl;
    //    cutIdArrT
    cout<<"Tracklets"<<endl;
    for (Int_t i=0;i<cutIdArrT->GetLast()+1;i++)
    {
	cout<<i<<". "<<(((TObjString*)((*cutIdArrT)[i]))->GetString()).Data()<<endl;
    }
    
    cout<<"Pairs"<<endl;
    for (Int_t j=0;j<cutIdArrP->GetLast()+1;j++)
    {
	cout<<j<<". "<<(((TObjString*)((*cutIdArrP)[j]))->GetString()).Data()<<endl;
    }
    cout<<"************************************************************"<<endl;


    
    // fill array with cut objects    
    cuts = new TObjArray(1);
    if (!pFileCutIn)
    { // user has not supplied a file with cut objects
	// create default cut objects for the categories opened in init()  
	if (isSim)
	{
	    HRichCutTrackletSim *trackcut = new HRichCutTrackletSim("trackcut","default");
	    //	    trackcut->setEvtType(1);
	    cuts->Add(trackcut);
	    HRichCutPairSim *paircut = new HRichCutPairSim("paircut","default");
	    cuts->Add(paircut);
	}
	else
	{
	    HRichCutTracklet *trackcut = new HRichCutTracklet("trackcut","default");
	    //	    trackcut->setEvtType(0);
	    cuts->Add(trackcut);
	    HRichCutPair *paircut = new HRichCutPair("paircut","default");
	    cout<<"still alive"<<endl;
	    cuts->Add(paircut);
	}
	Warning("initCuts","default cuts created");

    }
    else
    { // user has supplied a filew/ cut objects
	TIter cutlist(pFileCutIn->GetListOfKeys());
	TKey *cuto=0;
	cout<<"File: "<<pFileCutIn->GetName()<<" opened for reading ..."<<endl;
	while ((cuto=(TKey*)cutlist()))
	{// note that of each class only the instance with the highest cycle nb is read
	    // therefore always the latest version is used, but you can store many in one file
	    // the instances are distinct as they are TNamed with the date as title at creation
	    cout<<"Cut loaded: "<<cuto->GetName()<<" "<<cuto->GetTitle()<<endl;
	    cuts->Add((HRichCutO*)pFileCutIn->Get(cuto->GetName()));
	}
    }

    // there is one (most recent) cut object per category
    // switch it into different states via an identifying string when applying it for selection

    // now set a string to identify the object to be used for cutting (if user has not yet set one)
    if (!trackletcut) trackletcut = new TString("trackcut");
    if (!paircut)     paircut     = new TString("paircut");
    
}

Bool_t HRichHistoFiller::finalize() {
    HRichUtilFunc::saveHistos(pFileOut,pHistArray);
    HRichUtilFunc::saveCuts(pFileOut,cuts);
    pFileOut->cd();
    tree->Write();
    pFileOut->Close();
    for (Int_t arr=0;arr<cutIdArrT->GetLast()+1;arr++) delete ((TObjString*)(*cutIdArrT)[arr]);
    delete cutIdArrT;
    for (Int_t arr=0;arr<cutIdArrP->GetLast()+1;arr++) delete ((TObjString*)(*cutIdArrP)[arr]);
    delete cutIdArrP;
    delete trackletcut;
    delete paircut;
    return kTRUE;
}


Int_t HRichHistoFiller::execute()
{
    if (kMakeSave &&
    	nCounterProcessedNbEvents%nMakeSave==0) HRichUtilFunc::saveHistos(pFileOut,pHistArray);
    //    cout<<"start evt "<<nCounterProcessedNbEvents<<endl;
    // TRACKLET part
    // select tracklets and store them in different obj array
    
    // all objects of one category in one event are regarded as the (object) set
    // a selection of these objects is a subset, a selection larger in size than a specific
    // subset is the respective superset.

    // set up an auto-expandable array to hold arrays with object subsets
    TObjArray* tracksubsets = new TObjArray(5);

    // user has requested a "direct" cut for all given cut names
    if (kCombiCutT) 
    {
	tracksubsets->Add(selectTracklets(0,0));
	if (kAntiHistos) tracksubsets->Add(selectTracklets(0,0,kTRUE));
    }
    // cut iteratively by performing all single cuts, then combine single cuts systematically  
    else
    {
	for (Int_t trackcuts=0;trackcuts<cutIdArrT->GetLast()+1;trackcuts++)
	{//loop over cut names given by user, create a subset for each cutname
	    tracksubsets->Add(selectTracklets((((TObjString*)((*cutIdArrT)[trackcuts]))
					       ->GetString()).Data(),0));
	    if (kAntiHistos) tracksubsets->Add(selectTracklets((((TObjString*)((*cutIdArrT)[trackcuts]))
								->GetString()).Data(),0,kTRUE));
	}
	// create subsets for all possible single cut combinations
	//cout<<(Int_t) userDefCutCombinatorics<<"   "<<userDefTrackletCutLevelDepth<<endl;
	//if (userDefCutCombinatorics) makeCutCombinatorics(tracksubsets,userDefTrackletCutLevelDepth,"T");
    }

    // fill histograms for all existing subsets in list tracksubsets
    fillHistos(tracksubsets,"T");

    // ----------------------------------------------
	
    for (Int_t arr=0;arr<tracksubsets->GetLast()+1;arr++) delete ((TObjArray*)(*tracksubsets)[arr]);
    delete tracksubsets;
    // ----------------------------------------------------------------------------------

    // PAIR part
    // select pairs and store them in different obj arrays


    TObjArray* pairsubsets = new TObjArray(5);

    if (kCombiCutP) 
    {
	pairsubsets->Add(selectPairs(0,0));
	if (kAntiHistos) pairsubsets->Add(selectPairs(0,0,kTRUE));
    }
    // cut iteratively by performing all single cuts, then combine single cuts systematically  
    else
    {
	for (Int_t paircuts=0;paircuts<cutIdArrP->GetLast()+1;paircuts++)
	{
	    pairsubsets->Add(selectPairs((((TObjString*)((*cutIdArrP)[paircuts]))
					  ->GetString()).Data(),0));
	    if (kAntiHistos) pairsubsets->Add(selectPairs((((TObjString*)((*cutIdArrP)[paircuts]))
							   ->GetString()).Data(),0,kTRUE));
	}

	//	    if (userDefCutCombinatorics) makeCutCombinatorics(pairsubsets,userDefPairCutLevelDepth,"P");
    }
    fillHistos(pairsubsets,"P");

   
    // -----------------------------------

    for (Int_t arr=0;arr<pairsubsets->GetLast()+1;arr++) delete ((TObjArray*)(*pairsubsets)[arr]);
    delete pairsubsets;

    Int_t kReturnValue=0;//proceed with next task
	
    nCounterProcessedNbEvents++;
    //    cout<<"end evt -------------------------------------------------------"<<endl;
    return kReturnValue;
    
}


TObjArray* HRichHistoFiller::selectTracklets(const Char_t* cutStateIdentifier,TObjArray *t,Bool_t kBar)
{

    if(cutIdArrT->GetLast()==-1) return NULL;
    // cutStateIdentifier: name of the cut state in the respective cut object
    // with which the selection should be done

    // t: a subset array previously created serving as object input for the selection
    // does not need to be given

    TString cutStateId; // cut state name provided to the cut object to switch it
    TString *a=0; // name of subset in creation
    if (kCombiCutT && cutIdArrT->GetLast()>0) 
    {// a direct cut with more than one cut name
	a = buildCutStateId(cutIdArrT,"T");
	cutStateId = *a;
	if (!a->Contains(".")) cutStateId.Remove(0,1);
	//cout<<a->Data()<<endl;
    }
    else if (kCombiCutT && cutIdArrT->GetLast()==0) 
    {// a direct cut with only one name (senseless, but the dummy user might specify it)
	cutStateId=((TObjString*)((*cutIdArrT)[cutIdArrT->GetLast()]))->GetString();;
	a = buildArrName(cutStateId.Data(),t,"T");
    }
    else if (!kCombiCutT && cutIdArrT->GetLast() > -1) 
    {// a single cut (w/ possible combinatorics) 
	cutStateId=cutStateIdentifier;
	a = buildArrName(cutStateId.Data(),t,"T");
	//cout<<a->Data()<<endl;
    }
    else return NULL;
    // find valid cut object in cut array
    // name is that provided by user or default
    HRichCutTracklet *trackcut = 0;
    HRichCutTrackletSim *trackcutsim = 0;

    if (isSim) 
    {
	trackcutsim = (HRichCutTrackletSim*)(cuts->FindObject(trackletcut->Data()));
    }
    else 
    {
	trackcut = (HRichCutTracklet*)(cuts->FindObject(trackletcut->Data()));
    }

    // switch the cut object into the appropriate state for this selection
    // according to the cut state name that was found above.
    // such a cut state name can be a "single" like "goodRing" or 
    // a dot-seperated list like "goodRing.angularMatch"
    // It is the job of the cut object to distinguish  between single and list
    // and to perform a single or a multiple cut on the object
    Bool_t isTrackletCut = kFALSE;
    if (trackcutsim && isSim) isTrackletCut=trackcutsim->switchTo(cutStateId.Data());
    else if(trackcut && !isSim) isTrackletCut=trackcut->switchTo(cutStateId.Data());
    else Error("selectTracklets","cut not found");
    //     if (isSim) trackcutsim->printCutList();
    //     else trackcut->printCutList();
    // if no valid state of the cut object could be found, do not create a subset
    if (!isTrackletCut) {delete a; return NULL;}

    // setup array for subset objects
    TObjArray *arr = new TObjArray(5);
    if (kBar) a->Prepend("TNOT");
    arr->SetName(a->Data());
    delete a;
    
    // the actual object property checking 
    if(t)
    {// function was called not for the complete category content but  only for
	// the content of the previously created subset
	Int_t max = t->GetLast()+1;
	HHitMatch *h=0;
	HHitMatchSim *hs=0;
	for (Int_t i=0;i<max;i++)
	{//loop over all objs in subset
	    if (isSim) hs = ((HHitMatchSim*)(*t)[i]);
	    else h = ((HHitMatch*)(*t)[i]);
	    // call check function of cut for category object
	    if (kBar)
	    {
		if (isSim && hs && !trackcutsim->check(hs)) arr->Add(hs);
		else if (!isSim && h && !trackcut->check(h)) arr->Add(h);
	    }
	    else 
	    {
		if (isSim && hs && trackcutsim->check(hs)) arr->Add(hs);
		else if (!isSim && h && trackcut->check(h)) arr->Add(h);
	    }
	}
    }
    else
    {// select from all category objects
	HHitMatch *h=0;
	HHitMatchSim *hs=0;
	pIterMatchHit->Reset();

	if (kBar) // anti-histo
	{
	    if (isSim)
	    {
		while(( hs = (HHitMatchSim *)pIterMatchHit->Next()))
		{
		    if (!trackcutsim->check(hs)) {arr->Add(hs);}
		}
	    }
	    else 
	    {
		while(( h = (HHitMatch *)pIterMatchHit->Next())) 
		{
		    if (!trackcut->check(h)) arr->Add(h);
		}
	    }
	}
	else
	{
	    if (isSim)
	    {
		while(( hs = (HHitMatchSim *)pIterMatchHit->Next()))
		{
		    if (trackcutsim->check(hs)) {arr->Add(hs);}
		}
	    }
	    else 
	    {
		while(( h = (HHitMatch *)pIterMatchHit->Next())) 
		{
		    if (trackcut->check(h)) arr->Add(h);
		}
	    }
	}

    }
    //    if (nCounterProcessedNbEvents==2) cout<<"Tracklet subset created: "<<arr->GetName()<<endl;
    return arr;
}


TObjArray* HRichHistoFiller::selectPairs(const Char_t* cutStateIdentifier,TObjArray *t,Bool_t kBar)
{
    if(cutIdArrP->GetLast()==-1) return NULL;
    // c.f. selectTracklets for overall description

    // FIXME: Direct (list-like) cuts with mixed tracklet and pair content are not possible itm
    // direct pure cuts should work

    TString cutStateId; // cut state name provided to the cut object to switch it
    TString *a=0; // name of subset in creation
    if (kCombiCutP && cutIdArrP->GetLast()>0) 
    {// a direct cut with more than one cut name
	a = buildCutStateId(cutIdArrP,"P");
	cutStateId = *a;
	if (!a->Contains(".")) cutStateId.Remove(0,1);
    }
    else if (kCombiCutP && cutIdArrP->GetLast()==0) 
    {// a direct cut with only one name (senseless, but the dummy user might specify it)
	cutStateId=((TObjString*)((*cutIdArrP)[cutIdArrP->GetLast()]))->GetString();;
	a = buildArrName(cutStateId.Data(),t,"P");
    }
    else if (!kCombiCutP && cutIdArrP->GetLast()>-1)
    {// a single cut (w/ possible combinatorics)
	cutStateId=cutStateIdentifier;
	a = buildArrName(cutStateId.Data(),t,"P");
	//cout<<a->Data()<<endl;
    }
    else return NULL;

    // note that a tracklet property cut can be given to a pair for selection
    // as a pair consists of two tracklets
    HRichCutPair *paircuto = 0;
    HRichCutPairSim *paircutosim = 0;
    HRichCutTracklet *trackcut = 0;
    HRichCutTrackletSim *trackcutsim = 0;

    if (isSim) 
    {
	paircutosim = (HRichCutPairSim*)(cuts->FindObject(paircut->Data()));
	//cout<<paircutosim->GetName()<<endl;
    }
    else 
    {
	paircuto = (HRichCutPair*)(cuts->FindObject(paircut->Data()));
    }
    Bool_t isPairCut = kFALSE;
    Bool_t isTrackletCut = kFALSE;
    //cout<<cutStateId.Data()<<endl;
    if (paircutosim && isSim) isPairCut = paircutosim->switchTo(cutStateId.Data());
    else if(paircuto && !isSim) isPairCut = paircuto->switchTo(cutStateId.Data());
    else Error("selectPairs","cut not found");
    if (!isPairCut)
    { // if the cutStateId.Data() does not yield a pair cut, try if it's a tracklet cut 
	if (isSim) 
	{
	    //cout<<"trackcutsim"<<endl;
	    trackcutsim = (HRichCutTrackletSim*)(cuts->FindObject(trackletcut->Data()));
	}
	else 
	{
	    trackcut = (HRichCutTracklet*)(cuts->FindObject(trackletcut->Data()));
	}
	if (trackcutsim && isSim) isTrackletCut = trackcutsim->switchTo(cutStateId.Data());
	else if(trackcut && !isSim) isTrackletCut = trackcut->switchTo(cutStateId.Data());
	else Error("selectPairs","cut not found");
	if (!isTrackletCut) {delete a; return NULL;}
    }


    TObjArray *arr = new TObjArray(5);
    if (kBar) a->Prepend("PNOT");
    //cout<<a->Data()<<endl;
    arr->SetName(a->Data());
    //cout<<arr->GetName()<<endl;
    delete a;

    Int_t mm = 0;
    if(t) mm = (t->GetLast()+1)*2;
    else mm = getHitMatchCat()->getEntries()*2;
    Int_t *tracklistP = new Int_t[mm];
    Int_t *tracklistT = new Int_t[mm];
    for (Int_t tt=0;tt<mm;tt++) tracklistP[tt]=tracklistT[tt]=-2;


    if(t)
    {
	Int_t max = t->GetLast()+1;
	HDiHitMatch *h=0;
	HDiHitMatchSim *hs=0;
	for (Int_t i=0;i<max;i++)
	{

	    if (isSim) hs = ((HDiHitMatchSim*)(*t)[i]);
	    else h = ((HDiHitMatch*)(*t)[i]);

	    if (isPairCut)
	    {
		if (kBar) // fill discarded pairs 
		{
		    if (isSim && hs && !paircutosim->check(hs)) arr->Add(hs);
		    else if (!isSim && h && !paircuto->check(h)) arr->Add(h);
		}
		else // fill good pairs
		{
		    if (isSim && hs)
		    {
			if (paircutosim->check(hs)) arr->Add(hs);
			else 
			{
			    // store the track indizes of the discarded pair in array tracklistP
			    HRichCut::isNewIndex(hs->getIndTrk1(),tracklistP,mm);
			    HRichCut::isNewIndex(hs->getIndTrk2(),tracklistP,mm);
			}
		    }
		    
		    else if (!isSim && h)
		    {
			if (paircuto->check(h)) arr->Add(h);
			else 
			{
			    // store the track indizes of the discarded pair in array tracklistP
			    HRichCut::isNewIndex(h->getIndTrk1(),tracklistP,mm);
			    HRichCut::isNewIndex(h->getIndTrk2(),tracklistP,mm);
			}
		    }

		}
	    }


	    else if(isTrackletCut)
	    {// retrieve tracklets contained in pair, use check on these 
		// to decide on pair
		if (kBar)
		{
		    if(!isSim && h && 
		       (!trackcut->check((HHitMatch*)getHitMatchCat()->
					getObject(h->getIndTrk1())) ||
		       !trackcut->check((HHitMatch*)getHitMatchCat()->
					getObject(h->getIndTrk2())) )
		       ) arr->Add(h);
		    
		    if(isSim && hs && 
		       (!trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					   getObject(hs->getIndTrk1())) ||
		       !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					   getObject(hs->getIndTrk2())) )
		       ) arr->Add(hs);
		}
		else
		{
		    if(!isSim && h )
		    {
			if( trackcut->check((HHitMatch*)getHitMatchCat()->
					    getObject(h->getIndTrk1()))       &&
			    trackcut->check((HHitMatch*)getHitMatchCat()->
					    getObject(h->getIndTrk2()))
			    ) 
			{
			    arr->Add(h);
			}
			else 
			{
			    HRichCut::isNewIndex(h->getIndTrk1(),tracklistT,mm);
			    HRichCut::isNewIndex(h->getIndTrk2(),tracklistT,mm);
			}
		    }

		    if(isSim && hs )
		    {
			if( trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					       getObject(hs->getIndTrk1()))       &&
			    trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					       getObject(hs->getIndTrk2()))
			    ) 
			{
			    arr->Add(hs);
			}
			else 
			{
			    HRichCut::isNewIndex(hs->getIndTrk1(),tracklistT,mm);
			    HRichCut::isNewIndex(hs->getIndTrk2(),tracklistT,mm);
			}
		    }

		}
		
	    } 
	}
    }
    else // look to the original HLinearCategory from the data source
    {
	HDiHitMatch *h=0;
	HDiHitMatchSim *hs=0;
	pIterDiMatchHit->Reset();
	if (isPairCut)
	{
	    if (kBar)
	    {
		if (isSim)
		{
		    while(( hs = (HDiHitMatchSim *)pIterDiMatchHit->Next()))
		    {
			if (!paircutosim->check(hs)) arr->Add(hs);
		    }
		}
		else 
		{
		    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
		    {
			if (!paircuto->check(h)) arr->Add(h);
		    }
		}
	    }
	    else
	    {
		if (isSim)
		{
		    while(( hs = (HDiHitMatchSim *)pIterDiMatchHit->Next()))
		    {
			if (paircutosim->check(hs)) arr->Add(hs);
			else 
			{
			    // store the track indizes of the discarded pair in array tracklistP
			    HRichCut::isNewIndex(hs->getIndTrk1(),tracklistP,mm);
			    HRichCut::isNewIndex(hs->getIndTrk2(),tracklistP,mm);
			}
		    }
		}
		else 
		{
		    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
		    {
			if (paircuto->check(h)) arr->Add(h);
			else 
			{
			    // store the track indizes of the discarded pair in array tracklistP
			    HRichCut::isNewIndex(h->getIndTrk1(),tracklistP,mm);
			    HRichCut::isNewIndex(h->getIndTrk2(),tracklistP,mm);
			}
		    }
		}
	    }
	}
	else if (isTrackletCut)
	{
	    if (kBar)
	    {
		if (isSim)
		{
		    while(( hs = (HDiHitMatchSim *)pIterDiMatchHit->Next()))
		    {

			if (
			    !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->getObject(hs->getIndTrk1())) || 
			    !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->getObject(hs->getIndTrk2()))
			    ) arr->Add(hs);
		    }
		}
		else 
		{
		    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
		    {
			if (
			    !trackcut->check((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1())) || 
			    !trackcut->check((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2()))
			    ) arr->Add(h);
		    }
		}
	    }
	    else
	    {
		if (isSim)
		{
		    while(( hs = (HDiHitMatchSim *)pIterDiMatchHit->Next()))
		    {

			if (
			    trackcutsim->check((HHitMatchSim*)getHitMatchCat()->getObject(hs->getIndTrk1())) && 
			    trackcutsim->check((HHitMatchSim*)getHitMatchCat()->getObject(hs->getIndTrk2()))
			    ) arr->Add(hs);
			else 
			{
			    HRichCut::isNewIndex(hs->getIndTrk1(),tracklistT,mm);
			    HRichCut::isNewIndex(hs->getIndTrk2(),tracklistT,mm);
			}
		    }
		}
		else 
		{
		    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
		    {
			if (
			    trackcut->check((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1())) && 
			    trackcut->check((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2()))
			    ) arr->Add(h);
			else 
			{
			    HRichCut::isNewIndex(h->getIndTrk1(),tracklistT,mm);
			    HRichCut::isNewIndex(h->getIndTrk2(),tracklistT,mm);
			}
		    }
		}
	    }
	}
    }

    // If "strict cutting" is switched on
    // pairs are thrown away, when they contain a track that was 
    // thrown away in another check.

    // FIXME: what about the anti samples in case of strict track cutting ???
    TObjArray *filtered = 0;
    //cout<<arr->GetLast()<<endl;
    if (arr->GetLast()>-1 && !kBar)
    {
	if (kStrictPairCutP) filtered = filterPairs(arr,tracklistP); 
	if (kStrictPairCutT && kStrictPairCutP) filtered = filterPairs(filtered,tracklistT);
	else if (kStrictPairCutT && !kStrictPairCutP) filtered = filterPairs(arr,tracklistT);
    }
    delete [] tracklistT;
    delete [] tracklistP;

    if (filtered)   return filtered;
    else            return arr;

}


void HRichHistoFiller::fillHistos(TObjArray *subsets,const Char_t * catIdentifier)
{
    TString catId(catIdentifier);
    //call histogram filling routine for each subset of objects
    for (Int_t arr=0;arr<subsets->GetLast()+1;arr++) 
    {
	TObjArray* t = ((TObjArray*)(*subsets)[arr]);
	//if (t && t->GetLast()+1 > 0) cout<<t->GetName()<<" : "<<t->GetLast()+1<<endl;
	if (t)
	{
	    if (countCutsInSubset(t->GetName())>=userDefHistoMinCutLvl) 
	    {
		//		cout<<t->GetName()<<" "<<countCutsInSubset(t->GetName())<<endl;
		Int_t a=0;
		if (!catId.CompareTo("T")) a = fillTrackHistosFromTracklets(t,t->GetName());
		else if (!catId.CompareTo("P")) a = fillPairHistosFromPairs(t,t->GetName());
		if (!a) Error("fillHistos","fillXXXHistosFromXXX() failed");
	    }
	} //else  Error("fillHistos","no subset found");
    }
}

// void HRichHistoFiller::fillPairHistos(TObjArray *subsets)
// {
//     // call pair property histogram filling routine for each subset of pairs
//     for (Int_t arr=0;arr<subsets->GetLast()+1;arr++) 
//     {
// 	TObjArray* t = ((TObjArray*)(*subsets)[arr]);
// 	//if (t && t->GetLast()+1 > 0) cout<<t->GetName()<<" : "<<t->GetLast()+1<<endl;
// 	if (!t || !fillPairHistosFromPairs(t,t->GetName())) Error("execute","fillPairHistosFromPairs() failed");
//     }
// }

Int_t HRichHistoFiller::fillDetHistosFromTracklets(TObjArray* trackArray,const Char_t *cNameTrackSample)
{
    // input is subset of tracklets, propagate creation history o subset with cNameTrackSample
    // retrieve objects (det hits) contained in tracklet and fill respective predefined property histos
    Int_t ret_val=1;
    Int_t max = trackArray->GetLast()+1;
    // helper arrays to check uniqueness of object
    // do not fill same det hit object twice into histogram if it occurs in two different higher lvl objs
    Int_t *rings = new Int_t[max];
    Int_t *tofs = new Int_t[max];
    Int_t *particles = new Int_t[max*MAXPARTICLES];
    for (Int_t j=0;j<max;j++) rings[j]=tofs[j]=-2;
    for (Int_t k=0;k<max*MAXPARTICLES;k++) particles[k]=-2;
    Int_t nRingCounter = 0;
    TString samplenamerings(trackArray->GetName());
    samplenamerings.Prepend("Rings.");
    for (Int_t i=0;i<max;i++)
    {
	HHitMatchSim *hs = 0;
	HHitMatch *h = 0;
	if (isSim) hs = ((HHitMatchSim*)(*trackArray)[i]);
	else h = ((HHitMatch*)(*trackArray)[i]);
	if (isSim) h=hs;
	TString samplename(trackArray->GetName());
	if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);
	if (kSplitSector) 
	{
	    TString sector("sec");
	    sector+=h->getSector();
	    appendName(samplename,sector.Data());
	}

	// RICH
	Bool_t isNewRing = HRichCut::isNewIndex(h->getRichInd(),rings,max);
	if (isNewRing)
	{
	    nRingCounter++;
	    fillTH1F(HRichHistFac::getHRingRadiusBasename(),samplename.Data(),h->getRadius());
	    fillTH2F(HRichHistFac::getHRingRadiusThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRadius());
		
	    fillTH1F(HRichHistFac::getHRingAmplitudeBasename(),samplename.Data(),h->getRingAmplitude());
	    fillTH2F(HRichHistFac::getHRingAmplitudeThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingAmplitude());
		
	    fillTH1F(HRichHistFac::getHRingPadNrBasename(),samplename.Data(),h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingPadNrThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingPadNr());
		
	    fillTH1F(HRichHistFac::getHRingLocMax4Basename(),samplename.Data(),h->getRingLocalMax4());
	    fillTH2F(HRichHistFac::getHRingLocMax4ThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingLocalMax4());
		
	    fillTH1F(HRichHistFac::getHRingPMBasename(),samplename.Data(),h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingPMThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingPatMat());
		
	    fillTH1F(HRichHistFac::getHRingHTBasename(),samplename.Data(),h->getRingHouTra());
	    fillTH2F(HRichHistFac::getHRingHTThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingHouTra());

 	    fillTH1F(HRichHistFac::getHRingCentroidBasename(),samplename.Data(),h->getCentroid());
	    fillTH2F(HRichHistFac::getHRingCentroidThetaBasename(),samplename.Data(),h->getRichTheta(),h->getCentroid());

	    Float_t avChrg = ((Float_t)h->getRingAmplitude()) / ((Float_t)h->getRingPadNr());
	    fillTH1F(HRichHistFac::getHRingAvChrgBasename(),samplename.Data(),avChrg);
	    fillTH2F(HRichHistFac::getHRingAvChrgThetaBasename(),samplename.Data(),h->getRichTheta(),avChrg);

	    fillTH2F(HRichHistFac::getHRingAvChrgLocMax4Basename(),samplename.Data(),avChrg,h->getRingLocalMax4());
	    fillTH2F(HRichHistFac::getHRingAvChrgCentroidBasename(),samplename.Data(),avChrg,h->getCentroid());
	    fillTH2F(HRichHistFac::getHRingAvChrgPatMatBasename(),samplename.Data(),avChrg,h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingAvChrgPadNrBasename(),samplename.Data(),avChrg,h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingLocMax4CentroidBasename(),samplename.Data(),h->getRingLocalMax4(),h->getCentroid());
	    fillTH2F(HRichHistFac::getHRingLocMax4PatMatBasename(),samplename.Data(),h->getRingLocalMax4(),h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingLocMax4PadNrBasename(),samplename.Data(),h->getRingLocalMax4(),h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingCentroidPatMatBasename(),samplename.Data(),h->getCentroid(),h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingCentroidPadNrBasename(),samplename.Data(),h->getCentroid(),h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingPatMatPadNrBasename(),samplename.Data(),h->getRingPatMat(),h->getRingPadNr());
	    
	    fillTH2F(HRichHistFac::getHRingXYBasename(),samplename.Data(),h->getRingCenterX(),h->getRingCenterY());
	    
	    if (isSim)
	    {
		HTrackInfo * t = hs->getTrackInfoObj();

		for (Int_t part=0;part<MAXPARTICLES;part++)
		{
		    if (t->getParId(part)==-1) break;
		    Bool_t isNewPart = HRichCut::isNewIndex(t->getTrkNb(part),particles,max*MAXPARTICLES);

		    if (isNewPart) fillTH2F(HRichHistFac::getHRingParIdWeightBasename(),samplename.Data(),
					    (Float_t)t->getParId(part),t->getRichWeight(part));
		}

	    }
	    
	}
	

	//TOF
	Bool_t isNewTof = HRichCut::isNewIndex(h->getTofInd(),tofs,max);
	if (isNewTof)
	{
	    fillTH1F(HRichHistFac::getHTofBasename(),samplename.Data(),h->getTofTof());
	}
	
	
    }
    
    if (nRingCounter) fillTH1F(HRichHistFac::getHMultBasename(),samplenamerings.Data(),nRingCounter);
    delete [] rings;
    delete [] tofs;
    delete [] particles;

    return ret_val;
}

Int_t HRichHistoFiller::fillTrackHistosFromTracklets(TObjArray* trackArray,const Char_t *cNameTrackSample)
{
    // function takes array with selected tracks and fills fixed histograms.
    // function makes sure that track is not used twice and that components
    // of a track are not used twice
    // N.B.: if track was created using the COMPLEX MODE (HRichCorrelator), a check for the unique nature
    // of a kick track is not sufficient, as there will be tracks that have no kicktrack
    Int_t ret_val=1;
    Int_t max = trackArray->GetLast()+1;
    // define helper arrays
    // for deflection angle histo
    Int_t *tupt = new Int_t[max];
    Int_t *tups = new Int_t[max];
    Int_t *particles = new Int_t[max*MAXPARTICLES];
    for (Int_t k=0;k<max*MAXPARTICLES;k++) particles[k]=-2;
    // for mom vs beta histo
    Int_t *kicks=new Int_t[max];
    // init helper arrays
    for (Int_t j=0;j<max;j++) tupt[j]=tups[j]=kicks[j]=-2;

    Int_t nTrackCounterem  = 0;//cntr all q<0 recon tracks
    Int_t nTrackCounterep  = 0;
    Int_t nTrackCounter0   = 0;

    Int_t nTrackCounterem4 = 0;//cntr all real fully tracked e- (disreg. rest content)
    Int_t nTrackCounterep4 = 0;

    Int_t nTrackCounterfh = 0;//cntr all min partial fakes w/o e+||e- 
    Int_t nTrackCounterfmixed = 0;//cnt all partial fakes w/ and w/o e+||e-
    Int_t nTrackCounterffull = 0;//cnt all complete fakes
    Int_t nTrackCounterfmixedlep = 0;//cnt all partial fakes w/ e+ && e-

    Int_t nTrackCounterfem = 0;//cntr all min partial fakes w/  e+||e-
    Int_t nTrackCounterfep = 0;

    TString samplenametracks(trackArray->GetName());
    samplenametracks.Prepend("Tracks.");
    TString samplenametracks0(samplenametracks);
    TString samplenametracksem(samplenametracks);
    TString samplenametracksep(samplenametracks);
    samplenametracksem.Append(".em");
    samplenametracksep.Append(".ep");
    samplenametracks0.Append(".all");

    TString samplenamefullfake(samplenametracks);appendName(samplenamefullfake,"fullfake");
    TString samplenamemixfake(samplenametracks);appendName(samplenamemixfake,"mixfake");
    TString samplenamemixlepfake(samplenametracks);appendName(samplenamemixlepfake,"mixlepfake");
    TString samplenameepfake(samplenametracks);appendName(samplenameepfake,"epfake");
    TString samplenameemfake(samplenametracks);appendName(samplenameemfake,"emfake");
    TString samplenamehfake(samplenametracks);appendName(samplenamehfake,"hfake");
    TString samplenamefulltrackep(samplenametracks);appendName(samplenamefulltrackep,"fulltrack.ep");
    TString samplenamefulltrackem(samplenametracks);appendName(samplenamefulltrackem,"fulltrack.em");

    TObjArray *geant = 0;
    if (isSim) geant = HRichUtilFunc::getGeantParticleInfo(trackArray);
 
    // start loop over preselected tracks
    for (Int_t i=0;i<max;i++)
    {

	HHitMatchSim *hs = 0;
	HHitMatch *hi = 0;
	if (isSim) hs = ((HHitMatchSim*)(*trackArray)[i]);
	else hi = ((HHitMatch*)(*trackArray)[i]);
	if (isSim) hi=hs;
	
	if (!hi) continue;

	// fill deflection angle using each mdc-meta combi only once
	TString samplename(trackArray->GetName());
	appendName(samplename,"MDC_META");
	if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);
	Int_t mdcidx=hi->getMdcInd();
	Int_t metaidx=-1;
	Int_t *arr=0;
	if (hi->getTofInd()>=0) {metaidx=hi->getTofInd();arr=tupt;}
	else if (hi->getShowInd()>=0) {metaidx=hi->getShowInd();arr=tups;}
	Bool_t kNewDeflAngle = HRichCut::isNew2Tuple(mdcidx,metaidx,arr,max);
	if (kNewDeflAngle) {fillTH1F(HRichHistFac::getHDeflAngleBasename(),samplename.Data(),
				     HRichUtilFunc::getDeflectionAngle(hi));
	// 	TString aa="";
	// 	if (hi->getTofInd()>=0) aa="tof";
	// 	else if (hi->getShowInd()>=0) aa="sho";
	//cout<<samplename.Data()<<" : "<<mdcidx<<" ("<<aa.Data()<<")  "<<metaidx<<endl;
	}
	
	
	
	// fill kick track histos for different kicktracks
	Int_t trackind=hi->getKickInd();
	Bool_t kNewTrack=HRichCut::isNewIndex(trackind,kicks,max);
	//kNewTrack=kTRUE;
	if(kNewTrack)
	{
	    if (isSim) // for each rich-kicktrack combination
	    {

		//		cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&& new particle &&&&&&&&&&&&&&&&&&"<<endl;
		HTrackInfo * t = hs->getTrackInfoObj();
		TString *pSampleNameFake=0;


		Bool_t fullyTracked=kFALSE;
		for (Int_t part=0;part<MAXPARTICLES;part++) 
		{ 
		    
		    if (t->calcCorrCode(part)==4) 
		    {
			fullyTracked=kTRUE;
			//			cout<<"fully tracked particle "<<endl;
			if (t->getParId(part)==2) {nTrackCounterep4++;pSampleNameFake = &samplenamefulltrackep;}
			else if (t->getParId(part)==3) {nTrackCounterem4++;pSampleNameFake = &samplenamefulltrackem;}
			break;
		    }
		}

		if (!fullyTracked)
		{

		    Bool_t kEm = kFALSE;
		    Bool_t kEp = kFALSE;
		    Bool_t kH  = kFALSE;
		    for (Int_t part1=0;part1<MAXPARTICLES;part1++) 
		    { 
			
			if (t->getTrkNb(part1) > -1 && 
			    t->calcCorrCode(part1)<4 && t->calcCorrCode(part1)>=0) 
			{
			    //			    cout<<"fake particle "<<endl;
			    if (t->getParId(part1)==2) kEp=kTRUE;
			    else if (t->getParId(part1)==3) kEm=kTRUE;
			    else if (t->getParId(part1)>3) kH=kTRUE;
			}
		    }

		    TString nase ="";Int_t nasecnt=0;
		    if ((kEp||kEm) && kH) {pSampleNameFake=&samplenamemixfake;nTrackCounterfmixed++;nase="mixedfake";nasecnt++;}
		    if (kEm && kEp && !kH) {pSampleNameFake=&samplenamemixlepfake; nTrackCounterfmixedlep++;nase="elecposfake";nasecnt++;}
		    else if (kEp && !kH) {pSampleNameFake=&samplenameepfake; nTrackCounterfep++;nase="positronfake";nasecnt++;}
		    else if (kEm && !kH) {pSampleNameFake=&samplenameemfake; nTrackCounterfem++;nase="elecfake";nasecnt++;}
		    if ( kH && !kEp && !kEm ) {pSampleNameFake=&samplenamehfake; nTrackCounterfh++;nase="hadron fake";nasecnt++;}
		    if (!kEp && !kEm && !kH) {pSampleNameFake=&samplenamefullfake;nTrackCounterffull++;nase="fullfake";nasecnt++;}
		    //    		    cout<<nasecnt<<"  "<<hs->getKickInd()<<endl;
		    // 	    if (nase.CompareTo("")!=0){
		    // 			cout<<"*******************************"<<endl;
		    // 			cout<<nase.Data()<<endl;
		    // 		    t->dumpToStdout();
		    // 		    }
		}
		//		cout<<"stillalive"<<endl;
		if (pSampleNameFake) 
		  {
		    fillTH1F(HRichHistFac::getHMomBasename(),pSampleNameFake->Data(),hi->getKickMom());
		    fillTH1F(HRichHistFac::getHPolarAngleBasename(),pSampleNameFake->Data(),hi->getMdcTheta());
		    testhitsim=hs;
		    tree->Fill();
		  }
		//		cout<<"stillalive 2"<<endl;
		
	    }
	    // EXPERIMENTAL INFORMATION HISTOS
	    if (hi->getKickCharge()==1) {nTrackCounterep++;}
	    if (hi->getKickCharge()==-1) {nTrackCounterem++;}
	    nTrackCounter0++;
// 	    cout<<" **********************"<<endl;
// 	    cout<<nTrackCounterep<<"  "<<nTrackCounterem<<"  "<<nTrackCounter0<<endl;
	    //cout<<samplename.Data()<<" : "<<trackind<<endl;
	    samplename="";
	    samplename=trackArray->GetName();
	    if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);

	    TLorentzVector* l = hi->getLVec();
	    //cout<<samplename.Data()<<" : "<<trackind<<endl;
	    //
	    // 2D ===============================================================
	    fillTH2F(HRichHistFac::getHMomChrgvsBetaBasename(),samplename.Data(),
		     hi->getKickBeta(),hi->getKickMom()*hi->getKickCharge());
	    //
	    fillTH2F(HRichHistFac::getHMomChrgvsPolarAngleBasename(),samplename.Data(),
		     hi->getKickMom()*hi->getKickCharge(),hi->getMdcTheta());
	    //
	    fillTH2F(HRichHistFac::getHMomtChrgvsPolarAngleBasename(),samplename.Data(),
		     l->Pt()*hi->getKickCharge(),hi->getMdcTheta());
	    //
	    fillTH2F(HRichHistFac::getHMassvsBetaBasename(),samplename.Data(),
		     hi->getKickMass(),hi->getKickBeta());
	    //
	    fillTH2F(HRichHistFac::getHMassChrgvsBetaBasename(),samplename.Data(),
		     hi->getKickMass()*hi->getKickCharge(),hi->getKickBeta());
	    
// 	    Float_t rad2deg = 180./TMath::Pi();
	    fillTH2F(HRichHistFac::getHPolarHitChrgvsAzimuthBasename(),samplename.Data(),
		     hi->getMdcTheta()*hi->getKickCharge(),hi->getMdcPhi());

	    fillTH2F(HRichHistFac::getHMdcRvsMdcZBasename(),samplename.Data(),
		     hi->getMdcZ(),hi->getMdcR());

	    if (hi->getMdcClusterSize()>0)
	    {
		TString tmp(samplename);
		tmp.Append(".chip");
		fillTH2F(HRichHistFac::getHMdcRvsMdcZBasename(),tmp.Data(),
			 hi->getMdcZ(),hi->getMdcR());
	    }

	    if (hi->getMdcClusterSize()<0)
	    {
		TString tmp(samplename);
		tmp.Append(".chim");
		fillTH2F(HRichHistFac::getHMdcRvsMdcZBasename(),tmp.Data(),
			 hi->getMdcZ(),hi->getMdcR());
	    }

	    fillTH2F(HRichHistFac::getHMdcRvsMdcChi2Basename(),samplename.Data(),
		     hi->getMdcClusterSize(),hi->getMdcR());

	    
	    // 1D ===============================================================
	    //
	    fillTH1F(HRichHistFac::getHMassBasename(),samplename.Data(),
		     hi->getKickMass());
	    //

	    // split 1D single track spectra in e+ and e- 
	    TString samplenameep(samplename);appendName(samplenameep,"ep");
	    TString samplenameem(samplename);appendName(samplenameem,"em");
	    TString *pSampleName=0;
	    if (hi->getKickCharge()==1) pSampleName=&samplenameep;
	    else if (hi->getKickCharge()==-1) pSampleName=&samplenameem;

	    fillTH1F(HRichHistFac::getHPolarAngleBasename(),pSampleName->Data(),
		     hi->getMdcTheta());

	    fillTH1F(HRichHistFac::getHAzimuthalAngleBasename(),pSampleName->Data(),
		     hi->getMdcPhi());

	    fillTH1F(HRichHistFac::getHMomtBasename(),pSampleName->Data(),
		     l->Pt());

	    fillTH1F(HRichHistFac::getHMomBasename(),pSampleName->Data(),
		     hi->getKickMom());


	    fillTH1F(HRichHistFac::getHKickPullBasename(),pSampleName->Data(),
		     hi->getKickPull());

	    fillTH1F(HRichHistFac::getHBetaBasename(),pSampleName->Data(),
		     hi->getKickBeta());


	    fillTH1F(HRichHistFac::getHRapidityBasename(),pSampleName->Data(),
		     l->Rapidity());


	    fillTH1F(HRichHistFac::getHPseudoRapidityBasename(),pSampleName->Data(),
		     l->PseudoRapidity());

	    testhit=hi;
	    tree->Fill();     
	    // SIMULATION INFORMATION HISTOS

	    if (isSim)
	    {

		
		HTrackInfo * t = hs->getTrackInfoObj();

		TString samplenamemult(samplename);appendName(samplenamemult,"partpertrk");
		// multiplicity of GEANT particles in recon track
		fillTH1F(HRichHistFac::getHMultBasename(),samplenamemult.Data(),t->getPartNr());

		
	// 	TString *pSampleNameFake=0;


// 		Bool_t fullyTracked=kFALSE;
// 		for (Int_t part=0;part<MAXPARTICLES;part++) 
// 		{ 

// 		    if (t->calcCorrCode(part)==4) 
// 		    {
// 			fullyTracked=kTRUE;
// 			if (t->getParId(part)==2) {nTrackCounterep4++;pSampleNameFake = &samplenamefulltrackep;}
// 			else if (t->getParId(part)==3) {nTrackCounterem4++;pSampleNameFake = &samplenamefulltrackem;}
// 			break;
// 		    }
// 		}

// 		if (!fullyTracked)
// 		{

// 		    Bool_t kEm = kFALSE;
// 		    Bool_t kEp = kFALSE;
// 		    Bool_t kH  = kFALSE;
// 		    for (Int_t part=0;part<MAXPARTICLES;part++) 
// 		    { 
			
// 			if (t->calcCorrCode(part)<4) 
// 			{
// 			    if (t->getParId(part)==2) kEp=kTRUE;
// 			    else if (t->getParId(part)==3) kEm=kTRUE;
// 			    else if (t->getParId(part)>3) kH=kTRUE;
// 			}
// 		    }

// 		    Char_t *nase = 0;
// 		    if ((kEp||kEm) && kH) {pSampleNameFake=&samplenamemixfake;nTrackCounterfmixed++;nase="mixedfake";}
// 		    if (kEm && kEp && !kH) {pSampleNameFake=&samplenamemixlepfake; nTrackCounterfmixedlep++;nase="elecposfake";}
// 		    else if (kEp && !kH) {pSampleNameFake=&samplenameepfake; nTrackCounterfep++;nase="positronfake";}
// 		    else if (kEm && !kH) {pSampleNameFake=&samplenameemfake; nTrackCounterfem++;nase="elecfake";}
// 		    if ( kH && !kEp && !kEm ) {pSampleNameFake=&samplenamehfake; nTrackCounterfh++;nase="hadron fake";}
// 	// 	    if (nase){
// // 			cout<<"*******************************"<<endl;
// // 			cout<<nase<<endl;
// // 		    t->dumpToStdout();
// // 		    }
// 		}
// 		//		cout<<"stillalive"<<endl;
// 		if (pSampleNameFake) fillTH1F(HRichHistFac::getHMomBasename(),pSampleNameFake->Data(),hi->getKickMom());
// 		//		cout<<"stillalive 2"<<endl;
		

		// loop over GEANT particles found in this recon track
		for (Int_t part=0;part<MAXPARTICLES;part++)
		{
		    if (t->getParId(part)==-1) break;//no more particles

		    Bool_t isNewPart = HRichCut::isNewIndex(t->getTrkNb(part),particles,max*MAXPARTICLES);
		    //
		    Int_t nCorrCode = t->calcCorrCode(part);//corr code of part in recon track
		    // ------------------
		    fillTH2F(HRichHistFac::getHMomCorrCodeGeantBasename(),samplename.Data(),
			     t->getTotMom(part)-hs->getKickMom(),nCorrCode*hs->getKickCharge());
		    fillTH2F(HRichHistFac::getHMomvsMomBasename(),samplename.Data(),
			     t->getTotMom(part),hs->getKickMom()*hs->getKickCharge());
		    fillTH1F(HRichHistFac::getHCorrCodeGeantBasename(),samplename.Data(),
			     nCorrCode*hs->getKickCharge());
		    fillTH2F(HRichHistFac::getHPartCorrCodeGeantBasename(),samplename.Data(),
			     t->getParId(part),nCorrCode*hs->getKickCharge());
		    fillTH2F(HRichHistFac::getHCorrCodeGeantPolarAngleBasename(),samplename.Data(),
			     nCorrCode*hs->getKickCharge(),hs->getMdcTheta());
		    
		    //-------------------

		    // check for new particle
		    if (isNewPart) fillTH1F(HRichHistFac::getHParIdBasename(),samplename.Data(),
					    (Float_t)t->getParId(part));
		    
		    TString samplenametmp(samplename);
		    samplenametmp.Append(".RICH");
		    if (isNewPart) fillTH2F(HRichHistFac::getHRingParIdWeightBasename(),samplenametmp.Data(),
					    (Float_t)t->getParId(part),t->getRichWeight(part));
		    
		    TString samplenametmp2(samplename);
		    samplenametmp2.Append(".MDC");
		    if (isNewPart) fillTH2F(HRichHistFac::getHRingParIdWeightBasename(),samplenametmp2.Data(),
					    (Float_t)t->getParId(part),t->getMdcWeight(part));
		    
		    if (isNewPart) fillTH2F(HRichHistFac::getHWeightWeightBasename(),samplename.Data(),
					    (Float_t)t->getRichWeight(part),t->getMdcWeight(part));
		    
		    
		    if (isNewPart)
		    {
			if(t->calcCorrCode(part)==4)//it is not a fake
			{
			    TString sss(samplename.Data());
			    sss.Append(".true");
			    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsMomBasename(),
							sss.Data(),
							1./HRichUtilFunc::getDeflectionAngle(hi),
							hi->getKickMom()*hi->getKickCharge()
							);
	    //


			}
			// check if kicktrack is fake
			//if(t->getMatchedMT_S(part)==1)//it is not a fake
			//{
// 			if (t->getTotMom(part)>0. && hs->getKickMom()>0. )//&& hs->getKickBeta()>0.935 && TMath::Abs(hs->getKickPull())<3.)
// 			{
// 			    TString samplenametmp3(samplename);
// 			    samplenametmp3.Append(".momerrgt50");
// 			    Float_t pG = 1./t->getTotMom(part);
// 			    Float_t pK = 1./hs->getKickMom();
// 			    Float_t relerr = (pG-pK)/pG;
// 			    fillTH2F(HRichHistFac::getHGeantMomKickMomDiffBasename(),samplename.Data(),
// 				     t->getTotMom(part),relerr);
// 			    if (relerr>0.5) fillTH1F(HRichHistFac::getHCorrCodeGeantBasename(),samplenametmp3.Data(),
// 						     nCorrCode*hs->getKickCharge());
// 			    if (relerr>0.5) fillTH2F(HRichHistFac::getHPartCorrCodeGeantBasename(),samplenametmp3.Data(),
// 						     t->getParId(part),nCorrCode*hs->getKickCharge());
// 			    if (relerr>0.5) fillTH2F(HRichHistFac::getHMomCorrCodeGeantBasename(),samplenametmp3.Data(),
// 						     t->getTotMom(part),nCorrCode*hs->getKickCharge());
// 			    if (relerr>0.5) fillTH2F(HRichHistFac::getHCorrCodeGeantPolarAngleBasename(),samplenametmp3.Data(),
// 						     nCorrCode*hs->getKickCharge(),hs->getMdcTheta());
// 			}

// 			if (t->getTotMom(part)>0. && hs->getKickMom()>0. )//&& hs->getKickBeta()>0.935 && TMath::Abs(hs->getKickPull())<3.)
// 			{
// 			    TString samplenametmp3(samplename);
// 			    samplenametmp3.Append(".fac6");
// 			    Float_t pratio = hs->getKickMom()/t->getTotMom(part);
// 			    Float_t th=3.;
// 			    if (pratio>th)
// 			    {
// 				fillTH2F(HRichHistFac::getHGeantMomKickMomDiffBasename(),samplename.Data(),
// 					 t->getTotMom(part),pratio/th);
// 				fillTH1F(HRichHistFac::getHCorrCodeGeantBasename(),samplenametmp3.Data(),
// 							 nCorrCode*hs->getKickCharge());
// 				fillTH2F(HRichHistFac::getHPartCorrCodeGeantBasename(),samplenametmp3.Data(),
// 							 t->getParId(part),nCorrCode*hs->getKickCharge());
// 				fillTH2F(HRichHistFac::getHMomCorrCodeGeantBasename(),samplenametmp3.Data(),
// 							 t->getTotMom(part),nCorrCode*hs->getKickCharge());
// 				fillTH2F(HRichHistFac::getHCorrCodeGeantPolarAngleBasename(),samplenametmp3.Data(),
// 							 nCorrCode*hs->getKickCharge(),hs->getMdcTheta());
// 			    }
// 			}

		    } // end of check
		} // endfor particles in HTrackInfo
	    }//isSim
	    
	    // use a MDC-META combi only once, even if two kicktracks are different,cf. above !
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsMassBasename(),samplename.Data(),
					HRichUtilFunc::getDeflectionAngle(hi),hi->getKickMass());
	    //
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsMomBasename(),samplename.Data(),
					1./HRichUtilFunc::getDeflectionAngle(hi),
					hi->getKickMom()*hi->getKickCharge());
	    //
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsBetaBasename(),samplename.Data(),
					HRichUtilFunc::getDeflectionAngle(hi),hi->getKickBeta());

	}// end of check for new track (kick track)
    } // end of loop over recon tracks


    // Histogram the particles from GEANT Kine which were found in the tracks of the array
    if (isSim && geant)
    {

	TString samplename(trackArray->GetName());if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);

	TString samplenamemultgparts(samplename);appendName(samplenamemultgparts,"geantparticles");
	// multiplicity of geant particles found in this recon track sample
	fillTH1F(HRichHistFac::getHMultBasename(),samplenamemultgparts.Data(),geant->GetLast()+1);

	TString samplenamegeant(samplename);appendName(samplenamegeant,"geant");

	// loop over GEANT particles in this track sample
	for (Int_t ii=0;ii<geant->GetLast()+1;ii++) 
	{
	    HGeantParticleInfo *info = (HGeantParticleInfo*)((*geant)[ii]);
	    TString samplenamemultgeant(samplenamegeant);appendName(samplenamemultgeant,"trkperpart");
	    // multiplicity of recon tracks in which particle appears
	    fillTH1F(HRichHistFac::getHMultBasename(),samplenamemultgeant.Data(),info->getTrackNr());

	    Float_t weight = 1.;
	    Bool_t kFullTrack = kFALSE;
	    Int_t nFullTrackIdx = 0;
	    for (Int_t tmp=0;tmp<TRACKS;tmp++) 
	    {
		if (info->getCorrCode(tmp)==4) 
		{
		    kFullTrack=kTRUE;
		    nFullTrackIdx = info->getReconTrk(tmp);
		    break;
		}
	    }
	    if (kFullTrack==kFALSE && info->getTrackNr()>0.) weight = 1./info->getTrackNr();
	    for (Int_t track=nFullTrackIdx;track<TRACKS;track++)
	    {
		Int_t trk = info->getReconTrk(track);
		if (trk==-1) break;
		Int_t nCorrCode = info->getCorrCode(track);
		HHitMatchSim* hs = (HHitMatchSim*)((*trackArray)[trk]);
		HTrackInfo *t = (HTrackInfo*) hs->getTrackInfoObj();
		Int_t part = -1;
		for (Int_t partt=0;partt<MAXPARTICLES;partt++) {if (t->getTrkNb(partt)==info->getGeantTrackNr()) part=partt;}
		if (part==-1) {cout<<"Error: track number not found !"<<endl; break;}
		fillTH2F(HRichHistFac::getHMomCorrCodeGeantBasename(),samplenamegeant.Data(),
 			 t->getTotMom(part),nCorrCode*hs->getKickCharge(),weight);
		TString dsd(samplenamegeant);appendName(dsd,"reconmom");
		fillTH2F(HRichHistFac::getHMomCorrCodeGeantBasename(),dsd.Data(),
			 hs->getKickMom(),nCorrCode*hs->getKickCharge(),weight);

		fillTH1F(HRichHistFac::getHCorrCodeGeantBasename(),samplenamegeant.Data(),
			 nCorrCode*hs->getKickCharge(),weight);

		fillTH2F(HRichHistFac::getHPartCorrCodeGeantBasename(),samplenamegeant.Data(),
			 t->getParId(part),nCorrCode*hs->getKickCharge(),weight);

		fillTH2F(HRichHistFac::getHCorrCodeGeantPolarAngleBasename(),samplenamegeant.Data(),
			 nCorrCode*hs->getKickCharge(),hs->getMdcTheta(),weight);
		    
		fillTH1F(HRichHistFac::getHParIdBasename(),samplenamegeant.Data(),
			 (Float_t)t->getParId(part),weight);
		    
		TString samplenametmp(samplenamegeant);
		samplenametmp.Append(".RICH");
		fillTH2F(HRichHistFac::getHRingParIdWeightBasename(),samplenametmp.Data(),
					(Float_t)t->getParId(part),t->getRichWeight(part),weight);
		    
		TString samplenametmp2(samplenamegeant);
		samplenametmp2.Append(".MDC");
		fillTH2F(HRichHistFac::getHRingParIdWeightBasename(),samplenametmp2.Data(),
			 (Float_t)t->getParId(part),t->getMdcWeight(part),weight);
		
		fillTH2F(HRichHistFac::getHWeightWeightBasename(),samplenamegeant.Data(),
			 (Float_t)t->getRichWeight(part),t->getMdcWeight(part),weight);
		if (nCorrCode==4) break;
	    }
	}
    } // end of sim&&geant



    //    cout<<"now filling mult histo  "<<HRichHistFac::getHMultBasename()<<"   "<<samplename1.Data()<<endl;
//     cout<<" &&&&&&&&&&&&&&&&&&&&&&&&&&& "<<endl;
//     cout<<nTrackCounterep<<"  "<<nTrackCounterem<<"  "<<nTrackCounter0<<endl;
//     cout<<" &&&&&&&&&&&&&&&&&&&&&&&&&&&^^^^ "<<endl;
    if (nTrackCounter0) fillTH1F(HRichHistFac::getHMultBasename(),samplenametracks0.Data(),nTrackCounter0);
    if (nTrackCounterem) fillTH1F(HRichHistFac::getHMultBasename(),samplenametracksem.Data(),nTrackCounterem);
    if (nTrackCounterep) fillTH1F(HRichHistFac::getHMultBasename(),samplenametracksep.Data(),nTrackCounterep);
    if (nTrackCounterfmixed) fillTH1F(HRichHistFac::getHMultBasename(),samplenamemixfake.Data(),nTrackCounterfmixed);
    if (nTrackCounterffull) fillTH1F(HRichHistFac::getHMultBasename(),samplenamefullfake.Data(),nTrackCounterffull);
    if (nTrackCounterfmixedlep) fillTH1F(HRichHistFac::getHMultBasename(),samplenamemixlepfake.Data(),nTrackCounterfmixedlep);
    if (nTrackCounterfep) fillTH1F(HRichHistFac::getHMultBasename(),samplenameepfake.Data(),nTrackCounterfep);
    if (nTrackCounterfem) fillTH1F(HRichHistFac::getHMultBasename(),samplenameemfake.Data(),nTrackCounterfem);
    if (nTrackCounterfh) fillTH1F(HRichHistFac::getHMultBasename(),samplenamehfake.Data(),nTrackCounterfh);
    if (nTrackCounterep4) fillTH1F(HRichHistFac::getHMultBasename(),samplenamefulltrackep.Data(),nTrackCounterep4);
    if (nTrackCounterem4) fillTH1F(HRichHistFac::getHMultBasename(),samplenamefulltrackem.Data(),nTrackCounterem4);
    delete [] tupt;
    delete [] tups;
    delete [] kicks;

    if (isSim && geant)
    {
	for (Int_t ii=0;ii<geant->GetLast()+1;ii++) 
	    if ((HGeantParticleInfo*)((*geant)[ii])) 
		delete (HGeantParticleInfo*)((*geant)[ii]);
	delete geant;
    }
    if (kDetHTracks) if (!fillDetHistosFromTracklets(trackArray,trackArray->GetName())) Error("execute","fillDetHistosFromTracklets() failed");
	
    return ret_val;
}

Int_t HRichHistoFiller::fillTrackHistosFromPairs(TObjArray *pairs,const Char_t *cNamePairSample)
{
    Int_t ret_val=1;
    Int_t max = pairs->GetLast()+1;
    Int_t maxTrackArray = 2*max;
    Int_t *nNewTrack=new Int_t[maxTrackArray];
    for (Int_t i=0;i<maxTrackArray;i++) nNewTrack[i]=-2;
    TObjArray *tracks = new TObjArray(5);
    TString samplename(cNamePairSample);
    tracks->SetName(samplename.Data());
    for (Int_t i=0;i<max;i++)
    {

	HDiHitMatch *h = ((HDiHitMatch*)(*pairs)[i]);
	
   	if (HRichCut::isNewIndex(h->getIndTrk1(),nNewTrack,maxTrackArray)) 
	    tracks->Add((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1()));
   	if (HRichCut::isNewIndex(h->getIndTrk2(),nNewTrack,maxTrackArray)) 
	    tracks->Add((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2()));

    }

    if (kTrackHPairs) 
    {
	for (Int_t trackcuts=0;trackcuts<cutIdArrT->GetLast()+1;trackcuts++)
	{
	    TString n = ((TObjString*)((*cutIdArrT)[trackcuts]))->GetString();
	    TObjArray *t = 0;
	    if (!n.Contains("nocut")) 
	    {
		t = selectTracklets(n.Data(),tracks);
		fillTrackHistosFromTracklets(t,cNamePairSample);
		if (kDetHPairs)   fillDetHistosFromTracklets(t,cNamePairSample);
		delete t;
		if (kAntiHistos)
		{
		    t = selectTracklets(n.Data(),tracks,kTRUE);
		    fillTrackHistosFromTracklets(t,cNamePairSample);
		    if (kDetHPairs)   fillDetHistosFromTracklets(t,cNamePairSample);
		    delete t;
		}
	    }
	}
    }

    if (kTrackHPairs) fillTrackHistosFromTracklets(tracks,cNamePairSample);
    if (kDetHPairs)   fillDetHistosFromTracklets(tracks,cNamePairSample);
    delete tracks;
    delete nNewTrack;

    return ret_val;
}

Int_t HRichHistoFiller::fillPairHistosFromPairs(TObjArray *t,const Char_t *cNamePairSample)
{
    Int_t ret_val=1;
    Int_t max = t->GetLast()+1;
    Int_t maxTracks = 2*max;
    // helper arrays to determine unique nature of index tuples
    Int_t *nTrack2Tuple  = new Int_t[maxTracks];
    Int_t *nDalitz2Tuple = new Int_t[maxTracks];
    Int_t *nConv2Tuple   = new Int_t[maxTracks];
    for (Int_t i=0;i<maxTracks;i++) nTrack2Tuple[i]=nDalitz2Tuple[i]=nConv2Tuple[i]=-2;
    Int_t nuPairCounter = 0;
    Int_t nlpPairCounter = 0;
    Int_t nlmPairCounter = 0;
    Int_t ndalPairCounter = 0;
    Int_t netaPairCounter = 0;
    Int_t nconvPairCounter = 0;
    TString addtohistname("");
    TString upair(cNamePairSample);
    upair.Append(".Punlike");
    TString lppair(cNamePairSample);
    lppair.Append(".Plike+");
    TString lmpair(cNamePairSample);
    lmpair.Append(".Plike-");
    // for multiplicity
    TString dalpair(cNamePairSample);
    dalpair.Append(".Pdalitz");
    dalpair.Prepend("Pairs.");
    TString etapair(cNamePairSample);
    etapair.Append(".Peta");
    etapair.Prepend("Pairs.");
    TString convpair(cNamePairSample);
    convpair.Append(".Pconv");
    convpair.Prepend("Pairs.");
    TString pairsamplesim(cNamePairSample);
    TString *pairsample=0;
    HRichCutPairSim dalcut("paircut","default");
    dalcut.switchTo("isDalitzPair");
    HRichCutPairSim etacut("paircut","default");
    etacut.switchTo("isEtaDalitzPair");
    HRichCutPairSim convcut("paircut","default");
    convcut.switchTo("isConvPair");

    for (Int_t i=0;i<max;i++)
    {
	HDiHitMatchSim *hds = 0;
	HDiHitMatch *hdi = 0;
	if (isSim) hds = ((HDiHitMatchSim*)(*t)[i]);
	else hdi = ((HDiHitMatch*)(*t)[i]);
	if (isSim) hdi=hds;
	
	if (!hdi) continue;

	Bool_t isNewPair = HRichCut::isNew2Tuple(hdi->getIndTrk1(),hdi->getIndTrk2(),nTrack2Tuple,maxTracks);

	if (isNewPair)
	{
	    if (hdi->getCharge()==0) {pairsample = &upair; nuPairCounter++;}
	    if (hdi->getCharge()==2) {pairsample = &lppair; nlpPairCounter++;}
	    if (hdi->getCharge()==-2) {pairsample = &lmpair; nlmPairCounter++;} 

	    setHistNameAddition(addtohistname,"",pairsample->Data());
	    fillTH1F(HRichHistFac::getHInvMassBasename(),addtohistname.Data(),hdi->getInvMass());

	    // Kienle's histo

	    // 	    HHitMatch *hi = (HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1());
	    // 	    HHitMatch *hj = (HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2());
	    // 	    TLorentzVector* vi = ((TLorentzVector*)hi->getLVec());
	    // 	    TLorentzVector* vj = ((TLorentzVector*)hj->getLVec());
	    // 	    Float_t psum = (vi->Vect()+vj->Vect()).Mag();
	    // 	    Float_t esum = sqrt((vi->Vect()).Mag()*(vi->Vect()).Mag() + 0.511*0.511)+
	    // 		sqrt((vj->Vect()).Mag()*(vj->Vect()).Mag() + 0.511*0.511);
	    // 	    fillTH2F(HRichHistFac::getHMomvsEnergyBasename(),addtohistname.Data(),psum,esum);

 	    HHitMatch *hi = (HHitMatch*)getHitMatchCat()->getObject(hdi->getIndTrk1());
 	    HHitMatch *hj = (HHitMatch*)getHitMatchCat()->getObject(hdi->getIndTrk2());
 	    TLorentzVector* vi = ((TLorentzVector*)hi->getLVec());
 	    TLorentzVector* vj = ((TLorentzVector*)hj->getLVec());
	    TLorentzVector vsum = (*vi) + (*vj);
	    //	    setHistNameAddition(addtohistname,"",pairsample->Data());
	    
	    
// 	    fillTH1F(HRichHistFac::getHPolarAngleBasename(),addtohistname.Data(),
// 		     (hi->getMdcTheta()+hj->getMdcTheta())/2.);

// 	    fillTH1F(HRichHistFac::getHAzimuthalAngleBasename(),addtohistname.Data(),
// 		     (hi->getMdcPhi()+hj->getMdcPhi())/2.);

	    fillTH1F(HRichHistFac::getHPolarAngleBasename(),addtohistname.Data(),
		     vsum.Theta()*TMath::RadToDeg());

	    fillTH1F(HRichHistFac::getHAzimuthalAngleBasename(),addtohistname.Data(),
		     vsum.Phi()*TMath::RadToDeg());

	    fillTH1F(HRichHistFac::getHMomtBasename(),addtohistname.Data(),
		     vsum.Pt());

	    fillTH1F(HRichHistFac::getHRapidityBasename(),addtohistname.Data(),
		     vsum.Rapidity());

	    fillTH2F(HRichHistFac::getHMomtRapidityBasename(),addtohistname.Data(),
		     vsum.Rapidity(),vsum.Pt());

	    fillTH2F(HRichHistFac::getHInvMassRapidityBasename(),addtohistname.Data(),
		     vsum.Rapidity(),hdi->getInvMass());

	    fillTH2F(HRichHistFac::getHInvMassMomtBasename(),addtohistname.Data(),
		     vsum.Pt(),hdi->getInvMass());

	    fillTH2F(HRichHistFac::getHInvMassOpangBasename(),addtohistname.Data(),
		     hdi->getOpangKICK(),hdi->getInvMass());

	    fillTH2F(HRichHistFac::getHMomtOpangBasename(),addtohistname.Data(),
		     hdi->getOpangKICK(),vsum.Pt());

	    fillTH2F(HRichHistFac::getHRapidityOpangBasename(),addtohistname.Data(),
		     hdi->getOpangKICK(),vsum.Rapidity());

	    setHistNameAddition(addtohistname,"KICK",pairsample->Data());
	    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hdi->getOpangKICK());

	    setHistNameAddition(addtohistname,"MDC",pairsample->Data());
	    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hdi->getOpangMDC());
	    
	    if (isSim)
	    {
		if (dalcut.check(hds))
		{
		    Int_t a=0;
		    Int_t b=0;
		    dalcut.getDalitzPairTrkNb(hds,a,b);
		    Bool_t isNewDalPair = HRichCut::isNew2Tuple(a,b,nDalitz2Tuple,maxTracks);
		    if (isNewDalPair)
		    {
			ndalPairCounter++;
			setHistNameAddition(addtohistname,"dalitz",pairsample->Data());
			fillTH1F(HRichHistFac::getHInvMassBasename(),addtohistname.Data(),hds->getInvMass());
			fillTH1F(HRichHistFac::getHMomtBasename(),addtohistname.Data(),
				 vsum.Pt());
			setHistNameAddition(addtohistname,"KICKdalitz",pairsample->Data());
			fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hds->getOpangKICK());
			setHistNameAddition(addtohistname,"MDCdalitz",pairsample->Data());
			fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hds->getOpangMDC());
		    }
		    //hds->dumpToStdoutSim();
		}
		if (etacut.check(hds))
		{
// 		    Int_t a=0;
// 		    Int_t b=0;
// 		    dalcut.getDalitzPairTrkNb(hds,a,b);
// 		    Bool_t isNewEtaPair = HRichCut::isNew2Tuple(a,b,nDalitz2Tuple,maxTracks);
// 		    if (isNewDalPair)
// 		    {
			netaPairCounter++;
			setHistNameAddition(addtohistname,"eta",pairsample->Data());
			fillTH1F(HRichHistFac::getHInvMassBasename(),addtohistname.Data(),hds->getInvMass());
			fillTH1F(HRichHistFac::getHMomtBasename(),addtohistname.Data(),
				 vsum.Pt());
			setHistNameAddition(addtohistname,"KICKeta",pairsample->Data());
			fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hds->getOpangKICK());
			setHistNameAddition(addtohistname,"MDCeta",pairsample->Data());
			fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hds->getOpangMDC());
			//		    }
		    //hds->dumpToStdoutSim();
		}
		
		if (convcut.check(hds))
		{
		    Int_t a=0;
		    Int_t b=0;
		    dalcut.getConvPairTrkNb(hds,a,b);
		    Bool_t isNewConvPair = HRichCut::isNew2Tuple(a,b,nConv2Tuple,maxTracks);
		    if (isNewConvPair)
		    {
			nconvPairCounter++;
			setHistNameAddition(addtohistname,"conv",pairsample->Data());
			fillTH1F(HRichHistFac::getHInvMassBasename(),addtohistname.Data(),hds->getInvMass());
			fillTH1F(HRichHistFac::getHMomtBasename(),addtohistname.Data(),
				 vsum.Pt());
			setHistNameAddition(addtohistname,"KICKconv",pairsample->Data());
			fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hds->getOpangKICK());
			setHistNameAddition(addtohistname,"MDCconv",pairsample->Data());
			fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),hds->getOpangMDC());
		    }
		}
	    }
	    
	}	    
    }
    TString samplenamepairu(upair.Data());
    samplenamepairu.Prepend("Pairs.");
    TString samplenamepairlp(lppair.Data());
    samplenamepairlp.Prepend("Pairs.");
    TString samplenamepairlm(lmpair.Data());
    samplenamepairlm.Prepend("Pairs.");
    if (nuPairCounter) fillTH1F(HRichHistFac::getHMultBasename(),samplenamepairu.Data(),nuPairCounter);
    if (nlpPairCounter) fillTH1F(HRichHistFac::getHMultBasename(),samplenamepairlp.Data(),nlpPairCounter);
    if (nlmPairCounter) fillTH1F(HRichHistFac::getHMultBasename(),samplenamepairlm.Data(),nlmPairCounter);
    if (ndalPairCounter) fillTH1F(HRichHistFac::getHMultBasename(),dalpair.Data(),ndalPairCounter);
    if (netaPairCounter) fillTH1F(HRichHistFac::getHMultBasename(),etapair.Data(),netaPairCounter);
    if (nconvPairCounter) fillTH1F(HRichHistFac::getHMultBasename(),convpair.Data(),nconvPairCounter);
    delete [] nTrack2Tuple;
    delete [] nConv2Tuple;
    delete [] nDalitz2Tuple;
    if (kTrackHPairs) if (!fillTrackHistosFromPairs(t,t->GetName())) Error("execute","fillTrackHistosFromPairs() failed");
    
    return ret_val;
}

void HRichHistoFiller::fillTH1F(const Char_t *basename,const Char_t *addtohistname,Float_t val,Float_t weight)
{
    TString base(basename);
    base.Append(addtohistname);
    TH1F *h = (TH1F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH1F*) HRichHistFac::getHistoByBaseName(basename,addtohistname);
	pHistArray->Add(h);			
    }
    h->Fill(val,weight);
}
void HRichHistoFiller::fillTH2F(const Char_t *basename,const Char_t *addtohistname,Float_t val1,Float_t val2,Float_t weight)
{
    TString base(basename);
    base.Append(addtohistname);
    TH2F *h = (TH2F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH2F*) HRichHistFac::getHistoByBaseName(basename,addtohistname);
	if (h) pHistArray->Add(h);
    }
    h->Fill(val1,val2,weight);
}
void HRichHistoFiller::setHistNameAddition(TString &str,const Char_t* histname,const Char_t* samplename)
{
    str="";
    str.Append(histname);
    str.Append(".");
    str.Append(samplename);
}
void HRichHistoFiller::appendName(TString &s,const Char_t*c)
{
    s.Append(".");
    s.Append(c);
}
void HRichHistoFiller::prependName(TString &s,const Char_t*c)
{
    s.Prepend(c);
}

TObjArray* HRichHistoFiller::getSet(const TObjArray* superset,const Char_t *subsetIdentifier)
{// find and return subset array from superset array
    return (TObjArray*)superset->FindObject(subsetIdentifier);
}

void HRichHistoFiller::setCuts(const Char_t * cuts,TObjArray *t,const Char_t *fieldseperator)
{// fill array with cut names

    TString sCuts=cuts;

    Int_t len = sCuts.Length();
    if (len>1)
    {
	TString tmp;
	tmp="";
	for (Int_t ii=0; ii < len; ii++)
	{ 
	    TString s(sCuts[ii]);
	    if (!s.CompareTo(fieldseperator))
	    {
		t->Add(new TObjString(tmp.Data()));
		tmp="";
	    }
	    else
	    {
		tmp.Append(s);
	    }
	    if (ii==len-1) t->Add(new TObjString(tmp.Data()));
	}
    }
}

void HRichHistoFiller::makeCutCombinatorics(TObjArray* t,Int_t level,const Char_t* catId)
{
    // t : array with single cut subsets
    // level: depth of combination, 2 e.g. means that the maximum cut will be the combination of
    // single cuts
    // catId: Identifier for data to cut on, e.g. "T" tracklets or "P" pairs
    TString cid(catId);
    Int_t max = t->GetLast()+1;
    Int_t maxold=0;
    Int_t nC=max-level;
    if (max>1 && nC>=0)
    {// more than subset (for one or less no combinatorics)
	while (nC<max-1) //user def. lvl dependent iterations 
	{
	    nC++;
	    Int_t maxnew = t->GetLast()+1;
	    // combine each two subsets 
	    for (Int_t subsa=maxold;subsa<maxnew-1;subsa++)
	    {
		TString first(((TObjArray*)((*t)[subsa]))->GetName());
		TString cutCatIdentifier(first[0]);

		for (Int_t subsb=subsa+1;subsb<maxnew;subsb++)
		{
		    TString second(((TObjArray*)((*t)[subsb]))->GetName());
		    TObjArray *splitCSIs = splitSubsetName(second);
		    for (Int_t ss=0;ss<splitCSIs->GetLast()+1;ss++)
		    {
			TString secondhelper( (((TObjString*)((*splitCSIs)[ss]))->GetString()).Data() );
			if (!first.Contains(secondhelper.Data()))
			{
			    TString *finalname = buildArrName(secondhelper.Data(),getSet(t,first.Data()),
							      cutCatIdentifier.Data());
			    if (!finalname->Contains("nocut") && isNewSubset(*finalname,t))
			    {
				TObjArray *a = 0;
				if (!cid.CompareTo("T")) a=selectTracklets(secondhelper.Data(),getSet(t,first.Data()));
				if (!cid.CompareTo("P")) a=selectPairs(secondhelper.Data(),getSet(t,first.Data()));
				//cout<<a->GetName()<<endl;
				t->Add(a);
			    }
			    delete finalname;
			}
			delete (TObjString*)(*splitCSIs)[ss];
		    }
		    delete splitCSIs;
		}
	    }
	    maxold=maxnew;
	}
    }
}
Bool_t HRichHistoFiller::isNewSubset(TString &s,TObjArray *t)
{ // check whether cut combination given in s is already contained in a subset from t
    Bool_t ret_val = kTRUE;
    TObjArray *splitCSIs = splitSubsetName(s);
    Int_t nCuts = splitCSIs->GetLast()+1;
    Int_t nSets = t->GetLast()+1;

    for (Int_t tt=0;tt<nSets;tt++)
    {
	TString subsetname( ((TObjArray*)((*t)[tt]))->GetName() );
	Int_t nbCiS = countCutsInSubset(subsetname.Data());
	if (nbCiS>=nCuts)
	{
	    Int_t isContained=0;
	    for (Int_t ss=0;ss<nCuts;ss++)
	    {
		TString cutname( (((TObjString*)((*splitCSIs)[ss]))->GetString()).Data() );
		if (subsetname.Contains(cutname.Data())) isContained++;
	    }
	    if (isContained==nCuts) {ret_val=kFALSE;break;}
	}
    }

    for (Int_t ss=0;ss<nCuts;ss++) delete (TObjString*)(*splitCSIs)[ss];
    delete splitCSIs;

    return ret_val;
}

Int_t HRichHistoFiller::countCutsInSubset(const Char_t *n)
{
    // as each two cuts are seperated by a "." by def.
    // count nb of "." and add one
    TString s(n);
    Int_t nC=0;
    for (Int_t i=0;i<s.Length();i++) {TString ss(s[i]);if (!ss.CompareTo(".")) nC++;}
    return ++nC;
}

TObjArray* HRichHistoFiller::splitSubsetName(TString &s)
{// each two cuts are seperated by a "." by def.
    // return array with strings of cutnames
    TObjArray *t = new TObjArray(2);
    Int_t len = s.Length();
    TString tmp; tmp="";
    for (Int_t i=0;i<len;i++)
    {
	TString st(s[i]);
	if (!st.CompareTo("."))
	{
	    tmp.Remove(0,1);
	    t->Add(new TObjString(tmp.Data()));
	    tmp="";
	}
	else
	{
	    tmp.Append(st);
	}
	if (i==len-1) 
	{
	    tmp.Remove(0,1);
	    t->Add(new TObjString(tmp.Data()));
	}
    }

    return t;
}

TString* HRichHistoFiller::buildArrName(const Char_t* cutStateIdentifier,TObjArray *t,const Char_t* cutCatIdentifier)
{
    TString helper1(cutStateIdentifier); // e.g. cut name "goodRing"
    helper1.Prepend(cutCatIdentifier); // e.g. category identifier "T" for tracklets
    TString helper2;
    TString *s = 0;
    if(t)
    {
	helper2=t->GetName();
	helper2.Append(".");
	helper2.Append(helper1);
	s = new TString(helper2);
    }
    else s = new TString(helper1);
    return s;
}

TString* HRichHistoFiller::buildCutStateId(TObjArray* t,const Char_t* cutCatId)
{
    TString *s = new TString(cutCatId);
    for (Int_t i=0;i<t->GetLast()+1;i++)
    {
	TString tmp = ((TObjString*)((*t)[i]))->GetString();
	if (i>0 && !tmp.Contains("nocut")) {s->Append(".");s->Append(cutCatId);}
	if (!tmp.Contains("nocut")) s->Append(((TObjString*)((*t)[i]))->GetString());
    }
    return s;
}

void HRichHistoFiller::reconPi0Mass(void)
{// reconstruct pi0 mass from 4 converted gammas
    // function should not be here
    TObjArray *t = new TObjArray(4);


    HDiHitMatch *h=0;
    pIterDiMatchHit->Reset();
    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
    {
	if (
	    h->getCorrCode1()  >= 4      &&
	    h->getCorrCode2()  >= 4      &&
	    h->getKICKCharge() == 0      &&
	    h->getNbDMdcHit()  == 2      
	    )
	    t->Add(h);
    }
    if (t->GetLast()+1 < 2) return;
    Int_t max = (t->GetLast()+1)*2;
    Int_t *mdc = new Int_t[max]; 
    for (Int_t i=0;i<t->GetLast();i++)
    {
	HDiHitMatch *h1 = ((HDiHitMatch*)(*t)[i]);

	HHitMatchSim *h1i = (HHitMatchSim*)getHitMatchCat()->getObject(h1->getIndTrk1());
	HHitMatchSim *h1j = (HHitMatchSim*)getHitMatchCat()->getObject(h1->getIndTrk2());
	HTrackInfo *ti1   = h1i->getTrackInfoObj();
	HTrackInfo *ti2   = h1j->getTrackInfoObj();
	Bool_t k1=kFALSE;
	Bool_t k2=kFALSE;
	Bool_t goon=kFALSE;
	for (Int_t k=0;k<MAXPARTICLES;k++) if (ti1->getConvLep(k)==1) k1=kTRUE;
	for (Int_t l=0;l<MAXPARTICLES;l++) if (ti2->getConvLep(l)==1) k2=kTRUE;
	if (k1&&k2) goon=kTRUE;
	if (
	    goon 
	    //HRichCut::isNewIndex(h1i->getMdcInd(),mdc,max) &&
	    //HRichCut::isNewIndex(h1j->getMdcInd(),mdc,max) 
	    )
	{
	    h1i->dumpToStdoutSim();
	    h1j->dumpToStdoutSim();
	    TLorentzVector* v1i = ((TLorentzVector*)h1i->getLVec());
	    TLorentzVector* v1j = ((TLorentzVector*)h1j->getLVec());
	
	    TLorentzVector sum1 = *v1i + *v1j;


	    for (Int_t j=i+1;j<(t->GetLast()+1);j++)
	    {
		HDiHitMatch *h2 = ((HDiHitMatch*)(*t)[j]);

		HHitMatchSim *h2i = (HHitMatchSim*)getHitMatchCat()->getObject(h2->getIndTrk1());
		HHitMatchSim *h2j = (HHitMatchSim*)getHitMatchCat()->getObject(h2->getIndTrk2());

		HTrackInfo *ti11   = h2i->getTrackInfoObj();
		HTrackInfo *ti22   = h2j->getTrackInfoObj();
		Bool_t k11=kFALSE;
		Bool_t k22=kFALSE;
		Bool_t goon2=kFALSE;
		for (Int_t k=0;k<MAXPARTICLES;k++) if (ti11->getConvLep(k)==1) k11=kTRUE;
		for (Int_t l=0;l<MAXPARTICLES;l++) if (ti22->getConvLep(l)==1) k22=kTRUE;
		if (k11&&k22) goon2=kTRUE;
		if (
		    goon2
		    //HRichCut::isNewIndex(h2i->getMdcInd(),mdc,max) &&
		    //HRichCut::isNewIndex(h2j->getMdcInd(),mdc,max)
		    ) 


		{
		    h2i->dumpToStdoutSim();
		    h2j->dumpToStdoutSim();

		    TLorentzVector* v2i = ((TLorentzVector*)h2i->getLVec());
		    TLorentzVector* v2j = ((TLorentzVector*)h2j->getLVec());
	    
		    TLorentzVector sum2 = *v2i + *v2j;

		    Float_t opang = sum1.Angle(sum2.Vect());
		    Float_t invMass = 2.*sin(opang/2.)*sqrt( sum1.P()*sum2.P() );
		    fillTH1F(HRichHistFac::getHInvMassBasename(),"TAPS",invMass);
		    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),"TAPS",opang*57.3);
		}
	    }
	}

    }

    delete t;
    delete [] mdc;
}

TObjArray* HRichHistoFiller::filterPairs(TObjArray *in,Int_t *list)
{
    // input is an array of pairs and an array of integers
    // all pairs which contain a track with an index in the list are discarded
    // this is done by creating a new pair array in which the pairs are copied
    // that have passed the test.

    TObjArray *out = new TObjArray(in->GetLast()+1);
    out->SetName(in->GetName());
    Bool_t kDiscardPair = kFALSE;
    Int_t max = in->GetLast()+1;
    for (Int_t i=0;i<max;i++)
    {
	HDiHitMatch *h = ((HDiHitMatch*)(*in)[i]);

	Int_t trk1=h->getIndTrk1();
	Int_t trk2=h->getIndTrk2();

	for(Int_t j=0;j<(Int_t)(sizeof(*list)/sizeof(Int_t));j++)
	{
	    if (list[j]==-2) break;
	    if (trk1==list[j] || trk2==list[j]) kDiscardPair = kTRUE;
	    else kDiscardPair = kFALSE;
	}

	if (kDiscardPair == kFALSE) out->Add(h);

    }
    delete in;
    return out;
}
