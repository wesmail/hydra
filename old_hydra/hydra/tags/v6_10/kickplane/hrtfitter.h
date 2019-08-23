#ifndef HRTFITTER_H
#define HRTFITTER_H

#include "hrtfunctional.h"
#include "hrtmatrix.h"
#include "hludecomposer.h"
#include <TH1.h>

class HRtFitter {
public:
  HRtFitter(void);
  ~HRtFitter(void);
  void setInitialValue(HRtVector &p0) { fP0 = p0; }
  Bool_t fit(HRtVector &m, HRtMatrix &W,
	     HRtVector &out,HRtMatrix &cov,Float_t &chi2);
private:
  void calcFitMatrix(void);
  HRtFunctional fL;
  HRtVector fP0;
  HRtMatrix fA,fAt,fAtWA,fM;
  HLuDecomposer fLu;
  Float_t fMaxIters; //FIXME: read from rtdb
  TH1S *fControl;
};


#endif
