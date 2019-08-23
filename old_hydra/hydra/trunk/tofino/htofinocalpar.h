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
  HTofinoCalPar(const Char_t* name="TofinoCalPar",
                const Char_t* title="Calibration parameters for Tofino",
                const Char_t* context="TofinoStandardCalib");
  ~HTofinoCalPar();

  void setSetup(Int_t nSectors, Int_t nCells);

  void setCellClassName(const Char_t* pszName);
  Char_t* getCellClassName();

  HTofinoCalParCell*& getSlot(HLocation &loc);
  HTofinoCalParCell* getObject(HLocation &loc);
 
  Float_t getOffset(HLocation &loc);
  Float_t getSlope(HLocation &loc);
  Float_t getDeltaSlope(HLocation &loc);
  Float_t getDeltaOffset(HLocation &loc);
  Float_t getCFDThreshold(HLocation &loc);
  Float_t getADCThreshold(HLocation &loc);
  Float_t getADCPedestal(HLocation &loc);
  void getElossParams(HLocation &loci, Float_t *eloss);
  Float_t getCoordPosition(HLocation &loc);
  Float_t getMinLength(HLocation &loc);
  Int_t  getSectors(){return m_nSectors;}
  Int_t  getCells(){return m_nCells;}
  void setOffset(HLocation &loc, Float_t fOffset);
  void setSlope(HLocation &loc, Float_t fSlope);
  void setDeltaSlope(HLocation &loc, Float_t fDeltaSlope);
  void setDeltaOffset(HLocation &loc, Float_t fDeltaOffset);
  void setCFDThreshold(HLocation &loc, Float_t fCFDThreshold);
  void setADCThreshold(HLocation &loc, Float_t fADCThreshold);
  void setADCPedestal(HLocation &loc, Float_t fADCPedestal);
  void setElossParams(HLocation &loc, Float_t *fElossParams);
  void setCoordPosition(HLocation &loc, Float_t fCoordPos);
  void setMinLength(HLocation &loc, Float_t fLen);

  Float_t calcDriftTime(Int_t nSector, Int_t nCell, Float_t fY);
  Float_t calcLightPropTime(Int_t nSector, Int_t nCell, Float_t fY);
  Float_t calcLightPropTimeFull(Int_t nSector, Int_t nCell, Float_t fY);
  Float_t calcDeltaLightPropTime(Int_t nSector, Int_t nCell, Float_t fTime);
  Float_t channelToTime(Int_t nSector, Int_t nCell, Float_t fTime);
  Float_t timeToChannel(Int_t nSector, Int_t nCell, Float_t fTime);
  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
 
  virtual Bool_t defaultInit();
  void Print();

  ClassDef(HTofinoCalPar,1) //Tofino calibration parameter container

private:
  Int_t m_nSectors;         //sizes of calibration structure
  Int_t m_nCells;

  Char_t m_szClassName[40]; //name of class used for keeping data for 
                            //calibration of one cell 

  HObjTable m_ParamsTable;  //structure which contains all object 
                            //for calibration of one cell
  HLocation fLoc;           //!
};

#endif




