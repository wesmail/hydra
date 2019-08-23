//*-- AUTHOR Bjoern Spruck
//*-- created : 24.03.06

#ifndef HHODOCALPAR_H
#define HHODOCALPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HHodoCalParChan : public TObject
{
protected:
   Float_t time_slope;  // TDC slope
   Float_t time_offset; // TDC offset
   Float_t adc_slope;  // ADC slope
   Float_t adc_offset; // ADC offset
public:
   HHodoCalParChan() { clear(); }
   ~HHodoCalParChan() {;}
   Float_t getTDCSlope(void){ return time_slope;}
   Float_t getTDCOffset(void){ return time_offset;}
   Float_t getADCSlope(void){ return adc_slope;}
   Float_t getADCOffset(void){ return adc_offset;}

   void fill(Float_t ts,Float_t to,Float_t as,Float_t ao) {
      time_slope=ts;
      time_offset=to;
      adc_slope=as;
      adc_offset=ao;
   }
   void fill(Float_t *s) {
      time_slope=s[0];
      time_offset=s[1];
      adc_slope=s[2];
      adc_offset=s[3];
   }

   void fill(HHodoCalParChan& r) {
      time_slope = r.getTDCSlope();
      time_offset = r.getTDCOffset();
      adc_slope = r.getADCSlope();
      adc_offset = r.getADCOffset();
   }
   void setTDCSlope(Float_t s) { time_slope=s; }
   void setTDCOffset(Float_t o) { time_offset=o; }
   void setADCSlope(Float_t s) { adc_slope=s; }
   void setADCOffset(Float_t o) { adc_offset=o; }

   void clear() {
      time_slope=0.F;
      time_offset=0.F;
      adc_slope=0.F;
      adc_offset=0.F;
   }
   ClassDef(HHodoCalParChan,1) // Chan level of the HODO calibration parameters
};

class HHodoCalParMod: public TObject
{
protected:
   TObjArray* array;  // pointer array containing HHodoCalParChan objects
public:
   HHodoCalParMod(Int_t n=8);
   ~HHodoCalParMod();
   HHodoCalParChan& operator[](Int_t i) {
      return *static_cast<HHodoCalParChan*>((*array)[i]);
   }
   Int_t getSize() { return array->GetEntries(); }
   ClassDef(HHodoCalParMod,1) // Module level of Hodo Calibration parameters
};

class HHodoCalPar : public HParSet
{
protected:
   TObjArray* array;     // array of pointers of type HHodoCalParMod
public:
   HHodoCalPar(const Char_t* name="HodoCalPar",
                  const Char_t* title="Calibration parameters for Hodo",
                  const Char_t* context="HodoCalProduction");
   ~HHodoCalPar();
   HHodoCalParMod& operator[](Int_t i) {
      return *static_cast<HHodoCalParMod*>((*array)[i]);
   }
   Int_t getSize() { return array->GetEntries(); }
   Bool_t init(HParIo* input,Int_t* set);
   Int_t write(HParIo* output);
   void clear();
   void printParam();
   void readline(const Char_t*, Int_t*);
   void putAsciiHeader(TString&);
   Bool_t writeline(Char_t*, Int_t, Int_t);
   ClassDef(HHodoCalPar,1) // Container for the HODO calibration parameters
};

#endif  /*!HHODOCALPAR_H*/
