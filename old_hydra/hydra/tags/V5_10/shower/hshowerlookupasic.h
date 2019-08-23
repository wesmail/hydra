#ifndef HShowerLookupAsic_H
#define HShowerLookupAsic_H
//#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "hshowerlookupasictab.h"

class HParHadAsciiFileIo;

class HShowerLookupAsic : public HShowerParSet {
public:
  HShowerLookupAsic();
  ~HShowerLookupAsic();

  void setSetup(Int_t nSectors, Int_t nModules, Int_t nRows); 

  void setCellClassName(Char_t* pszName);
  Char_t* getCellClassName();

  HShowerLookupAsicTab* getSlot(HLocation &loc);
  HShowerLookupAsicTab* getObject(HLocation &loc);
 
  Int_t getAsicNr(HLocation &loc);
  void setAsicNr(HLocation &loc, Int_t nAsicNr);

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t defaultInit();

  void Print();

  ClassDef(HShowerLookupAsic,1) //ROOT extension

private:
  void allocateTables();

  Int_t m_nSectors;
  Int_t m_nModules;
  Int_t m_nRows;

  Char_t m_szClassName[40];

  HObjTable m_ParamsTable;
};

#endif
