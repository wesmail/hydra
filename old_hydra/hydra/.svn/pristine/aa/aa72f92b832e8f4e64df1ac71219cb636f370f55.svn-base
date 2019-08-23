#ifndef HMATRIXCATEGORY_H
#define HMATRIXCATEGORY_H

#include "hcategory.h"
#include <TArrayI.h>
#include <TClonesArray.h>

class HIndexTable;
class HMatrixCatIter;
class TClass;

class HMatrixCategory : public HCategory {
friend class HMatrixCatIter;
protected:
  HIndexTable *fIndexTable; //Table indicating the position in fData for the data objects
  Int_t fNDataObjs; //Number of data objects actually stored.
  TClonesArray *fData; // Container for the data in the Matrix category;
public:
   HMatrixCategory(void);					        
   HMatrixCategory(Text_t *className,Int_t nSizes,Int_t *sizes,Float_t fillRate=0.1);
   ~HMatrixCategory(void);
   void setup(Text_t *className,Int_t nSizes,Int_t *sizes,Float_t fillRate=0.1);
   const Text_t *getClassName(void);
   TClass *getClass(void);
   TArrayI *getSizes(void);
   Int_t getSize(Int_t aIdx);
   void activateBranch(TTree *tree,Int_t splitLevel);
   void makeBranch(TBranch *parent);
   Bool_t isSelfSplitable(void);
   TObject *&getNewSlot(HLocation &aLoc);
   TObject *&getSlot(HLocation &aLoc);
   HDataObject *getObject(HLocation &aLoc);
   Bool_t query(TCollection *aCol,HFilter &aFilter);
   Bool_t query(TCollection *aCol,HLocation &aLoc);
   Bool_t query(TCollection *aCol,HLocation &aLoc,HFilter &aFilter);
   Bool_t filter(HFilter &aFilter);
   Bool_t filter(HLocation &aLoc,HFilter &aFilter);
   TIterator *MakeIterator(Bool_t dir=kIterForward);
   void Clear(void);
   void Browse(TBrowser *b);
   ClassDef(HMatrixCategory,1) //Category where data are stored in a vector
};

#endif /* !HMATRIXCATEGORY_H */
