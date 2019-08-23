// $Id: hrichpairana.h,v 1.6 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:12:04
//
#ifndef HRICHPAIRANA_H
#define HRICHPAIRANA_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TFile.h"
class HCategory;
class HIterator;
class HHitMatch;
class HRichPairAna : public HReconstructor {
public:

	HRichPairAna();
        HRichPairAna(const Text_t *name,const Text_t *title);
	~HRichPairAna();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void iniCounters();
        void iniHistos();
        void calcOpenAngles();
        void countCorrelationObjects();
        Int_t countRTMobjs();
        Int_t countRMobjs();
        Int_t fillRTMobjs(TObjArray*);
        Int_t fillRTMfullobjs(TObjArray*);
        Int_t fillRMobjs(TObjArray*);
        void calcOpeningAngle(TObjArray*,TH1*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getRichHitCat() { return fRichPID; };
        Bool_t delHitMatch(TObjArray *);
        void filterTrackletIndividual(Int_t);
        void filterTrackletInGroup(Int_t);
        Bool_t removeSingleRing();
        Bool_t isGood(HHitMatch*,Int_t);
        Bool_t isFilteredEvt(Int_t);
        Bool_t isSkipNextTask(Int_t);
        Bool_t isDoubleRingEvt();
        ClassDef(HRichPairAna,1) //ROOT extension

protected:

	HCategory* pHitMatchCat; //!
        HCategory *fRichPID; //!

        HIterator* pIterMatchHit; //!
        HIterator* fRichIter;    //!
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

#endif // HRICHPAIRANA_H

