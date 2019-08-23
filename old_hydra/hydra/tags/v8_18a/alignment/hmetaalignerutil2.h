#ifndef HALIGNER2_H
#define HALIGNER2_H

#include "hmetaalignerutil.h"
#include "hreconstructor.h"
#include "hshowerhit.h"
#include "htofhit.h"
#include "hmdcseg.h"
#include "hcategory.h"
#include "hshowergeometry.h"
#include "htofgeompar.h"
#include "hmdcgeompar.h"
#include "hiterator.h"
#include "hspecgeompar.h"
#include "hshowergeometry.h"
#include <TNtuple.h>

class HMetaAlignerTask2 : public HReconstructor {
public:
  HMetaAlignerTask2(void);
  HMetaAlignerTask2(Float_t fShowerDX, Float_t fShowerDY, Float_t fShowerDZ,
                Float_t fWindowSize);
  ~HMetaAlignerTask2(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Float_t distance(HGeomVector &r, HGeomVector &a, HGeomVector &p);
protected:
  HLocation fInnerOuterLoc;
  HCategory *fMdcHits; //!
  HCategory *fShowerHits; //!
  HCategory *fTofHits; //!
  HIterator *fMdcIter; //!
  HIterator *fShowerIter; //!
  HIterator *fTofIter; //!
  HMetaAlignerKickMdcTransform    fMdcGeom;
  HMetaAlignerKickTofTransform    fTofGeom;
  HMetaAlignerKickShowerTransform fShowerGeom;

  Float_t m_fShowerDX;
  Float_t m_fShowerDY;
  Float_t m_fShowerDZ;
  Float_t m_fWindowSize;


private:
  TNtuple *fData;
public:
  ClassDef(HMetaAlignerTask2,1)
};






#endif
