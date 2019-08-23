#ifndef HMDCTRACKGSPLINECORR_H
#define HMDCTRACKGSPLINECORR_H

#include "hreconstructor.h"
#include "TString.h"
#include "TNtuple.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgfieldpar.h"
#include "TObject.h"
class HCategory;
class HMdcGeomPar;
class HSpecGeomPar;
class HIterator;
class HGeomVector;
class HGeomTransform;
class HGeantMdc;
class HGeantKine;
class HGeantTof;
class HGeantShower;
class TNtuple;
class HReconstructor;
class HMdcTrackGSpline;
class HMdcGetContainers;
//class HMdcTrackGFieldPar;
//class HMdctrackGCorrPar;

class HMdcTrackGSplineCorr:public HReconstructor{
 private:
  HMdcGetContainers *fGetCont;
  HCategory *fCatMdc;
  HCategory *fCatKine;
  HMdcGeomPar *fMdcGeometry;
  HSpecGeomPar *fSpecGeometry;
  HMdcTrackGSpline *Spline;
  HIterator *fMdcIter;
  HIterator *fKineIter;
  HMdcTrackGFieldPar *field;
  HMdcTrackGCorrPar *corr;
  TNtuple *nt;
  Float_t pKine,pGeant,pRecon,phi,theta;
  Float_t px,py,pz,p06,p16,p26,p36;
  Int_t ID,parent,tracknd;
  Bool_t ismdc1234;
  Bool_t ismdc123;
  Bool_t ismdcmeta;
 public:
  HMdcTrackGSplineCorr(void);
  HMdcTrackGSplineCorr(Text_t *name,Text_t *title);
  ~HMdcTrackGSplineCorr(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  void takePoint(HGeantMdc *,HGeomVector &, Float_t &,Int_t,Int_t,Int_t);
  Int_t mdc1234();
  //   void mdc123(){;}
   //   void mdcmeta(){;}
  ClassDef( HMdcTrackGSplineCorr,0)
    };
#endif
    
