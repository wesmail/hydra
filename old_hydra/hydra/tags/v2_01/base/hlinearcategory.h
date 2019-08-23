#ifndef HLINEARCATEGORY_H
#define HLINEARCATEGORY_H

#include "hcategory.h"

class HLinearCatIter;

class HLinearCategory : public HCategory {
friend class HLinearCatIter;
protected:
  TClonesArray *fData; //Data array.
public:
  HLinearCategory(void);
  HLinearCategory(Text_t *className,Int_t size=1000);
  ~HLinearCategory(void);
  const Text_t *getClassName(void);
  void makeBranch(TBranch *parent);
  void activateBranch(TTree *tree,Int_t splitLevel);
  TObject *&getNewSlot(HLocation &aLoc);
  TObject *&getSlot(HLocation &aLoc);
  HDataObject *getObject(HLocation &aLoc);
  Bool_t filter(HFilter &aFilter);
  Bool_t filter(HLocation &aLoc,HFilter &aFilter);
  void Clear(void);
  Bool_t isSelfSplitable(void);
  TIterator *MakeIterator(Bool_t dir=kIterForward);
  ClassDef(HLinearCategory,1) //Generic data category
};
  

#endif /* !HLINEARCATEGORY_H */
