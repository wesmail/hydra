#ifndef HSPLINETRACKF_H
#define HSPLINETRACKF_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hmdctrkcand.h"
#include "TObject.h"
#include "htofhit.h"
#include "hshowerhit.h"
#include "hmdcgetcontainers.h"
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
class HMdcTrackGFieldPar;
class HMdcTrackGCorrPar;
class HMdcTrackGSpline;
class HMdcSegSim;
class HSplineTrack;
class HGeomTransform;
class HTofHitSim;
class HShowerHitTofTrack;

class HSplineTrackF : public HReconstructor {
private:
//const HGeomVector &targetPosition;
Int_t flag;
Int_t tofflag;
Int_t showerflag;
Double_t TOFdistance;
Double_t tofmin;
Double_t showermin;
Double_t target;
TH1F *histoXtof;
TH1F *histoYtof;
TH1F *histoXshower;
TH1F *histoYshower;
Double_t distanceTof;
Double_t C;
Int_t system;
Short_t outerHitInd;
Float_t tof;
Float_t mass2;
Float_t beta;
Double_t Xcalc,Ycalc;
Char_t sector;
Double_t quality, qualityY;
Double_t temp1[3],temp2[3];
HGeomTransform *tRans[6];
  HMdcGetContainers *fGetCont;
  Float_t Xtof,Ytof,Ztof;
   HGeomVector point1,point2,point;
   Float_t ZET,RO,PHI,THETA;//this is for META MATCHING
   Int_t polarity;
   Double_t dist;
   Short_t IndTrkCand;  
   HSplineTrack *sp;
   TObject *slot;
   HCategory *fCatMdcSegsMatch;
   HCategory *fCatSplineTrack;
   HCategory *fCatMdcSegSim;
   HCategory *fCatTof;
   HCategory *fCatShower;
   HCategory *fCatTofino;  
   HMdcGeomPar *fMdcGeometry;
   HSpecGeomPar *fSpecGeomPar;
   
   HMdcTrackGSpline *Spline;
   HIterator *fMdcSegsMatchIter;
   HIterator *IterTof;
   HIterator *IterTofino;
   HIterator *IterShower;
   HMdcTrackGFieldPar *field;
   HMdcTrackGCorrPar *corr;
   Float_t Momentum;
   HLocation sectorloc;
public:
   HSplineTrack  *fillData(HMdcSegSim *,Bool_t);
   HSplineTrackF(void);
   HSplineTrackF(Text_t name[], Text_t title[],Int_t flag=1);
   ~HSplineTrackF();
   Int_t execute();
   Int_t mdc1234();
   Bool_t init();
   Bool_t reinit();
   Bool_t finalize();
   
   ClassDef(HSplineTrackF,0)
      };
#endif

