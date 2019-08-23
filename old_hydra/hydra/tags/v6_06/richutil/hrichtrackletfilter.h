// File: hrichtrackletfilter.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/14 15:19:56
//

#ifndef HRICHTRACKLETFILTER_H
#define HRICHTRACKLETFILTER_H

#include "hreconstructor.h"

class HCategory;
class HIterator;
class HHitMatch;
class HRichTrackletFilter : public HReconstructor {
public:

	HRichTrackletFilter();
        HRichTrackletFilter(Text_t *name,Text_t *title);
	~HRichTrackletFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HCategory* getMdcSegCat() { return fMdcSeg ; };
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
        void resetCounters();
        Bool_t delHitMatch(TObjArray *);
        void filter();
        void filterTrackletIndividual(Int_t);
        void filterTrackletInGroup(Int_t);
        Bool_t isGood(HHitMatch*,Int_t);
        Bool_t isDoubleRingEvt();
        Bool_t removeSingleRing();
        Int_t countCorrelationObjects();
        Bool_t isFilteredEvt(Int_t);
        Bool_t isSkipNextTask(Int_t);
    Bool_t containsRM();
    Bool_t isTargetEvt();
    void dumpCorrelationObjectStat();
 
       ClassDef(HRichTrackletFilter,1) //ROOT extension

private:
	HCategory* pHitMatchCat; //!
        HCategory* fRichPID; //!
        HCategory* pHitMatchHeaderCat; //!
        HCategory* fMdcSeg; //!
        HIterator* pIterMatchHit; //!
        HIterator* fRichIter;    //!
        HIterator* pIterMatchHitHeader; //!
        HIterator* fMdcSegIter;    //!
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

#endif // HRICHTRACKLETFILTER_H
