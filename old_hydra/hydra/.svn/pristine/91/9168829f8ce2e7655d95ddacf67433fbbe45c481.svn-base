#ifndef HMDCSIZESCELLS_H
#define HMDCSIZESCELLS_H

#include "TObject.h"
#include "TClonesArray.h"
#include "hmdcgeomobj.h"
#include "hgeomtransform.h"

class HMdcGetContainers;
class HMdcLayerGeomPar;
class HMdcGeomPar;
class HMdcDetector;
class HMdcTrap;
class HMdcGeomStruct;

class HMdcSizesCellsCell : public TObject {
  friend class HMdcSizesCells;
protected:
  HGeomVector wireSizes[2];      // wire (2-poins)
  Bool_t status;
public:
  HMdcSizesCellsCell():status(kFALSE) {;}
  ~HMdcSizesCellsCell() {;}
  const HGeomVector* getWirePoint(Int_t n){ return (n<0||n>1) ? 0:&wireSizes[n]; }
  void setStatus(Bool_t stat) { status=stat; }
  Bool_t getStatus() { return status; }  
  void clear();
  ClassDef(HMdcSizesCellsCell,0)
}; 

class HMdcSizesCellsLayer : public TObject {
  friend class HMdcSizesCells;
protected:
  Short_t sector;
  Short_t module;
  Short_t layer;
  Short_t nCells;
  Float_t halfCatDist;
  Float_t pitch;
  Float_t cosWireOr;
  Float_t sinWireOr;
  Float_t wireOffSet;    //= (CentralWireNr()-1.)*pitch
  HGeomTransform sysRSec;
  HMdcPrPlane layerPlane;   // in SECTOR coord.sys.! (!!!nap.drug.kl.!!!)
  HGeomTransform sysRMod;
  TClonesArray* array;  // array of pointers of type HMdcSizesCellsCell
public:
  HMdcSizesCellsLayer(Int_t sec, Int_t mod, Int_t lay);
  ~HMdcSizesCellsLayer();
  HMdcSizesCellsCell& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsCell*>((*array)[i]);
  }
  Int_t getSize();
  const HGeomTransform* getSecTrans() { return &sysRSec; }
  const HGeomTransform* getModTrans() { return &sysRMod; }
  void setLayerPlane() {layerPlane.setPrPlane(sysRSec);}
  const HMdcPrPlane* getLayerPlane() {return &layerPlane;}
  void calcIntersection( HGeomVector &p1, HGeomVector &p2, HGeomVector &out) {
    layerPlane.calcIntersection(p1,p2-p1,out);}
  void transSecToRotLayer(HGeomVector& point);
  Float_t getHalfCatDist() {return halfCatDist;}
  Float_t getCatDist() {return 2.*halfCatDist;}
  Float_t getPitch() {return pitch;}
  Float_t getCosWireOr() {return cosWireOr;}
  Float_t getSinWireOr() {return sinWireOr;}
  Float_t getTanWireOr() {return sinWireOr/cosWireOr;}
  Float_t getWireOffSet() {return wireOffSet;}
  Int_t calcCellNum(Float_t x, Float_t y);
      
  Double_t getAlfa(const HGeomVector& p1, const HGeomVector& p2);
  Double_t getDist(const HGeomVector& p1, const HGeomVector& p2, Int_t cell);
  void getImpact(const HGeomVector& p1, const HGeomVector& p2, Int_t cell, 
      Double_t &alfa, Double_t &per);
  
  ClassDef(HMdcSizesCellsLayer,0)
}; 
      

class HMdcSizesCellsMod : public TObject {
  friend class HMdcSizesCells;
protected:
  Short_t sector;
  Short_t module;
  HGeomTransform sysRSec;
  TObjArray* array;  // array of pointers of type HMdcSizesCellsLayer 
public:
  HMdcSizesCellsMod(Int_t sec, Int_t mod);
  ~HMdcSizesCellsMod();
  HMdcSizesCellsLayer& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsLayer*>((*array)[i]);
    }
  Int_t getSize();
  const HGeomTransform* getSecTrans() { return &sysRSec; }
    
  ClassDef(HMdcSizesCellsMod,0)
};


class HMdcSizesCellsSec : public TObject {
  friend class HMdcSizesCells;
protected:
  Short_t sector;
  TObjArray* array;   // array of pointers of type HMdcSizesCellsMod
  HGeomTransform sysRLab;
public:
  HMdcSizesCellsSec(Int_t sec);
  ~HMdcSizesCellsSec();
  HMdcSizesCellsMod& operator[](Int_t i) {
    return *static_cast<HMdcSizesCellsMod*>((*array)[i]);
  }
  Int_t getSize();
  const HGeomTransform* getLabTrans() { return &sysRLab; }
  
  ClassDef(HMdcSizesCellsSec,0)
};


class HMdcSizesCells : public TObject {
protected:
  static HMdcSizesCells* fMdcSizesCells;
  HMdcGetContainers* fGetCont;
  HMdcLayerGeomPar* fLayerGeomPar;
  HMdcGeomPar* fGeomPar;
  HMdcDetector* fMdcDet;
  HMdcGeomStruct* fMdcGeomStruct;
  TObjArray* array;     // array of pointers of type HMdcSizesCellsSec 
  Bool_t  changed;    
public:
  static HMdcSizesCells* getObject();
  static HMdcSizesCells* getExObject();
  static void deleteCont();
  HMdcSizesCellsSec& operator[](Int_t i) {
    return *static_cast<HMdcSizesCellsSec*>((*array)[i]);
  }
  Int_t  getSize();
  Bool_t initContainer();
  Bool_t hasChanged() {return changed;}
  void   clear();
  Bool_t getCellVol(Int_t sec,Int_t mod,Int_t lay,Int_t cell,HMdcTrap& volCell);
protected:
  HMdcSizesCells();
  ~HMdcSizesCells();
  Bool_t fillCont(Int_t sec);

  ClassDef(HMdcSizesCells,0)
};

#endif  /*!HMDCSIZESCELLS_H*/
