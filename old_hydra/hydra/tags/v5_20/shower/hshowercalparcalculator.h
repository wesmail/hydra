#ifndef HShowerCalParCalculator_H
#define HShowerCalParCalculator_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;

class HShowerCalParCalculator : public HReconstructor {
public:
	HShowerCalParCalculator(Int_t nPeaksNumber = 2);
        HShowerCalParCalculator(Text_t *name,Text_t *title, 
                                            Int_t nPeaksNumber = 2);
	~HShowerCalParCalculator();

        Bool_t init(void);
        Bool_t reinit(void);
        Bool_t finalize(void);
        Int_t execute(void);

        HParSet* getCalPar(){return m_pCalPar;}
        HParSet* getCalParHist(){return m_pCalParHist;}

        void setCalPar(HParSet*);
        void setCalParHist(HParSet*);

        void initCalPar();
        

        ClassDef(HShowerCalParCalculator,1) //ROOT extension

private:
        Int_t nn;
        Int_t m_nPeaksNumber;
        HLocation m_zeroLoc;
        HLocation m_loc; 

        HCategory *m_pRawCat; //!Pointer to the raw data category

        HParSet *m_pCalPar; //!Pointer to the cal data category
        HParSet *m_pCalParHist;

        HIterator *fIter;  //!
};

#endif
