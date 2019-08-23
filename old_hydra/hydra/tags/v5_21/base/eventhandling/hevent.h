#ifndef HEVENT_H
#define HEVENT_H


#include <TNamed.h>
#include <TBranch.h>
#include <TCollection.h>
#include "haddef.h"
#include "hgeantdef.h"
#include "calodef.h"


class HDataObject;
class HDataObjId;
class HLocation;
class HFilter;
class HCategory;
class HEventHeader;

class HEvent : public TObject
{
// Base class for the different event types.
protected:
  TString *fName; //-Event's name
  TString *fTitle; //-Event's title
public:
  HEvent(void);
  HEvent(const Text_t *aName,const Text_t *aTitle);
  virtual ~HEvent(void);
  virtual const Text_t  *GetName() const {return fName->Data();}
  virtual const Text_t  *GetTitle() const {return fTitle->Data();}
  virtual HCategory *getCategory(Cat_t aCat)=0;
  virtual Bool_t addCategory(Cat_t aCat,HCategory *cat,Option_t opt[])=0;
  virtual Bool_t removeCategory(Cat_t aCat)=0;
  virtual void makeBranch(TBranch *parent)=0;

  virtual void activateBranch(TTree *tree,Int_t splitLevel)=0;
  virtual void Clear(void)=0;
  virtual void clearAll(Int_t level)=0;
  virtual HEventHeader *getHeader(void) {return NULL;}
  ClassDef(HEvent,1) //Base class for the different kind of events
};

#endif /* !HEVENT_H */