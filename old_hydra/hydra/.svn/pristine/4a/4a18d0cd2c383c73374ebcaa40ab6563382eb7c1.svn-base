#ifndef HKICKTRACKFBSIM_H
#define HKICKTRACKFBSIM_H

#include "hkicktrackfb.h"
#include "TClonesArray.h"
#include "TNtuple.h"
#include "hcategory.h"
//modified by A.Sadovski to adapt for HBaseTrack output

class HKickTrackFBSim : public HKickTrackFB {
protected:
  HCategory *fKineCat;
  TNtuple *fData,*fControl;
  virtual void fillControl(HKickCandidate &candidate, HKickImpact *imp);
  virtual HKickTrackB *fillData(HKickCandidate &candidate,HKickImpact *imp);
  virtual HKickTrack *fillDataInOldCat(HKickCandidate &candidate,HKickImpact *imp);
  virtual void readMetaHits(HLocation &);
  HKickCandidateEvaluator *makeEvaluator(void);
  HKickTrackB *makeTrack(void);
  HKickTrack *makeOldTrack(void);
  HCategory *makeOutputCategory(HEvent *event);
  HCategory *makeOldOutputCategory(HEvent *event);
  Bool_t fUseSimulatedTof;
public:
  HKickTrackFBSim(void) {}
  HKickTrackFBSim(const Text_t name[],const Text_t title[]);
  ~HKickTrackFBSim(void) { }
  Bool_t isTofSimulated(void) { return fEvaluator->isTofSimulated(); }
  void setSimulatedTof(void) { 
    fUseSimulatedTof = kTRUE; 
    if (fEvaluator) fEvaluator->setSimulatedTof();
  }
  Bool_t init(void);
  Bool_t finalize(void);
  ClassDef(HKickTrackFBSim,0)
};

#endif
