#ifndef HMDCDITIZER_H
#define HMDCDITIZER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "TRandom.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmdcdigitgeompar.h"
#include "hmdcdigitpar.h"

class HIterator;

class HMdcDigitizer : public HReconstructor {
private:
  HLocation loc;                        // Location for new object
  HCategory* fGeantRawCat;              // Pointer to sim data category
  HCategory* fCalCat;                   // Pointer to cal data category
  HMdcDigitGeomPar* fDigitGeomPar;      // Digitisation "geom." parameters
  HMdcDigitPar* fDigitPar;              // Digitisation "phys." parameters
  HIterator* iter;                      //! Iterator over GeantRaw category
  TRandom fRnd;                         // Random generator

  Float_t spaceResol;                   // Space resolution for MDC
  Float_t dTime[15];                    // Drift Time

public:
  HMdcDigitizer(void);
  HMdcDigitizer(Text_t *name,Text_t *title);
  ~HMdcDigitizer(void);
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  TRandom *ran;
  Float_t select(float* item, int count); // Puts the drift time values into increasing order
public:
  ClassDef(HMdcDigitizer,0) //Digitizer of MDC data.
};

#endif









