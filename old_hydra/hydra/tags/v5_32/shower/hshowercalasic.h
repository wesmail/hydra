#ifndef HShowerCalAsic_H
#define HShowerCalAsic_H

#include "TNamed.h"
#include "TList.h"


#include "hparset.h"
#include "hshowerparset.h"
#include "hshowerasic.h"

class HShowerCalAsic : public HShowerParSet {
public:
  HShowerCalAsic();
  ~HShowerCalAsic();

  HShowerAsic* getAsic(Int_t nAsicNr);
  Bool_t addAsic(Int_t nAsicNr, HShowerAsic* pAsic);
  Bool_t addAsic(HShowerAsic* pAsic);
 
  Float_t getOffset(Int_t nAsicNr, Int_t nIdx);
  Float_t getSlope(Int_t nAsicNr, Int_t nIdx);
  void setOffset(Int_t nAsicNr, Int_t nIdx, Float_t fOffset);
  void setSlope(Int_t nAsicNr, Int_t nIdx, Float_t fSlope);

  virtual Bool_t initAscii(HParHadAsciiFileIo*);
  virtual Bool_t writeAscii(HParHadAsciiFileIo*);

  ClassDef(HShowerCalAsic,1) //ROOT extension

private:
  TList*  m_pList;
};

#endif
