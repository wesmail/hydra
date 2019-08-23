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
class HKickPlane2;
class HGeantTof;
class HGeantShower;
class TNtuple;
class HReconstructor;
class HMdcTrackGSpline;
class HMdcGetContainers;
class HMagnetPar;

class HMdcTrackGSplineCorr:public HReconstructor{
 private:
  Float_t  RadiusV;
  TString fileName;
  TString outDir;
  HGeomVector POINT[4];
  HGeomVector POINT3Cham;
  HMdcGetContainers *fGetCont;
  
      HKickPlane2 *kickplane;
      
  HCategory *fCatMdc;
  HCategory *fCatKine;
  HMdcGeomPar *fMdcGeometry;
  HSpecGeomPar *fSpecGeometry;
  HMdcTrackGSpline *Spline;
  HIterator *fMdcIter;
  HIterator *fKineIter;
  HMdcTrackGFieldPar *field;
    HGeomTransform* trLS[6][4][7];
    HMdcTrackGCorrPar *corr;
  TNtuple *nt4,*nt3;
  Float_t pKine,pGeant,pRecon,phi,theta,pRecon3Cham;
  Float_t px,py,pz,p06,p16,p26,p36;
  Int_t ID,parent,tracknd;
  Bool_t ismdc1234;
  Bool_t ismdc123;
  Bool_t ismdcmeta;
  HMagnetPar *pMagnet;
  Float_t fScal;
   HGeantMdc *pGeantMdc;
     
  void calcChamber3Mode(HGeomVector *);
  
 public:
  HMdcTrackGSplineCorr(void);
  HMdcTrackGSplineCorr(Text_t *name,Text_t *title);
  virtual ~HMdcTrackGSplineCorr(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Bool_t takePoint(HGeantMdc *,HGeomVector &, Float_t &,Int_t,Int_t,Int_t);
  Int_t mdc1234();
  
  void SetFileName(TString temp="file.root");
  void SetOutDir(TString outDir="");  
  ClassDef( HMdcTrackGSplineCorr,0)
    };
#endif
