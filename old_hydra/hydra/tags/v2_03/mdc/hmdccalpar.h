#ifndef HMDCCALPAR_H
#define HMDCCALPAR_H

#include "TObject.h"
#include "TClonesArray.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hparset.h"

class HMdcCalParCell : public TObject {
protected:
    float slope;
    float offset;
public:
    HMdcCalParCell() : slope(1.F), offset(0.F) {;}
    ~HMdcCalParCell() {;}
    float getSlope() {return slope;}
    float getOffset() {return offset;}
    void fill(float s,float o) {
      slope=s;
      offset=o;
    }
    void setSlope(float s) {slope=s;}
    void setOffset(float o) {offset=o;}
    void clear() {
      slope=0.F;
      offset=0.F;
    }
    ClassDef(HMdcCalParCell,1) // Cell level of the MDC calibration parameters 
};


class HMdcCalParLayer : public TObject {
protected:
    TClonesArray* array; // array of pointers of type HMdcCalParCell
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
    TObjArray* array;     // array of pointers of type HMdcCalParSec 
    TNtuple* ntuple;      // !Ntuple with all calibration parameters 
public:
    HMdcCalPar();
    ~HMdcCalPar();
    HMdcCalParSec& operator[](Int_t i) {
      return *static_cast<HMdcCalParSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set);
    Int_t write(HParIo* output);
    void clear();
    TNtuple* getNtuple();
    ClassDef(HMdcCalPar,1) // Container for the MDC calibration parameters
};

#endif  /*!HMDCCALPAR_H*/

