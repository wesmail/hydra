#ifndef HRTMETACORRELATOR_H
#define HRTMETACORRELATOR_H

#include "TClonesArray.h"
#include "TFile.h"
#include "TNtuple.h"
#include "hgeomvector.h"
#include "hlocation.h"

class HCategory;
class HTofGeomPar;
class HIterator;
class HShowerGeometry;
class HSpecGeomPar;
class HTofHit;
class HShowerHitTof;
class HEvent;
class HRuntimeDb;

class HRtMetaPoint : public HGeomVector {
 protected:
  Float_t fPlaneSlope,fPlaneOffset;
  HGeomVector fErrors;
  Int_t fTimesUsed; // 
  Float_t fTof;
  Char_t fSystem;
  Int_t flag;
  TObject *fMetaHit; //!
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
  void setMetaHit(TObject *p) { fMetaHit = p; }
  void setSlope(Float_t s) { fPlaneSlope = s; }
  void setOffset(Float_t s) { fPlaneOffset = s; }
  Float_t getSlope(void) { return fPlaneSlope; }
  Float_t getOffset(void) { return fPlaneOffset; }
  TObject *getMetaHit(void) { return fMetaHit; }
  HGeomVector &getErrors(void) { return fErrors; }
  ClassDef(HRtMetaPoint,1) //A point in TOF or Shower
};

class HRtMetaCorrelator : public TObject {
 public:
  HRtMetaCorrelator(void);
  ~HRtMetaCorrelator(void) { delete fMetaPoints; }
  Bool_t init(HEvent *ev,HRuntimeDb *rtdb);
  Bool_t finalize(void);
  void readMetaPoints(void);
  void addPoint(HTofHit *data);
  void addPoint(HShowerHitTof *data);
  void setSector(Int_t s) { fSecLoc[0] = s; }
  Float_t getDistance(void) { return fLatestDistance; }
  Float_t getResX(void) { return fResX; }
  Float_t getResY(void) { return fResY; }
  HRtMetaPoint &createMetaPoint(void);
  HRtMetaPoint *findClosestPoint(HGeomVector &r,HGeomVector &alpha,
				 Float_t leverArm);
  
  Int_t getNMetaPoints(void) { return fMetaPoints->GetEntries(); }
  HRtMetaPoint &getMetaPoint(Int_t i) 
    { return *((HRtMetaPoint *)fMetaPoints->UncheckedAt(i)); }
 protected:
  virtual Bool_t getShowerCategory(HEvent *event);
  HCategory *fTofCat;//!
  HCategory *fShowerCat;//!
  TClonesArray *fMetaPoints; //! Array of HGeomVector
  HTofGeomPar *fTofGeometry;//!
  HShowerGeometry *fShowerGeometry;//!
  HSpecGeomPar *fSpecGeometry; //!Spectrometer geometry
  HIterator *fTofIter;//!
  HIterator *fShowerIter;//!
  HLocation fSecLoc;//!
  Float_t fMaxDist;
  Float_t fLatestDistance; //FIXME: Quick and dirty hack
  Float_t fResX,fResY;
  TFile *fOut; //!
  TNtuple *fControl;//!
 public:
  ClassDef(HRtMetaCorrelator,1)
};

#endif
