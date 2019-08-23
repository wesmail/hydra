#ifndef HMDCLOOKUPTB_H
#define HMDCLOOKUPTB_H

#include "TObject.h"
#include "hparset.h"
#include "hmdcgeomobj.h"
#include "hlocation.h"
#include "hmdclistcells.h"

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
    HMdcLookUpTbCell();
    ~HMdcLookUpTbCell();
    void clear() { line=0; }
    void init(Int_t yBinMinT, Int_t yBinMaxT);
    Bool_t addLine(Short_t nc1, Short_t nc2);
    ClassDef(HMdcLookUpTbCell,0)
};

class HMdcLookUpTbLayer: public TObject {
  friend class HMdcLookUpTb;
  protected:
    TObjArray* array;
  public:
    HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer);
    ~HMdcLookUpTbLayer();
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
    Int_t nLayers;            // num. of working layers
  public:
    HMdcLookUpTbMod(Int_t sec, Int_t mod);
    ~HMdcLookUpTbMod();
    HMdcLookUpTbLayer& operator[](Int_t i) {
        return *static_cast<HMdcLookUpTbLayer*>((*array)[i]);
      }
    Int_t getSize(void);
    void setNLayers(Int_t nl) {nLayers=nl;}
    Int_t getNLayers(void) {return nLayers;}

  ClassDef(HMdcLookUpTbMod,0)
};
    
//----------HMdcClFnStack-----------------------------
class HMdcClFnStack : public TObject {
  protected:
    Int_t* stack;         // stack array
    Int_t  stackSize;     // stack size
    Int_t stackPos;
  public:
    HMdcClFnStack(Int_t size);
    ~HMdcClFnStack();
    void init(void) {stackPos=0;}
    void push(Int_t n) {stack[stackPos++]=n;}
    Int_t pop(void) {return (stackPos>0) ? stack[--stackPos] : -1;}
    void checkSize(void) {if(stackPos>=stackSize) increaseStack();}
    void increaseStack();
    
  ClassDef(HMdcClFnStack,0);
};
    
//----------HMdcCluster-------------------------------    
class HMdcCluster : public TObject {
  friend class HMdcLookUpTbSec;
  protected:
    Double_t radToDeg;
    HMdcList12GroupCells lCells1;
    HMdcList12GroupCells lCells2;
    Bool_t status;         // =kFALSE - removed by merging
    Short_t nMergedClus;   // Number of merged clusters
    Double_t sumWt;        // sum weghts of bins
    Int_t nBins;           // num. of bins in cluster
    Double_t meanX;        // var. for calculation of cluster shape
    Double_t meanY;        // -/-
    Double_t meanXX;       // -/-
    Double_t meanYY;       // -/-
    Double_t meanYX;       // -/-
    Double_t meanXWt;      // position of cluster (calc. with weghts)
    Double_t meanYWt;      // -/-
    Double_t meanXXWt;     // X dispersion
    Double_t meanYYWt;     // Y dispersion
    Double_t x;            // cluster position
    Double_t y;            //
    Float_t errX;          // position errors
    Float_t errY;          //
    Double_t sigma1;       // cluster shape
    Double_t sigma2;       //
    Float_t alpha;         //
    Int_t nLMax;           // cluster region on the project plane in bins
    Int_t nLMin;           // [0] - line on the plot with maximal nY (nLMax)
    Int_t* xMaxCl;         //
    Int_t* xMinCl;         //
    // Variables for matching:
    Int_t numSegCl;        // number of clusters in segment
    HMdcCluster* clusMod1; // mod.1 cluster address
    HMdcCluster* clusMod2; // mod.2 cluster address
  public:
    HMdcCluster();
    ~HMdcCluster();
    void clear(void);
    inline void addBin(Double_t x,Double_t y,Double_t wt);
    inline void addBin(Double_t x,Double_t y,Double_t wt,Int_t nx,Int_t ny);
    void calcXY(void) {
      x=meanXWt/sumWt;
      y=meanYWt/sumWt;
    }
    void addClus(HMdcCluster& clst2, Bool_t flag=kFALSE);
    void calcClusParam(Double_t xSt2, Double_t ySt2);
    void fillClus(HMdcClus* clus, Int_t nLst);
    void initArr(Int_t size);
    inline Bool_t hasOverlap(HMdcCluster& cls2);
    
  ClassDef(HMdcCluster,0)
};

