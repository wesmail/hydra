//*-- Author : Jochen Markert 18.07.2007

#include "hpidtracksorter.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hpidgeanttrackset.h"
#include "hpidtrackcandsim.h"
#include "hpidtrackcand.h"
#include "hpidtrackdata.h"
#include "hpidhitdata.h"
#include "piddef.h"
#include "htool.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

#define DEBUG 0

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
//
// HPidTrackSorter
//
// This Class loops over the HPidTRackCand category and
// analyze the objects. Purpose of the procedure is to categorize
// the objects in the case of multiple usage of the single detector
// hits (RICH, InnerMDC, OuterMDC, META) as double hit and to provide
// a hint which of the candidates is the best with respect to certain
// criteria (described by the bit flags below). If a HPidTrackCand
// object has been marked before by an other task with HPidTrackCand::kIsUsed it
// will be ignored by the sort process and stay unchanged. For the description
// of the differnt set/get functions of the HPidTrackCand objects see the
// documentation.
// To use HPidTrackSorter without changing the flags of the HPidTrackCand objects
// (non persitent way inside a macro or reconstructor) one can call the
// backupFlags() function before and modification of the flags and
// restore the flags by calling restoreFlags() after the privat work has been
// done.
//
// For debugging purpose one cand set screen printouts to check the sorting
// (setDebug() + setPrintLevel(Int_t level)(level = 1-3)).
//
// The analysis of the HPidTrackCand objects is done in several steps:
//
// 1. Create the HPidTrackSorter object and call the HPidTrackSorter::Init()
//    To initialze the sorter object with category pointers etc ...
// 2. The internal structures are filled per event from the HPidTrackCand
//    category by calling the
//    Int_t HPidTrackSorter::fill(Bool_t (*function)(HPidTrackCand* ))
//    function which expects a function pointer to a function taking an
//    HPidTrackCand pointer as argument and returning a Bool_t if the conditions on
//    the HPidTrackCand object are fullfilled (like for example in
//    Bool_t HPidTrackSorter::selectHadrons(HPidTrackCand* pcand). For objects
//    failing the test the kIsRejected bit is set. The user can provide
//    his own selection functions to replace the build in selectLeptons(.....)
//    and selectHadrons(....) select functions.
//    The function pointer can be a pointer to a global function or member function
//    of an Object for example:
//
//    Bool_t select(HPidTrackCand* cand){             // global function
//             if ( put all selection criteria here ) return kTRUE;
//             else                                   return kFALSE;
//    } or
//
//    static Bool_t dummy::select(HPidTrackCand* cand){ // member function of object dummy
//                                                      // needs to be declared static !
//             if ( put all selection criteria here ) return kTRUE;
//             else                                   return kFALSE;
//    }
//    would be called in the code like
//
//    dummy d;
//    HPidTrackCand* p= new HPidTrackCand() // just to get an object
//    HPidTrackSorter sorter;               // create the sorter object
//    // now we have 3 possibilities to call the
//    // test function
//    1. sorter.fill(select)        // global function
//    2. sorter.fill(dummy::select) // member function of object dummy (static call without object creation)
//    3. sorter.fill(d.select)      // member function of real object dummy
// 3. For all accepted objects the quality criteria and double hit flags are
//    evaluated in within the selected sample and flagged to the HPidTrackCand objects.
//    The objects which fullfill the given minimum criteria the kIsAcceptedXXX bit is
//    flagged. For the RICH, InnerMDC, OuterMDC and META the kIsDoubleHitXXX bit are set
//    if the same detector hit index has been used more than once in the sample.
// 4. With the call of
//    Int_t HPidTrackSorter::selectBest(HPidTrackSorter::ESwitch byQuality, Int_t byParticle)
//    the final selection on the objects is performed. The by the fill() procedure
//    selected objects are sorted by the selected quality criteria. Starting from the
//    best object one goes down the list and marks all objects with the kIsUsed and
//    kIsBestXXX bit if they do not reuse an already used detector hit index
//    (double hit rejection). In case Int_t byParticle == kIsLepton the RICH index
//    is taken into account, otherwise not. For leptons the kIsLepton bit is flagged
//    in addition. Individual detector hits can be ignored in the selection procedure with
//    the setIgnoreXXX(kTRUE) functions (by default all hits are taken into account)
//    HPidTrackSorter::ESwitch byQuality can be used as quality criteria switch for
//
//          kIsBestHitRich          (by number of pads)
//          kIsBestHitInnerMdc      (by chi2, non fitted segments last)
//	    kIsBestHitOuterMdc      (by chi2, non fitted segments last)
//	    kIsBestHitMeta          (by metamatch quality)
//          kIsBestRKMeta           (by RK metamatch quality)
//          kIsBestRKRICH           (by RK RICH match quality)
//          kIsBestRK               (by RK chi2, none fitted outer segments with lower priority)
//          kIsBestSPLINE           (by chi2)
//          kIsBestKICK             (by pull)
//          kIsBestRKRKMETA         (by RK chi2 * RK META match quality)
//          kIsBestRKRKRICH         (by RK chi2 * RK RICH match quality)
//          kIsBestRKMETAQA         (by RK chi2 * META match quality)
//
// 5. The scheme allows for multiple selection following each other as
//    the already used objects are excluded from the next steps (for example first
//    select leptons and in the second iteration hadrons). The detector hit indicies
//    of the previous selection of kIsUsed marked objects are added to list of used
//    indicies by default not allowing to use a detector hit twice. To skip that
//    memory one can set setIgnorePreviousIndex(kTRUE).
// 6. After the analysis call HPidTrackSorter::finalize(). After the selection
//    the user has to take into account only HPidTrackCand objects which  have
//    been flagged with kIsUsed. The objects slected by the Lepton selection will
//    be marked in addition with kIsLepton to identify them easily.
// 7. For lepton selections the internal selectLepton() function cand be manipulated.
//    With setRICHMatching(HPidTrackSorter::EMatch ,Float_t window) the matching between
//    inner MDC and RICH hit can be selected (default: HPidTrackSorter::kUseRKRICHWindow,4 degree ).
//
//    A selection of unique lepton and hadron tracks could look like
//    the following example :
//
// void myMacro(Int_t nEvents,TString inputDir, TString inFile)
// {
//    Hades* myHades = new Hades;
//    gHades->setQuietMode(2);
//
//    HRootSource* source = new HRootSource();
//    source->setDirectory(((Text_t *)inputDir.Data()));
//    source->addFile((Text_t *)inFile.Data());
//
//    myHades->setDataSource(source);
//
//    if(!myHades->init()){
//	cout<<"Hades Init() failed!"<<endl;
//	exit(1);
//    }
//
//
//    //------------------------------------------------------------------------
//    // get category ointers and iterators
//    HCategory* pidCat  = (HCategory*)gHades->getCurrentEvent()->getCategory(catPidTrackCand);
//    HIterator* iter    = NULL;
//    if(pidCat)
//    {
//         iter = (HIterator *)pidCat->MakeIterator("native");
//    } else {
//        cout<<"Category Pointer is NULL!"<<endl;
//	exit(1);
//    }
//    //------------------------------------------------------------------------
//
//    --------------------------------------------------------------------------------------------
//    //--------------------------CONFIGURATION---------------------------------------------------
//    //At begin of the program (outside the event loop)
//    HPidTrackSorter sorter;
//    //sorter.setOutputFile("testpidcleaner.root");                  // for debug ntuple output
//    //sorter.setDebug();                                            // for debug
//    //sorter.setPrintLevel(3);                                      // max prints
//    //sorter.setRICHMatching(HPidTrackSorter::kUseRKRICHWindow,4.); // select matching RICH-MDC for selectLeptons() function
//    //sorter.setIgnoreRICH();                                       // do not reject double RICH hits
//    //sorter.setIgnoreInnerMDC();                                   // do not reject double inner MDC hits
//    //sorter.setIgnoreOuterMDC();                                   // do not reject double outer MDC hits
//    //sorter.setIgnoreMETA();                                       // do not reject double META hits
//    //sorter.setIgnorePreviousIndex();                              // do not reject indices from previous selctions
//    sorter.init();                                                  // get catgegory pointers etc...
//    --------------------------------------------------------------------------------------------
//
//
//    //------------------------------------------------------------------------
//    // Event loop
//    for(Int_t i = 1;i < nEvents; i ++)
//    {
//        //----------break if last event is reached-------------
//        if(!gHades->eventLoop(1)) break;
//
//        //------------------------------------------------------------------------
//        // run your selection and set all flags
//
//        // if you do not want to modify the flags
//        // for the following tasks etc (needs call of
//        // restoreFlags() at the end of the eventloop)
//        //sorter.backupFlags();
//
//        sorter.resetFlags(kTRUE,kTRUE,kTRUE,kTRUE);     // reset all flags for flags (0-28) ,reject,used,lepton
//        Int_t nCandLep     = sorter.fill(HPidTrackSorter::selectLeptons);   // fill only good leptons
//        Int_t nCandLepBest = sorter.selectBest(HPidTrackSorter::kIsBestRKRKMETA,HPidTrackSorter::kIsLepton);
//        Int_t nCandHad     = sorter.fill(HPidTrackSorter::selectHadrons);   // fill only good hadrons (already marked good leptons will be skipped)
//        Int_t nCandHadBest = sorter.selectBest(HPidTrackSorter::kIsBestRKRKMETA,HPidTrackSorter::kIsHadron);
//        //------------------------------------------------------------------------
//
//        //------------------------------------------------------------------------
//        // analylize marked tracks (kIsUsed) ....
//    	  iter->Reset();
//        HPidTrackCand* pcand;
//        while((pcand = (HPidTrackCandSim*)iter->Next()) != NULL)
//	  {
//           // accept only objects marked as good
//	     if(!pcand->isFlagBit(HPidTrackCand::kIsUsed) ) continue;
//
//           // do other selection......
//        }
//        //------------------------------------------------------------------------
//
//        // if you do not want to modify the flags
//        // for the following tasks etc (needs call of
//        // backupFlags() at the beginning of the eventloop)
//        // sorter.restoreFlags();
//    }
//    //------------------------------------------------------------------------
//    //At the end of program
//    sorter.finalize();
// }
////////////////////////////////////////////////////////////////////////////////

