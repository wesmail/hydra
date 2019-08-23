// File: hrichevtfilter.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/07/23 15:56:02
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
        Bool_t containsRichHit();
        Bool_t isTofMultTrigger();
        Bool_t containsCorrRichHit();
       ClassDef(HRichEvtFilter,1) //ROOT extension

private:
	HCategory* pHitMatchCat; //!
        HCategory* fRichPID; //!
        HCategory* pHitMatchHeaderCat; //!
        HCategory* fMdcSeg; //!
        HCategory* pHitRichCat; //!
        HCategory* fTofHits; //!
        HCategory* fTofinoCal;//!
        HIterator* pIterMatchHit; //!
        HIterator* fRichIter;    //!
        HIterator* pIterMatchHitHeader; //!
        HIterator* fMdcSegIter;    //!
        HIterator* fTofIter; //!

    Int_t nCntProcessedEvents;//!
    Int_t nCounterNbRejectedEvts;//!
    Int_t nMult;//!
    Int_t nFilter; //!
    TString filterOption; //!
    
};

#endif // HRICHEVTFILTER_H
