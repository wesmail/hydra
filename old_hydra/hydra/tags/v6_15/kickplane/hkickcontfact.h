#ifndef HKICKCONTFACT_H
#define HKICKCONTFACT_H

#include "hcontfact.h"

class HParSet;
class HContainer;

class HKickContFact : public HContFact {
private:
  void setAllContainers();
public:
  HKickContFact();
  ~HKickContFact() {}
  HParSet* createContainer(HContainer*);
  ClassDef(HKickContFact,0) // Factory for all Kickplane parameter containers
};

#endif  /* !HKICKCONTFACT_H */
