#ifndef HSPLINETRACKF_H
#define HSPLINETRACKF_H
#include <iostream>
#include <fstream>
#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hmdctrkcand.h"
#include "TObject.h"
#include "hshowerhit.h"
#include "hmdcgetcontainers.h"
#include "heventheader.h"
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
class HMdcSeg;
class HSplineTrack;
class HGeomTransform;
class HTofHit;
class HMetaMatch;
class HTofCluster;
class HShowerHitTofTrack;
class HMagnetPar;
class HSplineTrackF : public HReconstructor {
private:
   HGeomVector pointMeta;
   HGeomTransform *tRans[6];


   HEventHeader *evHeader;
   
   Double_t target;
   Double_t distanceTof;
   Float_t xTof,yTof,zTof;
   Double_t C;
   Int_t system;
   Short_t outerHitInd;
   Short_t indTrkCand;
   Short_t index1;
   Short_t index2;
   Float_t tof;
   Float_t mass2;
   Float_t beta;
   Double_t TOFdistance;
   Char_t sector;
   HMdcGetContainers *fGetCont;
   HMdcSeg *segments[2];
   Int_t polarity;
   Double_t dist;
   Short_t IndTrkCand;  
   Int_t tofClSize;
   Short_t metaInd;
   HSplineTrack *sp;
   TObject *slot;
   HCategory *fCatKine;
   HCategory *fCatMdcTrkCand;
   HCategory *fCatSplineTrack;
   HCategory *fCatMdcSegSim;
   HCategory *fCatTof;
   HCategory *fCatTofCluster;
   HMetaMatch *pMetaMatch;
   HMdcTrkCand *pMdcTrkCand;      
   HCategory *fCatShower;
   HCategory *fCatTofino;
   HCategory *fCatMetaMatch;  
   HMdcGeomPar *fMdcGeometry;
   HSpecGeomPar *fSpecGeomPar;
   HTofinoGeomPar *fTofinoGeomPar;
   HKickPlane2 *kickplane;
   HTofHit *pTofHit;
   HMetaMatch *pMdcSegs;
   HShowerHitTofTrack *pShowerHitTofTrack; 
   HMdcTrackGSpline *Spline;
   HIterator *fMetaMatchIter;
   HIterator *IterTof;
   HIterator *IterTofino;
   HIterator *IterShower;
   HMdcTrackGFieldPar *field;
   HMdcTrackGCorrPar *corr;
   HMagnetPar *pMagnet;
   Float_t Momentum;
   Float_t errP,errMass2;
   HLocation sectorloc;
   void calcMomentum(HMdcSeg **,HMetaMatch *);
   void calcMomentum(HMdcSeg *segments[]);
   Bool_t doMassStuff(HMetaMatch *);
   Bool_t doMomentum(HMetaMatch *, Bool_t &);
   Bool_t condition;
   Int_t firstCandIndex;
   Float_t fScal;
   Float_t qSpline;
   Int_t numChambers;
   Double_t calcTofinoDistance(HGeomVector &,HGeomVector ,HShowerHitTofTrack *);
public:
   HSplineTrack  *fillData(HMdcSeg *,Bool_t);
   HSplineTrackF(void);
   HSplineTrackF(Text_t name[], Text_t title[]);
   ~HSplineTrackF();
   Int_t execute();
   Int_t mdc1234();
   Bool_t init();
   Bool_t reinit();
   Bool_t finalize();
   ClassDef(HSplineTrackF,0)
      };
#endif

