#include "hrtfunctional.h"
#include "hsavitzkygolay.h"

HRtFunctional::HRtFunctional(void) : fSavGol(5) {
  HSavitzkyGolay savgol;
  fNLeft = 2;  //FIXME: These parameters should be read from rtdb
  fNRight = 2;
  fM = 2;
  savgol.calcCoefficients(fSavGol,2,2,1,2);
}

HRtFunctional::~HRtFunctional(void) {
}

Bool_t HRtFunctional::setPoint(const HRtVector &p) {
  //Sets the evaluation point. Returns kFALSE if the functional is not
  //known at point p. (p is out of the parametrized region)
  return kTRUE;
}

Float_t HRtFunctional::partialDerivative(Int_t pi,Int_t pj) {
  // Calculates the partial derivative of the functional with respect to
  //the pj component of p and evaluates it on p.
  Float_t sum = 0;
  Int_t temp = fPoint[pj];

  //FIXME: implement case when we are in the edges
  fPoint[pj]=temp-fNLeft;
  for (int n=0;n<fNLeft+fNRight+1;n++,fPoint[pj]++) {
    HRtMeasurement &m=fData.getValue(fPoint);
    sum += fSavGol(n) * m[pi];
  }
  return sum / fStep[pi];
}

void HRtFunctional::calcDerivatives(HRtMatrix &result) {
  // Calculates the partial derivatives of the functional 
  // at point p and stores the result in the matrix "result". 
  // p is snaped to the closest point in the grid.
  for (Int_t i=0;i<result.getRows();i++)
    for (Int_t j=0;j<result.getCols();j++)
      result(i,j) = partialDerivative(i,j);
}

void HRtFunctional::calcValue(HRtVector &result) {
  // Calculates the value of the functional at p and stores the result in
  // "result". p is snaped to the closest point in the grid.
}


ClassImp(HRtFunctional)