void HMdcCluster::addBin(Double_t x,Double_t y,Double_t wt) {
  sumWt += wt;
  meanX += x;
  meanY += y;
  meanXX += x*x;
  meanYY += y*y;
  meanYX += y*x;
  meanXWt += x*wt;
  meanYWt += y*wt;
  meanXXWt += x*x*wt;
  meanYYWt += y*y*wt;
  nBins++;
}

void HMdcCluster::addBin(Double_t x,Double_t y,Double_t wt, Int_t nx,Int_t ny) {
  sumWt += wt;
  meanX += x;
  meanY += y;
  meanXX += x*x;
  meanYY += y*y;
  meanYX += y*x;
  meanXWt += x*wt;
  meanYWt += y*wt;
  meanXXWt += x*x*wt;
  meanYYWt += y*y*wt;
  nBins++;
  // Finding of min.Xbin & max.Xbin for each line (y) in cluster
  if(nLMax<0) nLMax=nLMin=ny;
  else if(ny>nLMax) {
    if(ny-nLMax > 1) for(Int_t i=nLMax+1; i<ny; i++) xMinCl[i]=-1;
    nLMax=ny;
  } else if(ny<nLMin) {
    if(nLMin-ny > 1) for(Int_t i=ny+1; i<nLMin; i++) xMinCl[i]=-1;
    nLMin=ny;
  } else if(xMinCl[ny]>=0) {
    if(nx<xMinCl[ny]) xMinCl[ny]=nx;
    else if(nx>xMaxCl[ny]) xMaxCl[ny]=nx;
    return;
  }
  xMinCl[ny]=xMaxCl[ny]=nx;
}

Bool_t HMdcCluster::hasOverlap(HMdcCluster& cls2) {
  if(!cls2.status) return kFALSE;
  if(nLMax>cls2.nLMax || nLMin<cls2.nLMin) return kFALSE;
  for(Int_t ln=nLMax; ln>=nLMin; ln--) {
    if(xMinCl[ln]<0) continue;
    if(cls2.xMinCl[ln]<0) return kFALSE;
    if(xMinCl[ln]<cls2.xMinCl[ln] || xMaxCl[ln]>cls2.xMaxCl[ln]) return kFALSE;
  }
  return kTRUE;
}

//-------Sec.-----------------------------
class HMdcLookUpTbSec : public TObject {
  friend class HMdcLookUpTb;
  protected:
    Int_t sector;                // sector number
    Int_t nSegments;             // num. of segments for clus. finder
                                 // Size of plot:
    Int_t    nBinX;              // num.bins along axis X,
    Double_t xLow;               // low edge of X
    Double_t xUp;                // upper edge of X
    Double_t xStep;              // step X = (xUp-xLow)/nBinX
    Int_t    nBinY;              // num.bins along axis Y
    Double_t yLow;               // low edge of Y
    Double_t yUp;                // upper edge of Y
    Double_t yStep;              // step Y= (yUp-yLow)/nBinY
    Double_t xHStep;             // = xStep/2
    Double_t yHStep;             // = yStep/2
    Double_t xHStep2;            // = xStep^2
    Double_t yHStep2;            // = yStep^2
    Double_t xLowHSt;            // =xLow+xHStep
    Double_t yLowHSt;            // =yLow+yHStep
    Double_t* xBinsPos;           // Bins positions
    Double_t* yBinsPos;           //

    Int_t size, size4;           // length of plot (in 2-byts & 4-byts words)
    Short_t* hitPlS1;            // pointer to plot of segment 1
    UChar_t* hPlMod1;            // hPlMod1=hitPlS1
    UChar_t* hPlMod2;            // hPlMod2=hitPlS1+1
    Int_t* hitPl4S1;             // hitPl4S1=hitPlS1
    Short_t* hitPlS2;            // pointer to plot of segment 1
    UChar_t* hPlMod3;            // hPlMod3=hitPlS2
    UChar_t* hPlMod4;            // hPlMod4=hitPlS2+1
    Int_t* hitPl4S2;             // hitPl4S2=hitPlS2
    
    Int_t sizeBAr;               // The size of the bit array plotBAr (in bytes)
    static Int_t sizeBArSt;      // sizeBArSt>=sizeBAr;
    Int_t sizeBAr4b;
    static UChar_t* plotBArSc;  // Bit array (1 bit - 1 cell in plot) (for scaning).
    Int_t maxBinBAr4Sc;          //
    Int_t minBinBAr4Sc;          //
    static UChar_t* plotBArM1;  // Bit array for MDC1.
    Int_t maxBinBAr4M1;          //
    Int_t minBinBAr4M1;          //
    static UChar_t* plotBArM2;  // Bit array for MDC2.
    Int_t maxBinBAr4M2;          //
    Int_t minBinBAr4M2;          //
    
