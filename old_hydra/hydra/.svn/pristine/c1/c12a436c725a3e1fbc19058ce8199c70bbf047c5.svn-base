#ifndef HRTTRACKFIND_H
#define HRTTRACKFIND_H

#include "hreconstructor.h"
#include "hrtfitter.h"
#include "hlocation.h"
#include "hgeomvertexfit.h"
#include <TClonesArray.h>
#include <TNtuple.h>
#include "hrttrackevaluator.h"
#include "hrttrackio.h"

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
    
class HRtTrackFind : public HReconstructor {
public:
  enum EMode { Mdc3Pos, Mdc34, Meta };
  HRtTrackFind(Text_t name[], Text_t title[]);
  ~HRtTrackFind(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setMode(EMode mode) { fMode = mode; }
  void ignoreCuts(Bool_t f=kTRUE) { fIgnoreCuts = f; }
 protected:
  void extractMeasurements(HMdcSeg *inner, HRtSegmentRef *outer,HRtVector &m, 
			   HRtMatrix &w);
  virtual HRtTrackEvaluator *makeEvaluator(void);
  virtual HRtTrackIo *makeTrackIo(void);

  enum EMode fMode;
  Bool_t fIgnoreCuts;
  TNtuple *fControl;      // Control ntuple
  //HRtFitter fFitter;      // Fitter algorithm
  HCategory *fCatSeg;     //! Input category with segments
  HIterator *fInSegIter;  //! Iterator on inner segments
  HRtTrackingPar *fParam; //! Tracking parameters
  HLocation fLocInner; //!
  HDetector *fMdc;                //!Mdc detector
  TClonesArray fSegmentRefs;      //!Array of segment references
  HRtTrackEvaluator *fEvaluator;  //!Evaluates track candidates
  HRtTrackIo *fTrackIo;            //Input/Output for outer segments and tracks
 public:
  ClassDef(HRtTrackFind,2)  //Reference trajectories algorithm
};

#endif
