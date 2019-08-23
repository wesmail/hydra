#ifndef HRECOBJECT_H
#define HRECOBJECT_H

#include "hdataobject.h"
#include <TObjArray.h>
#include "haddef.h"
#include "hlocation.h"
#include "hdataobjid.h"

class HRecObject : public HDataObject
{
private:
  TObjArray fParts; //Identifiers of the objects used to build "this"
  Int_t fNParts; //Number of objects used to build "this"
public:
  HRecObject(void);
  ~HRecObject(void);
  HRecObject(HRecObject &aObj);
  void setNParts(Int_t aNParts);
  Int_t getNParts(void);
  void setPart(Int_t aPartId, Cat_t cat,HLocation &loc);
  HDataObjId *getPartId(Int_t aPartId);
  ClassDef(HRecObject,1) //Data object obtained from other data objects
};

#endif /* !HRECOBJECT_H */
