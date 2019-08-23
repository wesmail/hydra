#ifndef HMDCTARGETMON_H
#define HMDCTARGETMON_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include <TStopwatch.h>

class HIterator;
class HCategory;
class HMdcTimeCut;
//class HMdcGetContainers;
//class HMdcSizesCells;
class TH1F;
class TH2S;
class TFile;
class HEventHeader;
class HGeomTransform;
class TPad;
class TPaveLabel;

class HMdcTargetMon : public HReconstructor {
  protected:

    Bool_t mode;              // kTRUE - show canvases at the call finalize
                              // kFALSE - show canvases and update them each
                              // 1000 events
    TString typePrintFile;    // "" - ps, ".gif", ...
    HCategory* fMdcHitCat;    // pointer to HMdcHit data category
    HIterator* iterMdcHit;    // iterator
    HLocation locHit;         // location

    HEventHeader* evHeader;
    HGeomVector target;

//    HMdcGetContainers* fGetContainers;
//    HMdcSizesCells* fSizesCells;
    Bool_t isCoilOff;

    Bool_t firstEvent;
    UInt_t dateFEvent;
    UInt_t timeFEvent;
    UInt_t dateLEvent;
    UInt_t timeLEvent;
    UInt_t runId;
    TString fileName;
    TString dateTime;

    HGeomTransform* transf[24];

    Int_t nSectors;
    Int_t nPads[6];         // [comb]
    TPad*  fPads[6][12];
    TPaveLabel* label;      // fileName & time label
    Int_t mdcInSec[6];      // [sec]
    Bool_t listMdc[24];

    Double_t theta[24][500];
    Double_t phi[24][500];
    Double_t x[24][500];
    Double_t y[24][500];
    Double_t z[24][500];
    Double_t r2Hit[24][500];
    Short_t nhits[24];

    Double_t dPhiCut;
    Double_t r0Cut;

    Int_t combToMdc[6][2];
    TH1F* dPhiH[6];           // 6 combinations of MDC,s 1-2,1-3,1-4,2-3,2-4,3-4
    TH1F* r0H[6];             // -/-
    TH1F* zVerH[6][6];        // [sec][comb]
    TH1F* zVerDscH[6][6][2];  // [sec][comb][DownscalingFlag]
    Int_t nZmBins;            // histogramms parameters
    Double_t zmin;            //
    Double_t zmax;            //
    TH2S* zVerVsEventH[6][6]; // -/-
    char cnCut[100];
    char cnComb[6][100];
    char ctCut[100];
    char ctComb[6][100];

    TStopwatch timer;
    UInt_t plotType;

    Double_t timeStep;        // canvases updating time step
  public:
    HMdcTargetMon(Bool_t isClOff=kFALSE, UInt_t pt=0);
    HMdcTargetMon(Text_t *name,Text_t *title, Bool_t isClOff=kFALSE,
        UInt_t pt=0);
    ~HMdcTargetMon(void);
    virtual Int_t execute(void);
    virtual Bool_t init(void);
    virtual Bool_t reinit(void);
    virtual Bool_t finalize(void);
    void drawCanvCut(void);
    void setTimeStep(Double_t tst) {timeStep=(tst<3.) ? 3.:tst;}
    void setMode(Bool_t m, Char_t* fileType=0);
    void setHistPar(Int_t nb, Double_t z1, Double_t z2) {
      nZmBins=nb;
      zmin=z1;
      zmax=z2;
    }

  private:
    void setParam(UInt_t pt=0);
    void extractFileInfo(void);
    void fillDateTime(void);
    void setCanvName(void);

  ClassDef(HMdcTargetMon,0)
};

#endif
