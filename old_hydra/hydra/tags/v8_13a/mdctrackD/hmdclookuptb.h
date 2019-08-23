#ifndef HMDCLOOKUPTB_H
#define HMDCLOOKUPTB_H

#include "TObject.h"
#include "hparset.h"
#include "hmdcgeomobj.h"
#include "hlocation.h"
#include <stdlib.h>

class TH2C;
class TObjArray;
class HCategory;
class HMdcGeomPar;
class HMdcDetector;
class HMdcGetContainers;
class HMdcLayerGeomPar;
class HMdcSizesCells;
class HSpecGeomPar;
class HMdcTrackDContFact;
class HMdcClus;
class HMdcCluster;
class HMdcList12GroupCells;
class HMdcClFnStack;
class HMdcClFnStacksArr;

class HMdcLookUpTbCell: public TObject {
  friend class HMdcLookUpTbSec;
  protected:
    Short_t nLines;
    Short_t line;
    Short_t yBinMin;
    Short_t yBinMax;
    Short_t *xBinMin;
    Short_t *xBinMax;
  public:
    HMdcLookUpTbCell(void);
    ~HMdcLookUpTbCell(void);
    void   clear(void) {line=0;}
    void   init(Int_t yBinMinT, Int_t yBinMaxT);
    Bool_t addLine(Short_t nc1, Short_t nc2);
    ClassDef(HMdcLookUpTbCell,0)
};

class HMdcLookUpTbLayer: public TObject {
  friend class HMdcLookUpTb;
  friend class HMdcLookUpTbSec;
  protected:
    TObjArray* array;
    Int_t      nCells;              // num. of cells in layer           
  public:
    HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer);
    ~HMdcLookUpTbLayer(void);
    HMdcLookUpTbCell& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbCell*>((*array)[i]);
    }
    Int_t getSize(void);

    ClassDef(HMdcLookUpTbLayer,0)
};


//-------Mod.-----------------------------

class HMdcLookUpTbMod : public TObject {
  protected:
    TObjArray* array;
    Int_t      nLayers;       // num. of working layers
  public:
    HMdcLookUpTbMod(Int_t sec, Int_t mod);
    ~HMdcLookUpTbMod(void);
    HMdcLookUpTbLayer& operator[](Int_t i) {
        return *static_cast<HMdcLookUpTbLayer*>((*array)[i]);
      }
    Int_t getSize(void);
    void  setNLayers(Int_t nl) {nLayers=nl;}
    Int_t getNLayers(void)     {return nLayers;}

  ClassDef(HMdcLookUpTbMod,0)
};

//-------Sec.-----------------------------
class HMdcLookUpTbSec : public TObject {
  friend class HMdcLookUpTb;
  protected:
    Int_t       sector;             // sector number
    Int_t       nSegments;          // num. of segments for clus. finder
                                    // Size of plot:
    Bool_t      isCoilOff;
    Int_t       nBinX;              // num.bins along axis X,
    Double_t    xLow;               // low edge of X
    Double_t    xUp;                // upper edge of X
    Double_t    xStep;              // step X = (xUp-xLow)/nBinX
    Int_t       nBinY;              // num.bins along axis Y
    Double_t    yLow;               // low edge of Y
    Double_t    yUp;                // upper edge of Y
    Double_t    yStep;              // step Y= (yUp-yLow)/nBinY
    Double_t    xHStep2;            // = (xStep/2)^2
    Double_t    yHStep2;            // = (yStep/2)^2
    Double_t*   xBinsPos;           // Bins positions
    Double_t*   yBinsPos;           //

    Int_t       size;               // project plot size
    static UChar_t* hPlMod[4];      // hPlMod[0] - mod.1, ...
    static Int_t    hPlModsSize;    // real length of hPlMod[mod] (can be >size)

    Int_t       sizeBAr;            // Size of the bit array plotBAr (in bytes)
    static Int_t sizeBArSt;         // sizeBArSt>=sizeBAr;
    static UChar_t* plotBArSc;      // Bit array (1 bit - 1 bin) (for scaning).
    Int_t       maxBinBAr4Sc;       // regiong of bins for scaning
    Int_t       minBinBAr4Sc;       //

