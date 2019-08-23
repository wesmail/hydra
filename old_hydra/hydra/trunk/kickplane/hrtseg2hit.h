#ifndef HRTSEG2HIT_H
#define HRTSEG2HIT_H

#include "TObject.h"
#include "hreconstructor.h"
#include "hlocation.h"
#include "hgeomvector.h"

class HCategory;
class HIterator;
class HMdcGeomPar;
class HMdcHit;
class HMdcSeg;

class HRtSeg2Hit : public HReconstructor {
public:
  HRtSeg2Hit(const Text_t name[],const Text_t title[]);
  ~HRtSeg2Hit(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void) { return kTRUE;}
  void setLeverArm(Int_t la) { fLeverArm = la; }
protected:
  HMdcHit *fillData(HMdcSeg *seg,HGeomVector &r,HGeomVector &alpha);
  HCategory *fSegCat; //! Input segment category
  HIterator *fSegIter; //! Iterator on seg category
  HCategory *fHitCat; //! Output hit category
  HMdcGeomPar *fMdcGeometry; //! MDC geometry
  Int_t fLeverArm;    //! Inner/Outer lever arm
  HLocation fHitLoc; //!
public:
  ClassDef(HRtSeg2Hit,1)
};

#endif
