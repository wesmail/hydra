#ifndef HTRIGGERTASKSET_H
#define HTRIGGERTASKSET_H

#include "htaskset.h"

class HTriggerTaskSet : public HTaskSet {

public:
  HTriggerTaskSet(void);
  HTriggerTaskSet(Text_t name[],Text_t title[]);
  ~HTriggerTaskSet(void);

 // function to be used for derived task sets (HRichIPUTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HTriggerTaskSet,1) // Set of tasks
};

#endif /* !HTRIGGERTASKSET_H */
