// $Id: hrichhistofiller.cc,v 1.6 2003-04-04 13:15:45 eberl Exp $
// Last update by Thomas Eberl: 03/03/13 18:08:02
using namespace std;
#include "hrichhistofiller.h"
#include "hdihitmatch.h"
#include "hdihitmatchsim.h"
#include "hrichcut.h"
#include "htrackinfo.h"
#include "hrichcuttracklet.h"
#include "hrichcutpair.h"
#include "hrichcuttrackletsim.h"
#include "hrichcutpairsim.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatch.h"
#include "hhitmatchsim.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "hrichhistfac.h"
#include "hhitmatchsim.h"
#include "htrackinfo.h"
#include <TH1.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TKey.h>
#include <string.h>
#include <iostream> 
#include <iomanip>

ClassImp(HRichHistoFiller)

HRichHistoFiller::HRichHistoFiller(Text_t *name,Text_t *title, Char_t* filename,Char_t* cutfilein,Char_t* world) :
  HReconstructor(name,title)
{
    pFileName  = filename;// output filename for histos
    pInFileName = cutfilein;// file with cut objects 
    if (!strcmp(pInFileName,"")) pInFileName=0;
    //set flag for simulation or real data 
    TString s(world);
    if (!s.CompareTo("real")) isSim=kFALSE;
    else if (!s.CompareTo("simulation")) isSim=kTRUE;
    else Error("","neither real nor simulation data selected");

    // set several switched to their default value
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

    kAntiHistos = kTRUE;

    cutIdArrT  = new TObjArray(5);
    cutIdArrP  = new TObjArray(5);

    trackletcut =  paircut = 0;
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
	pFileOut = new TFile(pFileName,"RECREATE");
	if (pInFileName) pFileCutIn = new TFile(pInFileName);
	else pFileCutIn=0;
 	initCuts();
	nCounterProcessedNbEvents=0; //total nb of evts seen by this task
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
	    cuts->Add(trackcut);
	    HRichCutPairSim *paircut = new HRichCutPairSim("paircut","default");
	    cuts->Add(paircut);
	}
	else
	{
	    HRichCutTracklet *trackcut = new HRichCutTracklet("trackcut","default");
	    cuts->Add(trackcut);
	    HRichCutPair *paircut = new HRichCutPair("paircut","default");
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
    //cout<<"start evt"<<endl;
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


TObjArray* HRichHistoFiller::selectTracklets(const char* cutStateIdentifier,TObjArray *t,Bool_t kBar)
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


TObjArray* HRichHistoFiller::selectPairs(const char* cutStateIdentifier,TObjArray *t,Bool_t kBar)
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
		if (kBar)
		{
		    if (isSim && hs && !paircutosim->check(hs)) arr->Add(hs);
		    else if (!isSim && h && !paircuto->check(h)) arr->Add(h);
		}
		else
		{
		    if (isSim && hs && paircutosim->check(hs)) arr->Add(hs);
		    else if (!isSim && h && paircuto->check(h)) arr->Add(h);
		}
	    }


	    else if(isTrackletCut)
	    {// retrieve tracklets contained in pair, use check on these 
		// to decide on pair
		if (kBar)
		{
		    if(!isSim && h && 
		       !trackcut->check((HHitMatch*)getHitMatchCat()->
				       getObject(h->getIndTrk1())) &&
		       !trackcut->check((HHitMatch*)getHitMatchCat()->
				       getObject(h->getIndTrk2()))
		       ) arr->Add(h);
		    
		    if(isSim && hs && 
		       !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					  getObject(hs->getIndTrk1())) &&
		       !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					  getObject(hs->getIndTrk2()))
		       ) arr->Add(hs);
		}
		else
		{
		    if(!isSim && h && 
		       trackcut->check((HHitMatch*)getHitMatchCat()->
				       getObject(h->getIndTrk1())) &&
		       trackcut->check((HHitMatch*)getHitMatchCat()->
				       getObject(h->getIndTrk2()))
		       ) arr->Add(h);
		    
		    if(isSim && hs && 
		       trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					  getObject(hs->getIndTrk1())) &&
		       trackcutsim->check((HHitMatchSim*)getHitMatchCat()->
					  getObject(hs->getIndTrk2()))
		       ) arr->Add(hs);
		}
		
	    } 
	}
    }
    else
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
		    }
		}
		else 
		{
		    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
		    {
			if (paircuto->check(h)) arr->Add(h);
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
			    !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->getObject(hs->getIndTrk1())) && 
			    !trackcutsim->check((HHitMatchSim*)getHitMatchCat()->getObject(hs->getIndTrk2()))
			    ) arr->Add(hs);
		    }
		}
		else 
		{
		    while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
		    {
			if (
			    !trackcut->check((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1())) && 
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
		    }
		}
	    }
	}
    }
    return arr;

}


