#ifndef HHODOCONTFACT_H
#define HHODOCONTFACT_H

#include "hcontfact.h"

class HParSet;
class HContainer;

class HHodoContFact : public HContFact {
private:
  void setAllContainers(void);
public:
  HHodoContFact(void);
  ~HHodoContFact(void) {}
  HParSet* createContainer(HContainer*);
  ClassDef(HHodoContFact,0) // Factory for parameter containers in libHodo
};

#endif  /* !HHODOCONTFACT_H */
