#ifndef HShowerGeometry_H
#define HShowerGeometry_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"
#include "hshowersectorgeom.h"
#include "hshowermodulegeom.h"

class HParHadAsciiFileIo;

class HShowerGeometry : public HShowerParSet {
public:
  HShowerGeometry();
  ~HShowerGeometry();

  Int_t getSectors(){return m_nSectors;}
  Int_t getModules(){return m_nModules;}
  Int_t getRows(){return m_nRows;}
  Int_t getColumns(){return m_nColumns;}
  void setLab(Int_t nLab = 1){m_nLab = nLab;}
  Bool_t IsLab(){return m_nLab;}

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual void clear();

  HShowerFrame* getFrame(Int_t n){return &frames[n];} 
  HShowerWireTab* getWireTab(Int_t nMod){return &wires[nMod];}
  HShowerPadTab* getPadParam(Int_t nMod){return &pads[nMod];}
  void attachTables();

  void getLocalCoord(HLocation& loc, HGeomVector& v);
  void getLabCoord(HLocation& loc, HGeomVector& v);

  const HGeomTransform&  getTransform(Int_t sect);
  const HGeomTransform&  getTransform(Int_t sect, Int_t mod);

  void translModules();
  void rotateSectors();
  Bool_t transToLab();
  
  ClassDef(HShowerGeometry,1) //ROOT extension

public:
  Int_t m_nSectors;
  Int_t m_nModules;
  Int_t m_nRows;
  Int_t m_nColumns;
  Int_t m_nLab;

  HShowerSectorGeom sectors[6]; //max. 6 sectors
  HShowerFrame frames[3];
  HShowerWireTab wires[3];//max. 3 modules
  HShowerPadTab pads[3];
};

#endif
