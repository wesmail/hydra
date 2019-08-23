#ifndef HMDCEFFICIENCY_H
#define HMDCEFFICIENCY_H

#include "hreconstructor.h"
//#include "iostream.h"
#include "hlocation.h"
#include "TROOT.h"
#include "TPaveText.h"
#include "hgeomtransform.h"

class HIterator;
class HMdcGeomStruct;
class TH1F;
class TH2F;
class TCanvas;
class HMdcCal1;
class TSlider;
class HMdcLookupGMod;
class HMdcTimeCutD;
class HCategory;
class HMdcLookUpTb;
class HMdcLookUpTbSec;
class HMdcRawModStru;
class HMdcGeomPar;
class HMdcLayerGeomPar;
class HGeomVector;
class HMdcDetector;
class HMdcSizesCells;
class HMdcGetContainers;

class HMdcEff : public TObject {
  protected:
    Float_t  tNhits; // total num. of hits (used for eff. cells only)
    Float_t  tNhitsCutS; // total num. of hits (cut for time1&time2 only)
    Float_t  tNhitsNoCut; // total num. of hits (no cut)
    Float_t  tNhitsClus;
    Double_t n0;     // num. times cells was not fired
    Double_t errN0;
    Double_t n1;     // num. of single hits (the cell was fired but closest cells not)
    Double_t errN1;
    Double_t n2m;    // num. of double hits ( two fired close cell)
    Double_t errN2m; // at calculation of eff. of N wires (from Nf to Nl)
    Double_t n2b;    // n2m = num. of double hits for pairs Nf:Nf+1 ... Nl-1:Nl
    Double_t errN2b; // n2b = num. of double hits for pairs Nf-1:Nf and Nl:Nl+1
                     // (for one cell - n2m=0, for all cells n2b=0)

    Double_t eff;       // efficiency
    Double_t errEff;    // error of efficiency

    Double_t eM;
    Double_t errM;

    Double_t single;       // fraction of track wich go through one cell only
    Double_t errSingle;    // error ...

    Double_t p0;           // probability to have 0 hits in layer
    Double_t errP0;        // error ...
    Double_t p1;           // probability to have 1 hits in layer
    Double_t errP1;        // error ...
    Double_t p2;           // probability to have 2 hits in layer
    Double_t errP2;        // error ...

    Double_t totNumTr;     // for M=1 or efficiency of MDC totNumTr=n0+n1+n2m

  public:
    HMdcEff(){;}
    virtual void calcEff(Double_t iN0,     Double_t eN0,
                         Double_t iN1,     Double_t eN1,
                         Double_t iN2m,    Double_t eN2m,
                         Double_t iN2b=0., Double_t eN2b=0.);
    void setTotNhits(Float_t nHits) {tNhits=nHits;}
    void setTotNhitsCutS(Float_t nHits) {tNhitsCutS=nHits;}
    void setTotNhitsNoCut(Float_t nHits) {tNhitsNoCut=nHits;}
    void setTotNhitsClus(Float_t nHits) {tNhitsClus=nHits;}
    Float_t getTotNhits(void) {return tNhits;}
    Float_t getTotNhitsNoCut(void) {return tNhitsNoCut;}
    Float_t getTotNhitsCutS(void) {return tNhitsCutS;}
    Float_t getTotNhitsClus(void) {return  tNhitsClus;}
    Double_t getN0(void) {return n0;}
    Double_t getN1(void) {return n1;}
    Double_t getN2(void) {return n2m+n2b;}
    Double_t getN2m(void) {return n2m;}
    Double_t getN2b(void) {return n2b;}
    Double_t getEfficiency(void) {return eff;}
    Double_t getErrEfficiency(void) {return errEff;}
    Double_t getFracSingle(void) {return single;}
    Double_t getErrFracSingle(void) {return errSingle;}
    Double_t getP0(void) {return p0;}
    Double_t getP1(void) {return p1;}
    Double_t getP2(void) {return p2;}
    Double_t getErrP0(void) {return errP0;}
    Double_t getErrP1(void) {return errP1;}
    Double_t getErrP2(void) {return errP2;}
    Double_t getTotNumTracks(void) {return totNumTr;}

  protected:
    void calculation();
    virtual Double_t efficiency(Double_t n0T, Double_t n1T, 
                                Double_t n2mT, Double_t n2bT=0.);
    Double_t HMdcEff::fractSingle(Double_t effT, Double_t n0T, Double_t n1T, 
                                  Double_t n2mT, Double_t n2bT=0.);

