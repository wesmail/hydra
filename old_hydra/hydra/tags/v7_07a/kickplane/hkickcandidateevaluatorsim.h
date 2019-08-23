#ifndef HKICKCANDIDATEEVALUATORSIM_H
#define HKICKCANDIDATEEVALUATORSIM_H

#include "hkickcandidateevaluator.h"

class HCategory;

class HKickCandidateEvaluatorSim : public HKickCandidateEvaluator {

 protected:
  HCategory *fGeantCat; //!
public:
  HKickCandidateEvaluatorSim(HKickPlane2 *k,HKickMatchPar *p,HCategory *c) : 
    HKickCandidateEvaluator(k,p) { fGeantCat=c;}
  ~HKickCandidateEvaluatorSim(void) {}
  void evaluate(HKickImpact *imp,HTofHit *hit,
		HKickCandidate &out);
  void evaluate(HKickImpact *imp,HTofCluster *cluster,HKickCandidate &out);
  void evaluate(HKickImpact *imp,HShowerHit *hit,HKickCandidate &out);
  ClassDef(HKickCandidateEvaluatorSim,0)
};

#endif
