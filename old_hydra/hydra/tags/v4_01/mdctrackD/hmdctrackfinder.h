#ifndef HMDCTRACKFINDER_H
#define HMDCTRACKFINDER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdclookuptb.h"

class HIterator;

class HMdcTrackFinder : public HReconstructor {
private:
  HLocation loc;                        // location for new object
  HCategory* fCalCat;                   // pointer to cal data category
  HIterator* iter;
  HMdcLookUpTb* fLookUpTb;
  Int_t minbin;
  
  Float_t cutLeft;                    // Cut: cutLeft< Time1-Time2 <cutRight
  Float_t cutRight;                   // cutLeft=cutRight=0. - no cut
  Int_t level4;                       // Level of 4-layers finding (<= level4 Mdc hits)
  Int_t level5;                       // Level of 5-layers finding 

public:
  HMdcTrackFinder(Int_t mbv=12);
  HMdcTrackFinder(Text_t *name,Text_t *title, Int_t mbv=12);
  ~HMdcTrackFinder(void) {};
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void setCut(Float_t cutL, Float_t cutR) {cutLeft=cutL; cutRight=cutR;}
  void setLevel(Int_t l4, Int_t l5) {level4=l4; level5=l5;}
public:
  ClassDef(HMdcTrackFinder,0) //Digitizes MDC data.
};

#endif









