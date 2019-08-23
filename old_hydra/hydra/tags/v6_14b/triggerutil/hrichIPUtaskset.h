#ifndef HRICHIPUTASKSET_H
#define HRICHIPUTASKSET_H

#include "htaskset.h"

class HRichIPUTaskSet : public HTaskSet {

public:
  HRichIPUTaskSet(void);
  HRichIPUTaskSet(Text_t name[],Text_t title[]);
  ~HRichIPUTaskSet(void);

 // function to be used for derived task sets (HRichIPUTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HRichIPUTaskSet,1) // Set of tasks
};

#endif /* !HRICHIPUTASKSET_H */
