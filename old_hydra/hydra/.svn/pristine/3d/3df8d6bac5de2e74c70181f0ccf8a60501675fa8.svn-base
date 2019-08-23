#ifndef HMDCTFTEST_H
#define HMDCTFTest_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "TH2.h"
#include "hmdcsimeventinf.h"

class HIterator;

class HMdcTFTest : public HReconstructor {
private:
  HLocation loc;                        // location for new object
  HCategory* fCalCat;                   // pointer to cal data category
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb; 
  HMdcGeomPar* fGeomPar;
  HMdcSizesCells* fSizesCells;
  HMdcSimEventInf inform;
  
  Float_t xEvVr,yEvVr,zEvVr;          // Event vertex
  Int_t eventId;
  
  Float_t dens;
  
  const HGeomTransform* coordSys;
  HGeomTransform* sysLayers[24];
  
  Int_t analClust(HMdcCluster& clst, Int_t* nTrL,  Int_t* nTrLT, Int_t& nHitsInCl);
  void printClust(Int_t nc, HMdcCluster& clst, 
                  Int_t nTracksClust, Int_t* nTrList, Int_t* nHitsTr);
  void calcCross(HGeomVector* vc1, HGeomVector* vc2, 
                const HGeomTransform* trans, HGeomVector* vcNew);
  Int_t  getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys);
  void calcCrossProj(Int_t nTr,const HGeomTransform* trans, HGeomVector *cross, Int_t nTm); 
  Int_t calcQualtTr(Int_t sec, Int_t nTr);
  

  void executeClust(Int_t nc,  HMdcCluster& clst, Int_t nTracksClust, Int_t* nTrList, Int_t nHitsInCl, Int_t* nHitsTr);
  void setHists(void);
  void writeHists(void);
  
//list finded tracks
  Int_t findedTr[500];         // >=1 - track finded
  
//Hists:
  TH1F* hist1[200];
  TH2F* plots[200];
  
  
public:
  HMdcTFTest();
  HMdcTFTest(Text_t *name,Text_t *title);
  ~HMdcTFTest(void) {;}
  Bool_t init(void);
  void   setParContainers();
  Bool_t finalize(void);
  Int_t  execute(void);
public:
  ClassDef(HMdcTFTest,0) //Digitizes MDC data.
};

#endif
