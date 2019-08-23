//*-- Author : Jochen Markert 18.07.2007

#include "hpidtrackcleaner.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "piddef.h"

#include <iostream>

using namespace std;



//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
//
// HPidTrackCleaner
//
// This HReconstructor loops over the HPidTRackCand category and
// analysis the objects. Purpose of the procedure is to categorize
// the objects in the case of multiple usage of the single detector
// hits (RICH, InnerMDC, OuterMDC, META) as double hit and to provide
// a hint which of the candidates is the best with respect to certain
// criteria (described by the bit flags below). This class is a simple
// wrapper for HPidTrackSorter (see the corresponding documentation)
//
// The selection of good tracks first for leptons and
// second for hadrons will be performed. All flags on HPidTrackCand
// objects will be reseted at the beginning of the event. The best tracks will
// be selected according to HPidTrackSorter::selectXXX() functions
// and HPidTrackSorter::kIsBestRKRKMETA criteria (RKChi2 * RK_META_MATCH_QUALITY).
//
// For debugging purpose one cand set screen print outs to check the sorting
// (setDebug() + setPrintLevel(Int_t level)(level = 1-3).
//
////////////////////////////////////////////////////////////////////////////////

ClassImp(HPidTrackCleaner)


HPidTrackCleaner::HPidTrackCleaner(void)
    : HReconstructor("PidTrackCleaner", "PID track candidate cleaner")
{
    clear();
}

HPidTrackCleaner::HPidTrackCleaner(Text_t* name, Text_t* title)
: HReconstructor(name, title)
{
    clear();
}

HPidTrackCleaner::~HPidTrackCleaner(void)
{

}

void HPidTrackCleaner::clear()
{
    pPidTrackCandCat    = NULL;
}

Bool_t HPidTrackCleaner::init(void)
{
    // get the category pointer to the HPidTrackCand category and
    // init the HPidTrackSorter object.
    pPidTrackCandCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);
    if (!pPidTrackCandCat) {
	Error("HPidTrackCleaner::init()"," No HPidTrackCand Input -> Switching HPidTrackCleaner OFF");
    }

    return sorter.init();
}

Int_t HPidTrackCleaner::execute(void)
{
    // perform the selction of good tracks first for leptons and
    // second for hadrons. All flags on HPidTrackCand objects will
    // be reseted at the beginning of the event. The best tracks will
    // be selected according to HPidTrackSorter::selectXXX() functions
    // and HPidTrackSorter::kIsBestRKRKMETA criteria (RKChi2 * RK_META_MATCH_QUALITY).

    if(!pPidTrackCandCat) return 0; // nothing to do

    // clean all flags !
    sorter.resetFlags(kTRUE,kTRUE,kTRUE,kTRUE);     // flags,reject,used,lepton

    //-------LEPTONS---------------------------
    sorter.fill(HPidTrackSorter::selectLeptons);    // fill only good leptons
    sorter.selectBest(HPidTrackSorter::kIsBestRKRKMETA,HPidTrackSorter::kIsLepton);

    //-------HADRONS---------------------------
    sorter.fill(HPidTrackSorter::selectHadrons);    // fill only good hadrons (already marked good leptons will be skipped)
    sorter.selectBest(HPidTrackSorter::kIsBestRKRKMETA,HPidTrackSorter::kIsHadron);

    sorter.fillNtuple();
    return 0;

}

Bool_t HPidTrackCleaner::finalize(void)
{
    // write ntuple to file (if it exist)
    return sorter.finalize();
}

void HPidTrackCleaner::setOutputFile(TString filename )
{
    // create output root file filename if filename not empty,
    // otherwise pidtrackcleaner.root in local directory
    // exit the program if file cannot be created.

    sorter.setOutputFile(filename);
}
