//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/
using namespace std;
#include "hindextransform.h"
#include <iostream> 
#include <iomanip>

//-------------------------------------------------------------------------
//
// File name:       hindextransform.cc
// Subject:         Index transformations between multi and one dimensional
//                  arrays.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HIndexTransform.
// This class contains a set of static functions which allow to perform
// index transformations between multi and one dimensional arrays.
//
//-------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////
// This class contains a set of static functions which allow to perform
// index transformations between multi and one dimensional arrays.
///////////////////////////////////////////////////////////////////////
ClassImp(HIndexTransform)


//---------------------------------------------------------------------
//******  Int_t HIndexTransform::mult2lin(Int_t n, Int_t *maxind, Int_t *idx)
//                        
// This function makes transformation of a set of indexes of 
// a multidimensional array to the index of a corresponding one dimensional 
// array.
//
// n - number of dimensions of a multi dimensional array
// maxind - array of maximum sizes in each dimension
// idx - array of indexes to be transformed
//
//---------------------------------------------------------------------

Int_t HIndexTransform::mult2lin(Int_t n, Int_t *maxind, Int_t *idx) {
//                        
// This function makes transformation of a set of indexes of 
// a multidimensional array to the index of a corresponding one dimensional 
// array.
//
// n - number of dimensions of a multi dimensional array
// maxind - array of maximum sizes in each dimension
// idx - array of indexes to be transformed
//

  if(n <= 0) return -1;
  if(!maxind) return -1;
  if(!idx) return -1;

  if(n == 0) return idx[0];

  Int_t i,j,r;

  for(i=0; i<n; i++) if(idx[i] >= maxind[i]) return -1;

  Int_t *koef = new Int_t[n];

  for(i=0; i<n; i++) {

    r = 1;
    for(j=i+1; j<n; j++) r *= maxind[j];

    koef[i] = r;
  }

  Int_t retval = 0;

  for(i=0; i<n; i++) retval += koef[i]*idx[i];

  delete [] koef;

  return retval;

}


//---------------------------------------------------------------------
//******  Bool_t HIndexTransform::lin2mult(Int_t n, Int_t *maxind, Int_t idx,
//                                         Int_t* retar)
//                        
// This function makes transformation of an index of a one dimensional array 
// to the set of indexes of a corresponding multidimensional array.
//
// n - number of dimensions of a multi dimensional array
// maxind - array of maximum sizes in each dimension
// idx - index in the one dimensional array to be transformed
// retar - pointer to the array of integers where to store indexes
//
//---------------------------------------------------------------------

Bool_t HIndexTransform::lin2mult(Int_t n,Int_t* maxind,Int_t idx,
				 Int_t* retar) {
//                        
// This function makes transformation of an index of a one dimensional array 
// to the set of indexes of a corresponding multidimensional array.
//
// n - number of dimensions of a multi dimensional array
// maxind - array of maximum sizes in each dimension
// idx - index in the one dimensional array to be transformed
// retar - pointer to the array of integers where to store indexes
//

  if(n <= 0) return kFALSE;
  if(!maxind) return kFALSE;
  if(idx < 0) return kFALSE;
  if(!retar) return kFALSE;

  if(n == 0) return kFALSE;

  Int_t i,j,r;

  r = 1;
  for(i=0; i<n; i++) r *= maxind[i];
  if(idx >= r) return kFALSE;

  Int_t *koef = new Int_t[n];

  for(i=0; i<n; i++) {

    r = 1;
    for(j=i+1; j<n; j++) r *= maxind[j];

    koef[i] = r;
  }

  Int_t *temp = new Int_t[n];

  temp[0] = idx;

  for(i=1; i<n; i++) temp[i] = temp[i-1] % koef[i-1];

  for(i=0; i<n-1; i++) retar[i] = temp[i] / koef[i];

  retar[n-1] = temp[n-2] % koef[n-2];

  delete [] koef;
  koef = 0;
  delete [] temp;
  temp = 0;

  for(i=0; i<n; i++) { if(retar[i] >= maxind[i]) return kFALSE; }

  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HIndexTransform::print(Int_t n, Int_t *maxind, Int_t* idx)
//                        
// This function prints the result of lin2mult function.
//
//---------------------------------------------------------------------

void HIndexTransform::print(Int_t n, Int_t *maxind, Int_t *idx) {
//                        
// This function prints the result of lin2mult function.
//

 Int_t retval = mult2lin(n,maxind,idx);

 cout << retval << endl;

}


//---------------------------------------------------------------------
//******  void HIndexTransform::print(Int_t n, Int_t *maxind, Int_t idx)
//                        
// This function prints the result of mult2lin function.
//
//---------------------------------------------------------------------

void HIndexTransform::print(Int_t n, Int_t *maxind, Int_t idx) {
//                        
// This function prints the result of mult2lin function.
//

  Int_t *retar = new Int_t[n];
  if(!lin2mult(n,maxind,idx,retar)) cout << "error" << endl;

  else {

    for(Int_t i=0; i<n; i++) cout << retar[i] << "   ";
    cout << endl;

  }

  if(retar) delete retar;

}







