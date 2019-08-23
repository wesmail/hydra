#ifndef HSTARTUNPACKER_H
#define HSTARTUNPACKER_H

#include "hldunpack.h"
#include "hlocation.h"

class HIterator;
class HCategory;
class HStartLookup;

class HStartUnpacker: public HldUnpack {
friend class  HStartMonitor;
protected:
  Int_t subEvtId;           //! subevent id
  HLocation loc;
  HIterator* iter;
  HCategory* tboxCat;       //!
  HStartLookup* lookup;
  Int_t scalData[32];       //!

  void clearScalData(void) {
    for(Int_t i=0;i<32;i++) scalData[i]=0;
  }
public:
  HStartUnpacker(Int_t id=412);
  ~HStartUnpacker(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute();
  Bool_t init(void);
  ClassDef(HStartUnpacker,0)         // unpack START data
};          

#endif /* !HSTARTUNPACKER_H */











