//*-- Author : M.Sanchez (11/06/2001)
//*-- Modified :
using namespace std;
#include "hludecomposer.h"
#include <assert.h>
#include <iostream> 
#include <iomanip>
#include <TMath.h>

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////
// HLuDecomposer
// 
//  Implements LU decomposition of a HRtMatrix and backsubsitution
//to invert the matrix or solve linear problems
//
//  Both functions alter the data passed as arguments
//////////////////////////////////////////////////////////////

HLuDecomposer::HLuDecomposer(void) {
  fPermutationParity = 0;
  fA = 0;            //Processed matrix
}

HLuDecomposer::~HLuDecomposer(void) {
  fA = 0;
}
  
void HLuDecomposer::decompose(HRtMatrix &a) {
  // Decomposes matrix a into a L and U matrixes. Decomposition is done
  //in place, i.e. "a" is overwritten.
  Int_t n = a.getCols();
  Int_t i,imax=-1,j,k;
  Float_t big,dum,sum,temp;
  assert(a.getCols() == a.getRows());
  HRtVector vv(n);
  fA = &a;

  fPermutations.Set(n);
  fPermutationParity = 1;
  for (i=0;i<n;i++) {
    big = 0.;
    for (j=0;j<n;j++) 
      if ( (temp = TMath::Abs(a(i,j))) > big) big = temp;
    if (big == 0.) {
      Error("decompose","Input matrix is singular");
      return;
    }
    vv(i) = 1./big;
  }
  for (j=0;j<n;j++) {
    for (i=0;i<j;i++) {
      sum = a(i,j);
      for (k=0;k<i;k++) sum -= a(i,k)*a(k,j);
      a(i,j) = sum;
    }
    big = 0.;
    for (i=j;i<n;i++) {
      sum = a(i,j);
      for (k=0;k<j;k++) sum -= a(i,k)*a(k,j);
      a(i,j) = sum;
      if ( (dum=vv(i)*TMath::Abs(sum)) >= big) {
	big = dum;
	imax = i;
      }
    }
    if (j != imax) {
      for (k=0;k<n;k++) {
	dum       = a(imax,k);
	a(imax,k) = a(j,k);
	a(j,k)    = dum;
      }
      fPermutationParity *= -1;
      vv(imax) = vv(j);
    }

    fPermutations[j] = imax;
    if (a(j,j) == 0.) {
      a.print();
      Error("decompose","Matrix is singular");
      return;
    }
    if (j != n) {
      dum = 1./a(j,j);
      for (i=j+1;i<n;i++) a(i,j) *= dum;
    }
  }
}

void HLuDecomposer::backSubstitute(HRtVector &b) {
  // Stores the solution of the linear ecuation Ax = b into the vector
  //b. The system matrix A had to previously be specified using
  //decompose()
  assert(fA);
  Int_t i,ii=-1,ip,j;
  Float_t sum;
  HRtMatrix &a = *fA;
  Int_t n = a.getCols();
  
  for (i=0; i<n; i++) {
    ip = fPermutations[i];
    sum = b(ip);
    b(ip) = b(i);
    if ( !(ii<0) )
      for (j=ii;j<=i-1;j++) sum -= a(i,j)*b(j);
    else if (sum) ii = i;
    b(i) = sum;
  }
 
  for (i=n-1;i>=0;i--) {
    sum = b(i);
    for (j=i+1; j<n; j++) sum -= a(i,j)*b(j);
    b(i) = sum / a(i,i);
  }
}

void HLuDecomposer::backSubstitute(HRtMatrix &b) {
  // Stores in b the result of inverse(lu)*b. This is faster than
  //doing the actual inverse matrix calculation and then multiplying.
  assert(fA);
  Int_t i,ii=-1,ip,j;
  Float_t sum;
  HRtMatrix &a = *fA;
  Int_t n = a.getCols();
  
  for (int col=0;col<b.getCols();col++) {
    for (i=0; i<n; i++) {
      ip = fPermutations[i];
      sum = b(ip,col);
      b(ip,col) = b(i,col);
      if ( !(ii<0) )
	for (j=ii;j<=i-1;j++) sum -= a(i,j)*b(j,col);
      else if (sum) ii = i;
      b(i,col) = sum;
    }
    
    for (i=n-1;i>=0;i--) {
      sum = b(i,col);
      for (j=i+1; j<n; j++) sum -= a(i,j)*b(j,col);
      b(i,col) = sum / a(i,i);
    }
  }
}

ClassImp(HLuDecomposer)
