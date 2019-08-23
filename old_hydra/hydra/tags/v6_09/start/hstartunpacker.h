#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HCategory;
class HStartLookup;

class HStartUnpacker: public HldUnpack {
friend class  HStartMonitor;
protected:
  Int_t subEvtId;              //! subevent id
  HLocation loc;
  HStartLookup* lookup;
public:
  HStartUnpacker(Int_t id=412);
  ~HStartUnpacker(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute(); 
  Bool_t init(void);
  ClassDef(HStartUnpacker,0)         // unpack START data
};          

#endif /* !HSTARTUNPACKER_H */











