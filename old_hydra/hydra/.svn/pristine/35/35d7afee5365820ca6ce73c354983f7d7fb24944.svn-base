#ifndef HKICKTRACKFSIM_H
#define HKICKTRACKFSIM_H

#include "hkicktrackf.h"
#include "TClonesArray.h"
#include "TNtuple.h"
#include "hcategory.h"

class HKickTrackFSim : public HKickTrackF {
protected:
  HCategory *fKineCat;
  TNtuple *fData,*fControl;
  virtual void fillControl(HKickCandidate &candidate, HKickImpact *imp);
  virtual HKickTrack *fillData(HKickCandidate &candidate,HKickImpact *imp);
  virtual void readMetaHits(HLocation &);
  HKickCandidateEvaluator *makeEvaluator(void);
  HKickTrack *makeTrack(void);
  HCategory *makeOutputCategory(HEvent *event);
  Bool_t fUseSimulatedTof;
public:
  HKickTrackFSim(const Text_t name[],const Text_t title[]);
  ~HKickTrackFSim(void) { }
  Bool_t isTofSimulated(void) { return fEvaluator->isTofSimulated(); }
  void setSimulatedTof(void) { 
    fUseSimulatedTof = kTRUE; 
    if (fEvaluator) fEvaluator->setSimulatedTof();
  }
  Bool_t init(void);
  Bool_t finalize(void);
  ClassDef(HKickTrackFSim,0)
};

#endif
