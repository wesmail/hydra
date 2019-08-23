#ifndef HMDCEFFMENU_H
#define HMDCEFFMENU_H

#include "TObject.h"
#include "hmdcefficiency.h"
#include "TDialogCanvas.h"
#include "TButton.h"
#include "TPaveLabel.h"
#include "TText.h"

class HMdcEffMenu : public TDialogCanvas {
  protected:
    static HMdcEffMenu* fEffMenu;
    HMdcEfficiency* mdcEff;
    HMdcEfficiencySec* feffSec;
    HMdcEfficiencyMod* feffMod;
    HMdcEfficiencyLayer* feffLay;
    TButton *bSecMod[6][4];
    TButton *bLayer[6];
    TButton *bGroupMdcLay[2];
    TButton *button[30];
    TButton *bPlot[3];
    TButton *bTypePlot[3];
    TButton *bHist[5];
    TButton *bHistMb;
    TButton *bXYlayer;
    TButton *bTimes;
    Int_t indSec[6];
    Int_t indMod[6][4];
    Int_t indLay[6];          //for all selected modules
    Int_t numLay;
    Int_t indPlot[3];
    Int_t typePlot;
    Int_t indHist[5];
    Int_t indHistMb;
    Int_t numHists;
    Int_t indXYlayers;
    Int_t indTimes;
    Int_t indGroup[2];
    Int_t sumBnXY;
    Int_t sumBnTimes;
    TPaveLabel* pvLSumXY;
    TPaveLabel* pvLSumTimes;
    Char_t buf1[100];
    Char_t buf2[100];
    TText* text;
    Int_t totModsSel;
    Int_t nTotEv;
    Bool_t calcEffFlag;
    
    HMdcEffMenu(HMdcEfficiency *feff);
    ~HMdcEffMenu();
    void menu();
  public:
    static HMdcEffMenu* makeMenu(HMdcEfficiency *feff);
    static void deleteMenu(void);
    static void closeDial(void) {fEffMenu->Close();}
    void selectingSecMod(Int_t s, Int_t m);
    void selectingLayer(Int_t l);
    void setListPads();
    void setListLayer();
    void selectingPlot(Int_t n);
    void selectingTypePlot(Int_t n);
    void selectingHist(Int_t n);
    void selectingXYlayer();
    void drawTimes();
    void selectingHistMb();
    void selectingGroup(Int_t n);
    void drawButton(TButton* bt, Int_t nFnt=42, Int_t nTCl=1);
    void selectingSumTimes(Int_t n);
    void selectingSumXY(Int_t n);
    void printClusters();
    void doDraw(Int_t ind=1);
    void doPlots(Int_t ind=1);
    void doEventPlots(Int_t ind=1);
    void doEvents(Int_t nEv);
    void calcEff(void);
    ClassDef(HMdcEffMenu,0) //Digitizes MDC data.
};
    
#endif
