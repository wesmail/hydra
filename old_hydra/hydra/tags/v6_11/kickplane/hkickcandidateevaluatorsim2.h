#ifndef HKICKCANDIDATEEVALUATORSIM2_H
#define HKICKCANDIDATEEVALUATORSIM2_H

#include "hkickcandidateevaluatorsim.h"

class HKickCandidateEvaluatorSim2 : public HKickCandidateEvaluatorSim {
public:
  HKickCandidateEvaluatorSim2(HKickPlane2 *k, HKickMatchPar *p,HCategory *c) : 
    HKickCandidateEvaluatorSim(k,p,c) {}
  ~HKickCandidateEvaluatorSim2(void) {}
  void evaluate(HKickImpact *imp,HTofHit *hit,
		HKickCandidate &out);
  ClassDef(HKickCandidateEvaluatorSim2,0)
};

#endif
