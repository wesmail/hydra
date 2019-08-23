#ifndef HRTMETAMATCH_H
#define HRTMETAMATCH_H

#include "hreconstructor.h"
#include "hmdcgeompar.h"
#include "hrtmetacorrelator.h"

class HCategory;
class HLocation;
class HIterator;
class HShowerGeometry;
class HSpecGeomPar;
//class HKickMdcTrk;
class HTofHit;
class HShowerHitTof;
class HRtMdcTrk;
class HKickPlane2;
class HKickMatchPar;
class HRtMatchingPar;

class HRtMetaMatch : public HReconstructor {
public:
  HRtMetaMatch(Text_t name[], Text_t title[]);
  ~HRtMetaMatch(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
protected:
  HRtMetaCorrelator fCorrelator;
  HLocation         fHitLoc;        //!
  HLocation         fSecLoc;        //!
  HCategory         *fMdcCat;       //!
  HIterator         *fMdcIter;      //!
  HCategory         *fTrackCat;     //!
  HMdcGeomPar       *fMdcGeometry;  //!
  HSpecGeomPar      *fSpecGeometry; //!
  HKickPlane2       *fKickplane;    //!
  HKickMatchPar     *fMatchpar;     //!
  HRtMatchingPar    *fMdcMetaMatchPar;

  void evaluate(HRtMdcTrk *track, HRtMetaPoint &point);
  void fillData(HRtMdcTrk *track, HRtMetaPoint &point,Float_t pmeta);
public:
  ClassDef(HRtMetaMatch,1) //Matches tracks in MDC and hits in TOF/Shower
};

#endif
