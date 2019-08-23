#ifndef HMDCTRACKFINDER_H
#define HMDCTRACKFINDER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "hmdctimecut.h"

class HIterator;

class HMdcTrackFinder : public HReconstructor {
private:
  HCategory* fCalCat;         // pointer to CAL1 data category
  Bool_t isGeant;             // =kTRUE for GEANT data
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb;
  HMdcDetector* fmdc;
  Int_t nModules[6];
  Int_t listModules[6][4];
  Bool_t isCoilOff;
  Int_t typeClFinder;                  // =0 - finding in all mdc in sec. 
                                       // =1 - finding in each mdc indep.
  HCategory* fClusCat;        // pointer to HMdcClus data category
  Bool_t perMdcClus;          // =kTRUE,kFALSE - write or not HMdcClus to file
  HLocation locCl;
  
  HMdcTimeCut* fcut;          // Cut 
  
  Int_t level4;               // Level of 4-layers finding (<= level4 Mdc hits)
  Int_t level5;               // Level of 5-layers finding 
  
  void fillCatMdcClus(Int_t sec);
public:
  HMdcTrackFinder(Bool_t per=kFALSE, Bool_t isCOff=kFALSE, Int_t typeClFn=0);
  HMdcTrackFinder(Text_t *name, Text_t *title, Bool_t per=kFALSE, Bool_t isCOff=kFALSE, Int_t typeClFn=0);
  ~HMdcTrackFinder(void) {;}
  Bool_t init(void);
  void setParContainers();
  void setNLayers(Int_t *lst);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void setLevel(Int_t l4, Int_t l5) {level4=l4; level5=l5;}
public:
  ClassDef(HMdcTrackFinder,0) //Digitizes MDC data.
};

#endif









