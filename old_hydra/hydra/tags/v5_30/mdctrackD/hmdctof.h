#ifndef HMDCTOF_H
#define HMDCTOF_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"
#include "hmdccal1.h"
#include "htofhit.h"
#include "TH2.h"
#include "TCanvas.h"
//#include "hmdcsimeventinf.h"

class HIterator;

class HMdcTof : public HReconstructor {
private:
//  HLocation loc;                        // location for new object
  HCategory* fCalCat;                   // pointer to cal data category
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb; 
  HMdcGeomPar* fGeomPar;
  HMdcSizesCells* fSizesCells;
  
  HCategory *fTofHitCat;
  HIterator* tofIter;
  
  Int_t totClust;
  Int_t eventId;
  
  Int_t nTof; 
  Float_t xTof[50];
  Float_t yTof[50];
  
  Float_t tmA[64];
  Float_t tmB[64];
  
//==Efficiency=================
  Int_t nClust6;
  Int_t nClust5;
  Int_t nClust5l[6];
  Int_t nSingHit;
  Int_t nDublHit;
  Int_t nSingHitL[6];
  Int_t nDublHitL[6];
//==============================
  
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
  HMdcTof();
  HMdcTof(Text_t *name,Text_t *title);
  ~HMdcTof(void) {;}
  Bool_t init(void);
  void   setParContainers();
  Bool_t finalize(void);
  Int_t  execute(void);
  Int_t getNtof() {return nTof;}
  TH1F* getHist(Int_t n);
  TH2C* getPlot(Int_t n);
  TH2F* getPlotF(Int_t n);
  void printClusters(Int_t sec);
  void setCut(Float_t cutL, Float_t cutR) {cutLeft=cutL; cutRight=cutR;}

  ClassDef(HMdcTof,0) //Digitizes MDC data.
};

#endif
