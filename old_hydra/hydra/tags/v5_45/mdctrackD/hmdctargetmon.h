#ifndef HMDCTARGETMON_H
#define HMDCTARGETMON_H

#include "hreconstructor.h"
#include "hgeomvector.h"

class HIterator;
class HCategory;
class HMdcTimeCut;
//class HMdcGetContainers;
class HMdcSizesCells;
class TH1F;
class TH2S;
class TNtuple;
class TFile;
class HEventHeader;
class HGeomTransform;

class HMdcTargetMon : public HReconstructor {
  protected:
//    TH1* hist[1000];
    TNtuple* tuple;
    
    HCategory* fMdcHitCat;        // pointer to HMdcHit data category
    HIterator* iterMdcHit;        // iterator
    HLocation locHit;             // location
    
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
    
    Text_t* htFileName;                     // file name for hist. & tupl.
    TFile* htFile;
    HGeomTransform* transf[24];
    
    Int_t nSectors;
    Int_t nPads[6];         // [comb]
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
    TH1F* dPhiH[6];       // 6 combinations of MDC,s 1-2,1-3,1-4,2-3,2-4,3-4.
    TH1F* r0H[6];         // -/-
    TH1F* zVerH[6][6];    // [sec][comb]
    TH2S* zVerVsEventH[6][6];  // -/-
    char cnCut[100];
    char cnComb[6][100];
    char ctCut[100];
    char ctComb[6][100];
    
    Int_t step;
  public:
    HMdcTargetMon(Bool_t isClOff=kFALSE, Int_t step=500, Text_t *fileName=0);
    HMdcTargetMon(Text_t *name,Text_t *title, Bool_t isClOff=kFALSE, 
        Int_t step=500, Text_t *fileName=0);
    ~HMdcTargetMon(void);
    virtual Int_t execute(void);
    virtual Bool_t init(void);
    virtual Bool_t reinit(void);
    virtual Bool_t finalize(void);
    void drawCanvCut(void);
//    void drawCanv(void);  //MSG: Implementation is missing

//    TH1* getHist(Int_t n){return (n>=0 && n<=1000) ? hist[n]:0;}

  private:
    void setParam(Text_t *fileName);
    void extractFileInfo(void);
    void fillDateTime(void);
    void setCanvName(void);

  ClassDef(HMdcTargetMon,0)
};

#endif
