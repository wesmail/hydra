// $Id: hpaircontfact.h,v 1.1 2005-03-23 12:15:27 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-03-18 15:38:55
//
// ---------------------------------------------------------------------------
#ifndef HPAIRCONTFACT_H
#define HPAIRCONTFACT_H
// ---------------------------------------------------------------------------
#include <hcontfact.h>
// ---------------------------------------------------------------------------
class HContainer;
class HParSet;
// ---------------------------------------------------------------------------
class HPairContFact : public HContFact {
private:
  void setAllContainers();
public:
  HPairContFact();
  ~HPairContFact() {}
  HParSet* createContainer(HContainer*);
  ClassDef(HPairContFact,0) // Factory for parameter containers in libPairs
};

#endif  /* !HPAIRCONTFACT_H */
