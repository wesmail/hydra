#ifndef HTOFTASKSET_H
#define HTOFTASKSET_H

#include "htaskset.h"

class HTofTaskSet : public HTaskSet {

public:
  HTofTaskSet(void);
  HTofTaskSet(const Text_t name[],const Text_t title[]);
  ~HTofTaskSet(void);

 // function to be used for derived task sets (HTofTaskSet, etc.)
  HTask *make(const Char_t *select="",const Option_t *option="");

  ClassDef(HTofTaskSet,1) // Set of tasks
};

#endif /* !HTOFTASKSET_H */
