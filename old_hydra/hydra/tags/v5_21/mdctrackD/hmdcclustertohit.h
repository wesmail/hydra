#ifndef HMDCCLUSTERTOHIT_H
#define HMDCCLUSTERTOHIT_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdclookuptb.h" 

class HIterator;

class HMdcClusterToHit : public HReconstructor {
private:
  HLocation locHit;            // location for new "hit" object
  HLocation locSeg;            // location for new "segment" object
  HCategory* fHitCat;          // pointer to HIT data category
  HCategory* fSegCat;          // pointer to Segment data category
//  Bool_t isGeant;            // =kTRUE for GEANT data
  HMdcLookUpTb* fLookUpTb;     
  HMdcDetector* fmdc;
 
  HMdcGeomPar* fGeomPar;
  HGeomVector  targetM[6];     // middle point of target in coor.sys. of sectors
  Double_t pA[6],pB[6],pD[6];  // pA[sec]*X+pB[sec]*Y+Z=pD[sec] - projection's
                               // plane
  Double_t tAx[6],tBx[6];      // X=tAx[ns]*Z+tBx[ns] - target line
  Double_t tAy[6],tBy[6];      // Y=tAy[ns]*Z+tBy[ns] 
  Double_t dZt[6];             // error of vertex
  
  void calcSeg(Int_t ns, Double_t zTag, Double_t xCl, Double_t yCl,
         Double_t &theta, Double_t &phi, Double_t &zm, Double_t &r0);
public:
  HMdcClusterToHit();
  HMdcClusterToHit(Text_t *name,Text_t *title);
  ~HMdcClusterToHit(void) {;}
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
public:
  ClassDef(HMdcClusterToHit,0) 
};

#endif









