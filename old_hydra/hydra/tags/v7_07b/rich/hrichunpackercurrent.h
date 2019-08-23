#ifndef HRICHUNPACKERCURRENT_H
#define HRICHUNPACKERCURRENT_H

#include "hldunpack.h"
#define DEBUG  0x3f //in scaler format jan04
#define RICH_ADC_CRATE 4
#define RICH_ADC_SLOT  13
#define RICH_ADC_MIN   0
#define RICH_ADC_MAX   5

class HIterator;
class HCategory;

class HRichUnpackerCurrent: public HldUnpack {
private:
  Int_t startEvt;           //!
  Int_t iEventNr;             //!
  Int_t subEvtId;           //! subevent id
  HCategory* m_pHitHdrCat;     //! Rich header
  HIterator* fIterHitHeader; //!

public:
  HRichUnpackerCurrent(Int_t id = 416, Int_t strtEvt = 0);
  ~HRichUnpackerCurrent(void) {}
  Int_t getSubEvtId() const { return subEvtId; }
  Int_t execute();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  ClassDef(HRichUnpackerCurrent,1)         // unpack RICH current data
};

#endif /* !HRICHUNPACKERCURRENT_H */
