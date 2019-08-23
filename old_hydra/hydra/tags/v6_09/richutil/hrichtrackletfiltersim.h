// File: hrichtrackletfiltersim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/21 14:17:12
//

#ifndef HRICHTRACKLETFILTERSIM_H
#define HRICHTRACKLETFILTERSIM_H

//#include "hrichtrackletfilter.h"
#include "hreconstructor.h"
class HCategory;
class HIterator;
class HHitMatchSim;
class HRichTrackletFilterSim : public HReconstructor {
public:

	HRichTrackletFilterSim();
        HRichTrackletFilterSim(Text_t *name,Text_t *title);
	~HRichTrackletFilterSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
        HCategory* getGeantKineCat(){return fGeantKineCat;};
        void resetCounters();
        Bool_t delHitMatch(TObjArray *);
        void filterTrackletIndividual(Int_t);
        void filterTrackletInGroup(Int_t);
        Bool_t isGood(HHitMatchSim*,Int_t);
        Int_t countCorrelationObjects();
    void dumpCorrelationObjectStat();
       ClassDef(HRichTrackletFilterSim,1) //ROOT extension

private:
	HCategory* pHitMatchCat; //!
        HCategory* fRichPID; //!
        HCategory* pHitMatchHeaderCat; //!
        HIterator* pIterMatchHit; //!
        HIterator* fRichIter;    //!
        HIterator* pIterMatchHitHeader; //!
        HIterator *iter_kine; //!
        HCategory* fGeantKineCat; //!
        // COUNTERS
    Int_t nCntRTMfull;//!
    Int_t nCntRTMnoRM;//!
    Int_t nCntRTMnoTM;//!
    Int_t nCntRTMnoTR;//!
    Int_t nCntRTM;//!
    Int_t nCntProcessedEvents;//!
    Int_t nCounterNbRejectedHits;//!
    Int_t nCntProcessedObjs;//!
    
};

#endif // HRICHTRACKLETFILTERSIM_H
