#ifndef HLOCATEDDATAOBJECT_H
#define HLOCATEDDATAOBJECT_H

#include "hdataobject.h"

class HLocatedDataObject : public HDataObject {
 public:
  HLocatedDataObject(void) {}
  ~HLocatedDataObject(void) {}
  virtual Int_t getNLocationIndex(void)=0;
  virtual Int_t getLocationIndex(Int_t n)=0;
  virtual HLocation* getLocation() {return NULL;};
    
  ClassDef(HLocatedDataObject,1) //Data object with localization data
};

#endif /* !HLOCATEDDATAOBJECT_H */
