// $Id: hrichpairanasim.h,v 1.4 2008-09-18 13:05:36 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:12:22
//
#ifndef HRICHPAIRANASIM_H
#define HRICHPAIRANASIM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TFile.h"
class HCategory;
class HIterator;
class HHitMatchSim;
class HRichPairAnaSim : public HReconstructor {
public:

	HRichPairAnaSim();
        HRichPairAnaSim(Text_t *name,Text_t *title);
	~HRichPairAnaSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void iniCounters();
        void iniHistos();
        void countCorrelationObjects();
        Int_t countRTMobjs();
        Int_t countRMobjs();
        Int_t fillRTMobjs(TObjArray*);
        Int_t fillRTMfullobjs(TObjArray*);
        Int_t fillRMobjs(TObjArray*);
        void calcOpeningAngle(TObjArray*,TH1*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitMatchHeaderCat(){return pHitMatchHeaderCat;};
        HCategory* getRichHitCat() { return fRichPID; };
        Bool_t delHitMatch(TObjArray *);
        void filter();
        Bool_t isGood(HHitMatchSim*);
        ClassDef(HRichPairAnaSim,1) //ROOT extension

protected:

	HCategory* pHitMatchCat; //!
        HCategory *fRichPID; //!
        HCategory* pHitMatchHeaderCat; //!
        HIterator* pIterMatchHit; //!
        HIterator* fRichIter;    //!
        HIterator* pIterMatchHitHeaderSim; //!
    // COUNTERS
    Int_t nCntRTMfull;//!
    Int_t nCntRTMnoRM;//!
    Int_t nCntRTMnoTM;//!
    Int_t nCntRTMnoTR;//!
    Int_t nCntRTM;//!
    Int_t nCntProcessedEvents;//!
    Int_t nCounterNbRejectedHits;//!
     
    //Histos
    TFile *pFileOut;//!
    TH1F* pHistRTMopenAngle;//!
    TH1F* pHistRMopenAngle;//!
};

#endif // HRICHPAIRANASIM_H

