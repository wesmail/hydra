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
class HMdcSizesCellsSec;
class HSpecGeomPar;
class HMdcClus;
class HMdcCluster;
class HMdcClustersArrs;
class HMdcList12GroupCells;
class HMdcClFnStack;
class HMdcClFnStacksArr;
class HMdcSecListCells;

class HMdcLookUpTbCell: public TObject {
  protected:
    Short_t nLines;        // Number of lines (Y-dir.) in cell projections 
    Short_t line;          // Counter of lines. After filling should be eq. nLines
    Short_t yBinMin;       // First line in the projection
    Short_t yBinMax;       // Last line in the projection
    Short_t *xBinMin;      // First X bin in the projection for each Y line
    Short_t *xBinMax;      // Last X bin in the projection for each Y line
  public:
    HMdcLookUpTbCell(void);
    ~HMdcLookUpTbCell(void);
    void    clear(void) {line=0;}
    void    init(Int_t yBinMinT, Int_t yBinMaxT);
    Bool_t  addLine(Short_t nc1, Short_t nc2);
    Short_t getNLines(void) const        {return line;}
    Short_t getYBinMin(void) const       {return yBinMin;}
    Short_t getYBinMax(void) const       {return yBinMax;}
    Short_t getXBinMin(Short_t yb) const {return xBinMin[yb];}
    Short_t getXBinMax(Short_t yb) const {return xBinMax[yb];}
    ClassDef(HMdcLookUpTbCell,0)
};

class HMdcLookUpTbLayer: public TObject {
  protected:
    HMdcLookUpTbCell* array; // max 208
    Int_t      nCells;       // num. of cells in layer           
  public:
    HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer);
    ~HMdcLookUpTbLayer(void);
    HMdcLookUpTbCell& operator[](Int_t i) {return array[i];}
    Int_t getSize(void);

    ClassDef(HMdcLookUpTbLayer,0)
};


//-------Mod.-----------------------------

class HMdcLookUpTbMod : public TObject {
  protected:
    TObjArray* array;      // Array of HMdcLookUpTbLayer objects
    Int_t      nLayers;    // Number of working layers
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
  protected:
    Int_t       sector;             // sector number
    Int_t       segment;            // =0 -inner segment, -1 inner&outer
    Int_t       nSegments;          // num. of segments for clus. finder
    Int_t       nModules;           // num. of modules (eq.2 or 4)
    Bool_t      isCoilOff;
                                    // Size of plot:
    Int_t       nBinX;              // num.bins along axis X,
    Double_t    xLow;               // low edge of X
    Double_t    xUp;                // upper edge of X
    Double_t    xStep;              // step X = (xUp-xLow)/nBinX
    Int_t       nBinY;              // num.bins along axis Y
    Double_t    yLow;               // low edge of Y
    Double_t    yUp;                // upper edge of Y
    Double_t    yStep;              // step Y= (yUp-yLow)/nBinY
    Float_t     xFirstBin;          //
    Float_t     yFirstBin;          //
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

    Bool_t      mergeFlag;          // =KTRUE - merge clusters
    
    HMdcClFnStack*     stack;       // stack
    HMdcClFnStacksArr* stacksArr;   // array of stacks

    TObjArray*  array;              // array of pointers of type HMdcLookUpTbMod

    HCategory*  fClusCat;           // category of clusters
    HLocation   locClus;            // location of cluster category
    Int_t       counter[2];         // Counters of HMdcClus containers [segment]
    Bool_t      isGeant;            // =kTRUE - geant data
    Bool_t      fillTrackList;      // =kTRUE - filling tracks list in ClusSim
    Bool_t      noFiredCells;       // =kTRUE if no fired cells
    Int_t       maxAmp[4];          // number of fired layes in MDC
    
    HMdcSecListCells* pListCells;   // list of fired wires in current event
    
    Int_t       minAmp[4];          // max.clus. for mdc1,2,3,4
    Int_t       nMods;              // num. of modules in sector for cl.finding
    Int_t       typeClFinder;       // =0 - finding in all mdc in sec.
                                    // =1 - finding in each mdc indep.
                                    // =2 - combinedchamber+chamber clusters
    Int_t       neighbBins[8];
    HMdcPlane   prPlane;            // Projections plane
    Float_t     target[3];          // target
    Float_t     eTarg[3];           // errors of target position
    HGeomVector targVc[3];          // [0] First target point in sec.coor.sys.
                                    // [1] Last target point in sec.coor.sys.
                                    // [2] Middle point in sec.coor.sys.

    Int_t       nClusters;          // counter of clusters

    TH2C*       hist;               //
    Int_t       plBining;

