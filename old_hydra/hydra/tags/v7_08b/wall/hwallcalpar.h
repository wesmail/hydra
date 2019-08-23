#ifndef HWALLCALPAR_H
#define HWALLCALPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HWallCalParCell : public TObject {
protected:
    Float_t TDC_slope;
    Float_t TDC_offset;
    Float_t ADC_slope;
    Float_t ADC_offset;

public:
    HWallCalParCell() { clear(); }
    ~HWallCalParCell() {;}
    Float_t getTDC_Slope()  { return TDC_slope; }
    Float_t getTDC_Offset() { return TDC_offset; }
    Float_t getADC_Slope()  { return ADC_slope; }
    Float_t getADC_Offset() { return ADC_offset; }
    void fill(Float_t Ts,Float_t To,Float_t As,Float_t Ao) {
      TDC_slope =Ts;
      TDC_offset=To;
      ADC_slope =As;
      ADC_offset=Ao;
    }
    void fill(HWallCalParCell& r) {
      TDC_slope =r.getTDC_Slope();
      TDC_offset=r.getTDC_Offset();
      ADC_slope =r.getADC_Slope();
      ADC_offset=r.getADC_Offset();
    }
    void setTDC_Slope(Float_t Ts)  { TDC_slope =Ts; }
    void setTDC_Offset(Float_t To) { TDC_offset=To; }
    void setADC_Slope(Float_t As)  { ADC_slope =As; }
    void setADC_Offset(Float_t Ao) { ADC_offset=Ao; }

    void clear() {
      TDC_slope =0.F;
      TDC_offset=0.F;
      ADC_slope =0.F;
      ADC_offset=0.F;
    }
    ClassDef(HWallCalParCell,1) // Chan level of the START calibration parameters 
};

class HWallCalParFW: public TObject {
protected:
  TObjArray* array;  // pointer array containing HStartCalParChan objects
public:
  HWallCalParFW(Int_t n=384);
  ~HWallCalParFW();
  HWallCalParCell& operator[](Int_t i) {
      return *static_cast<HWallCalParCell*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HWallCalParFW,1) // Module level of Start Calibration parameters
};

class HWallCalPar : public HParSet {
protected:
  TObjArray* array;     // array of pointers of type HStartCalParMod 
public:
  HWallCalPar(const char* name="StartCalPar",
               const char* title="Calibration parameters for Start",
               const char* context="StartCalProduction");
  ~HWallCalPar();
  HWallCalParFW& operator[](Int_t i) {
    return *static_cast<HWallCalParFW*>((*array)[i]);

  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t);
  ClassDef(HWallCalPar,1) // Container for the WALL calibration parameters
};

#endif  /*!HWALLCALPAR_H*/









