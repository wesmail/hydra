// $Id: hrichobjfilter.h,v 1.4 2009-07-15 11:39:17 halo Exp $
// Last update by Thomas Eberl: 04/10/12 11:28:59
//
#ifndef HRICHOBJFILTER_H
#define HRICHOBJFILTER_H

#include "hreconstructor.h"
class HCategory;
class HIterator;
class HParSet;
class HRichObjFilter : public HReconstructor {
public:

	HRichObjFilter();
        HRichObjFilter(const Text_t *name,const Text_t *title,const Char_t *swt);
	~HRichObjFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HCategory* getRichHitCat() { return pRichHitCat; };
        void setRichHitCat(HCategory* cat) { pRichHitCat=cat; };
        HCategory* getHitMatchCat() { return pHitMatchCat; };
        void setHitMatchCat(HCategory* cat) { pHitMatchCat=cat; };
        void filter(){};
    ClassDef(HRichObjFilter,1) 

private:
    // data members
        HCategory* pRichHitCat;//!
        HIterator* pRichHitIter;//!
        HCategory* pHitMatchCat;//!
        HIterator* pHitMatchIter;//!
        HCategory* pDiHitMatchCat;//!
        HIterator* pDiHitMatchIter;//!
        HParSet* pCorrPar;//!
        Int_t nEvtsProcessed;//!
        TString *pFilterSwitch; //!
    // member functions
        void setCorrelationPar(HParSet* pPar){pCorrPar = pPar;};
        HParSet* getCorrelationPar(){return pCorrPar;};
};

#endif // HRICHOBJFILTER_H