ClassImp(HPidTrackSorter)

Bool_t HPidTrackSorter::kDebug         = kFALSE;
Int_t  HPidTrackSorter::printLevel     = 1;

Int_t    HPidTrackSorter::kSwitchIndex        = 0;
Int_t    HPidTrackSorter::kSwitchQuality      = 0;
Int_t    HPidTrackSorter::kSwitchParticle     = 0;
Int_t    HPidTrackSorter::kSwitchRICHMatching = kUseRKRICHWindow;
Float_t  HPidTrackSorter::fRICHMDCWindow      = 4.;

Bool_t  HPidTrackSorter::kIgnoreRICH          = kFALSE;
Bool_t  HPidTrackSorter::kIgnoreInnerMDC      = kFALSE;
Bool_t  HPidTrackSorter::kIgnoreOuterMDC      = kFALSE;
Bool_t  HPidTrackSorter::kIgnoreMETA          = kFALSE;
Bool_t  HPidTrackSorter::kIgnorePreviousIndex = kFALSE;

  

HPidTrackSorter::HPidTrackSorter(void)
    : TNamed("PidTrackSorter","PID track candidate sorter")
{
    clear();
}

HPidTrackSorter::HPidTrackSorter(TString name, TString title)
: TNamed(name, title)
{
    clear();
}

HPidTrackSorter::~HPidTrackSorter(void)
{
    if(nameIndex)   delete [] nameIndex;
    if(nameQuality) delete [] nameQuality;
    if(iterPidTrackCandCat) delete iterPidTrackCandCat;
}

void HPidTrackSorter::clear()
{
    // initialize the member variables
    fout         = NULL;
    nt           = NULL;
    isSimulation = kFALSE;
    nameIndex    = 0;
    nameQuality  = 0;
    pPidTrackCandCat     =  NULL;
    iterPidTrackCandCat  =  NULL;
    fill_Iteration       =  0;
    selectBest_Iteration =  0;
    currentEvent         = -1;
}

Bool_t HPidTrackSorter::init(void)
{
    // get the pointer to the HPidTrackCand categegory and create the
    // corresponding iterator.

    pPidTrackCandCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);
    if (!pPidTrackCandCat) {
	Error("HPidTrackSorter::init()"," No HPidTrackCand Input -> Switching HPidTrackSorter OFF");
    }
    else iterPidTrackCandCat = (HIterator *) pPidTrackCandCat->MakeIterator();

    HCategory* catKine = gHades->getCurrentEvent()->getCategory(catGeantKine);
    if(catKine) isSimulation = kTRUE;

    if(kDebug) {

	Int_t size;
	nameIndex   = HTool::parseString("kIsIndexRICH,kIsIndexInnerMDC,kIsIndexOuterMDC,"
					 "kIsIndexTOF,kIsIndexSHOWER,kIsIndexRPC,kIsIndexMETA",size,",",kFALSE);
        cout<<"-------------------------------------------"<<endl;
	cout<<"HPidTrackSorter::init() :"<<endl;
	cout<<"index :: " <<endl;
	for(Int_t i = 0; i < size; i ++){
	    cout<<i <<" "<<nameIndex[i].Data()<<endl;
	}
	nameQuality = HTool::parseString("kIsBestHitRICH,kIsBestHitInnerMDC,kIsBestHitOuterMDC,kIsBestHitMETA,"
					 "kIsBestRKMETA,kIsBestRKRICH,kIsBestRK,kIsBestSPLINE,kIsBestKICK,"
					 "kIsBestRKRKMETA,kIsBestRKRKRICH,kIsBestRKMETAQA",size,",",kFALSE);
        for(Int_t i = 0; i < size; i ++){
	    cout<<i + NUMBER_OF_INDEX<<" "<<nameQuality[i].Data()<<endl;
	}
	cout<<"-------------------------------------------"<<endl;
    }

     return kTRUE;
}

Bool_t HPidTrackSorter::finalize(void)
{
    // clean up the memory and
    // write ntuple to file (if it exist)
    cleanUp();
    if(fout)
    {
	fout->cd();
	nt  ->Write();
        fout->Close();
    }
    return kTRUE ;
}

Int_t HPidTrackSorter::clearVector( vector<candidate*>& all_candidates)
{
    // delete all candidate structs in vector
    // from heap and call the clear function of
    // vector afterwards.
    for(UInt_t i = 0; i < all_candidates.size() ; ++ i)
    {
	candidate* cand = all_candidates[i];
        delete cand;
    }
    all_candidates.clear();
    return 0;
}


Bool_t HPidTrackSorter::cmpIndex(candidate* a, candidate* b)
{
    // sorts the candidate list ascending by the index
    // of the detector hit of interest.

    switch (kSwitchIndex) {
    case kIsIndexRICH:
	// sort in ascending order by Rich index
	return a->ind_RICH     < b->ind_RICH;
    case kIsIndexInnerMDC:
	// sort in ascending order by inner MDC segment index
	return a->ind_innerMDC < b->ind_innerMDC;
    case kIsIndexOuterMDC:
	// sort in ascending order by outer MDC segment index
	return a->ind_outerMDC < b->ind_outerMDC;
    case kIsIndexTOF:
	// sort in ascending order by TOF index
	return a->ind_TOF      < b->ind_TOF;
    case kIsIndexSHOWER:
	// sort in ascending order by SHOWER index
	return a->ind_SHOWER   < b->ind_SHOWER;
    case kIsIndexRPC:
	// sort in ascending order by RPC index
	return a->ind_RPC      < b->ind_RPC;

    default:
	::Error("HPidTrackSorter::CmpIndex()","Unknown switch statement %i!",kSwitchIndex);
	return kFALSE;

    }
}

