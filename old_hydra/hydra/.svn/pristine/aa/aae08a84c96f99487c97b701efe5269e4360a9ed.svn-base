#ifndef HMDC12FIT_H
#define HMDC12FIT_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hmdctrackfitter.h"
#include "hmdclistcells.h"
#include "TTree.h"

class HIterator;
class HMdcClus;
class HMdcHit;
class HMdcSeg;
class HMdcDetector;
class HMdcGetContainers;
class HMdcSizesCells;
class HMdcSizesCellsMod;
class HGeomVector;
class HGeomTransform;
class HCategory;
class HMdcTrackFitter;
class HMdcList12GroupCells;
class HMdcCellGroup;

class HMdc12Fit : public HReconstructor {
private:
  Int_t nModsSeg[6][2];           // [sec][seg] num. modules in segment
  Bool_t mdcExist[6][4];          // =kTRUE 
  HMdcTrackFitter* trackFitter;   // =* HMdcTrackFitterA or *HMdcTrackFitterH
  
  // Cont.:
  HMdcDetector* fMdcDetector;         // pointers to cont.
  HMdcGetContainers* fGetCont;        //
  HMdcSizesCells* fSizesCells;        //
  HMdcSizesCellsMod* fSCModAr[6][4];  //
  
  // Categories:
  HLocation loc;           // location for new object
  HCategory* fCalCat;      // pointer to cal data category
  HCategory* fClusCat;     // pointer to cal data category
  HCategory* fSegCat;      // pointer to Segment data category
  HCategory* fHitCat;      // pointer to HMdcHit data category
  Bool_t isCatHitClus;     // =kFALSE if catMdcHit cr. not for HMdcHit(Sim)Clus
  HCategory* fClusFitCat;  // cat. for debuging inform.
  HCategory* fWireFitCat;  // -/- 
  HLocation locSeg;        // location for new "segment" object
  HLocation locHit;        // location for new "segment" object
  HLocation locClus;       // location for new "segment" object
  HLocation locClusFit;    // location for new "HMdcClusFit" object
  HLocation locWireFit;    // location for new "HMdcWireFit" object
  Bool_t isGeant;          // kTRUE for GEANT data
  
  // Fit options:
//  Int_t fitType;        // =0 - Hedar's fitter , =1 -Alexander's fitter
//  Int_t version;        // =0 - old version of drift time calc.
                        // =1 - using of funct. HMdcCal2Par::calcTime(...)
                        // for recalc. distance to drift time.
//  Int_t indep;          // =0 - fit all hits in cluster
                        // =1 - fit hits in each MDC independent
  Int_t modForSeg[2];   // num. module for filling HMdcSeg
                        // if modForSeg[0]<0 and modForSeg[1]<0 - HMdcSeg
                        // will not filled
  
  // Cluster inform.:
  HMdcList12GroupCells listCells; // list cells in cluster
  HMdcClus * fClst;    // Pointer to HMdcClus obj.
  Int_t sector;        // Cluster address
  Int_t segment;       // -/-
  Double_t xVertex;    // Target or point on the kick plane
  Double_t yVertex;    //
  Double_t zVertex;    //
  Double_t xPlane[3];  // [0] - point in the middle of plane of the first mdc,
  Double_t yPlane[3];  // [1] - point in the middle of plane of the second mdc,
  Double_t zPlane[3];  // [3] - point on the project plane
  Double_t parA[3];    // parameters of the planes: parA*x+parB*y+z=parD
  Double_t parB[3];    // [0]-[3] - the same as in x(y:z)Plane
  Double_t parD[3];    //
  Int_t nClTimes;      // Num. of hits in cluster
  Int_t nMdcTime[2];   // Num. of hits in first and second MDC
  Int_t* modIndx;      // Modules list
  Int_t* layIndx;      // Layers list
  Int_t* cellIndx;     // Cells list
  Int_t* timeIndx;     // Times list: =1->time1, =2->time2 in HMdcCal1
  Double_t* tdcTime;   // TDC times list
  Double_t* xLeft;     // Wire positions lists
  Double_t* yLeft;     //
  Double_t* zLeft;     //
  Double_t* xRight;    //
  Double_t* yRight;    //
  Double_t* zRight;    //
  HMdcSizesCellsLayer** fSizeCellsLayer;
  HIterator* iterClus;

