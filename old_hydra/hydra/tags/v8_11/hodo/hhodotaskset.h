#ifndef HHODOTASKSET_H
#define HHODOTASKSET_H

#include "htaskset.h"

class HHodoTaskSet : public HTaskSet {
public:
  HHodoTaskSet(void) : HTaskSet() {}
  HHodoTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {}
  ~HHodoTaskSet(void) {}
  HTask* make(const char* select="", Option_t* option="");
  ClassDef(HHodoTaskSet,1) // Set of tasks for the Pion Hodoscopes
};

#endif /* !HHODOTASKSET_H */
