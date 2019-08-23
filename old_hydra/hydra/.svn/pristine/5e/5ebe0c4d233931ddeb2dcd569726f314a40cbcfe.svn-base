#ifndef HRICHGEOMETRY_H
#define HRICHGEOMETRY_H
#pragma interface

#include "hparset.h"
#include "hrichparset.h"
#include "hlocation.h"
#include "hrichpad.h"
#include "hrichwire.h"
#include "hrichframe.h"
#include "hrichsectorpar.h"
// #include "hrichmodulegeom.h"

class HParHadAsciiFileIo;

class HRichGeometry : public HRichParSet {

public:

  HRichGeometry();
  ~HRichGeometry();

  Int_t getSectorsNr();
  Int_t getSector(Int_t sec);
  Int_t getColumns(); // pads in X
  Int_t getRows(); // pads in Y
  Int_t getPadsNr();
  Int_t getPadStatus(Int_t sec, Int_t padnr);

  void setLab(Int_t nLab = 1) { fLab = nLab; }
  Bool_t IsLab() { return fLab; }


  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual void clear();


  HRichFrame* getFramePar() { return &fFrame; }
  HRichWireTab* getWiresPar() { return &fWires; }
  HRichPadTab* getPadsPar() { return &fPads; }

  void getLocalCoord(HLocation& loc, HGeomVector& v);
  void getLabCoord(HLocation& loc, HGeomVector& v);

  const HGeomTransform&  getTransform(Int_t sec);

  // void translModules();
  void rotateSectors();
  Bool_t transToLab();
 

  ClassDef(HRichGeometry,1) //ROOT extension

public:

  Int_t fSectorsNr;
  Int_t fSectorActive[6];
  Int_t fColumns;
  Int_t fRows;
  Int_t fLab;


  HRichSectorPar fSectors[6];
  HRichFrame fFrame;
  HRichWireTab fWires;
  HRichPadTab fPads;


};

#endif // HRICHGEOMETRY_H