    Int_t* plotBArSc4b;         // plotBArSc4b=plotBArSc
    Int_t* plotBAr4bM1;         // plotBAr4bM1=plotBArM1
    Int_t* plotBAr4bM2;         // plotBAr4bM2=plotBArM2

    Bool_t setLUpTb;             // kTRUE or kFALSE
    static HMdcClFnStack* stack; // stack

    TObjArray* array;            // array of pointers of type HMdcLookUpTbMod

    HCategory* fClusCat;         // category of clusters
    HLocation locClus;           // location of cluster category
    Int_t counter[2];            // Counters of HMdcClus containers [segment]
    Bool_t isGeant;              // =kTRUE - geant data
    Short_t nHitsTot;
    UChar_t nHits[24];           //
    Short_t hits[24][250];       // n. cell
    Short_t hitsNTime[24][250];  // n.times,
    Short_t hitsDel[24][250];    // 0 - hit will put in plot
			         // 1 - time2 was deleted
			         // 2 - time1 was deleted
			         // -1 - hit was deleted
    Int_t minAmp[4];             // max.clus. for mdc1,2,3,4
    Int_t nMods;                 // num. of modules in sector for cl.finding
    Int_t typeClFinder;          // =0 - finding in all mdc in sec.
                                 // =1 - finding in each mdc indep.
    Int_t nearbyBins[8];
    HMdcPrPlane prPlane;    // Projections plane
    Float_t target[3];      // target
    Float_t eTarg[3];       // errors of target position
    HGeomVector targVc[2];  // [0] First point of target in coor. sys. of sector
                            // [1] Last point of target in coor. sys. of sector

    Int_t nClusters;
    void calcxy();

    TH2C* hist;
    Int_t plBining;

    // -- Clusters parameters ---
    Int_t nModSeg[2];
    HMdcCluster clusArr[500];      // array of clusters parameters
    Int_t nClsArr;                 // counter of clusters in arraies
    
    HMdcCluster clusArrM1[500];    // array of clusters parameters in MDC1
    Int_t nClsArrM1;               // counter of clusters in MDC1
    HMdcCluster clusArrM2[500];    // array of clusters parameters in MDC2
    Int_t nClsArrM2;               // counter of clusters in MDC2
    
    HMdcClus* fClus;       // pointer to cluster;
//     Int_t nLMax;           //
//     Int_t nLMin;           //
//     Short_t xMaxCl[500];   //
//     Short_t xMinCl[500];   //
    Int_t nFirstClust;     // needed for clusters merging

  public:
    HMdcLookUpTbSec(Int_t sec, Int_t nSegs, Int_t inBinX, Int_t inBinY);
    ~HMdcLookUpTbSec();
    HMdcLookUpTbMod& operator[](Int_t i) {
        return *static_cast<HMdcLookUpTbMod*>((*array)[i]);
    }
    Int_t getSize(void);
    Int_t getNClusters(void){return nClusters;}

    Int_t getMaxClus(void){return minAmp[0]+minAmp[1]+minAmp[2]+minAmp[3];}
    Int_t getMaxClus(Int_t m) {return (m>=0&&m<4) ? minAmp[m]:0;}
    Int_t getNBinX(void){return nBinX;}
    Int_t getNHitsTot(void){return nHitsTot;}
    Double_t getXlow(void){return xLow;}
    Double_t getXup(void){return xUp;}
    Int_t getNBinY(void){return nBinY;}
    Double_t getYlow(void){return yLow;}
    Double_t getYup(void){return yUp;}
    void clearwk(void);
    void setCell(Short_t mod, Short_t layer, Short_t cel, Short_t nTimes);
    Int_t findClusters(Int_t* imax);
    void deleteHit(Short_t mod, Short_t layer, Short_t cell, Short_t nTime);
    void setParPlane(const HMdcPrPlane &plane) {prPlane.setPrPlane(plane);}
    void setTargetF(const HGeomVector& vec) {targVc[0]=vec;}
    void setTargetL(const HGeomVector& vec) {targVc[1]=vec;}
    const HMdcPrPlane& getPrPlane(void) {return prPlane;}
    const HGeomVector& getTargetF(){return targVc[0];}
    const HGeomVector& getTargetL(){return targVc[1];}
    TH2C* fillTH2C(Char_t* name, Char_t* title, Int_t type=0, Int_t binig=2);
    void makePlot(void);
    void setTypeClFinder(Int_t type) {typeClFinder=type;}
    Int_t getTypeClFinder(void) {return typeClFinder;}
    
