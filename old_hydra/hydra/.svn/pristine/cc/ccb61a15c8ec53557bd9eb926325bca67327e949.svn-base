#ifndef HMDC12FIT_H
#define HMDC12FIT_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include <TTree.h>
#include <TCanvas.h>
#include <TStopwatch.h>
#include "hmatrixcategory.h"
#include "hindextable.h"
#include "hmdccal1.h"
#include "TH2.h"
#include "hmdctrackfitter.h"
#include "TNtuple.h"
#include "hgeomtransform.h"
#include "hmdclistcells.h"

class HIterator;
class HMdcClus;
class HMdcHit;
class HMdcDetector;
class HMdcGetContainers;
class HMdcSizesCells;

class HMdc12Fit : public HReconstructor {
private:
  HLocation loc;                       // location for new object
  HCategory* fCalCat;                  // pointer to cal data category
  HCategory* fClusCat;                 // pointer to cal data category
  HCategory* fSegCat;                  // pointer to Segment data category
  HCategory* fHitCat;                  // pointer to HMdcHit data category
  HCategory* fGeantKineCat;              // pointer to geant kine category
  HCategory* fGeantMdcCat;               // pointer to geant mdc data category
  HLocation locSeg;                    // location for new "segment" object
  HLocation locHit;                    // location for new "segment" object
  HLocation locClus;                   // location for new "segment" object
  HIterator* iter;
  HIterator* iterClus;
  HMdcDetector* fmdc;
  HMdcGetContainers* fGetCont;
  HMdcSizesCells* fSizesCells;

  Int_t fEventId, clSeqNum;


  void calcCross(HGeomVector* vc1, HGeomVector* vc2, 
                const HGeomTransform* trans, HGeomVector* vcNew);
  Bool_t fillHit(Int_t sec, Int_t mod, HMdcHit* fHit);
  
  const HGeomTransform* coordSys;
  HMdcCal1* cal;
  

  Bool_t executeClust(HMdcClus * clst);
//   void printClust(Int_t nc, HMdcCluster& clst);
  void setHists(void);
  void writeHists(void);
  
  
//list finded tracks
  
//Hists:
  TH1F* hist1[200];
  TH2F* hist2[200];
  TH2C* plots[200][6];
  TFile *hfile;
  TNtuple *hitTuple;   
  TNtuple *trackTuple;   
  TStopwatch timer; //For benchmarking
 
private:
  
  Float_t calcVertexDist( Float_t x0, Float_t y0, Float_t z0,
	    Float_t x1, Float_t y1, Float_t z1,
	    Float_t x2, Float_t y2, Float_t z2);
  
  HMdcTrackFitter trackFitter;

  Float_t TDCTime[250];
 	 
  Float_t xLeft[250];
  Float_t yLeft[250];
  Float_t zLeft[250];
  Float_t xRight[250];
  Float_t yRight[250];
  Float_t zRight[250];
  
  Float_t qualityWeight[250];  // reflect number of fiered wiers per plane in cluster.
  
  Short_t address[250][4];  // mod,layer,cell,time (1 or 2) coordintes of hits
  HMdcList12GroupCells listCells; // copy of list cells in cluster
  
  Int_t plane[250];
  
  Float_t xPlane, yPlane, zPlane;
   
  Int_t nClTimes;
  Int_t NMDCTime[2];
  
  Bool_t fprint;
  Bool_t fhists;
      
public:
  HMdc12Fit();
  HMdc12Fit(Text_t *name,Text_t *title);
  ~HMdc12Fit(void);
  Bool_t init(void);
  Bool_t reinit(void);
  void setParContainers();
  Bool_t finalize(void);
  Int_t execute(void);
public:
  ClassDef(HMdc12Fit,0) //Digitizes MDC data.
};

#endif
