#ifndef HHypListFiller_H
#define HHypListFiller_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hhyplist.h"

class HCategory;
class HIterator;

class HHypListFiller : public HReconstructor {
public:
        HHypListFiller();
        HHypListFiller(Text_t *name,Text_t *title);
       ~HHypListFiller();

       Bool_t SetExitList(Int_t e_list);

        Bool_t init(void);
        Int_t  execute(void);
        Bool_t finalize(void);
		
        void setContCat(HCategory* pContCat){m_pContCat = pContCat;}

private:
	 Int_t exitList;

        HCategory *m_pContCat; //!Pointer to the hit data category

        ClassDef(HHypListFiller,0) //ROOT extension
};

#endif
