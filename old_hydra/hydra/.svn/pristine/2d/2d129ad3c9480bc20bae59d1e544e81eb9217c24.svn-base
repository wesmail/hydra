#ifndef HKICKCANDIDATEEVALUATORSIM_H
#define HKICKCANDIDATEEVALUATORSIM_H

#include "hkickcandidateevaluator.h"

class HKickCandidateEvaluatorSim : public HKickCandidateEvaluator {

public:
  HKickCandidateEvaluatorSim(HKickPlane2 *k,HKickMatchPar *p) : 
    HKickCandidateEvaluator(k,p) {}
  ~HKickCandidateEvaluatorSim(void) {}
  void evaluate(HKickImpact *imp,HTofHit *hit,
		HKickCandidate &out);
  void evaluate(HKickImpact *imp,HShowerHit *hit,HKickCandidate &out);
  ClassDef(HKickCandidateEvaluatorSim,0)
};

#endif
