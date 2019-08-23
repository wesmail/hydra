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
class HIterator;
class HMdcTrkCand;
class HMdcTrackGSpline;
class HMetaMatch;
class HGeomTransform;
class HKickPlane2;
class HMdcGetContainers;
class HRichHit;
class HRichHitIPU;



class HMetaMatchF : public HReconstructor {
  private:
    HGeomVector metaDir;
    HGeomVector metaDirShower;
    Bool_t condGeant;
    Int_t totalNumber;
    Float_t sigmaTofX;
    Float_t sigmaTofY;
    Float_t sigmaShowerX;
    Float_t sigmaShowerY;
    Float_t sTofX;
    Float_t sTofY;
    Float_t sShowerX;
    Float_t sShowerY;
    Float_t qualityTOF;
    Float_t qualitySHOWER;
    HMdcSeg *segments[2];
    Short_t trkCandIndex;
    const HGeomTransform *tRans[6];
    HGeomVector POINTS[4];
    HMdcGetContainers *fGetCont;
    HMetaMatch *meta;
    HCategory *fCatTrkCand;
    HCategory *fCatMetaMatch;
    HCategory *fCatMdcSeg;
    HCategory *fCatTof;
    HCategory *fCatShower;
    HCategory *fCatRich;
    HCategory *fCatRichIPU;
    HCategory *fCatTofCluster;
  
    HKickPlane2 *kickplane;
//     HMdcGeomPar *fMdcGeometry;
//     HSpecGeomPar *fSpecGeomPar;
    HMdcTrackGSpline *Spline;
    HIterator *fTrkCandIter;
    HIterator *IterTof;
    HIterator *IterShower;
    HIterator *IterRich;
    HIterator *IterRichIPU;
    HIterator *IterTofCluster;
    HLocation sectorloc;
    
    Int_t nShowerHits[6];
    HGeomVector showerHits[6][200];
    Short_t indexShrHit[6][200];
    Char_t  showerIsUsed[6][200];
    Char_t  showerIsInOver[6][200];

    Int_t nTofHits[6];
    HGeomVector tofHits[6][200];
    Short_t indexTofHit[6][200];
    Char_t  tofIsUsed[6][200];

     Char_t tofClustSize[6][200];
     Char_t*  tofClustSizeSec;

    Char_t sector;                    // current sector
    Int_t nShowerHitsSec;             // == nShowerHits[sec] for current sector
    HGeomVector* showerHitsSec;       // ...
    Short_t* indexShrHitSec;          // ...
    Char_t*  showerIsUsedSec;         // ...
    Char_t*  showerIsInOverSec;       // ...
    Int_t nTofHitsSec;                // == nTofHits[sec] for current sector
    HGeomVector* tofHitsSec;          // ...
    Short_t* indexTofHitSec;          // ...
    Char_t*  tofIsUsedSec;            // ...

    Short_t aRichIndTable[RICH_TAB_SIZE];
    Short_t aRichIPUIndTable[RICH_TAB_SIZE];
    Char_t nRichId;
    Char_t nRichIPUId;
    Float_t dThRich;                   // (matching window)^2
    Float_t dPhRich;                   // ...
    Float_t dThRichIPU;                // ...
    Float_t dPhRichIPU;                // ...

    Float_t  rad2deg;   
    
    
    
    HCategory *fCatKickTrack;
    HCategory *fCatKickTrackB;
    HIterator *iterMetaMatch;
    HIterator *iterKickTrack;
    HLocation locKickTrack;
    HLocation locMetaMatch;
    HLocation locTofHit;
    HLocation segLoc;
    
  public:
    HMetaMatchF(void);
    HMetaMatchF(Text_t name[], Text_t title[]);
    void setInitParam(void);
    ~HMetaMatchF();
    Int_t execute();
    Bool_t init();
    Bool_t reinit();
    Bool_t finalize();
    HMetaMatch *fillMeta(Int_t hShower, Int_t hTof);

    void richMatching(void);
    Bool_t isRichMdcCorrelation(Float_t mdcTheta,Float_t mdcPhi, HRichHit *pRich);
    Bool_t isRichMdcCorrelation(Float_t mdcTheta,Float_t mdcPhi, HRichHitIPU *pRich);

    void collectTofHits(void);
    void collectShowerHits(void);
    void setCurrentSector(Int_t sec);
    Int_t makeOuterSegMatch(Short_t index2);
    Int_t makeInnerSegMatch(Int_t firstCandInd);
    Int_t indShrOverlap(Int_t nTof);
    Double_t qualityTof(Int_t hit);
    Double_t qualityShower(Int_t hit);
    void clearShrInOverlap(void);
    void clearIsUsedArr(void);
    
    
    Bool_t initTrMatch();
    Int_t executeTrMatch();

    void addTofCluster(HTofCluster* pTofCluster);
    void addTofHit(HTofHit* pTofHit,Int_t clSize=0);


    ClassDef(HMetaMatchF,0)
};
#endif

