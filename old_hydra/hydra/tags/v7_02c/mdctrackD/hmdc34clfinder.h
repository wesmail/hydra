#ifndef HMDC34CLFINDER_H
#define HMDC34CLFINDER_H

#include "TObject.h"
#include "hmdctrackddef.h"
#include "hlocation.h"
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

class HMdc34ClFinderLayer: public HMdcListLayCells {
  protected:
  public:
    // projection of wire (one for all wires in layer)
    Short_t nBinX;   // projection of wire (one for all wires in layer)
    Short_t *yBin;   // Y bins for X bins  0 - nBinX
    Int_t yFirst;    // yFirst=yBin[0] or [nBinx-1]
    Int_t nYLines;   // num. of lines
    Int_t wOrType;   // wires orientation type
    Short_t *xBin1;  // size of xBin array = nYLines + 1
    Short_t *xBin2;  // size of xBin array = nYLines + 1
    Int_t nCells;    // num.of cells in layer
    Double_t tgY;    // (y1-y2)/(x1-x2) one for all cells
    Double_t tgZ;    // (z1-z2)/(x1-x2) one for all cells
    Double_t y0[2];  // Let x,y,z - point on the kick plane, then:
    Double_t z0[2];  // y'=tgY*x+nCell*yStep+y0[n] n=0 left rib (nearest to tag)
    Double_t yStep;  // Z'=tgZ*x+nCell*zStep+z0[n]  n=1 right rib
    Double_t zStep;  //
    Int_t nPSegOnKick[2][2];  // first index [0]-up rib; [1]-down rib;
                              // second index [0]-lift rib; [1]-right rib;
    Short_t *cellShUp;        // cellShUp(Down)[nCells] keep shift of wire
    Short_t *cellShDown;      // proj.(*yBin). Recalculated for each cluster.

    HMdc34ClFinderLayer(Int_t sec, Int_t mod, Int_t layer);
    ~HMdc34ClFinderLayer();
    Bool_t createArrayBins(Short_t nBins);

    ClassDef(HMdc34ClFinderLayer,0)
};

//-------Mod.-----------------------------

class HMdc34ClFinderMod : public TObject {
  protected:
    TObjArray* array;
  public:
    HMdc34ClFinderMod(Int_t sec, Int_t mod);
    ~HMdc34ClFinderMod();
    void clear(void);
    HMdc34ClFinderLayer& operator[](Int_t i) {
        return *static_cast<HMdc34ClFinderLayer*>((*array)[i]);
    }
    Int_t getNCells(void);
    ClassDef(HMdc34ClFinderMod,0)
};

//-------Plot------------------------------
class HMdcProjPlot : public HMdcPlane {
  protected:
  public:
    Float_t yMin,yMax;       // The sizes of the plot.
    Float_t xMin,xMax;
    Float_t xMinD,xMaxD;     // The small side of the MDC's proj.
    Short_t nBinX;           //  num.bins along axis X (xMin-xMax)
    Float_t stX;             //  step X = (xMax-xMin)/nBinX
    Short_t nBinY;           //  num.bins along axis Y
    Float_t stY;             //  step Y= (yMax-yMin)/nBinY
    Short_t* xMinL;          // In units of bin X num.
    Short_t* xMaxL;
    Short_t* yMinL;          // yMaxL[nx]=nBinY-1
    Double_t* xBinsPos;      // Bins positions
    Double_t* yBinsPos;      //

    UChar_t* plModF;         // first module in segment project plot
    UChar_t* plModS;         // second module in segment project plot
    Int_t size;              // The size of the plModF[] and plModS[]
    Int_t sizeBAr;           // The size of the bit array plotBAr (in bytes)
    UChar_t* plotBArSc;      // Bit array (1 bit - 1 bin in plot) (for scaning)
    Int_t* plotBArSc4b;      // plotBArSc4b=plotBArSc
    static Char_t* weights;  // array of pr.plot bins weights
    static Int_t wtArrSize;  // weights array size
    Int_t minBin4Sc;         // bins region in plotBArSc for scaning
    Int_t maxBin4Sc;         //

    TH2C* rootPlot;
    TH2C* rootPlotV2;

