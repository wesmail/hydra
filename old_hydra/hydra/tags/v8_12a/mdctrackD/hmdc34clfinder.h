#ifndef HMDC34CLFINDER_H
#define HMDC34CLFINDER_H

#include "TObject.h"
#include "hlocation.h"
#include "hmdclistgroupcells.h"
#include "hmdclistcells.h"
#include "hmdcgeomobj.h"
#include "hparset.h"
#include "hgeomvector.h"

class HMdcClus;
class HMdcSeg;
class TH2C;
class HKickPlane2;
class HMdcGetContainers;
class HMdcDetector;
class HSpecGeomPar;
class HMdcSizesCells;
class HMdcGeomPar;
class HCategory;
class HMdcClFnStack;
class HMdcClFnStacksArr;
class HMdcCluster;

class HMdc34ClFinderLayer: public TObject {
  protected:
  public:
    // projection of wire (one for all wires in layer)
    HMdcLayListCells* cells;    // list of fired wires
    Short_t  nBinX;             // projection of wire (one for layer)
    Short_t* yBin;              // Y bins for X bins  0 - nBinX
    Int_t    yFirst;            // yFirst=yBin[0] or [nBinx-1]
    Int_t    nYLines;           // num. of lines
    Int_t    wOrType;           // wires orientation type
    Short_t* xBin1;             // size of xBin array = nYLines + 1
    Short_t* xBin2;             // size of xBin array = nYLines + 1
    Int_t    nCells;            // num.of cells in layer
    Double_t tgY;               // (y1-y2)/(x1-x2) one for all cells
    Double_t tgZ;               // (z1-z2)/(x1-x2) one for all cells
    Double_t y0[2];             // Let x,y,z - point on the kick plane, then:
    Double_t z0[2];             // y'=tgY*x+nCell*yStep+y0[n] n=0 left rib
    Double_t yStep;             //          (nearest to tag)
    Double_t zStep;             // z'=tgZ*x+nCell*zStep+z0[n] n=1 right rib
    Int_t    nPSegOnKick[2][2]; // first index [0]-up rib; [1]-down rib;
                                // second index [0]-lift rib; [1]-right rib;
    Short_t* cellShUp;          // cellShUp(Down)[nCells] keep shift of wire
    Short_t* cellShDown;        // proj.(*yBin). Recalculated for each cluster.

    HMdc34ClFinderLayer(Int_t sec, Int_t mod, Int_t layer);
    ~HMdc34ClFinderLayer();
    Bool_t createArrayBins(Short_t nBins);
    void   setCellsList(HMdcLayListCells& event) {cells = &event;}

    ClassDef(HMdc34ClFinderLayer,0)
};

//-------Mod.-----------------------------

class HMdc34ClFinderMod : public TObject {
  protected:
    TObjArray* array;
  public:
    HMdc34ClFinderMod(Int_t sec, Int_t mod);
    ~HMdc34ClFinderMod();
    HMdc34ClFinderLayer& operator[](Int_t i) {
        return *static_cast<HMdc34ClFinderLayer*>((*array)[i]);
    }
    Int_t getNCells(void);
    void  setCellsList(HMdcModListCells& event);
        
    ClassDef(HMdc34ClFinderMod,0)
};

//-------Plot------------------------------
class HMdcProjPlot : public HMdcPlane {
  protected:
  public:
    Double_t  yMin,yMax;     // The sizes of the plot.
    Double_t  xMin,xMax;
    Double_t  xMinD,xMaxD;   // The small side of the MDC's proj.
    Short_t   nBinX;         //  num.bins along axis X (xMin-xMax)
    Double_t  stX;           //  step X = (xMax-xMin)/nBinX
    Short_t   nBinY;         //  num.bins along axis Y
    Double_t  stY;           //  step Y= (yMax-yMin)/nBinY
    Short_t*  xMinL;         // In units of bin X num.
    Short_t*  xMaxL;
    Short_t*  yMinL;         // yMaxL[nx]=nBinY-1
    Double_t* xBinsPos;      // Bins positions
    Double_t* yBinsPos;      //
    Double_t  xHStep2;       // = (stX/2)^2
    Double_t  yHStep2;       // = (stY/2)^2

    UChar_t*  plModF;        // first module in segment project plot
    UChar_t*  plModS;        // second module in segment project plot
    Int_t     size;          // The size of the plModF[] and plModS[]
    Int_t     sizeBAr;       // The size of the bit array plotBAr (in bytes)
    UChar_t*  plotBArSc;     // Bit array (1 bit - 1 bin in plot) (for scaning)
    Int_t*    plotBArSc4b;   // plotBArSc4b=plotBArSc
    static Char_t* weights;  // array of pr.plot bins weights
    static Int_t wtArrSize;  // weights array size
    Int_t     minBin4Sc;     // bins region in plotBArSc for scaning
    Int_t     maxBin4Sc;     //

    TH2C*     rootPlot;
    TH2C*     rootPlotV2;

