#ifndef HSPLINETASKSET_H
#define HSPLINETASKSET_H

#include "htaskset.h"
class HSplineTrackF;
class HMetaMatchF;
class HSplineTofClF;

class HSplineTaskSet:public HTaskSet
{
 public:
  HSplineTaskSet();
  HSplineTaskSet(Text_t name[], Text_t title[]);
  ~HSplineTaskSet();
  HTask* make(const char* sel="",Option_t* opt="");
 private:
  void parseArguments(TString);
  static Bool_t isSpline;
  static Bool_t isMetaMatching;
  static Bool_t   isClustering;
//  static Bool_t isSimulation;
  static HSplineTofClF* pSplineTofClF;
  static HMetaMatchF* pMetaMatchF;
  static HSplineTrackF* pSplineTrackF;
  //static HTofClusterF* pTofClusterF;
  ClassDef(HSplineTaskSet,1)
};
#endif  /* ! HSPLINETASKSET_H */
