//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005

#ifndef HRICHIPUTASKSET_H
#define HRICHIPUTASKSET_H

#include "htaskset.h"

class HRichIPUTaskSet : public HTaskSet
{
public:
  HRichIPUTaskSet(void);
  HRichIPUTaskSet(Text_t name[],Text_t title[]);
  ~HRichIPUTaskSet(void);

  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HRichIPUTaskSet,1) // Set of tasks for RICH IPU
};

#endif /* !HRICHIPUTASKSET_H */
