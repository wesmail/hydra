#include "hkickcandidateevaluatorsim2.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "htofhitsim2.h"
#include "hshowerhit.h"
#include "hkickimpactsim.h"
#include "hkickcandidate.h"
#include "hshowerhittoftrack.h"
#include "hmdcsimseg.h"

void HKickCandidateEvaluatorSim2::evaluate(HKickImpact *imp,
					   HTofHit *hit,
					   HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the TOF. It internaly uses the generic evaluation function but adds specific
  //code for TOF hits (mass calculation)

  HKickCandidateEvaluator::evaluate(imp,hit,out);

  //SIM
  HTofHitSim2 *hitsim = dynamic_cast<HTofHitSim2 *>(hit);
  Int_t geTrack = ((HKickImpactSim *)imp)->getTrack();
  if (hitsim != 0) {
    if ( hitsim->containsTrack(geTrack) )
      out.setFlag(1);
    else
      out.setFlag(kFALSE);
  } else Warning("evaluate","TOF data doesn't look like simulation");
}

ClassImp(HKickCandidateEvaluatorSim2)
