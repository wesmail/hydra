// $Id: htimelvl1evtfilter.h,v 1.1 2007-02-09 15:35:40 halo Exp $
// Last update by Filip Krizek: 24/11/06 
//
#ifndef HTIMELVL1EVTFILTER_H
#define HTIMELVL1EVTFILTER_H

#include "hcategory.h"
#include "hreconstructor.h"

class HCategory;
class HIterator;
class HTimeLvl1EvtFilter : public HReconstructor {
public:

	HTimeLvl1EvtFilter();
        HTimeLvl1EvtFilter(Text_t *name,Text_t *title,TString opt, Int_t m=0,Float_t timecut=100.);
	~HTimeLvl1EvtFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();              
        HCategory* getHitTofCat(){return fTofHits;};
        HCategory* getCalTofinoCat(){return fTofinoCal;};
        void resetCounters();
        Bool_t isFilteredEvt(TString);  
        Bool_t isSkipNextTask(Int_t swt);
	Bool_t isTofMultTrigger();
       ClassDef(HTimeLvl1EvtFilter,1) //ROOT extension

private:
	
        HCategory* fTofHits; //!
        HCategory* fTofinoCal;
        HIterator* fTofIter; //!
        HIterator* fTofinoIter; //!

    Int_t nCntProcessedEvents;//!
    Int_t nCounterNbRejectedEvts;//!
    Int_t nMult;//!
    Float_t fTimeCut;
    Int_t nFilter; //!
    TString filterOption; //!
    
};

#endif //  HTIMELVL1EVTFILTER_H
