#ifndef HRTMDCHIT2SEGSIM_H
#define HRTMDCHIT2SEGSIM_H

#include "hreconstructor.h"
#include "hmdchit.h"
#include "hgeomvector.h"
#include "hmdcsegsim.h"

class HCategory;
class HIterator;
class HMdcGeomPar;
class HSpecGeomPar;

class HRtMdcHit2SegSim : public HReconstructor {
private:
  HCategory *fInput; //!Input Hits
  HCategory *fOutput; //!Output segments
  HMdcGeomPar *fMdcGeo; //!MDC geometry
  HSpecGeomPar *fSpecGeometry; //!
  HIterator *fInputIter; //!Iterator on input data
  Bool_t fUseTarget;
  HLocation fLoc;
  Int_t fModule;
  void transform(HMdcHit *hit, HGeomVector &r,HGeomVector &alpha);
  void fillData(HMdcSegSim *mdcSeg,HGeomVector &r,HGeomVector &alpha);
public:
  HRtMdcHit2SegSim(Text_t name[],Text_t title[]);
  virtual ~HRtMdcHit2SegSim(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void useTarget(Bool_t f=kTRUE) { fUseTarget = f; }
  Bool_t usingTarget(void) { return fUseTarget; }
  void setModule(Int_t mod) {fModule = mod; }
  ClassDef(HRtMdcHit2SegSim,1) //Hit to Segment translator.
};

#endif