Bool_t HPidTrackSorter::cmpQuality(candidate* a, candidate* b)
{
    // sorts the candidate list by the quality critera
    // of interest (ascending or decending depending on
    // the quantity). The best object is always the first.

    switch (kSwitchQuality) {
    case kIsBestHitRICH:
	// sort in descending order by Ring pad number
	return a->ring_PadNr            > b->ring_PadNr;
    case kIsBestHitInnerMDC:
	// sort in ascending order by inner MDC chi2
	return a->innerMDCChi2          < b->innerMDCChi2;
    case kIsBestHitOuterMDC:
	// sort in ascending order by outer MDC chi2
	return a->outerMDCChi2          < b->outerMDCChi2;
    case kIsBestHitMETA:
	// sort in ascending order by METAMATCH QUALITY
	return a->metamatch_Quality     < b->metamatch_Quality;
    case kIsBestRKMETA:
	// sort in ascending order by RK METAMATCH QUALITY
        return a->RK_META_match_Quality < b->RK_META_match_Quality;
    case kIsBestRKRICH:
	// sort in ascending order by RK RICH MATCH QUALITY
	return a->RK_RICH_match_Quality < b->RK_RICH_match_Quality;
    case kIsBestRK:
        // sort in ascending order by RK chi2
	return a->RK_Chi2               < b->RK_Chi2;
    case kIsBestSPLINE:
	// sort in ascending order by SPLINE chi2
	return a->SPLINE_Chi2           < b->SPLINE_Chi2;
    case kIsBestKICK:
        // sort in ascending order by KICK pull
	return a->KICK_Pull             < b->KICK_Pull;
    case kIsBestRKRKMETA:
	// sort in ascending order by RK_Chi2 * RK_META_match_Quality
        return (a->RK_Chi2 * a->RK_META_match_Quality)  < (b->RK_Chi2 * b->RK_META_match_Quality);
    case kIsBestRKRKRICH:
	// sort in ascending order by RK_Chi2 * RK_RICH_match_Quality
	return (a->RK_Chi2 * a->RK_RICH_match_Quality)  < (b->RK_Chi2 * b->RK_RICH_match_Quality);
    case kIsBestRKMETAQA:
	// sort in ascending order by RK_Chi2 * META match_Quality
	return (a->RK_Chi2 * a->metamatch_Quality)      < (b->RK_Chi2 * b->metamatch_Quality);
    default:
	::Error("HPidTrackSorter::CmpQuality()","Unknown switch statement %i!",kSwitchQuality);
        return kFALSE;

    }
}


Bool_t HPidTrackSorter::rejectIndex(candidate* a, HPidTrackSorter::ESwitch byIndex, Int_t& index)
{
    // if function return kTRUE the Object will be rejected
    // rejects candidates from the actual list, where the
    // detector hit of interest is not defined.

    switch (byIndex) {
    case kIsIndexRICH:
	// NO RICH
	return ( index = a->ind_RICH )     < 0;
    case kIsIndexInnerMDC:
	// NO inner MDC segment
	return ( index = a->ind_innerMDC ) < 0;
    case kIsIndexOuterMDC:
	// NO outer MDC segment
	return ( index = a->ind_outerMDC ) < 0;
    case kIsIndexTOF:
	// NO TOF
	return ( index = a->ind_TOF )      < 0;
    case kIsIndexSHOWER:
	// NO SHOWER
	return ( index = a->ind_SHOWER )   < 0;
    case kIsIndexRPC:
	// NO RPC
	return ( index = a->ind_RPC )      < 0;
    default:
	::Error("HPidTrackSorter::rejectIndex()","Unknown switch statement %i!",byIndex);
        return kFALSE;
    }
}
Bool_t HPidTrackSorter::rejectQuality(candidate* a, HPidTrackSorter::ESwitch byQuality)
{
    // if function return kTRUE the Object will be rejected
    // rejects the candidates from the actual list if the quality
    // of interest does not match the mimnum requirement.

    switch (byQuality) {
    case kIsBestHitRICH:
	// No Hit
	return a->ind_RICH < 0;
    case kIsBestHitInnerMDC:
	// NO fitted inner MDC segments
	return a->innerMDCChi2 < 0;
    case kIsBestHitOuterMDC:
	// NO fitted outer MDC segments
	return (a->outerMDCChi2 < 0 || a->ind_outerMDC < 0);
    case kIsBestHitMETA:
	// NO META
	return (a->ind_TOF < 0 && a->ind_SHOWER < 0 && a->ind_RPC < 0);
    case kIsBestRKMETA:
	// NO META or NO RK
	return (a->RK_Chi2 == 1000000. || a->RK_Chi2< 0 || (a->ind_TOF < 0 && a->ind_SHOWER < 0 && a->ind_RPC < 0));
    case kIsBestRKRICH:
	// NO RICH or NO RK
	return (a->RK_Chi2 == 1000000. || a->RK_Chi2< 0 || a->ind_outerMDC < 0 || a->ind_RICH < 0);
    case kIsBestRK:
	// NO outer MDC segment or NO RK
	return (a->RK_Chi2 == 1000000. || a->RK_Chi2< 0);
    case kIsBestSPLINE:
	// NO outer MDC segment or NO SPLINE
	return (a->SPLINE_Chi2 < 0 || a->ind_outerMDC < 0);
    case kIsBestKICK:
        // NO META or NO KICK
	return (a->KICK_Pull == -1 || (a->ind_TOF < 0 && a->ind_SHOWER < 0 && a->ind_RPC < 0));
    case kIsBestRKRKMETA:
	// NO META or NO RK
        return (a->ind_TOF < 0 && a->ind_SHOWER < 0 && a->ind_RPC < 0) || (a->RK_Chi2 == 1000000. || a->RK_Chi2< 0);
    case kIsBestRKRKRICH:
	// NO RICH or NO RK
	return (a->ind_RICH < 0) || (a->RK_Chi2 == 1000000. || a->RK_Chi2< 0);
    case kIsBestRKMETAQA:
	// NO META or NO RK
	return (a->ind_TOF < 0 && a->ind_SHOWER < 0 && a->ind_RPC < 0) || (a->RK_Chi2 == 1000000. || a->RK_Chi2< 0);

    default:
	::Error("HPidTrackSorter::rejectQuality()","Unknown switch statement %i!",byQuality);
        return kFALSE;

    }
}