  // Fit variables: 
  Double_t* qualityWeight;  // now == 1 ???
  Double_t* dev;            // deviations array
  Double_t* distWire;       // min.dist. to wires array
  Double_t* alphaDrDist;    // angle fo min.dist
  Bool_t* inCell;           // =kTRUE - tr.cross cell
  Double_t* weight;         // weight array
  Double_t* fullTime;       // calcul.time + tof
  UChar_t hitIndex[12][250][2]; //[lay.][cell][time] the table for
                                // finding hit index in lists by layer,cell,time
  Int_t fittingMod;      // =0,1,2 - MDC1(3),MDC2(4),MDC1+2(3+4) is fitting now
  Int_t firstTime[3];    // Array for calculations "firstHit" and "lastHit"
  Int_t firstHit;        // Index of the first hit in lists for loops
  Int_t lastHit;         // Index of the last hit in lists for loops
  Int_t nFitedTimes;     // Num. of hits with hitStatus==1 
  Char_t* hitStatus;     // Hits which have status=1 will used this in fit only
                         // =0 this hit was fited (for dubl.tr.in cl. only)
                         // -1 may by will fited (-/-) Not implement yet !!!
  Double_t cutWeight;    // cut for Tukey weight
  
  
  Int_t nMdcHits[4];          // num. of HMdcHit in mod.
  HMdcHit* fMdcHit[4][100];   // array of pointers to HMdcHit
  Int_t indexHit[4][100];     // and corresponding indexis in catMdcHit
  Bool_t isHitFitted[4][100]; // kTRUE - fit ok., kFALSE - cluster.
  Int_t nMdcSegs[2];          // num. of HMdcSeg in segment
  HMdcSeg* fMdcSeg[2][100];   // array of pointers to HMdcHit
  Int_t indexSeg[2][100];  // array of HMdcSeg indexes
  
  Bool_t prntSt;         // kTRUE - status printed already

  // Options for print and output:  
//  Bool_t fprint;   // =kFALSE - debuging prints off
//  Bool_t fntuple;  // =kTRUE - fill mdchit str. and write it in hfile
public:
  HMdc12Fit(Int_t type, Int_t ver, Int_t indep, Bool_t hst=kFALSE, Bool_t prnt=kFALSE);
  HMdc12Fit(Text_t *name,Text_t *title, Int_t type, Int_t ver, Int_t indep,
      Bool_t hst=kFALSE, Bool_t prnt=kFALSE);
  HMdc12Fit(Text_t *name,Text_t *title);
  HMdc12Fit();
  ~HMdc12Fit(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void setModForSeg(Int_t ms1, Int_t ms2);
  void printStatus(void);

private:
  void setParContainers();
  Bool_t fitCluster(Double_t threshold);
  Bool_t fillArrays(HMdcList12GroupCells& lCells);
//  void fillMdcHitSeg(void);
  Bool_t fillHitByFit(Int_t iMod);
  Bool_t fillSegByModFit(Int_t iMod);
  Bool_t fillHitSegBySegFit();
  
  Bool_t fillHitByClus(Int_t iMod);
  Bool_t fillSegByModClus(Int_t iMod);
  Bool_t fillHitSegByClus(void);
  
  Bool_t fillSegBy2Hits(Int_t nHit1, Int_t nHit2);
  void fillClusInf(HMdcSeg* fSeg, Bool_t fitSt);
  void fillClusInf(HMdcHit* fHit, Int_t iMod, Bool_t fitSt);
  
  Double_t testFitResult();
  void unsetHits(HMdcList12GroupCells& lCells);
//   inline void calcCross(HGeomVector* vc1, HGeomVector* vc2, 
//                 const HGeomTransform* trans, HGeomVector* vcNew);
  void fillFitCat(void);
  Double_t calcVertexDist( Double_t x0, Double_t y0, Double_t z0,
	    Double_t x1, Double_t y1, Double_t z1,
	    Double_t x2, Double_t y2, Double_t z2);
  void calcMinFunctional(void);
  void connectArrays(void);
  HMdcHit* getHitSlot(Int_t mod);
  HMdcSeg* getSegSlot(void); 
  void setGroup(HMdcList12GroupCells* lCl, HMdcCellGroup* lHit,Int_t fmod=-1);

  ClassDef(HMdc12Fit,0) //Digitizes MDC data.
};

#endif
