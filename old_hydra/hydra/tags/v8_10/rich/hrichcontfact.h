#ifndef HRICHCONTFACT_H
#define HRICHCONTFACT_H

#include "hcontfact.h"


class HParSet;
class HContainer;

class HRichContFact : public HContFact {
private:
  void setAllContainers();
public:
  HRichContFact();
  ~HRichContFact() {}
  HParSet* createContainer(HContainer*);
  ClassDef(HRichContFact,0) // Factory for parameter containers in libRich
};

#endif  /* !HRICHCONTFACT_H */
