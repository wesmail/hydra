#ifndef HKICKDETDIGI_H
#define HKICKDETDIGI_H

#include "hreconstructor.h"
#include "tofdef.h"
#include <TIterator.h>

class HCategory;
class HMdcGeomPar;
class HIterator;
class HGeantMdc;
class HGeomVector;


class HKickDetDigi : public HReconstructor {
protected:
  HLocation fMdcSectorLoc,fMdcSecModLoc;
  HCategory *fMdcInput;
  HCategory *fTofInput;
  HCategory *fMdcOut;
  HCategory *fCatKine;
  HMdcGeomPar *fGeometry; //Geometry transformation
  HCategory *fTofOut;
  HIterator *fMdcIter;
  TIterator *fTofIter;
  Bool_t fFillTof; 
  Bool_t fIsTofPerfect;
  Bool_t fIsMdcPerfect;

  Int_t digiMdc2(void);
  Int_t digiTof(void);
  void calcPosDir(HGeantMdc *data, HGeantMdc *data2,
	       Float_t x, Float_t y, Float_t x2, Float_t y2,
	       HGeomVector &r,HGeomVector &dir);
  HGeantMdc *findPartner(HGeantMdc *geMdc, Int_t sector,
			 Int_t mod, Int_t layer);
  void fillData(HGeomVector &r,HGeomVector &alpha,Double_t p,Int_t track);
 public:
  HKickDetDigi(Text_t name[],Text_t title[]);
  ~HKickDetDigi(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setTofFilling(Bool_t f) { fFillTof=f; }
  Bool_t isFillingTof(void) { return fFillTof; }
  Bool_t isTofPerfect(void) { return fIsTofPerfect; }
  Bool_t isMdcPerfect(void) { return fIsMdcPerfect; }
  void setPerfectMdc(void) { fIsMdcPerfect = kTRUE; }
  void setPerfectTof(void) { fIsTofPerfect = kTRUE; }
  ClassDef(HKickDetDigi,0)
};

#endif
