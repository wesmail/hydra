// $Id: hrichdumpraw.h,v 1.4 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:10:04
//
#ifndef HRICHDUMPRAW_H
#define HRICHDUMPRAW_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TH2.h"
#include "TFile.h"
class HCategory;
class HIterator;

class HRichDumpRaw : public HReconstructor {
public:

	HRichDumpRaw();
        HRichDumpRaw(const Text_t *name,const Text_t *title);
	~HRichDumpRaw();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        HCategory* getRawCat(){return fRawCat;}
        void setCalCat(HCategory* pRawCat){fRawCat = pRawCat;}
    
        ClassDef(HRichDumpRaw,1) //ROOT extension

private:
	Int_t nCounter;
        HCategory *fRawCat; //!
        HIterator* fIter;    //!
	TH2F *h1;//!
	TFile *ff;//!
};

#endif // HRICHDUMPRAW_H
