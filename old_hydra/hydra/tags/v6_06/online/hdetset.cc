//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hdetset.cc
// Subject:         Detector sets.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
// Remarks:         The concept of detector sets was initially proposed and
//                  developed by
//                  P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the classes HDetSet, HDetSetOut,
// HShowerDetSet, HMdcDetSet, HRichDetSet, HTofDetSet.
// These classes provide all functions to maintain the online objects of 
// the type "detector set".
//
//-------------------------------------------------------------------------

#include "hdetset.h"
#include "hcalclocation.h"
#include "hindextransform.h"
#include <iostream.h>

#define ShMaxCells     18432
#define ShMaxModules   18
#define MdcMaxCells    28800
#define MdcMaxModules  24
#define TofMaxCells    384
#define TofMaxModules  6
#define RichMaxCells   55296
#define RichMaxModules 6

char userword[50];

///////////////////////////////////////////////////////////////////////
// The class HDetSet is a base class which defines generic behaviour
// for the online objects of the type "detector set".
// HTofDetSet    - implementation for the TOF detector.
// HMdcDetSet    - implementation for the MDC detector.
// HShowerDetSet - implementation for the Shower detector.
// HRichDetSet   - implementation for the RICH detector.
//
// HDetSetOut    - outcome of the procedure of "evaluation"
//                 (list of physical locations in the event structure 
//                  of the fired cells which belong to the detector set)
//
// The concept of detector sets was initially proposed and developed by
// P.Finocchiaro (INFN-LNS, Catania, "finocchiaro@lns.infn.it") 
/////////////////////////////////////////////////////////////////////////
ClassImp(HDetSetOut)
ClassImp(HDetSet)
ClassImp(HShowerDetSet)
ClassImp(HTofDetSet)
ClassImp(HMdcDetSet)
ClassImp(HRichDetSet)


//*******************************************************
//
//----- HDetSet member functions --------------------------
//

//---------------------------------------------------------------------
//******  HDetSet::HDetSet()
//
// Constructor of the class HDetSet.
//
//---------------------------------------------------------------------

HDetSet::HDetSet() : TNamed() {
//
// Constructor of the class HDetSet.
//

  fDetName = -1;
  fCategory = -1;
  resetIter();
  kMulti = kFALSE;
  fMinCell = -1;
  fMaxCell = -1;
  fOut = 0;
  kEvaluated = kFALSE;

}


//---------------------------------------------------------------------
//******  void HDetSet::evaluate()
//
// This function makes evaluation of a detector set.
// Evaluation means creation of a list of locations (inside the category
// of the detector set) with hits inside the cells of the detector set.
//
//---------------------------------------------------------------------

void HDetSet::evaluate() {
//
// This function makes evaluation of a detector set.
// Evaluation means creation of a list of locations (inside the category
// of the detector set) with hits inside the cells which belong to
// the detector set.
//

  kEvaluated = kFALSE;

  if(!fOut) {
    fOut = new HDetSetOut();
    if(!fOut) return;
    if(!fOut->init()) return;
  }

  fOut->clearList();
  HCalcLocation::calcLoc(this);
  kEvaluated = kTRUE;

}


//---------------------------------------------------------------------
//******  void HDetSetOut::clearList()
//
// This function clears the detector set output list.
//
//---------------------------------------------------------------------

void HDetSetOut::clearList() {
//
// This function clears the detector set output list.
//

  //  cout << "Enter clearList" << endl;  

  if(fList) fList->Delete();

  //  cout << "Exit clearList" << endl;

}


//---------------------------------------------------------------------
//******  void HDetSet::calcMinMax()
//
// This function calculates minimum and maximum cell number inside 
// the detector set. This information is used by the mechanism of 
// multihistogram definition.
//
//---------------------------------------------------------------------

void HDetSet::calcMinMax() {
//
// This function calculates minimum and maximum cell number inside 
// the detector set. This information is used by the mechanism of 
// multihistogram definition.
//

  Int_t idx[4];
  Int_t n;

  resetIter();

  if(!iterate(n,idx)) {
    fMinCell = fMaxCell = -1;
    return;
  }
  if(fCategory == 1 || fCategory == 33) { // MDC hit or RICH hit
    fMinCell = fMaxCell = -1;
    return;
  }
  else fMinCell = fMaxCell = fIter;

  while(iterate(n,idx)) {

    if(fIter > fMaxCell) fMaxCell = fIter;

  }

}


//---------------------------------------------------------------------
//******  Bool_t HDetSet::setSingleAddress(Int_t *idx, Int_t n)
//
// This function sets a detector set which contains only one cell. 
//
//---------------------------------------------------------------------

