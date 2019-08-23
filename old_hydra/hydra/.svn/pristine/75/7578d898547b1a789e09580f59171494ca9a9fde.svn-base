#ifndef HMDC34TRACKFINDER_H
#define HMDC34TRACKFINDER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "hmdctimecut.h"
#include "hmdc34clfinder.h"
#include "hmdcseg.h"
#include "hmdcclussim.h"

#include "TCanvas.h" //----????

class HIterator;

class HMdc34TrackFinder : public HReconstructor {
private:
  HCategory* fCalCat;        // pointer to CAL1 data category
  Bool_t isGeant;            // =kTRUE for GEANT data
  HIterator* iter;           // iterator for fCalCat
  HMdc34ClFinder* fClFinder; // Clusters finder
  HMdcDetector* fmdc;
  const Int_t *modules;      // pointer to list of modules in HMdcDetector
  HCategory* fClusCat;        // pointer to HMdcClus data category
  Bool_t perMdcClus;         // =kTRUE,kFALSE - write or not HMdcClus to file
  HMdcClusFilter filter;     // filter for removing deleted clusters
    
  HMdcTimeCut* fcut;         // Cut 
  
  Int_t level4;              // Level of 4-layers finding (<= level4 Mdc hits)
  Int_t level5;              // Level of 5-layers finding
  HLocation locSeg;          // location for "segment" object
  HCategory* fSegCat;        // pointer to Segment data category
  HIterator* iterSeg;
  
TCanvas* canv[6]; //--------------?????????
Char_t nameCanv[6][20]; //-----??????

public:
  HMdc34TrackFinder(Bool_t per=kFALSE);
  HMdc34TrackFinder(Text_t *name, Text_t *title, Bool_t per=kFALSE);
  ~HMdc34TrackFinder(void) {;}
  Bool_t init(void);
  Bool_t reinit(void) {return kTRUE;} //???????????????
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void setLevel(Int_t l4, Int_t l5) {level4=l4; level5=l5;}
public:
  ClassDef(HMdc34TrackFinder,0) //Digitizes MDC data.
};

#endif
