#ifndef HRTTRACKFIND_H
#define HRTTRACKFIND_H

#include "hreconstructor.h"
#include "hrtfitter.h"
#include "hlocation.h"
#include "hgeomvertexfit.h"
#include <TClonesArray.h>
#include <TNtuple.h>

class HCategory;
class HIterator;
class HKickPlane2;
class HRtTrackingPar;
class HMdcSeg;
class HRtSegmentRef;
class HDetector;
class HMdcGeomPar;
class HSpecGeomPar;

class HRtTrackEvaluator : public TObject {
public:
  HRtTrackEvaluator(void) : fKickPlane(0), fParam(0) {}
  ~HRtTrackEvaluator(void){}
  void setKickPlane(HKickPlane2 *k) { fKickPlane = k; }
  void setTrackingPar(HRtTrackingPar *p) { fParam = p; }
  void setSpecGeomPar(HSpecGeomPar *p) { fSpecGeomPar = p; }
  void evaluate(HMdcSeg *inner,HRtSegmentRef *outer,Bool_t onlyMdc3);
  Bool_t isGoodCombination(void);
  void fillTrackParameters(HRtVector &p);
  virtual void fillControl(TNtuple *control, Bool_t onlyMdc3);
  HGeomVector &getDir(void) { return fDir; }
  Float_t calcEvaluatedLength(void);
  Int_t getCharge() { return fPolarity; }
  Float_t getDistance(void) { return fDistance; }
  Float_t getDistanceKick(void) { return fDistanceKick; }
  Float_t getLeverArm(void) { return fLeverArm; }
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
  HRtTrackingPar *fParam; //!
  HSpecGeomPar *fSpecGeomPar; //!
  HGeomVertexFit fVertexFitter;      // Vertex fitter
  HGeomVector    fRClosest;
  HGeomVector    fDir; //! Track direction used for momentum calc.
public:
  ClassDef(HRtTrackEvaluator,1)
};

class HRtTrackFind : public HReconstructor {
public:
  HRtTrackFind(Text_t name[], Text_t title[]);
  ~HRtTrackFind(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setMdc3Mode(Bool_t mode=kFALSE) { fMdc3Mode = mode; }
  void setDisp(Float_t y,Float_t z) { 
    Warning("setDisp","Outer chamber displacement y=%.2f, z=%.2f",y,z);
    fDispY=y; fDispZ=z; 
  }
  void setMaxChi2(Float_t c) { fMaxChi2 = c; }
protected:
  Int_t readOuterSegments(void);
  Int_t readOuterHits(void);
  void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,
		   HMdcSeg *innerSeg,HRtSegmentRef *outerSeg);
  void extractMeasurements(HMdcSeg *inner, HRtSegmentRef *outer,
			   HRtVector &m, HRtMatrix &w);
  virtual HRtTrackEvaluator *makeEvaluator(void);

  Float_t fDispY,fDispZ;
  Float_t fMaxChi2; //Maximum chi2 of accepted input
  Bool_t fMdc3Mode;
  HMdcGeomPar *fMdcGeometry; //!
  HSpecGeomPar *fSpecGeometry; //!
  TNtuple *fControl;      // Control ntuple
  //HRtFitter fFitter;      // Fitter algorithm
  HCategory *fCatSeg;     //! Input category with segments
  HCategory *fCatHit;     //! Input category with hits
  HCategory *fCatTrk;     //! Output category with tracks
  HIterator *fInSegIter;  //! Iterator on inner segments
  HIterator *fOutSegIter; //! Iterator on outer segments
  HIterator *fOutHitIter; //! Iterator on outer hits
  HKickPlane2 *fKickPlane; //! Kickplane container
  HRtTrackingPar *fParam; //! Tracking parameters
  HLocation fLocInner, fLocOuter; //!
  HLocation fLocSector;           //! 
  HDetector *fMdc;                //!Mdc detector
  TClonesArray fSegmentRefs;      //!Array of segment references
  HRtTrackEvaluator *fEvaluator;  //!Evaluates track candidates
public:
  ClassDef(HRtTrackFind,1)  //Reference trajectories algorithm
};

#endif
