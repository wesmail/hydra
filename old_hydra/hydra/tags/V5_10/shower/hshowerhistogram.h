#ifndef HShowerHistogram_H
#define HShowerHistogram_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hdataobject.h"

class HCategory;
class HIterator;

class HShowerHistogram : public HReconstructor {
public:
	HShowerHistogram();
        HShowerHistogram(Text_t *name,Text_t *title);
	~HShowerHistogram();

        Bool_t init(void);
        Bool_t finalize(void); 
        Int_t execute(void);
 
	Int_t draw(void);

        ClassDef(HShowerHistogram,1) //ROOT extension

private:


        HIterator* fIter;     //!
};

#endif
