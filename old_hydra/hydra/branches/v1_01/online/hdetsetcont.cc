//-------------------------------------------------------------------------
//
// File name:       hdetsetcont.cc
// Subject:         Container of detector sets.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HDetSetCont.
// This class serves as a container of detector sets.
// All detector sets are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

#include "hdetsetcont.h"
#include "hdetset.h"

ClassImp(HDetSetCont)


//---------------------------------------------------------------------
//******  HDetSetCont::HDetSetCont()
//
// Constructor of the class HDetSetCont.
//
//---------------------------------------------------------------------

HDetSetCont::HDetSetCont() : TObject()  {

  fDetSetAr = 0;

}


//---------------------------------------------------------------------
//******  HDetSetCont::~HDetSetCont()
//
// Destructor of the class HDetSetCont.
//
//---------------------------------------------------------------------

HDetSetCont::~HDetSetCont() {

  if(fDetSetAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fDetSetAr;
    
  }
  
}


//---------------------------------------------------------------------
//******  Bool_t HDetSetCont::init()
//
// This function makes initialization of array fDetSetAr (actual container
// of detector sets).
//
//---------------------------------------------------------------------

Bool_t HDetSetCont::init() {

  if(fDetSetAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fDetSetAr;
    fDetSetAr = 0;

  }
  fDetSetAr = new TObjArray(500);
  if(!fDetSetAr) return kFALSE;
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HDetSetCont::add(HDetSet *p)
//
// This function adds to the list of detector sets a new detector set at
// the next empty slot.
//
//---------------------------------------------------------------------

Bool_t HDetSetCont::add(HDetSet *p) {

  if(!fDetSetAr) return kFALSE;
  if(!p) return kFALSE;
  Int_t i;
  HDetSet *pSet;
  for(i=0; i < getEntries(); i++) {
    pSet = at(i);
    if(pSet) {
      if(!strcmp(pSet->GetName(),p->GetName())) return kFALSE;
    }
  }
  for(i=0; i < getEntries(); i++) {
    pSet = at(i);
    if(!pSet) {
      fDetSetAr->AddAt(p,i);
      return kTRUE;
    }
  }
  fDetSetAr->Add(p);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HDetSetCont::addAt(HDetSet *p, Int_t idx)
//
// This function adds to the list of detector sets a new detector set at
// the position idx.
//
//---------------------------------------------------------------------

Bool_t HDetSetCont::addAt(HDetSet *p, Int_t idx) {

  if(!fDetSetAr) return kFALSE;
  if(!p) return kFALSE;
  if(idx < 0) return kFALSE;
  Int_t i;
  HDetSet *pSet;
  for(i=0; i < getEntries(); i++) {
    pSet = at(i);
    if(pSet) {
      if(!strcmp(pSet->GetName(),p->GetName())) return kFALSE;
    }
  }
  fDetSetAr->AddAt(p,idx);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HDetSetCont::removeAt(Int_t idx, Bool_t kSulSerio = kTRUE)
//
// This function removes from the list of detector sets the one with
// index idx. It also takes care of actual deleting the object in case
// the flag kSulSerio is true.
//
//---------------------------------------------------------------------

void HDetSetCont::removeAt(Int_t idx, Bool_t kSulSerio) {

  if(!fDetSetAr) return;
  if(idx < 0) return;
  HDetSet *pDel;
  pDel = (HDetSet*) fDetSetAr->RemoveAt(idx);
  if(kSulSerio) { 
    if(pDel) { delete pDel; pDel = 0; }
  }

}


//---------------------------------------------------------------------
//******  Int_t HDetSetCont::getEntries()
//
// This function returns number of entries in array fDetSetAr.
// Returns -1 if the array is not initialized.
//
//---------------------------------------------------------------------

Int_t HDetSetCont::getEntries() {

  if(!fDetSetAr) return -1;
  return (fDetSetAr->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HDetSet* HDetSetCont::at(Int_t idx)
//
// This function returns address of the detector set stored at position
// idx in fDetSetAr.
// Returns 0 in case fDetSetAr is not initialized or idx is out of bounds.
//
//---------------------------------------------------------------------

HDetSet* HDetSetCont::at(Int_t idx) {

  if(!fDetSetAr) return 0;
  if(idx < 0) return 0;
  return ((HDetSet*) fDetSetAr->At(idx));

}


//---------------------------------------------------------------------
//******  Int_t HDetSetCont::find(const Char_t* name)
//
// This function finds (in the list of detector sets) and returns index
// of the detector set. Input argument is the name of the detector set to
// be found. In case of no detector set found the function returns -1.
//
//---------------------------------------------------------------------

Int_t HDetSetCont::find(const Char_t* name) {

  if(!fDetSetAr) return -1;
  Int_t i;
  HDetSet *pSet;

  for(i=0; i < getEntries(); i++) {

    pSet = at(i);
    if(pSet) {
      if(!strcmp(name,pSet->GetName())) return i;
    }
  }

  return -1;

}








