#ifndef HSTARTHITF_H
#define HSTARTHITF_H

#include "hreconstructor.h"
#include "hlocation.h"
 
class HCategory;
class HIterator;
class HStartSelfCoPar;
class TH1F;

class HStartHitF : public HReconstructor {
protected:
  HCategory *calCat;          // pointer to the cal data
  HCategory *hitCat;          // pointer to the hit data
  HIterator *iter;            // iterator on cal data.
  HLocation loc;              // location for new hit object
  HStartSelfCoPar* selfcopar; // start selfcoincidence parameters
  Bool_t fControlHistograms;  // flag for control histograms

  TH1F *histMultStart;
  TH1F *histPatternStart;
  TH1F *histStartStat;
  TH1F *histStartC[8];
  TH1F *histStartH[8];

public: 
  HStartHitF(void);
  HStartHitF(Text_t* name,Text_t* title);
  ~HStartHitF(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);

  void enableControlHistograms() { fControlHistograms = kTRUE; }

  ClassDef(HStartHitF,0) // Hit finder for START detector
};

#endif /* !HSTARTHITF_H */
