Int_t HPidTrackSorter::flagAccepted(vector <candidate*>& all_candidates, HPidTrackSorter::ESwitch byQuality)
{
    // flag all candidates which does satisfy the quality
    // criteria from input with kIsAcceptedXXX bit. If kDebug
    // is set and printLevel >=2 the statistics for the accepted
    // objects is printed.


    Int_t ctAll           = 0;
    Int_t ctAcceptQuality = 0;
    HPidTrackCand* pcand;
    for(UInt_t i = 0; i < all_candidates.size(); ++ i)
    {
        ++ ctAll;
	candidate* cand = all_candidates[i];
	if(rejectQuality(cand, byQuality))      continue;
	++ ctAcceptQuality;

	pcand = (HPidTrackCand*) pPidTrackCandCat->getObject(cand->ind_PidCand);
	if(!pcand) {
	    Error("HPidTrackSorter::flagAccepted()","NULL pointer retrieved for HPidTrackCand at index %i",cand->ind_PidCand);
	    continue;
	}
	switch (byQuality) {
	case kIsBestHitRICH:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedHitRICH);
	    break;
	case kIsBestHitInnerMDC:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedHitInnerMDC);
	    break;
	case kIsBestHitOuterMDC:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedHitOuterMDC);
	    break;
	case kIsBestHitMETA:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedHitMETA);
	    break;
	case kIsBestRKMETA:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedRKMETA);
	    break;
	case kIsBestRKRICH:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedRKRICH);
	    break;
	case kIsBestRK:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedRK);
	    break;
	case kIsBestSPLINE:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedSPLINE);
	    break;
	case kIsBestKICK:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedKICK);
	    break;
	case kIsBestRKRKMETA:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedRKRKMETA);
	    break;
	case kIsBestRKRKRICH:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedRKRKRICH);
	    break;
	case kIsBestRKMETAQA:
	    pcand -> setFlagBit(HPidTrackCand::kIsAcceptedRKMETAQA);
	    break;
	default :

	    Error("HPidTrackSorter::flagAccepted()","Unknown switch statement : %i!",byQuality);
	    exit(1);
	    break;
	}

    }
    if(kDebug && printLevel >= 2){
	cout<<"flagAccepted(): "
	    <<"s: "                  <<selectBest_Iteration
	    <<", f: "                <<fill_Iteration
	    <<", kSwitchParticle: "  <<kSwitchParticle
	    <<", quality "           <<nameQuality[byQuality-NUMBER_OF_INDEX].Data()
	    <<", candidate= "        <<ctAll
	    <<", acc. quality = "    <<ctAcceptQuality
	    <<endl;

    }
    return 0;
}

Int_t HPidTrackSorter::flagDouble(vector <candidate*>& all_candidates, HPidTrackSorter::ESwitch byIndex)
{
    // sort vector in ascending order by the particular index byindex. Loops
    // over the vector and extract objects with same detetector index and marks them
    // as double hits in HPidTrackCand (kIsDoubleHitXXX bit). Non valid detecor
    // hits (index -1) are not taken into account. If kDebug
    // is set and printLevel >=2 the statistics for the accepted
    // objects is printed.

    kSwitchIndex   = byIndex;
    sort(all_candidates.begin(), all_candidates.end(), this->cmpIndex);


    Int_t index;
    Int_t old_index =-99;
    vector <candidate*> daughter;

    Int_t ctAll          = 0;
    Int_t ctAcceptIndex  = 0;
    Int_t ctAcceptDoubles= 0;
    Int_t ctCheck        = 0;

    for(UInt_t i = 0; i < all_candidates.size(); ++ i)
    {
        ++ ctAll;
	candidate* cand = all_candidates[i];
	if(rejectIndex  (cand, byIndex, index)) continue;
        ++ ctAcceptIndex;
	if(index == old_index || old_index == -99)
	{ // still same index
	    daughter.push_back(cand);
	} else {
            // finalize list
	    if(!daughter.empty())
	    {
                setFlagsDouble(daughter,byIndex);
		if(daughter.size() >= 2){
		    ctAcceptDoubles += daughter.size();
		}
		ctCheck += daughter.size();
		daughter.clear();
	    }
	    daughter.push_back(cand);
	}
	old_index = index;
    }
    // finalize list last group
    if(!daughter.empty())
    {
	setFlagsDouble(daughter, byIndex);
	if(daughter.size() >= 2){
	    ctAcceptDoubles += daughter.size();
	}
	ctCheck += daughter.size();
    }
    if(kDebug && printLevel >= 2){
	cout<<"flagDouble(): "
	    <<"s: "                <<selectBest_Iteration
	    <<", f: "              <<fill_Iteration
	    <<", kSwitchParticle: "<<kSwitchParticle
	    <<", index "           <<nameIndex  [byIndex].Data()
	    <<", candidate= "      <<ctAll
	    <<", acc. index = "    <<ctAcceptIndex
            <<", acc. doubles = "  <<ctAcceptDoubles
            <<", check = "         <<ctCheck
	    <<endl;

    }
    return 0;

}

