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
class HKickMatchPar;
class HRtTrackingPar;
class HMdcSeg;
class HRtSegmentRef;
class HDetector;
class HMdcGeomPar;
class HSpecGeomPar;
class HEvent;
class HRuntimeDb;

class HRtTrackEvaluator : public TObject {
public:
  HRtTrackEvaluator(void) : fKickPlane(0), fParam(0) {}
  ~HRtTrackEvaluator(void){}
  void setKickPlane(HKickPlane2 *k) { fKickPlane = k; }
  void setMatchPar(HKickMatchPar *p) { fMatchPar = p; }
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
  HKickMatchPar *fMatchPar; //!
  HRtTrackingPar *fParam; //!
  HSpecGeomPar *fSpecGeomPar; //!
  HGeomVertexFit fVertexFitter;      // Vertex fitter
  HGeomVector    fRClosest;
  HGeomVector    fDir; //! Track direction used for momentum calc.
public:
  ClassDef(HRtTrackEvaluator,1)
};

class HRtTrackIo : public TObject {
 public:
  virtual Bool_t init(HEvent *ev,HRuntimeDb *rtdb) { return kTRUE; }
  virtual Int_t readOuterSegments(TClonesArray &segArray) = 0;
  virtual void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,
			   HMdcSeg *innerSeg,HRtSegmentRef *outerSeg,
			   HRtTrackEvaluator *evaluator)=0;
  void setSector(Int_t s) { fLocOuter[0] = fLocSector[0] = s; } 
 protected:
  HLocation fLocOuter;    // Location for outer hits
  HLocation fLocSector;
 public:
  ClassDef(HRtTrackIo,1)
};

class HRtMdcTrackIo : public HRtTrackIo {
 public:
  HRtMdcTrackIo(Bool_t mode);
  ~HRtMdcTrackIo(void);
  virtual Bool_t init(HEvent *ev,HRuntimeDb *rtdb);
  Int_t readOuterSegments(TClonesArray &segArray);
  void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,HMdcSeg *innerSeg,
		   HRtSegmentRef *outerSeg,HRtTrackEvaluator *evaluator);
  void setDisp(Float_t y,Float_t z) { 
    Warning("setDisp","Outer chamber displacement y=%.2f, z=%.2f",y,z);
    fDispY=y; fDispZ=z; 
  }

  void setMaxChi2(Float_t f) { fMaxChi2 = f; }
 protected:
  Int_t readOuterHits(TClonesArray &segments);
  Int_t readOuterSegs(TClonesArray &segments);
  Float_t fDispY, fDispZ; //Outer chamber displacements
  HCategory *fCatSeg;     //! Input category with segments
  HCategory *fCatHit;     //! Input category with hits 
  HCategory *fCatTrk;     //! Output category with tracks
  HIterator *fOutSegIter; //! Iterator on outer segments
  HIterator *fOutHitIter; //! Iterator on outer hits
  HMdcGeomPar *fMdcGeometry; //!
  HSpecGeomPar *fSpecGeometry; //!
  Float_t fMaxChi2; //Maximum chi2 of accepted input
  Bool_t fMdc3Mode;
 public:
  ClassDef(HRtMdcTrackIo,1)
};
    
class HRtMetaTrackIo : public HRtTrackIo {
 public:
  HRtMetaTrackIo(void) {}
  ~HRtMetaTrackIo(void) {}
  virtual Bool_t init(HEvent *ev,HRuntimeDb *rtdb) {return kTRUE;}
  Int_t readOuterSegments(TClonesArray &segArray) {return 0;}
  void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,
		   HMdcSeg *innerSeg,HRtSegmentRef *outerSeg,
		   HRtTrackEvaluator *evaluator) {}
 protected:
 public:
  ClassDef(HRtMetaTrackIo,1)
};
    
class HRtTrackFind : public HReconstructor {
public:
  enum EMode { Mdc3Pos, Mdc34, Meta };
  HRtTrackFind(Text_t name[], Text_t title[]);
  ~HRtTrackFind(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setMode(enum EMode mode) { fMode = mode; }
 protected:
  void extractMeasurements(HMdcSeg *inner, HRtSegmentRef *outer,HRtVector &m, 
			   HRtMatrix &w);
  virtual HRtTrackEvaluator *makeEvaluator(void);

  enum EMode fMode;
  TNtuple *fControl;      // Control ntuple
  //HRtFitter fFitter;      // Fitter algorithm
  HCategory *fCatSeg;     //! Input category with segments
  HIterator *fInSegIter;  //! Iterator on inner segments
  HKickPlane2 *fKickPlane; //! Kickplane container
  HKickMatchPar *fMatchPar; //!
  HRtTrackingPar *fParam; //! Tracking parameters
  HLocation fLocInner; //!
  HDetector *fMdc;                //!Mdc detector
  TClonesArray fSegmentRefs;      //!Array of segment references
  HRtTrackEvaluator *fEvaluator;  //!Evaluates track candidates
  HRtTrackIo *fTrackIo;            //Input/Output for outer segments and tracks
  HSpecGeomPar *fSpecGeometry;
 public:
  ClassDef(HRtTrackFind,2)  //Reference trajectories algorithm
};

#endif
