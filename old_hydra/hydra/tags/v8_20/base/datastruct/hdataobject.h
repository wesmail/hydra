#ifndef HDATAOBJECT_H
#define HDATAOBJECT_H

#if !defined(__CINT__) || defined(__MAKECINT__) 
#include "TObject.h"
#include "hdataobjid.h"
#endif

class HCategory;

class HDataObject : public TObject
{
protected:
  HLocation *fLocation; //! Location of this object in the event
public:
  HDataObject(void) : fLocation(0) {}
  ~HDataObject(void) { if(fLocation) delete fLocation;}
  void setLocation(HLocation &aLoc);
  HDataObjId *getId(void);
  HLocation *&getLocation(void);
  virtual Cat_t getCategory(void);
  Bool_t HasIdentifier(void);

  ClassDef(HDataObject,1) //Base class for event data objects
};

#endif /* !HDATAOBJECT */