    // -- Clusters parameters ---
    HMdcClustersArrs*   pClustersArrs; // clusters arraies
    Int_t               nModSeg[2];
    Int_t               clusArrSize;   // size of clusArr,clusArrM1,clusArrM2
    HMdcCluster*        clusArr;       // array of clusters parameters
    Int_t               nClsArr;       // counter of clusters in array

    static Short_t*     clusIndM1;     // array of MDC1 clusters indexis
    static Int_t        clIndArrSzM1;  // clusIndM1 array size
    HMdcCluster*        clusArrM1;     // array of clusters parameters in MDC1
    Int_t               nClsArrM1;     // counter of clusters in MDC1
    static Short_t*     clusIndM2;     // array of MDC2 clusters indexis
    static Int_t        clIndArrSzM2;  // clusIndM2 array size
    HMdcCluster*        clusArrM2;     // array of clusters parameters in MDC2
    Int_t               nClsArrM2;     // counter of clusters in MDC2
    
    Int_t               clusArrInd;    // current clus.array index:
                                       // 0-clusArr, 1-clusArrM1, 2-clusArrM2
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
    Int_t               nFirstClust;   // needed for clusters merger
    Bool_t              isSlotAv[2];   // =kFALSE if cluster slot not available

    Int_t               layerOrder[6]; // 

  public:
    HMdcLookUpTbMod& operator[](Int_t i) {
        return *static_cast<HMdcLookUpTbMod*>((*array)[i]);
    }
    Int_t    getSize(void);
    Int_t    getNClusters(void) const            {return nClusters;}
    Int_t    getMaxClus(Int_t m=-1) const;
    Int_t    getNBinX(void) const                {return nBinX;}
    Double_t getXlow(void) const                 {return xLow;}
    Double_t getXup(void) const                  {return xUp;}
    Int_t    getNBinY(void) const                {return nBinY;}
    Double_t getYlow(void) const                 {return yLow;}
    Double_t getYup(void) const                  {return yUp;}
    void     clearwk(void);
    Int_t    findClusters(Int_t* imax);
    void     setParPlane(const HMdcPlane &plane) {prPlane.setPlanePar(plane);}
    void     setTargetF(const HGeomVector& vec)  {targVc[0]=vec;}
    void     setTargetL(const HGeomVector& vec)  {targVc[1]=vec;}
    HMdcPlane& getPrPlane(void)                  {return prPlane;}
    const    HGeomVector& getTargetF(void)       {return targVc[0];}
    const    HGeomVector& getTargetL(void)       {return targVc[1];}
    TH2C*    fillTH2C(const Char_t* name,const Char_t* title, Int_t type=0,Int_t bining=2);
    void     setTypeClFinder(Int_t type)         {typeClFinder=type;}
    Int_t    getTypeClFinder(void)               {return typeClFinder;}
    Int_t    xBinNum(Double_t x)                 {return Int_t((x-xLow)/xStep);}
    Int_t    yBinNum(Double_t y)                 {return Int_t((y-yLow)/yStep);}
    Int_t    xBinNumInBounds(Double_t x);
    Int_t    yBinNumInBounds(Double_t y);
    Bool_t   calcXYBounds(Double_t& xL,Double_t& xU,Double_t& yL,Double_t& yU);
    void     setPrPlotSize(Double_t xL,Double_t xU,Double_t yL,Double_t yU);
    void     donotMergeClusters(void)            {mergeFlag=kFALSE;}
    
    HMdcLookUpTbSec(Int_t sec, Int_t nSegs, Int_t inBinX, Int_t inBinY);
    void     setClusCat(HCategory* cat)          {fClusCat  = cat;}
    void     setStack(HMdcClFnStack* st)         {stack     = st;}
    void     setStacksArr(HMdcClFnStacksArr* sA) {stacksArr = sA;}
    void     setCoilFlag(Bool_t flg)             {isCoilOff = flg;}
    void     calcTarget(HMdcSizesCellsSec& fSCSec,Double_t* targLenInc);
    Bool_t   calcLookUpTb(void);
    void     doNotFillTrackList(void)            {fillTrackList = kFALSE;}
    
