#ifndef HLINEARCATEGORY_H
#define HLINEARCATEGORY_H

#include "hcategory.h"

class HLinearCatIter;

class HLinearCategory : public HCategory {
friend class HLinearCatIter;
protected:
  TClonesArray *fData; // Data array of the category
public:
  HLinearCategory(void);
  HLinearCategory(Text_t *className,Int_t size=1000);
  ~HLinearCategory(void);
  const Text_t *getClassName(void);
  void makeBranch(TBranch *parent);
  void activateBranch(TTree *tree,Int_t splitLevel);
  TObject *&getNewSlot(HLocation &aLoc);
  TObject *&getSlot(HLocation &aLoc);
  TObject *&getSlot(Int_t index);
  HDataObject *getObject(HLocation &aLoc);
  HDataObject *getObject(Int_t index);
  Bool_t filter(HFilter &aFilter);
  Bool_t filter(HLocation &aLoc,HFilter &aFilter);
  void Clear(void);
  void Compress(void) {fData->Compress();}   // compresss the TClonesArray
  Bool_t isSelfSplitable(void);
  TIterator *MakeIterator(Bool_t dir=kIterForward);
  ClassDef(HLinearCategory,1) //Generic data category
};
  

#endif /* !HLINEARCATEGORY_H */

