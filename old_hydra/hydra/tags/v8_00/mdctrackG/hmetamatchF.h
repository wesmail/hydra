#ifndef HMETAMATCHF_H
#define HMETAMATCHF_H
#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hlocation.h"

#define RICH_TAB_SIZE 3

class HCategory;
class HMdcSeg;
class HTofCluster;
class HTofHit;
class HShowerHit;
class HIterator;
class HMdcTrkCand;
class HMdcTrackGSpline;
class HMetaMatch;
class HGeomTransform;
class HKickPlane2;
class HMdcGetContainers;
class HRichHit;
class HRichHitIPU;
class HTofGeomPar;
class HShowerGeometry;
class HMetaMatchPar;

class HMetaMatchF : public HReconstructor {
  private:
//    HGeomVector metaDir;           // for overl. Tof-Shower, now is not used
//    HGeomVector metaDirShower;     // for overl. Tof-Shower, now is not used
    Bool_t condGeant;
    
    // Cut parameters:
    HMetaMatchPar* fMatchPar;
    Float_t sigmaTofX[6];
    Float_t sigmaTofY[6];
    Float_t sTofX[6];
    Float_t sTofY[6];
    Float_t qualityTOFCut[6];
    Float_t sigma2MdcInShrX[6];
    Float_t sigma2MdcInShrY[6];
    Float_t sShowerX[6];
    Float_t sShowerY[6];
    Float_t qualitySHOWERCut[6];
    Float_t dThRich[6];                   // matching window for 6 sectors
    Float_t dPhRich[6];                   // ...
    //Float_t dThRichOff[6];                // ...
    Float_t dPhRichOff[6];                // ...
    Float_t qualityRichCut[6];            //
    Float_t dThRichIPU[6];                // ...
    Float_t dPhRichIPU[6];                // ...
   // Float_t dThRichIPUOff[6];             // ...
    Float_t dPhRichIPUOff[6];             // ...
    Float_t qualityRichIPUCut[6];
    
    Float_t richThetaMinCut[6];
    Float_t richThetaMaxCut[6];
    Float_t richIPUThetaMinCut[6];
    Float_t richIPUThetaMaxCut[6];
    
             // ...
    
    HMdcSeg *segments[2];
    Short_t trkCandIndex;                     // ??????????
    const HGeomTransform *labTrans[6];
    HGeomVector mdcTrackPar[4];
    HMdcGetContainers *fGetCont;
    HMetaMatch *meta;
    HCategory *fCatTrkCand;
    HCategory *fCatMetaMatch;
    HCategory *fCatMdcSeg;
    HCategory *fCatTof;
    HCategory *fCatTofCluster;
    HCategory *fCatShower;
    HCategory *fCatRich;
    HCategory *fCatRichIPU;

    HKickPlane2 *kickplane;
    HMdcTrackGSpline *Spline;
    HIterator *fTrkCandIter;
    HIterator *IterTof;
    HIterator *IterTofCluster;
    HIterator *IterShower;
    HIterator *IterRich;
    HIterator *IterRichIPU;
    HLocation sectorLoc;
    
    HTofGeomPar* fTofGeometry;
    HShowerGeometry *fShrGeometry;

    Int_t nShowerHits[6];
    HShowerHit* fShowerHits[6][200];
//    HGeomVector showerHits[6][200];
//    Char_t showerModule[6][200];    // Shower module of hit
    Short_t indexShrHit[6][200];
    Double_t xSegCrShr;             // current cross poinr mdc segment with
    Double_t ySegCrShr;             // shower or tof module in coor.sys. of mod.

    Int_t nTofHits[6];
    HGeomVector tofHits[6][200];
    Char_t tofModule[6][200];      // Tof module of hit
    Short_t indexTofHit[6][200];
    Char_t tofClustSize[6][200];   // 0 - HTofHit obj., >=1 - HTofCluster 
    Double_t xSegCrTof;             // current cross poinr mdc segment with
    Double_t ySegCrTof;             // shower or tof module in coor.sys. of mod. 
    
                                   // For each inner MDC segment is filled:
    Int_t nTrCndForTHit[200];      // Number of trackCands matched to TofHit
    Int_t nTrCndForSHit[200];      // Number of trackCand matched to ShowerHit
    Int_t mmTof[200][256];         // index of HMetaMatch obj. for Tof Matching
    Int_t mmShr[200][256];         // index of HMetaMatch obj. for Shower Matchint
    Float_t qualTof;               // quality of matching
    Float_t qualShower;            // quality of matching
    Float_t meta_dx;               // x-coordinate deviation between META and outer MDC
    Float_t meta_dy;               // y-coordinate deviation between META and outer MDC

    Char_t sector;                 // current sector                           
    Int_t nShowerHitsSec;          // == nShowerHits[sec] for current sector
//    HGeomVector* showerHitsSec;  // ...
    HShowerHit** fShowerHitsSec;   // ...
    Short_t* indexShrHitSec;       // ...
    Int_t nTofHitsSec;             // == nTofHits[sec] for current sector    
    HGeomVector* tofHitsSec;       // ...
    Short_t* indexTofHitSec;       // ...
    Char_t*  tofClustSizeSec;
//    Char_t* showerModuleSec;
    Char_t* tofModuleSec;
    const HGeomTransform* secLabTrans;

    Short_t aRichIndTable[RICH_TAB_SIZE];
    Short_t aRichIPUIndTable[RICH_TAB_SIZE];
    Float_t qualRich[RICH_TAB_SIZE];
    Int_t nRichId;
    Int_t nRichIPUId;

    Float_t  rad2deg;   
    
    HLocation locTofHit;
    
  public:
    HMetaMatchF(void);
    HMetaMatchF(Text_t name[], Text_t title[]);
    ~HMetaMatchF();
    Int_t execute();
    Bool_t init();
    Bool_t reinit();
    Bool_t finalize();
    
  private:
    void setInitParam(void);
    void makeRichMatching(void);
    void collectTofHits(void);
    void collectShowerHits(void);
    void makeOuterSegMatch(HMdcTrkCand* pTrkCand);
//    Int_t indShrOverlap(Int_t nTof);    // for overl. Tof-Shower, now is not used
    Int_t fillMeta(Int_t hShower, Int_t hTof, HMetaMatch* metaOld);
    Int_t getMetaMatchSlot(HMetaMatch* prevMM);
    Double_t qualityTof(Int_t hit,Float_t& dX_n, Float_t& dY_n);
    Double_t qualityShower(Int_t hit,Float_t& dX_n, Float_t& dY_n);
    Float_t qualityRich(Float_t mdcTheta,Float_t mdcPhi, HRichHit *pRich,Float_t &);
    Float_t qualityRichIPU(Float_t mdcTheta,Float_t mdcPhi, HRichHitIPU *pRich,Float_t &);
    void setCurrentSector(Int_t sec);
    void addTofCluster(HTofCluster* pTofCluster);
    void addTofHit(HTofHit* pTofHit,Int_t clSize=0);
    void addTof(HTofHit* pTofHit,Int_t index, Int_t clSize);
    void addRing(Float_t quality, Short_t ind, Short_t* indTable,Int_t& nRich);
    void fillMetaHitsInd(void);
    HMetaMatch* getMMObj(Int_t ind);
    void fillTofClstInd(void);
    HMetaMatch* setMetaHitsInd(HMetaMatch* meta, Int_t mmFirstInd, Int_t mmInd, 
        Int_t totNTrCnd);
    void setMatchingParam(void);
  public:
    ClassDef(HMetaMatchF,0)
};

#endif

