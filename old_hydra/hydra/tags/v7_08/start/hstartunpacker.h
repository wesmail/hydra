#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HStartLookup;

class HStartUnpacker: public HldUnpack {
  friend class  HStartMonitor;
  protected:
    Int_t subEvtId;        //! subevent id
    HLocation loc;         //! location in the matrix category for raw data
    HStartLookup* lookup;  //! lookup table for start unpacker
  public:
    HStartUnpacker(Int_t id=416);
    ~HStartUnpacker(void) {}
    Int_t getSubEvtId() const { return subEvtId; }
    Bool_t init(void);
    Int_t execute();

    ClassDef(HStartUnpacker,0) // Unpacker for START data
};

#endif /* !HSTARTUNPACKER_H */











