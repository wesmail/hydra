#pragma interface
#ifndef HCATEGORY_H
#define HCATEGORY_H

#include <TObject.h>
#include "hlocation.h"
#include <TCollection.h>
#include <TBranch.h>
#include <TClonesArray.h>

class TClass;
class HFilter;
class HDataObject;

class HCategory : public TObject {
protected:
  Bool_t fPersistency; //Indicates whether this category is stored in output.
  Cat_t fCat; //Identifier for this category
  Int_t fBranchingLevel; //Number of levels for the data in the category
  TObject *fHeader;//- Categorys header.
public:
  HCategory(void);
  ~HCategory(void);
  void setBranchingLevel(Int_t nLevel);
  void setCategory(Cat_t aCat);
  Bool_t IsPersistent(void);
  void setPersistency(Bool_t per);
  Cat_t getCategory(void);
  Int_t getBranchingLevel(void);
  TObject *getHeader(void) {return fHeader;}
  virtual const Text_t *getClassName(void)=0;
//  virtual TClass *getClass(void)=0;
  virtual TClass *getClass(void){return NULL;} 
  virtual void makeBranch(TBranch *parent)=0;
  virtual void activateBranch(TTree *tree,Int_t splitLevel);
  virtual TObject *&getNewSlot(HLocation &aLoc)=0;
  virtual TObject *&getSlot(HLocation &aLoc)=0;
  virtual HDataObject *getObject(HLocation &aLoc)=0;
  virtual Bool_t query(TCollection *aCol,HFilter &aFilter);
  virtual Bool_t query(TCollection *aCol,HLocation &aLoc);
  virtual Bool_t query(TCollection *aCol,HLocation &aLoc,HFilter &aFilter);
  virtual Bool_t filter(HFilter &aFilter)=0;
  virtual Bool_t filter(HLocation &aLoc,HFilter &aFilter)=0;
  virtual void Clear(void)=0;
  virtual Bool_t isSelfSplitable(void)=0;
  virtual TIterator *MakeIterator(Bool_t dir=kIterForward)=0;
  TIterator *MakeReverseIterator(void);
  Bool_t IsFolder(void);
  ClassDef(HCategory,1) //Generic data category
};

#endif /* !HCATEGORY */
