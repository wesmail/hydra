#ifndef HTofinoCalPar_H
#define HTofinoCalPar_H
#pragma interface

#include "htofinoparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "htofinocalparcell.h"

class HParHadAsciiFileIo;

class HTofinoCalPar : public HTofinoParSet {
public:
  HTofinoCalPar();
  ~HTofinoCalPar();

  void setSetup(Int_t nSectors, Int_t nCells);

  void setCellClassName(Char_t* pszName);
  Char_t* getCellClassName();

  HTofinoCalParCell*& getSlot(HLocation &loc);
  HTofinoCalParCell* getObject(HLocation &loc);
 
  Float_t getOffset(HLocation &loc);
  Float_t getSlope(HLocation &loc);
  Float_t getDeltaSlope(HLocation &loc);
  Float_t getDeltaOffset(HLocation &loc);
  Float_t getCoordPosition(HLocation &loc);
  void setOffset(HLocation &loc, Float_t fOffset);
  void setSlope(HLocation &loc, Float_t fSlope);
  void setDeltaSlope(HLocation &loc, Float_t fDeltaSlope);
  void setDeltaOffset(HLocation &loc, Float_t fDeltaOffset);
  void setCoordPosition(HLocation &loc, Float_t fCoordPos);

  Float_t calcDriftTime(Int_t nSector, Int_t nCell, Float_t fY);
  Float_t calcLightPropTime(Int_t nSector, Int_t nCell, Float_t fY);
  Float_t calcDeltaLightPropTime(Int_t nSector, Int_t nCell, Float_t fTime);
  Float_t channelToTime(Int_t nSector, Int_t nCell, Float_t fTime);
  Float_t timeToChannel(Int_t nSector, Int_t nCell, Float_t fTime);
  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
 
  virtual Bool_t defaultInit();
  void Print();

  ClassDef(HTofinoCalPar,1) //ROOT extension

private:
  Int_t m_nSectors;    //sizes of calibration structure
  Int_t m_nCells;

  Char_t m_szClassName[40]; //name of class used for keeping data for 
                            //calibration of one pad 

  HObjTable m_ParamsTable;  //structure which contains all object 
                            //for calibration of one pad
  HLocation fLoc;           //!
};

#endif