Int_t HPidTrackSorter::setFlagsDouble(vector<candidate*>& daughter, HPidTrackSorter::ESwitch byIndex)
{
    // flag the HPidTrackCand objects with the rank of the double hit criteria as output
    // form the sorting algorithms. Expects as input a vector of candidates with the
    // same hit index.

    if(daughter.empty()   ) return 0;    // nothing to do
    if(daughter.size() < 2) return 0;    // no double hit


    HPidTrackCand* cand;
    for(UInt_t i = 0; i < daughter.size(); ++ i)
    {  // loop over list and set flags
	cand = (HPidTrackCand*) pPidTrackCandCat->getObject(daughter[i]->ind_PidCand);
	if(!cand) {
	    Error("HPidTrackSorter::setFlagsDouble()","NULL pointer retrieved for HPidTrackCand at index %i",daughter[i]->ind_PidCand);
	    continue;
	}
	switch (byIndex)
	{
	case kIsIndexRICH:
	    cand -> setFlagBit(HPidTrackCand::kIsDoubleHitRICH);
	    break;
	case kIsIndexInnerMDC:
	    cand -> setFlagBit(HPidTrackCand::kIsDoubleHitInnerMDC);
	    break;
	case kIsIndexOuterMDC:
	    cand -> setFlagBit(HPidTrackCand::kIsDoubleHitOuterMDC);
	    break;
	case kIsIndexTOF:
	    cand -> setFlagBit(HPidTrackCand::kIsDoubleHitMETA);
	    break;
	case kIsIndexSHOWER:
	    cand -> setFlagBit(HPidTrackCand::kIsDoubleHitMETA);
	    break;
	case kIsIndexRPC:
	    cand -> setFlagBit(HPidTrackCand::kIsDoubleHitMETA);
	    break;
	default :

	    Error("HPidTrackSorter::setFlagsDouble()","Unknown switch statement : %i!",byIndex);
	    exit(1);
	    break;

	}
    }

    return 0;
}
Int_t HPidTrackSorter::fillInput(vector < candidate* >& all_candidates)
{
    // loop over HPidTrackCand category and fill a vector of temporary objects
    // which will be used for sorting and flagging of the objects in the end.
    // The candidate objects are initialzied with values of interest in a well
    // defined way, which makes the sorting and rejecting of objects later on easier.

    HPidTrackCand* pCand ;
    HPidTrackData* tdata ;
    HPidHitData*   hdata ;

    candidate* cand;
    iterPidTrackCandCat->Reset();
    while ((pCand = (HPidTrackCand *)iterPidTrackCandCat->Next()) != 0 )
    {

	tdata = pCand->getTrackData();
        hdata = pCand->getHitData();

	if(pCand->isFlagBit(HPidTrackCand::kIsUsed))
	{
	    if(!kIgnorePreviousIndex)
	    {
		//-------------------------------------------------------------------
		// add detector indices of already used objects to
		// the current index list
		if(hdata->getIndRICH()     >= 0 &&
		   (find(index_RICH.begin(),index_RICH.end()        ,hdata->getIndRICH())     != index_RICH.end()) ){
		    index_RICH.push_back(hdata->getIndRICH());
		}
		if(hdata->getIndInnerSeg() >= 0 &&
		   (find(index_InnerMDC.begin(),index_InnerMDC.end(),hdata->getIndInnerSeg()) != index_InnerMDC.end()) ){
		    index_InnerMDC.push_back(hdata->getIndInnerSeg());
		}
		if(hdata->getIndOuterSeg() >= 0 &&
		   (find(index_OuterMDC.begin(),index_OuterMDC.end(),hdata->getIndOuterSeg()) != index_OuterMDC.end()) ){
		    index_OuterMDC.push_back(hdata->getIndOuterSeg());
		}
		if(hdata->getIndTOF()      >= 0 &&
		   (find(index_TOF.begin(),index_TOF.end()          ,hdata->getIndTOF())      != index_TOF.end()) ){
		    index_TOF.push_back(hdata->getIndTOF());
		}
		if(hdata->getIndShower()   >= 0 &&
		   (find(index_SHOWER.begin(),index_SHOWER.end()    ,hdata->getIndShower())   != index_SHOWER.end()) ){
		    index_SHOWER.push_back(hdata->getIndShower());
		}
		/*
		 // for addaption of HPidTrackCand to RPC
		 if(hdata->getIndRpc()      >= 0 &&
		 (find(index_RPC.begin(),index_RPC.end()          ,hdata->getIndRpc())      != index_RPC.end()) ){
		 index_RPC.push_back(hdata->getIndRpc());
		 }
		 */
	    }
	    //-------------------------------------------------------------------

	    continue; // skip objects which are marked
	}
	if(pCand->isFlagBit(HPidTrackCand::kIsRejected))  continue; // skip objects which are marked

	for(Int_t i = 0; i < 29; i ++) { pCand->unsetFlagBit(i);  }  // clean condition, only reject/used/lepton bit stays!

        //-------------------------------------------------------------------
	// fill the structur objects with needed
        // information

	cand = new candidate;
        cand->ind_PidCand           = pPidTrackCandCat->getIndex(pCand);
	cand->ind_RICH              = hdata->getIndRICH();
	cand->ind_innerMDC          = hdata->getIndInnerSeg();
	cand->ind_outerMDC          = hdata->getIndOuterSeg();
	cand->ind_TOF               = hdata->getIndTOF();
	cand->ind_SHOWER            = hdata->getIndShower();
	cand->ind_RPC               = -1;
	cand->ring_PadNr            = hdata->getRingPadNr();
	cand->innerMDCChi2          = hdata->getInnerMdcChiSquare();

	cand->innerMDCChi2          = -1.;
	cand->outerMDCChi2          = -1.;
	cand->metamatch_Quality     = -1.;
	cand->RK_META_match_Quality = -1.;
	cand->RK_RICH_match_Quality = -1.;
        cand->RK_Chi2               = -1.;
        cand->RICH_RK_Corr          =  hdata->getRingCorrelation(ALG_RUNGEKUTTA);

	if( hdata->getIndInnerSeg() >= 0 ){
	    if(hdata->getInnerMdcChiSquare() > 0) cand->innerMDCChi2 = hdata->getInnerMdcChiSquare();
            else                                  cand->innerMDCChi2 = 1000000;
	}
        if( hdata->getIndOuterSeg() >= 0 ){
	    if(hdata->getOuterMdcChiSquare() > 0) cand->outerMDCChi2 = hdata->getOuterMdcChiSquare();
            else                                  cand->outerMDCChi2 = 1000000;
	}

	if( (hdata->getIndShower() >= 0   ||
	     hdata->getIndTOF()    >= 0 ) &&
	     hdata->getIndOuterSeg() >= 0
	  ){
	    cand->metamatch_Quality     = tdata->getMetaMatchingQuality();
	}
	if( tdata->getRKChiSquare() != 1000000. &&
	    tdata->getRKChiSquare() >  0 )
	{
	    if(hdata->getIndShower() >= 0 || hdata->getIndTOF()>= 0 )
	    {
		cand->RK_META_match_Quality = tdata->getRKMetaMatchingQuality();

		if(!finite(cand->RK_META_match_Quality)) {
		    cand->RK_META_match_Quality = -1;
		}
	    }
	    if(hdata->getIndRICH() >= 0 )
	    {
		if(tdata->getRKRichMatchingQuality() == -1.){
		    // recalculate RK-RICH matching (compatibility to old versions)
		    Float_t dthA                = hdata -> getDeltaThetaRKRICH(tdata->getRKTheta());                      // fRichTheta -fRKTheta;
		    Float_t dphA                = hdata -> getDeltaPhiRKRICH  (tdata->getRKPhi(), tdata->getRKTheta());   // ( fRichPhi- fRKPhi )*TMath::Sin(TMath::DegToRad()*fRKTheta)
		    tdata->setRKRichMatchingQuality( sqrt( dthA*dthA + dphA*dphA));
		}
		cand->RK_RICH_match_Quality = tdata->getRKRichMatchingQuality();
	    }
	}
	if(tdata->getRKChiSquare() != 1000000.   &&
	   tdata->getRKChiSquare() >  0.         &&
	   hdata->getOuterMdcChiSquare() >  0 ){
	    cand->RK_Chi2   = tdata->getRKChiSquare();
	}
	else if (tdata->getRKChiSquare() != 1000000.  &&
		 tdata->getRKChiSquare() >  0.        &&
		 hdata->getOuterMdcChiSquare() < 0) {
	    cand->RK_Chi2   = tdata->getRKChiSquare() + 100;
	}

	cand->SPLINE_Chi2      = tdata->getSplineChiSquare();
	if(tdata->getPull() != -1){
	    cand->KICK_Pull    = fabs(tdata->getPull());
	}
	else { cand->KICK_Pull = -1; }
	//-------------------------------------------------------------------


	all_candidates.push_back(cand);
    }
    return 0;
}

void HPidTrackSorter::printCand(candidate* cand, Int_t i,TString spacer)
{
    // print one candidate object and the flags which have been
    // already set to HPidTrackCand
    cout<<spacer.Data()<<i<<" --------------------------------------------"<<endl;
    cout<<spacer.Data()<<"cand " <<cand->ind_PidCand
	<<", R: "  <<cand->ind_RICH
	<<", iM: " <<cand->ind_innerMDC
	<<", oM: " <<cand->ind_outerMDC
	<<", T: "  <<cand->ind_TOF
	<<", S: "  <<cand->ind_SHOWER
	<<", RPC: "<<cand->ind_RPC
	<<endl;
    cout<<spacer.Data()
	<<"Ring "     <<cand->ring_PadNr
	<<", ichi2: " <<cand->innerMDCChi2
	<<", ochi2: " <<cand->outerMDCChi2
        <<", MQA: "   <<cand->metamatch_Quality<<endl;
    cout<<spacer.Data()
	<<"RKM: "     <<cand->RK_META_match_Quality
	<<", RKR: "   <<cand->RK_RICH_match_Quality
        <<", RK: "    <<cand->RK_Chi2
	<<", SPL: "   <<cand->SPLINE_Chi2
	<<", KICK: "  <<cand->KICK_Pull <<endl;
    cout<<spacer.Data()
	<<"RKRKMETA: "  <<(cand->RK_Chi2 != -1 && cand->RK_META_match_Quality != -1 ? cand->RK_Chi2*cand->RK_META_match_Quality : -1)
	<<", RKRKRICH: "<<(cand->RK_Chi2 != -1 && cand->RK_RICH_match_Quality != -1 ? cand->RK_Chi2*cand->RK_RICH_match_Quality : -1)
	<<", RKMETAQA: "<<(cand->RK_Chi2 != -1 && cand->metamatch_Quality     != -1 ? cand->RK_Chi2*cand->metamatch_Quality : -1)
	<<", isRingRK: "<<(Int_t) cand->RICH_RK_Corr
	<<endl;
    HPidTrackCand* pcand = (HPidTrackCand*) pPidTrackCandCat->getObject(cand->ind_PidCand);
    if(cand)
    {
	cout<<spacer.Data()<<flush;
	pcand->printFlags();
    } else {
	::Error("HPidTrackSorter::printCand()","NULL pointer retrieved for HPidTrackCand Index %i",cand->ind_PidCand);

    }

}
void HPidTrackSorter::printEvent(TString comment)
{
    Int_t ctcand = 0;

    cout<<"----------------------------------------"<<endl;
    cout<<"Event kSwitchParticle: "<<kSwitchParticle<<" "
 	<<setw(6)                  <<gHades->getEventCounter()
	<<"s: "                    <<selectBest_Iteration
	<<", f: "                  <<fill_Iteration
	<<" "                      <<comment.Data()
	<<endl;
    HPidTrackCand* pcand;
    iterPidTrackCandCat->Reset();
    while ((pcand = (HPidTrackCand *)iterPidTrackCandCat->Next()) != 0 )
    {
	cout <<setw(6)<<ctcand<<" "<<flush;
	pcand->printInfo();
	++ ctcand;
    }
}

