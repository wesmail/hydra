#pragma interface
#ifndef HBRANCHOBJECT_H
#define HBRANCHOBJECT_H

#include <TBranchObject.h>

class HBranchObject : public TBranchObject {
public:
  HBranchObject(void);
  HBranchObject(const Text_t *name,const Text_t *classname, void* addobj,Int_t basketsize,Int_t splitlevel=0,Int_t compress=-1);
  ~HBranchObject(void);
  virtual void SetAddress(void *add);
  ClassDef(HBranchObject,1) //Modified TBranchObject
};

#endif /* !HBRANCHOBJECT */
