// $Id: hrichdiag.h,v 1.5 2009-07-15 11:39:21 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:09:04
//
#ifndef HRICHDIAG_H
#define HRICHDIAG_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
class HCategory;
class HIterator;
class HRichUtilFunc;
class HRichDiag : public HReconstructor {
public:

	HRichDiag();
        HRichDiag(const Text_t *name,const Text_t *title,const Char_t*,Int_t);
	~HRichDiag();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        HCategory* getCalCat(){return pCalCat;}
        void setCalCat(HCategory* pCat){pCalCat = pCat;}
        HCategory* getHitCat(){return pHitCat;}
        void setHitCat(HCategory* pCat){pHitCat = pCat;}

        void initControlHistos();
        
        ClassDef(HRichDiag,1) //ROOT extension

protected:
	HRichUtilFunc *pUtil;//!
        TFile *pFileOut;//!
	Int_t nCounterProcessedNbEvents;
        Int_t nEvtNum;
        Float_t **xbins;//!
        Float_t **chrgavrg;//!
        Float_t **padmult;//!
        HCategory* pCalCat;//!
        HIterator* pCalIter;//!
        HCategory* pHitCat;//!
        HIterator* pHitIter;//!
    // HISTOGRAMS
    TObjArray* pHistArray;//!
    TObjArray* pGraphArray;//!
  };

#endif // HRICHDIAG_H