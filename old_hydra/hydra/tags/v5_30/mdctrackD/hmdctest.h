#ifndef HMDCTEST_H
#define HMDCTEST_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
//#include "hmdctimecut.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TROOT.h"

class HIterator;


class HMdcTest : public HReconstructor {
private:

  HCategory* fCalCat;                   // pointer to cal data category
  Bool_t isGeant;                       // =kTRUE for GEANT data
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb;
  HMdcGeomPar* fGeomPar;
  HMdcLayerGeomPar* fLayerGeomPar;      // Geometry parameters of the layers
  HGeomTransform* sysLayers[4][6];
  
  UChar_t qual[6][200];
  Short_t maxClusters[6];
  Int_t eventId;

  const HGeomTransform* coordSys;

  Int_t calcCross( HGeomVector* point1, HGeomVector* point2,
                   Short_t mod, Short_t layer, HGeomVector* cross);

  HMdcTimeCut* fcut;
  
  
  HLocation loc;                        // location for new object 
  TH2F *plot[100];
  TProfile *prFile[100];
  TH1F* hst[100];

public:

  TH2F* getPlot(Int_t n){return plot[n];}
  TProfile* getProfile(Int_t n){return prFile[n];}
  TH1F* getHist(Int_t n){return hst[n];}

  HMdcTest();
  HMdcTest(Text_t *name,Text_t *title);
  ~HMdcTest(void);


  Bool_t init(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  void drawProjPlots(Int_t sec);
  void drawHitsPlots(Int_t sec);
  
  void printClusters(Int_t sec);
  ClassDef(HMdcTest,0) //Digitizes MDC data.
};

#endif
