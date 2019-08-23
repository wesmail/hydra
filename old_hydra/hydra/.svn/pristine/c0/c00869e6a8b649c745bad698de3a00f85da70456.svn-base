#ifndef HKICKDETDIGI_H
#define HKICKDETDIGI_H

#include "hreconstructor.h"
#include "tofdef.h"
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
  HKickDetDigi(Text_t name[],Text_t title[]);
  ~HKickDetDigi(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setTofFilling(Bool_t f) { fFillTof=f; }
  void setMdcFilling(Bool_t f=kTRUE) { fFillMdc = f; }
  Bool_t isFillingTof(void) { return fFillTof; }
  Bool_t isTofPerfect(void) { return fIsTofPerfect; }
  Bool_t isMdcPerfect(void) { return fIsMdcPerfect; }
  void setPerfectMdc(void) { fIsMdcPerfect = kTRUE; }
  void setPerfectTof(void) { fIsTofPerfect = kTRUE; }
  void setFillingSegments(Bool_t inner,Bool_t outer);
  void setFillingHits(Bool_t f) { fFillMdcHits = f; }
  void setMomPos(Int_t i) { fMomPos = i; }
  void setTails(Bool_t t) { fSimulateTails = t; }
  ClassDef(HKickDetDigi,0)
};

inline void HKickDetDigi::setFillingSegments(Bool_t inner,Bool_t outer) {
  fFillSegments[0] = inner;
  fFillSegments[1] = outer;
}

#endif
