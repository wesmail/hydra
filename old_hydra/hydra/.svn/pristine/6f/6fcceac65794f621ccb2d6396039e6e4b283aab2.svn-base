#ifndef HRTMETAMATCH_H
#define HRTMETAMATCH_H

#include <TClonesArray.h>
#include <TFile.h>
#include <TNtuple.h>
#include "hgeomvector.h"
#include "hreconstructor.h"
#include "hmdcgeompar.h"

class HCategory;
class HLocation;
class HTofGeomPar;
class HIterator;
class HShowerGeometry;
class HSpecGeomPar;
//class HKickMdcTrk;
class HTofHit;
class HShowerHitTof;
class HRtMdcTrk;
class HKickPlane2;

class HRtMetaPoint : public HGeomVector {
 protected:
  Int_t fTimesUsed; // 
  Float_t fTof;
  Char_t fSystem;
  Int_t flag;
 public:
  HRtMetaPoint(void) { fTof = -1; fTimesUsed=0; }
  ~HRtMetaPoint(void) { }
  HRtMetaPoint(const HGeomVector &v) { 
    x=v.getX(); y=v.getX(); z=v.getZ();
  }
  Int_t getFlag(void) { return flag; }
  void setFlag(Int_t f) { f = flag; }
  Int_t getNTimesUsed(void) { return fTimesUsed; }
  Float_t getTof(void) { return fTof; }
  void incUsedTimes(void) { fTimesUsed++; }
  void setTof(Float_t f) { fTof = f; }
  Char_t getSystem(void) { return fSystem; }
  void setSystem(Char_t s) { fSystem = s; }
  ClassDef(HRtMetaPoint,0) //A point in TOF or Shower
};

class HRtMetaMatch : public HReconstructor {
 protected:
  HLocation fSecLoc;//!
  HLocation fHitLoc;//!
  HCategory *fMdcCat; //!
  HCategory *fTofCat;//!
  HCategory *fShowerCat;//!
  HCategory *fTrackCat;//!
  TClonesArray *fMetaPoints; //! Array of HGeomVector
  HTofGeomPar *fTofGeometry;//!
  HShowerGeometry *fShowerGeometry;//!
  HSpecGeomPar *fSpecGeometry; //!Spectrometer geometry
  HMdcGeomPar *fMdcGeometry;//!
  HIterator *fMdcIter;//!
  HIterator *fTofIter;//!
  HIterator *fShowerIter;//!
  HKickPlane2 *fKickplane; //!
  Float_t fMaxDist;
  Float_t fLatestDistance; //FIXME: Quick and dirty hack
  Float_t fResX,fResY;

  void fillData(HRtMdcTrk *track, HRtMetaPoint *point);
  HRtMetaPoint *findClosestPoint(HGeomVector &r,HGeomVector &alpha,
				 Float_t leverArm);
  void readMetaPoints(void);
  void addPoint(HTofHit *data);
  void addPoint(HShowerHitTof *data);
  HRtMetaPoint &createMetaPoint(void);
  Float_t calcMomentum(HRtMdcTrk *track,HRtMetaPoint *point);
  TFile *fOut; //!
  TNtuple *fControl;//!
 public:
  HRtMetaMatch(Text_t name[], Text_t title[]);
  ~HRtMetaMatch(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  ClassDef(HRtMetaMatch,1) //Matches tracks in MDC and hits in TOF/Shower
};

#endif
