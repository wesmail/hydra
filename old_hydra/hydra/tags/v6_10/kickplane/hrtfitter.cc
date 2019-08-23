#include "hrtfitter.h"
#include "hrttypes.h"

HRtFitter::HRtFitter(void) : fP0(5),
			     fA(8,5),fAt(fA),
			     fAtWA(5,5),fM(5,8) {
  fMaxIters = 100;
  fP0.zero();
  fControl = new TH1S("rtfit","Eff. of fitting procedure",6,0,3);
  // The bins in this histogram mean:
  //   0 ---> Fit was succesful
  //   1 ---> Fit hit one edge of the parameter space
  //   2 ---> Fit did not converge in maxIters
}

HRtFitter::~HRtFitter(void) {
}

Bool_t HRtFitter::fit(HRtVector &m, HRtMatrix &W, 
		      HRtVector &out, HRtMatrix &cov, Float_t &chi2) {
  //Input:
  //     m  ---> the measurements in the detection planes
  //     W  ---> Inverse Cov. matrix for the measurements
  //Output:
  //     out  -> Parameters of the fitted track
  //     cov  -> Covariance matrix of the fit
  //     chi2 -> Chi2 of the fit
  Bool_t converged=kFALSE;
  
  Int_t iters = 0;
  HRtVector f(2*kRtDetectionPlanes),delta(2*kRtDetectionPlanes);
  HRtVector res(2*kRtDetectionPlanes);

  out = fP0; 
  
  while (!converged && iters<fMaxIters) {
    iters++;
    if (!fL.setPoint(out)) {
      //Oops! We have got out of range. For the moment we will just
      //quit the fitting and fill some statistics
      fControl->Fill(1);
      return kFALSE;
    }
      
    fL.calcValue(f);
    fL.calcDerivatives(fA); //Fills fA and fAt, since fAt is a view of fA
    
    // Calculate fit matrix M
    fM.multiply(fAt,W);
    fAtWA.multiply(fM,fA);
    
    fLu.decompose(fAtWA);
    fLu.backSubstitute(fM); //Performs invert(cov)*fM and stores the 
                            //result in fM

    f-=m;
    delta.multiply(fM,f);
    out+=delta;
    
    //If the initial value is not good enough. We may have to introduce
    //the following commented code in order to preserve the direction of
    //movement but not its magnitude. Improves stability!
    // out /= 2.;

    if (fL.withinResolution(delta)) converged = kTRUE;
  } 

  if (converged) {
    // Fill control histogram.
    fControl->Fill(0);

    //Calculate covariance matrix.
    cov.identity();
    fLu.backSubstitute(cov); //Stores inverse of fAtWA in cov
    
    //Calculate chi2
    res.multiply(fA,delta); //res contains the residuals
    res += f;
    chi2 = W.convolute(res);
  } else {
    fControl->Fill(2);
  }
  return converged;
}
