#ifndef HTOFINOTASKSET_H
#define HTOFINOTASKSET_H

#include "htaskset.h"

class HTofinoTaskSet : public HTaskSet {

public:
  HTofinoTaskSet(void);
  HTofinoTaskSet(const Text_t name[],const Text_t title[]);
  ~HTofinoTaskSet(void);

 // function to be used for derived task sets (HTofinoTaskSet, etc.)
 HTask *make(const Char_t *select="",const Option_t *option="");

  ClassDef(HTofinoTaskSet,1) // Set of tasks
};

#endif /* !HTOFINOTASKSET_H */
