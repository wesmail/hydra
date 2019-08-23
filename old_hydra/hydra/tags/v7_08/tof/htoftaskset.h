#ifndef HTOFTASKSET_H
#define HTOFTASKSET_H

#include "htaskset.h"

class HTofTaskSet : public HTaskSet {

public:
  HTofTaskSet(void);
  HTofTaskSet(Text_t name[],Text_t title[]);
  ~HTofTaskSet(void);

 // function to be used for derived task sets (HTofTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HTofTaskSet,1) // Set of tasks
};

#endif /* !HTOFTASKSET_H */
