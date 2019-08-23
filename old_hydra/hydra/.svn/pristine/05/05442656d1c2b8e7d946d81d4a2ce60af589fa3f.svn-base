#ifndef HKICKMOMFITD_H
#define HKICKMOMFITD_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hkickplane.h"
#include "hspecgeompar.h"
#include "hiterator.h"
#include "hgeomvector.h"
#include "hkickplane.h"
#include "hmdctrackddef.h"

class HMdcClus;


class HKickMomFitD : public HReconstructor {
 protected:
  HCategory *fClusterCat;
  HCategory *fKickMdcTrkCat;
  HIterator *fClusterIter;
  HLocation fLaLocation;
  HLocation fSecLoc;
  HKickPlane *fKickPlane;
  HSpecGeomPar *fSpecGeometry;
  Double_t fMaxPull;
  void fillData(const HGeomVector &zero,HGeomVector &kick,
		HGeomVector &far, Double_t p, Double_t pull,
		Int_t pol,HMdcClus *);

 public:
  HKickMomFitD(Text_t name[],Text_t title[]);
  ~HKickMomFitD(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  ClassDef(HKickMomFitD,0)
};


#endif
