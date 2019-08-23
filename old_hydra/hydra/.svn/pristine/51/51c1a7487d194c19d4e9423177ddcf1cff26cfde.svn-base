// File: hrichpairana.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/09/18 17:29:01
//
#ifndef HRICHPAIRANA_H
#define HRICHPAIRANA_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <TH1.h>
#include <TObjArray.h>
#include <TFile.h>
class HCategory;
class HIterator;

class HRichPairAna : public HReconstructor {
public:

	HRichPairAna();
        HRichPairAna(Text_t *name,Text_t *title);
	~HRichPairAna();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void iniCounters();
        void iniHistos();
        void countCorrelationObjects();
        Int_t countRTMobjs();
        Int_t fillRTMobjs(TObjArray*);
        Int_t fillRTMfullobjs(TObjArray*);
        void calcOpeningAngle(TObjArray*);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getRichHitCat() { return fRichPID; };
    
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
     
    //Histos
    TFile *pFileOut;//!
    TH1F* pHistRTMopenAngle;//!
};

#endif // HRICHPAIRANA_H

