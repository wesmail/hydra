#ifndef HMETAMATCHF_H
#define HMETAMATCHF_H
#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hmdctrkcand.h"
#include "TObject.h"
#include "htofhit.h"
#include "hshowerhit.h"
#include "hmdcgetcontainers.h"
#include "hkickplane2.h"
#include "TH1F.h"
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
class HMdcTrkCand;
class HMdcTrackGSpline;
class HMdcSegSim;
class HMetaMatch;
class HGeomTransform;
class HTofHitSim;
class HShowerHitTofTrack;
class HMetaMatchF : public HReconstructor 
{
private:
   Bool_t condGeant;
   Int_t totalNumber;
   Float_t sigmaTofX;
   Float_t sigmaTofY;
   Float_t sigmaShowerX;
   Float_t sigmaShowerY;
   Float_t sTofX;
   Float_t sTofY;
   Float_t sShowerX;
   Float_t sShowerY;
   HMdcSegSim *segments[2];
   HTofHitSim *pTofHit;
   HMdcTrkCand *pTrkCand;
   HShowerHitTofTrack *pShowerHitTofTrack;
   Short_t index1,index2;
   Int_t tofflag;
   Int_t showerflag;
   Double_t metaDistance;
   Double_t distanceTof;
   Int_t system;
   Short_t trkCandIndex;
   Short_t metaHitIndex;
   Short_t firstCandIndex;
   Short_t indexprevios;
   Int_t candidatesNumber;
   Int_t tempcounter;
   Double_t qualityTof;
   Double_t qualityShower;
   Short_t metaInd;
   Short_t segInd1;
   Short_t segInd2;
   Double_t Xcalc,Ycalc;
   Float_t Xtof,Ytof,Ztof;
   Char_t sector;
   Double_t quality, qualityY;
   HGeomTransform *tRans[6];
   HGeomVector POINTS[4];
   HMdcGetContainers *fGetCont;
   HGeomVector point;
   HMetaMatch *meta;
   TObject *slot;
   Int_t tofcat,showercat;
   Short_t IndTrkCand;
   HCategory *fCatKine;
   HCategory *fCatTrkCand;
   HCategory *fCatMetaMatch;
   HCategory *fCatMdcSegSim;
   HCategory *fCatTof;
   HCategory *fCatShower;
   HCategory *fCatTofino;
   HKickPlane2 *kickplane;  
   HMdcGeomPar *fMdcGeometry;
   HSpecGeomPar *fSpecGeomPar;
   HMdcTrackGSpline *Spline;
   HIterator *fTrkCandIter;
   HIterator *IterTof;
   HIterator *IterTofino;
   HIterator *IterShower;
   HLocation sectorloc;
public:
   HMetaMatchF(void);
   HMetaMatchF(Text_t name[], Text_t title[]);
   ~HMetaMatchF();
   Int_t execute();
   Int_t matching1234();
   Int_t matching12();
   Bool_t init();
   Bool_t reinit();
   Bool_t finalize();
   HMetaMatch *fillMeta(Bool_t);
   void fillMeta1();
//   void evaluateMetaCandidates();//From HKickTrackF
//   void assignUniqueCandidates();//From HKickTrackF
//   void assignRemainingCandidates();//From HKickTrackF
//   void checkMetaOverlap();//From HKickTrackF
   ClassDef(HMetaMatchF,0)
      };
#endif

