#ifndef HTOFINOTASKSET_H
#define HTOFINOTASKSET_H

#include "htaskset.h"

class HTofinoTaskSet : public HTaskSet {

public:
  HTofinoTaskSet(void);
  HTofinoTaskSet(Text_t name[],Text_t title[]);
  ~HTofinoTaskSet(void);

 // function to be used for derived task sets (HTofinoTaskSet, etc.)
 HTask *make(const char *select="", Option_t *option="");

  ClassDef(HTofinoTaskSet,1) // Set of tasks
};

#endif /* !HTOFINOTASKSET_H */
