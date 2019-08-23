#ifndef HMDCCLUSFIT_H
#define HMDCCLUSFIT_H

#include "TObject.h"
#include <math.h>

class HMdcClusFit : public TObject {
protected:
  // Address:
  Char_t sec;
  Char_t seg;
  Char_t mod;          // =0,1,2,3 - module; =-1 - segment fit
  // HMdc12Fit param.: 
  Char_t fitAuthor;   // 0 - Hejdar's fit, 1 - Alexander's fit
  Char_t distTimeVer; // 0-calc. time by polinom., 1-by J.Market's func.
  Char_t fitType;     // 0 - segment fit, 1 - mdc fit 
  Char_t nParam;      // number of parameters
  
  // Fit results:
  Float_t functional;  // 
  Float_t timeOff;     //
  Short_t numOfWires;  // Num. of wires with weight > weight_min
  Char_t numOfLayers;  // Num. of layers with wire weight > weight_min
  Float_t x1;          // Track parameters:
  Float_t y1;          // Track is line (x1,y1,z1) - (x2,y2,z2)
  Float_t z1;          // in sector coor. system.
  Float_t x2;          //
  Float_t y2;          //
  Float_t z2;          //
  Short_t numIter;     // number of iterations
  
  Int_t indf;          // index of ???
  Int_t indl;          // index of ???
  
  Int_t clustIndex;    // cluster address: [sec],[seg],[clustIndex]

public:
  HMdcClusFit() : sec(-1), seg(-1), mod(-1), indf(-1), indl(-1) {;}
  ~HMdcClusFit(){;}
  void clear() {
   indf=indl=-1;
   sec=seg=mod=-1;
  }
//   virtual Bool_t isGeant() {return kFALSE;}
  void setSec(Char_t sc) {sec=sc;}
  void setIOSeg(Char_t sg) {seg=sg;}
  void setMod(Char_t m) {mod=m;}
  void setAddress(Char_t sc, Char_t sg, Char_t md) {sec=sc; seg=sg; mod=md;}
  void setFitAuthor(Char_t v) {fitAuthor=v;}
  void setDistTimeVer(Char_t v) {distTimeVer=v;}
  void setFitType(Char_t v) {fitType=v;}
  void setNParam(Char_t v) {nParam=v;}
  void setFunMin(Float_t v) {functional=v;}
  void setTimeOff(Float_t v) {timeOff=v;}
  void setNumOfWires(Int_t v) {numOfWires=v;}
  void setNumOfLayers(Int_t v) {numOfLayers=v;}
  void setX1(Float_t v) {x1=v;}
  void setY1(Float_t v) {y1=v;}
  void setZ1(Float_t v) {z1=v;}
  void setX2(Float_t v) {x2=v;}
  void setY2(Float_t v) {y2=v;}
  void setZ2(Float_t v) {z2=v;}
  void setNumIter(Short_t v) {numIter=v;}
  void setIndf(Int_t v) {indf=v;}
  void setIndl(Int_t v) {indl=v;}
  void setClustIndex(Int_t v) {clustIndex=v;}
  
  Char_t getSec(void) const {return sec;}
  Char_t getIOSeg(void) const {return seg;}
  Char_t getMod(void) const {return mod;}
  Char_t getFitAuthor(void) const {return fitAuthor;}
  Char_t getDistTimeVer(void) const {return distTimeVer;}
  Char_t getFitType(void) const {return fitType;}
  Char_t getNParam(Char_t v) const {return nParam;}
  Float_t getFunMin(void) const {return functional;}
  Float_t getTimeOff(void) const {return timeOff;}
  Short_t getNumOfWires(void) const {return numOfWires;}
  Char_t getNumOfLayers(void) const {return numOfLayers;}
  Float_t getX1(void) const {return x1;}
  Float_t getY1(void) const {return y1;}
  Float_t getZ1(void) const {return z1;}
  Float_t getX2(void) const {return x2;}
  Float_t getY2(void) const {return y2;}
  Float_t getZ2(void) const {return z2;}
  Short_t getNumIter(void) const {return numIter;}
  Int_t getIndf(void) const {return indf;}
  Int_t getIndl(void) const {return indl;}
  void getIndexis(Int_t &indexf,Int_t &indexl) const {
    indexf=indf;
    indexl=indl;
  }
  void getAddress(Int_t& sc, Int_t& sg, Int_t& md) const {
    sc=sec;
    sg=seg; 
    md=mod;
  }
  
  Int_t getClustIndex(void) {return clustIndex;}
  void getClustAddress(Int_t& sc, Int_t& sg, Int_t& ind) const {
    sc=sec;
    sg=seg; 
    ind=clustIndex;
  }
  

  Float_t getTheta(void) { 
    return acos((z2-z1)/sqrt((z2-z1)*(z2-z1)+(x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
  }
  Float_t getPhi(void) {return atan2(y2-y1,x2-x1);}
  Int_t getNextIndex(Int_t ind) {  //using: ind=-1;ind=getNextIndexChild(ind)
    return (ind<indf) ? indf:((ind<indl) ? ind+1:-1);
  }

  void print();

  ClassDef(HMdcClusFit,1)
};

#endif
