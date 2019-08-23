#include "hkickcandidateevaluatorsim.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "htofhitsim2.h"
#include "hshowerhit.h"
#include "hkickcandidate.h"
#include "hmdcsegsim.h"
#include "hshowerhittoftrack.h"
#include "hcategory.h"
#include "hgeantkine.h"

void HKickCandidateEvaluatorSim::evaluate(HKickImpact *imp,
					  HTofHit *hit,
					  HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the TOF. It internaly uses the generic evaluation function but adds specific
  //code for TOF hits (mass calculation)

  HKickCandidateEvaluator::evaluate(imp,hit,out);

  //SIM
  HTofHitSim *hitsim = dynamic_cast<HTofHitSim *>(hit);
  HMdcSegSim *segsim = dynamic_cast<HMdcSegSim *>(imp->getOriginalSegment());
  HGeantKine *geTrack;
  Int_t parentTracks[2];

  if (hitsim != 0 && segsim != 0) {
    geTrack = (HGeantKine *)fGeantCat->getObject(hitsim->getNTrack1()-1);
    parentTracks[0] = geTrack->getParentTrack();
    if (hitsim->getNTrack2() != 0) {
      geTrack = (HGeantKine *)fGeantCat->getObject(hitsim->getNTrack2()-1);
      parentTracks[1] = geTrack->getParentTrack();
    } else
      parentTracks[1] = 0;

    out.setFlag(-1);
    for (int i=segsim->getNTracks()-1; i>0; i--) {
      if ( hitsim->getNTrack1() == segsim->getTrack(i) || 
	   hitsim->getNTrack2() == segsim->getTrack(i) ) {
	out.setFlag(i);
	break;
      } else if (parentTracks[0] == segsim->getTrack(i) ||
	       parentTracks[1] == segsim->getTrack(i) ) {
	out.setFlag(100 + i);
      }
    }
  } else Warning("evaluate","TOF data doesn't look like simulation");
}

void HKickCandidateEvaluatorSim::evaluate(HKickImpact *imp,HShowerHit *hit,
					  HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the Shower. It internaly uses the generic evaluation function.
  HKickCandidateEvaluator::evaluate(imp,hit,out);

  //SIM
  HShowerHitTofTrack *hitsim = dynamic_cast<HShowerHitTofTrack *>(hit);
  HMdcSegSim *segsim = dynamic_cast<HMdcSegSim *>(imp->getOriginalSegment());
  HGeantKine *geTrack;

  if ( hitsim != 0 && segsim != 0) {
    geTrack = (HGeantKine *)fGeantCat->getObject(hitsim->getTrack() - 1);
    out.setFlag(-1);
    for (int i=segsim->getNTracks()-1; i>=0; i--) {
      if ( hitsim->getTrack() == segsim->getTrack(i) ) {
	out.setFlag(i);
	break;
      } else if (geTrack->getParentTrack() == segsim->getTrack(i))
 	out.setFlag(100 + i);
    }
  } else Error("evaluate",
	     "Shower/Tofino data does not seem to come from simulation");
}

ClassImp(HKickCandidateEvaluatorSim)
