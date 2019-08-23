#ifndef HMDCTRACKSCONTFACT_H
#define HMDCTRACKSCONTFACT_H

#include "hcontfact.h"

class HParSet;
class HContainer;

class HMdcTrackSContFact : public HContFact {
private:
  void setAllContainers();
public:
  HMdcTrackSContFact();
  ~HMdcTrackSContFact() {}
  HParSet* createContainer(HContainer*);
  ClassDef(HMdcTrackSContFact,0) // Factory for all Santiago tracking parameter containers
};

#endif  /* !HMDCTRACKSCONTFACT_H */
