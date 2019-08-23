#ifndef HRPCDIGIPAR_H
#define HRPCDIGIPAR_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"

class HRpcDigiParCell : public TObject {

private:
  Float_t Length;        // length of the strip in the inferior part [mm]
  Float_t X_left;        // X position of the left-most corner [mm]
  Float_t Y_left;        // Y position of the left-most corner [mm]
  Float_t width;         // width of the cell [mm]
  Float_t widthT;        // width of the tube [mm]
  Float_t thetaL;        // angle of the left-most slanting side
  Float_t thetaR;        // angle of the right-most slanting side

  //FIXME: I would like to add the dependencies with the threshold and t_o, sigma...,
  //Where to define it??

public:
  HRpcDigiParCell(void) {clear();}
  ~HRpcDigiParCell(void) {}
  Float_t getLength()   { return Length; }
  Float_t getX()        { return X_left; }
  Float_t getY()        { return Y_left; }
  Float_t getWidth()    { return width;  }
  Float_t getWidthT()   { return widthT; }
  Float_t getThetaL()   { return thetaL; }
  Float_t getThetaR()   { return thetaR; } 

  void fill(HRpcDigiParCell& c) {
    Length  = c.getLength();
    X_left  = c.getX();
    Y_left  = c.getY();
    width   = c.getWidth();
    widthT  = c.getWidthT();
    thetaL  = c.getThetaL();
    thetaR  = c.getThetaR();    
  }
  void setLength(Float_t f)   { Length = f; }
  void setX(Float_t f)        { X_left = f; }
  void setY(Float_t f)        { Y_left = f; }
  void setWidth(Float_t f)    { width  = f; }
  void setWidthT(Float_t f)   { widthT = f; }
  void setThetaL(Float_t f)   { thetaL = f; }
  void setThetaR(Float_t f)   { thetaR = f; }

  void setDefaults(Int_t f);
  void clear();
  ClassDef(HRpcDigiParCell,1)  // Cell level of the RPC digitization parameters
};

// FIXME: What is this good for??

class HRpcDigiParMod: public TObject {
private:
  TObjArray* array;  // pointer array containing HRpcDigiParCell objects
public:
  HRpcDigiParMod(Int_t s=0, Int_t m=0);
  ~HRpcDigiParMod();
  HRpcDigiParCell& operator[](Int_t i) {
      return *static_cast<HRpcDigiParCell*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HRpcDigiParMod,1)   // Module level of the RPC digitization parameters
};

// FIXME: What is this good for??

class HRpcDigiParSec: public TObject {
private:
  TObjArray* array;  // pointer array containing HRpcDigiParMod objects
public:
  HRpcDigiParSec(Int_t s=0, Int_t n=6);
  ~HRpcDigiParSec();
  HRpcDigiParMod& operator[](Int_t i) {
      return *static_cast<HRpcDigiParMod*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HRpcDigiParSec,1) // Sector level of the Rpc digitizaton parameters
};

class HRpcDigiPar : public HParSet {
private:
  TObjArray* array;     // array of pointers of type HRpcDigiParMod
  void printPause(void);
public:
  HRpcDigiPar(const char* name="RpcDigiPar",
              const char* title="RPC digitization parameters",
              const char* context="RpcDigiProduction");
  ~HRpcDigiPar(void);
  HRpcDigiParSec& operator[](Int_t i) {
    return *static_cast<HRpcDigiParSec*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t, Int_t);
  ClassDef(HRpcDigiPar,1)  // Container for the RPC digitization parameters
};

#endif /* !HRPCDIGIPAR_H */
