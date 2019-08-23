#include "hrttrackfindsim.h"
#include "hmdcsimseg.h"
#include "hmdcsimhit.h"
#include "hrtsegmentref.h"
#include <assert.h>

void HRtTrackEvaluatorSim::fillControl(TNtuple *control, Bool_t onlyMdc3) {
  if (onlyMdc3) {
    HMdcSimSeg *innerSeg = dynamic_cast<HMdcSimSeg *>(fInnerSeg);
    HMdcSimHit *outerSeg = dynamic_cast<HMdcSimHit *>(fOuterSeg->getSegment());

    if (innerSeg && outerSeg) {
      Bool_t valid = (innerSeg->getTrack(0) == outerSeg->getTrack());
      
      control->Fill(fDistance,fDistanceKick,fAngle,fDeltaPhi,valid,fP,
		    outerSeg->getMomentum(),
		    fRClosest.getX(),fRClosest.getY(),fRClosest.getZ());    
    }
  } else {
    HMdcSimSeg *outerSeg = dynamic_cast<HMdcSimSeg *>(fOuterSeg->getSegment());
    HMdcSimSeg *innerSeg = dynamic_cast<HMdcSimSeg *>(fInnerSeg);
    
    Bool_t valid = (innerSeg->getTrack(0) == outerSeg->getTrack(0));
    
    control->Fill(fDistance,fDistanceKick,fAngle,fDeltaPhi,valid,fP,
		  outerSeg->getMomentum(),
		  fRClosest.getX(),fRClosest.getY(),fRClosest.getZ());
  }
}

HRtTrackEvaluator *HRtTrackFindSim::makeEvaluator(void) {
  return new HRtTrackEvaluatorSim;
}

ClassImp(HRtTrackFindSim)
ClassImp(HRtTrackEvaluatorSim)
