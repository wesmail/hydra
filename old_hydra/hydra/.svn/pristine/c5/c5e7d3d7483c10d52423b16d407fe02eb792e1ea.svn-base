#ifndef HMDCRICHMATCH_H
#define HMDCRICHMATCH_H

#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HMdcLookUpTb;
class HMdcTimeCut;
class HMdcCalibrater1;
class TH1F;

class HMdcRichCorr : public HReconstructor  {
  private:
    TString fileName;
    TString runId;
    TString eventDate;
    TString eventTime;

    HMdcCalibrater1* fCalib1;  // pointer to mdc calibrater1

    HCategory* fCal1Cat;       // pointer to CAL1 data category
    HIterator* iterCal1;       // iterator for fCal1Cat

    HCategory* fClusCat;       // pointer to HMdcClus data category
    HIterator* iterMdcClus;    // iterator

    Int_t nMdcHits[6];         // num. of mdc hits in [sector]

    HCategory* fRichHitCat;    // pointer to HMatchURich or HRichHit data cat.
    HIterator* iterRichHit;
    Int_t richFlag;            // =0 - HMatchURich; =1 - HRichHit

    Int_t nRichHits[6];        // num. of rich hits in [sector]
    Float_t richTheta[6][400]; // rings param.
    Float_t richPhi[6][400];   //
    Int_t nMdcHitRing[6][400]; // kTRUE - ring

    Float_t radToDeg;          //

    HMdcLookUpTb* fLookUpTb;   // clus. finder for MDC1&2 (and 3&4 for type=1)
    Int_t level4;              // Level[seg] of 4-layers finding (<= level4 Mdc hits)
    Int_t level5;              // Level[seg] of 5-layers finding
    Int_t nLayers[6][4];       // Number of layers in modules [sec][mod]
    Int_t clFinderType;        // Cluster finder type

    HMdcTimeCut* fcut;         // Times cut

    // Matching cuts:
    Int_t nRingsCut;           // Cut for number of matched rings
    Float_t dThCuts[6][2][2];  // [sec][mod][] dTheta bounds (rich-mdc)
    Float_t dPhCuts[6][2][2];  // [sec][mod][] dPhi bounds (rich-mdc)

    // Statistics:
    Int_t nEvents;             // Total number of events.
    Int_t nRingEvents;         // Number of events with rings
    Int_t nMatchedEvents;      // Number of matched events
    Int_t nMatchedEvents1R;    // Number of events with 1 matched rings
    
    //Histograms:
    TString histFileDir;
    TString histFileSuffix;
    TString histFileOption;
    TH1F* hDThAll[2];
    TH1F* hDPhAll[2];
    TH1F* hDTh[6][2];
    TH1F* hDPh[6][2];
    TH1F* hNRings;
    TH1F* hNMRings;
    TH1F* hNEvents;

  public:
    HMdcRichCorr(void);
    HMdcRichCorr(Text_t *name, Text_t *title, Int_t nRCut=1);
    ~HMdcRichCorr(void);

    Bool_t init(void);
    Bool_t reinit(void);
    Bool_t finalize(void);
    Int_t  execute(void);
    void printStatus(void) const;

    void makeCalibrater1(Int_t vers=1, Int_t cut=1, Int_t domerge=0);

    void setNLayers(Int_t *lst);
    void setTypeClFinder(Int_t type) {clFinderType=type;}
    void setLevel(Int_t l4s1, Int_t l5s1) {
      level4=l4s1;
      level5=l5s1;
    }

    void setDThetaCuts(Float_t dTh1, Float_t dTh2);
    void setDPhiCuts(Float_t dPh1, Float_t dPh2);
    void setDThetaCuts(Float_t* dTh);
    void setDPhiCuts(Float_t* dPh);
    
    void setHistFile(char* dir,char* suf,char* option="NEW") {
      histFileDir=dir;
      histFileSuffix=suf;
      histFileOption=option;
    }

  private:
    void setup(void);
    void fillListByRichHit(void);
    void fillListByMathURich(void);
    Bool_t reinitMdcFinder(void);
    void findMdcClusters(void);
    void makeHist(void);

  ClassDef(HMdcRichCorr,0)
};

#endif