    HMdcProjPlot(UChar_t mSeg, Int_t inBinX, Int_t inBinY);
    ~ HMdcProjPlot();
    void   setEdges(Double_t iyMin, Double_t ixMinD, Double_t ixMaxD,
                   Double_t iyMax, Double_t ixMin,  Double_t ixMax);
    void   print(void);
    void   clearArr(void);
    void   clearBitArr(void);
    Int_t  xbin(Int_t bin) const {return bin%nBinX;}
    Int_t  ybin(Int_t bin) const {return bin/nBinX;}
    Bool_t testBin(Int_t nb)    {return HMdcBArray::testAndUnset(plotBArSc,nb);}
    TH2C*  getPlot(Char_t* name, Char_t* title);
    TH2C*  getPlotV2(Char_t* name, Char_t* title);

    ClassDef(HMdcProjPlot,0)
};

//-------Sec.------------------------------
class HMdc34ClFinderSec : public TObject {
  friend  class HMdc34ClFinder;
  protected:
    TObjArray*         array;         // array of HMdc34ClFinderMod objects
    Int_t              sector;
    Int_t              lMods[4];      // list of mdc's
    UChar_t            mSeg[2];       // mSeg[0,1] -segment 0,1
    HKickPlane2*       fkick;
    HGeomVector        target[2];     // Target in sec.coor.sys.
    HMdcProjPlot*      prPlotSeg2;    // pr.plot of MDC3-4
    HGeomVector        seg1[5];       // [0-3] -region, [4] - hit
    HGeomVector        dirSeg1[5];    // dirSeg1[4] - errors of seg1[4]
    HGeomVector        segOnKick[5];  // region of HMdcSeg-hit on the kick plane
    Double_t           al[4];
    Double_t           bl[4];
    Double_t           cl[4];
    Int_t              nbX[4];

    Int_t              minAmp[4];     // minAmp[2]-mdc3, minAmp[3]-mdc4
    Int_t              maxAmp[4];     // number of fired layes in MDC's
    Bool_t             notEnoughWrs;  // =kTRUE - not enough fired wires

    Int_t              nearbyBins[8]; // Lookup table for finding of nearby bins
    HMdcClFnStack*     stack;         // stack
    HMdcClFnStacksArr* stacksArr;     // array of stacks

    Short_t*           xMinClLines;
    Short_t*           xMaxClLines;
    Int_t              nLMinCl;
    Int_t              nLMaxCl;
    Bool_t             isClstrInited; // flag for "clus" object

    Int_t              counter;       // Counter of HMdcClus containers (loc[2])
    Int_t              indexPar;     
    Int_t              indexFCh;     
    Int_t              indexLCh;     

    Int_t*             xCMin[4];      // for cleaning proj. plots
    Int_t*             xCMax[4];      // [2] & [3] are using now
    Int_t*             cXMinM;        // current pointer to xCMin[mod]
    Int_t*             cXMaxM;        // -/-
    Int_t*             xMinM2;        // =xMin[2]
    Int_t*             xMaxM2;        // =xMax[2]
    UChar_t*           cPlMod;        // current mod.pr.plot
    HMdc34ClFinderMod*   cFMod;       // current module
    HMdc34ClFinderLayer* cFLay;       // current layer
    Int_t           nBinX;            // X size of plot

    Int_t           cell;             // var. for making layers proj.
    Int_t           nBinYM2;          //
    Int_t           shUp;             //
    Int_t           shDown;           //
    UChar_t         add;              //
    Int_t           nYLinesM1;        //
    Short_t*        xBin1L;           //
    Short_t*        xBin2L;           //
    Int_t           shUpN;            //
    Int_t           shDownN;          //
    Bool_t          test;             //
    Int_t           ny1;              //
    Int_t           ny2;              //
    Int_t           nbL;              //
    Int_t           nbF;              //

    HCategory*      fClusCat;         
    HLocation       locClus;
    Bool_t          isGeant;          
                                      
    // -- Clusters parameters ---     
    Int_t           typeClFinder;     // !=2 - cl.finding type for each mdc
                                      // =2  - cl.finding type for two mdc
    Int_t           realTypeClFinder; // =2 or 0
    
    Int_t           clusArrSize;      // size of clusArr,clusArrM1,clusArrM2
    HMdcCluster*    clusArr;          // array of clusters parameters
    Int_t           nClsArr;          // counter of clusters in arraies
    HMdcCluster*    cluster;          // current cluster

