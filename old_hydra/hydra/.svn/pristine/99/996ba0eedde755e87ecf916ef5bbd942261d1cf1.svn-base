#ifndef HShowerCopy_H
#define HShowerCopy_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hdataobject.h"
#include "hshowerrawsim.h"

class HCategory;
class HIterator;

class HShowerCopy : public HReconstructor {
public:
	HShowerCopy();
        HShowerCopy(Text_t *name,Text_t *title);
	~HShowerCopy();

        Int_t execute(void);
        virtual Bool_t init(void);
        Bool_t finalize(void) {return kTRUE;}

        void setInCat(HCategory* pInCat){m_pInCat = pInCat;}
        void setOutCat(HCategory* pOutCat){m_pOutCat = pOutCat;}
      
        ClassDef(HShowerCopy,1) //ROOT extension

	Bool_t copy(HShowerRawMatrSim *pMatr);  
  
        Int_t m_nEvents;

private:

	HLocation m_zeroLoc; 
        HCategory *m_pInCat; //Pointer to the rawMatr data category
        HCategory *m_pOutCat; //Pointer to the raw  data category

        HIterator* fIter;     //!
};

#endif