    static UChar_t* plotBArM[4];    // Bit arraies for MDC's.
    Int_t       maxBinBAr4M[4];     //
    Int_t       minBinBAr4M[4];     //

    Int_t*      xMin[4];            // for cleaning proj. plots
    Int_t*      xMax[4];            //

    Bool_t      setLUpTb;           // kTRUE or kFALSE
    
    HMdcClFnStack*     stack;       // stack
    HMdcClFnStacksArr* stacksArr;   // array of stacks

    TObjArray*  array;              // array of pointers of type HMdcLookUpTbMod

    HCategory*  fClusCat;           // category of clusters
    HLocation   locClus;            // location of cluster category
    Int_t       counter[2];         // Counters of HMdcClus containers [segment]
    Bool_t      isGeant;            // =kTRUE - geant data
    Bool_t      fillTrackList;      // =kTRUE - filling tracks list in ClusSim
    Short_t     nHitsTot;           // total num. of hits in sector
    Int_t       maxAmp[4];          // number of fired layes in MDC
    Int_t       nHits[24];          //
    Short_t     hits[24][250];      // n. cell
    Short_t     hitsNTime[24][250]; // n.times,
    Short_t     hitsDel[24][250];   // 0 - hit will put in plot
	                            // 1 - time2 was deleted
	                            // 2 - time1 was deleted
	                            // -1 - hit was deleted
    Int_t       minAmp[4];          // max.clus. for mdc1,2,3,4
    Int_t       nMods;              // num. of modules in sector for cl.finding
    Int_t       typeClFinder;       // =0 - finding in all mdc in sec.
                                    // =1 - finding in each mdc indep.
                                    // =2 - combinedchamber+chamber clusters
    Int_t       neighbBins[8];
    HMdcPlane   prPlane;            // Projections plane
    Float_t     target[3];          // target
    Float_t     eTarg[3];           // errors of target position
    HGeomVector targVc[2];          // [0] First target point in sec.coor.sys.
                                    // [1] Last target point in sec.coor.sys.

    Int_t       nClusters;          // counter of clusters

    TH2C*       hist;               //
    Int_t       plBining;

    // -- Clusters parameters ---
    Int_t               nModSeg[2];
    Int_t               clusArrSize;   // size of clusArr,clusArrM1,clusArrM2
    HMdcCluster*        clusArr;       // array of clusters parameters
    Int_t               nClsArr;       // counter of clusters in arraies

    static Short_t*     clusIndM1;     // array of MDC1 clusters indexis
    static Int_t        clIndArrSzM1;  // clusIndM1 array size
    HMdcCluster*        clusArrM1;     // array of clusters parameters in MDC1
    Int_t               nClsArrM1;     // counter of clusters in MDC1
    static Short_t*     clusIndM2;     // array of MDC2 clusters indexis
    static Int_t        clIndArrSzM2;  // clusIndM2 array size
    HMdcCluster*        clusArrM2;     // array of clusters parameters in MDC2
    Int_t               nClsArrM2;     // counter of clusters in MDC2
    
    HMdcCluster*        cClusArr;      // current clusters array
    Int_t*              cNClusArr;     // current clusters array counter
    Int_t               cSeg;
    Int_t               cMod1;
    Int_t               cMod2;
    UChar_t*            cPlModF;
    UChar_t*            cPlModS;
    Int_t               cMod;
    UChar_t*            cPlMod;
    Short_t*            clusInd;
        
    HMdcLookUpTbMod*    cFMod;         // current module
    UChar_t*            cHPlModM;      // current mod.pr.plot
    HMdcCluster*        clus;          // current cluster
    Bool_t              isClstrInited; // flag for "clus" object
    Int_t*              cXMinM;        // current xMin[mod]
    Int_t*              cXMaxM;        // current xMax[mod]
    Int_t*              pXMinM;        // xMin[mod] for previous mdc
    Int_t*              pXMaxM;        // xMax[mod] for previous mdc
    UChar_t*            cPlotBAr;      // current bit array

    Int_t               nLMaxCl;       // cluster region on proj.plane (bins)
    Int_t               nLMinCl;       // nLMinCl-nLMaxCl region of lines
    Short_t*            xMaxCl;        // regions of x bins for each line
    Short_t*            xMinCl;        // in nLMinCl-nLMaxCl region