void HPidTrackSorter::setOutputFile(TString filename )
{
    // create output root file filename if filename not empty,
    // otherwise pidtrackcleaner.root in local directory
    // exit the program if file cannot be created.

    if(filename.CompareTo("") == 0){

	fout = new TFile("pidtrackcleaner.root","RECREATE");

    } else {

	fout = new TFile(filename.Data(),"RECREATE");
    }
    if(fout){

	nt = new TNtuple("nt","nt",
			 "p:th:ph:"                    //3
			 "RKchiq:Spchiq:KPpull:"       //6
			 "system:sum0:isRICHRKCorr:"   //9
			 "common:geantID:"             //11
			 "best:double:"                //13
			 "IsDoubleHitRICH:IsDoubleHitInnerMDC:IsDoubleHitOuterMDC:kIsDoubleHitMETA:"  //17
			 "IsBestHitRICH:IsBestHitInnerMDC:IsBestHitOuterMDC:IsBestHitMETA:"           //21
			 "IsBestRKMETA:IsBestRKRICH:IsBestRK:IsBestSPLINE:IsBestKICK:"                //26
			 "IsBestRKRKMETA:IsBestRKRKRICH:IsBestRKMETAQA:"                              //29
			 "IsRejected:IsUsed:IsLepton:"                                                //32
			 "IsAcceptedHitRICH:IsAcceptedHitInnerMDC:IsAcceptedHitOuterMDC:IsAcceptedHitMETA:" //36
			 "IsAcceptedRKMETA:IsAcceptedRKRICH:IsAcceptedRK:IsAcceptedSPLINE:IsAcceptedKICK:"  //41
			 "IsAcceptedRKRKMETA:IsAcceptedRKRKRICH:IsAcceptedRKMETAQA:"                        //44
                         "evtNr"                                                                            //45
			);


    }  else {

	Error("HPidTrackSorter::setOutputFile()","Could not create outputfile !");
        exit(1);
    }
}
void HPidTrackSorter::fillNtuple()
{
     // fill debugging ntuple
    if(!iterPidTrackCandCat) return;
    if(!nt)                  return;
    HPidGeantTrackSet* PidGeant;
    HPidTrackCandSim* pTrCandSim;
    HPidTrackData* pTrack;
    HPidHitData*   pHit ;


    iterPidTrackCandCat->Reset();
    while ((pTrCandSim = (HPidTrackCandSim *) iterPidTrackCandCat->Next()) != 0) // begin of PidTrackCand iterator
    {
	Float_t data[45]={0};

	pTrack       = pTrCandSim  -> getTrackData();
	pHit         = pTrCandSim  -> getHitData();

	data[0]      = pTrack      -> getMomenta(4);
	data[1]      = pTrack      -> getRKTheta();
	data[2]      = pTrack      -> getRKPhi();
	data[3]      = pTrack      -> fRKChiSquare;
	data[4]      = pTrack      -> getSplineChiSquare();
	data[5]      = pTrack      -> getPull();
	data[6]      = pHit        -> getSystem();
	data[7]      = pHit        -> getShowerSum(0);
	data[8]      = pHit        -> getRingCorrelation(ALG_RUNGEKUTTA);
	if(isSimulation){
	    PidGeant = pTrCandSim->getGeantTrackSet();
	    data[9]      = PidGeant    -> getMostCommonCorrelation();
	    data[10]     = PidGeant    -> getGeantPID();
	}
	data[11]     = pTrCandSim  ->isFlagAllBestHit();   // best RICH,InnerMDC,OuterMDC,META
	data[12]     = pTrCandSim  ->isFlagOR (4,
					       HPidTrackCand::kIsDoubleHitRICH,
					       HPidTrackCand::kIsDoubleHitInnerMDC,
					       HPidTrackCand::kIsDoubleHitOuterMDC,
					       HPidTrackCand::kIsDoubleHitMETA);

	data[13]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsDoubleHitRICH);
	data[14]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsDoubleHitInnerMDC);
	data[15]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsDoubleHitOuterMDC);
	data[16]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsDoubleHitMETA);

	data[17]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestHitRICH);
	data[18]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestHitInnerMDC);
	data[19]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestHitOuterMDC);
	data[20]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestHitMETA);
	data[21]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestRKMETA);
	data[22]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestRKRICH);
	data[23]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestRK);
	data[24]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestSPLINE);
	data[25]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestKICK);

	data[26]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestRKRKMETA);
	data[27]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestRKRKRICH);
        data[28]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsBestRKMETAQA);

	data[29]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsRejected);
        data[30]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsUsed);
        data[31]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsLepton);

	data[32]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedHitRICH);
	data[33]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedHitInnerMDC);
	data[34]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedHitOuterMDC);
	data[35]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedHitMETA);
	data[36]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedRKMETA);
	data[37]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedRKRICH);
	data[38]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedRK);
	data[39]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedSPLINE);
	data[40]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedKICK);

	data[41]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedRKRKMETA);
	data[42]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedRKRKRICH);
        data[43]      =  pTrCandSim ->isFlagBit(HPidTrackCand::kIsAcceptedRKMETAQA);
        data[44]      =  gHades->getEventCounter();

	nt ->Fill(data);
    }
}
void HPidTrackSorter::resetFlags(Bool_t flag, Bool_t reject, Bool_t used, Bool_t lepton)
{
    // reset all flags of HPidTrackCand by looping over the
    // HPidTrackCand category. The flags kIsRejected, kIsUsed
    // and kIsLepton are special for selection and handles independend
    // from the other flags. The lower flags of objects marked
    // with kIsUsed cand be only reste if used == kRTUE.

    if(!iterPidTrackCandCat) return;
    HPidTrackCand* pCand;
    iterPidTrackCandCat->Reset();
    while ((pCand = (HPidTrackCand *) iterPidTrackCandCat->Next()) != 0) // begin of PidTrackCand iterator
    {
	if(flag){
	    if((pCand->isFlagBit(HPidTrackCand::kIsUsed) && used)  ||
	       !pCand->isFlagBit(HPidTrackCand::kIsUsed) ){
		// we want to keep the flags from previous selection
		// clean condition, only reject,used and lepton bit stays!
		for(Int_t i=0;i<HPidTrackCand::kIsLepton;i++) { pCand->unsetFlagBit(i);  }  
	    }
	}
	if(reject) pCand->unsetFlagBit(HPidTrackCand::kIsRejected);
	if(used)   pCand->unsetFlagBit(HPidTrackCand::kIsUsed);
	if(lepton) pCand->unsetFlagBit(HPidTrackCand::kIsLepton);
    }
}
void HPidTrackSorter::selection(Bool_t (*function)(HPidTrackCand* ))
{
    // loops over the HPidTrackCand category and marks all objects
    // which does not fullfill the selection criteria with the reject flag.
    // Objects marked as kIsUsed are skipped.
    if(!iterPidTrackCandCat) return;
    HPidTrackCand* pTrCand;
    iterPidTrackCandCat->Reset();
    while ((pTrCand = (HPidTrackCand *) iterPidTrackCandCat->Next()) != 0) // begin of PidTrackCand iterator
    {
	if( pTrCand->isFlagBit(HPidTrackCand::kIsUsed)) continue; // do not touch already used objects!
	if(!pTrCand->select(function)) pTrCand->setFlagBit(HPidTrackCand::kIsRejected);
    }
}
void HPidTrackSorter::cleanUp()
{
    // delete temporary candidate objects from
    // heap and clear the candidate vector. clear
    // all index vectors.

    clearVector(all_candidates);

    index_RICH.clear();
    index_InnerMDC.clear();
    index_OuterMDC.clear();
    index_SHOWER.clear();
    index_TOF.clear();
    index_RPC.clear();
}