Bool_t HDetSet::setSingleAddress(Int_t *idx, Int_t n) {
//
// This function sets a detector set which contains only one cell. 
//

  if(n != fSingleAddress.fN) return kFALSE;
  if(!idx) return kFALSE;

  for(Int_t i=0; i<n; i++) fSingleAddress[i] = idx[i];

  setMulti(kFALSE);

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HDetSet::setHitAddress(Int_t *idx, Int_t n)
//
// This function sets a detector set for hit level. 
// Not used so far.
//
//---------------------------------------------------------------------

Bool_t HDetSet::setHitAddress(Int_t *idx, Int_t n) {
//
// This function sets a detector set for hit level. 
// Not used so far.
//

  if(n != fHitAddress.fN) return kFALSE;
  if(!idx) return kFALSE;

  for(Int_t i=0; i<n; i++) fHitAddress[i] = idx[i];

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HDetSet::setMultiAddress(Int_t *idx, Int_t n)
//
// This function sets a multi detector set. 
//
//---------------------------------------------------------------------

Bool_t HDetSet::setMultiAddress(Int_t *idx, Int_t n) {
//
// This function sets a multi detector set. 
//

  if(n != fMultiAddress.fN) return kFALSE;
  if(!idx) return kFALSE;

  for(Int_t i=0; i<n; i++) fMultiAddress[i] = idx[i];

  setMulti();

  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HDetSet::clear()
//
// This function clears arrays which contain definition of the detector set. 
//
//---------------------------------------------------------------------

void HDetSet::clear() {
//
// This function clears arrays which contain definition of the detector set. 
//

  Int_t i;
  
  for(i=0; i<fMultiAddress.fN; i++) fMultiAddress[i] = 0;
  for(i=0; i<fSingleAddress.fN; i++) fSingleAddress[i] = -1;
  for(i=0; i<fHitAddress.fN; i++) fHitAddress[i] = 0;

}


const char* HDetSet::getUserWord() {
//
// The name of a detector set consists of two parts.
// 1. The first part of the name is defined by the system. It reflects
//    the detector type and data category(for example, "tofraw", "mdccal1").
// 2. The second part is a user word.
// This function returns the user word. 
//

  char tmp[50];
  Int_t i;

  sprintf(tmp,"%s",GetName());
  for(i = (strlen(tmp)-1); tmp[i] != '_'; i--);
  strncpy(userword,&tmp[i+1],strlen(tmp)-i+1);

  return userword;

}


//---------------------------------------------------------------------
//******  Bool_t HDetSet::isGeomEqual(HDetSet *pSet)
//
// This function checks if the geometrical parts of the two detector sets
// are identical. 
//
//---------------------------------------------------------------------

Bool_t HDetSet::isGeomEqual(HDetSet *pSet) {
//
// This function checks if the geometrical parts of the two detector sets
// are identical. 
//

  if(!pSet) return kFALSE;
  if((isMulti() && !pSet->isMulti()) || (!isMulti() && pSet->isMulti()))
    return kFALSE;
  if(isMulti() && pSet->isMulti()) {
    if(fMultiAddress.fN != (pSet->getMultiAddress()).fN) return kFALSE;

    for(Int_t i=0; i<fMultiAddress.fN; i++) {
      if(fMultiAddress[i] != (pSet->getMultiAddress())[i]) return kFALSE;
    }
  }

  if(!isMulti() && !pSet->isMulti()) {
    if(fSingleAddress.fN != (pSet->getSingleAddress()).fN) return kFALSE;
    
    for(Int_t i=0; i<fSingleAddress.fN; i++) {
      if(fSingleAddress[i] != (pSet->getSingleAddress())[i]) return kFALSE;
    }
  }

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HDetSet::isEqual(HDetSet *pSet)
//
// This function checks if the two detector sets are identical. 
//
//---------------------------------------------------------------------

Bool_t HDetSet::isEqual(HDetSet *pSet) {
//
// This function checks if the two detector sets are identical. 
//

  if(!isGeomEqual(pSet)) return kFALSE;

  if(fCategory != pSet->getCategory()) return kFALSE;

  return kTRUE;

}



//***************************************************************
//
//----- HShowerDetSet member functions --------------------------
//
//---------------------------------------------------------------------
//******  HShowerDetSet::HShowerDetSet()
//
// Constructor of the class HShowerDetSet. 
//
//---------------------------------------------------------------------

HShowerDetSet::HShowerDetSet() : HDetSet() { 
//
// Constructor of the class HShowerDetSet. 
//

  fMultiAddress.Set(1152); // 18 * 64 
  fHitAddress.Set(18);     // 6*3
  fSingleAddress.Set(4);   // sec mod row col
  clear();
  setDetName(SHOWER);

}


//---------------------------------------------------------------------
//******  Int_t* HShowerDetSet::iterateHit(Int_t &n)
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far, since needs modifications.
//
//---------------------------------------------------------------------

Int_t* HShowerDetSet::iterateHit(Int_t &n) {
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far, since needs modifications.
//

  //  Int_t maxind[2] = { 6, 3 };
  Int_t *idx = 0;

  while(fIter < ShMaxModules-1) {

    fIter++;
    if(fHitAddress[fIter] == 1) {

      n = 2;
      //      idx = HIndexTransform::lin2mult(2, maxind, fIter);
      break;
    }
  }

  return idx;

}



//---------------------------------------------------------------------
//******  Bool_t HShowerDetSet::iterate(Int_t &n, Int_t* idx)
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//
//---------------------------------------------------------------------

Bool_t HShowerDetSet::iterate(Int_t &n, Int_t* idx) {
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//

  Bool_t retval = kFALSE;
  if(!idx) return kFALSE;

  Int_t maxindcell[4] = { 6, 3, 32, 32 };
  Int_t max1[2] = { 6, 3 };
  Int_t max2[2] = { 8, 8 };
  Int_t max3[2] = { 18, 64 };
  Int_t curidx[2];

  Int_t i,j;
  Int_t index;

  while(fIter < ShMaxCells-1) {

    fIter++;
    if(HIndexTransform::lin2mult(4,maxindcell,fIter,idx)) {

      curidx[0] = idx[0];
      curidx[1] = idx[1];
      i = HIndexTransform::mult2lin(2,max1,curidx);
      
      curidx[0] = idx[2]/4;
      curidx[1] = idx[3]/4;
      
      j = HIndexTransform::mult2lin(2,max2,curidx);
      
      curidx[0] = i;
      curidx[1] = j;

      index = HIndexTransform::mult2lin(2,max3,curidx);

      if(fMultiAddress[index] == 1) {
	n = 4;
	retval = kTRUE;
	break;
      }
      else retval = kFALSE;
    }
    else retval = kFALSE;
  }

  return retval;

}

//***********************************************************
//
//----- HMdcDetSet member functions --------------------------
//
//---------------------------------------------------------------------
//******  HMdcDetSet::HMdcDetSet()
//
// Constructor of the class HMdcDetSet. 
//
//---------------------------------------------------------------------

HMdcDetSet::HMdcDetSet() : HDetSet() {
//
// Constructor of the class HMdcDetSet. 
//

  fMultiAddress.Set(1152); // 24 * 48 
  fHitAddress.Set(24);     // 6*4
  fSingleAddress.Set(4);   // sec mod layer cell
  clear();
  setDetName(MDC);

}


//---------------------------------------------------------------------
//******  Int_t* HMdcDetSet::iterateHit(Int_t &n)
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far, since needs modifications.
//
//---------------------------------------------------------------------

Int_t* HMdcDetSet::iterateHit(Int_t &n) {
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far, since needs modifications.
//

  //  Int_t maxind[2] = { 6, 4 };
  Int_t *idx = 0;

  while(fIter < MdcMaxModules-1) {

    fIter++;
    if(fHitAddress[fIter] == 1) {

      n = 2;
      //      idx = HIndexTransform::lin2mult(2, maxind, fIter);
      break;
    }
  }

  return idx;

}


//---------------------------------------------------------------------
//******  Bool_t HMdcDetSet::iterate(Int_t &n, Int_t* idx)
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//
//---------------------------------------------------------------------

Bool_t HMdcDetSet::iterate(Int_t &n, Int_t* idx) {
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//

  Bool_t retval = kFALSE;
  if(!idx) return kFALSE;
  Int_t maxindcell[4] = { 6, 4, 6, 200 };
  Int_t max1[2] = { 6, 4 };
  Int_t max2[2] = { 6, 8 };
  Int_t max3[2] = { 24, 48 };
  Int_t curidx[2];

  Int_t i,j;
  Int_t index;

  while(fIter < MdcMaxCells-1) {

    fIter++;
    if(HIndexTransform::lin2mult(4,maxindcell,fIter,idx)) {

      curidx[0] = idx[0];
      curidx[1] = idx[1];
      i = HIndexTransform::mult2lin(2,max1,curidx);

      curidx[0] = idx[2];
      curidx[1] = idx[3]/25;

      j = HIndexTransform::mult2lin(2,max2,curidx);

      curidx[0] = i;
      curidx[1] = j;

      index = HIndexTransform::mult2lin(2,max3,curidx);

      if(fMultiAddress[index] == 1) {
	n = 4;
	retval = kTRUE;
	break;
      }
      else retval = kFALSE;
    }
    else retval = kFALSE;
  }

  return retval;

}

//*************************************************************
//
//----- HTofDetSet member functions --------------------------
//
//---------------------------------------------------------------------
//******  HTofDetSet::HTofDetSet()
//
// Constructor of the class HTofDetSet. 
//
//---------------------------------------------------------------------
 
HTofDetSet::HTofDetSet() : HDetSet() {
//
// Constructor of the class HTofDetSet. 
//


  fMultiAddress.Set(48);   // 6*8 
  fHitAddress.Set(6);      // 6
  fSingleAddress.Set(3);   // sec mod rod
  clear();
  setDetName(TOF);

}


//---------------------------------------------------------------------
//******  Int_t* HTofDetSet::iterateHit(Int_t &n)
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far.
//
//---------------------------------------------------------------------

Int_t* HTofDetSet::iterateHit(Int_t &n) {
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far.
//

  Int_t *idx = 0;

  while(fIter < TofMaxModules-1) {

    fIter++;
    if(fHitAddress[fIter] == 1) {

      n = 1;
      idx = new Int_t;
      *idx = fIter;
      break;
    }
  }

  return idx;

}


//---------------------------------------------------------------------
//******  Bool_t HTofDetSet::iterate(Int_t &n, Int_t* idx)
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//
//---------------------------------------------------------------------

Bool_t HTofDetSet::iterate(Int_t &n, Int_t* idx) {
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//

  Bool_t retval = kFALSE;
  if(!idx) return kFALSE;

  Int_t maxindcell[3] = { 6, 8, 8};
  Int_t max1[2] = { 6, 8 };

  Int_t curidx[2];
  Int_t index;

  while(fIter < TofMaxCells-1) {

    fIter++;
    if(HIndexTransform::lin2mult(3,maxindcell,fIter,idx)) {

      curidx[0] = idx[0];
      curidx[1] = idx[1];

      index = HIndexTransform::mult2lin(2,max1,curidx);

      if(fMultiAddress[index] == 1) {
	n = 3;
	retval = kTRUE;
	break;
      }
      else retval = kFALSE;
    }
    else retval = kFALSE;
  }

  return retval;
}


//***************************************************************
//
//----- HRichDetSet member functions --------------------------
//
//---------------------------------------------------------------------
//******  HRichDetSet::HRichDetSet()
//
// Constructor of the class HRichDetSet. 
//
//---------------------------------------------------------------------

HRichDetSet::HRichDetSet() : HDetSet() { 
//
// Constructor of the class HRichDetSet. 
//

  fMultiAddress.Set(384);  // 6 * 64 
  fHitAddress.Set(6);      // 6*1
  fSingleAddress.Set(3);   // sec row col
  clear();
  setDetName(RICH);

}


//---------------------------------------------------------------------
//******  Int_t* HRichDetSet::iterateHit(Int_t &n)
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far.
//
//---------------------------------------------------------------------

Int_t* HRichDetSet::iterateHit(Int_t &n) {
//
// This function makes iteration over cells in the array fHitAddress. 
// Not used so far.
//

  Int_t *idx = 0;

  while(fIter < RichMaxModules-1) {

    fIter++;
    if(fHitAddress[fIter] == 1) {

      n = 1;
      idx = new Int_t;
      *idx = fIter;
      break;
    }
  }

  return idx;

}


//---------------------------------------------------------------------
//******  Bool_t HRichDetSet::iterate(Int_t &n, Int_t* idx)
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//
//---------------------------------------------------------------------

Bool_t HRichDetSet::iterate(Int_t &n, Int_t* idx) {
//
// This function makes iteration over cells in the array fMultiAddress. 
// idx - one dimensional array with cell indexes
// n   - idx array size
//
// Not used, since it is very slow.
//

  Bool_t retval = kFALSE;
  if(!idx) return kFALSE;
  Int_t maxindcell[3] = { 6, 96, 96};
  Int_t max1[2] = { 8, 8 };
  Int_t max2[2] = { 6, 64 };

  Int_t curidx[2];
  Int_t i;
  Int_t index;

  while(fIter < RichMaxCells-1) {

    fIter++;
    if(HIndexTransform::lin2mult(3,maxindcell,fIter,idx)) {

      curidx[0] = idx[1]/12;
      curidx[1] = idx[2]/12;

      i = HIndexTransform::mult2lin(2,max1,curidx);

      curidx[0] = idx[0];
      curidx[1] = i;

      index = HIndexTransform::mult2lin(2,max2,curidx);

      if(fMultiAddress[index] == 1) {
	n = 3;
	retval = kTRUE;
	break;
      }
      else retval = kFALSE;
    }
    else retval = kFALSE;
  }

  return retval;

}













