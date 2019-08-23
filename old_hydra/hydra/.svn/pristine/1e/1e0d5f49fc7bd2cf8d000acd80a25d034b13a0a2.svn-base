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

    Int_t size, size4;           // length of plot (in 2-byts & 4-byts words)
    Short_t* hitPlS1;            // pointer to plot of segment 1
    Int_t* hitPl4S1;             // hitPl4S1=hitPlS1
    Short_t* hitPlS2;            // pointer to plot of segment 1
    Int_t* hitPl4S2;             // hitPl4S2=hitPlS2
   //====================beamtime !!!!!!!!!!=====================
    Char_t* bMboard;
    Int_t indFill;
   //============================================================
    Bool_t setLUpTb;             // kTRUE or kFALSE
    static Int_t* stack;         // stack
    static Int_t  stackSize;     // stack size

    TObjArray* array;            // array of pointers of type HMdcLookUpTbMod

    Short_t tablBiN[4096];       // Total num. of layers in bin

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
    Double_t radToDeg;           // =180./acso(-1)
    Int_t nModSeg[2];
    static HMdcList12GroupCells cLCells1[500];
    static HMdcList12GroupCells cLCells2[500];
    Int_t nClsArr;                    // counter of clusters in arraies
    static Bool_t cStatus[500];       // =kFALSE - removed by merging
    static Short_t cNMergedClus[500]; // Number of merged clusters
    static Double_t cSumWt[500];      // sum weghts of bins
    static Int_t cNBins[500];         // num. of bins in cluster
    static Double_t cX[500];          // clusters positions
    static Double_t cY[500];          //
    static Double_t cMeanX[500];      // var. for calculation of cluster shape
    static Double_t cMeanY[500];      // -/-
    static Double_t cMeanXX[500];     // -/-
    static Double_t cMeanYY[500];     // -/-
    static Double_t cMeanYX[500];     // -/-
    static Double_t cMeanXWt[500];    // position of cluster (calc. with weghts)
    static Double_t cMeanYWt[500];    // -/-
    static Double_t cMeanXXWt[500];   // X dispersion
    static Double_t cMeanYYWt[500];   // Y dispersion
 

    
    
    HMdcClus* fClus;       // pointer to cluster;
    Int_t nLmax;           //
    Int_t nLmin;           //
    Short_t xMaxCl[500];   //
    Short_t xMinCl[500];   //
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
    Short_t getBin(Int_t nbin){return tablBiN[hitPlS1[nbin]];}
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
    void doPlot(void);
    void setTypeClFinder(Int_t type) {typeClFinder=type;}
    Int_t getTypeClFinder(void) {return typeClFinder;}
    
  protected:
    void fillClusCut(Int_t mod, Int_t segp);
    void findClusInSeg(Int_t seg, Short_t maxCl, Short_t *hPlot);
    void findClusInSec(Short_t maxCl);
    void findClusInMod(Int_t mod, Short_t maxCl, Short_t *hPlot);
    void increaseStack(Int_t stackB);
    void getClusterSlot(Int_t seg, HMdcList12GroupCells& list);
    void fillWiresList(Int_t mod, HMdcList12GroupCells& list);

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
