#ifndef HTOFINOCALIBRATER_H
#define HTOFINOCALIBRATER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"
#include "htofinocalpar.h"

class HIterator;
class HCategory;
class HTofinoCalPar;

class HTofinoCalibrater : public HReconstructor {

private:
  HLocation fLoc;             //! Location for new object
  HCategory* fCalCat;       //! Pointer to cal data category
  HCategory* fRawCat;         //! Pointer to raw data category
  HIterator* iter;            //! Iterator over Raw category
  HTofinoCalPar *pTofinoCalPar; //! Calibration parameters 
  HCategory *fStartHitCat; //! Category with start hits
public:
  HTofinoCalibrater(void);
  HTofinoCalibrater(Text_t* name,Text_t* title);
  ~HTofinoCalibrater(void) {;}
  void initParContainer();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

public:
  ClassDef(HTofinoCalibrater,1) // Calibrater for TOFINO raw data
};

#endif
