#ifndef HMATRIXCATEGORYSPLIT_H
#define HMATRIXCATEGORYSPLIT_H

#include "hcategorysplit.h"
#include <TArrayI.h>
#include <TString.h>


class HMatrixCategorySplit : public HCategorySplit {
public:
  HMatrixCategorySplit(void);
  HMatrixCategorySplit(Text_t *className,Int_t n,Int_t *siz,
		       Text_t *branchPattern,TObject *header=NULL); 
  ~HMatrixCategorySplit(void);
  Bool_t setup(Int_t *moduleSizes,Float_t fillRate=0.1);
  Bool_t setup(Int_t nActive,Int_t *activeModules,Int_t size,
	       Float_t fillRate=0.1);
  TObject *&getNewSlot(HLocation &aLoc);
  TObject *&getSlot(HLocation &aLoc);
  HDataObject *getObject(HLocation &aLoc);
  ClassDef(HMatrixCategorySplit,1) //Multibranch matrix category
};
  
		       
#endif /* !HMATRIXCATEGORYSPLIT_H */
