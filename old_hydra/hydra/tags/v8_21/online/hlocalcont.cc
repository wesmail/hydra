//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/


//-------------------------------------------------------------------------
//
// File name:       hlocalcont.cc
// Subject:         Container of local conditions.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HLocalCont.
// This class serves as a container of local conditions.
// All detector sets are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------
using namespace std;
#include "hlocalcont.h"
#include "hlocalcondition.h"
#include "herror.h"
#include "honlinecontrol.h"
#include "hconsistency.h"

#include <iostream> 
#include <iomanip>

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
// This class serves as a container of local conditions.
// All detector sets are stored in an array which is a TObjArray object.
////////////////////////////////////////////////////////////////////////
ClassImp(HLocalCont)


//---------------------------------------------------------------------
//******  HLocalCont::HLocalCont()
//
// Constructor of the class HLocalCont.
//
//---------------------------------------------------------------------

HLocalCont::HLocalCont() : TObject() {
//
// Constructor of the class HLocalCont.
//

  fLocalAr = 0;

}


//---------------------------------------------------------------------
//******  HLocalCont::~HLocalCont()
//
// Destructor of the class HLocalCont.
//
//---------------------------------------------------------------------

HLocalCont::~HLocalCont() {
//
// Destructor of the class HLocalCont.
//

  if(fLocalAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fLocalAr;
  }

}


//---------------------------------------------------------------------
//******  Bool_t HLocalCont::init()
//
// This function makes initialization of array fLocalAr (actual container
// of local conditions).
//
//---------------------------------------------------------------------

Bool_t HLocalCont::init() {
//
// This function makes initialization of array fLocalAr (actual container
// of local conditions).
//

  if(fLocalAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fLocalAr;
    fLocalAr = 0;

  }
  fLocalAr = new TObjArray(500);
  if(!fLocalAr) return kFALSE;
  return kTRUE;

}  


//---------------------------------------------------------------------
//******  Bool_t HLocalCont::add(HLocalCondition *p)
//
// This function adds to the list of local conditions a new condition at
// the next empty slot.
// 
//---------------------------------------------------------------------

