#ifndef HRPCGEOMCELLPAR_H
#define HRPCGEOMCELLPAR_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"

class HRpcGeomCellParCell : public TObject {

private:
  Float_t Length;        // length of the strip in the inferior part [mm]
  Float_t X_left;        // X position of the left-most corner [mm]
  Float_t Y_left;        // Y position of the left-most corner [mm]
  Float_t width;         // width of the cell [mm]
  Float_t widthT;        // width of the tube [mm]
  Float_t thetaL;        // angle of the left-most slanting side
  Float_t thetaR;        // angle of the right-most slanting side

public:
  HRpcGeomCellParCell(void) {clear();}
  ~HRpcGeomCellParCell(void) {}
  Float_t getLength()   { return Length; }
  Float_t getX()        { return X_left; }
  Float_t getY()        { return Y_left; }
  Float_t getWidth()    { return width;  }
  Float_t getWidthT()   { return widthT; }
  Float_t getThetaL()   { return thetaL; }
  Float_t getThetaR()   { return thetaR; } 

  void fill(HRpcGeomCellParCell& ce) {
    Length  = ce.getLength();
    X_left  = ce.getX();
    Y_left  = ce.getY();
    width   = ce.getWidth();
    widthT  = ce.getWidthT();
    thetaL  = ce.getThetaL();
    thetaR  = ce.getThetaR();    
  }
  void setLength(Float_t aLength)   { Length = aLength; }
  void setX(Float_t aX_left)        { X_left = aX_left; }
  void setY(Float_t aY_left)        { Y_left = aY_left; }
  void setWidth(Float_t awidth)     { width  = awidth; }
  void setWidthT(Float_t awidthT)   { widthT = awidthT; }
  void setThetaL(Float_t athetaL)   { thetaL = athetaL; }
  void setThetaR(Float_t athetaR)   { thetaR = athetaR; }

  void setDefaults(Int_t f);
  void clear();
  ClassDef(HRpcGeomCellParCell,1)  // Cell level of the RPC geometric parameters of the cells
};

class HRpcGeomCellParCol: public TObject {
private:
  TObjArray* array;  // pointer array containing HRpcGeomCellParCell objects
public:
  HRpcGeomCellParCol(Int_t s=0, Int_t co=0);
  ~HRpcGeomCellParCol();
  HRpcGeomCellParCell& operator[](Int_t i) {
      return *static_cast<HRpcGeomCellParCell*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HRpcGeomCellParCol,1)   // Column level of the RPC geometric parameters of the cells
};

class HRpcGeomCellParSec: public TObject {
private:
  TObjArray* array;  // pointer array containing HRpcGeomCellParCol objects
public:
  HRpcGeomCellParSec(Int_t s=0, Int_t co=0);
  ~HRpcGeomCellParSec();
  HRpcGeomCellParCol& operator[](Int_t i) {
      return *static_cast<HRpcGeomCellParCol*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HRpcGeomCellParSec,1) // Sector level of the Rpc geometric parameters of the cells
};

class HRpcGeomCellPar : public HParSet {
private:
  TObjArray* array;     // array of pointers of type HRpcGeomCellParCol
  void printPause(void);
public:
  HRpcGeomCellPar(const char* name    = "RpcGeomCellPar",
		  const char* title   = "Geometry parameters of cells for the Rpc Detector",
		  const char* context = "RpcGeomCellProduction");
  ~HRpcGeomCellPar(void);
  HRpcGeomCellParSec& operator[](Int_t i) {
    return *static_cast<HRpcGeomCellParSec*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t, Int_t);
  ClassDef(HRpcGeomCellPar,1)  // Container for the RPC geometric parameters of cells
};

#endif /* !HRPCGEOMCELLPAR_H */
