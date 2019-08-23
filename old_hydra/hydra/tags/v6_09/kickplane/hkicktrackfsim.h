#ifndef HKICKTRACKFSIM_H
#define HKICKTRACKFSIM_H

#include "hkicktrackf.h"
#include <TClonesArray.h>
#include <TNtuple.h>
#include "hcategory.h"

class HKickTrackFSim : public HKickTrackF {
protected:
  HCategory *fKineCat;
  TNtuple *fData,*fControl;
  virtual void fillControl(HKickCandidate &candidate, HKickImpact *imp);
  virtual HKickTrack *fillData(HKickCandidate &candidate,HKickImpact *imp);
  HKickCandidateEvaluator *makeEvaluator(void);
  HKickTrack *makeTrack(void);
  HCategory *makeOutputCategory(HEvent *event);
public:
  HKickTrackFSim(Text_t name[],Text_t title[]);
  ~HKickTrackFSim(void) { }
  Bool_t isTofSimulated(void) { return fEvaluator->isTofSimulated(); }
  void setSimulatedTof(void) { fEvaluator->setSimulatedTof(); }
  Bool_t init(void);
  Bool_t finalize(void);
  ClassDef(HKickTrackFSim,0)
};

#endif
