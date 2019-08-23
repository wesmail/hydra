#ifndef HMDCWIREFIT_H
#define HMDCWIREFIT_H

#include "TObject.h"
#include <math.h>

class HMdcWireFit : public TObject {
protected:
  // Address:
  Char_t sec;
  Char_t mod;
  Char_t layer; 
  Short_t cell;
  Char_t timeNum;   // =1 or 2 (time1 or time2)
  
  // Fit results:
  Float_t tdcTime;
  Float_t dev;
  Float_t weight;
  Float_t driftTime;
  Float_t fullTime;
  Float_t minDist;
  Float_t alpha;
  Bool_t isInCell;

public:
  HMdcWireFit() : sec(-1), mod(-1), layer(-1), cell(-1), timeNum(-1) {;}
  ~HMdcWireFit(){;}
  void clear() {
   sec=mod=layer=cell=timeNum=-1;
  }
// virtual Bool_t isGeant() {return kFALSE;}
  void setSec(Char_t s) {sec=s;}
  void setMod(Char_t m) {mod=m;}
  void setLayer(Char_t l) {layer=l;}
  void setCell(Short_t c) {cell=c;}
  void setAdsress(Char_t s,Char_t m,Char_t l,Short_t c) {
    sec=s;
    mod=m;
    layer=l;
    cell=c;
  }
  void setTimeNum(Char_t t) {timeNum=t;}
  void setDev(Float_t v) {dev=v;}
  void setWeight(Float_t v) {weight=v;}
  void setDriftTime(Float_t v) {driftTime=v;}
  void setFullTime(Float_t v) {fullTime=v;}
  void setTdcTime(Float_t v) {tdcTime=v;}
  void setMinDist(Float_t v) {minDist=v;}
  void setAlpha(Float_t v) {alpha=v;}
  void setIsInCell(Bool_t v) {isInCell=v;}
  
  Char_t getSec() const {return sec;}
  Char_t getMod() const {return mod;}
  Char_t getLayer() const {return layer;}
  Short_t getCell() const {return cell;}
  void getAddress(Int_t& s,Int_t& m,Int_t& l,Int_t& c) const {
    s=sec;
    m=mod;
    l=layer;
    c=cell;
  }
  Char_t getTimeNum() const {return timeNum;}
  Float_t getDev() const {return dev;}
  Float_t getWeight() const {return weight;}
  Float_t getDriftTime() const {return driftTime;}
  Float_t getFullTime() const {return fullTime;}
  Float_t getTdcTime() const {return tdcTime;}
  Float_t getMinDist() const {return minDist;}
  Float_t getAlpha() const {return alpha;}
  Bool_t getIsInCell() const {return isInCell;}

  void print();

  ClassDef(HMdcWireFit,1)
};

#endif
