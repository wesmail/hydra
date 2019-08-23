#ifndef HCATEGORYSPLITITER_H
#define HCATEGORYSPLITITER_H

#include "hiterator.h"
#include "hcategorysplit.h"

class HCategorySplitIter : public HIterator {
private:
  HLocation fLoc; //Current location
  Int_t fLowerBound; //Minimun value for fCursorStart
  Int_t fHigherBound; //Maximun value for fCursorEnd
  Int_t fCursorStart; //First module index in iteration
  Int_t fCursorEnd; //Last module index in iteration
  Int_t fCursor; //module cursor
  TClonesArray *fCurrentClones; //!Pointer to the current module
  Int_t fCloneStart; //First cell index in module
  Int_t fCloneEnd; //Last cell index in module
  Int_t fCloneCursor; //cell index
  Bool_t fFinnished; //Iteration is finished or not
  HCategorySplit *fCat;//!Category being iterated
  HCategorySplitIter(void);
public:
  HCategorySplitIter(HCategorySplit *cat,Bool_t dir=kIterForward);
  virtual ~HCategorySplitIter(void);
  TCollection *GetCollection(void) const;
  TObject *Next(void);
  void Reset(void);
  Bool_t gotoLocation(HLocation &loc);
  HLocation &getLocation(void);
  ClassDef(HCategorySplitIter,1) //Iterator for a HCategorySplit
};



#endif /* !HCATEGORYSPLITITER_H */
