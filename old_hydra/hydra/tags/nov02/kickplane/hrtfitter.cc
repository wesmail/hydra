#include "hrtfitter.h"
#include "hrttypes.h"

#undef RTFIT_DEBUG

HRtFitter::HRtFitter(void) : fP0(kRtParameterDim),
 			     fOldEstimate(kRtParameterDim), 
			     fA(kRtMeasurementDim,kRtParameterDim),fAt(fA),
			     fAtWA(kRtParameterDim,kRtParameterDim),
			     fM(kRtParameterDim,kRtMeasurementDim) {
  fMaxIters = 100;
  fP0.zero();
  fControl = new TH1S("rtfit","Eff. of fitting procedure",6,0,3);
  fAt.transposeSelf();
  // The bins in this histogram mean:
  //   0 ---> Fit was succesful
  //   1 ---> Fit hit one edge of the parameter space
  //   2 ---> Fit did not converge in maxIters
}

HRtFitter::~HRtFitter(void) {
}

Bool_t HRtFitter::init(void) {
  return fL.init();
}

enum HRtFitter::EFitResult HRtFitter::fit(HRtVector &m, 
    HRtMatrix &W, 
    HRtVector &out, Int_t pol, HRtMatrix &cov, Float_t &chi2) {
	using namespace std;
  //Input:
  //     m  ---> the measurements in the detection planes
  //     W  ---> Inverse Cov. matrix for the measurements
  //     out --> Initial fit value
  //     pol --> Initial guess for polarity     
  //Output:
  //     out  -> Parameters of the fitted track
  //     cov  -> Covariance matrix of the fit
  //     chi2 -> Chi2 of the fit
  enum EFitResult fitResult = HRtFitter::kUnknown;
  Bool_t converged = kFALSE;
  UInt_t pointId[2] = {kMaxInt,kMaxInt}; //Point id history
  UInt_t curPointId;

  Int_t iters = 0;
  HRtVector f(kRtMeasurementDim),delta(kRtParameterDim);
  HRtVector res(kRtMeasurementDim);

  #ifdef RTFIT_DEBUG 
  std::cout << "Fitting starts.----------------------------------" << std::endl; 
  #endif
  while (!converged && iters<fMaxIters) {
    #ifdef RTFIT_DEBUG 
      cout << "fitting step " << iters << "<--- "; 
      cout << "( " << 1/out(0) << ") ";
      out.print();
    #endif
    iters++;
    if (!fL.setPoint(out,pol)) {
      if (iters==1) {
        //We have fallen out of range, let's give it a second chance
        out(0)=fL.guessValue(0); //make a guess for momentum
	iters++;
	continue;
      } else {
	//Oops! We have got out of range. For the moment we will just
	//quit the fitting and fill some statistics
	fControl->Fill(1);
	fitResult = kOutOfBounds;
	converged = kFALSE;
	iters = fMaxIters;
      }
    } else {
      fL.calcValue(f);
      fL.calcDerivatives(fA); //Fills fA and fAt, fAt is a view of fA
      #ifdef RTFIT_DEBUG 
        cout << "grid point ";
        cout << "( " << 1/out(0) << ") ";
        out.print();
        fA.print("A");
      #endif

      // Calculate fit matrix M
      fM.multiply(fAt,W);
      #ifdef RTFIT_DEBUG 
        fM.print("AtW");
      #endif

      fAtWA.multiply(fM,fA);
      fAtWA(1,1) = fAtWA(2,2) = 1.;
      #ifdef RTFIT_DEBUG 
        fAtWA.print("AtWA");
      #endif

      fLu.decompose(fAtWA);
      fLu.backSubstitute(fM);//Performs invert(cov)*fM and stores the 
      //result in fM
      #ifdef RTFIT_DEBUG 
        fM.print("inv(AtWA) AtW");

        cout << "f: "; f.print();
        cout << "m: "; m.print();
      #endif
      f-=m;
      delta.multiply(fM,f);
      out-=delta;   // p = p0 - M * (x0 - x)
      #ifdef RTFIT_DEBUG 
        cout << "DELTA: "; delta.print();
      #endif

      //If the initial value is not good enough. We may have to 
      //introduce the following commented code in order to preserve 
      //the direction of movement but not its magnitude. Improves 
      //stability!
      // out /= 2.;

      if (fL.withinResolution(delta)) converged = kTRUE;
      else {
	curPointId = fL.getPointId();
	if (curPointId == pointId[1]) { //Cycle detected!!!
  	  #ifdef RTFIT_DEBUG
	    cout << "Cycle detected!!!" << endl;
          #endif
	  converged = kTRUE;
	} else {
	  pointId[1] = pointId[0];
	  pointId[0] = curPointId;
	}
      }

      if (out(0)<0) { //Fit changes polarity
	out(0) *= -1;
	pol *= -1;
      }
    }
  } 

  if (converged) {
    #ifdef RTFIT_DEBUG 
      cout << "Fitting has converged!!!" << endl;
      cout << "( " << 1/out(0) << ") ";
      out.print();
    #endif
    // Fill control histogram.
    fControl->Fill(0);
    fitResult = kConverged;

    //Calculate covariance matrix.
    cov.identity();
    fLu.backSubstitute(cov); //Stores inverse of fAtWA in cov
    
    //Calculate chi2
    res.multiply(fA,delta); //res contains the residuals
    res += f;
    chi2 = W.convolute(res);
  } else {
    fControl->Fill(2);
    if (fitResult==kUnknown) fitResult = kMaxIters;
  }
  #ifdef RTFIT_DEBUG 
    cout << "Fitting ends" << endl;
  #endif
  return fitResult;
}
