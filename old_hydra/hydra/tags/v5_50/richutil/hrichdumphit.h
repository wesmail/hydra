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
        HRichDumpHit(Text_t *name,Text_t *title);
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
