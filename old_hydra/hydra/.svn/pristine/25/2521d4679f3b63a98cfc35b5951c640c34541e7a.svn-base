#include "hrttrackfindsim.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hrtsegmentref.h"
#include "hcategory.h"
#include "hgeantkine.h"
#include "hades.h"
#include "hevent.h"
#include "hgeantmdc.h"
#include <TNtuple.h>
#include <assert.h>

void HRtTrackEvaluatorSim::fillControl(TNtuple *control, Bool_t onlyMdc3) {
  Float_t valid;
  Int_t mostCommonTrack, mostCommonTrackTimes;
  Int_t totalTimes=0;
  HGeantKine *geKine = 0;

  if (onlyMdc3) {
    HMdcSegSim *innerSeg = dynamic_cast<HMdcSegSim *>(fInnerSeg);
    HMdcHitSim *outerSeg = dynamic_cast<HMdcHitSim *>(fOuterSeg->getSegment());

    if (innerSeg && outerSeg) {

      mostCommonTrackTimes = 0;
      totalTimes = 0;
      for (int i=0;i<innerSeg->getNTracks();i++) 
	for (int j=0;j<outerSeg->getNTracks();j++) 
	  if (innerSeg->getTrack(i) == outerSeg->getTrack(j))
	    if (innerSeg->getNTimes(i) + outerSeg->getNTimes(i)>mostCommonTrackTimes) {
	      mostCommonTrack = innerSeg->getTrack(i);
	      mostCommonTrackTimes = innerSeg->getNTimes(i) + 
		outerSeg->getNTimes(j);
	    }

      for (int i=0;i<innerSeg->getNTracks();i++)
	totalTimes += innerSeg->getNTimes(i);
      for (int i=0;i<outerSeg->getNTracks();i++)
	totalTimes += outerSeg->getNTimes(i);

      valid = float(mostCommonTrackTimes) / float(totalTimes);

      //Obtain info of resolution of outer segment
      HGeantMdc *geMdc = 0;
      Float_t geX,geY,geTheta,gePhi;
      Float_t reX,reY,reTheta,rePhi;
      Int_t track = outerSeg->getTrack(0);
      geKine = (HGeantKine *)fCatKine->getObject(track-1);
      geKine->resetMdcIter();
      while ( (geMdc = (HGeantMdc *)geKine->nextMdcHit()) != 0) {
	if (geMdc->getLayer() == 6 && geMdc->getModule()==2) {
	  geMdc->getHit(geX,geY,geTheta,gePhi); //theta phi are placeholders.
	  geMdc->getIncidence(geTheta,gePhi);
	  if (gePhi>180) gePhi -= 360.;
	}
      }
      reX = outerSeg->getX();
      reY = outerSeg->getY();
      reTheta = outerSeg->getTheta();
      rePhi = outerSeg->getPhi();

      control->Fill(fDistance,fDistanceKick,fAngle,fDeltaPhi,valid,fP,
		    outerSeg->getChi2(), //outerSeg->getMomentum(),
		    fRClosest.getX(),fRClosest.getY(),fRClosest.getZ());
      
      fOutRes->Fill(valid,fDistanceKick,geX,geY,geTheta,gePhi,reX,reY,reTheta,rePhi,geKine->getTotalMomentum());
    }
  } else {
    HMdcSegSim *outerSeg = dynamic_cast<HMdcSegSim *>(fOuterSeg->getSegment());
    HMdcSegSim *innerSeg = dynamic_cast<HMdcSegSim *>(fInnerSeg);
    
    Bool_t valid = (innerSeg->getTrack(0) == outerSeg->getTrack(0));
    
    control->Fill(fDistance,fDistanceKick,fAngle,fDeltaPhi,valid,fP,
		  0, //outerSeg->getMomentum(),
		  fRClosest.getX(),fRClosest.getY(),fRClosest.getZ());
  }
}

HRtTrackEvaluator *HRtTrackFindSim::makeEvaluator(void) {
  return new HRtTrackEvaluatorSim(fCatKine,fOutRes);
}

Bool_t HRtTrackFindSim::init(void) {
  fOutRes = new TNtuple("outres","outres","valid:dkick:gex:gey:gethe:gephi:rex:rey:rethe:rephi:p");
  fCatKine = gHades->getCurrentEvent()->getCategory(catGeantKine);
 
  if (!HRtTrackFind::init()) return kFALSE;

  return (fCatKine!=0);
}

Bool_t HRtTrackFindSim::finalize(void) {
  HRtTrackFind::finalize();
  if (fOutRes) {
    fOutRes->Write();
  }
  return kTRUE;
}


ClassImp(HRtTrackFindSim)
ClassImp(HRtTrackEvaluatorSim)