Int_t  HPidTrackSorter::fillAndSetFlags()
{
    // clean up the memory and index vectors and fill the
    // candidate vector new. Flag the kIsAcceptedXXX criteria
    // and kIsDoubleHitXXX criteria of the HPidTrackCand objects of
    // the selection (skipp objects which are marked kIsUsed and
    // kIsRejected) returns the number of candidates.


    if(!pPidTrackCandCat) return 0; // nothing to do
    fill_Iteration++;

    cleanUp();
    //------------------------------------------------------------------
    // fill HPidTrackCand Info to temorary objects
    // which will be jused for sorting
    // all flags in HPidTrackCand will be reset (besides kIsRejected)
    fillInput(all_candidates);


    // list of thing to to
    // for each double hit
    HPidTrackSorter::ESwitch myIndex  [] = {
	kIsIndexRICH,
	kIsIndexInnerMDC,
	kIsIndexOuterMDC,
	kIsIndexTOF,
	kIsIndexSHOWER,
        kIsIndexRPC
    };

    // and for different quality criteria
    HPidTrackSorter::ESwitch myQuality[] = {
	kIsBestHitRICH,
        kIsBestHitInnerMDC,
        kIsBestHitOuterMDC,
	kIsBestHitMETA,
        kIsBestRKMETA,
        kIsBestRKRICH,
        kIsBestRK,
	kIsBestSPLINE,
	kIsBestKICK,
	kIsBestRKRKMETA,
	kIsBestRKRKRICH,
	kIsBestRKMETAQA
   };

    kSwitchIndex   = kIsIndexRICH;
    kSwitchQuality = kIsBestHitRICH;

    // check all objects for validity with respect to
    // quality criteria and flag them in HPidTrackCand
    for(UInt_t i = 0; i < (sizeof(myQuality) / sizeof(Int_t)); i ++){
	flagAccepted(all_candidates,myQuality[i]);
    }

    // vector is resorted by index! flag all double used
    // detector hits in HPidTrackCand
    for(UInt_t i = 0; i < (sizeof(myIndex) / sizeof(Int_t)); i ++){
	flagDouble(all_candidates,myIndex[i]);
    }

    return all_candidates.empty() ? 0 : all_candidates.size();
}

void  HPidTrackSorter::backupFlags()
{
    // backup the flags of HPidTrackCand objects. The flags cand be
    // restored to the objects by calling restoreFlags. The flags
    // which have been stored previously will be cleared.

    old_flags.clear();

    HPidTrackCand* pCand ;
    iterPidTrackCandCat->Reset();

    while ((pCand = (HPidTrackCand *)iterPidTrackCandCat->Next()) != 0 )
    {
         old_flags.push_back(pCand->getFlagField());
    }
}

Bool_t  HPidTrackSorter::restoreFlags()
{
    // restore the flags of HPidTrackCand objects since the last call of
    // backupFlags(). Returns kFALSE if size of vector old_flags does
    // not match the number of objects in HPidTrackCand category

    HPidTrackCand* pCand ;
    iterPidTrackCandCat->Reset();

    vector<Int_t>::iterator iter;
    iter = old_flags.begin();
    while ((pCand = (HPidTrackCand *)iterPidTrackCandCat->Next()) != 0 )
    {
	if(iter != old_flags.end()){
	    pCand->setFlagField(*iter);
	    iter ++;
	} else {
            Error("restoreFlags()","Size of backuped flags does not match number of HPidTrackCand objects!");
	    return kFALSE;
	}
    }
    return kTRUE;
}

Int_t HPidTrackSorter::fill(Bool_t (*function)(HPidTrackCand* ))
{
    // fill the input candidate vector. The kIsRejected flag and
    // lower flags (below kIsLepton) is reseted for all objects
    // not marked as kIsUsed. The filling is done twice once for
    // all objects and second after the application of the user
    // provide test function. Returns the number of candidates after
    // the selection. If kDebug is set and printLevel >= 1 the number
    // of candidates before and after selection will be printed.

    resetFlags(kTRUE,kTRUE,kFALSE,kFALSE);  // need to reset reject/double hit bits (already used objects will stay)!
    Int_t first  = fillAndSetFlags();       // fill all candidates
    selection(function);                    // flag candidates rejected if the they fail the test
    Int_t second = fillAndSetFlags();       // fill only good candidates
    if(kDebug && printLevel >= 1) {
	cout<<"fill(): "
	    <<"s: "                <<selectBest_Iteration
	    <<", f: "              <<fill_Iteration
	    <<", kSwitchParticle: "<<kSwitchParticle
	    <<", nCands first: "   <<first
	    <<", nCands second :"  <<second
	    <<endl;
    }
    return all_candidates.empty() ?  0 : all_candidates.size();
}

