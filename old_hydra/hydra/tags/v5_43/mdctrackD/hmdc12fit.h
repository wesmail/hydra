#ifndef HMDC12FIT_H
#define HMDC12FIT_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hmdctrackfitter.h"
#include "TNtuple.h"
#include "hmdclistcells.h"

class HIterator;
class HMdcClus;
class HMdcHit;
class HMdcSeg;
class HMdcDetector;
class HMdcGetContainers;
class HMdcSizesCells;
class HGeomVector;
class HGeomTransform;
class HCategory;

class HMdc12Fit : public HReconstructor {
private:
  HMdcTrackFitter trackFitter;
  HMdcList12GroupCells listCells;   // list cells in cluster
  Char_t hitStatus[250];
  Float_t TDCTime[250];
  
  HMdcSizesCellsLayer* fSizeCellsLayer[250];
  Int_t sector;
  Int_t segment;
  Int_t version;        // =0 - old version of drift time calc.
                        // =1 - using of funct. HMdcCal2Par::calcTime(...)
                        // for recalc. distance to drift time.
  Int_t modIndx[250];
  Int_t layIndx[250];
  Int_t cellIndx[250];
  Int_t timeIndx[250];  // time=1 or 2 -> time1 or time 2 in HMdcCal1
  
  Float_t xLeft[250];
  Float_t yLeft[250];
  Float_t zLeft[250];
  Float_t xRight[250];
  Float_t yRight[250];
  Float_t zRight[250];
  Float_t qualityWeight[250];  // reflect number of fiered wiers per plane in cluster.
  Float_t xPlane, yPlane, zPlane; 
  Int_t nClTimes;
  Int_t nFitedTimes;
  Int_t NMDCTime[2];
  UChar_t hitIndex[12][250][2]; //[layer][cell][time]
    
  HLocation loc;                       // location for new object
  HCategory* fCalCat;                  // pointer to cal data category
  HCategory* fClusCat;                 // pointer to cal data category
  HCategory* fSegCat;                  // pointer to Segment data category
  HCategory* fHitCat;                  // pointer to HMdcHit data category
  HLocation locSeg;                    // location for new "segment" object
  HLocation locHit;                    // location for new "segment" object
  HLocation locClus;                   // location for new "segment" object
//  HIterator* iter;
  HIterator* iterClus;
  HMdcDetector* fMdcDetector;
  HMdcGetContainers* fGetCont;
  HMdcSizesCells* fSizesCells;
  
  Bool_t isGeant;
  
  Int_t nModsSeg[6][2];   // [sec][seg] num. modules in segment
  Int_t modForSeg[2];     // num. module for filling HMdcSeg
                          // if modForSeg[0]<0 and modForSeg[1]<0 - HMdcSeg
                          // will not filled
  Int_t indexHit[4];
  HMdcClus * fClst;

  //Hists:  
  Bool_t fprint;
  Bool_t fntuple;
  TFile *hfile;
  TNtuple *hitTuple;   
  TNtuple *trackTuple;
public:
  HMdc12Fit(Bool_t hst=kFALSE, Bool_t prnt=kFALSE, Int_t ver=0);
  HMdc12Fit(Text_t *name,Text_t *title,Bool_t hst=kFALSE, Bool_t prnt=kFALSE, 
      Int_t ver=0);
  ~HMdc12Fit(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void fillSegByMod(Int_t ms1, Int_t ms2);

private:
  void setParContainers();
  Bool_t fitCluster(Float_t threshold);
  Bool_t fillArrays(HMdcList12GroupCells& lCells);
  void fillMdcHitSeg();
  Bool_t fillHit(Int_t mod, HMdcHit* fHit);
  void fillSeg(HMdcSeg* fSeg); 
  Float_t testFitResult();
  void unsetHits(HMdcList12GroupCells& lCells);
  void calcCross(HGeomVector* vc1, HGeomVector* vc2, 
                const HGeomTransform* trans, HGeomVector* vcNew);
  void setNTuples(void); 
  void fillNTuples(void);
  Float_t calcVertexDist( Float_t x0, Float_t y0, Float_t z0,
	    Float_t x1, Float_t y1, Float_t z1,
	    Float_t x2, Float_t y2, Float_t z2);
  void calcMinFunctional(void);

  ClassDef(HMdc12Fit,0) //Digitizes MDC data.
};

#endif
