// $Id: hpairevtmixer.h,v 1.7 2009-07-15 11:41:59 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-13 17:14:45
//
// ---------------------------------------------------------------------------
#ifndef HPAIREVTMIXER_H
#define HPAIREVTMIXER_H
// ---------------------------------------------------------------------------
#include <hreconstructor.h>
// ---------------------------------------------------------------------------
class TObjArray;
// ---------------------------------------------------------------------------
class HCategory;
class HIterator;
// ---------------------------------------------------------------------------
class HPairCutPar;
class HPairExt;
class HPairSimExt;
class HPairQA;
class HPidParticle;
// ---------------------------------------------------------------------------

class HPairEvtMixer : public HReconstructor {
public:

        HPairEvtMixer();
        HPairEvtMixer(const Text_t *name,
		      const Text_t *title,
		      Int_t nBufferSize=100,
		      Int_t nBufferMethod=1,
		      Int_t nPairAna=1,
		      Int_t nShrinkPoolByThisNumberOfEvents=1,
		      Int_t rndnr=0,
		      Int_t nMinNbLepsPerEvt=0,
		      Int_t nUnlikeSignOnly=0);
	~HPairEvtMixer();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();

        void setQAFileName(const Char_t* c){cQAFileName=c;};
        TString& getQAFileName(void){return cQAFileName;};
        void useUnlikesignOnly(Bool_t k){kOnlyUnlikeSign=k;};

protected:
        HCategory* getPidParticleCat(){return pCatPidParticle;};
        HCategory* getPairCat(){return pCatPair;};

    // mixed pair analysis
        Bool_t checkMixedPair(HPairExt*);
        Bool_t checkMixedPair(HPairSimExt*);

    // particle analysis
        Bool_t checkParticle(HPidParticle*);

    // event pool filling
        Int_t storeEvent();
        Int_t storeEventWithoutPairAnalysis();
        Int_t storeEventWithPairAnalysis();
        void  deleteStoredEvents();
        void  shrinkEventBuffer();

    // event mixing
        Int_t makeMixedPairs();
#if 0
        TObjArray*   doEventMixingTrackSubset();
#endif // 0
        TObjArray*   doEventMixingAllTracks();
    

    // data members
	Int_t nCounterProcessedNbEvents; 

    // input
	HCategory* pCatPidParticle; //!
        HIterator* pIterPidParticle; //!
    // input 2
	HCategory* pCatPair; //!
        HIterator* pIterPair; //!

    // pair cut parameter container
        HPairCutPar* pCuts;         //!

    
    Bool_t kIsReal;//!
    Bool_t kFirstMixing;//! 
    Bool_t kOnlyUnlikeSign;//! 
    Int_t evtrange; //! number of evts to store for mixing
    Int_t evtsStoredInRange; //! counter for nb of evts within evtrange
    Int_t nrMixObjs; //! number of rand. chosen objs per evt for mixing
    Int_t nCntMixedEvts; //! number of event pairs
    Int_t nBufferFillingMethod; //! 0: discrete (default); 1: ring 
    Int_t nShrinkPool; //! number of events to shift ring buffer
    Int_t nPairAnalysis; //! switch : 0 (default) no pair analysis 
                         //           1: do pair ana
    Int_t nMinParticlesPerEvent; //! min number of particles needed per event
                                 // before event is stored
    Int_t nStoredEvts;//! counter of stored events
    TObjArray* evtset; //! event pool holding 1 array with particles per event 

    // QA object
    TString cQAFileName; //!
    HPairQA* pQA;//!


    ClassDef(HPairEvtMixer,1) //Event mixing task of the pair module
};

#endif // HPAIREVTMIXER_H

