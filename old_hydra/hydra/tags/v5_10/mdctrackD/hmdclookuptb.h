#ifndef HMDCLOOKUPTB_H
#define HMDCLOOKUPTB_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"
#include "hmdc12cellprojec.h"
#include "hmdc12clusters.h"
#include "TH2.h"


class HMdcLookUpTbCell: public TObject {
protected:
    Int_t nLines;
public:
    
    Int_t line;
    Int_t yBinMin;
    Int_t yBinMax;
    Int_t *xBinMin;
    Int_t *xBinMax;
    Int_t getYBinMin(){return yBinMin;}
    Int_t getYBinMax(){return yBinMax;}
    HMdcLookUpTbCell(Int_t yBinMinT, Int_t yBinMaxT);
    ~HMdcLookUpTbCell();
    void addLine(Int_t nc1, Int_t nc2);
    ClassDef(HMdcLookUpTbCell,0)
};

class HMdcLookUpTbLayer: public TObject {
protected:    
    TObjArray* array;
public:
    HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer);
    ~HMdcLookUpTbLayer();
    void initCell(Int_t nce, Int_t yBinMinT, Int_t yBinMaxT);
    Bool_t isCell(Int_t nce){
      if((*array)[nce]) return kTRUE;
      return kFALSE;
    };
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
public:
  HMdcLookUpTbMod(Int_t sec, Int_t mod);
  ~HMdcLookUpTbMod();
  HMdcLookUpTbLayer& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbLayer*>((*array)[i]);
    }
  Int_t getSize();
  ClassDef(HMdcLookUpTbMod,0)
};


class HMdcLookUpTbSec : public TObject {
protected:
                       // Size of plot:
  Int_t    nBinX;      //  num.bins along axis X,
  Double_t xLow;       //  low edge of X
  Double_t xUp;        //  upper edge of X
  Double_t sh_x;       //  step X = (xUp-xLow)/nBinX
  Int_t    nBinY;      //  num.bins along axis Y
  Double_t yLow;       //  low edge of Y
  Double_t yUp;        //  upper edge of Y
  Double_t sh_y;       //  step Y= (yUp-yLow)/nBinY
  
  Int_t size, size4;   // length of plot (in 2-byts & 4-byts words)
  Short_t* hitplot;    // pointer to plot
  Int_t* hitplot4;     // hitplot4=hitplot
 //====================beamtime !!!!!!!!!!=====================
  Char_t* bMboard;
  Int_t indFill;
 //============================================================
  Bool_t setLUpTb;     // kTRUE or kFALSE
  
  TObjArray* array;    // array of pointers of type HMdcLookUpTbMod

  Short_t tablBiN[4096];       // Total num. of layers in bin
  Short_t testBiN[4096];       // Total num. of layers in bin for bins 4L+4L
  
  HMdc12Clusters clusters;     // Output of clusters finding
  Short_t nHitsTot;
  UChar_t nHits[12];             // 
  Short_t hits[12][250];         // n. cell
  Short_t hitsNTime[12][250];    // n.times,
  Short_t hitsDel[12][250];      // 0 - hit will put in plot
			         // 1 - time2 was deleted
			         // 2 - time1 was deleted
			         // -1 - hit was deleted
  Int_t closeBins[8];
  HGeomTransform planeCoord;   // Coordinat system of plan projection in sector
  Double_t Ap,Bp,Dp;           // Param. of projectin's plane (Ap*x+Bp*y+z=Dp) (Cp=1. !!!)
  HGeomVector targetF;  // First point of target in coor. sys. of sector
  HGeomVector targetL;  // Last point of target in coor. sys. of sector

  Int_t nClusters;
  void calcxy();
  
  TH2C* hist;
  
public: 
  HMdcLookUpTbSec(Int_t sec, Int_t inBinX, Int_t inBinY);
  ~HMdcLookUpTbSec();
  HMdcLookUpTbMod& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbMod*>((*array)[i]);
  }
  void setEdgesPlot(Double_t ixLow, Double_t ixUp, Double_t iyLow, Double_t iyUp);
  HMdc12Clusters& getClusters() {return clusters;}
  Int_t getSize();
  Int_t getNClusters(){return nClusters;}
  
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
  Int_t findClusters(Int_t imax=12);
  void deleteHit(Short_t mod, Short_t layer, Short_t cell, Short_t nTime);
  void printClusters(); 
  void setParPlane(const HGeomTransform &sys);
  void setTargetF(const HGeomVector& vec) {targetF=vec;}
  void setTargetL(const HGeomVector& vec) {targetL=vec;}
  const HGeomTransform& getParPlane(void) {return planeCoord;}
  const HGeomVector& getTargetF(){return targetF;}
  const HGeomVector& getTargetL(){return targetL;}
  TH2C* fillTH2C(Char_t* name, Char_t* title);
  void doPlot();  
  ClassDef(HMdcLookUpTbSec,0)
}; 

class HMdcLookUpTb : public HParSet {
protected: 
    TObjArray* array;     // array of pointers of type HMdcLookUpTbSec 
    Bool_t isContainer;
public:
    HMdcLookUpTb(Int_t nBinX=630);
    ~HMdcLookUpTb();
    HMdcLookUpTbSec& operator[](Int_t i) {
      return *static_cast<HMdcLookUpTbSec*>((*array)[i]);
    }
    Int_t getSize();
    Bool_t init(HParIo* input,Int_t* set) {return isContainer;}
    Int_t write(HParIo* output);
    void clear();
    void clearwk();
    Int_t  findClusters(Int_t imax=12);
    void printClusters(Int_t nsec=-1);
    ClassDef(HMdcLookUpTb,0)
};

//----------------------------------------------------------

class HMdcCalcLookUpTb : public TObject {
protected:
    HMdc12CellProjec CellProjec;
    
public:
    HMdcCalcLookUpTb();
    ~HMdcCalcLookUpTb();
    Bool_t calcLookUpTb(Int_t nSec, HMdcLookUpTb* fLookUpTb);
protected:
    void setParContainer();
    void calcX(HMdcTrapPlane& pr, Double_t y, Double_t &xLow, Double_t &xUp);
    ClassDef(HMdcCalcLookUpTb,0)
};
#endif  /*!HMDCLOOKUPTB_H*/
