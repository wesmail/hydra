// $Id: hrichtrackletfilter.h,v 1.7 2002-10-02 09:26:19 eberl Exp $
// Last update by Thomas Eberl: 02/10/02 11:25:17
//
#ifndef HRICHTRACKLETFILTER_H
#define HRICHTRACKLETFILTER_H

#include "hreconstructor.h"
#include <TH1.h>
class HCategory;
class HIterator;
class HHitMatch;
class HRichTrackletFilter : public HReconstructor {
public:

	HRichTrackletFilter();
        HRichTrackletFilter(Text_t *name,Text_t *title,Int_t filt);
	~HRichTrackletFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HCategory* getMdcSegCat() { return fMdcSeg ; };
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
        void resetCounters();
        Bool_t delHitMatch(TObjArray *);
        void filter(){};
        void filterTrackletIndividual(Int_t);
        void filterTrackletInGroup(Int_t);
        Bool_t isGood(HHitMatch*,Int_t);
        Bool_t removeSingleRing();
        Int_t countCorrelationObjects();
        Bool_t isSkipNextTask(Int_t);
        Bool_t containsRM(){};
        Bool_t isBentTrack(HHitMatch*);
        Bool_t isCloseRMinTheta(HHitMatch*);
        Bool_t isEvtWithoutCorr();
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

    Int_t nFilter;//!
};

#endif // HRICHTRACKLETFILTER_H
