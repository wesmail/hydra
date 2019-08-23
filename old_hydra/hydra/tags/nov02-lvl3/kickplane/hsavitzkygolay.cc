//*-- Author : M. Sanchez (12/06/2001)
//*-- Modified : M. Sanchez (20/06/2001)
using namespace std;
#include "hsavitzkygolay.h"
#include "hludecomposer.h"
#include <assert.h>
#include <TMath.h>
#include <iostream> 
#include <iomanip>

////////////////////////////////////////////////////////////////
//HSavitzkyGolay
//
//  Simple class to calculate the Savitzky-Golay coefficients
//
//  Basically the Savitzky-Golay filter consists on doing a fit
// of a set of data points to a polinomial of a certain degree and
// then use this polinomial to interpolate the data, calculate
// derivatives...
//
//  The Savitzky-Golay coefficients allow to make this procedure fast
// by converting the process of fitting and evaluating a polinomial at
// some point into a linear convolution.
//
//  For more documentation on the Savitzky-Golay filter and its
// applications see Numerical Recipes in C. 14.8
/////////////////////////////////////////////////////////////////

HSavitzkyGolay::HSavitzkyGolay(void) {
}

HSavitzkyGolay::~HSavitzkyGolay(void) {
}

void HSavitzkyGolay::calcCoefficients(HRtVector &out,
				      Int_t nl,Int_t nr,Int_t ld,Int_t m) {
  // Stores in vector out the Savitzky-Golay coefficients. 
  // Input
  //   nl: number of points to the left in the fit
  //   nr: number of points to the right in the fit
  //   m: degree of the polinomial
  //   ld: order of the coefficients. 0 for interpoletion, 
  //                                  1 for differentiation...
  assert( !(nl<0 || nr<0 || ld>m || nl+nr<m) );
  Int_t ipj,j,k,mm;
  Float_t fac,sum;
  Int_t np = nl + nr +1;
  HRtMatrix a(m+1,m+1);
  HRtVector b(m+1);
  HLuDecomposer lu;
  out.resizeTo(np);
  

  for (Int_t i=0; i<=m; i++) {
    for (j=0; j<=m; j++) {
      ipj = i+j;
      sum = 0.;
      for (k=-nl;k<=nr;k++) sum += TMath::Power(k,ipj);
      a(i,j) = sum;
    }
  }
 
  lu.decompose(a);
  b.zero();
  b(ld) = 1.;
  lu.backSubstitute(b);
  for (k=-nl; k<=nr; k++) {
    sum = b(0);
    fac = 1.;
    for (mm=0;mm<m;mm++) sum += b(mm+1)*(fac *= k);
    out(k+nl) = sum;
  }
}

ClassImp(HSavitzkyGolay)
