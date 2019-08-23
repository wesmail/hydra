#ifndef HRPCTASKSET_H
#define HRPCTASKSET_H

#include "htaskset.h"

class HRpcTaskSet : public HTaskSet {

public:
  HRpcTaskSet(void);
  HRpcTaskSet(Text_t name[],Text_t title[]);
  ~HRpcTaskSet(void);

 // function to be used for derived task sets (HRpcTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HRpcTaskSet,1) // Set of tasks
};

#endif /* !HRPCTASKSET_H */