void HRichHistoFiller::fillHistos(TObjArray *subsets,char * catIdentifier)
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
		//cout<<t->GetName()<<" "<<countCutsInSubset(t->GetName())<<endl;
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
    for (Int_t j=0;j<max;j++) rings[j]=tofs[j]=-2;


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
		    fillTH2F(HRichHistFac::getHRingParIdWeightBasename(),samplename.Data(),
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

    delete [] rings;
    delete [] tofs;

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
 // for mom vs beta histo
 Int_t *kicks=new Int_t[max];
 // init helper arrays
 for (Int_t j=0;j<max;j++) tupt[j]=tups[j]=kicks[j]=-2;

 //cout<<"start loop"<<endl;
    for (Int_t i=0;i<max;i++)
    {
	
	HHitMatch *hi = ((HHitMatch*)(*trackArray)[i]);
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
	TString aa="";
	if (hi->getTofInd()>=0) aa="tof";
	else if (hi->getShowInd()>=0) aa="sho";
	//cout<<samplename.Data()<<" : "<<mdcidx<<" ("<<aa.Data()<<")  "<<metaidx<<endl;
	}
	
	// fill kick track histos for different kicktracks
	Int_t trackind=hi->getKickInd();
	Bool_t kNewTrack=HRichCut::isNewIndex(trackind,kicks,max);
	if(kNewTrack)
	{
	    //cout<<samplename.Data()<<" : "<<trackind<<endl;
	    samplename="";
	    samplename=trackArray->GetName();
	    if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);
	    //cout<<samplename.Data()<<" : "<<trackind<<endl;
	    //
	    fillTH2F(HRichHistFac::getHMomChrgvsBetaBasename(),samplename.Data(),
		     hi->getKickMom()*hi->getKickCharge(),hi->getKickBeta());
	    //
	    fillTH2F(HRichHistFac::getHMassvsBetaBasename(),samplename.Data(),
		     hi->getKickMass(),hi->getKickBeta());
	    //
	    fillTH2F(HRichHistFac::getHMassChrgvsBetaBasename(),samplename.Data(),
		     hi->getKickMass()*hi->getKickCharge(),hi->getKickBeta());
	    //
	    fillTH1F(HRichHistFac::getHMassBasename(),samplename.Data(),
		     hi->getKickMass());
	    //
	    // use a MDC-META combi only once, even if two kicktracks are different,cf. above !
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsMassBasename(),samplename.Data(),
				      HRichUtilFunc::getDeflectionAngle(hi),hi->getKickMass());
	    //
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsBetaBasename(),samplename.Data(),
				      HRichUtilFunc::getDeflectionAngle(hi),hi->getKickBeta());

	}
    }
    delete [] tupt;
    delete [] tups;
    delete [] kicks;

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
    Int_t *nTrack2Tuple=new Int_t[maxTracks];

    for (Int_t i=0;i<maxTracks;i++) nTrack2Tuple[i]=-2;

    TString addtohistname("");
    for (Int_t i=0;i<max;i++)
    {
	HDiHitMatch *h = ((HDiHitMatch*)(*t)[i]);

	Bool_t isNewPair = HRichCut::isNew2Tuple(h->getIndTrk1(),h->getIndTrk2(),nTrack2Tuple,maxTracks);

	if (isNewPair)
	{
	    setHistNameAddition(addtohistname,"KICK",cNamePairSample);
	    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),h->getOpangKICK());

	    setHistNameAddition(addtohistname,"MDC",cNamePairSample);
	    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),h->getOpangMDC());

	    setHistNameAddition(addtohistname,"",cNamePairSample);
	    fillTH1F(HRichHistFac::getHInvMassBasename(),addtohistname.Data(),h->getInvMass());

	    // Kienle's histo

