#ifndef HMDCDIGITPAR_H
#define HMDCDIGITPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hparset.h"


class HMdcDigitParLayer : public TObject {

protected:
  Float_t driftVelocity;
  Float_t mdcEfficiency;
  Float_t spacePar0;
  Float_t spacePar1;
  Float_t spacePar2;

public:
  HMdcDigitParLayer() : driftVelocity(0.F), mdcEfficiency(0.F), 
    spacePar0(0.F),spacePar1(0.F),spacePar2(0.F) {;} 
  ~HMdcDigitParLayer() {;}
  Float_t getDriftVelocity() {return driftVelocity;}
  Float_t getMdcEfficiency() {return mdcEfficiency;}
  Float_t getSpacePar0() {return spacePar0;}
  Float_t getSpacePar1() {return spacePar1;}
  Float_t getSpacePar2() {return spacePar2;}
  void fill(Float_t a, Float_t b, Float_t c, Float_t d, Float_t e) {
    driftVelocity=a;
    mdcEfficiency=b;
    spacePar0=c;
    spacePar1=d;
    spacePar2=e;
  }
  void setDriftVelocity(Float_t a) {driftVelocity=a;}
  void setMdcEfficiency(Float_t b) {mdcEfficiency=b;}
  void setSpacePar0(Float_t c) {spacePar0=c;}
  void setSpacePar1(Float_t d) {spacePar1=d;}
  void setSpacePar2(Float_t e) {spacePar2=e;}
  void clear() {
    driftVelocity=0.;
    mdcEfficiency=0.;
    spacePar0=0.;
    spacePar1=0.;
    spacePar2=0.;
  }
  ClassDef(HMdcDigitParLayer,1) // Layer level of the MDC digitization parameters (physical parameters)
}; 
   

class HMdcDigitParMod : public TObject {
protected:
  TObjArray* array; 
public:
  HMdcDigitParMod();
  ~HMdcDigitParMod();
  HMdcDigitParLayer& operator[](Int_t i) {
      return *static_cast<HMdcDigitParLayer*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcDigitParMod,1) // Module level of the MDC digitization parameters (physical parameters)
};


class HMdcDigitParSec : public TObject {
protected:
    TObjArray* array;
public:
    HMdcDigitParSec();
    ~HMdcDigitParSec();
    HMdcDigitParMod& operator[](Int_t i) {
      return *static_cast<HMdcDigitParMod*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcDigitParSec,1) // Sector level of the MDC digitization parameters (physical parameters) 
};


class HMdcDigitPar : public HParSet {
protected:
    TObjArray* array;     // array of pointers of type HMdcDigitParSec 
    TNtuple* ntuple;      // !Ntuple with all calibration parameters 
public:
    HMdcDigitPar();
    ~HMdcDigitPar();
    HMdcDigitParSec& operator[](Int_t i) {
      return *static_cast<HMdcDigitParSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set);
    Int_t write(HParIo* output);
    void clear();
    TNtuple* getNtuple();
    ClassDef(HMdcDigitPar,1) // Container for the MDC digitization parameters (physical parameters)
};

#endif  /*!HMDCDIGITPAR_H*/
