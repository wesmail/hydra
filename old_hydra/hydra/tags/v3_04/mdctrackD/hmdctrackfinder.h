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
  

public:
  HMdcTrackFinder(Int_t mbv=12);
  HMdcTrackFinder(Text_t *name,Text_t *title, Int_t mbv=12);
  ~HMdcTrackFinder(void) {};
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
public:
  ClassDef(HMdcTrackFinder,0) //Digitizes MDC data.
};

#endif









