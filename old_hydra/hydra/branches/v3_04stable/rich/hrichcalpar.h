#ifndef HRICHCALPAR_H
#define HRICHCALPAR_H


#include "hparset.h"
#include "hrichparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "hrichcalparcell.h"
// #include "hrichreadparam.h"

class HParHadAsciiFileIo;

class HRichCalPar : public HRichParSet {

public:

  HRichCalPar();
  ~HRichCalPar();

  void setSetup(Short_t nSectors, Short_t nRows, Short_t nColumns); 

  //void setReadParam(HRichReadParam *pReadParam);
  //HRichReadParam* getReadParam();
  void setCellClassName(Char_t* pszName);
  Char_t* getCellClassName();

  HRichCalParCell* getSlot(HLocation &loc);
  HRichCalParCell* getObject(HLocation &loc);
 
  Float_t getOffset(HLocation &loc);
  Float_t getSlope(HLocation &loc);
  void setOffset(HLocation &loc, Float_t fOffset);
  void setSlope(HLocation &loc, Float_t fSlope);

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t defaultInit();

  void PrintCalPar();

  ClassDef(HRichCalPar,1) //ROOT extension

private:
  Int_t fSectors;
  Int_t fRows;
  Int_t fColumns;

  Char_t m_szClassName[40];

  HObjTable m_ParamsTable;
  // HRichReadParam *m_pReadParam;
};

#endif // HRICHCALPAR_H
