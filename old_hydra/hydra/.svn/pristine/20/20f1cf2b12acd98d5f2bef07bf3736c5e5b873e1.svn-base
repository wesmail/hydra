#ifndef HMDCSELFTRACKING_H
#define HMDCSELFTRACKING_H

#include "hreconstructor.h"

class TDirectory;
class TFile;
class HCategory;
class HIterator;
class TH1F;
class TH1D;
class TH2S;
class TProfile;

class HMdcSelfTracking : public HReconstructor {
protected:

// some local declarations:
Float_t t1[7][200], t2[7][200], tt1, tt2;

// *** cuts ***
Float_t tdifmin;
Float_t tdifmax;
Float_t tdifcut;
Float_t tmin;
Float_t tmax;
// *** end cuts ***

Int_t cellmin;
Int_t cellmax;

Int_t  module, sector, cell, layer;
Int_t  c[7][200];
Int_t nhit[7];
Int_t flag[10];
Int_t  ihit,lhit[3];
Int_t  i,ii,j,jj;
Int_t id;


Int_t il1;
Int_t il2;

Int_t cnt0;
Int_t cnt1;
Int_t cnt2;

Float_t counter[3][160];
Int_t ctrl[11];

    HCategory     *calCat;           // pointer to the CAL1 data
    HIterator     *iter;             // iterator on raw data

    Char_t *fNameRoot;               // file name of root output file

    TH1D *ptdif[2];
    TH1D *hlayer[7];
    TH1D *gain;
    TH1D *info;
    TH1D *control;
    TH1D *layer0[3];
    TH1F *eff;

    TH2S *pTime_cell[7];
    TH2S *pTime0_cell[7];
    TH2S *pTimesum_cell[3];
    TH2S *pTimesumdif[3];
    TH2S *pFish[3][120];
    TH2S *pTsum_ev[3][120];

    TProfile *pTsum_ev_prof[3][120];
    TFile* fout;
    
public:
    HMdcSelfTracking();
    HMdcSelfTracking(Text_t* name,Text_t* title);
    ~HMdcSelfTracking();
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute();
    void setOutputRoot (Char_t*);

protected:
    void createHist();
    void resetCounters();
    void fillControlHists();
    void fillHist  ();
    void writeHist ();
  //  void deleteHist();

public: // This has to be placed at the end (--> root dox)
    ClassDef(HMdcSelfTracking, 0)  // class for the self tracking of the Mdc
};

#endif

