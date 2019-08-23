#ifndef HRTFUNCTIONAL_H
#define HRTFUNCTIONAL_H

#include <TObject.h>
#include "hrtmatrix.h"
#include "hrttypes.h"

class HRtGrid  {
 public:
  HRtGrid(void) {}
  ~HRtGrid(void) {} 
  //FIXME: Implement
  HRtMeasurement &getValue(Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t i5) {
     return fData[0];
  }
  HRtMeasurement &getValue(Int_t point[]) {
     return fData[0];
  }
 protected:
  HRtMeasurement *fData; //[fEntries]
  Int_t fEntries;
  Int_t fSize[5]; //Size of the array in the 5 dimensions
  Int_t fRange;   //Dimension of measurements space
};

class HRtFunctional : public TObject {
protected:
  HRtGrid fData;    //Reference trajectories
  HRtVector fSavGol;  //Savitzky-Golay coefficients
  Int_t fNLeft,fNRight,fM; //Parameters for savitzky-golay filter
  Int_t fPoint[5];  //! Current point
  Float_t fStep[5]; //Grid steps
  Float_t partialDerivative(Int_t pi,Int_t pj);
public:
  HRtFunctional(void);
  ~HRtFunctional(void);
  Bool_t setPoint(const HRtVector &p);
  void calcDerivatives(HRtMatrix &result);
  void calcValue(HRtVector &result);
  Bool_t withinResolution(HRtVector &v);
  
  ClassDef(HRtFunctional,1)
};

inline Bool_t HRtFunctional::withinResolution(HRtVector &v) {
  Bool_t r = kFALSE;
  if ( (v(0) < fStep[0]) &&
       (v(1) < fStep[1]) &&
       (v(2) < fStep[2]) &&
       (v(3) < fStep[3]) &&
       (v(4) < fStep[4]) )
    r = kTRUE;
  return r;
}

#endif
