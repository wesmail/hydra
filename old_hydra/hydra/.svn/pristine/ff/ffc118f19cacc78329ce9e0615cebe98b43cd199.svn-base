#ifndef HMDCBEAMTEST_H
#define HMDCBEAMTEST_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "TH2.h"
#include "TCanvas.h"

class HIterator;

class HMdcBeamTest : public HReconstructor {
private:
//  HLocation loc;                        // location for new object
  HCategory* fCalCat;                   // pointer to cal data category
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb; 
  HMdcGeomPar* fGeomPar;
  HMdcSizesCells* fSizesCells;
  
  Int_t totClust;
  Int_t eventId;
  
  const HGeomTransform* coordSys;
  HGeomTransform* sysLayers[12];
  
  void calcCross(HGeomVector* vc1, HGeomVector* vc2, 
                const HGeomTransform* trans, HGeomVector* vcNew);
  Int_t  getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys);
  void executeClust(Int_t nc,  HMdcCluster& clst);
  void setHists(void);
  void writeHists(void);
  
//list finded tracks
  Int_t findedTr[500];         // >=1 - track finded
  
//Hists:
  TH1F* hist1[200];
  TH2C* plots[200];
  TH2F* plotF[200];

  Float_t cutLeft;                    // Cut: cutLeft< Time1-Time2 <cutRight
  Float_t cutRight;                   // cutLeft=cutRight=0. - no cut  
  
public:
  HMdcBeamTest();
  HMdcBeamTest(Text_t *name,Text_t *title);
  ~HMdcBeamTest(void) {};
  Bool_t init(void);
  void   setParContainers();
  Bool_t finalize(void);
  Int_t  execute(void);
  TH1F* getHist(Int_t n);
  TH2C* getPlot(Int_t n);
  TH2F* getPlotF(Int_t n);
  void printClusters(Int_t sec);
  void setCut(Float_t cutL, Float_t cutR) {cutLeft=cutL; cutRight=cutR;}

  ClassDef(HMdcBeamTest,0) //Digitizes MDC data.
};

#endif
