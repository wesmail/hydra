#include "hrtmdctrackiosim.h"
#include "hrtmdctrksim.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"

void HRtMdcTrackIoSim::acceptTrack(HRtVector &p,HRtMatrix &cov,
				   Float_t chi2,HMdcSeg *innerSeg,
				   HRtSegmentRef *outerSeg,
				   HRtTrackEvaluator *evaluator) {
  HRtMdcTrkSim *out = 0;
  // Fills the track parameters in the output category and some statistics.
  TObject *slot = 0;
  int k=0;
  UChar_t totalTimes=0;

  slot = fCatTrk->getNewSlot (fLocSector);
  if (slot) {
    out = new (slot) HRtMdcTrkSim;
    //FIXME: The covariance matrix needs to be transformed from 1/p to p
    out->setSector( fLocSector[0] );
    out->setP (1 / p (0), cov (0, 0));
    out->setZ (p (1), cov (1, 1));
    out->setR (p (2), cov (2, 2));
    out->setPhi (p (3), cov (3, 3));
    out->setTheta (p (4), cov (4, 4));
    out->setOuterPos (outerSeg->getR());
    out->setOuterDir (evaluator->getDir());	//outerSeg->getAlpha());
    out->setLength (evaluator->calcEvaluatedLength());
    out->setCharge (evaluator->getCharge());
    out->setQuality (evaluator->getDistanceKick(),
		     evaluator->getDistance(),
		     evaluator->getDeltaPhi());
    out->setLeverArm (evaluator->getLeverArm());
  
    HMdcSegSim *seg = (HMdcSegSim *)innerSeg;

    HMdcHitSim *hit = dynamic_cast<HMdcHitSim *>(outerSeg->getSegment());
    HMdcSegSim *seg2= dynamic_cast<HMdcSegSim *>(outerSeg->getSegment());

    for (int i=0; i<seg->getNTracks(); i++) totalTimes += seg->getNTimes(i);

    if (hit) {
      for (int i=0; i<seg->getNTracks(); i++) {
        for (int j=0; j<hit->getNTracks(); j++) {
  	  if (seg->getTrack(i) == hit->getTrack(j)) { //Common track
	    out->setTrack(k, seg->getTrack(i));
	    out->setNTimes(k, seg->getNTimes(i) + hit->getNTimes(j));
	    k++;
	  }
        }
      }
      for (int i=0; i<hit->getNTracks(); i++) totalTimes += hit->getNTimes(i);
    } else if (seg2) {
      for (int i=0; i<seg->getNTracks(); i++) {
        for (int j=0; j<seg2->getNTracks(); j++) {
  	  if (seg->getTrack(i) == seg2->getTrack(j)) { //Common track
	    out->setTrack(k, seg->getTrack(i));
	    out->setNTimes(k, seg->getNTimes(i) + seg2->getNTimes(j));
	    k++;
	  }
        }
      }    
      for (int i=0; i<seg2->getNTracks(); i++) totalTimes += seg2->getNTimes(i);
    }

    out->setTotalTimes(totalTimes);
    out->setNTracks(k);
  } else Warning("acceptTrack","No free slot at sector %i, track lost",fLocSector[0]);
}

ClassImp(HRtMdcTrackIoSim)

