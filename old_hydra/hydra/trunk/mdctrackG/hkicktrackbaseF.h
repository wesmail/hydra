#ifndef HKICKTRACKBASEF_H
#define HKICKTRACKBASEF_H
#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hmdctrkcand.h"
#include "TObject.h"
#include "hshowerhit.h"
#include "hmdcgetcontainers.h"
#include "TH1F.h"
#include "hkickplane2.h"
class HCategory;
class HMdcSeg;
class HMdcGeomPar;
class HTofGeomPar;
class HTofinoGeomPar;
class HSpecGeomPar;
class HIterator;
class HRuntimeDb;
class HShowerGeometry;
class HEvent;
class HMdcMetaMatch;
class HMdcTrackGFieldPar;
class HMdcTrackGCorrPar;
class HMdcTrackGSpline;
class HMdcSegSim;
class HSplineTrack;
class HKickTrack123B;
class HGeomTransform;
class HTofHitSim;
class HMetaMatch;
class HShowerHitTofTrack;

class HKickTrackBaseF : public HReconstructor {
private:
   Double_t betaCalc;
   HGeomVector pointMeta;
  // HGeomTransform trCheck;
   HGeomTransform *tRans[6];
   Double_t target;
   Double_t distanceTof;
   Float_t xTof,yTof,zTof;
   Double_t C;
   Double_t pi;
   Int_t system;
   Int_t tofSystem;  // tofSector*1000 + tofModule*100 + tofCell
   Short_t outerHitInd;
   Short_t indTrkCand;
   Short_t index1;
   Short_t index2;
   Float_t tof;
   Float_t mass2;
   Float_t beta;
   Float_t eloss;
   Double_t TOFdistance;
   Char_t sector;
   HMdcGetContainers *fGetCont;
   HMdcSeg *segments[2];
   Int_t polarity;
   Double_t dist;
   Short_t IndTrkCand;  
   Short_t metaInd;
   HSplineTrack *sp;
   TObject *slot;
   HCategory *fCatKine;
   HCategory *fCatMdcTrkCand;
   //HCategory *fCatSplineTrack;
   HCategory *fCatKickTrack123B;
   HCategory *fCatMdcSegSim;
   HCategory *fCatTof;
   HMetaMatch *pMetaMatch;
   HMdcTrkCand *pMdcTrkCand;      
   HCategory *fCatShower;
   HCategory *fCatTofino;
   HCategory *fCatMetaMatch;  
   HMdcGeomPar *fMdcGeometry;
   HTofGeomPar *fTofGeometry;       //! TOF geometry.
   HTofinoGeomPar *fTofinoGeometry; //! Tofino Geometry
   HSpecGeomPar *fSpecGeomPar;
   HKickPlane2 *kickplane;
   HTofHitSim *pTofHit;
   HMetaMatch *pMdcSegs;
   HShowerHitTofTrack *pShowerHitTofTrack; 
   //HMdcTrackGSpline *Spline;
   
   HIterator *fMetaMatchIter;
   HIterator *IterTof;
   HIterator *IterTofino;
   HIterator *IterShower;
   HMdcTrackGFieldPar *field;
   HMdcTrackGCorrPar *corr;
   Float_t Momentum;
   Float_t MomentumMETA;
   HLocation sectorloc;
public:
   HKickTrack123B *fillData(HMdcSeg *,Bool_t);
   HKickTrackBaseF(void);
   HKickTrackBaseF(const Text_t name[],const Text_t title[]);
  ~HKickTrackBaseF();
   Int_t execute();
   Int_t kicktrackMDC123();
   Bool_t init();
   Bool_t reinit();
   Bool_t finalize();
   ClassDef(HKickTrackBaseF,0)
};
#endif

