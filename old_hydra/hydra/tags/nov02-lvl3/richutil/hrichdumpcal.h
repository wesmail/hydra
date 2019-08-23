// $Id: hrichdumpcal.h,v 1.2 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 18:09:18
//
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
