#if !defined HHISTOGRAM_H
#define HHISTOGRAM_H

//-------------------------------------------------------------------------
//
// File name:       hhistogram.h
// Subject:         Histogrammes.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the classes HDrawingOpt and
// HHistogram.
// The HHistogram class provides a set of functions to maintain
// the online objects of the type "histogram".
// The HDrawingOpt class is a service class for HHistogram which allows
// to set and store drawing options for the histogrammes.
//
//-------------------------------------------------------------------------

#include <TNamed.h>
#include <TH1.h>
#include <TH2.h>

class HUserParameter;
class TList;

class HDrawingOpt : public TObject {

 public:

  Float_t xstart;
  Float_t xstop;
  Float_t ystart;
  Float_t ystop;
  Float_t scale;
  Text_t  option[20];

 public:

  HDrawingOpt() : TObject() {}
  virtual ~HDrawingOpt() {}

 public:

  ClassDef(HDrawingOpt,1)

};


class HHistogram : public TNamed {

 protected:

  TH1F       *fHist1;       // 1d histogram
  TH2F       *fHist2;       // 2d histogram
  Int_t       fDim;         // number of dimensions
  Bool_t      kMulti;       // multi/non multi histogram
  Int_t       fCType;       // type of condition to which histogram is assigned
                            // (local or global)
  Int_t       fCIdx;        // condition index
  Text_t      fCName[100];  // condition name (consistency check)
  Int_t       fDSIdx1;      // main det set index (axis x)
  Text_t      fDSName1[100]; // main det set name (axis x)
  Int_t       fDSIdx2;      // second det set index (axis y)
  Text_t      fDSName2[100]; // second det set name (axis y)
  HUserParameter *fPar1;    // x-axis parameter
  HUserParameter *fPar2;    // y-axis parameter (if needed)
  Bool_t      kConsistent;  // consistency flag
  Bool_t      kPause;       // do not fill the histogram if true
  Bool_t      kLCount;      // if true, X axis parameter - condition counter
  Int_t       fLCountIdx;   // 1, 2 or 3 (count1, count2 or countS)
  Float_t     fTheta;
  Float_t     fPhi;
  HDrawingOpt fDrawing;
  Int_t       xcursor;
  Int_t       ycursor;

 public:

  HHistogram();
  virtual ~HHistogram();
  Bool_t checkConsistency(Bool_t kMsg = kFALSE);
  void fill();
  void action();
  TList* isSameDS(Int_t axis);
  void setDim(Int_t i) { fDim = i; }
  Int_t getDim() const { return fDim; }
  void setMulti(Bool_t kFlag = kTRUE) { kMulti = kFlag; }
  Bool_t isMulti() const { return kMulti; }
  void setCType(Int_t type) { fCType = type; } //0 means no condition
  Int_t getCType() const { return fCType; }
  Bool_t isConditioned() { return ((fCType>0) ? kTRUE : kFALSE); }
  void setPause(Bool_t kFlag = kTRUE) { kPause = kFlag; } 
  Bool_t isPause() const { return kPause; }
  void setCIdx(Int_t idx) { fCIdx = idx; }
  Int_t getCIdx() const { return fCIdx; }
  void setCName(const char* cname) { if(cname) sprintf(fCName,"%s",cname); }
  void setDSIdx(Int_t ds, Int_t pos) {
    if(pos==1) fDSIdx1 = ds;
    if(pos==2) fDSIdx2 = ds;
  }
  Int_t getDSIdx(Int_t pos) const { 
    if(pos==1) return fDSIdx1;
    if(pos==2) return fDSIdx2;
    return -1;
  }
  void setDSName(const char* name, Int_t pos) {
    if(name && pos==1) sprintf(fDSName1,"%s",name);
    if(name && pos==2) sprintf(fDSName2,"%s",name);
  }
  void setParameter(HUserParameter *pPar, Int_t idx) {
    if(pPar && idx==1) fPar1 = pPar;
    if(pPar && idx==2) fPar2 = pPar;
  }
  HUserParameter *getParameter(Int_t idx) const {
    if(idx==1) return fPar1;
    if(idx==2) return fPar2;
    return 0;
  }
  void setHist1(TH1F *hist);
  void setHist2(TH2F *hist);
  TH1F* getHist1() const { return fHist1; }
  TH2F* getHist2() const { return fHist2; }
  Bool_t isConsistent() const { return kConsistent; }
  void print();
  void setCount(Bool_t kFlag) { kLCount = kFlag; }
  Bool_t isCount() const { return kLCount; }
  void setCountIdx(Int_t idx) { if(idx>0 && idx<4) fLCountIdx = idx; }
  Int_t getCountIdx() const { return fLCountIdx; }
  void setTheta(Float_t t) { fTheta = t; }
  void setPhi(Float_t p) { fPhi = p; }
  Float_t getTheta() const { return fTheta; }
  Float_t getPhi() const { return fPhi; }
  void setDrawingOpt(HDrawingOpt *p, Bool_t kSave = kFALSE);
  void setDrawingOpt(Bool_t kDefault = kTRUE);
  void setCursors(Int_t x=1, Int_t y=1) { xcursor = x; ycursor = y; }
  void getCursors(Int_t &x, Int_t &y) { x = xcursor; y = ycursor; }

 public:

  ClassDef(HHistogram,1)

};

#endif