  protected:
    void fillClusCat(Int_t mod, Int_t segp);
    void findClusInSeg(Int_t seg, Short_t minAm, Short_t *hPlot);
    void findClusInSec(Short_t minAm);
    void findClusInMod(Int_t mod, Short_t minAm, Short_t *hPlot);
    void increaseStack(Int_t stackB);
    void getClusterSlot(Int_t seg, HMdcList12GroupCells& list);
    void fillWiresList(Int_t mod, HMdcCluster& cls, HMdcList12GroupCells& list);
    void makeModPlot(Int_t mod,Int_t minAm, Short_t* hPlot);
    void makeSecPlot(Int_t minAm);
    void makeSegPlot(Int_t seg, Int_t minAm);
    void makeMod1Plot(Int_t minAm);
    void makeMod2Plot(Int_t minAm);
    void findClusInSeg1(void);
    void mergeClusInMod(Int_t mod, Int_t nClArr, HMdcCluster* clArr);
    void mergeClusInSeg(Int_t seg, Int_t nClArr, HMdcCluster* clArr);
    void mergeClusInSec(Int_t nClArr, HMdcCluster* clArr);
    void scanPlotInMod(Int_t mod, UChar_t* hPlMod, UChar_t* plotBAr, 
        HMdcCluster* clArr, Int_t& nClArr, Int_t minBin, Int_t maxBin);
    void scanPlotInSeg(Int_t seg, UChar_t* hPlSeg, UChar_t* plotBAr, 
        HMdcCluster* clArr, Int_t& nClArr);
    
  ClassDef(HMdcLookUpTbSec,0)
};

class HMdcLookUpTb : public HParSet {
  protected:
    static HMdcLookUpTb* fMdcLookUpTb;
    TObjArray* array;     // array of pointers of type HMdcLookUpTbSec
    Int_t nBinX;
    Int_t nBinY;
    Bool_t isCoilOff;
    HMdcGetContainers* fGetCont;
    HMdcSizesCells* fSizesCells;
    HMdcGeomPar* fMdcGeomPar;
    HMdcDetector* fMdcDet;
    HSpecGeomPar* fSpecGeomPar;
    HMdcLayerGeomPar* fLayerGeomPar;
    HCategory* fMdcClusCat;           // category of clusters
    Double_t targLenInc[2];
  public:
    void setPar(Int_t inBinX, Bool_t isCOff);
    void setTargLenInc(Double_t lf,Double_t rt) {
      targLenInc[0]=lf; targLenInc[1]=rt; }
    static HMdcLookUpTb* getExObject();
    static HMdcLookUpTb* getObject();
    static void deleteCont();
    HMdcLookUpTbSec& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbSec*>((*array)[i]);
    }
    Int_t getSize(void);
    Bool_t init(HParIo* input,Int_t* set) {return kTRUE;}
    Bool_t initContainer();
    void clear();
    Int_t  findClusters(Int_t *imax);
  protected:
    HMdcLookUpTb(const char* name="MdcLookUpTb",
                 const char* title="Look Up Table for Dubna track finding",
                 const char* context="");
    ~HMdcLookUpTb();
    Bool_t calcPrPlane(Int_t sec);
    Bool_t calcTarget(Int_t sec);
    Bool_t calcPlotSize(Int_t sec);
    Bool_t calcCellsProj(Int_t sec);
    Double_t calcDist(HGeomVector& p1, HGeomVector& p2);
    void calcPoint(HMdcPointPlane& pr, HGeomVector& p11, HGeomVector& p21,
                                       HGeomVector& p12, HGeomVector& p22);
    Bool_t calcCellProj(Int_t sec,HMdcTrap& cellSize,HMdcTrapPlane& cellProj);
    Bool_t calcLookUpTb(Int_t nSec);
    void calcX(HMdcTrapPlane& pr, Double_t y, Double_t &xLow, Double_t &xUp);
  ClassDef(HMdcLookUpTb,0)
};

#endif  /*!HMDCLOOKUPTB_H*/