  ClassDef(HMdcEff,0)
};


class HMdcEffMod : public HMdcEff {
  friend class HMdcEfficiency;
  protected:
    Double_t p0l; // probability to have not fired wires in 6 layers
    Double_t p1l; // probability to have fired wires in 1 layer only
    Double_t p2l; // probability to have fired wires in 2 layers
    Double_t p3l; // probability to have fired wires in 3 layers
    Double_t p4l; // probability to have fired wires in 4 layers
    Double_t p5l; // probability to have fired wires in 5 layers
    Double_t p6l; // probability to have fired wires in 6 layers
  public:
    HMdcEffMod(){;}
    virtual void calcEff(Double_t iN0,     Double_t eN0,
                         Double_t iN1,     Double_t eN1,
                         Double_t iN2m,    Double_t eN2m,
                         Double_t iN2b=0., Double_t eN2b=0.);
    Double_t getP0l(void) {return p0l;}
    Double_t getP1l(void) {return p1l;}
    Double_t getP2l(void) {return p2l;}
    Double_t getP3l(void) {return p3l;}
    Double_t getP4l(void) {return p4l;}
    Double_t getP5l(void) {return p5l;}
    Double_t getP6l(void) {return p6l;}

  // Double_t getTotNumTr(void);

  protected:
    virtual Double_t efficiency(Double_t n0, Double_t n1, 
                                Double_t n2m, Double_t n2b=0.);

  ClassDef(HMdcEffMod,0)
};



class HMdcEfficiencyLayer : public TObject {
  friend class HMdcEfficiency;
  protected:
    Short_t  sector;
    Short_t  module;
    Short_t  layer;
    Short_t  is;

    Short_t nCells;
    Int_t *nCellTot;   // Total num. times a cell was fired (without cut)
    Int_t *nCellCutS;  // Total num. times a cell was fired (with cut ~common stop)
    Int_t *nCellStat;  // Total num. times a cell was fired (after cut)
    Int_t *nCellsClus; //  cells which have a part in clusters
    Int_t *n0;         // n0[cell number] num. times cell was not fired
    Int_t *n1;         // n1[cell number] num. single hits (the cell was fired
                       //        but closest cells not)
    Int_t *n2;         // n2[cell number] num. double hits (this cell and
                       //        cell "cell_number-1" was fired)
    Int_t nBinSum;
    HMdcEff eff;
    TH1F *hists[13];
    Int_t nPads;
    Int_t listPads[5];
    TCanvas* canvEff;
    Text_t nameCanv[20];
    Text_t titleCanv[60];

    HMdcEff effXY;
    TCanvas* canvXY[3];
    Text_t nameCanvXY[3][20];
    Text_t titleCanvXY[3][60];
    TPaveText titlePadXY[3];
    Int_t indDrawTitXY[3];

    Int_t nBinsSumXY;
    Int_t n0xy[256];
    Int_t n1xy[256];
    Int_t n2xy[256];
    TPaveText pvTxEffXY[3][256];
    UChar_t indDrawPv[3][256];

    UChar_t *nClustCell; // nClustCell[nCell] - num. of clusters for cell=nCell

  public:
    HMdcEfficiencyLayer(Short_t sec, Short_t mod, Short_t lay);
    ~HMdcEfficiencyLayer();
    void clear();
    void clearClCells(void);
    void addCellTot(Short_t cell);
    void addCellCutS(Short_t cell);
    void addCellClus(Short_t cell);
    void addCell(Short_t cell);
    void addCellInClust(Short_t cell);
    UChar_t getNumClCell(Short_t cell) {return nClustCell[cell];}
    void add0Hit(Short_t cell, Int_t nB);
    void add1Hit(Short_t cell, Int_t nB);
    void add2Hit(Short_t cell1, Short_t cell2, Int_t nB);

    Int_t getCellStat(Short_t cell);
    Short_t getNumCells(){return nCells;}
    Short_t getN0Hit(Short_t cell){return n0[cell];}
    Short_t getN1Hit(Short_t cell){return n1[cell];}
    Short_t getN2Hit(Short_t cell){
                     return (cell+1<nCells) ? n2[cell]+n2[cell+1] : n2[cell];}
    void getEffCells(HMdcEff* efficiency, 
                     Short_t fCell=-1000, Short_t lCell=1000);
    void getEffXY(HMdcEff* efficiency);
    void getEffXY(HMdcEff* efficiency, Short_t xBin, Short_t yBin);
    void getEffXY(HMdcEff* efficiency, Short_t fXbin, Short_t lXbin,
                                       Short_t fYbin, Short_t lYbin);
    void fillHists(Int_t iBinSum=1);
    void drawTotNhits();
    void drawNHits();
    void drawHistEff();
    void drawHistSing();
    void drawProbabilitis();
    void updateCanv(void);
    void setListPads(Int_t* listPad);
    void drawCanvas();
    void delCanvas();

