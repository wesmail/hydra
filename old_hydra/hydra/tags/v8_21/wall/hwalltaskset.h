#ifndef HWALLTASKSET_H
#define HWALLTASKSET_H

#include "htaskset.h"

class HWallTaskSet : public HTaskSet {
public:
  HWallTaskSet(void) : HTaskSet() {}
  HWallTaskSet(const Text_t name[],const Text_t title[]) : HTaskSet(name,title) {}
  ~HWallTaskSet(void) {}
  HTask* make(const Char_t* select="",const Option_t* option="");
  ClassDef(HWallTaskSet,1) // Set of tasks for the Forward Wall
};

#endif /* !HWALLTASKSET_H */
