#ifndef HMDC12CELLPROJEC_H
#define HMDC12CELLPROJEC_H

#include "TObject.h"
#include "TClonesArray.h"
#include "TArrayI.h"
#include "hparset.h"
#include "hmdcgeomobj.h"
#include "hmdcsizescells.h"
#include "hgeomtransform.h"
#include "hspecgeompar.h"



class HMdc12CellProjecCell : public TObject {
protected:
  HMdcTrapPlane cellPr;  //Projection of cell (4 poins on the plane) ???
  Bool_t set;
public:
  HMdc12CellProjecCell():set(kFALSE) {;}
  ~HMdc12CellProjecCell() {;}
  HMdcTrapPlane& cellProjec(){return cellPr;}
  Bool_t isCell(){return set;} 
  void setFALSE() {set=kFALSE;}
  void setTRUE() {set=kTRUE;}  
  void clear();
  ClassDef(HMdc12CellProjecCell,1)
}; 

class HMdc12CellProjecLayer : public TObject {
protected:
  // array of pointers of type HMdc12CellProjecCell
  TClonesArray* array;
public:
  HMdc12CellProjecLayer(Int_t sec, Int_t mod, Int_t layer);
  ~HMdc12CellProjecLayer();
  HMdc12CellProjecCell& operator[](Int_t i) {
      return *static_cast<HMdc12CellProjecCell*>((*array)[i]);
  }
  Int_t getSize();
  ClassDef(HMdc12CellProjecLayer,1)
}; 
      

class HMdc12CellProjecMod : public TObject {
protected: 
  // array of pointers of type HMdc12CellProjecLayer 
  TObjArray* array;
public:
  HMdc12CellProjecMod(Int_t sec, Int_t mod);
  ~HMdc12CellProjecMod();
  HMdc12CellProjecLayer& operator[](Int_t i) {
      return *static_cast<HMdc12CellProjecLayer*>((*array)[i]);
    }
  Int_t getSize();
  ClassDef(HMdc12CellProjecMod,1)
};


class HMdc12CellProjecSec : public TObject {
protected:
    TObjArray* array;   // array of pointers of type HMdc12CellProjecMod
    HGeomTransform coordSys;
    Int_t sector;
public:
    HMdc12CellProjecSec(Int_t sec);
    ~HMdc12CellProjecSec();
    void setSys(HGeomTransform &sys){coordSys.setTransform(sys);}
    HGeomTransform& getSys(){return coordSys;}
    HMdc12CellProjecMod& operator[](Int_t i) {
      return *static_cast<HMdc12CellProjecMod*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdc12CellProjecSec,1)
};


class HMdc12CellProjec : public TObject {
protected:
    TObjArray* array;     // array of pointers of type HMdc12CellProjecSec
    Bool_t  isContainer; 
public:
    HMdc12CellProjec();
    ~HMdc12CellProjec();
    HMdc12CellProjecSec& operator[](Int_t i) {
      return *static_cast<HMdc12CellProjecSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init() {return isContainer;}
    void clear();
//    TNtuple* getNtuple();
    ClassDef(HMdc12CellProjec,1)
};

//--------------------------------------------------------

class HMdcCalc12CellProjec : public TObject {
protected:
//    TObjArray* array;     // array of pointers of type HMdcCalcLookUpTbSec 
//    TNtuple* ntuple;      // !Ntuple with all calibration parameters 
    HMdcSizesCells* fSizesCells;
    HMdcGeomPar* fGeomPar;
    HMdcDetector* fMdcDet;
    HSpecGeomPar* fSpecGeomPar;
    
    Double_t Ap,Bp,Cp,Dp;
    HGeomVector pointsProj[16];
    HGeomVector targetPs[2];    //Target
    
    void setParContainers();
    void calcProjPoint(HMdcTrap& cell, HGeomTransform& secsys);
    void calcPoint(HMdcPointPlane& proj, HGeomVector& p1l1, HGeomVector& p2l1, 
                                         HGeomVector& p1l2, HGeomVector& p2l2);
    Double_t calcDist(HGeomVector& p1, HGeomVector& p2);
public:
    HMdcCalc12CellProjec();
    ~HMdcCalc12CellProjec();
    Bool_t calcMdc12Proj(Int_t nSec, HMdc12CellProjec* f12CellProjec);

    ClassDef(HMdcCalc12CellProjec,1)
};

#endif  /*!HMDC12CELLPROJEC_H*/
