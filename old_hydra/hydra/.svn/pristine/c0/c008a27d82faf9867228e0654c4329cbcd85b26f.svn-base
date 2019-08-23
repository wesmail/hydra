#ifndef HMDCCLUSTERTOHIT_H
#define HMDCCLUSTERTOHIT_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HMdcDetector;
class HMdcGetContainers;
class HMdcClus;
class HGeomVector;
class HMdcSeg;
class HMdcHit;

class HMdcClusterToHit : public HReconstructor {
  private:
    HLocation locHit;      // location for new "hit" object
    HLocation locSeg;      // location for new "segment" object
    HCategory* fHitCat;    // pointer to HMdcHit data category
    HCategory* fSegCat;    // pointer to HMdcSeg data category
  //  HCategory* fTrackCat;  // pointer to HMdcTrk data category
    HIterator* iter;
    HLocation locClust;    // location for "cluster" object
    HCategory* fClustCat;  // pointer to Cluster data category
  //  Bool_t isGeant;        // =kTRUE for GEANT data     
    HMdcDetector* fmdc; 
    HMdcGetContainers* fGetCont;
    Int_t tFillSeg;         // =-1 - don't fill HMdcSeg
                           // =0 (or 1) - at the typeClustFinder=1 
                           // (see hmdctrackfinder.cc) HMdcSeg will filled
                           // by hits from MDC1 (or MDC2)
                           // If in segment 1 used only one MDC (1 or 2)
                           // and tFillSeg=0 or 1 
                           // HMdcSeg will filled by Hit in this MDC.
  private:  
    void calcSeg(Double_t xTag, Double_t yTag, Double_t zTag, 
                 Double_t xCl,  Double_t yCl,  Double_t zCl,
           Double_t &theta, Double_t &phi, Double_t &zm, Double_t &r0);
    void fillSeg(HMdcClus* fClst,HMdcSeg* fSeg);
    void calcHit(HGeomVector& p1, HGeomVector& p2,
      Float_t &x, Float_t &y, Float_t &l, Float_t &m);
    Bool_t fillHit(HMdcClus* fClst,Short_t mod,HMdcHit* fHit);
    
  public:
    HMdcClusterToHit(Int_t fSeg=1);
    HMdcClusterToHit(Text_t *name, Text_t *title, Int_t fSeg=1);
    ~HMdcClusterToHit(void) {;}
    Bool_t init(void);
    Bool_t reinit(void);
    Bool_t finalize(void) {return kTRUE;}
    Int_t execute(void);
  ClassDef(HMdcClusterToHit,0) 
};

#endif