    void drawCanvXYlist();
    void updateCanvXYlist();
    void delCanvXYlist();
    void drawCanvXYnHits() {drawCanvXY(0);}
    void drawCanvXYefficiency() {drawCanvXY(1);}
    void drawCanvXYsingle() {drawCanvXY(2);}
    void drawCanvXYprob() {drawCanvXY(3);}
    void setBiningXY(Int_t nBinsSum);

  protected:
    void setTitle(TH1F* &hist, Char_t* nm);
    void drawLineTot(TH1F* hst, Int_t color, Float_t eff, Float_t errEff);
    void setPads();
    void creatCanvas();
    void createHists(void);
    void setPavesXY();
    void drawCanvXY(Int_t nPl);
    void fillPaveXYnHits(Int_t nPv);
    void fillPaveXYeff(Int_t nPv);
    void fillPaveXYsing(Int_t nPv);

  ClassDef(HMdcEfficiencyLayer,0)
};

class HMdcEfficiencyMod : public TObject {
  protected:
    Short_t  sector;
    Short_t  module;
    Int_t n0;   // n0 num. times a layer was not fired
    Int_t n1;   // n1 num. single hits
    Int_t n2;   // n2 num. double hits

    Float_t xLow;   // Low and up bordes of project plane
    Float_t xUp;    //
    Float_t dX;     // step of plot of efficiency
    Float_t yLow;   //
    Float_t yUp;    //
    Float_t dY;     //
    Int_t n0xy[256];    // arrays for keeping of inform. about eff. of MDC regions
    Int_t n1xy[256];    //
    Int_t n2xy[256];    //
  //  TPaveText pvTxEffXY[256];

    TObjArray* array;    // array of pointers of type HMdcEfficiencyLayer

    Int_t listLayers[6]; // The list of layers for drawing of hists
    Int_t nPads;
    TCanvas* canvases[5];
    Text_t nameCanv[5][20];
    Text_t titleCanv[5][60];


    HMdcEff eff;
    HMdcEffMod effMod;
    HMdcLookupGMod* flookupGMod;  // pointer to cont. HMdcLookupGeom[sec]mod]
    HMdcRawModStru* frawModStru;  // pointer to cont. HMdcLookupRaw[sec]mod]
    TH1F* hsEffMb;
    TH1F* hsEffMb4;
    Int_t nChannels;
    TCanvas* canvasMb;
    Text_t nameCanvMb[20];
    Text_t titleCanvMb[60];
    TSlider* mbSlider;
    Text_t nameSliderMb[20];

    TH1F* hsTimes[9];
    TH1F* drHsTimes[15];
    Float_t minTimeBin[3];
    Float_t maxTimeBin[3];
    Text_t nameCanvTimes[20];
    Text_t titleCanvTimes[60];
    Int_t biningTimes;

  public:
    HMdcEfficiencyMod(Short_t sec, Short_t mod);
    ~HMdcEfficiencyMod();
    HMdcEfficiencyLayer& operator[](Int_t i) {
        return *static_cast<HMdcEfficiencyLayer*>((*array)[i]);
    }
    void clear();
    void add0Hit(Int_t nB) { n0++; if(nB>=0) n0xy[nB]++;}
    void add1Hit(Int_t nB) { n1++; if(nB>=0) n1xy[nB]++;}
    void add2Hit(Int_t nB) { n2++; if(nB>=0) n2xy[nB]++;}
    Int_t getBinNum(Float_t x, Float_t y);
    void getEffXY(HMdcEffMod* efficiency);
    void getEffXY(HMdcEffMod* efficiency, Short_t xBin, Short_t yBin);
    void getEffXY(HMdcEffMod* efficiency, Short_t fXbin, Short_t lXbin,
                                          Short_t fYbin, Short_t lYbin);

    void setListLayers(Int_t* listLay);
    void updateCanv(void);

    void drawHists(Int_t nCanv);
    void drawCanvTotNhits();
    void drawCanvNHits();
    void drawCanvHistEff();
    void drawCanvHistSing();
    void drawCanvProbabilitis();

