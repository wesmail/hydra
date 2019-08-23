#ifndef HTOFINOUNPACKER_H
#define HTOFINOUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HTofinoLookup;

class HTofinoUnpacker: public HldUnpack {
protected:
  Int_t subEvtId;              //! subevent id
  HLocation loc;
  HTofinoLookup* lookup;
public:
  HTofinoUnpacker(Int_t id=412);
  ~HTofinoUnpacker(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute();
  Bool_t init(void);
  ClassDef(HTofinoUnpacker,0)         // unpack TOFINO data
};

#endif /* !HTOFINOUNPACKER_H */
