#ifndef HTOFHITFSIM_H
#define HTOFHITFSIM_H

#include "htofhitf.h"

class HTofHit;
class HTofRaw;
class HCategory;
class HIterator;

class HTofHitFSim : public HTofHitF {

public:
  HTofHitFSim(void) { }
  HTofHitFSim(Text_t* name,Text_t* title) : HTofHitF(name,title) { }
  ~HTofHitFSim(void);
  Bool_t init();
  Int_t execute();  
  ClassDef(HTofHitF,0) // Tof hit finder for simulated data.
};

#endif /* !HTOFHITFSIM_H */

