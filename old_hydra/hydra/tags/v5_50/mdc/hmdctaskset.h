#ifndef HMDCTASKSET_H
#define HMDCTASKSET_H

#include "htaskset.h"

class HMdcTaskSet : public HTaskSet {

public:
  HMdcTaskSet(void);
  HMdcTaskSet(Text_t name[],Text_t title[]);
  ~HMdcTaskSet(void);

 // function to be used for derived task sets (HMdcTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HMdcTaskSet,1) // Set of tasks
};

#endif /* !HMDCTASKSET_H */
