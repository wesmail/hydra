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
class HMdcGeantCell;

class HMdcDigitizer : public HReconstructor {
private:
  HCategory* fGeantMdcCat;              //! MDC HGeant input data 
  HLocation loc;                        // Location for new object
  HCategory* fGeantCellCat;             // Pointer to sim data category
  HCategory* fCalCat;                   // Pointer to cal data category
  HMdcGeantCell* hit;                   // Pointer to HMdcGeantCell hit
  HMdcGeantCell* hitNeigh;              // Pointer to neighbour
  HMdcLayerGeomPar* fDigitGeomPar;      // Digitisation "geom" parameters
  HMdcDigitPar* fDigitPar;              // Digitisation "phys" parameters
  HIterator* iterin;                    // Iterator over input category
  HIterator* itercell;                  // Iterator over cell category
  Int_t fEventId;                       // Number of current event
  Float_t yDist;                        // Distance to the sence wire
  Float_t pi;                           // Il y quatre pis a une vache
  Float_t fUnit;                        // Length unit conversion for HGeant 

public:
  HMdcDigitizer(void);
  HMdcDigitizer(Text_t* name,Text_t* title, Float_t unit=10.);
  ~HMdcDigitizer(void);
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  void setParContainers();
  Bool_t transform(Float_t,Float_t,Float_t,Float_t,Float_t,Int_t);
  void storeCell(Float_t,Float_t,Int_t);
  void select(Float_t*,Int_t*,Int_t);

  ClassDef(HMdcDigitizer,0) // Digitizer of MDC data
};

#endif









