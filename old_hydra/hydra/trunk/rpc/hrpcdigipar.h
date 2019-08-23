#ifndef HRPCDIGIPAR_H
#define HRPCDIGIPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hlocation.h"
#include "hparset.h"

class HRpcDigiParCell : public TObject {

private:
  Float_t vprop;         // signal velocity [mm/ns]
  Float_t S_x;           // position resolution [mm]
  Float_t S_time;        // time resolution [ps]
  Float_t V_th;          // threshold voltage [v]
  Float_t eff;           // efficiency

public:
  HRpcDigiParCell(void) {clear();}
  ~HRpcDigiParCell(void) {}
  Float_t getVprop()    { return vprop;   }
  Float_t getSigmaX()   { return S_x;     }
  Float_t getSigmaT()   { return S_time;  }
  Float_t getVthres()   { return V_th;    }
  Float_t getEff()      { return eff;     }

  void fill(HRpcDigiParCell& c) {
    vprop   = c.getVprop();
    S_x     = c.getSigmaX();
    S_time  = c.getSigmaT();
    V_th    = c.getVthres();
    eff     = c.getEff();
  }
  void setVprop(Float_t avprop)     { vprop   = avprop;   }
  void setSigmaX(Float_t aS_x)      { S_x     = aS_x;     }
  void setSigmaT(Float_t aS_time)   { S_time  = aS_time;  }
  void setVthres(Float_t aV_th)     { V_th    = aV_th;    }
  void setEff   (Float_t aeff)      { eff     = aeff;     }

  void setDefaults(Int_t f);
  void clear();
  ClassDef(HRpcDigiParCell,2)  // Cell level of the RPC digitization parameters
};

class HRpcDigiParCol: public TObject {
private:
  TObjArray* array;  // pointer array containing HRpcDigiParCell objects
public:
  HRpcDigiParCol(Int_t s=0, Int_t co=0);
  ~HRpcDigiParCol();
  HRpcDigiParCell& operator[](Int_t i) {
      return *static_cast<HRpcDigiParCell*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HRpcDigiParCol,1)   // Column level of the RPC digitization parameters
};

class HRpcDigiParSec: public TObject {
private:
  TObjArray* array;  // pointer array containing HRpcDigiParCol objects
public:
  HRpcDigiParSec(Int_t s=0, Int_t co=0);
  ~HRpcDigiParSec();
  HRpcDigiParCol& operator[](Int_t i) {
      return *static_cast<HRpcDigiParCol*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HRpcDigiParSec,1) // Sector level of the Rpc digitizaton parameters
};

class HRpcDigiPar : public HParSet {
private:
  TObjArray* array;     // array of pointers of type HRpcDigiParCol
  void printPause(void);
public:
  HRpcDigiPar(const Char_t* name="RpcDigiPar",
              const Char_t* title="RPC digitization parameters",
              const Char_t* context="RpcDigiProduction");
  ~HRpcDigiPar(void);
  HRpcDigiParSec& operator[](Int_t i) {
    return *static_cast<HRpcDigiParSec*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t*, Int_t, Int_t, Int_t);
  ClassDef(HRpcDigiPar,1)  // Container for the RPC digitization parameters
};

#endif /* !HRPCDIGIPAR_H */
