// $Id: hrichobjfilter.h,v 1.1 2002-11-28 15:49:27 eberl Exp $
// Last update by Thomas Eberl: 02/11/26 15:59:25
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
        HRichObjFilter(Text_t *name,Text_t *title,char *swt);
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
        HParSet* pCorrPar;//!
        Int_t nEvtsProcessed;//!
        TString *pFilterSwitch; //!
    // member functions
        void setCorrelationPar(HParSet* pPar){pCorrPar = pPar;};
        HParSet* getCorrelationPar(){return pCorrPar;};
};

#endif // HRICHOBJFILTER_H
