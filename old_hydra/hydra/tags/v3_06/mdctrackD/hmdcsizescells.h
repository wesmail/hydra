#ifndef HMDCSIZESCELLS_H
#define HMDCSIZESCELLS_H

#include "TObject.h"
#include "TClonesArray.h"
#include "TArrayI.h"
#include "hparset.h"
#include "hmdcgeomobj.h"
#include "hmdclayergeompar.h"
#include "hmdcsizescells.h"
#include "hmdcgeompar.h"
#include "hmdcdetector.h"


class HMdcSizesCellsCell : public TObject {
protected:
  HMdcTrap cellSizes;            // volume of cell (8-points)
  HGeomVector wireSizes[2];      // ends of wire (2-poins)
  Bool_t set;                     
public:
  HMdcSizesCellsCell():set(kFALSE) {;}
  ~HMdcSizesCellsCell() {;}
  HMdcTrap& cell(){return cellSizes;}
  HGeomVector* getWirePoint(Int_t n){
           if(n==0||n==1) return &wireSizes[n]; else return 0;}
  Bool_t isCell(){return set;} 
  void setFALSE() {set=kFALSE;}
  void setTRUE() {set=kTRUE;}   
  void clear();
  ClassDef(HMdcSizesCellsCell,1)
}; 

class HMdcSizesCellsLayer : public TObject {
protected:
  // array of pointers of type HMdcSizesCellsCell
  TClonesArray* array;
public:
  HMdcSizesCellsLayer(Int_t sec, Int_t mod, Int_t layer);
  ~HMdcSizesCellsLayer();
  HMdcSizesCellsCell& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsCell*>((*array)[i]);
  }
  Int_t getSize();
  ClassDef(HMdcSizesCellsLayer,1)
}; 
      

class HMdcSizesCellsMod : public TObject {
protected: 
  // array of pointers of type HMdcSizesCellsLayer 
  TObjArray* array;
public:
  HMdcSizesCellsMod(Int_t sec, Int_t mod);
  ~HMdcSizesCellsMod();
  HMdcSizesCellsLayer& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsLayer*>((*array)[i]);
    }
  Int_t getSize();
  ClassDef(HMdcSizesCellsMod,1)
};


class HMdcSizesCellsSec : public TObject {
protected:
    TObjArray* array;   // array of pointers of type HMdcSizesCellsMod
    Int_t sector;
public:
    HMdcSizesCellsSec(Int_t sec);
    ~HMdcSizesCellsSec();
    HMdcSizesCellsMod& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsMod*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcSizesCellsSec,1)
};


class HMdcSizesCells : public HParSet {
protected:
    TObjArray* array;     // array of pointers of type HMdcSizesCellsSec 
    Bool_t  isContainer;
public:
    HMdcSizesCells();
    ~HMdcSizesCells();
    HMdcSizesCellsSec& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsSec*>((*array)[i]);
    }
    Int_t  getSize();
    Bool_t init(HParIo* input,Int_t* set) {return isContainer;}
    Int_t  write(HParIo* output);
    void   clear();
    ClassDef(HMdcSizesCells,1)
};


//----------------------------------------------------- 
class HMdcCalcSizesCells : public TObject {
protected: 
    Double_t at[4],bt[4];
    Double_t tLine,bLine;
    Int_t nLine1,nLine2;
    Double_t a,b;             // line's parametrs y=a*x+b
    HGeomVector volDrGas[8];  // Object for drift gas vol. (8-points)
    HGeomVector volSeWi[8];   // Object for layer from sen. wires vol.(8-points)
    HMdcTrap volCell;         // Object for cell vol.(8-points)

    HMdcLayerGeomPar* fLayerGeomPar;
    HMdcGeomPar* fGeomPar;
    HMdcDetector* fMdcDet;
    
    void calcBLines();
    Bool_t calcWireLine(HGeomVector& lineP1, HGeomVector& lineP2);
    void calcVolCell();
    void setParContainers();
    
public:
    HMdcCalcSizesCells();
    ~HMdcCalcSizesCells() {};
    Bool_t calcVolCells(Int_t nSector, HMdcSizesCells* fSizesCells);
    
    ClassDef(HMdcCalcSizesCells,1)
};


#endif  /*!HMDCSIZESCELLS_H*/
