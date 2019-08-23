#ifndef HRTMDCTRACKIO_H
#define HRTMDCTRACKIO_H

#include "hrttrackio.h"
#include "hcategory.h"
#include "hspecgeompar.h"
#include "hmdcgeompar.h"

class HRtMdcTrackIo : public HRtTrackIo {
 public:
  HRtMdcTrackIo(Bool_t mode=kTRUE);
  ~HRtMdcTrackIo(void);
  virtual Bool_t init(HEvent *ev,HRuntimeDb *rtdb);
  Int_t readOuterSegments(TClonesArray &segArray);
  void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,HMdcSeg *innerSeg,
		   HRtSegmentRef *outerSeg,HRtTrackEvaluator *evaluator,
		   HRtFitter::EFitResult fitResult);
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
  TString fOutputClassName; //!
 public:
  ClassDef(HRtMdcTrackIo,1)
};

#endif
