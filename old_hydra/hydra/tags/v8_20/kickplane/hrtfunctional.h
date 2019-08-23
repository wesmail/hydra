#ifndef HRTFUNCTIONAL_H
#define HRTFUNCTIONAL_H

#include "TObject.h"
#include "hrtmatrix.h"
#include "hrttypes.h"
#include "hrtmeasurement.h"
#include "hrtsavgolmanager.h"
#include "hrtsavgolpar.h"
#include "hrtdata.h"
#include "hrtgrid.h"
#include <math.h>


class HRtFunctional : public TObject {
protected:
  HRtData *fData;    //!Reference trajectories
  HRtSavGolManager *fSavGol;  //! Savitzky-Golay coefficients
  HRtSavGolPar *fPolinomPar;//Parameters for savitzky-golay filter
  UInt_t *fPoint;  //! Current point
  HRtGrid *fCurrentGrid; //! for polarity
  HRtMeasurement *fPointValue; //!
  Float_t partialDerivative(Int_t pi,Int_t pj);
  enum ESide {kLeft, kRight} fSectorSide; //! Current sector side
  Float_t fDeg2Rad; //!
public:
  HRtFunctional(void);
  ~HRtFunctional(void);
  Bool_t setPoint(HRtVector &p, Int_t pol);
  UInt_t getPointId(void) { return fCurrentGrid->getCurrentAddress(); }
  void calcDerivatives(HRtMatrix &result);
  void calcValue(HRtVector &result);
  Float_t guessValue(Int_t coord) { return fCurrentGrid->getMidValue(coord); }
  inline Bool_t withinResolution(HRtVector &v);
  Bool_t init(void);
  Bool_t reinit(void);
  
  ClassDef(HRtFunctional,1)
};

inline Bool_t HRtFunctional::withinResolution(HRtVector &v) {
  Bool_t r = kFALSE;
  if ( (fabs(v(0)) < fCurrentGrid->getBinSize(0)/2.) &&
       (fabs(v(1)) < fCurrentGrid->getBinSize(1)/2.) &&
       (fabs(v(2)) < fCurrentGrid->getBinSize(2)/2.) &&
       (fabs(v(3)) < fCurrentGrid->getBinSize(3)/2.) &&
       (fabs(v(4)) < fCurrentGrid->getBinSize(4)/2.) )
    r = kTRUE;
  return r;
}

#endif
