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
  Float_t spacePar1;
  Float_t spacePar2;
  Float_t spacePar3;
  Float_t spacePar4;
  Float_t spacePar5;
public:
  HMdcDigitParLayer() : driftVelocity(0.F), mdcEfficiency(0.F), 
    spacePar1(0.F),spacePar2(0.F),spacePar3(0.F),
    spacePar4(0.F),spacePar5(0.F) {;} 
  ~HMdcDigitParLayer() {;}
  Float_t getDriftVelocity() {return driftVelocity;}
  Float_t getMdcEfficiency() {return mdcEfficiency;}
  Float_t getSpacePar1() {return spacePar1;}
  Float_t getSpacePar2() {return spacePar2;}
  Float_t getSpacePar3() {return spacePar3;}
  Float_t getSpacePar4() {return spacePar4;}
  Float_t getSpacePar5() {return spacePar5;}
  void fill(Float_t a, Float_t b, Float_t c, Float_t d, Float_t e, 
	    Float_t f=0.4F,Float_t g=2.2F) {
    driftVelocity=a;
    mdcEfficiency=b;
    spacePar1=c;
    spacePar2=d;
    spacePar3=e;
    spacePar4=f;
    spacePar5=g;
  }
  void fill(HMdcDigitParLayer&);
  void setDriftVelocity(Float_t a) {driftVelocity=a;}
  void setMdcEfficiency(Float_t b) {mdcEfficiency=b;}
  void setSpacePar1(Float_t c) {spacePar1=c;}
  void setSpacePar2(Float_t d) {spacePar2=d;}
  void setSpacePar3(Float_t e) {spacePar3=e;}
  void setSpacePar4(Float_t f) {spacePar4=f;}
  void setSpacePar5(Float_t g) {spacePar5=g;}
  void clear() {
    driftVelocity=0.;
    mdcEfficiency=0.;
    spacePar1=0.;
    spacePar2=0.;
    spacePar3=0.;
    spacePar4=0.;
    spacePar5=0.;
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
    HMdcDigitPar(const char* name="MdcDigitPar",
                 const char* title="digitisation parameters for Mdc",
                 const char* context="");
    ~HMdcDigitPar();
    HMdcDigitParSec& operator[](Int_t i) {
      return *static_cast<HMdcDigitParSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set);
    Int_t write(HParIo* output);
    void readline(const char*, Int_t*);
    void putAsciiHeader(TString&);
    Bool_t writeline(char *, Int_t, Int_t, Int_t);
    void clear();
    TNtuple* getNtuple();
    void printParam();
    ClassDef(HMdcDigitPar,1) // Container for the MDC digitization parameters (physical parameters)
};

#endif  /*!HMDCDIGITPAR_H*/
