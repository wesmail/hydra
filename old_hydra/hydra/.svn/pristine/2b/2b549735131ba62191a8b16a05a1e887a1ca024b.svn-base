#ifndef HRTMDCRES_H
#define HRTMDCRES_H

#include "hreconstructor.h"
#include "hgeomtransform.h"
#include "hlocation.h"
#include "TNtuple.h"

class HCategory;
class HIterator;
class HMdcGeomPar;
class HSpecGeomPar;

class HRtMdcRes : public HReconstructor {
 public:
  HRtMdcRes(const Text_t name[],const Text_t title[],Int_t sec=0,Int_t mod=2);
  ~HRtMdcRes(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Bool_t reinit(void);
 protected:
  HCategory *fCatSeg; //!
  HCategory *fCatHit;//!
  HIterator *fSegIter;//!
  HIterator *fHitIter;//!
  HMdcGeomPar *fMdcGeometry;//!
  HSpecGeomPar *fSpecGeometry;//!
  HGeomTransform fModTrans;//!
  HLocation fLoc,fSegLoc;//!
  TNtuple *fData;//!
 public:
  ClassDef(HRtMdcRes,1)
};

#endif
