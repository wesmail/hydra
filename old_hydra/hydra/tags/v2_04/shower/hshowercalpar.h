#ifndef HShowerCalPar_H
#define HShowerCalPar_H
//#pragma interface

#include "hparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "hshowercalparcell.h"
#include "hshowerreadparam.h"

class HShowerCalPar : public HParSet {
public:
  HShowerCalPar();
  ~HShowerCalPar();

  void setSetup(Int_t nSectors, Int_t nModules, Int_t nRows, Int_t nColumns); 

  void setReadParam(HShowerReadParam *pReadParam);
  HShowerReadParam* getReadParam();
  void setCellClassName(Char_t* pszName);
  Char_t* getCellClassName();

  HShowerCalParCell* getSlot(HLocation &loc);
  HShowerCalParCell* getObject(HLocation &loc);
 
  Float_t getOffset(HLocation &loc);
  Float_t getSlope(HLocation &loc);
  void setOffset(HLocation &loc, Float_t fOffset);
  void setSlope(HLocation &loc, Float_t fSlope);

  virtual Bool_t init();

  ClassDef(HShowerCalPar,1) //ROOT extension

private:
  Int_t m_nSectors;
  Int_t m_nModules;
  Int_t m_nRows;
  Int_t m_nColumns;

  Char_t m_szClassName[40];

  HObjTable m_ParamsTable;
  HShowerReadParam *m_pReadParam;
};

#endif
