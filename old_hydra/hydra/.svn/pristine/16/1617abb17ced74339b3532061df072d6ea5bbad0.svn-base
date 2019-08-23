#ifndef HMDC12FIT_H
#define HMDC12FIT_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hmdctrackfitter.h"
#include "hmdchitsegfiller.h"
#include "hsymmat.h"

class HIterator;
class HMdcClus;
class HMdcHit;
class HMdcSeg;
class HMdcDetector;
class HMdcGetContainers;
class HGeomVector;
class HGeomTransform;
class HCategory;
class HMdcTrackFitter;
class HMdcList12GroupCells;
class HMdcCellGroup;
class HMdc34ClFinder;
class HMdcTrkCand;
class HMdcTrackInfSim;

class HMdcFittersArray : public TObject {
  private:
    HMdcTrackFitter** fitterArr;  // array of point. to HMdcTrackFitterA(B) obj.
    Int_t size;                   // size of fitterArr
    Int_t nObj;                   // number of objects in fitterArr
    HMdcTrackFitInOut* fitpar;
    Int_t fitAuthor;
  public:
    HMdcFittersArray(void);
    ~HMdcFittersArray(void);
    void init(Int_t fitAut, HMdcTrackFitInOut* fpar);
    HMdcTrackFitter* getNewFitter(void);
    HMdcTrackFitter* getFitter(Int_t ind) {
      return (ind<0 || ind>=nObj) ? 0:fitterArr[ind];
    }
    HMdcTrackFitter* operator[](Int_t ind) {
      return (ind<0 || ind>=nObj) ? 0:fitterArr[ind];
    }
    void reset(void) {nObj=0;}
    Int_t getNumObjs(void) {return nObj;}
  private:
    void expand(void);
    
  ClassDef(HMdcFittersArray,0) // Array of HMdcTrackFitterA(B,...) objects
};

class HMdc12Fit : public HReconstructor {
  protected:
    HMdcTrackFitInOut fitpar;       // obj. keep pointer to param. and categ.
    HMdcFittersArray fittersArr[2]; // arrays of HMdcTrackFitterH or A fitters
    HMdcTrackFitter* fitter;        // carrent fitter.            

    // Categories:
    HCategory* fClusCat;        // pointer to MdcClus data category
    HIterator* iterClus;        // iterator for catMdcClus
    HCategory* fSegCat;         // pointer to Segment data category
    HCategory* fHitCat;         // pointer to HMdcHit data category
    HCategory* fClusInfCat;     // pointer to HMdcSeg data category
    HCategory* fTrkCandCat;     // pointer to HMdcTrkCand data category
    HLocation  locSeg;          // location for new "segment" object
    HLocation  locHit;          // location for new "hit" object
    HLocation  locTrkCand;      // location for new HMdcTrkCand object
    HLocation  locClus;         // location for HMdcClus object
    HLocation  locClInf;        // location for new "HMdcWireFit" object
    Bool_t     isGeant;         // kTRUE for GEANT data

    Int_t      modForSeg[2];    // num. module for filling HMdcSeg
                                // if modForSeg[0]<0 and modForSeg[1]<0
                                // HMdcSeg will not filled
    // Fit variables:
    Int_t      nFitedTimes;     // Num. of hits with hitStatus==1
    
    Bool_t     prntSt;          // kTRUE - status printed already
    Bool_t     notFillByClus;   // kTRUE - don't fill HMdcHit&HMdcSeg by cluster
    Int_t      clusFitAlg;      // clusters fitting algorithm
    Bool_t     isCosmicData;    // =kTRUE for cosmic data

    HMdcHitSegFiller hitSegFiller;    
    HMdc34ClFinder*  f34ClFinder;
    HMdcTrackInfSim* pTrackInfSim;
    
  public:
    HMdc12Fit(Int_t type, Int_t ver, Int_t indep,
              Bool_t hst=kFALSE, Bool_t prnt=kFALSE);
    HMdc12Fit(const Text_t *name,const Text_t *title, Int_t type, Int_t ver, Int_t indep,
              Bool_t hst=kFALSE, Bool_t prnt=kFALSE);
    HMdc12Fit(const Text_t *name,const Text_t *title);
    HMdc12Fit();
    ~HMdc12Fit(void);
    Bool_t init(void);
    Bool_t reinit(void);
    Bool_t finalize(void);
    Int_t  execute(void);
    void   setSignalSpeed(Float_t sp)     {fitpar.setSignalSpeed(sp);}
    void   setNotFillByClus(void)         {notFillByClus=kTRUE;}
    void   printStatus(void);
    void   setCosmicData(void)            {isCosmicData=kTRUE;}
    HMdcTrackFitInOut& getFitInOutCont(void) {return fitpar;}

  protected:
    void     setParContainers(void);
    void     fitAlgorithm1(void);
    void     fitAlgorithm2(void);
    void     fitAlgorithm3(void);
    void     fitAlgorithm3b(void);
    void     fitAlgorithm4(void);
    Bool_t   fitMod(HMdcClus* fClst, Int_t arrInd=0);
    Bool_t   fitSeg(HMdcClus* fClst, Int_t arrInd=0);
    Bool_t   fitSec(HMdcClus* fClst1, HMdcClus* fClst2);
    Bool_t   fit2Sectors(HMdcClus* fClst1, HMdcClus* fClst2,
                         HMdcClus* fClst3, HMdcClus* fClst4);
    Bool_t   fitCombClusIndFit(HMdcClus* fClst, Int_t arrInd=0);
    Bool_t   fitChambClus(HMdcClus* fClst, Int_t arrInd=0);
    Bool_t   fitMixedClus(HMdcClus* fClst, Int_t arrInd=0);
    
    Int_t    fillHitByFit(Int_t mod);
    Bool_t   fillHitSegByFit(Int_t iMod=-99);
    Int_t    fillHitByClus(Int_t mod);
    Bool_t   fillHitSegByClus(Int_t iMod=-99);

    Bool_t   fillSegBy2Hits(Int_t ind1, Int_t ind2);
    void     fillClusInf(HMdcSeg* fSeg,Bool_t secondSec=kFALSE);
    void     fillClusInf(HMdcHit* fHit,Int_t iMod,Char_t fitSt,
                         Int_t indClusFit=-1);

    Double_t calcVertexDist(Double_t x0, Double_t y0, Double_t z0,
	                    Double_t x1, Double_t y1, Double_t z1,
	                    Double_t x2, Double_t y2, Double_t z2);
    void     calcMinFunctional(void);
    HMdcHit* getHitSlot(Int_t sec, Int_t mod, Int_t& index);
    HMdcSeg* getSegSlot(Int_t sec,Int_t seg);
    void     setGroup(HMdcList24GroupCells* lCl, HMdcSeg* fSeg, Int_t seg);
    void     setGroup(HMdcList24GroupCells* lCl, HMdcHit* fHit, Int_t mod);
    Int_t    findSeg2Clusters(HMdcTrackFitter* fitter, Bool_t flag);
    void     copySignId(HMdcHit* hit,Int_t mod,Int_t lay,HMdcSeg* seg);
    HMdcTrkCand* fillTrkCandISeg(void);
    HMdcTrkCand* fillTrkCandOSeg(HMdcTrkCand* fTrkCand);

  ClassDef(HMdc12Fit,0) // Manager class for Dubna track straight piece fitter.
};

#endif
