//*-- Created 30.3.2006 by B. Spruck

#ifndef HHODOHIT2START_H
#define HHODOHIT2START_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;

class HHodoHit2Start : public HReconstructor {
protected:
  HCategory *hitHodoCat;      //! pointer to the hit data
  HCategory *hitStartCat;     //! pointer to the hit data
  HIterator *iter;            //! iterator on hodo hit data.
  HLocation loc;              //! location for new hit object
 public:
  HHodoHit2Start(void);
  HHodoHit2Start(Text_t* name,Text_t* title);
  ~HHodoHit2Start(void);
  Bool_t init(void);
  Bool_t finalize(void) { return kTRUE; }
  Int_t execute(void);
public:
  ClassDef(HHodoHit2Start,0) // Hit2Start: Copy Hodo Hit data to Start Hit data
};

#endif /* !HHODOHIT2START_H */
















