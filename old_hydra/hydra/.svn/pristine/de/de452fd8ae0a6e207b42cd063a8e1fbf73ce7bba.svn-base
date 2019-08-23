// $Id: hrichdumphit.h,v 1.3 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:09:33
//
#ifndef HRICHDUMPHIT_H
#define HRICHDUMPHIT_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;

class HRichDumpHit : public HReconstructor {
public:

	HRichDumpHit();
        HRichDumpHit(const Text_t *name,const Text_t *title);
	~HRichDumpHit();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        HCategory* getHitCat(){return fHitCat;}
        void setHitCat(HCategory* pHitCat){fHitCat = pHitCat;}
    
        ClassDef(HRichDumpHit,1) //ROOT extension

protected:
	Int_t nCounter;
        Int_t nEvtCounter;
        HCategory *fHitCat; //!
        HIterator* fIter;    //!
};

#endif // HRICHDUMPHIT_H
