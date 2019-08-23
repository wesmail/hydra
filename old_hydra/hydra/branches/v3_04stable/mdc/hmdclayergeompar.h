#ifndef HMDCLAYERGEOMPAR_H
#define HMDCLAYERGEOMPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hparset.h"


class HMdcLayerGeomParLay : public TObject {

protected:
  Float_t pitch;          // Distance bitween sense wires
  Int_t numWires;         //! Number of wires
  Float_t catDist;        // Cell thickness
  Float_t wireOrient;     // Inclination angle
  Float_t centralWireNr;  // Number of wire which goes through geometrical center of MDC

public:
  HMdcLayerGeomParLay() : pitch(0.F), numWires(0), catDist(0.F), 
    wireOrient(0.F), centralWireNr(-1.F) {;}
  ~HMdcLayerGeomParLay() {;}
  Float_t getPitch() {return pitch;}
  Int_t getNumWires() {return numWires;}
  Float_t getCatDist() {return catDist;}
  Float_t getWireOrient() {return wireOrient;}
  Float_t getCentralWireNr() {return centralWireNr;}
  void fill(Float_t a, Float_t c, Float_t d, Float_t e) {
    pitch=a;
    catDist=c;
    wireOrient=d;
    centralWireNr=e;
  }
  void setPitch(Float_t a) {pitch=a;}
  void setNumWires(Int_t b) {numWires=b;}
  void setCatDist(Float_t c) {catDist=c;}
  void setWireOrient(Float_t d) {wireOrient=d;}
  void setCentralWireNr(Float_t e) {centralWireNr=e;}
  void clear() {
    pitch=0.F;
    numWires=0;
    catDist=0.F;
    wireOrient=0.F;
    centralWireNr=-1.F;
  }
  ClassDef(HMdcLayerGeomParLay,1) // Layer level of Parameter container HMdcLayerGeomPar
}; 
   

class HMdcLayerGeomParMod : public TObject {
protected:
  TObjArray* array; 
public:
  HMdcLayerGeomParMod();
  ~HMdcLayerGeomParMod();
  HMdcLayerGeomParLay& operator[](Int_t i) {
      return *static_cast<HMdcLayerGeomParLay*>((*array)[i]);
  }
  Int_t getSize();
  ClassDef(HMdcLayerGeomParMod,1) // Module level of Parameter container HMdcLayerGeomPar
};


class HMdcLayerGeomParSec : public TObject {
protected:
    TObjArray* array;
public:
    HMdcLayerGeomParSec();
    ~HMdcLayerGeomParSec();
    HMdcLayerGeomParMod& operator[](Int_t i) {
      return *static_cast<HMdcLayerGeomParMod*>((*array)[i]);
    }
  Int_t getSize();
  ClassDef(HMdcLayerGeomParSec,1) // Sector level of Parameter container HMdcLayerGeomPar
};


class HMdcLayerGeomPar : public HParSet {
protected:
    TObjArray* array;     // array of pointers of type HMdcLayerGeomParSec 
    TNtuple* ntuple;      //! Ntuple with all parameters 
public:
    HMdcLayerGeomPar();
    ~HMdcLayerGeomPar();
    HMdcLayerGeomParSec& operator[](Int_t i) {
      return *static_cast<HMdcLayerGeomParSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set);
    Int_t write(HParIo* output);
    void clear();
    TNtuple* getNtuple();
    ClassDef(HMdcLayerGeomPar,1) // Parameter container for the geometry of MDC layers

};

#endif  /*!HMDCLAYERGEOMPAR_H*/
