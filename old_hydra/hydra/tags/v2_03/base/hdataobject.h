#ifndef HDATAOBJECT_H
#define HDATAOBJECT_H

#include <TObject.h>
#include "hdataobjid.h"

class HCategory;

class HDataObject : public TObject
{
protected:
  HLocation *fLocation; //! Location of this object in the event
public:
  HDataObject(void); 
  ~HDataObject(void);
  void setLocation(HLocation &aLoc);
  HDataObjId *getId(void);
  HLocation *&getLocation(void);
  virtual Cat_t getCategory(void);
  Bool_t HasIdentifier(void);

  ClassDef(HDataObject,1) //Base class for event data objects
};

EXTERN TObject *gNullObject;

#endif /* !HDATAOBJECT */
