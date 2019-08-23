#ifndef HRTMDCTRACKIOSIM_H
#define HRTMDCTRACKIOSIM_H

#include "hrtmdctrackio.h"

class HRtMdcTrackIoSim : public HRtMdcTrackIo {
 public:
  HRtMdcTrackIoSim(Bool_t mode) : HRtMdcTrackIo(mode) {  
    fOutputClassName = "HRtMdcTrkSim";
  }
  ~HRtMdcTrackIoSim(void) {}
  void acceptTrack(HRtVector &p,HRtMatrix &cov, Float_t chi2,HMdcSeg *innerSeg,
		   HRtSegmentRef *outerSeg, HRtTrackEvaluator *evaluator,
		   HRtFitter::EFitResult fitResult);
  ClassDef(HRtMdcTrackIoSim,1)
};

#endif
