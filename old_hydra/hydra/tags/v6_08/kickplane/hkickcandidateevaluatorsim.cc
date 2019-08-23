#include "hkickcandidateevaluatorsim.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "htofhitsim2.h"
#include "hshowerhit.h"
#include "hkickimpactsim.h"
#include "hkickcandidate.h"
#include "hshowerhittoftrack.h"
#include "hmdcsimseg.h"

void HKickCandidateEvaluatorSim::evaluate(HKickImpact *imp,
					  HTofHit *hit,
					  HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the TOF. It internaly uses the generic evaluation function but adds specific
  //code for TOF hits (mass calculation)

  HKickCandidateEvaluator::evaluate(imp,hit,out);

  //SIM
  HTofHitSim *hitsim = dynamic_cast<HTofHitSim *>(hit);
  Int_t geTrack = ((HKickImpactSim *)imp)->getTrack();
  if (hitsim != 0) {
    if ( hitsim->getNTrack1() == geTrack || hitsim->getNTrack2() == geTrack )
      out.setFlag(1);
    else
      out.setFlag(kFALSE);
  } else Warning("evaluate","TOF data doesn't look like simulation");
}

void HKickCandidateEvaluatorSim::evaluate(HKickImpact *imp,HShowerHit *hit,
					  HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the Shower. It internaly uses the generic evaluation function.
  HKickCandidateEvaluator::evaluate(imp,hit,out);

  //SIM
  HShowerHitTofTrack *hitsim = dynamic_cast<HShowerHitTofTrack *>(hit);
  if ( hitsim != 0 )
    if ( hitsim->getTrack() == ((HKickImpactSim *)imp)->getTrack() )
      out.setFlag(2);
    else
      out.setFlag(kFALSE);
  else Error("evaluate",
	     "Shower/Tofino data does not seem to come from simulation");
}

ClassImp(HKickCandidateEvaluatorSim)
