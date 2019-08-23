#ifndef HMDC34CLFINDER_H
#define HMDC34CLFINDER_H

#include "TObject.h"
#include "hmdctrackddef.h"
//#include "hevent.h"
#include "hlocation.h"
#include "hmdclistcells.h"
#include "hmdcgeomobj.h"
#include "hparset.h"
#include "hgeomvector.h"

class HMdcClus;
class HMdcSeg;
class TH2C;
class HKickPlane;
class HMdcGetContainers;
class HMdcDetector;
class HSpecGeomPar;
class HMdcSizesCells;
class HMdcGeomPar;
class HCategory;

class HMdc34ClFinderLayer: public HMdcListCells {
  protected:
  public:
    // Lookup Table Mdc34:
    Short_t nBinX;   // The projection of wire (one for all wires)
    Short_t *yBin;   // Y bins for X bins  0 - nBinX
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
    void setCell(Int_t cell, UChar_t nt) {setTime(cell,nt|1);} // nHits=1 =>1, nHits=2 =>3

    ClassDef(HMdc34ClFinderLayer,0)
};

//-------Mod.-----------------------------

class HMdc34ClFinderMod : public TObject {
  protected:
    TObjArray* array;
  public:
    HMdc34ClFinderMod(Int_t sec, Int_t mod);
    ~HMdc34ClFinderMod();
    void clear();
    HMdc34ClFinderLayer& operator[](Int_t i) {
        return *static_cast<HMdc34ClFinderLayer*>((*array)[i]);
    }
    Int_t getNCells();
    ClassDef(HMdc34ClFinderMod,0)
};

//-------Plot------------------------------
class HMdcProjPlot : public TObject, public HMdcPrPlane {
  protected:
  public:
    Float_t yMin,yMax;   // The sizes of the plot.
    Float_t xMin,xMax;
    Float_t xMinD,xMaxD; // The small side of the MDC's proj.
    Short_t nBinX;       //  num.bins along axis X (xMin-xMax)
    Float_t stX;         //  step X = (xMax-xMin)/nBinX
    Short_t nBinY;       //  num.bins along axis Y
    Float_t stY;         //  step Y= (yMax-yMin)/nBinY
    Short_t* xMinL;      // In units of bin X num.
    Short_t* xMaxL;

    Short_t* plot;       // Proj. plot
    UChar_t* plot1b;     // plot1b=plot
    Int_t*   plot4b;     // plot4b=plot
    Int_t size;          // The size of the Short_t* plot
    Int_t sizeBAr;       // The size of the bit array plotBAr (in bytes)
    Int_t sizeBAr4b;
    UChar_t* plotBArSc;  // Bit array (1 bit - 1 cell in plot) (for scaning).
    Int_t* plotBArSc4b;  // plotBArSc4b=plotBArSc

    TH2C* rootPlot;
    TH2C* rootPlotV2;

    HMdcProjPlot(Int_t inBinX, Int_t inBinY);
    ~ HMdcProjPlot();
    void setEdges(Float_t iyMin, Float_t ixMinD, Float_t ixMaxD,
                  Float_t iyMax, Float_t ixMin, Float_t ixMax);
    void print();
    void HMdcProjPlot::clearArr() {memset(plot,0,size*2);}
    TH2C* getPlot(Char_t* name, Char_t* title);
    TH2C* getPlotV2(Char_t* name, Char_t* title);

    ClassDef(HMdcProjPlot,0)
};

//-------Sec.------------------------------
class HMdc34ClFinderSec : public TObject {
  friend  class HMdc34ClFinder;
  protected:
    TObjArray* array;         // array of pointers of type HMdc34ClFinderMod
    Int_t sector;
    Int_t *lMods;             // pointer to the list of mdc's
    UChar_t mSeg[2];          // mSeg[0,1] -segment 0,1
    static HKickPlane* fkick;
    HGeomVector target[2];      // The first & last points of tar. in coor.sys. of sec.
    // The plot of MDC3-4 ???
    HMdcProjPlot *prPlotSeg2;

    HGeomVector seg1[5];       // [0-3] -region, [4] - hit
    HGeomVector dirSeg1[5];    // dirSeg1[4] - errors of seg1[4]
    HGeomVector segOnKick[5];  // region of  of HMdcSeg-hit on the kick plane
    Double_t al[4],bl[4],cl[4];
    Int_t nbX[4];

    Int_t* minBin;               // minBin[2]-mdc3, minBin[3]-mdc4

    Int_t nearbyBins[8];        // Lookup table for finding of nearby bins
    static Int_t* stackSeg2;    // stack
    static Int_t  stackSizeSeg2;

    static Short_t xMinClLines[300];
    static Short_t xMaxClLines[300];

    Int_t counter;              // Counter of HMdcClus containers (loc[2])
    Int_t indexPar;

    HCategory* fClusCat;
    HLocation locClus;
    Bool_t isGeant;

  protected:
    void makePlotSeg2();
    Int_t calcYbin(HMdc34ClFinderLayer& fLay,Int_t upDo,Int_t leRi,Int_t cell);
    Int_t scanPlotSeg2();
    void calcClusterSeg2(Int_t nBin, HMdcClus *clus);
    Int_t findClustersSeg2();

  public:
    HMdc34ClFinderSec(Int_t sec, Int_t inBinX, Int_t inBinY);
    ~HMdc34ClFinderSec();
    HMdc34ClFinderMod& operator[](Int_t i) {
        return *static_cast<HMdc34ClFinderMod*>((*array)[i]);
    }
    void clear();
    Int_t findClustersSeg2(HMdcSeg* fSeg, Int_t *mBin);
    Int_t findClustersSeg2(HMdcClus* fClus, Int_t *mBin);
    HMdcProjPlot* getPlotSeg2() {return prPlotSeg2;}

    Int_t getNBinX(){return prPlotSeg2->nBinX;}
    Int_t getNBinY(){return prPlotSeg2->nBinY;}
    void printClusters();
    void setTargetF(const HGeomVector& vec) {target[0]=vec;}
    void setTargetL(const HGeomVector& vec) {target[1]=vec;}
    const HGeomVector& getTargetF(){return target[0];}
    const HGeomVector& getTargetL(){return target[1];}
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
    HKickPlane* fKickPlane;
    HMdcGeomPar* fMdcGeomPar;
    HCategory* fMdcClusCat;
  public:
    static HMdc34ClFinder* getExObject();
    static HMdc34ClFinder* getObject();
    static void deleteCont();
    HMdc34ClFinderSec& operator[](Int_t i) {
      return *static_cast<HMdc34ClFinderSec*>((*array)[i]);
    }
    Bool_t init(HParIo* input,Int_t* set) {return kTRUE;}
    Bool_t initContainer();
    void clear();
    void printClusters(Int_t nsec=-1);
  protected:
    HMdc34ClFinder();
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
