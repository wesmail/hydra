#ifndef HTofinoShowerMap_H
#define HTofinoShowerMap_H
#pragma interface

#include "htofinoparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "htofinoshowermapcell.h"

class HParHadAsciiFileIo;

class HTofinoShowerMap : public HTofinoParSet {
public:
  HTofinoShowerMap(const char* name="TofinoShowerMap",
                   const char* title="Position of shower pads relative to tofino cells",
                   const char* context="");
  ~HTofinoShowerMap();

  void setSetup(Int_t nRows, Int_t nColumns);

  void setCellClassName(Char_t* pszName);
  Char_t* getCellClassName();

  HTofinoShowerMapCell*& getSlot(HLocation &loc);
  HTofinoShowerMapCell* getObject(HLocation &loc);
 
  Int_t getCellNumber(HLocation &loc);
  Int_t getCellNumber(Int_t nRow, Int_t nColumn);
  void setCellNumber(HLocation &loc, Int_t nCell);

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
 
  virtual Bool_t defaultInit();
  void Print();

  ClassDef(HTofinoShowerMap,1) //Tofino/Shower mapping parameter container

private:
  Int_t m_nRows;            //sizes of calibration structure
  Int_t m_nColumns;

  Char_t m_szClassName[40]; //name of class used for keeping data for 
                            //calibration of one cell

  HObjTable m_ParamsTable;  //structure which contains all object 
                            //for calibration of one cell
  HLocation fLoc; //!

};

#endif
