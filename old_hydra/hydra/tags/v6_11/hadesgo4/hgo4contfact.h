#ifndef HGO4CONTFACT_H
#define HGo4CONTFACT_H

#include "hcontfact.h"

class HParSet;
class HContainer;

class HGo4ContFact : public HContFact {
private:
  void setAllContainers();
public:
  HGo4ContFact();
  ~HGo4ContFact() {}
  HParSet* createContainer(HContainer*); 
  ClassDef(HGo4ContFact,0) // Factory for all GO4 parameter containers
};

#endif  /* !HGO4CONTFACT_H */
