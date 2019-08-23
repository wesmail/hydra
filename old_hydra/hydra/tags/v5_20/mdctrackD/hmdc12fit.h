#ifndef HMDC12FIT_H
#define HMDC12FIT_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"
#include <TTree.h>
#include <TCanvas.h>
#include <TStopwatch.h>
#include "hmatrixcategory.h"
#include "hindextable.h"
#include "hmdccal1.h"
#include "TH2.h"
#include "hmdctrackfitter.h"

class HIterator;

class HMdc12Fit : public HReconstructor {
private:
  HLocation loc;                        // location for new object
  HCategory* fCalCat;                   // pointer to cal data category
  HCategory* fSegCat;                   // pointer to Segment data category
  HLocation locSeg;                     // location for new "segment" object
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb;
  HMdcGeomPar* fGeomPar;
  HMdcSizesCells* fSizesCells;
  HGeomTransform* sysLayers[12];

  Int_t fEventId, clSeqNum;


  
  Int_t  getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys);
  void calcCross(HGeomVector* vc1, HGeomVector* vc2, 
                const HGeomTransform* trans, HGeomVector* vcNew);

  
  const HGeomTransform* coordSys;
  HMdcCal1* cal;
  

  void executeClust(Int_t nc,  HMdcCluster& clst);
  void printClust(Int_t nc, HMdcCluster& clst);
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
  
  Short_t address[250][3];  // layer, cell coordintes of hits
  
  Int_t plane[250];
  
  Float_t xPlane, yPlane, zPlane;
   
  Int_t nClTimes;
  Int_t NMDCTime[2];
  
  Bool_t fprint;
      
public:
  HMdc12Fit();
  HMdc12Fit(Text_t *name,Text_t *title);
  ~HMdc12Fit(void) {};
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void);
  Int_t execute(void);
public:
  ClassDef(HMdc12Fit,0) //Digitizes MDC data.
};

#endif
