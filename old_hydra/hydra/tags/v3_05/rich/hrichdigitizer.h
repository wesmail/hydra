#ifndef HRichDigitizer_H
#define HRichDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hdataobject.h"

class HCategory;
class HIterator;

class HRichDigitizer : public HReconstructor {
public:
	HRichDigitizer();
        HRichDigitizer(Text_t *name,Text_t *title);
	~HRichDigitizer();

        virtual Int_t execute(void);
        virtual Bool_t digitize(HDataObject*){return 0;}
        HRichDigitizer &operator=(HRichDigitizer &c);

        HCategory* getInCat(){return m_pInCat;}
        HCategory* getOutCat(){return m_pOutCat;}
        HParSet* getDigiParSet(){return m_pDigiParSet;}

        void setInCat(HCategory* pInCat){m_pInCat = pInCat;}
        void setOutCat(HCategory* pOutCat){m_pOutCat = pOutCat;}
        void setDigiParSet(HParSet* pDigiParSet){m_pDigiParSet= pDigiParSet;}
        
        virtual Bool_t init(void);

        ClassDef(HRichDigitizer,1) //ROOT extension

        Int_t lNrEvent;
private:

        HCategory *m_pInCat; //Pointer to the raw data category
        HCategory *m_pOutCat; //Pointer to the cal data category

        HParSet *m_pDigiParSet; //Pointer to the cal data category
        HIterator *fIter;     //!

};

#endif
