//*-- AUTHOR Bjoern Spruck
//*-- created : 19.4.06

#ifndef HHODOREFHITPAR_H
#define HHODOREFHITPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HHodoRefHitParMod : public TObject
{
protected:
   Float_t time_trig_ref_low;// Lower bound of trig ref search window
   Float_t time_trig_ref_high;// Upper bound of trig ref search window
   Float_t time_signal_low;// Lower bound of signal window
   Float_t time_signal_high;// Upper bound of signal window
public:
   HHodoRefHitParMod() { clear(); }
   ~HHodoRefHitParMod() {;}
   Float_t getTrigRefLow(void){ return time_trig_ref_low;}
   Float_t getTrigRefHigh(void){ return time_trig_ref_high;}
   Float_t getHitLow(void){ return time_signal_low;}
   Float_t getHitHigh(void){ return time_signal_high;}

   void setTrigRefLow(Float_t s){ time_trig_ref_low=s;}
   void setTrigRefHigh(Float_t s){ time_trig_ref_high=s;}
   void setHitLow(Float_t s){ time_signal_low=s;}
   void setHitHigh(Float_t s){ time_signal_high=s;}

   void fill(Float_t tl,Float_t th,Float_t hl,Float_t hh) {
      time_trig_ref_low=tl;
      time_trig_ref_high=th;
      time_signal_low=hl;
      time_signal_high=hh;
   }
   void fill(Float_t *s) {
      time_trig_ref_low=s[0];
      time_trig_ref_high=s[1];
      time_signal_low=s[2];
      time_signal_high=s[3];
   }

   void fill(HHodoRefHitParMod& r) {
      time_trig_ref_low = r.getTrigRefLow();
      time_trig_ref_high = r.getTrigRefHigh();
      time_signal_low = r.getHitLow();
      time_signal_high = r.getHitHigh();
   }

   void clear() {
      time_trig_ref_low=0.F;
      time_trig_ref_high=0.F;
      time_signal_low=0.F;
      time_signal_high=0.F;
   }
   ClassDef(HHodoRefHitParMod,1) // Chan level of the HODO calibration parameters
};

class HHodoRefHitPar : public HParSet
{
protected:
   TObjArray* array;     // array of pointers of type HHodoRefHitParMod
public:
   HHodoRefHitPar(const Char_t* name="HodoRefHitPar",
                  const Char_t* title="Reference Trigger and Hit parameters for Hodo",
                  const Char_t* context="HodoRawAndHitProduction");
   ~HHodoRefHitPar();
   HHodoRefHitParMod& operator[](Int_t i) {
      return *static_cast<HHodoRefHitParMod*>((*array)[i]);
   }
   Int_t getSize() { return array->GetEntries(); }
   Bool_t init(HParIo* input,Int_t* set);
   Int_t write(HParIo* output);
   void clear();
   void printParam();
   void readline(const Char_t*, Int_t*);
   void putAsciiHeader(TString&);
   Bool_t writeline(Char_t*, Int_t);
   ClassDef(HHodoRefHitPar,1) // Container for the HODO RefHit parameters
};

#endif  /*!HHODOREFHITPAR_H*/
