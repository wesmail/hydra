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
class HMdcSizesCells;

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
    HMdcDetector* fmdc;
    HMdcGetContainers* fGetCont;
    HMdcSizesCells* fSizesCells;
    HMdcClus* fClst;
    HMdcHit* fHit;
    HMdcSeg* fSeg;
    Bool_t isGeant;
    Bool_t isCatHitClus;   // =kFALSE if catMdcHit cr. not for HMdcHit(Sim)Clus
    Int_t tFillSeg[2];     // [0]-segment 1, [1]-segment 2
                           // =-1 - don't fill HMdcSeg
                           // =0,1 (2,3) - at the typeClustFinder=1
                           // (see hmdctrackfinder.cc) HMdcSeg will filled
                           // by hits from MDC1,MDC2 (MDC3,MDC4)
                           // If in segment used only one MDC and tFillSeg=0,1
                           // HMdcSeg will filled by Hit in this MDC.
    Bool_t prntSt;         // kTRUE - status printed already
  private:
    void fillSeg(void);
    Bool_t fillHit(Short_t mod);
    void setParContainers(void);
    void fillClusInf(Int_t iMod);

  public:
    HMdcClusterToHit();
    HMdcClusterToHit(Text_t *name, Text_t *title);
    HMdcClusterToHit(Int_t fSeg1, Int_t fSeg2);
    HMdcClusterToHit(Text_t *name, Text_t *title,Int_t fSeg1,Int_t fSeg2);
    ~HMdcClusterToHit(void);
    Bool_t init(void);
    Bool_t reinit(void);
    Bool_t finalize(void) {return kTRUE;}
    Int_t execute(void);
    void printStatus(void);

  ClassDef(HMdcClusterToHit,0)
};

#endif









