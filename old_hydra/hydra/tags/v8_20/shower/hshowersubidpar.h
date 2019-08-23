#ifndef HShowerSubIdPar_H
#define HShowerSubIdPar_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"

class HParHadAsciiFileIo;

class HShowerSubIdPar : public HShowerParSet {
public:
  HShowerSubIdPar();  //only for Streamer function

public:
  HShowerSubIdPar(Int_t nSubId);

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t defaultInit(Int_t subId);

  void setSubID(Int_t nSubId){m_nSubId = nSubId;}
  void setPartOfSector(Int_t nPartOfSector) {m_nPartOfSector = nPartOfSector;}
  void setSector(Int_t nSector){m_nSector = nSector;}
  void setSwap(Int_t nSwap){m_nSwap = nSwap;}
  Int_t getSubID(){return m_nSubId;}
  Int_t getSector(){return m_nSector;}
  Int_t getPartOfSector(){return m_nPartOfSector;}
  Int_t getSwap(){return m_nSwap;}

  ClassDef(HShowerSubIdPar,1) //ROOT extension
private:
  Int_t m_nSubId;         //sub event id
  Int_t m_nSector;       //sector number 
  Int_t m_nPartOfSector; //0 = 0-15 rows; 1 = 16-31 rows
  Int_t m_nSwap;         //swap plugs flag 
};

#endif
