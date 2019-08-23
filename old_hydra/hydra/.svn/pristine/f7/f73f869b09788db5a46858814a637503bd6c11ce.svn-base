#ifndef HMDCDITIZER_H
#define HMDCDITIZER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "TRandom.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HMdcLayerGeomPar;
class HMdcDigitPar;

class HMdcDigitizer : public HReconstructor {
private:
  HLocation loc;                        // Location for new object
  HCategory* fGeantCellCat;             // Pointer to sim data category
  HCategory* fCalCat;                   // Pointer to cal data category
  HMdcLayerGeomPar* fDigitGeomPar;      // Digitisation "geom." parameters
  HMdcDigitPar* fDigitPar;              // Digitisation "phys." parameters
  HIterator* iter;                      // Iterator over GeantCell category
public:
  HMdcDigitizer(void);
  HMdcDigitizer(Text_t* name,Text_t* title);
  ~HMdcDigitizer(void);
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void select(Float_t*,Int_t*,Int_t);
public:
  ClassDef(HMdcDigitizer,0) //Digitizer of MDC data.
};

#endif









