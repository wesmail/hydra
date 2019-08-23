#ifndef HRTFITTER_H
#define HRTFITTER_H

#include "hrtfunctional.h"
#include "hrtmatrix.h"
#include "hludecomposer.h"
#include <TH1.h>

class HRtFitter {
public:
  enum EFitResult {kConverged=0,kMaxIters=2,kOutOfBounds=1,kUnknown=3};
  HRtFitter(void);
  ~HRtFitter(void);
  void setInitialValue(HRtVector &p0) { fP0 = p0; }
  enum EFitResult fit(HRtVector &m, HRtMatrix &W,
      HRtVector &out,Int_t pol,HRtMatrix &cov,Float_t &chi2);
  Bool_t init(void);
  Bool_t reinit(void) { return fL.reinit(); }
private:
  void calcFitMatrix(void);
  HRtFunctional fL;
  HRtVector fP0;
  HRtVector fOldEstimate;
  HRtMatrix fA,fAt,fAtWA,fM;
  HLuDecomposer fLu;
  Int_t fMaxIters; //FIXME: read from rtdb
  TH1S *fControl;
};


#endif
