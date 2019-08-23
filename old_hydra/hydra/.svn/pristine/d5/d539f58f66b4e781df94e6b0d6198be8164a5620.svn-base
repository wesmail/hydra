#ifndef HKICKMATCH_H
#define HKICKMATCH_H

/* class HKickMatchCandidate : public TObject { */
/*  protected: */
/*   HKickMdcTrk *fMdcTrack; */
/*   HTofHit *fTofHit; */
/*   Float_t fDeltaX; */
/*   Float_t fDeltaY; */
/*  public: */
/*   HKickMatchCandidate(void); */
/*   ~HKickMatchCandidate(void); */
/*   HKickMdcTrk *getMdcTrack(void) { return fMdcTrack; } */
/*   HTofHit *getTofHit(void) { return fTofHit; } */
/*   Float_t getDeltaX(void) { return fDeltaX; } */
/*   Float_t getDeltaY(void) { return fDeltaY; } */
/*   void setDeltaX(Float_t dx) { fDeltaX = dx; } */
/*   void setDeltaY(Float_t dy) { fDeltaY = dy; } */
/*   void setMdcTrack(HKickMdcTrk *track) { fMdcTrack = track; } */
/*   void setTofHit(HTofHit *tof) { fTofHit = tof; } */
/*   ClassDef(HKickMatchCandidate,0) */
/* }; */

#include <TClonesArray.h>
#include <TFile.h>
#include <TNtuple.h>
#include "hgeomvector.h"
#include "hreconstructor.h"

class HCategory;
class HLocation;
class HTofGeomPar;
class HIterator;
class HShowerGeometry;
class HKickMdcTrk;
class HTofHit;
class HShowerHitTof;


class HKickMetaPoint : public HGeomVector {
 protected:
  Int_t fTimesUsed; // 
  Float_t fTof;
  Char_t fSystem;
  Int_t flag;
 public:
  HKickMetaPoint(void) { fTof = -1; fTimesUsed=0; }
  ~HKickMetaPoint(void) { }
  HKickMetaPoint(const HGeomVector &v) { 
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
  ClassDef(HKickMetaPoint,0) //A point in TOF or Shower
};

class HKickMatch : public HReconstructor {
 protected:
  HLocation fSecLoc;
  HCategory *fMdcCat;
  HCategory *fTofCat;
  HCategory *fShowerCat;
  HCategory *fTrackCat;
  HCategory *fStartCalCat;
  TClonesArray *fMetaPoints; // Array of HGeomVector
  HTofGeomPar *fTofGeometry;
  HShowerGeometry *fShowerGeometry;
  HIterator *fMdcIter;
  HIterator *fTofIter;
  HIterator *fShowerIter;
  Float_t fMaxDist;
  Float_t fLatestDistance; //FIXME: Quick and dirty hack
  void fillData(HKickMdcTrk *track, HKickMetaPoint *point);
  HKickMetaPoint *findClosestPoint(HKickMdcTrk *track);
  void readMetaPoints(void);
  void addPoint(HTofHit *data);
  void addPoint(HShowerHitTof *data);
  HKickMetaPoint &createMetaPoint(void);
  TFile *fOut;
  TNtuple *fControl;
  Float_t fTimeOffset;
 public:
  HKickMatch(Text_t name[], Text_t title[]);
  ~HKickMatch(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  ClassDef(HKickMatch,0) //Matches tracks in MDC and hits in TOF/Shower
};

#endif