  protected:
    HMdcLookUpTbSec(void) {}
    ~HMdcLookUpTbSec(void);
    Bool_t    fillLookUpTb(Int_t m, Int_t l,HMdcTrapPlane& cellPr,
                                            HMdcLookUpTbCell& fCell);
    void      clearPrArrs(void);
    void      clearPrMod(Int_t mod);
    void      fillClusCat(Int_t mod, Int_t segp, Int_t tpClFndr);
    void      findClusInSeg(Int_t seg);
    void      findClusInSec(void);
    void      findClusInMod(Int_t mod);
    Int_t     getClusterSlot(Int_t seg, HMdcList12GroupCells& list);
    void      fillModWiresList(Int_t mod, HMdcList12GroupCells& list);
    Bool_t    fillModCluster(Int_t mod);
    Bool_t    fillSegCluster(void);
    Bool_t    fillSecCluster(void);
    void      makeModPlot(Int_t mod);
    void      makeSPlot(void);
    void      makeLayProjV1(Int_t mod,Int_t layer);
    void      makeLayProjV1b(Int_t mod,Int_t layer);
    void      makeLayProjV2(Int_t mod,Int_t layer);
    void      findClusInSeg1(void);
    void      mergeClusInMod(Int_t mod);
    void      mergeClusMod1to2(void);
    void      mergeClusInSeg(void);
    void      mergeClusInSec(void);
    void      testClusMod12toSeg(void);
    void      scanPlotInMod(Int_t mod);
    Bool_t    calcMixedClusterFixedLevel(Int_t nBin);
    Bool_t    calcMixedClusterFloatLevel(Int_t nBin);
    void      scanPlotInSeg1(Int_t seg, UChar_t* plotBAr);
    Bool_t    calcClusterInSecFixedLevel(Int_t nBin);
    Bool_t    calcClusterInSecFloatLevel(Int_t nBin);
    Bool_t    calcClusterInSegFixedLevel(Int_t nBin);
    Bool_t    calcClusterInSegFloatLevel(Int_t nBin);
    Bool_t    calcClusterInSeg1FixedLevel(Int_t nBin);
    Bool_t    calcClusterInSeg1FloatLevel(Int_t nBin);
    void      testSeg1ModClMatching(void);
    Bool_t    calcClusterInModFixedLevel(Int_t nBin);
    Bool_t    calcClusterInModFloatLevel(Int_t nBin);
    void      calcClParam(void);

    void      initCluster(Int_t nBin);
    void      reinitCluster(Int_t nBin);
    void      initCluster(Int_t nBin,Int_t amp);
    void      reinitCluster(Int_t nBin,Int_t amp);
    void      addBinInCluster(Int_t nBin,UChar_t wt);
    void      initClusterT2(Int_t nBin,Int_t amp);
    void      reinitClusterT2(Int_t nBin,Int_t amp);
    void      addBinInClusterT2(Int_t nBin,UChar_t wt);
    Bool_t    increaseClusterNum(void);
    inline void   setCurrentArraySec(void);
    inline void   setCurrentArrayMod1(void);
    inline void   setCurrentArrayMod2(void);

  ClassDef(HMdcLookUpTbSec,0)
};

inline void HMdcLookUpTbSec::setCurrentArraySec(void) {
  cClusArr   = clusArr;
  cNClusArr  = &nClsArr;
  clusArrInd = 0;
}

inline void HMdcLookUpTbSec::setCurrentArrayMod1(void) {
  cClusArr   = clusArrM1;
  cNClusArr  = &nClsArrM1;
  clusArrInd = 1;
}

inline void HMdcLookUpTbSec::setCurrentArrayMod2(void) {
  cClusArr   = clusArrM2;
  cNClusArr  = &nClsArrM2;
  clusArrInd = 2;
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
    Bool_t               mergeFlag;     // =KTRUE - merge clusters
  public:
    static HMdcLookUpTb* getExObject(void) {return fMdcLookUpTb;}
    static HMdcLookUpTb* getObject(void);
    static void          deleteCont(void);
    static void          setUseFixedLevel(void)     {useFixedLevel=kTRUE ;}
    static void          setUseFloatLevel(void)     {useFixedLevel=kFALSE;}
    static Bool_t        getUseFixedLevelFlag(void) {return useFixedLevel;}
    HMdcLookUpTbSec& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbSec*>((*array)[i]);
    }
    Int_t  getSize(void);
    Bool_t init(HParIo* input,Int_t* set)           {return kTRUE;}
    virtual Bool_t initContainer(void);
    void   clear(void);
    Int_t  findClusters(Int_t *imax);
    void   setTargLenInc(Double_t lf,Double_t rt);
    void   setTypeClFinder(Int_t type);
    void   setPar(Int_t inBinX, Bool_t isCOff,Int_t inBinY=0);
    void   setQuietMode(Bool_t md=kTRUE)            {quietMode=md;}
    void   donotFillTrackList(void);
    void   donotMergeClusters(void)                 {mergeFlag=kFALSE;}
  protected:
    HMdcLookUpTb(const Char_t* name="MdcLookUpTb",
                 const Char_t* title="Look Up Table for Dubna track finding",
                 const Char_t* context="");
    ~HMdcLookUpTb(void);
    virtual Bool_t calcPrPlane(Int_t sec);
    virtual Bool_t calcPlotSize(Int_t sec);
    Bool_t   calcTarget(Int_t sec);

  ClassDef(HMdcLookUpTb,0)
};

#endif  /*!HMDCLOOKUPTB_H*/
