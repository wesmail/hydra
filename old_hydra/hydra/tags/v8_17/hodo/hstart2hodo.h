//*-- Created 30.10.2007 by B. Spruck

#ifndef HSTART2HODO_H
#define HSTART2HODO_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;

class HStart2Hodo : public HReconstructor {
protected:
  HCategory *rawHodoCat;      //! pointer to the hit data
  HCategory *rawStartCat;     //! pointer to the hit data
  HIterator *iter;            //! iterator on hodo hit data.
  HLocation loc;              //! location for new hit object
  Int_t smod, hmod;           //! only this hodo module is used
 public:
  HStart2Hodo(void);
  HStart2Hodo(Text_t* name,Text_t* title);
  ~HStart2Hodo(void);
  void set_mod(Int_t sm,Int_t hm){smod=sm; hmod=hm;};
  Bool_t init(void);
  Bool_t finalize(void) { return kTRUE; }
  Int_t execute(void);
public:
  ClassDef(HStart2Hodo,0) // Hodo Hit2Start: Copy Hodo Hit data to Start Hit data
};

#endif /* !HSTART2HODO_H */
