// 	    HHitMatch *hi = (HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1());
// 	    HHitMatch *hj = (HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2());
// 	    TLorentzVector* vi = ((TLorentzVector*)hi->getLVec());
// 	    TLorentzVector* vj = ((TLorentzVector*)hj->getLVec());
// 	    Float_t psum = (vi->Vect()+vj->Vect()).Mag();
// 	    Float_t esum = sqrt((vi->Vect()).Mag()*(vi->Vect()).Mag() + 0.511*0.511)+
// 		sqrt((vj->Vect()).Mag()*(vj->Vect()).Mag() + 0.511*0.511);
// 	    fillTH2F(HRichHistFac::getHMomvsEnergyBasename(),addtohistname.Data(),psum,esum);

	}	    
    }

    delete [] nTrack2Tuple;
    if (kTrackHPairs) if (!fillTrackHistosFromPairs(t,t->GetName())) Error("execute","fillTrackHistosFromPairs() failed");
    
    return ret_val;
}

void HRichHistoFiller::fillTH1F(const char *basename,const char *addtohistname,Float_t val)
{
    TString base(basename);
    base.Append(addtohistname);
    TH1F *h = (TH1F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH1F*) HRichHistFac::getHistoByBaseName(basename,addtohistname);
	pHistArray->Add(h);			
    }
    h->Fill(val);
}
void HRichHistoFiller::fillTH2F(const char *basename,const char *addtohistname,Float_t val1,Float_t val2)
{
    TString base(basename);
    base.Append(addtohistname);
    TH2F *h = (TH2F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH2F*) HRichHistFac::getHistoByBaseName(basename,addtohistname);
	if (h) pHistArray->Add(h);
    }
    h->Fill(val1,val2);
}
void HRichHistoFiller::setHistNameAddition(TString &str,const Char_t* histname,const Char_t* samplename)
{
    str="";
    str.Append(histname);
    str.Append(".");
    str.Append(samplename);
}
void HRichHistoFiller::appendName(TString &s,const char*c)
{
    s.Append(".");
    s.Append(c);
}
void HRichHistoFiller::prependName(TString &s,const char*c)
{
    s.Prepend(c);
}

TObjArray* HRichHistoFiller::getSet(const TObjArray* superset,const char *subsetIdentifier)
{// find and return subset array from superset array
    return (TObjArray*)superset->FindObject(subsetIdentifier);
}

void HRichHistoFiller::setCuts(Char_t * cuts,TObjArray *t,Char_t *fieldseperator)
{// fill array with cut names

    TString sCuts=cuts;

    Int_t len = sCuts.Length();
    if (len>1)
    {
	TString tmp;
	tmp="";
	for (int ii=0; ii < len; ii++)
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

void HRichHistoFiller::makeCutCombinatorics(TObjArray* t,Int_t level,char* catId)
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

Int_t HRichHistoFiller::countCutsInSubset(const char *n)
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

TString* HRichHistoFiller::buildArrName(const char* cutStateIdentifier,TObjArray *t,const char* cutCatIdentifier)
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

TString* HRichHistoFiller::buildCutStateId(TObjArray* t,Char_t* cutCatId)
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
