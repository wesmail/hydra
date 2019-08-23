#pragma implementation
//*-- Author : Manuel Sanchez Garcia
//*-- Modified : 27/05/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)

/////////////////////////////////////////////////////////
//HIndexTable
//
//  This class handles an index table with an entry per 
//  possible HLocation. In such a way that, given a HLocation,it returns
//  an index.
//
//  You can also iterate on the index table (on the locations) using the
// functions gotoBegin(),gotoLocation() and next().
//
//  You can also access the indexes as if they were lineally distributed,
//
//////////////////////////////////////////////////////////
#include "hindextable.h"
#include "hlocation.h"
#include "hdebug.h"

ClassImp(HIndexTable)

HIndexTable::HIndexTable(void) {
  //Default constructor
}

HIndexTable::~HIndexTable(void) {
  //Destructor
}

void HIndexTable::setDimensions(Int_t nDim,Int_t *sizes) {
  //Sets the number of dimensions of the table and its corresponding
  //sizes.
  //
  //Input:
  // nDim ---> number of indexes to define an entry
  // sizes --> max values of those indexes.
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HIndexTable::setDimensions");
  gDebuger->message("nDim= %i",nDim);
#endif
  Int_t prod=1,i=0;
  fSizes.Set(nDim,sizes);
  for (i=0;i<nDim;i++) { prod*=sizes[i];}
#if DEBUG_LEVEL>2
  gDebuger->message("Setting index array");
#endif
  fIndexArray.Set(prod);
  for (i=0;i<prod;i++) fIndexArray[i]=-1;
#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HIndexTable::setDimensions");
#endif
}

inline void HIndexTable::setIndex(HLocation &aLoc,Int_t idx) {
  //Sets the value of the index associated to aLoc to the value idx
  Int_t la;
  la=aLoc.getLinearIndex(&fSizes);
  fIndexArray[la]=idx;
}

inline Int_t HIndexTable::getIndex(HLocation &aLoc) {
  //Returns the value of the index associated to the location aLoc
  //If the index was not set with setIndex, this function returns -1
  return (fIndexArray[aLoc.getLinearIndex(&fSizes)]);
}

void HIndexTable::Clear(void) {
  //Resets all the indexes in the table.
  Int_t i;
  for (i=0;i<fIndexArray.fN;i++) fIndexArray[i]=-1;
}

TArrayI *HIndexTable::getDimensions(void) {
  //Returns an array of integers containing the sizes of the dimensions of the table
  return &fSizes;
}

inline void HIndexTable::setIndex(Int_t linAddr,Int_t idx) {
  //This is the counterpart of getIndex(Int_t linAddr). That is, sets an index given its
  //linear address. 
  fIndexArray[linAddr]=idx;
}

const Int_t HIndexTable::getIndex(Int_t linAddr) {
  //Returns the index at the linear address "linAddr". The linear address can
  // be calculated from the sizes and the location. For example:
  //
  //  If the location is 4:16 and the sizes are 200 and 320 then the 
  //corresponding linear address is:
  //linAddr = 4 x 320 + 16
  return fIndexArray[linAddr];
}

inline Int_t HIndexTable::getEntries(void) {
  //Returns the number of entries (possible locations) in the index table
  return fIndexArray.fN;
}

Bool_t HIndexTable::checkLocation(HLocation &aLoc) {
  //Checks if there is an entry in the index table for this location
  Int_t i;
  for (i=0;i<fSizes.fN;i++) {
    if (aLoc.getIndex(i)>=fSizes[i]) return kFALSE;
  }
  return kTRUE;
}

Int_t HIndexTable::gotoLocation(HLocation &aLoc) {
  //This function along with HIndexTable::next allows iteration on the index
  //table through all the indexes corresponding to the location aLoc
  //
  //gotoLocation positions a cursor at the index given by aLoc. It should be called
  //before any call to HIndexTable::next()
  //
  //Return value:
  //  The number of positions corresponding to the given location.
  Int_t n=1,i=0;
  if (aLoc.getNIndex()==fSizes.fN) 
    n=1;
  else {
    n=1;
    for(i=aLoc.getNIndex();i<fSizes.fN;i++) 
      n*=fSizes[i];
  }
  fCurrentPos=aLoc.getLinearIndex(&fSizes);
  return n;
}

Int_t HIndexTable::gotoBegin(void) {
  //This function positions the cursor for iterating on the HIndexTable at its
  // very beggining.
  fCurrentPos=0;
  return fSizes.fN;
}

Int_t HIndexTable::next(void) {
  //This function along with HIndexTable::gotoLocation allows iteration on the index
  //table.
  //
  //next advances to the next location by incrementing a cursor. It returns
  //the index associated to the current location (the location before advancing)
  Int_t idx;
  idx=fIndexArray[fCurrentPos];
  if (fCurrentPos<fIndexArray.fN-1) fCurrentPos++;
  return idx;
}
