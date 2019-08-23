#ifndef HRTMETAMATCHEFF_H
#define HRTMETAMATCHEFF_H

#include "hreconstructor.h"
#include "hrtmetacorrelatorsim.h"
#include "hrtmdctrksim.h"
#include "hrtmetacorrelator.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"

class TNtuple;
class HCategory;
class HIterator;
class HLocation;
class HMdcGeomPar;
class HSpecGeomPar;

class HRtMetaMatchEff : public HReconstructor {
 public:
  HRtMetaMatchEff(Text_t name[]="rt.mmeff",Text_t title[]="rt.mmeff");
  ~HRtMetaMatchEff(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
 protected:
  Float_t calcValidity(HRtMdcTrk *track, HRtMetaPoint &point);
  void fillControl(HRtMdcTrk *track, HRtMetaPoint &point);
  HRtMetaCorrelatorSim fCorrelator; //!
  HKickPlane2 *fKickplane;  //! 
  HKickMatchPar *fMatchpar;  //!
  TNtuple *fControl; //!
  HCategory *fMdcCat; //!
  HCategory *fKineCat; //!
  HIterator *fMdcIter; //!
  HLocation fHitLoc; //!
  HLocation fSecLoc; //!
  HMdcGeomPar *fMdcGeometry;
  HSpecGeomPar *fSpecGeometry;
  Float_t fResolPar[2][2]; //! pm3_pmeta resol. for shower, tof
 public:
  ClassDef(HRtMetaMatchEff,1)
};

#endif
