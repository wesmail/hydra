#include "hkickcandidateevaluatorsim2.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "htofhitsim2.h"
#include "hshowerhit.h"
#include "hmdcsegsim.h"
#include "hkickcandidate.h"
#include "hshowerhittoftrack.h"
#include "hcategory.h"
#include "hgeantkine.h"

void HKickCandidateEvaluatorSim2::evaluate(HKickImpact *imp,
					   HTofHit *hit,
					   HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the TOF. It internaly uses the generic evaluation function but adds specific
  //code for TOF hits (mass calculation)

  HKickCandidateEvaluator::evaluate(imp,hit,out);

  //SIM
  HTofHitSim2 *hitsim = dynamic_cast<HTofHitSim2 *>(hit);
  HMdcSegSim *segsim = dynamic_cast<HMdcSegSim *>(imp->getOriginalSegment());
  Int_t parentTracks[2];
  HGeantKine *geTrack;

  if (hitsim != 0 && segsim != 0) {
    geTrack = (HGeantKine *)fGeantCat->getObject(hitsim->getTrackNb(0)-1);
    parentTracks[0] = geTrack->getParentTrack();
    if (hitsim->getNTrack() > 1) {
      geTrack = (HGeantKine *)fGeantCat->getObject(hitsim->getTrackNb(1)-1);
      parentTracks[1] = geTrack->getParentTrack();
    } else
      parentTracks[1] = 0;

    out.setFlag(-1);
    for (int i=segsim->getNTracks()-1; i>=0; i--) {
      if ( hitsim->containsTrack(segsim->getTrack(i)) ) {
	out.setFlag(i);
	break;
      } else if (parentTracks[0] == segsim->getTrack(i) ||
	       parentTracks[1] == segsim->getTrack(i) ) {
	out.setFlag(100 + i);
      }
    }
  } else Warning("evaluate","TOF data doesn't look like simulation");
}

ClassImp(HKickCandidateEvaluatorSim2)
