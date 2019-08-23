#ifndef HRTTRACKEVALUATOR_H
#define HRTTRACKEVALUATOR_H

#include "hreconstructor.h"
#include "hrtfitter.h"
#include "hlocation.h"
#include "hgeomvertexfit.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "TClonesArray.h"
#include "TNtuple.h"

class HCategory;
class HIterator;
class HKickPlane2;
class HKickMatchPar;
class HRtTrackingPar;
class HMdcSeg;
class HRtSegmentRef;
class HDetector;
class HMdcGeomPar;
class HSpecGeomPar;
class HEvent;
class HRuntimeDb;
class HRtMatchingPar;
class HGeomTransform;

class HRtTrackEvaluator : public TObject {
public:
  HRtTrackEvaluator(void) : fKickPlane(0) {}
  ~HRtTrackEvaluator(void){}
  Bool_t init(void);
  Bool_t reinit(void);
  void evaluate(Bool_t onlyMdc3);
  Bool_t isGoodCombination(void);
  void fillTrackParameters(HRtVector &p);
  virtual void fillControl(TNtuple *control, Bool_t onlyMdc3);
  HGeomVector &getDir(void) { return fDir; }
  Float_t calcEvaluatedLength(void);
  Int_t getCharge() { return fPolarity; }
  Float_t getDistance(void) { return fDistance; }
  Float_t getDistanceKick(void) { return fDistanceKick; }
  Float_t getDeltaPhi(void) { return fDeltaPhi; }
  Float_t getLeverArm(void) { return fLeverArm; }
  void setInnerSeg(HMdcSeg *seg);
  void setOuterSeg(HRtSegmentRef *seg);
  void extractMeasurements(HRtVector &m,HRtMatrix &W);
protected:
  HMdcSeg *fInnerSeg; //!
  HRtSegmentRef *fOuterSeg;  //!
  Float_t fDistance;
  Float_t fLeverArm;
  Float_t fDistanceKick;
  Float_t fDeltaPhi;   // Diff. between predicted and measuremed phi deflection
  Float_t fP;          // Estimated momentum
  Float_t fAngle;      // Deflection angle
  Int_t fPolarity;     // Polarity of particle
  HKickPlane2 *fKickPlane; //!FIXME: fill from track finder
  HKickMatchPar *fMatchPar; //!
  HRtMatchingPar *fMdcMatchingPar; //!
  HSpecGeomPar *fSpecGeomPar; //!
  HMdcGeomPar  *fMdcGeom;     //!
  HGeomTransform fMdcModule[4]; //!
  HGeomVertexFit fVertexFitter;      // Vertex fitter
  HGeomVector    fRClosest;
  HGeomVector    fRKick1; //Intersection of inner track with kickplane
  HGeomVector    fDir; //! Track direction used for momentum calc.
  HGeomVector    fInnerPos;
  HGeomVector    fInnerAlpha;
  void computeDistance(Float_t &dist, Float_t &errDist);
public:
  ClassDef(HRtTrackEvaluator,1)
};

#endif
