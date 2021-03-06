#pragma interface
#ifndef HPARTIALEVENT_H
#define HPARTIALEVENT_H

#include "hevent.h"
#include <TTree.h>
#include <TBranch.h>
#include <TObjArray.h>

class HCategory;

class HPartialEvent : public HEvent
{
protected:
  Int_t fRecLevel; //Reconstruction level of the event
  Cat_t fBaseCategory; //Identifier for the first category stored in this partial event
  TObject *fHeader; //-Sub event's header.
  TObjArray *fCategories; //- Categories 
public:
  HPartialEvent(void);
  HPartialEvent(const Text_t *aName,const Text_t *aTitle,Cat_t aBaseCat);
  ~HPartialEvent(void);
  void makeBranch(TBranch *parent);
  void activateBranch(TTree *tree,Int_t splitLevel);
  void addCategory(Cat_t aCat,HCategory *category);
  TObjArray *getCategories(void);
  HCategory *getCategory(Cat_t aCat);
  Bool_t addCategory(Cat_t aCat,HCategory *cat,Option_t opt[]);
  Bool_t removeCategory(Cat_t aCat);
  void setRecLevel(Int_t aRecLevel);
  Int_t getRecLevel(void);
  void setSubHeader(TObject *header) {fHeader=header;}
  TObject *getSubHeader(void) {return fHeader;}
  virtual void Clear(void);
  void clearAll(Int_t level);
  Bool_t isEmpty(void) {return fCategories->IsEmpty();}
  Bool_t IsFolder(void) {return kFALSE;}
  void Browse(TBrowser *b);
  void Streamer(class TBuffer const& b);
  ClassDef(HPartialEvent,1) //Event data corresponding to each main detector
};

#endif /* !HPARTIALEVENT_H */
