#include "hlocation.h"
#include <iostream.h>

//*-- Author : Manuel Sanchez 
//*-- Modified : 27/05/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

////////////////////////////////////
//HLocation
//
//  ATENCION! Se ha eliminado el index check en operator[]
// 
//  A location is a set of indexes defining the location of a particular 
//data object within the HEvent structure; so within a loop is recommended to 
//directly use one HLocation instead of a set of indexes (i,j,k,l...) (and
//better an HIterator instead of HLocation).
//
//  An HEvent holds the data corresponding to an event, those data are arranged
// in categories (mdc raw data, mdc cal data, rich rings,...) which are objects
// instantianting one HCategory, to access one of these categories the user
// can call HEvent::getCategory(). Within the categories, each data object has
// a location (given by an object instantiating HLocation) which identifies 
// that object in the category (in some sense, the location is a generalized
// index).
//
////////////////////////////////

ClassImp(HLocation) 

HLocation::HLocation(void) {
  //Default constructor
  fOffset=0;
}

HLocation::HLocation(HLocation &aLoc) {
  //Copy constructor
  fIndexes.Copy(aLoc.fIndexes);
}

HLocation::~HLocation(void) {
  //Destructor
   fIndexes.Reset(); 
}

void HLocation::set(Int_t nIndex,...)
{
  //Sets all the indexes in the HLocation at once:
  //
  //The first argument is the number of indexes to be used and the next 
  //arguments are the actual indexes (as many as nIndex)
   va_list ap;
   Int_t i,n=nIndex;
   va_start (ap,nIndex);
   fIndexes.Set(nIndex);
   for (i=0;i<n;i++) {
      fIndexes[i]=va_arg(ap,Int_t);
   }
   va_end(ap);
}

void HLocation::setNIndex(Int_t nIdx) {
  //Sets the number of indexes to be used and resets all indexes to 0
  fIndexes.Set(nIdx);
}

inline Int_t HLocation::getIndex(Int_t aIdx) {
  //Returns the value of the index aIdx
  return(fIndexes[aIdx]);
}

//inline Int_t &HLocation::operator[](Int_t aIdx) {
  //Returns the index aIdx.
//  return fIndexes[aIdx];
//}

inline void HLocation::setIndex(Int_t aIdx, Int_t aValue) {
  //Sets the value of the index aIdx
  fIndexes[aIdx]=aValue;
}

inline Int_t HLocation::getLinearIndex(TArrayI *sizes) {
  //Given a sizes vector, this method calculates the linear address corresponding to a particular
  //location.
  Int_t r=1,i;
  
  r=fIndexes.At(0);
  for (i=1;i<fIndexes.fN;i++) 
    r=r*sizes->At(i)+fIndexes.At(i);
  for(;i<sizes->fN;i++) {
    r*=sizes->fArray[i];
  }
  return (r);
}

inline Int_t HLocation::getNIndex(void) {
  //Returns the number of indexes held by this object.
   return fIndexes.fN;
}

void HLocation::incIndex(Int_t nIndex) {
  //Increments the index nIndex and sets all the following indexes to 0
  Int_t i;
  fIndexes[nIndex]++;
  for (i=nIndex+1;i<fIndexes.fN;i++) fIndexes[i]=0;
}

inline void HLocation::setOffset(Int_t aIdx) {
  //Sets the offset (the last index) to the value aIdx
  fIndexes[fIndexes.fN-1]=aIdx;
}

inline Int_t HLocation::getOffset(void) {
  //Returns the value of the offset (the last index)
  return fIndexes[fIndexes.fN-1];
}

inline Int_t HLocation::operator++(void) {
  //Increments by one the offset.
  return fIndexes[fIndexes.fN-1]++;
}

inline void HLocation::operator+=(Int_t shift) {
  //Increments the offset by "shift"
  fIndexes[fIndexes.fN-1]+=shift;
}

inline void HLocation::operator--(void) {
  //Decrements the offset by one
  fIndexes[fIndexes.fN-1]--;
}

inline void HLocation::operator-=(Int_t shift) {
  //Decrements the offset by "shift"
  fIndexes[fIndexes.fN-1]-=shift;
}

inline HLocation &HLocation::operator=(HLocation &loc) {
  loc.fIndexes.Copy(fIndexes);
  return loc;
}

void HLocation::readIndexes(HLocation &loc) {
  //Sets the loc.getNIndex() first indexes to the value given in loc
  //and the others are set to 0
  Int_t n=0,i=0;
  n=(fIndexes.fN>loc.getNIndex())?loc.getNIndex():fIndexes.fN;
  for (i=0;i<n;i++) {
    fIndexes.fArray[i]=loc.fIndexes.fArray[i];
  }
  for (;i<fIndexes.fN;i++) fIndexes[i]=0;
}

void HLocation::Dump(void) {
  //Dumps the object to cout in the format:
  //
  //index1:index2:....:offset
  Int_t i;
  
  cout << "fIndexes.fN|";
  for (i=0;i<fIndexes.fN;i++) {
    if (i==fIndexes.fN-1) cout << fIndexes[i] << endl;
    else cout << fIndexes[i] << ":" ;
  }
} 


