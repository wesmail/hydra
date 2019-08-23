#ifndef HTOFINOCONTFACT_H
#define HTOFINOCONTFACT_H

#include "hcontfact.h"

class HParSet;
class HContainer;

class HTofinoContFact : public HContFact {
private:
  void setAllContainers();
public:
  HTofinoContFact();
  ~HTofinoContFact() {}
  HParSet* createContainer(HContainer*);
  ClassDef(HTofinoContFact,0) // Factory for parameter containers in libTofino
};

#endif  /* !HTOFINOCONTFACT_H */
