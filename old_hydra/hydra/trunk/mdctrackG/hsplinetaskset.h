#ifndef HSPLINETASKSET_H
#define HSPLINETASKSET_H

#include "htaskset.h"
class HSplineTrackF;
class HMetaMatchF;
class HSplineTofClF;
class HKickTrackFB;
class HRKTrackBF;
class HSplineTaskSet:public HTaskSet
{
 public:
  HSplineTaskSet();
  HSplineTaskSet(const Text_t name[],const Text_t title[]);
  ~HSplineTaskSet();
  HTask* make(const Char_t* sel="",const Option_t* opt="");
  
  void setSplinePar();
  
 private:
  void parseArguments(TString);
  static Bool_t isSpline;
  static Bool_t isMetaMatching;
  static Bool_t isClustering;
  static Bool_t isSimulation;
  static Bool_t isKickB;
  static Bool_t isAlsoOld;
  static Bool_t isDebug;
  static Bool_t isKickCluster;
  static Bool_t isControlHists;
  static Bool_t isMultipleTarget;
  static Bool_t isRunge;  

  static HSplineTofClF* pSplineTofClF;
  static HMetaMatchF* pMetaMatchF;
  static HSplineTrackF* pSplineTrackF;
  static HKickTrackFB*  pKickB;
  static HRKTrackBF*    pRunge;
  //static HTofClusterF* pTofClusterF;
  Bool_t isSplinePar;
  
  ClassDef(HSplineTaskSet,1)
};
#endif  /* ! HSPLINETASKSET_H */
