#ifndef HMDCTRACKFINDER_H
#define HMDCTRACKFINDER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "hmdctimecut.h"
#include "hspecgeompar.h" 

class HIterator;

class HMdcTrackFinder : public HReconstructor {
private:
  HLocation loc;                        // location for new object
  HCategory* fCalCat;                   // pointer to CAL1 data category
  HCategory* fHitCat;                   // pointer to HIT data category
  Bool_t isGeant;                       // =kTRUE for GEANT data
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb;
  Int_t minbin;
  
  HMdcTimeCut* fcut;                   // Cut 
  
  Int_t level4;                       // Level of 4-layers finding (<= level4 Mdc hits)
  Int_t level5;                       // Level of 5-layers finding 
  
  
  // For filling HMdcXY !
  HMdcGeomPar* fGeomPar;
  HGeomVector  targetM[6]; 
  void fillHitCont(void);
  Int_t calcCross( const HGeomVector *p1, const HGeomVector *p2,
                   Int_t mod, HGeomVector *cross);

public:
  HMdcTrackFinder(Int_t mbv=12);
  HMdcTrackFinder(Text_t *name,Text_t *title, Int_t mbv=12);
  ~HMdcTrackFinder(void) {};
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void setLevel(Int_t l4, Int_t l5) {level4=l4; level5=l5;}
public:
  ClassDef(HMdcTrackFinder,0) //Digitizes MDC data.
};

#endif









