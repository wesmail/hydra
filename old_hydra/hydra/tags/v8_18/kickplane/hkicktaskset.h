#ifndef HKICKTASKSET_H
#define HKICKTASKSET_H

#include "htaskset.h"

class HKickTaskSet : public HTaskSet {

public:
  HKickTaskSet(void);
  HKickTaskSet(Text_t name[],Text_t title[]);
  ~HKickTaskSet(void);

 // function to be used for derived task sets (HKickTaskSet, etc.)
  HTask *make(const Char_t *select="", Option_t *option="");

  ClassDef(HKickTaskSet,1) // Set of tasks
};

#endif /* !HKICKTASKSET_H */
