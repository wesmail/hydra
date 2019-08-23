#ifndef HRTTRACKFINDSIM_H
#define HRTTRACKFINDSIM_H

#include "hrttrackfind.h"
#include <TNtuple.h>


class HRtTrackEvaluatorSim : public HRtTrackEvaluator {
 protected:
  HCategory *fCatKine; //!
  TNtuple *fOutRes; //!
public:
  HRtTrackEvaluatorSim(HCategory *c, TNtuple *n) {
    fCatKine = c;
    fOutRes = n;
  }
  ~HRtTrackEvaluatorSim(void) {}
  void fillControl(TNtuple *control, Bool_t onlyMdc3);
  ClassDef(HRtTrackEvaluatorSim,1)
};

class HRtTrackFindSim : public HRtTrackFind {
public:
  HRtTrackFindSim(Text_t name[]="tfsim", Text_t title[]="tfsim") : HRtTrackFind(name,title) {}
  ~HRtTrackFindSim(void) {}
  Bool_t init(void);
  Bool_t finalize(void);
protected:
  HRtTrackEvaluator *makeEvaluator(void);
  HRtTrackIo *makeTrackIo(void);
  HCategory *fCatKine; //!
  TNtuple *fOutRes; //!
public:
  ClassDef(HRtTrackFindSim,1)
};


#endif
