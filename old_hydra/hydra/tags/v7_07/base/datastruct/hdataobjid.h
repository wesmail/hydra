#ifndef HDATAOBJID_H
#define HDATAOBJID_H

#include <TObject.h>
#include "haddef.h"
#include "hgeantdef.h"
#include "hlocation.h"

class HDataObjId : public TObject {
protected:
  Cat_t fCategory; //The category the object belongs to
  HLocation fLocation; //Localization of the object in the event
public:
  HDataObjId(void);
  HDataObjId(Cat_t aCat,HLocation &aLoc);
  ~HDataObjId(void);
  void setCategory(Cat_t aCat) {fCategory=aCat;}
  void setLocation(HLocation &aLoc);
  Cat_t getCategory(void) {return fCategory; }
  HLocation &getLocation(void) { return fLocation; }
  // operator=
  // operator>
  ClassDef(HDataObjId,1) //Identifier for each data object
}; // Completar...

#endif /* !HDATAOBJID */