    void delCanv(Int_t nCanv);
    void delCanvTotNhits() {delCanv(0);}
    void delCanvNHits(){delCanv(1);}
    void delCanvHistEff(){delCanv(2);}
    void delCanvHistSing(){delCanv(3);}
    void delCanvProbabilitis(){delCanv(4);}

    void fillHistMb();
    void drawCanvEffMb();
    void delCanvMb();
    void ExecuteEvent(Int_t event, Int_t px, Int_t py);  

    void fillTimesAll(HMdcCal1* cal) {fillTimes(cal, 0);}
    void fillTimesCls(HMdcCal1* cal) {fillTimes(cal, 3);}
    void fillTimesSelCls(HMdcCal1* cal) {fillTimes(cal, 6);}
    void drawCanvTimes(Int_t bining=-1);
    void delCanvTimes();

  protected:
    void fillTimes(HMdcCal1* cal, Int_t nHist);
    void setNameCanv(void);
    void creatCanvas(Int_t nCanv);
    void creatCanvasMb(void);
    void setPads(TCanvas* canv);
    void drawLineGrMb(TH1F* hst);
    void drawLineEffMb(TH1F* hst);

  ClassDef(HMdcEfficiencyMod,0)
};

class HMdcEfficiencySec : public TObject {
  protected:
    Short_t  sector;
    TObjArray* array;   // array of pointers of type HMdcEfficiencyMod
    TCanvas* canvPlotProj;
    Char_t nameCanvProj[20];
    Char_t titleCanvProj[60];
    TCanvas* canvPlotsHits;
    Char_t nameCanvHits[20];
    Char_t titleCanvHits[60];
    TH2F* plotHits[2];

  public:
    HMdcEfficiencySec(Short_t sec);
    ~HMdcEfficiencySec();
    HMdcEfficiencyMod& operator[](Int_t i) {
      return *static_cast<HMdcEfficiencyMod*>((*array)[i]);
    } 
    TH2F* getPlotAllHits() {return plotHits[0];}
    TH2F* getPlotSelHits() {return plotHits[1];}
    TCanvas* getCanvasProj() {return (TCanvas*)gROOT->FindObject(nameCanvProj);}
    TCanvas* getCanvasHits() {return (TCanvas*)gROOT->FindObject(nameCanvHits);}
    void drawCanvProj();
    void drawCanvHits();
    void delCanvProj();
    void delCanvHits();
    void creatPlots(HMdcLookUpTbSec* flookupTbSec);

  protected:

  ClassDef(HMdcEfficiencySec,0)
};



class HMdcEfficiency : public HReconstructor {
  private:
    Bool_t isCreated;
    TObjArray* array;     // array of pointers of type HMdcEfficiencySec

    HCategory* fCalCat;                // pointer to cal data category
    HLocation loc;                     // location for new object 
    Bool_t isGeant;                    // =kTRUE for GEANT data
    HIterator* iter;
    HMdcLookUpTb* fLookUpTb;
    HMdcGetContainers* fGetCont;
    HMdcGeomStruct* fGeomStr;          //
    HMdcDetector* fMdcDetector;
    HMdcSizesCells* fSizesCells;
    HGeomTransform sysLayers[6][4][6]; //[sec][mod][layer]

    UChar_t qual[6][200][4];     //[sec][nCl][mod]
  //  Short_t maxClusters[6];
    Int_t eventId;

    Int_t typePrPlot[6];
    TCanvas* canvEventProj;
    Char_t nameCanvEvent[20];
    Char_t titleCanvEvent[60];

    HMdcTimeCutD* fcut;
  public:
    HMdcEfficiency();
    HMdcEfficiency(Text_t *name,Text_t *title);
    ~HMdcEfficiency(void);

    HMdcEfficiencySec& operator[](Int_t i) {
        return *static_cast<HMdcEfficiencySec*>((*array)[i]);
    }


    Bool_t init(void);
    Bool_t reinit(void);
    Bool_t finalize(void);
    Int_t  execute(void);
    void drawProjPlotsSec(Int_t sec, Int_t type=0);
    void drawProjPlots(Int_t sec, Int_t type=0);
    void drawHitsPlots(Int_t sec);
    void drawCanvEvent();
    void delCanvEvent();
    void drawPrPlEvent(Int_t type=0);

    void calcEfficiency(Int_t iBinSum=1);
    void printClusters(Int_t sec);
  private:  
    void initSecArray();
  ClassDef(HMdcEfficiency,0) //Digitizes MDC data.
};

#endif
