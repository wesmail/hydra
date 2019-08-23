#ifndef HShowerGeometry_H
#define HShowerGeometry_H
#pragma interface

#include "hgeomvector2.h"
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
  HShowerGeometry(const char* name="ShowerGeometry",
                  const char* title="Geometry parameters for Shower",
                  const char* context="");
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
  void getLabCoord(HLocation& loc, HGeomVector2& v);
  void getSphereCoord(HLocation& loc, HGeomVector2& v);

  const HGeomTransform&  getTransform(Int_t sect);
  const HGeomTransform&  getTransform(Int_t sect, Int_t mod);

  void translModules();
  void rotateSectors();
  Bool_t transToLab();
  Bool_t transPadToLab();
  
  ClassDef(HShowerGeometry,1) // ROOT extension

public:
  Int_t m_nSectors;           //number of sectors  
  Int_t m_nModules;           //number of modules
  Int_t m_nRows;              //number of rows
  Int_t m_nColumns;           //number of columns
  Int_t m_nLab;		      //determinate if lab coordinates have been calculated				

  HShowerSectorGeom sectors[6]; //definitions of coordinates in lab

  HShowerFrame frames[3];   //definitions of local coordinates of frames
  HShowerWireTab wires[3];  //definitions of local coordinates of wires
  HShowerPadTab pads[3];    //definitions of local coordinates of pads
};

#endif
