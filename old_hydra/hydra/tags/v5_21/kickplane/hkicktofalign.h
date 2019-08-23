#ifndef HKICKTOFALIGN_H
#define HKICKTOFALIGN_H

#include "hkickcandidateevaluator.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "hkickcandidate.h"
#include <TFile.h>
#include <TNtuple.h>

class HKickTofAlign : public HKickCandidateEvaluator {
 private:
  TFile *fOut;
  TNtuple *fControl;
public:
 HKickTofAlign(HKickPlane *k) : HKickCandidateEvaluator(k) {init();}
 ~HKickTofAlign(void) {finalize();}
 void evaluate(HKickImpact *imp,HTofHit *hit,HKickCandidate &out);
 Bool_t init(void);
 Bool_t finalize(void);
 ClassDef(HKickTofAlign,0)
};

#endif