Int_t HPidTrackSorter::selectBest(HPidTrackSorter::ESwitch byQuality, Int_t byParticle)
{
    // perfomes the selection of the best tracks (with respect to the chosen quality
    // criteria), which makes shure, that each detector hit is used only once. If
    // In case Int_t byParticle == kIsLepton the RICH index
    // is taken into account, otherwise not. For leptons the kIsLepton bit is flagged
    // in addition. Individual detector hits can be ignored in the selection procedure with
    // the setIgnoreXXX(kTRUE) functions (by default all hits are taken into account).
    // Returns the number of selected best HPidTrackCand objects.
    // if kDebug is set and  printLevel >= 1 the number of sected objects and
    // the statistics of the rejection on double hits i the snhgle detectors is printed.
    // if printLevel >=3 the properties of the candidates (accepted and rejected) are
    // printed.

    if(gHades->getEventCounter() != currentEvent){
	currentEvent         = gHades->getEventCounter();
        selectBest_Iteration = 0;
    }
    selectBest_Iteration++;

    kSwitchQuality  = byQuality;
    kSwitchParticle = byParticle;
    Int_t ct        = 0;
    Int_t ctdouble[4] = {0};

    sort(all_candidates.begin(),all_candidates.end(),this->cmpQuality);

    for(UInt_t i = 0; i < all_candidates.size(); ++ i){
        candidate* cand = all_candidates[i];

        //-------------------------------------------------------
	// skip the candidates which contain a single detector hit
        // which has been used before
        Int_t isDoubleHit = 0;
	if( (kSwitchParticle == kIsLepton && !kIgnoreRICH ) ) {
            // look to RICH hits only in case of leptons
	    if(cand->ind_RICH     >= 0 &&
	       (find(index_RICH.begin(),index_RICH.end()        ,cand->ind_RICH)     != index_RICH.end()) ){
		isDoubleHit |= 0x1 << 0;
		ctdouble[0] ++;
	    }
	}
	if( !kIgnoreInnerMDC ) {
	    if(cand->ind_innerMDC >= 0 &&
	       (find(index_InnerMDC.begin(),index_InnerMDC.end(),cand->ind_innerMDC) != index_InnerMDC.end()) ){
		isDoubleHit |= 0x1 << 1;
		ctdouble[1] ++;
	    }
	}
	if( !kIgnoreOuterMDC ) {
	    if(cand->ind_outerMDC >= 0 &&
	       (find(index_OuterMDC.begin(),index_OuterMDC.end(),cand->ind_outerMDC) != index_OuterMDC.end()) ){
		isDoubleHit |= 0x1 << 2;
		ctdouble[2] ++;
	    }
	}
	if( !kIgnoreMETA ) {
	    if(cand->ind_TOF      >= 0 &&
	       (find(index_TOF.begin(),index_TOF.end()          ,cand->ind_TOF)      != index_TOF.end()) ){
		isDoubleHit |= 0x1 << 3;
		ctdouble[3] ++;

	    }

	    if(cand->ind_SHOWER   >= 0 &&
	       (find(index_SHOWER.begin(),index_SHOWER.end()    ,cand->ind_SHOWER)   != index_SHOWER.end()) ){
		isDoubleHit |= 0x1 << 3;
		ctdouble[3] ++;
	    }
	    if(cand->ind_RPC      >= 0 &&
	       (find(index_RPC.begin(),index_RPC.end()          ,cand->ind_RPC)      != index_RPC.end()) ){
		isDoubleHit |= 0x1 << 3;
		ctdouble[3] ++;
	    }
	}
	if(isDoubleHit){
            if(kDebug && printLevel >= 3){
		TString out="";
		for(Int_t j=3;j>=0;j--){
		    out+= ( (isDoubleHit>>j) & 0x1 );
		}
		cout<<"--selectBest(): skip double hit with pattern : "<<out.Data()<<endl;
		printCand(cand,i);
	    }
	    continue;
	}
        //-------------------------------------------------------

        //-------------------------------------------------------
	// mark the candidate as best and add the single detector hit indices
	// to the lookup
	if(cand->ind_RICH     >= 0 ){
	    index_RICH.push_back(cand->ind_RICH);
	}
	if(cand->ind_innerMDC >= 0 ){
	    index_InnerMDC.push_back(cand->ind_innerMDC);
	}
	if(cand->ind_outerMDC >= 0 ){
	    index_OuterMDC.push_back(cand->ind_outerMDC);
	}
	if(cand->ind_TOF      >= 0 ){
	    index_TOF.push_back(cand->ind_TOF);
	}
	if(cand->ind_SHOWER   >= 0 ){
	    index_SHOWER.push_back(cand->ind_SHOWER);
	}
	if(cand->ind_RPC      >= 0 ){
	    index_RPC.push_back(cand->ind_RPC);
	}

	HPidTrackCand* pcand = (HPidTrackCand*) pPidTrackCandCat->getObject(cand->ind_PidCand);
	if(!cand) {
	    Error("HPidTrackSorter::selectBest()","NULL pointer retrieved for HPidTrackCand at index %i",cand->ind_PidCand);
	    continue;
	}
	switch (byQuality)
	{
	case kIsBestHitRICH:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestHitRICH);
	    break;
	case kIsBestHitInnerMDC:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestHitInnerMDC);
	    break;
	case kIsBestHitOuterMDC:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestHitOuterMDC);
	    break;
	case kIsBestHitMETA:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestHitMETA);
	    break;
	case kIsBestRKMETA:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestRKMETA);
	    break;
	case kIsBestRKRICH:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestRKRICH);
	    break;
	case kIsBestRK:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestRK);
	    break;
	case kIsBestSPLINE:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestSPLINE);
	    break;
	case kIsBestKICK:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestKICK);
	    break;
	case kIsBestRKRKMETA:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestRKRKMETA);
	    break;
	case kIsBestRKRKRICH:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestRKRKRICH);
	    break;
	case kIsBestRKMETAQA:
	    pcand -> setFlagBit(HPidTrackCand::kIsBestRKMETAQA);
	    break;
	default :

	    Error("HPidTrackSorter::selectBest()","Unknown switch statement : %i!",byQuality);
	    exit(1);
	    break;

	}
        ++ ct;
	pcand ->setFlagBit(HPidTrackCand::kIsUsed);
	if(byParticle == kIsLepton){
	    pcand ->setFlagBit(HPidTrackCand::kIsLepton);
	}
	if(kDebug && printLevel >= 3){
	    cout<<"++selectBest(): select hit : "<<endl;
	    printCand(cand,i);
	}
	//-------------------------------------------------------

    }
    if(kDebug && printLevel >= 1)
    {
	cout<<"selectBest(): "
	    <<"s: "                <<selectBest_Iteration
	    <<", f: "              <<fill_Iteration
	    <<", kSwitchParticle: "<<kSwitchParticle
	    <<", nBest cand: "     <<ct
	    <<", double RICH: "    <<ctdouble[0]
            <<", InnerMDC: "       <<ctdouble[1]
            <<", double OuterMDC: "<<ctdouble[2]
            <<", double META: "    <<ctdouble[3]
	    <<endl;
    }
    fill_Iteration = 0;

  return ct;
}

void HPidTrackSorter::setRICHMatching(HPidTrackSorter::ERichMatch match, Float_t window)
{
    // switch between the matching cut RICH-MDC inside the selectLeptons() function. The method
    // is selected by the options defined in HPidTrackSorter::ERichMatch.
	kSwitchRICHMatching = match;
	fRICHMDCWindow      = window;
}


Bool_t HPidTrackSorter::selectLeptons(HPidTrackCand* pcand)
{
    // build in selection function for lepton candidates.
    // Requires besides an RICH hit, RK + META and fitted innerMDC
    // segment. The RICH-MDC matching cut depends on the
    // settings of the matching method and window (set by
    // HPidTrackSorter::setRICHMatching . default is
    // kUseRKRICHWindow, window = 4. degree)

    HPidTrackData* pTrack  = pcand  -> getTrackData();
    HPidHitData*    pHit   = pcand  -> getHitData();
    Bool_t test = kFALSE;
    if( pcand->isFlagAND(5,
			 HPidTrackCand::kIsAcceptedHitRICH,
			 HPidTrackCand::kIsAcceptedHitInnerMDC,
			 HPidTrackCand::kIsAcceptedHitOuterMDC,
			 HPidTrackCand::kIsAcceptedHitMETA,
			 HPidTrackCand::kIsAcceptedRK
			)
       &&
       pHit->getInnerMdcChiSquare() > 0
       &&
       pTrack->getRKChiSquare()     < 10000
      ) test = kTRUE;

    if(test)
    {
	if( kSwitchRICHMatching == kUseRICHIndex)
	{
	    return kTRUE;
	}
	else if ( kSwitchRICHMatching == kUseRKRICHCorrelation	&&  pHit -> getRingCorrelation(ALG_RUNGEKUTTA) )
	{
	    return kTRUE;
	}
	else if ( kSwitchRICHMatching == kUseRKRICHWindow
		 &&
		 fabs(pHit->getDeltaThetaRKRICH(pTrack->getRKTheta())) < fRICHMDCWindow
		 &&
		 fabs(pHit->getDeltaPhiRKRICH(pTrack->getRKPhi(),pTrack->getRKTheta())) < fRICHMDCWindow
		)
	{
	    return kTRUE;
	}
	else return kFALSE;
    }
    else return kFALSE;
}
Bool_t HPidTrackSorter::selectHadrons(HPidTrackCand* pcand )
{
    // build in selection function for hadron candidates.
    // Requires besides RK + META and fitted innerMDC.

    HPidTrackData* pTrack  = pcand  -> getTrackData();
    HPidHitData*    pHit   = pcand  -> getHitData();

    if( pcand->isFlagAND(4,
			 HPidTrackCand::kIsAcceptedHitInnerMDC,
			 HPidTrackCand::kIsAcceptedHitOuterMDC,
			 HPidTrackCand::kIsAcceptedHitMETA,
			 HPidTrackCand::kIsAcceptedRK
			)
       &&
       pHit->getInnerMdcChiSquare() > 0
       &&
       pTrack->getRKChiSquare()     < 10000
      ) return kTRUE;


    return kFALSE;
}

