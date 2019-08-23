#ifndef HBITFIELDCATEGORY_H
#define HBITFIELDCATEGORY_H

#include "TArrayI.h"
#include "hlocation.h"
#include "hcompositecategory.h"
#include "hclonesarray.h"
#include "hbarray.h"
#include "htree.h"



class HBitFieldCategory: public HCompositeCategory {

 protected:
 HClonesArray *fData;         //  Data array of the category
 Int_t fNDataObjs;            //  Multiplicity /evts
 HBArray *fBitIndex;          //  bit-based index table
 Bool_t testbit ;             //! Io flag 

public:
  HBitFieldCategory(void);
  HBitFieldCategory(Text_t *className, Int_t nDim, Int_t *sizes,
                     Float_t fillRate=0.1);
  ~HBitFieldCategory(void);
  
  const Text_t *getClassName(void);
  void setCapacity(Text_t *className,
			  Int_t nDim,
			  Int_t *sizes, Float_t fillrate=0.1);

  TClass *getClass(void) {return fData->GetClass();}
  void makeBranch(TBranch *parent);
  void makeBranch(HTree *tree);
  void activateBranch(TTree *tree,Int_t splitLevel);
  Int_t getEntries() {return fData->GetEntriesFast();}
  TObject *&getNewSlot(HLocation &aLoc,Int_t* pIndex=0);
  TObject *&getSlot(HLocation &aLoc,Int_t* pIndex=0);
  TObject *&getSlot(Int_t index);
  TObject *getObject(HLocation &aLoc);
  TObject *getObject(Int_t index);
  Bool_t filter(HFilter &aFilter);
  Bool_t filter(HLocation &aLoc,HFilter &aFilter);
  void Clear(void);
  void Compress() {fData->Compress();}   
  inline Bool_t isSelfSplitable(void);
  TIterator *MakeIterator(Option_t* opt="catIter",Bool_t dir=kIterForward);
  Int_t getIndex(HLocation &aLoc) {return aLoc[0];}
  const TCollection* getCollection(){return fData;} 
  HBArray* getBitField() {return fBitIndex;}
  void setBitField(HBArray* arr) { fBitIndex=arr;}
  void setClonesArray( HClonesArray* arr ) { fData=arr;}  
 ClassDef(HBitFieldCategory,1) // Bit based data category
};

//----------------------------inlines------------------------------------
inline Bool_t HBitFieldCategory::isSelfSplitable(void) {
 return kFALSE;
}
 
#endif /* !HBITFIELDCATEGORY_H */