    HMdcProjPlot(UChar_t mSeg, Int_t inBinX, Int_t inBinY);
    ~ HMdcProjPlot();
    void setEdges(Float_t iyMin, Float_t ixMinD, Float_t ixMaxD,
                  Float_t iyMax, Float_t ixMin, Float_t ixMax);
    void print(void);
    void clearArr(void);
    TH2C* getPlot(Char_t* name, Char_t* title);
    TH2C* getPlotV2(Char_t* name, Char_t* title);

    ClassDef(HMdcProjPlot,0)
};

//-------Sec.------------------------------
class HMdc34ClFinderSec : public TObject {
  friend  class HMdc34ClFinder;
  protected:
    TObjArray* array;           // array of pointers of type HMdc34ClFinderMod
    Int_t sector;
    Int_t *lMods;               // pointer to the list of mdc's
    UChar_t mSeg[2];            // mSeg[0,1] -segment 0,1
    static HKickPlane2* fkick;
    HGeomVector target[2];      // The first & last points of tar. in coor.sys. of sec.
    // The plot of MDC3-4 ???
    HMdcProjPlot *prPlotSeg2;

    HGeomVector seg1[5];        // [0-3] -region, [4] - hit
    HGeomVector dirSeg1[5];     // dirSeg1[4] - errors of seg1[4]
    HGeomVector segOnKick[5];   // region of  of HMdcSeg-hit on the kick plane
    Double_t al[4],bl[4],cl[4];
    Int_t nbX[4];

    Int_t minAmp[4];             // minAmp[2]-mdc3, minAmp[3]-mdc4
    Int_t maxAmp[4];             // number of fired layes in MDC's
    Bool_t notEnoughWrs;         // =kTRUE if segment has not enough fired wires

    Int_t nearbyBins[8];         // Lookup table for finding of nearby bins
    static HMdcClFnStack* stack; // stack

    static Short_t xMinClLines[300];
    static Short_t xMaxClLines[300];

    Int_t counter;               // Counter of HMdcClus containers (loc[2])
    Int_t indexPar;
    Int_t indexFCh;
    Int_t indexLCh;

    Int_t* xCMin[4];             // for cleaning proj. plots
    Int_t* xCMax[4];             // [2] & [3] are using now
    Int_t* cXMinM;               // current pointer to xCMin[mod]
    Int_t* cXMaxM;               // -/-
    Int_t* xMinM2;               // =xMin[2]
    Int_t* xMaxM2;               // =xMax[2]
    UChar_t* cPlMod;             // current mod.pr.plot
    HMdc34ClFinderMod* cFMod;    // current module
    HMdc34ClFinderLayer* cFLay;  // current layer
    Int_t nBinX;                 // X size of plot

    Int_t cell;                  // var. for making layers proj.
    Int_t nBinYM2;               //
    Int_t shUp;                  //
    Int_t shDown;                //
    UChar_t add;                 //
    Int_t nYLinesM1;             //
    Short_t *xBin1L;             //
    Short_t *xBin2L;             //
    Int_t shUpN;                 //
    Int_t shDownN;               //
    Bool_t test;                 //
    Int_t ny1;                   //
    Int_t ny2;                   //
    Int_t nbL;                   //
    Int_t nbF;                   //

    HCategory* fClusCat;
    HLocation locClus;
    Bool_t isGeant;

    // -- Clusters parameters ---
    Int_t typeClFinder;               // !=2 - cl.finding condition for each mdc
                                      // =2  - cl.finding condition for two mdc
    Int_t realTypeClFinder;           // =2 or 0
    Double_t radToDeg;                // =180./acso(-1)
    static HMdcList12GroupCells cLCells2[500]; // lists of cells in clusters
    Int_t nClsArr;                    // counter of clusters in arraies
    static Bool_t cStatus[500];       // =kFALSE - removed by merging
    static Short_t cNMergedClus[500]; // Number of merged clusters
    static Double_t cSumWt[500];      // sum weights of bins
    static Int_t cNBins[500];         // num. of bins in cluster
    static Double_t cX[500];          // clusters positions
    static Double_t cY[500];          //
    static Double_t cMeanX[500];      // var. for calculation of cluster shape
    static Double_t cMeanY[500];      // -/-
    static Double_t cMeanXX[500];     // -/-
    static Double_t cMeanYY[500];     // -/-
    static Double_t cMeanYX[500];     // -/-
    static Double_t cMeanXWt[500];    // position of cluster (calc. with weights)
    static Double_t cMeanYWt[500];    // -/-
    static Double_t cMeanXXWt[500];   // X dispersion
    static Double_t cMeanYYWt[500];   // Y dispersion

