#ifndef HRICHTASKSET_H
#define HRICHTASKSET_H

#include "htaskset.h"

class HRichTaskSet : public HTaskSet {

public:
  HRichTaskSet(void);
  HRichTaskSet(Text_t name[],Text_t title[]);
  ~HRichTaskSet(void);

 // function to be used for derived task sets (HRichTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HRichTaskSet,1) // Set of tasks
};

#endif /* !HRICHTASKSET_H */
