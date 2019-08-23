#ifndef HTOFOFFLINER_H
#define HTOFOFFLINER_H

#include "hreconstructor.h"
#include <iostream>
#include <hstartdef.h>
#include <tofdef.h>

class HCategory;
class HIterator;
class TH1F;
class TH2F;
class TFile;

class HTofOfflineR : public HReconstructor {
protected:
  HCategory* fTBoxCat;       //! pointer to the tof raw data
  HIterator* iter_tbox;         //! iterator on tof raw data.
  HCategory* fRawTofCat;       //! pointer to the tof raw data
  HIterator* iter_tof;         //! iterator on tof raw data.
  HCategory* fHitTofCat;       //! pointer to the tof hit data
  HIterator* iter_htof;         //! iterator on tof hit data.
  HCategory* fRawTofinoCat;       //! pointer to the tofino raw data
  HIterator* iter_tofino;         //! iterator on tofino raw data.
  HCategory* fCalTofinoCat;       //! pointer to the tofino cal data
  HIterator* iter_ctofino;         //! iterator on tofino cal data.
  HCategory* fRawStartCat;       //! pointer to the start raw data
  HIterator* iter_start;         //! iterator on start raw data.
  HCategory* fCalStartCat;       //! pointer to the start cal data
  HIterator* iter_cstart;         //! iterator on start cal data.
  HCategory* fHitStartCat;       //! pointer to the start hit data
  HIterator* iter_hstart;         //! iterator on start hit data.

  TH1F *histStartScalerPattern;
  TH1F *histVetoScalerPattern;
  TH1F *histInputScalerPattern;
  TH1F *histReduceScalerPattern;

  TH2F *histMultvsTrigBit;
  TH2F *histMultvsDownFlag;
  TH2F *histMultvsDownFlagM4;


  TH1F *histMultStart;
  TH1F *histMultVeto;
  TH1F *histPatternStart;
  TH1F *histPatternVeto;
  TH1F *histPatternHalo;
  TH1F *histStartStat;
  TH1F *histMultTofL;
  TH1F *histMultTofR;
  TH1F *histMultTof;
  TH1F *histMultTofTofino;
  TH1F *histMultTofTofinoTrigger;
  TH1F *histMultAnalog;
  TH2F *histMult2;
  TH1F *histPatternTof;
  TH1F *histPatternTofl;
  TH1F *histPatternTofr;
  TH2F *histTDCleft;
  TH2F *histTDCright;
  TH2F *histADCleft;
  TH2F *histADCright;
  TH2F *histADCleftOK;
  TH2F *histADCrightOK;
  TH2F *histADCl_cal;
  TH2F *histADCl_cal_mip;
  TH2F *histADCr_cal;
  TH2F *histADCr_cal_mip;
  TH2F *histADC_cal;
  TH2F *histADC_cal_mip;
  TH2F *histTDCsum;
  TH2F *histTDCdiff;
  TH2F *histADCsum;
  TH2F *histADCpos;
  TH1F *histTOFsum;
  TH1F *histTOFsumnocor;
  TH1F *histPOSsum;
  TH2F *histEdepvstofsum;
  TH1F *histTOFtheta;
  TH1F *histTOFth[6];
  TH1F *histTOFphi;
  TH2F *histTOFpos[6];
  TH2F *histTOFyvsx;
  TH1F *histLeftT[6][8][8];
  TH1F *histRightT[6][8][8];
  TH1F *histSum[6][8][8];
  TH1F *histDiff[6][8][8];
  TH2F *histEdepvstof[6][8][8];
  TH1F *histStartT[16];
  TH1F *histStartC[16];
  TH1F *histStartH[16];
  TH1F *histVetoT[16];
  TH1F *histHaloT[16];
  TH1F *histTOFsumS[16];
  TH1F *histReduce[8];
  TH1F *histInput[8];
  TH1F *histStartScaler[8];
  TH1F *histVetoScaler[8];
  TH1F *histPatternTofino;
  TH1F *histMultTofino;
  TH1F *histTofinoTime[6][4];
  TH2F *histTofinoTDC;
  TH2F *histTofinoADC;
  TH2F *histTofinoADCok;

  TH2F *histtrigger;
  TH2F *histtrigger2;
  TH1F *histtofdif;

  Int_t counter_execute,empty,full;

  Char_t *fNameAsciiTofOfflineR;                    // file name of ascii output file
  Char_t *fNameRootTofOfflineR;                     // file name of root output file
  TFile *ofile;
  void createHist(TFile*);
  void writeHist (TFile*);

public:
  HTofOfflineR();
  HTofOfflineR(Text_t* name,Text_t* title);
  ~HTofOfflineR();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  //  void initParContainer();
  void setOutputAscii(Char_t*);
  void setOutputRoot (Char_t*);

  ClassDef(HTofOfflineR,0)
};

#endif /* !HTOFOFFLINER_H */

