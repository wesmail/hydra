//*-- Author: Ilse Koenig
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggercontfact.h,v 1.2 2005-07-15 09:00:25 wuestenf Exp $

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
  ~HTriggerContFact() {};
  HParSet* createContainer(HContainer*);
  ClassDef(HTriggerContFact,0) // Factory for parameter containers in libTrigger
};

#endif  /* !HTRIGGERCONTFACT_H */
