#ifndef HMDCDIGITGEOMPAR_H
#define HMDCDIGITGEOMPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hparset.h"


class HMdcDigitGeomParLayer : public TObject {

protected:
  Float_t pitch;          // Distance bitween sense wires
  Int_t numWires;         //! Number of wires
  Float_t catDist;        // Cell thickness
  Float_t wireOrient;     // Inclination angle
  Float_t centralWireNr;  // Number of wire which goes through geometrical center of MDC

public:
  HMdcDigitGeomParLayer() : pitch(0.F), numWires(0), catDist(0.F), 
    wireOrient(0.F), centralWireNr(-1.F) {;}
  ~HMdcDigitGeomParLayer() {;}
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
  ClassDef(HMdcDigitGeomParLayer,1) // Layer level of the MDC digitization parameters (geometrical parameters)
}; 
   

class HMdcDigitGeomParMod : public TObject {
protected:
  TObjArray* array; 
public:
  HMdcDigitGeomParMod();
  ~HMdcDigitGeomParMod();
  HMdcDigitGeomParLayer& operator[](Int_t i) {
      return *static_cast<HMdcDigitGeomParLayer*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcDigitGeomParMod,1) // Module level of the MDC digitization parameters (geometrical parameters)
};


class HMdcDigitGeomParSec : public TObject {
protected:
    TObjArray* array;
public:
    HMdcDigitGeomParSec();
    ~HMdcDigitGeomParSec();
    HMdcDigitGeomParMod& operator[](Int_t i) {
      return *static_cast<HMdcDigitGeomParMod*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcDigitGeomParSec,1) // Sector level of the MDC digitization parameters (geometrical parameters)
};


class HMdcDigitGeomPar : public HParSet {
protected:
    TObjArray* array;     // array of pointers of type HMdcDigitGeomParSec 
    TNtuple* ntuple;      // !Ntuple with all calibration parameters 
public:
    HMdcDigitGeomPar();
    ~HMdcDigitGeomPar();
    HMdcDigitGeomParSec& operator[](Int_t i) {
      return *static_cast<HMdcDigitGeomParSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set);
    Int_t write(HParIo* output);
    void clear();
    TNtuple* getNtuple();
    ClassDef(HMdcDigitGeomPar,1) // Container for the MDC digitization parameters (geometrical parameters)

};

#endif  /*!HMDCDIGITGEOMPAR_H*/
