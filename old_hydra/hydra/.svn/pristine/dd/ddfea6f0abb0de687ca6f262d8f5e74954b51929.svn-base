#ifndef HSTARTTASKSET_H
#define HSTARTTASKSET_H

#include "htaskset.h"

class HStartTaskSet : public HTaskSet {

public:
  HStartTaskSet(void);
  HStartTaskSet(const Text_t name[],const Text_t title[]);
  ~HStartTaskSet(void);

 // function to be used for derived task sets (HRichTaskSet, etc.)
  HTask *make(const Char_t *select="",const Option_t *option="");

  ClassDef(HStartTaskSet,1) // Set of tasks
};

#endif /* !HSTARTTASKSET_H */