    HMdcClus*           fClus;         // pointer to cluster;
    Int_t               nFirstClust;   // needed for clusters merging
    Bool_t              isSlotAv[2];   // =kFALSE if cluster slot not available

  public:
    HMdcLookUpTbMod& operator[](Int_t i) {
        return *static_cast<HMdcLookUpTbMod*>((*array)[i]);
    }
    Int_t    getSize(void);
    Int_t    getNClusters(void)                  {return nClusters;}
    Int_t    getMaxClus(Int_t m=-1);
    Int_t    getNBinX(void)                      {return nBinX;}
    Int_t    getNHitsTot(void)                   {return nHitsTot;}
    Double_t getXlow(void)                       {return xLow;}
    Double_t getXup(void)                        {return xUp;}
    Int_t    getNBinY(void)                      {return nBinY;}
    Double_t getYlow(void)                       {return yLow;}
    Double_t getYup(void)                        {return yUp;}
    void     clearwk(void);
    void     setCell(Short_t mod, Short_t layer, Short_t cel, Short_t nTimes);
    Int_t    findClusters(Int_t* imax);
    void     deleteHit(Short_t mod, Short_t layer, Short_t cell, Short_t nTime);
    void     setParPlane(const HMdcPlane &plane) {prPlane.setPlanePar(plane);}
    void     setTargetF(const HGeomVector& vec)  {targVc[0]=vec;}
    void     setTargetL(const HGeomVector& vec)  {targVc[1]=vec;}
    const    HMdcPlane& getPrPlane(void)         {return prPlane;}
    const    HGeomVector& getTargetF(void)       {return targVc[0];}
    const    HGeomVector& getTargetL(void)       {return targVc[1];}
    TH2C*    fillTH2C(Char_t* name, Char_t* title, Int_t type=0,Int_t bining=2);
    void     setTypeClFinder(Int_t type)         {typeClFinder=type;}
    Int_t    getTypeClFinder(void)               {return typeClFinder;}

  protected:
    HMdcLookUpTbSec(void) {}
    HMdcLookUpTbSec(Int_t sec, Int_t nSegs, Int_t inBinX, Int_t inBinY);
    ~HMdcLookUpTbSec(void);
    void     clearPrArrs(void);
    void     clearPrMod(Int_t mod);
    void     calcMaxAmp(void);
    void     fillClusCat(Int_t mod, Int_t segp, Int_t tpClFndr);
    void     findClusInSeg(Int_t seg);
    void     findClusInSec(void);
    void     findClusInMod(Int_t mod);
    Int_t    getClusterSlot(Int_t seg, HMdcList12GroupCells& list);
    void     fillModWiresList(Int_t mod, HMdcList12GroupCells& list);
    Bool_t   fillModCluster(Int_t mod);
    Bool_t   fillSegCluster(void);
    Bool_t   fillSecCluster(void);
    void     makeModPlot(Int_t mod);
    void     makeSPlot(void);
    void     makeLayProjV1(Int_t layer, Int_t indLay);
    void     makeLayProjV1b(Int_t layer, Int_t indLay);
    void     makeLayProjV2(Int_t layer, Int_t indLay);
    void     findClusInSeg1(void);
    void     mergeClusInMod(Int_t mod);
    void     mergeClusMod1to2(void);
    void     mergeClusInSeg(void);
    void     mergeClusInSec(void);
    void     testClusMod12toSeg(void);
    void     scanPlotInMod(Int_t mod);
    Bool_t   calcMixedClusterFixedLevel(Int_t nBin);
    Bool_t   calcMixedClusterFloatLevel(Int_t nBin);
    void     scanPlotInSeg1(Int_t seg, UChar_t* plotBAr);
    Bool_t   calcClusterInSecFixedLevel(Int_t nBin);
    Bool_t   calcClusterInSecFloatLevel(Int_t nBin);
    Bool_t   calcClusterInSegFixedLevel(Int_t nBin);
    Bool_t   calcClusterInSegFloatLevel(Int_t nBin);
    Bool_t   calcClusterInSeg1FixedLevel(Int_t nBin);
    Bool_t   calcClusterInSeg1FloatLevel(Int_t nBin);
    void     testSeg1ModClMatching(void);
    Bool_t   calcClusterInModFixedLevel(Int_t nBin);
    Bool_t   calcClusterInModFloatLevel(Int_t nBin);
    void     calcClParam(void);

