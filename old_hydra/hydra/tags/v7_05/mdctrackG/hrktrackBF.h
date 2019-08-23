#ifndef HRKTRACKBF_H
#define HRKTRACKBF_H
#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hmdctrkcand.h"
#include "TObject.h"
#include <TNtuple.h>
#include "hmdcgetcontainers.h"
#include "htofclustersim.h"
#include "hrktrackB.h"
class HCategory;
class HMdcSeg;
class HMdcHit;
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
class HMatrixCategory;
class HSplineTrack;
class HGeomTransform;
class HTofHitSim;
class HTofHit;
class HTofClusterSim;
class HTofCluster;
class HMetaMatch;
class HShowerHitTofTrack;
class KRKTrackB;
class HRungeKutta;
class TH1F;
class TH2F;

class HRKTrackBF : public HReconstructor {
 private:
   Float_t fieldFactor; //stores user supplied field scaling factor internally
   Short_t mode;        //mode = {0 || 1 || 2} for initial momentum as {selfGuess, KickTrack123-Guess, SplineTrack-Guess}
 
   HGeomTransform     *tRans[6]; //used for META point transformation to MdcSegment-ccord-system
   HMdcTrackGFieldPar *field;
   HSpecGeomPar       *fSpecGeomPar;
   HMdcGetContainers  *fGetCont;
   HTofGeomPar        *fTofGeometry;
   HTofinoGeomPar     *fTofinoGeometry;
   HCategory          *fCatMetaMatch;
   HIterator          *fMetaMatchIter;
   HCategory          *fCatMdcTrkCand;
   HCategory          *fCatRKTrackB;
   HMetaMatch         *pMetaMatch;
   HMdcTrkCand        *pMdcTrkCand;
   HCategory          *fCatMdcSegSim;
   HCategory          *fCatMdcHitSim;
   HCategory          *fSplineTrack; //can be used here for initial value as RK-0-guess
   HSplineTrack       *pSplineTrack;     
   HCategory          *fKickTrack;   //can be used here for initial value as RK-0-guess
   HSplineTrack       *pKickTrack;     
   HCategory          *fCatKine;
   HCategory          *fCatShower;
   HCategory          *fCatTof;
   HCategory          *fCatTofCluster;
   HCategory          *fCatRKTrack;
   HRungeKutta        *pRungeKutta;

   HShowerHitTofTrack *pShowerHitTofTrack;
   HTofHit            *pTofHit;
   HTofCluster        *pTofCluster;

   Short_t  sector;
   Short_t  system;
   Int_t    indTrkCand;
   Int_t    index1;
   Int_t    index2;
   HMdcSeg *pMdcSeg1;
   HMdcSeg *pMdcSeg2;
   //HMdcHitSim* pMdc1Hit;
   //HMdcHitSim* pMdc2Hit;
   //HMdcHitSim* pMdc3Hit;
   ////HMdcHitSim* pMdc4Hit;
   HMdcHit* pMdc1Hit;
   HMdcHit* pMdc2Hit;
   HMdcHit* pMdc3Hit;
   //HMdcHit* pMdc4Hit;
   Double_t x[4], y[4];
   Int_t    metaInd;
   Float_t  tof; //Time-of-flight
   Float_t  xTof,yTof,zTof;
   HGeomVector pointMeta;

   HLocation   sectorloc;

   Bool_t  success; //flag indicating severe problems on input of RK
   Short_t qRK; //polarity
   Float_t pRK; //momentum
   Float_t momentumGuess; //initial value for momentum if supplied
   Float_t momGeantABS;   //momentum taken from Genat Kine for debugging information
   Float_t chiqRK;
   Float_t track_length;   //full track length (Target--MDC1--MDC4--META)
   Float_t track_lengthSP; //track length taken from Spline
   Float_t beta;
   Float_t mass2;
   Float_t eloss;
   Float_t RK_the;
   Float_t RK_phi;
   Float_t RK_seg_the;
   Float_t RK_seg_phi;
   Float_t RKvert_the;
   Float_t RKvert_phi;
   Float_t RKxyzMETA[3];
   Double_t pointSeg2META[3];
   

 protected:
   Bool_t   bRKdebug;         //!
   Bool_t   bMdcAlignMonitor; //!
   TNtuple *fRKdebug;         //!

   TH1F* dxMDC[ 4][6]; //module,sector
   TH1F* dyMDC[ 4][6];
   TH2F* dxxMDC[4][6];
   TH2F* dyyMDC[4][6];
                                                                
   
   //TEST// To be erased
   Float_t Kine_the;
   Float_t Kine_phi;
   Int_t   iGparticleMdcSeg1; //pid from geant kine

   Bool_t bDumpGeomSector[6];

 public:
   HRKTrackBF(Text_t name[] ,Float_t FIELD, Short_t MODE); // MODE = {0,1,2} Take initial momentum from: 0-self guess, 1-from KickPlane, 2-from SplineTrack
   HRKTrackBF();                                           // Initial momentum is taken as a self guess (MODE=0)
  ~HRKTrackBF(void); 
 
   void clear();
   void SetDebugOn(); //enables parralel storage of RK self test information into NTuple "fRKdebug" meant for simulation only
   void MakeGeomSnapshotSector(Int_t s);
   void SetMdcAlignMonitorOn(); //enables monitoring of MDC alignment using (RKfit-HMdcHit) information

   Bool_t init();
   Bool_t reinit();
   Int_t  execute();
   Bool_t finalize();

   HRKTrackB *fillData(HMdcSeg *);

   ClassDef(HRKTrackBF,0) //Runge-Kutta reconstructor
};
#endif