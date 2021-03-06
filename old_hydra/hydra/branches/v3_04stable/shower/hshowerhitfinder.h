#ifndef HShowerHitFinder_H
#define HShowerHitFinder_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hratree.h"

class HCategory;
class HIterator;
class HShowerHit;
class HShowerPID;
class HShowerHitHeader;

class HShowerHitFinder : public HReconstructor {
public:
	HShowerHitFinder();
        HShowerHitFinder(Text_t *name,Text_t *title);
	~HShowerHitFinder();

        Int_t execute(void);
        Bool_t init(void);
        Bool_t finalize(void);
        Bool_t initParameters(void);

        Bool_t lookForHit(HLocation &fLoc);
	Float_t calculateSum(HLocation &fLoc, int nRange, int* pncs = NULL);
	Float_t calculateVar(HLocation &fLoc, int nRange, float avg);
	Bool_t isLocalMax(HLocation &fLoc);
        HShowerHitFinder &operator=(HShowerHitFinder &c);

        HCategory* getCalCat(){return m_pCalCat;}
        HCategory* getHitCat(){return m_pHitCat;}
        HCategory* getPIDCat(){return m_pPIDCat;}
        HParSet* getHitFPar(){return m_pHitFPar;}
        HParSet* getGeometry(){return m_pGeometry;}

        void setCalCat(HCategory* pCalCat){m_pCalCat = pCalCat;}
        void setHitCat(HCategory* pHitCat){m_pHitCat = pHitCat;}
        void setPIDCat(HCategory* pPIDCat){m_pPIDCat = pPIDCat;}
        void setHitFPar(HParSet* pPar){m_pHitFPar= pPar;}
        void setGeometry(HParSet* pGeometry){m_pGeometry= pGeometry;}
        

        ClassDef(HShowerHitFinder,1) //ROOT extension

private:
        HRaTree* m_pCellArr;

        HCategory *m_pCalCat; //Pointer to the cal data category
        HCategory *m_pHitCat; //Pointer to the hit data category
        HCategory *m_pPIDCat; //Pointer to the hit data category
        HCategory *m_pHitHdrCat; //Pointer to the hit header data category

        HParSet *m_pHitFPar; //Pointer to the cal data category
        HParSet *m_pGeometry; //!

        HIterator *fIter; //!

	void fillSums(HShowerHit* hit, HLocation &fLoc);
        virtual void calcCoord(HShowerHit* hit, HLocation &fLoc);

        void fillPID(HShowerHit* hit, HShowerPID* pid);

	HShowerHitHeader* getHitHeader(HLocation &fLoc);
	void updateClusters(HLocation &fLoc);
	void updateLocalMax(HLocation &fLoc);
	void updateFiredCells(HLocation &fLoc);
};

#endif
