#ifndef HHYPCONTFACT_H
#define HHYPCONTFACT_H

#include "hcontfact.h"

class           HParSet;

//class             HContainer;

class           HHypContFact:public HContFact
{
private:
  void            setAllContainers();
public:
                  HHypContFact();
                 ~HHypContFact()
  {
  }
  HParSet        *createContainer(HContainer *);

  ClassDef(HHypContFact, 0)     // Factory for all Hyp parameter containers
};

#endif /* !HHYPCONTFACT_H */
