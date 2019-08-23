// $Id: hrichevtfilter.h,v 1.4 2004-10-15 08:25:26 eberl Exp $
// Last update by Thomas Eberl: 04/04/23 10:52:36
//
#ifndef HRICHEVTFILTER_H
#define HRICHEVTFILTER_H

#include "hreconstructor.h"

class HCategory;
class HIterator;
class HHitMatch;
class HRichEvtFilter : public HReconstructor {
public:

	HRichEvtFilter();
        HRichEvtFilter(Text_t *name,Text_t *title,TString opt, Int_t m=0);
	~HRichEvtFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HCategory* getMdcSegCat() { return fMdcSeg ; };
        HCategory* getKickCat() { return fKickTrackCat ; };
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitRichCat(){return pHitRichCat;};
        HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
        HCategory* getHitTofCat(){return fTofHits;};
        HCategory* getCalTofinoCat(){return fTofinoCal;};
        void resetCounters();
        Bool_t isDoubleRingEvt();
        Bool_t isFilteredEvt(TString);
        Bool_t isSkipNextTask(Int_t);
        Bool_t containsRM();
        Bool_t isMdcMultTrigger();
        Bool_t isKickMult();
        Bool_t containsRichHit();
        Bool_t isTofMultTrigger();
        Bool_t containsCorrRichHit();
        Bool_t containsGoodCorr();
        Bool_t isTargetEvt();
       ClassDef(HRichEvtFilter,1) //ROOT extension

private:
	HCategory* pHitMatchCat; //!
        HCategory* fRichPID; //!
        HCategory* pHitMatchHeaderCat; //!
        HCategory* fMdcSeg; //!
        HCategory* fKickTrackCat; //!
        HCategory* pHitRichCat; //!
        HCategory* fTofHits; //!
        HCategory* fTofinoCal;//!
        HIterator* pIterMatchHit; //!
        HIterator* fRichIter;    //!
        HIterator* pIterMatchHitHeader; //!
        HIterator* fMdcSegIter;    //!
        HIterator* fKickIter;    //!
        HIterator* fTofIter; //!

    Int_t nCntProcessedEvents;//!
    Int_t nCounterNbRejectedEvts;//!
    Int_t nMult;//!
    Int_t nFilter; //!
    TString filterOption; //!
    
};

#endif // HRICHEVTFILTER_H