    void     initCluster(Int_t nBin);
    void     reinitCluster(Int_t nBin);
    void     initCluster(Int_t nBin,Int_t amp);
    void     reinitCluster(Int_t nBin,Int_t amp);
    void     addBinInCluster(Int_t nBin,Char_t wt);
    void     initClusterT2(Int_t nBin,Int_t amp);
    void     reinitClusterT2(Int_t nBin,Int_t amp);
    void     addBinInClusterT2(Int_t nBin,Char_t wt);
    Bool_t   increaseClusterNum(void);
    inline void setCurrentArray(HMdcCluster* clArr, Int_t* nClArr);

  ClassDef(HMdcLookUpTbSec,0)
};

inline void HMdcLookUpTbSec::setCurrentArray(HMdcCluster* clArr, Int_t* nClArr) {
  cClusArr  = clArr;
  cNClusArr = nClArr;
}

class HMdcLookUpTb : public HParSet {
  protected:
    static HMdcLookUpTb* fMdcLookUpTb;
    TObjArray*           array;         // array of HMdcLookUpTbSec objects
    Int_t                nBinX;         // project plot sizes
    Int_t                nBinY;         // -/-
    Bool_t               isCoilOff;
    HMdcGetContainers*   fGetCont;
    HMdcSizesCells*      fSizesCells;
    HMdcGeomPar*         fMdcGeomPar;
    HMdcDetector*        fMdcDet;
    HSpecGeomPar*        fSpecGeomPar;
    HMdcLayerGeomPar*    fLayerGeomPar;
    HCategory*           fMdcClusCat;   // category of clusters
    Double_t             targLenInc[2];
    Bool_t               quietMode;
    HMdcClFnStack*       stack;         // stack
    HMdcClFnStacksArr*   stacksArr;     // array of stacks
    static Bool_t        useFixedLevel; //
  public:
    static HMdcLookUpTb* getExObject(void) {return fMdcLookUpTb;}
    static HMdcLookUpTb* getObject(void);
    static void deleteCont(void);
    static void          setUseFixedLevel(void)     {useFixedLevel=kTRUE ;}
    static void          setUseFloatLevel(void)     {useFixedLevel=kFALSE;}
    static Bool_t        getUseFixedLevelFlag(void) {return useFixedLevel;}
    HMdcLookUpTbSec& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbSec*>((*array)[i]);
    }
    Int_t  getSize(void);
    Bool_t init(HParIo* input,Int_t* set)          {return kTRUE;}
    Bool_t initContainer(void);
    void   clear(void);
    Int_t  findClusters(Int_t *imax);
    void   setTargLenInc(Double_t lf,Double_t rt);
    void   setTypeClFinder(Int_t type);
    void   setPar(Int_t inBinX, Bool_t isCOff);
    void   setQuietMode(Bool_t md=kTRUE)           {quietMode=md;}
    void   donotFillTrackList(void);
  protected:
    HMdcLookUpTb(const char* name="MdcLookUpTb",
                 const char* title="Look Up Table for Dubna track finding",
                 const char* context="");
    ~HMdcLookUpTb(void);
    Bool_t   calcPrPlane(Int_t sec);
    Bool_t   calcTarget(Int_t sec);
    Bool_t   calcPlotSize(Int_t sec);
    Bool_t   calcCellsProj(Int_t sec);
    Double_t calcDist(HGeomVector& p1, HGeomVector& p2);
    void     calcPoint(HMdcPointPlane& pr, HGeomVector& p11, HGeomVector& p21,
                                           HGeomVector& p12, HGeomVector& p22);
    Bool_t   calcCellProj(Int_t sec,HMdcTrap& cellSize,HMdcTrapPlane& cellProj);
    Bool_t   calcLookUpTb(Int_t nSec);
    void     calcX(HMdcTrapPlane& pr,Double_t y, Double_t &xLow,Double_t &xUp);
  ClassDef(HMdcLookUpTb,0)
};

#endif  /*!HMDCLOOKUPTB_H*/
