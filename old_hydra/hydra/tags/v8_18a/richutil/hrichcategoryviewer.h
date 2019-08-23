#ifndef HRICHCATEGORYVIEWER_H
#define HRICHCATEGORYVIEWER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;

class HRichCategoryViewer : public HReconstructor {
public:

	HRichCategoryViewer(Text_t *name,Text_t *title,Int_t category);
        HRichCategoryViewer();
	~HRichCategoryViewer();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
 Int_t fCategory;
	
private:
	ClassDef(HRichCategoryViewer,1) //ROOT extension
};

#endif // HRICHCATEGORYVIEWER_H


