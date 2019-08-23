#ifndef HALIGNER_H
#define HALIGNER_H

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
#include "TNtuple.h"


class HMetaAlignerKickShowerTransform : public TObject {
public:
  HMetaAlignerKickShowerTransform(void);
  HMetaAlignerKickShowerTransform(Float_t showerDX, Float_t showerDY,Float_t showerDZ);
  ~HMetaAlignerKickShowerTransform(void);
  void setGeometry(HShowerGeometry *geom);
  void transform(HShowerHit *hit, HGeomVector &r) const;
  void setTranslation(Float_t showerDX, Float_t showerDY,Float_t showerDZ){
    fShowerDX = showerDX;
    fShowerDX = showerDY;
    fShowerDX = showerDY;
  }
  void toLocal(HGeomVector &r,Int_t sector,HGeomVector &out) const;
  const HGeomTransform &getTransform(Int_t sector) const;
  Bool_t init(void);
  void getSigmas(HShowerHit *hit, Float_t &fSigmaX, Float_t &fSigmaY) const;
protected:
  Float_t fShowerDX,fShowerDY,fShowerDZ;
  HShowerGeometry *fGeometry;
public:
  ClassDef(HMetaAlignerKickShowerTransform,1)
};

class HMetaAlignerKickTofTransform : public TObject {
public:
  HMetaAlignerKickTofTransform(void);
  ~HMetaAlignerKickTofTransform(void);
  void setGeometry(HTofGeomPar *geom);
  void transform(HTofHit *hit,
         HGeomVector &r) const;
  void toLocal(HGeomVector &r, HTofHit *data,
           HGeomVector &out) const;
  void getLocal(HTofHit *data, HGeomVector &r) const;
  HGeomTransform &getTransform(Int_t sector, Int_t module) const;
  Bool_t init(void);
  void getSigmas(HTofHit *hit, Float_t &fSigmaX, Float_t &fSigmaY) const;
protected:
  HTofGeomPar *fGeometry; //!
  HSpecGeomPar *fSpecGeometry;//!
public:
  ClassDef(HMetaAlignerKickTofTransform,1)
};

class HMetaAlignerKickMdcTransform : public TObject {
public:
  HMetaAlignerKickMdcTransform(void);
  ~HMetaAlignerKickMdcTransform(void);
  void setGeometry(HMdcGeomPar *geom);
  void transform(HMdcSeg *hit,
         HGeomVector &r,HGeomVector &alpha) const;
  Bool_t init(void);
protected:
  HMdcGeomPar *fGeometry; //!
  HSpecGeomPar *fSpecGeometry;//!
public:
  ClassDef(HMetaAlignerKickMdcTransform,1)
};

class HMetaAlignerTask : public HReconstructor {
public:
  HMetaAlignerTask(void);
  HMetaAlignerTask(Float_t fShowerDX, Float_t fShowerDY, Float_t fShowerDZ);
  ~HMetaAlignerTask(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Float_t distance(HGeomVector &r, HGeomVector &a, HGeomVector &p);
  void calcClosestPoint(HGeomVector &r, HGeomVector &a,
            HGeomVector &r2,HGeomVector &p);
  Float_t calcSign(HGeomVector &r,HGeomVector &alpha,HGeomVector &rOut);
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

private:
  TNtuple *fData,*fDataLocal;
public:
  ClassDef(HMetaAlignerTask,2)
};






#endif
