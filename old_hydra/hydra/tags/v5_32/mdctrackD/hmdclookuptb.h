#ifndef HMDCLOOKUPTB_H
#define HMDCLOOKUPTB_H

#include "TObject.h"
#include "hparset.h"
#include "hmdc12clusters.h"
#include "hmdcgeomobj.h"

class TH2C;
class TObjArray;
class HMdcGeomPar;
class HMdcDetector;
class HMdcGetContainers;
class HMdcLayerGeomPar;
class HMdcSizesCells;
class HSpecGeomPar;

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
    Int_t getSize();

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
    Int_t getSize();
    void setNLayers(Int_t nl) {nLayers=nl;}
    Int_t getNLayers() {return nLayers;}

    ClassDef(HMdcLookUpTbMod,0)
};

//-------Sec.-----------------------------
class HMdcLookUpTbSec : public TObject {
  friend class HMdcLookUpTb;
  protected:
    Int_t nSegments;     // num. of segments for clus. finder
                         // Size of plot:
    Int_t    nBinX;      //  num.bins along axis X,
    Double_t xLow;       //  low edge of X
    Double_t xUp;        //  upper edge of X
    Double_t sh_x;       //  step X = (xUp-xLow)/nBinX
    Int_t    nBinY;      //  num.bins along axis Y
    Double_t yLow;       //  low edge of Y
    Double_t yUp;        //  upper edge of Y
    Double_t sh_y;       //  step Y= (yUp-yLow)/nBinY
    Double_t shX2;
    Double_t shY2;

    Int_t size, size4;   // length of plot (in 2-byts & 4-byts words)
    Short_t* hitplot;    // pointer to plot
    Int_t* hitplot4;     // hitplot4=hitplot
    Short_t* hitplotS2;    // pointer to plot
    Int_t* hitplot4S2;     // hitplot4=hitplot
   //====================beamtime !!!!!!!!!!=====================
    Char_t* bMboard;
    Int_t indFill;
   //============================================================
    Bool_t setLUpTb;     // kTRUE or kFALSE
    static Int_t* stack;    // stack                                            
    static Int_t  stackSize;
    
    TObjArray* array;    // array of pointers of type HMdcLookUpTbMod

    Short_t tablBiN[4096];       // Total num. of layers in bin

    HMdc12Clusters clusters;     // Output of clusters finding
    Short_t nHitsTot;
    UChar_t nHits[24];             // 
    Short_t hits[24][250];         // n. cell
    Short_t hitsNTime[24][250];    // n.times,
    Short_t hitsDel[24][250];      // 0 - hit will put in plot
			           // 1 - time2 was deleted
			           // 2 - time1 was deleted
			           // -1 - hit was deleted
    Int_t maxClust[4];             // max.clus. for mdc1,2,3,4
    Int_t nMods;                   // num. of modules in sector for cl.finding
    Int_t typeClFinder;                  // =0 - finding in all mdc in sec. 
                                         // =1 - finding in each mdc indep.
    Int_t nearbyBins[8];
    HMdcPrPlane prPlane;  // Projections plane
    HGeomVector target[2];  // [0] First point of target in coor. sys. of sector
                            // [1] Last point of target in coor. sys. of sector

    Int_t nClusters;
    void calcxy();

    TH2C* hist;
    Int_t plBining;

  public: 
    HMdcLookUpTbSec(Int_t sec, Int_t nSegs, Int_t inBinX, Int_t inBinY);
    ~HMdcLookUpTbSec();
    HMdcLookUpTbMod& operator[](Int_t i) {
        return *static_cast<HMdcLookUpTbMod*>((*array)[i]);
    }
    HMdc12Clusters& getClusters() {return clusters;}
    Int_t getSize();
    Int_t getNClusters(){return nClusters;}

    Int_t getMaxClus(){return maxClust[0]+maxClust[1]+maxClust[2]+maxClust[3];}
    Int_t getMaxClus(Int_t m) {return (m>=0&&m<4) ? maxClust[m]:0;}
    Int_t getNBinX(){return nBinX;}
    Int_t getNHitsTot(){return nHitsTot;}
    Double_t getXlow(){return xLow;}
    Double_t getXup(){return xUp;}
    Int_t getNBinY(){return nBinY;}
    Double_t getYlow(){return yLow;}
    Double_t getYup(){return yUp;}
    Short_t getBin(Int_t nbin){return tablBiN[hitplot[nbin]];}
    void clearwk();
    void setCell(Short_t mod, Short_t layer, Short_t cel, Short_t nTimes);
    Int_t findClusters(Int_t* imax);
    void deleteHit(Short_t mod, Short_t layer, Short_t cell, Short_t nTime);
    void printClusters(); 
    void setParPlane(const HMdcPrPlane &plane) {prPlane.setPrPlane(plane);}
    void setTargetF(const HGeomVector& vec) {target[0]=vec;}
    void setTargetL(const HGeomVector& vec) {target[1]=vec;}
    const HMdcPrPlane& getParPlane(void) {return prPlane;}
    const HGeomVector& getTargetF(){return target[0];}
    const HGeomVector& getTargetL(){return target[1];}
    TH2C* fillTH2C(Char_t* name, Char_t* title, Int_t type=0, Int_t binig=2);
    void doPlot();
    void setTypeClFinder(Int_t type) {typeClFinder=type;}
    Int_t getTypeClFinder() {return typeClFinder;}
  protected:
    void testClust(Int_t nClFirst, Int_t nClusters);
    void findClusInSeg(Int_t seg, Short_t maxCl, Short_t *hPlot);
    void findClusInSec(Short_t maxCl);
    void findClusInMod(Int_t mod, Short_t maxCl, Short_t *hPlot);
    void calcQualClust();
    
  ClassDef(HMdcLookUpTbSec,0)
}; 

class HMdcLookUpTb : public HParSet {
  protected: 
    static HMdcLookUpTb* fMdcLookUpTb;
    TObjArray* array;     // array of pointers of type HMdcLookUpTbSec 
    Bool_t status;
    Int_t nBinX;
    Int_t nBinY;
    Bool_t isCoilOff;
    HMdcGetContainers* fGetCont;
    HMdcSizesCells* fSizesCells;
    HMdcGeomPar* fMdcGeomPar;
    HMdcDetector* fMdcDet;
    HSpecGeomPar* fSpecGeomPar;
    HMdcLayerGeomPar* fLayerGeomPar;
  public:
    void setPar(Int_t inBinX, Bool_t isCOff);
    static HMdcLookUpTb* getExObject();
    static HMdcLookUpTb* getObject();
    static void deleteCont();
//    Bool_t hasChanged() {return status;} 
    HMdcLookUpTbSec& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set) {return kTRUE;}
    Bool_t initContainer();
    void clear();
    void clearwk();
    Int_t  findClusters(Int_t *imax);
    void printClusters(Int_t nsec=-1);
  protected:
    HMdcLookUpTb();
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
