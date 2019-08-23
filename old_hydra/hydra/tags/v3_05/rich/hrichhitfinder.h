#ifndef HRichHitFinder_H
#define HRichHitFinder_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hratree.h"

class HCategory;
class HIterator;
class HRichHitHeader;

class HRichHitFinder : public HReconstructor {
public:
	HRichHitFinder();
        HRichHitFinder(Text_t *name,Text_t *title);
	~HRichHitFinder();

        Bool_t init(void);
        Bool_t finalize(void);
        Int_t execute(void);
        Bool_t lookForHit(HLocation &fLoc);
        HRichHitFinder &operator=(HRichHitFinder &c);

        HCategory* getCalCat(){return m_pCalCat;}
        HCategory* getHitCat(){return m_pHitCat;}
        HParSet* getHitFPar(){return m_pHitFPar;}

        void setCalCat(HCategory* pCalCat){m_pCalCat = pCalCat;}
        void setHitCat(HCategory* pHitCat){m_pHitCat = pHitCat;}
        void setHitFPar(HParSet* pPar){m_pHitFPar= pPar;}
        

        ClassDef(HRichHitFinder,1) //ROOT extension

private:
        HRaTree* m_pCellArr;

        HCategory *m_pCalCat; //Pointer to the cal data category
        HCategory *m_pHitCat; //Pointer to the hit data category
        HCategory *m_pHitHdrCat; //Pointer to the hit header data category

        HParSet *m_pHitFPar; //Pointer to the cal data category

        HIterator *fIter;    //!
};

#endif
