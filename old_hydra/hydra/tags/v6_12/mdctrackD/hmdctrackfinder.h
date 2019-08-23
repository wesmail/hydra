#ifndef HMDCTRACKFINDER_H
#define HMDCTRACKFINDER_H

#include "hreconstructor.h"

class HIterator;
class HCategory;
class HMdc34ClFinder;
class HMdcTimeCut;
class HMdcLookUpTb;

class HMdcTrackFinder : public HReconstructor {
private:
  HCategory* fCalCat;           // pointer to CAL1 data category
  Bool_t isGeant;               // =kTRUE for GEANT data
  HIterator* iter;              // iterator for fCal1Cat
  HMdcLookUpTb* fLookUpTb;      // clus. finder for MDC1&2 (and 3&4 for type=1)
  HMdc34ClFinder* f34ClFinder;  // Clusters finder for MDC3&4
  Int_t nModules[6];            //
  Int_t listModules[6][4];      //
//  Bool_t isCoilOff;
  Int_t typeClFinder;           // =0 - finding in all mdc in sec.
                                // =1 - finding in each mdc indep.
  HCategory* fClusCat;          // pointer to HMdcClus data category
  Bool_t perMdcClus;            // =kTRUE - write HMdcClus to file
  HLocation locClus;            // loc. for HMdcClus category
  HLocation locNewCl;           // loc. for HMdcClus category
  HIterator* iterClus;          // iterator for fClusCat
  
  HMdcTimeCut* fcut;            // Times cut 
  
  Int_t level4[2];       // Level[seg] of 4-layers finding (<= level4 Mdc hits)
  Int_t level5[2];       // Level[seg] of 5-layers finding
  
  Bool_t prntSt;         // kTRUE - status printed already
    
public:
  HMdcTrackFinder(Bool_t isCOff, Int_t typeClFn=0);
  HMdcTrackFinder(Text_t *name, Text_t *title, 
                  Bool_t isCOff, Int_t typeClFn=0);
  HMdcTrackFinder();
  HMdcTrackFinder(Text_t *name, Text_t *title);
  ~HMdcTrackFinder(void);
  void setPersistency(Bool_t perCl);
  Bool_t init(void);
  Bool_t reinit(void);
  void setParContainers();
  void setNLayers(Int_t *lst);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void setLevel(Int_t l4s1, Int_t l5s1, Int_t l4s2, Int_t l5s2);
  void setLevelSeg1(Int_t l4, Int_t l5);
  void setLevelSeg2(Int_t l4, Int_t l5);
  void printStatus(void);
  
  ClassDef(HMdcTrackFinder,0) //Digitizes MDC data.
};

#endif