  protected:
    void makeModS2Plot(Int_t mod);
    void makeSeg2Plot(void);
    void makeSeg2PlotTp2(void);
    Int_t calcYbin(Int_t upDo,Int_t leRi,Int_t cell);
    Int_t scanPlotSeg2(void);
    void calcClusterSeg2(Int_t nBinF);
    Int_t findClustersSeg2(void);
    void setLayerVar(Int_t lay);
    Bool_t setNextCell(void);
    void setYLimits(void);
    Bool_t setXLimits(Int_t ny, Short_t nx1,Short_t nx2);
    void makeLayProjV1(Int_t lay);
    void makeLayProjV1b(Int_t lay);
    void makeLayProjV2(Int_t lay);
    void clearPrMod(Int_t mod=-1);
    void clearPrSegTp2(void);
    Bool_t testMaxAmp(void);

  public:
    HMdc34ClFinderSec(Int_t sec, Int_t inBinX, Int_t inBinY);
    ~HMdc34ClFinderSec();
    HMdc34ClFinderMod& operator[](Int_t i) {
        return *static_cast<HMdc34ClFinderMod*>((*array)[i]);
    }
    void clear(void);
    void setMinBin(Int_t *mBin);
    Bool_t notEnoughWires(void) const {return notEnoughWrs;}
    Int_t findClustersSeg2(HMdcSeg* fSeg, HMdcClus* fClus, Int_t *mBin=0);
    Int_t findClustersSeg2(HMdcClus* fClus, Int_t *mBin=0);
    TH2C* getPlot(Char_t* name, Char_t* title,Int_t ver=0);
    HMdcProjPlot* getPlotSeg2(void) {return prPlotSeg2;}

    Int_t getNBinX(void){return prPlotSeg2->nBinX;}
    Int_t getNBinY(void){return prPlotSeg2->nBinY;}
    void printClusters(void);
    void setTargetF(const HGeomVector& vec) {target[0]=vec;}
    void setTargetL(const HGeomVector& vec) {target[1]=vec;}
    const HGeomVector& getTargetF(void){return target[0];}
    const HGeomVector& getTargetL(void){return target[1];}
    void setTypeClFinder(Int_t type) {typeClFinder=type;}
    Int_t getTypeClFinder(void) {return typeClFinder;} 
    ClassDef(HMdc34ClFinderSec,0)
};

//----------------------------------------------------------
class HMdc34ClFinder : public HParSet {
  protected:
    static HMdc34ClFinder* fMdc34ClFinder;
    TObjArray* array;     // array of pointers of type HMdc34ClFinderSec
    HMdcGetContainers* fGetCont;
    HMdcDetector* fMdcDet;
    HSpecGeomPar* fSpecGeomPar;
    HMdcSizesCells* fSizesCells;
    HKickPlane2* fKickPlane;
    HMdcGeomPar* fMdcGeomPar;
    HCategory* fMdcClusCat;
  public:
    static HMdc34ClFinder* getExObject(void);
    static HMdc34ClFinder* getObject(void);
    static void deleteCont(void);
    HMdc34ClFinderSec& operator[](Int_t i) {
      return *static_cast<HMdc34ClFinderSec*>((*array)[i]);
    }
    Bool_t init(HParIo* input,Int_t* set) {return kTRUE;}
    Bool_t initContainer(void);
    void clear(void);
    void printClusters(Int_t nsec=-1);
  protected:
    HMdc34ClFinder(const char* name="Mdc34ClFinder",
                   const char* title="LookupTable & cluster finder for Mdc3,4 segment",
                   const char* context="");
    ~HMdc34ClFinder();
    Bool_t calcTarget(Int_t sec);
    Bool_t calcProjPlaneSeg2(Int_t sec);
    Bool_t calcSizePlotSeg2(Int_t sec);
    Bool_t calcWiresProj(Int_t sec);
    void calcCrossLines(HGeomVector& p1l1, HGeomVector& p2l1,
              HGeomVector& p1l2, HGeomVector& p2l2, HGeomVector& cross);
    Float_t xLine(HGeomVector& p1, HGeomVector& p2, Float_t yi);

    ClassDef(HMdc34ClFinder,0)
};


#endif  /*!HMDC34CLFINDER_H*/
