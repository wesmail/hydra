#ifndef HSHOWERTASKSET_H
#define HSHOWERTASKSET_H

#include "htaskset.h"

class HShowerTaskSet : public HTaskSet {

public:
  HShowerTaskSet(void);
  HShowerTaskSet(Text_t name[],Text_t title[]);
  ~HShowerTaskSet(void);

 // function to be used for derived task sets (HShowerTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HShowerTaskSet,1) // Set of tasks
};

#endif /* !HSHOWERTASKSET_H */
