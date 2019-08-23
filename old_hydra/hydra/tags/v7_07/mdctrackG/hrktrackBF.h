#ifndef HRKTRACKBF_H
#define HRKTRACKBF_H

#include "hreconstructor.h"
#include "hgeomtransform.h"
#include "hmdctrkcand.h"
#include "hmdcgetcontainers.h"
#include "htofclustersim.h"
//#include "hrktrack.h"

class HCategory;
class HIterator;
class HMetaMatch;
class HMdcSeg;
class HMdcHit;
class HMdcTrkCand;
class HBaseTrack;
class HSplineTrack;
class HShowerHitTofTrack;
class HTofHit;
class HTofCluster;
class HRungeKutta;
class HRKTrackB;
class HMdcTrackGFieldPar;
class HMagnetPar;
class HSpecGeomPar;
class HMdcGeomPar;
class HTofGeomPar;
class HTofinoGeomPar;
class HShowerGeometry;
class HMdcGetContainers;
class HMdcSizesCells;
class TNtuple;

class HRKTrackBF : public HReconstructor {
 private:
   Float_t fieldFactor; // field scaling factor
   Short_t mode;        // mode = {0 || 1 || 2} for initial momentum as {selfGuess, KickTrack123-Guess, SplineTrack-Guess}
 
   HGeomTransform      secTrans[6];     // sector transformation, used to transform META point to MdcSegment-ccord-system
   HMdcTrackGFieldPar* field;           // field map
   HMagnetPar*         pMagnet;         // parameter container for magnet settings
   HSpecGeomPar*       fSpecGeomPar;    // sector and target geometry
   HMdcGetContainers*  fGetCont;        // pointer to HMdcGetContainers object
   HTofGeomPar*        fTofGeometry;    // TOF geometry
   HTofinoGeomPar*     fTofinoGeometry; // Tofino geometry
   HShowerGeometry*    fShowerGeometry; // Shower geometry
   HMdcSizesCells*     pMSizesCells;    // pointer to HMdcSizesCells objects
   
   HCategory*          fCatMetaMatch;   // pointer to MetaMatch category
   HIterator*          fMetaMatchIter;  // iterator on the MetaMatch category
   HMetaMatch*         pMetaMatch;      // pointer to the MetaMatch object
   HCategory*          fCatMdcTrkCand;  // pointer to MDC track candidate category
   HMdcTrkCand*        pMdcTrkCand;     // pointer to the MDC track candidate
   HCategory*          fCatMdcSeg;      // pointer to the MDC segments category
   HCategory*          fCatMdcHit;      // pointer to the MDC hit
   HCategory*          fSplineTrack;    // pointer to the Spline track category (for initial momentum guess)
   HSplineTrack*       pSplineTrack;    // Spline track 
   HCategory*          fKickTrack;      // pointer to the Kick track category (for initial momentum guess)
   HBaseTrack*       pKickTrack;        // Kick track     
   HCategory*          fCatKine;        // pointer to the Kine category
   HCategory*          fCatShower;      // pointer to the Shower category
   HShowerHitTofTrack* pShowerHitTofTrack; // pointer to the Shower/Tofino hit category
   HCategory*          fCatTof;         // pointer to the Tof hit category
   HTofHit*            pTofHit;         // tof hit
   HCategory*          fCatTofCluster;  // pointer to the Tof cluster category
   HTofCluster*        pTofCluster;     // Tof cluster
   HCategory*          fCatRKTrack;     // pointer to the Runge Kutta track category 
   HRungeKutta*        pRungeKutta;     // Runge Kutta track

   HLocation   sectorloc;        // sector location
   Short_t  sector;              // sector number (0..5)
   Short_t  system;              // meta detector (0==Tofino, 1==Tof)
   HMdcSeg* pMdcSeg1;            // pointer to inner segment
   HMdcSeg* pMdcSeg2;            // pointer to outer segment
   Float_t vertex[3];            // vertex
   HGeomVector normVecTofino[6]; // normal vector on the Tofino module in the sector coordinate system
   HGeomVector centerTofino[6];  // physical center of Tofino module in the sector coordinate system
   HGeomVector normVecShower[6]; // normal vector on the Shower module in the sector coordinate system
   HGeomVector normVecTof[6][8]; // normal vector on each Tof module in the sector coordinate system
   HGeomVector metaNormVec;      // normal vector on the meta module in the sector coordinate system
   HGeomVector pointMeta;        // META hit

   Bool_t  success;              // flag indicating severe problems on input of RK
   Short_t qRK;                  // polarity
   Float_t pRK;                  // momentum
   Float_t momentumGuess;        // initial value for momentum if supplied
   Float_t chiqRK;               // chi2 of Runge Kutta
   Float_t trackLength;          // full track length (Target--MDC1--MDC4--META)
   Float_t tof;                  // time-of-flight
   Float_t beta;                 // beta value
   Float_t mass2;                // mass**2
   Float_t RKxyzMETA[3];         // META point after RK fitting
   
 public:
   HRKTrackBF();
   HRKTrackBF(Text_t name[], Short_t m=2);
   HRKTrackBF(Text_t name[],Float_t field, Short_t m=2);
   ~HRKTrackBF(void); 
 
   void clear();
   Bool_t init();
   Bool_t reinit();
   Int_t  execute();
   Bool_t finalize() {return kTRUE;}

 private:
   HRKTrackB* fillData(HMdcSeg*);
   void calcPosDirFromSegment(HMdcSeg*,Double_t*,Double_t*);

   ClassDef(HRKTrackBF,0) // Runge-Kutta reconstructor
};

#endif
