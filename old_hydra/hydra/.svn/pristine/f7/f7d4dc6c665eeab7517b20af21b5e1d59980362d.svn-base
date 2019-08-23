// $Id: hpairevtmixer.h,v 1.4 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-04-20 13:14:49
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
class HPidParticleExt;
// ---------------------------------------------------------------------------

class HPairEvtMixer : public HReconstructor {
public:

        HPairEvtMixer();
        HPairEvtMixer(Text_t *name,
		      Text_t *title,
		      Int_t nBufferSize=100,
		      Int_t nBufferMethod=1,
		      Int_t nPairAna=1,
		      Int_t nShrinkPoolByThisNumberOfEvents=1,
		      Int_t rndnr=0,
		      Int_t nMin=0);
	~HPairEvtMixer();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();

        void setQAFileName(Char_t* c){cQAFileName=c;};
        TString& getQAFileName(void){return cQAFileName;};


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
        TObjArray*   doEventMixingTrackSubset();
        TObjArray*   doEventMixingAllTracks();
    

    // data members
	Int_t nCounterProcessedNbEvents; 

    // input
	HCategory* pCatPidParticle; //!
        HIterator* pIterPidParticle; //!
    // output
	HCategory* pCatPair; //!
        HIterator* pIterPair; //!

    // pair cut parameter container
        HPairCutPar* pCuts;         //!

    
    Bool_t kIsReal;//!
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

    TObjArray* evtset; //! event pool holding 1 array with particles per event 

    // QA object
    TString cQAFileName; //!
    HPairQA* pQA;//!


    ClassDef(HPairEvtMixer,0) //Event mixing task of the pair module
};

#endif // HPAIREVTMIXER_H

