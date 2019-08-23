#ifndef HShowerUnpackParam_H
#define HShowerUnpackParam_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hshowerhist.h"
#include "TList.h"

class HShowerCalPar;
class HParHadAsciiFileIo;
class HShowerSubIdPar;

class HShowerUnpackParam : public HShowerParSet {
public:
  HShowerUnpackParam(const char* name="ShowerUnpackParam",
                     const char* title="Parameter for Shower unpacker",
                     const char* context="");
  ~HShowerUnpackParam();

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t defaultInit();

  Int_t calcModule(UInt_t d) {return ( (d & 0x1FF) >> 2) %3; }
  Int_t calcColumn(UInt_t d) {return ( (d & 0x1FF)>>2) / 3;}
  Int_t calcRow(UInt_t d) {return (d & 0x03) + (d>>7 & 0x18);}
  
  Bool_t useThreshold(){return m_nUseThresholds;}
  Int_t getThreshold(Int_t nSect, Int_t nMod){return m_nThresholds[nSect*6 + nMod];}
  Int_t getColumnNumber(Int_t nByteNumber)
                     {return m_nColumnsNumbers[nByteNumber];}
  Int_t getRowNumber(Int_t nByteNumber)
                     {return m_nRowsNumbers[nByteNumber];}
  Int_t getModuleNumber(Int_t nByteNumber)
                     {return m_nModulesNumbers[nByteNumber];}

  HShowerSubIdPar* getSubIdPar(Int_t nSubId);

  ClassDef(HShowerUnpackParam,1) //ROOT extension
private:
  Bool_t m_nIsInit;
  Bool_t m_nUseThresholds;
  Int_t m_nThresholds[18];
  Int_t m_nColumnsNumbers[384];
  Int_t m_nRowsNumbers[384];
  Int_t m_nModulesNumbers[384];

  TObjArray* m_pSubIdArr; //
};

#endif
