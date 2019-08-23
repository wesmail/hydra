#ifndef HRTM3METAMATCH_H
#define HRTM3METAMATCH_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hrtmetacorrelator.h"
#include "hrtmetaseg.h"

class HMdcGeomPar;
class HSpecGeomPar;
class HCategory;
class HGeomTransform;
class HMdcHit;
class HGeomVector;
class HRtMetaPoint;

class HRtM3MetaMatch : public HReconstructor {
 public:
  HRtM3MetaMatch(Text_t name[], Text_t title[]);
  ~HRtM3MetaMatch(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t execute(void);
 protected:
  virtual HRtMetaSeg *fillData(HMdcHit *hit, HGeomVector &pos, HGeomVector &alpha,
			HRtMetaPoint *point);
  virtual HRtMetaSeg *makeSegment(TObject *s);
  virtual HCategory *makeOutputCategory(HEvent *ev);
  virtual void fillControl(HMdcHit *h, HRtMetaPoint &p,Float_t dx,Float_t dy);
			   
  HMdcGeomPar *fMdcGeometry; //!
  HSpecGeomPar *fSpecGeometry; //!
  HCategory *fCatMdc; //!
  HIterator *fMdcIter; //!
  HCategory *fCatSeg; //!
  HLocation fHitLoc; //!
  HLocation fSecLoc; //!
  HRtMetaCorrelator fCorrelator; //!
  HGeomTransform *fMdc2SecTrans[6]; //!
  Int_t fModule; //MDC Module to be matched with Meta
 public:
  ClassDef(HRtM3MetaMatch,1)
};

#endif
