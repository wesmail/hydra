#ifndef HTRIGGERCONTFACT_H
#define HTRIGGERCONTFACT_H

#include "hcontfact.h"

class HParSet;
class HContainer;

class HTriggerContFact : public HContFact {
private:
  void setAllContainers();
public:
  HTriggerContFact();
  ~HTriggerContFact() {}
  HParSet* createContainer(HContainer*);
  ClassDef(HTriggerContFact,0) // Factory for parameter containers in libTrigger
};

#endif  /* !HTRIGGERCONTFACT_H */
