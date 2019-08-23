#ifndef HSTARTHITF_H
#define HSTARTHITF_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "htofinocalpar.h"
#include "hdetector.h"
 
class HCategory;
class HIterator;
class HStartSelfCoPar;
class TH1F;
class HTofRaw;
class HSpectrometer;
class HTofCalPar;
class HTofinoCalPar;

class HStartHitF : public HReconstructor {
protected:
  HDetector *tofdet;
  HDetector *tofinodet;
  HCategory *fTofRawCat;         // pointer to the TOF Raw data
  HTofCalPar *fTofCalPar;        // TOF  Calibration parameters
  HIterator *iterTof;            // iterator over Tof Raw data

  HCategory *fTofinoRawCat;      // pointer to the TOFino Raw data
  HTofinoCalPar *pTofinoCalPar;  // TOFino  Calibration parameters
  HIterator *iterTofino;         // iterator over Tofino Raw data



  HCategory *calCat;          // pointer to the cal data
  HCategory *hitCat;          // pointer to the hit data
  HIterator *iter;            // iterator over cal data.
  HLocation loc;              // location for new hit object
  HStartSelfCoPar* selfcopar; // start selfcoincidence parameters
  Bool_t fControlHistograms;  // flag for control histograms

  TH1F *histMultStart;
  TH1F *histPatternStart;
  TH1F *histStartStat;
  TH1F *histStartC[8];
  TH1F *histStartH[8];
  TH1F *m_pTofinoTime;
  TH1F *m_pTofTime;


public: 
  HStartHitF(void);
  HStartHitF(Text_t* name,Text_t* title);
  ~HStartHitF(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  void initParContainer();
  void enableControlHistograms() { fControlHistograms = kTRUE; }

  ClassDef(HStartHitF,0) // Hit finder for START detector
};

#endif /* !HSTARTHITF_H */

