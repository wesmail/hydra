// File: showerunpacker.h
//
// Author: Leszek Kidon
// Last update: 25/5/2000
//

#ifndef HShowerUnpacker_H
#define HShowerUnpacker_H

#include "haddef.h"
#include "hldsubevt.h"
#include "hldunpack.h"
#include "hlocation.h"
#include "showerdef.h"

#include "hshowerunpackparam.h"
//default unpacker 

class HShowerRawHist;

class HShowerUnpacker: public HldUnpack {
public:
  HShowerUnpacker(Int_t nSubId);
  ~HShowerUnpacker();

  virtual int getSubEvtId() const { return m_nSubId; }

  Int_t execute();
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);

private:
  Int_t getChargeHigh(UInt_t d) {return (d>>8) & 0xFF;} 
  Int_t getChargeLow(UInt_t d) {return d & 0xFF;}
  Int_t getSector(UInt_t d) {return m_nSector;}    //not defined 
  Int_t getModule(UInt_t d) {return ( (d & 0x1FF) >> 2) %3; }

  Int_t getColumn(UInt_t d) { return ( (d & 0x1FF)>>2) / 3;}

  Int_t getRowHigh(UInt_t d) { return (d & 0x03) + (d>>7 & 0x18)+4; }
  Int_t getRowLow(UInt_t d) {  return (d & 0x03) + (d>>7 & 0x18);}

  virtual Int_t getRowOffset(UInt_t d) 
        { return m_nRowOffset + (d>>7) & 0x18; }
/*
  virtual Int_t getRowOffset(UInt_t d) 
        { return m_nRowOffset + m_nSwap*((d>>7) & 0x18); }
*/
  Int_t frameIdentifier(UInt_t d) { return (d>>14) &0x03;}
  Int_t dataStatus(UInt_t d) { return (d>>12) & 0x01;}

  Int_t fillData(class HLocation &loc, Int_t nCharge, Int_t nEvtId);
  void setParams(HShowerUnpackParam* pParams) {m_pParams = pParams;}
  void configure(HShowerUnpackParam* pParams);

  HShowerUnpackParam* getParams() {return m_pParams;}
  int dump();

  HLocation m_loc; //!Location of object to be unpacked.
  HLocation m_zeroLoc; //!
  int nEvNr;

  Int_t m_nSubId;
  Int_t m_nSector;
  Int_t m_nRowOffset;
  Int_t m_nSwap;

  HShowerUnpackParam* m_pParams;
  HShowerRawHist* m_pHist;
  ClassDef(HShowerUnpacker,0)		// unpack Shower data
};

#endif /* !HShowerUnpacker_H */