  protected:
    HMdc34ClFinderSec(void) {}
    HMdc34ClFinderSec(Int_t sec, Int_t inBinX, Int_t inBinY);
    ~HMdc34ClFinderSec();
    void   makeModS2Plot(Int_t mod);
    void   makeSeg2Plot(void);
    void   makeSeg2PlotTp2(void);
    Int_t  calcYbin(Int_t upDo,Int_t leRi,Int_t cell);
    Int_t  scanPlotSeg2(void);
    Bool_t calcClusterSeg2FixedLevel(Int_t nBin);
    Bool_t calcClusterSeg2FloatLevel(Int_t nBin);
    Bool_t fillClusterSeg2(void);
    Int_t  findClustersSeg2(void);
    void   initCluster(Int_t nBin,Int_t amp);
    void   reinitCluster(Int_t nBin,Int_t amp);
    void   addBinInCluster(Int_t nBin,Char_t wt);
    Bool_t increaseClusterNum(void);
    void   mergeClustSeg2(void);
    Int_t  fillClusCat(void);
    void   setLayerVar(Int_t lay);
    Bool_t setNextCell(void);
    void   setYLimits(void);
    Bool_t setXLimits(Int_t ny, Short_t nx1,Short_t nx2);
    void   makeLayProjV1(Int_t lay);
    void   makeLayProjV1b(Int_t lay);
    void   makeLayProjV2(Int_t lay);
    void   clearPrMod(Int_t mod=-1);
    void   clearPrSegTp2(void);
    Bool_t testMaxAmp(void);

  public:
    HMdc34ClFinderMod& operator[](Int_t i) {
        return *static_cast<HMdc34ClFinderMod*>((*array)[i]);
    }
    void   clear(void);
    void   setMinBin(Int_t *mBin);
    Bool_t notEnoughWires(void) const         {return notEnoughWrs;}
    Int_t  findClustersSeg2(HMdcSeg* fSeg,
                            HMdcClus* fClus,
                            Int_t *mBin=0);
    Int_t  findClustersSeg2(HMdcClus* fClus,
                            Int_t *mBin=0);
    TH2C*  getPlot(Char_t* name,
                   Char_t* title,Int_t ver=0);
    HMdcProjPlot* getPlotSeg2(void)           {return prPlotSeg2;}

    Int_t  getNBinX(void)                     {return prPlotSeg2->nBinX;}
    Int_t  getNBinY(void)                     {return prPlotSeg2->nBinY;}
    void   setTargetF(const HGeomVector& vec) {target[0]=vec;}
    void   setTargetL(const HGeomVector& vec) {target[1]=vec;}
    const  HGeomVector& getTargetF(void)      {return target[0];}
    const  HGeomVector& getTargetL(void)      {return target[1];}
    void   setTypeClFinder(Int_t type)        {typeClFinder=type;}
    Int_t  getTypeClFinder(void)              {return typeClFinder;} 
    void   setCellsList(HMdcSecListCells& event);
    
    ClassDef(HMdc34ClFinderSec,0)
};

//----------------------------------------------------------
class HMdc34ClFinder : public HParSet {
  protected:
    static HMdc34ClFinder* fMdc34ClFinder;
    TObjArray*             array;         // array of HMdc34ClFinderSec objects
    HMdcGetContainers*     fGetCont;
    HMdcDetector*          fMdcDet;
    HSpecGeomPar*          fSpecGeomPar;
    HMdcSizesCells*        fSizesCells;
    HKickPlane2*           fKickPlane;
    HMdcGeomPar*           fMdcGeomPar;
    HCategory*             fMdcClusCat;
    Short_t*               xMinClLines;
    Short_t*               xMaxClLines;
    HMdcClFnStack*         stack;         // stack
    HMdcClFnStacksArr*     stacksArr;     // array of stacks
    static Bool_t          quietmode;
    static Bool_t          useFixedLevel; //
  public:
    static HMdc34ClFinder* getExObject(void);
    static HMdc34ClFinder* getObject(void);
    static void            deleteCont(void);
    static void            setQuietMode(Bool_t quiet) {quietmode=quiet;}
    static Bool_t          getQuietMode()             {return quietmode;}
    static void            setUseFixedLevel(void)     {useFixedLevel=kTRUE ;}
    static void            setUseFloatLevel(void)     {useFixedLevel=kFALSE;}
    static Bool_t          getUseFixedLevelFlag(void) {return useFixedLevel;}
    HMdc34ClFinderSec& operator[](Int_t i) {
      return *static_cast<HMdc34ClFinderSec*>((*array)[i]);
    }
    Bool_t init(HParIo* input,Int_t* set)             {return kTRUE;}
    Bool_t initContainer(HMdcEvntListCells& event);
    void   setCellsList(HMdcEvntListCells& event);
    void   clear(void);
  protected:
    HMdc34ClFinder(const char* name    = "Mdc34ClFinder",
                   const char* title   = "Cluster finder for outer MDCs",
                   const char* context = "");
    ~HMdc34ClFinder();
    Bool_t   calcTarget(Int_t sec);
    Bool_t   calcProjPlaneSeg2(Int_t sec);
    Bool_t   calcSizePlotSeg2(Int_t sec);
    Bool_t   calcWiresProj(Int_t sec);
    void     calcCrossLines(HGeomVector& p1l1, HGeomVector& p2l1,
                            HGeomVector& p1l2, HGeomVector& p2l2,
                            HGeomVector& cross);
    Double_t xLine(HGeomVector& p1, HGeomVector& p2, Double_t yi);

    ClassDef(HMdc34ClFinder,0)
};


#endif  /*!HMDC34CLFINDER_H*/
