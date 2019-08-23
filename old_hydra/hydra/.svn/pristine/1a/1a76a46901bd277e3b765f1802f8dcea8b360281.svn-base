#ifndef HRTTRACKFINDSIM_H
#define HRTTRACKFINDSIM_H

#include "hrttrackfind.h"

class HRtTrackEvaluatorSim : public HRtTrackEvaluator {
public:
  HRtTrackEvaluatorSim(void) {}
  ~HRtTrackEvaluatorSim(void) {}
  void fillControl(TNtuple *control, Bool_t onlyMdc3);
  ClassDef(HRtTrackEvaluatorSim,1)
};

class HRtTrackFindSim : public HRtTrackFind {
public:
  HRtTrackFindSim(Text_t name[], Text_t title[]) : HRtTrackFind(name,title) {}
  ~HRtTrackFindSim(void) {}
protected:
  HRtTrackEvaluator *makeEvaluator(void);
public:
  ClassDef(HRtTrackFindSim,1)
};


#endif
