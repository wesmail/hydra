#include "hglobalcont.h"
#include "hglobalcondition.h"
#include "herror.h"
#include "honlinecontrol.h"
#include "hconsistency.h"

//-------------------------------------------------------------------------
//
// File name:       hglobalcont.cc
// Subject:         Container of global conditions.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HGlobalCont.
// This class serves as a container of global conditions.
// All global conditions are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

ClassImp(HGlobalCont)

//---------------------------------------------------------------------
//******  HGlobalCont::~HGlobalCont()
//
// Destructor of the class HGlobalCont.
//
//---------------------------------------------------------------------

HGlobalCont::~HGlobalCont() {

  if(fGlobalAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fGlobalAr;
  }

}


//---------------------------------------------------------------------
//******  Bool_t HGlobalCont::init()
//
// This function makes initialization of array fGlobalAr (actual container
// of global conditions).
//
//---------------------------------------------------------------------

Bool_t HGlobalCont::init() {

  if(fGlobalAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fGlobalAr;
    fGlobalAr = 0;

  }
  fGlobalAr = new TObjArray(500);
  if(!fGlobalAr) return kFALSE;
  return kTRUE;

}  


//---------------------------------------------------------------------
//******  Bool_t HGlobalCont::add(HGlobalCondition *p)
//
// This function adds to the list of global conditions a new condition at
// the next empty slot.
// 
//---------------------------------------------------------------------

Bool_t HGlobalCont::add(HGlobalCondition *p) {

  if(!fGlobalAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  Int_t i;
  HGlobalCondition *pGlobal;
  for(i=0; i < getEntries(); i++) {
    pGlobal = at(i);
    if(pGlobal) {
      if(!strcmp(pGlobal->GetName(),p->GetName())) return kFALSE;
    }
  }
  for(i=0; i < getEntries(); i++) {
    pGlobal = at(i);
    if(!pGlobal) {
      fGlobalAr->AddAt(p,i);
      return kTRUE;
    }
  }
  fGlobalAr->Add(p);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HGlobalCont::addAt(HGlobalCondition *p, Int_t idx)
//
// This function adds to the list of global conditions a new condition at
// the position idx.
// 
//---------------------------------------------------------------------

Bool_t HGlobalCont::addAt(HGlobalCondition *p, Int_t idx) {

  if(!fGlobalAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  if(idx < 0) return kFALSE;
  Int_t i;
  HGlobalCondition *pGlobal;
  for(i=0; i < getEntries(); i++) {
    pGlobal = at(i);
    if(pGlobal) {
      if(!strcmp(pGlobal->GetName(),p->GetName())) return kFALSE;
    }
  }
  fGlobalAr->AddAt(p,idx);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HGlobalCont::removeAt(Int_t idx, Bool_t kSulSerio = kTRUE)
//
// This function removes from the list of global conditions the one with
// index idx. It also takes care of actual deleting the objectin case
// the flag kSulSerio is true.
// 
//---------------------------------------------------------------------

void HGlobalCont::removeAt(Int_t idx, Bool_t kSulSerio) {

  if(!fGlobalAr) {
    return;
  }
  if(idx < 0) return;
  HGlobalCondition *pDel;
  pDel = (HGlobalCondition*) fGlobalAr->RemoveAt(idx);
  if(kSulSerio) {
    if(pDel) { delete pDel; pDel = 0; }
  }

}


//---------------------------------------------------------------------
//******  Int_t HGlobalCont::getEntries()
//
// This function returns number of entries in array fGlobalAr.
// Returns -1 if the array is not initialized.
//
//---------------------------------------------------------------------

Int_t HGlobalCont::getEntries() {

  if(!fGlobalAr) {
    return -1;
  }
  return (fGlobalAr->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HGlobalCondition* HGlobalCont::at(Int_t idx)
//
// This function returns address of the global condition stored at position
// idx in fGlobalAr.
// Returns 0 in case fGlobalAr is not initialized or idx is out of bounds.
//
//---------------------------------------------------------------------

HGlobalCondition* HGlobalCont::at(Int_t idx) {

  if(!fGlobalAr) {
    return 0;
  }
  if(idx < 0) return 0;
  return ((HGlobalCondition*) fGlobalAr->At(idx));

}


//---------------------------------------------------------------------
//******  Int_t HGlobalCont::find(const Char_t* name)
//
// This function finds (in the list of global conditions) and returns index 
// of the global condition. Input argument is the name of the condition to
// be found. In case of no condition found the function returns -1.
// 
//---------------------------------------------------------------------

Int_t HGlobalCont::find(const Char_t* name) {

  if(!fGlobalAr) {
    return -1;
  }
  Int_t i;
  HGlobalCondition *pGlobal;

  for(i=0; i < getEntries(); i++) {

    pGlobal = at(i);
    if(pGlobal) {
      if(!strcmp(name,pGlobal->GetName())) return i;
    }
  }

  return -1;

}


//---------------------------------------------------------------------
//******  void HGlobalCont::checkConsistency()
//
// This function calls checkConsistency() functions of all global conditions
// that are stored in the list fGlobal.
// All found inconsistent globall conditions are stored in array fGCIdx of
// the class HConsistency (fConsistency).
//
//---------------------------------------------------------------------

void HGlobalCont::checkConsistency() {

  if(!gOnline) return (HError::message("gOnline not initialized"));
  if(!gOnline->getConsistency()) 
    return (HError::message("fConsistency not initialized"));
  if(!fGlobalAr) 
    return (HError::message("Container of global conditions not initialized"));

  HGlobalCondition *pGlobal;

  for(Int_t i=0; i<getEntries(); i++) {

    pGlobal = at(i);
    if(pGlobal) {
      if(!pGlobal->checkConsistency()) 
	(*(gOnline->getConsistency())).fGCIdx[i] = 1;
      else (*(gOnline->getConsistency())).fGCIdx[i] = 0;
    }
  }

}  






















