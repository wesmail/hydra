#ifndef HRICHDUMPCAL_H
#define HRICHDUMPCAL_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;

class HRichDumpCal : public HReconstructor {
public:

	HRichDumpCal();
        HRichDumpCal(Text_t *name,Text_t *title);
	~HRichDumpCal();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
        HCategory* getCalCat(){return fCalCat;}
        void setCalCat(HCategory* pCalCat){fCalCat = pCalCat;}
    
        ClassDef(HRichDumpCal,1) //ROOT extension

private:
	Int_t nCounter;
        HCategory *fCalCat; //!
        HIterator* fIter;    //!
};

#endif // HRICHDUMPCAL_H