Bool_t HLocalCont::add(HLocalCondition *p) {
//
// This function adds to the list of local conditions a new condition at
// the next empty slot.
// 

  if(!fLocalAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  Int_t i;
  HLocalCondition *pLocal;
  for(i=0; i < getEntries(); i++) {
    pLocal = at(i);
    if(pLocal) {
      if(!strcmp(pLocal->GetName(),p->GetName())) return kFALSE;
    }
  }
  for(i=0; i < getEntries(); i++) {
    pLocal = at(i);
    if(!pLocal) {
      fLocalAr->AddAt(p,i);
      return kTRUE;
    }
  }
  fLocalAr->Add(p);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HLocalCont::addAt(HLocalCondition *p, Int_t idx)
//
// This function adds to the list of local conditions a new condition at
// the position idx.
// 
//---------------------------------------------------------------------

Bool_t HLocalCont::addAt(HLocalCondition *p, Int_t idx) {
//
// This function adds to the list of local conditions a new condition at
// the position idx.
// 

  if(!fLocalAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  if(idx < 0) return kFALSE;

  Int_t i;
  HLocalCondition *pLocal;
  for(i=0; i < getEntries(); i++) {

    pLocal = at(i);
    if(pLocal) {
      if(!strcmp(pLocal->GetName(),p->GetName())) return kFALSE;
    }
  }

  fLocalAr->AddAt(p,idx);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HLocalCont::removeAt(Int_t idx, Bool_t kSulSerio = kTRUE)
//
// This function removes from the list of local conditions the one with
// index idx. It also takes care of actual deleting the object in case
// the flag kSulSerio is true.
// 
//---------------------------------------------------------------------

void HLocalCont::removeAt(Int_t idx, Bool_t kSulSerio) {
//
// This function removes from the list of local conditions the one with
// index idx. It also takes care of actual deleting the object in case
// the flag kSulSerio is true.
// 

  if(!fLocalAr) {
    return;
  }
  if(idx < 0) return;
  HLocalCondition *pDel;
  pDel = (HLocalCondition*) fLocalAr->RemoveAt(idx);
  if(kSulSerio) {
    if(pDel) { delete pDel; pDel = 0; }
  }
}


//---------------------------------------------------------------------
//******  Int_t HLocalCont::getEntries()
//
// This function returns number of entries in array fLocalAr.
// Returns -1 if the array is not initialized.
//
//---------------------------------------------------------------------

Int_t HLocalCont::getEntries() {
//
// This function returns number of entries in array fLocalAr.
// Returns -1 if the array is not initialized.
//

  if(!fLocalAr) {
    return -1;
  }
  return (fLocalAr->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HLocalCondition* HLocalCont::at(Int_t idx)
//
// This function returns address of the local condition stored at position
// idx in fLocalAr.
// Returns 0 in case fLocalAr is not initialized or idx is out of bounds.
//
//---------------------------------------------------------------------

HLocalCondition* HLocalCont::at(Int_t idx) {
//
// This function returns address of the local condition stored at position
// idx in fLocalAr.
// Returns 0 in case fLocalAr is not initialized or idx is out of bounds.
//

  if(!fLocalAr) {
    return 0;
  }
  if(idx < 0) return 0;
  return ((HLocalCondition*) fLocalAr->At(idx));

}


//---------------------------------------------------------------------
//******  Int_t HLocalCont::find(const Char_t* name)
//
// This function finds (in the list of local conditions) and returns index 
// of the local condition. Input argument is the name of the condition to
// be found. In case of no condition found the function returns -1.
// 
//---------------------------------------------------------------------

Int_t HLocalCont::find(const Char_t* name) {
//
// This function finds (in the list of local conditions) and returns index 
// of the local condition. Input argument is the name of the condition to
// be found. In case of no condition found the function returns -1.
// 

  if(!fLocalAr) {
    return -1;
  }
  Int_t i;
  HLocalCondition *pLocal;

  for(i=0; i < getEntries(); i++) {

    pLocal = at(i);
    if(pLocal) {
      if(!strcmp(name,pLocal->GetName())) return i;
    }
  }

  return -1;

}


//---------------------------------------------------------------------
//******  void HLocalCont::checkConsistency()
//
// This function calls checkConsistency() functions of all local conditions
// that are stored in the list fLocal.
// All found inconsistent local conditions are stored in array fLCIdx of
// the class HConsistency (fConsistency).
//
//---------------------------------------------------------------------

void HLocalCont::checkConsistency() {
//
// This function calls checkConsistency() functions of all local conditions
// that are stored in the list fLocal.
// All found inconsistent local conditions are stored in array fLCIdx of
// the class HConsistency (fConsistency).
//

  if(!gOnline) return (HError::message("gOnline not initialized"));
  if(!gOnline->getConsistency()) 
    return (HError::message("fConsistency not initialized"));
  if(!fLocalAr) 
    return (HError::message("Container of local conditions not initialized"));

  HLocalCondition *pLocal;

  for(Int_t i=0; i<getEntries(); i++) {

    pLocal = at(i);
    if(pLocal) {
      if(!pLocal->checkConsistency()) 
	(*(gOnline->getConsistency())).fLCIdx[i] = 1;
      else (*(gOnline->getConsistency())).fLCIdx[i] = 0;
    }
  }

}  


/*
//---------------------------------------------------------------------
//  Int_t HLocalCont::checkConsistency()
//
// This function calls checkConsistency() functions of all local conditions
// that are stored in the list fLocal.
// The return value has the following meanings:
//
//-1 - container of local conditions is not initialized (error status)
// 0 - all local conditions have passed consistency check
// 1 - there are inconsistent conditions but the user has clicked OK button
//     on the panel with the warning message
// 2 - there are inconsistent conditions and the user has clicked Cancel button
//     on the panel with the warning message
//
//---------------------------------------------------------------------

Int_t HLocalCont::checkConsistency() {

  Bool_t kCheckResult = kFALSE;
  Bool_t kWasInconsistent = kFALSE;

  if(!fLocalAr) {
    HError::message("container of local conditions is not initialized");
    return -1;
  }

  HLocalCondition *pLocal;
  Text_t msg[5000];
  Text_t tmp[5000];
  Int_t  count = 0;

  for(Int_t i=0; i<getEntries(); i++) {

    pLocal = at(i);
    if(pLocal) {

      kCheckResult = pLocal->checkConsistency();
      if(!kCheckResult) {

	if(count == 0) 
	  sprintf(msg,"%s","Following conditions will be inconsistent:");
	sprintf(tmp,"%s",msg);
	count++;
	if(count%5 != 1) sprintf(msg,"%s%s%s",tmp,"\t",pLocal->GetName());
	else sprintf(msg,"%s%s%s",tmp,"\n",pLocal->GetName());
	kWasInconsistent = kTRUE;
      }
    }
  }

  if(kWasInconsistent) return (HError::warning(msg));
  return 0;

}
*/










