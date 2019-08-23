//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hconditionout.cc
// Subject:         Output of the local condition.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HConditionOut.
// This class is an output of the local condition. It stores the list of 
// locations for the hits which passed the condition, the list of geometrical
// locations for the hits which passed the condition and the condition 
// counters.
// count1 - condition was true
// count2 - condition was false
// countS = count1 + count2
//
//-------------------------------------------------------------------------

#include "hconditionout.h"

///////////////////////////////////////////////////////////////////////////
// This class is an output of the local condition. It stores the list of 
// locations for the hits which passed the condition, the list of geometrical
// locations for the hits which passed the condition and the condition 
// counters.
// count1 - condition was true
// count2 - condition was false
// countS = count1 + count2
///////////////////////////////////////////////////////////////////////////
ClassImp(HConditionOut)


//---------------------------------------------------------------------
//******  HConditionOut::HConditionOut()
//
// Constructor of the class HConditionOut.
//
//---------------------------------------------------------------------

HConditionOut::HConditionOut() : TObject() {
//
// Constructor of the class HConditionOut.
//

  fLocList = 0;
  fIdxList = 0;
  kTrueFlag = kFALSE;
  count1 = count2 = countS = 0;

}


//---------------------------------------------------------------------
//******  HConditionOut::~HConditionOut()
//
// Destructor of the class HConditionOut.
//
//---------------------------------------------------------------------

HConditionOut::~HConditionOut() {
//
// Destructor of the class HConditionOut.
//

  if(fLocList) { fLocList->Delete(); delete fLocList; }
  if(fIdxList) { fIdxList->Delete(); delete fIdxList; }

}


//---------------------------------------------------------------------
//******  Bool_t HConditionOut::init()
//
// This function makes initialization of lists fLocList and fIdxList.
//
//---------------------------------------------------------------------

Bool_t HConditionOut::init() {
//
// This function makes initialization of lists fLocList and fIdxList.
//

  if(fLocList) {
    delete fLocList;
    fLocList = 0;
  }
  if(fIdxList) {
    delete fIdxList;
    fIdxList = 0;
  }
  fLocList = new TList();
  fIdxList = new TList();
  if(!fLocList || !fIdxList) return kFALSE;

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Int_t HConditionOut::getCount(Int_t number)
//
// This function returns value of one of the condition counters.
//
// number = 1 - count1 (condition was true)
//          2 - count2 (condition was false)
//          3 - countS (count1 + count2)
//
//---------------------------------------------------------------------

Int_t HConditionOut::getCount(Int_t number) {
//
// This function returns the value of one of the condition counters.
//
// number = 1 - count1 (condition was true)
//          2 - count2 (condition was false)
//          3 - countS (count1 + count2)
//

  switch(number) {

  case 1:  return count1;
  case 2:  return count2;
  case 3:  return countS;
  default: return -1;

  }

}


//---------------------------------------------------------------------
//******  void HConditionOut::incCount(Int_t number)
//
// This function increments one of the condition counters.
//
// number = 1 - count1 (condition was true)
//          2 - count2 (condition was false)
//
//---------------------------------------------------------------------

void HConditionOut::incCount(Int_t number) {
//
// This function increments one of the condition counters.
//
// number = 1 - count1 (condition was true)
//          2 - count2 (condition was false)
//

  switch(number) {

  case 1:  { count1++; countS++; kTrueFlag = kTRUE; break; }
  case 2:  { count2++; countS++; break; }
  default: break;

  }

}


//---------------------------------------------------------------------
//******  void HConditionOut::clearList()
//
// This function clears list fLocList and fIdxList and also takes care
// of actual deleting of the elements of the list fIdxList.
//
//---------------------------------------------------------------------

void HConditionOut::clearList() {
//
// This function clears list fLocList and fIdxList and also takes care
// of actual deleting of the elements of the list fIdxList.
//

  TObject *pObj;
  if(fLocList) fLocList->Clear();
  if(fIdxList) {
    TIter iter2(fIdxList);
    while((pObj = iter2())) {
      fIdxList->Remove(pObj);
      if(pObj) delete pObj;
    }
  }

}

  



