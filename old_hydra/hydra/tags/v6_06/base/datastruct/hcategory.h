#pragma interface
#ifndef HCATEGORY_H
#define HCATEGORY_H

#include <TObject.h>
#include "hlocation.h"
#include <TCollection.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include "hdataobject.h"

class TClass;
class HFilter;
class TObject;

class HCategory : public TObject {
protected:
  Bool_t fPersistency; //! Indicates whether this category is stored in output.
  Cat_t fCat; // Identifier for this category
  Int_t fBranchingLevel; // Number of levels for the data in the category
  TObject *fHeader;// Category header.
public:
  HCategory(void);
  ~HCategory(void);
  void setBranchingLevel(Int_t nLevel);
  void setCategory(Cat_t aCat);
  Bool_t IsPersistent(void);
  void setPersistency(Bool_t per);
  Cat_t getCategory(void);
  Int_t getBranchingLevel(void);
  void setCatHeader(TObject* fObj) {fHeader=fObj;}
  TObject *getCatHeader(void) {return fHeader;}
  virtual const Text_t *getClassName(void);
//  virtual TClass *getClass(void) {};
  virtual TClass *getClass(void){return NULL;} 
  virtual void makeBranch(TBranch *parent);
  virtual void activateBranch(TTree *tree,Int_t splitLevel);
  virtual TObject *&getNewSlot(HLocation &aLoc,Int_t* pIndex=0);
  virtual TObject *&getSlot(HLocation &aLoc,Int_t* pIndex=0);
  virtual TObject *getObject(HLocation &aLoc);
  virtual TObject *getObject(Int_t index) {return NULL;}
  virtual Bool_t query(TCollection *aCol,HFilter &aFilter);
  virtual Bool_t query(TCollection *aCol,HLocation &aLoc);
  virtual Bool_t query(TCollection *aCol,HLocation &aLoc,HFilter &aFilter);
  virtual Bool_t filter(HFilter &aFilter);
  virtual Bool_t filter(HLocation &aLoc,HFilter &aFilter);
  virtual void Clear(Option_t *opt="");
  virtual Bool_t isSelfSplitable(void);
  virtual TIterator *MakeIterator(Option_t *opt="catIter",
                                  Bool_t dir=kIterForward);
  TIterator *MakeReverseIterator(void);
  Bool_t IsFolder(void) const;
  virtual Int_t getIndex(HLocation& loc) {return -1;}
  virtual Int_t getIndex(TObject* pObj) {return -1;}
  virtual Int_t getEntries() {return 0;}
  virtual void sort(void) {};
  ClassDef(HCategory,2) //Generic data category
};

R__EXTERN TObject *gNullObject;

#endif /* !HCATEGORY */
