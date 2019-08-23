// $Id: hpidobjfilter.h,v 1.1 2003-02-26 08:53:08 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 09:05:25
//
#ifndef HPIDOBJFILTER_H
#define HPIDOBJFILTER_H

#include "hreconstructor.h"
class HCategory;
class HIterator;
class HParSet;
class HPidObjFilter : public HReconstructor {
public:

	HPidObjFilter();
        HPidObjFilter(Text_t *name,Text_t *title,char *swt);
	~HPidObjFilter();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HCategory* getRichHitCat() { return pRichHitCat; };
        void setRichHitCat(HCategory* cat) { pRichHitCat=cat; };
        void filter(){};
    ClassDef(HPidObjFilter,1) 

private:
    // data members
        HCategory* pRichHitCat;//!
        HIterator* pRichHitIter;//!
        Int_t nEvtsProcessed;//!
        TString *pFilterSwitch; //!

};

#endif // HPIDOBJFILTER_H
