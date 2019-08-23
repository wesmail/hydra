#ifndef HKICKDETDIGI_H
#define HKICKDETDIGI_H

#include "hreconstructor.h"
#include "tofdef.h"
#include "hsymmat.h"
#include <TIterator.h>
#include <TF1.h>

class HCategory;
class HMdcGeomPar;
class HIterator;
class HGeantMdc;
class HGeomVector;
class HSpecGeomPar;

class HKickDetDigi : public HReconstructor {
protected:
  TF1 *probDistrX[4]; //!
  TF1 *probDistrY[4]; //!
  Float_t fMdcResolX[4];
  Float_t fMdcResolY[4];
  Float_t fMdcTailX[4];
  Float_t fMdcTailY[4];
  HSymMat4 fMdcSegCov; //!
  HLocation fMdcSectorLoc,fMdcSecModLoc;
  HCategory *fMdcInput;
  HCategory *fTofInput;
  HCategory *fMdcOut;
  HCategory *fMdcHitOut;
  HCategory *fCatKine;
  HMdcGeomPar *fGeometry; //Geometry transformation
  HSpecGeomPar *fSpecGeometry; //Spectrometer geometry
  HCategory *fTofOut;
  HIterator *fMdcIter;
  TIterator *fTofIter;
  Bool_t fFillTof; 
  Bool_t fIsTofPerfect;
  Bool_t fIsMdcPerfect;
  Bool_t fIsMdcAngErrConst; //Symmetric error in slope
  Bool_t fSimulateTails;
  Bool_t fFillMdc;
  Bool_t fFillMdcHits;
  Bool_t fFillSegments[2];  // iner, outer
  Bool_t fFillOuterSegments;
  Int_t fMomPos; //Where is momentum obtained from?

  Int_t digiMdc2(void);
  Int_t digiTof(void);
  void calcPosDir(HGeantMdc *data, HGeantMdc *data2,
	       Float_t x, Float_t y, Float_t x2, Float_t y2,
	       HGeomVector &r,HGeomVector &dir);
  HGeantMdc *findPartner(HGeantMdc *geMdc, Int_t sector,
			 Int_t mod, Int_t layer);
  void fillData(HGeomVector &r,HGeomVector &alpha,Double_t p,Int_t track);
  void fillHit(HGeantMdc *geMdc, Float_t x, Float_t y,Float_t p);
 public:
  HKickDetDigi(Text_t name[]="kick.digi",Text_t title[]="kick.digi");
  ~HKickDetDigi(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setTofFilling(Bool_t f) { fFillTof=f; }
  void setMdcFilling(Bool_t f=kTRUE) { fFillMdc = f; }
  Bool_t isFillingTof(void) { return fFillTof; }
  Bool_t isTofPerfect(void) { return fIsTofPerfect; }
  Bool_t isMdcPerfect(void) { return fIsMdcPerfect; }
  Bool_t isMdcAngErrConst(void) { return fIsMdcAngErrConst; }
  void setMdcAngErrConst(Bool_t f=kTRUE) { fIsMdcAngErrConst = f; }
  void setPerfectMdc(void) { fIsMdcPerfect = kTRUE; }
  void setPerfectTof(void) { fIsTofPerfect = kTRUE; }
  void setFillingSegments(Bool_t inner,Bool_t outer);
  void setFillingHits(Bool_t f) { fFillMdcHits = f; }
  void setMomPos(Int_t i) { fMomPos = i; }
  void setTails(Bool_t t) { fSimulateTails = t; }
  void setMdcResolution(Float_t x1,Float_t y1,Float_t x2,Float_t y2,
      Float_t x3,Float_t y3,Float_t x4,Float_t y4);
  void setMdcTails(Float_t x1,Float_t y1,Float_t x2,Float_t y2,
      Float_t x3,Float_t y3,Float_t x4,Float_t y4);
  ClassDef(HKickDetDigi,0)
};

inline void HKickDetDigi::setFillingSegments(Bool_t inner,Bool_t outer) {
  fFillSegments[0] = inner;
  fFillSegments[1] = outer;
}

#endif
