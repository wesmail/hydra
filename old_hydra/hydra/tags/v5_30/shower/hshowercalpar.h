#ifndef HShowerCalPar_H
#define HShowerCalPar_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "hshowercalparcell.h"

class HParHadAsciiFileIo;
class HShowerCalAsic;
class HShowerLookupAsic;

class HShowerCalPar : public HShowerParSet {
public:
  HShowerCalPar();
  ~HShowerCalPar();

  void setSetup(Int_t nSectors, Int_t nModules, Int_t nRows, Int_t nColumns); 
  Int_t getNSectors() {return m_nSectors;}
  Int_t getNModules() {return m_nModules;}
  Int_t getNRows() {return m_nRows;}
  Int_t getNColumns() {return m_nColumns;}
   

  void setCellClassName(Char_t* pszName);
  Char_t* getCellClassName();

  HShowerCalParCell*& getSlot(HLocation &loc);
  HShowerCalParCell* getObject(HLocation &loc);
 
  Float_t getOffset(HLocation &loc);
  Float_t getSlope(HLocation &loc);
  Int_t getAsicNr(HLocation &loc);
  void setOffset(HLocation &loc, Float_t fOffset);
  void setSlope(HLocation &loc, Float_t fSlope);
  void setAsicNr(HLocation &loc, Int_t nAsicNr);

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
 
  void setAsicTables(HShowerLookupAsic *pLA, HShowerCalAsic *pCA);
  virtual Bool_t defaultInit();
  void takeDataFromAsic();
  void propagateDataToAsic();
  void Print();

  ClassDef(HShowerCalPar,1) //ROOT extension

private:
  Int_t m_nSectors;    //sizes of calibration structure
  Int_t m_nModules;
  Int_t m_nRows;
  Int_t m_nColumns;

  Char_t m_szClassName[40]; //name of class used for keeping data for 
                            //calibration of one pad 

  HObjTable m_ParamsTable;  //structure which contains all object 
                            //for calibration of one pad

  HShowerLookupAsic* m_pLookupAsic; //!reference to HShowerLookupAsic structure
  HShowerCalAsic* m_pCalAsic; //!reference to HShowerCalAsic structure

};

#endif
