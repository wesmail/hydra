// $Id: hrichevtmixer.h,v 1.2 2003-07-03 12:21:52 eberl Exp $
// Last update by Thomas Eberl: 03/06/21 17:48:41
//
#ifndef HRICHEVTMIXER_H
#define HRICHEVTMIXER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HDiHitMatch;
class HHitMatch;
class HHitMatchSim;
class TObjArray;
class TRandom;
class HRichEvtMixer : public HReconstructor {
public:

	HRichEvtMixer();
        HRichEvtMixer(Text_t *name,Text_t *title,Char_t* world,Int_t evts=10,Int_t rndnr=2,Char_t *cuts="nocuttracklet");
	~HRichEvtMixer();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        ClassDef(HRichEvtMixer,1) //ROOT extension

protected:
        Bool_t createHitDiMatch(HHitMatchSim*,HHitMatchSim*,TObjArray &,TObjArray &);
        HHitMatchSim* copyHitMatch(HHitMatchSim*);
        Bool_t createHitDiMatch(HHitMatch*,HHitMatch*,TObjArray &,TObjArray &);
        HHitMatch* copyHitMatch(HHitMatch*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitDiMatchCat(){return pHitDiMatchCat;};
        void doEventMixingTrackSubset();
        void doEventMixingAllTracks();
        void storeEvent(Int_t);
        void deleteStoredEvents();
	Int_t nCounterProcessedNbEvents; 
	HCategory* pHitMatchCat; //!
        HIterator* pIterMatchHit; //!
	HCategory* pHitDiMatchCat; //!
        HIterator* pIterDiMatchHit; //!
    TRandom *ran; //!
    TObjArray* cuts;//! array with cut objects
    Bool_t isSim;//! simulation or real data world
    Int_t evtrange; //! number of evts to store for mixing
    Int_t evtsStoredInRange; //! counter for nb of evts within evtrange
    Int_t nrMixObjs; //! number of rand. chosen objs per evt for mixing
    Int_t nCntMixedEvts; //!
    TObjArray* evtset; //!
    TString *cutStateId;//!
};

#endif // HRICHEVTMIXER_H

