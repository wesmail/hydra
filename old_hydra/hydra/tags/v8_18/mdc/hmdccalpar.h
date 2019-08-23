#ifndef HMDCCALPAR_H
#define HMDCCALPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hparset.h"
#include "hmdccalparraw.h"

class HMdcCalParCell : public TObject {
protected:
    Float_t slope;       // gain
    Float_t offset;      // offset
    Float_t slopeErr;    // error of gain
    Float_t offsetErr;   // error of offset
    Int_t slopeMethod;   // number of method used to determine the slope
    Int_t offsetMethod;  // number of method used to determine the offset
public:
    HMdcCalParCell() : slope(1.F), offset(0.F),slopeErr(0), offsetErr(0),slopeMethod(0), offsetMethod(0) {;}
    ~HMdcCalParCell() {;}
    Float_t getSlope()       {return slope;}
    Float_t getOffset()      {return offset;}
    Float_t getSlopeErr()    {return slopeErr;}
    Float_t getOffsetErr()   {return offsetErr;}
    Int_t getSlopeMethod() {return slopeMethod;}
    Int_t getOffsetMethod(){return offsetMethod;}
    void fill(Float_t s,Float_t o ,Float_t sE,Float_t oE, Int_t sM,Int_t oM ) {
      slope=s;
      offset=o;
      slopeErr=sE;
      offsetErr=oE;
      slopeMethod=sM;
      offsetMethod=oM;
    }
    void fill(HMdcCalParCell& r) {
      slope=r.getSlope();
      offset=r.getOffset();
      slopeErr=r.getSlopeErr();
      offsetErr=r.getOffsetErr();
      slopeMethod=r.getSlopeMethod();
      offsetMethod=r.getOffsetMethod();
    }
    void setSlope       (Float_t s) {slope=s;}
    void setOffset      (Float_t o) {offset=o;}
    void setSlopeErr    (Float_t sE) {slopeErr=sE;}
    void setOffsetErr   (Float_t oE) {offsetErr=oE;}
    void setSlopeMethod (Int_t sM) {slopeMethod=sM;}
    void setOffsetMethod(Int_t oM) {offsetMethod=oM;}
    void clear() {
      slope=1.F;
      offset=0.F;
      slopeErr=0.F;
      offsetErr=0.F;
      slopeMethod=0;
      offsetMethod=0;
    }
    ClassDef(HMdcCalParCell,1) // Cell level of the MDC calibration parameters 
};


class HMdcCalParLayer : public TObject {
protected:
    TObjArray* array; // array of pointers of type HMdcCalParCell
public:
    HMdcCalParLayer(Int_t n=0);
    ~HMdcCalParLayer();
    HMdcCalParCell& operator[](Int_t i) {
      return *static_cast<HMdcCalParCell*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcCalParLayer,1) // Layer level of the MDC calibration parameters
};


class HMdcCalParMod : public TObject {
protected:
    TObjArray* array;   // array of pointers of type HMdcCalParLayer
public:
    HMdcCalParMod(Int_t sec=0, Int_t mod=0, Int_t n = 6);
    ~HMdcCalParMod();
    HMdcCalParLayer& operator[](Int_t i) {
      return *static_cast<HMdcCalParLayer*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcCalParMod,1) // Module level of the MDC calibration parameters
};


class HMdcCalParSec : public TObject {
protected:
    TObjArray* array;   // array of pointers of type HMdcCalParMod
    Int_t sector;       // number of sector
public:
    HMdcCalParSec(Int_t sec=0, Int_t n = 4);
    ~HMdcCalParSec();
    HMdcCalParMod& operator[](Int_t i) {
      return *static_cast<HMdcCalParMod*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcCalParSec,1) // Sector level of the MDC calibration parameters
};


class HMdcCalPar : public HParSet {
protected:
    TObjArray* array;     //  array of pointers of type HMdcCalParSec 
    TNtuple* ntuple;      //! Ntuple with all calibration parameters 
public:
    HMdcCalPar(const Char_t* name="MdcCalPar",
               const Char_t* title="calibration parameters for Mdc",
               const Char_t* context="MdcCalParRawProduction");
    ~HMdcCalPar();
    HMdcCalParSec& operator[](Int_t i) {
      return *static_cast<HMdcCalParSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set);
    Int_t write(HParIo* output);
    void putAsciiHeader(TString&);
    Bool_t writeline(Char_t *, Int_t, Int_t, Int_t, Int_t);
    void clear();
    TNtuple* getNtuple();
    Bool_t fillMdcCalParRaw(HMdcCalParRaw&);
    ClassDef(HMdcCalPar,1) // Container for the MDC calibration parameters
};

#endif  /*!HMDCCALPAR_H*/

