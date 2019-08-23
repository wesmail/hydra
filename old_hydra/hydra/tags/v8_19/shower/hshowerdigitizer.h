#ifndef HShowerDigitizer_H
#define HShowerDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TObject.h"

class HCategory;
class HIterator;

class HShowerDigitizer : public HReconstructor {
public:
        HShowerDigitizer();
        HShowerDigitizer(Text_t *name,Text_t *title);
        ~HShowerDigitizer();

        virtual Int_t execute(void);
        virtual Bool_t digitize(TObject*){return 0;}
        HShowerDigitizer &operator=(HShowerDigitizer &c);

        HCategory* getInCat(){return m_pInCat;}
        HCategory* getOutCat(){return m_pOutCat;}
        HParSet* getDigiParSet(){return m_pDigiParSet;}
        HParSet* getGeometry(){return m_pGeometry;}

        void setInCat(HCategory* pInCat){m_pInCat = pInCat;}
        void setOutCat(HCategory* pOutCat){m_pOutCat = pOutCat;}
        void setDigiParSet(HParSet* pDigiParSet){m_pDigiParSet= pDigiParSet;}
        void setGeometry(HParSet* pGeometry){m_pGeometry= pGeometry;}

        virtual Bool_t init(void);
        Bool_t initParameters(void);

        ClassDef(HShowerDigitizer,0) // Shower digitizer base class

        Int_t lNrEvent;

protected:
        virtual Int_t sort(void){return 0;}

        HCategory *m_pInCat;    //!Pointer to the raw data category
        HCategory *m_pOutCat;   //!Pointer to the cal data category

        HParSet *m_pDigiParSet; //!
        HParSet *m_pGeometry;   //!

        HIterator* fIter;       //!
};

#endif
