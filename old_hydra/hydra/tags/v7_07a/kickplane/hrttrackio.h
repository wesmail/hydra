#ifndef HRTTRACKIO_H
#define HRTTRACKIO_H

#include <TClonesArray.h>
#include "hrtmatrix.h"
#include "hmdcseg.h"
#include "hrtsegmentref.h"
#include "hrttrackevaluator.h"
#include "hlocation.h"
#include "hrtfitter.h"

class HRtTrackIo : public TObject {
 public:
  virtual Bool_t init(HEvent *ev,HRuntimeDb *rtdb) { return kTRUE; }
  virtual Int_t readOuterSegments(TClonesArray &segArray) = 0;
  virtual void acceptTrack(HRtVector &p,HRtMatrix &cov,Float_t chi2,
			   HMdcSeg *innerSeg,HRtSegmentRef *outerSeg,
			   HRtTrackEvaluator *evaluator,
			   HRtFitter::EFitResult fitResult=HRtFitter::kUnknown)=0;
  void setSector(Int_t s) { fLocOuter[0] = fLocSector[0] = s; } 
 protected:
  HLocation fLocOuter;    // Location for outer hits
  HLocation fLocSector;
 public:
  ClassDef(HRtTrackIo,1)
};


#endif
