// $Id: htimelvl1evtfilter.h,v 1.3 2009-07-15 11:35:01 halo Exp $
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
        HTimeLvl1EvtFilter(const Text_t *name,const Text_t *title,TString opt, Int_t m=0,Float_t timecut=100., Float_t MultSmearing=0.);
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
    Float_t fMultSmearing; //smearing in multiplicity due to analogue summing
    Int_t nFilter; //!
    TString filterOption; //!
    
};

#endif //  HTIMELVL1EVTFILTER_H
