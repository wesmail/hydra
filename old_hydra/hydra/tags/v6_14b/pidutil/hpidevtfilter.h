// $Id: hpidevtfilter.h,v 1.1 2003-02-26 08:53:57 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 09:02:23
//
#ifndef HPIDEVTFILTER_H
#define HPIDEVTFILTER_H

#include "hreconstructor.h"

class HCategory;
class HIterator;

class HPidEvtFilter : public HReconstructor {
public:

	HPidEvtFilter();
        HPidEvtFilter(Text_t *name,Text_t *title,TString opt, Int_t m=0);
	~HPidEvtFilter();

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
        Bool_t isFilteredEvt(TString);
        Bool_t isSkipNextTask(Int_t);
        Bool_t isMdcMultTrigger();
        Bool_t containsRichHit();
        Bool_t isTofMultTrigger();
       ClassDef(HPidEvtFilter,1) //ROOT extension

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

#endif // HPIDEVTFILTER_H
