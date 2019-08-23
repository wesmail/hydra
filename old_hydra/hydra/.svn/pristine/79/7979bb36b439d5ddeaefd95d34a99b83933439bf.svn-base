#include "hhistocont.h"
#include "hhistogram.h"
#include "herror.h"
#include "honlinecontrol.h"
#include "hconsistency.h"

//-------------------------------------------------------------------------
//
// File name:       hhistocont.cc
// Subject:         Container of histogrammes.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HHistoCont.
// This class serves as a container of histogrammes.
// All histogrammes are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

ClassImp(HHistoCont)


//---------------------------------------------------------------------
//******  HHistoCont::HHistoCont()
//
// Constructor of the class HHistoCont.
//
//---------------------------------------------------------------------

HHistoCont::HHistoCont() : TObject() {

  fHistoAr = 0;

}


//---------------------------------------------------------------------
//******  HHistoCont::~HHistoCont()
//
// Destructor of the class HHistoCont.
//
//---------------------------------------------------------------------

HHistoCont::~HHistoCont() {

  if(fHistoAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fHistoAr;
  }

}


//---------------------------------------------------------------------
//******  Bool_t HHistoCont::init()
//
// This function makes initialization of array fHistoAr (actual container
// of histogrammes).
//
//---------------------------------------------------------------------

Bool_t HHistoCont::init() {

  if(fHistoAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fHistoAr;
    fHistoAr = 0;

  }
  fHistoAr = new TObjArray(500);
  if(!fHistoAr) return kFALSE;
  return kTRUE;

}  


//---------------------------------------------------------------------
//******  Bool_t HHistoCont::add(HHistogram *p)
//
// This function adds to the list of histogrammes a new histogram at
// the next empty slot.
// 
//---------------------------------------------------------------------

Bool_t HHistoCont::add(HHistogram *p) {

  if(!fHistoAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  Int_t i;
  HHistogram *pHisto;
  for(i=0; i < getEntries(); i++) {
    pHisto = at(i);
    if(pHisto) {
      if(!strcmp(pHisto->GetName(),p->GetName())) return kFALSE;
    }
  }
  for(i=0; i < getEntries(); i++) {
    pHisto = at(i);
    if(!pHisto) {
      fHistoAr->AddAt(p,i);
      return kTRUE;
    }
  }
  fHistoAr->Add(p);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HHistoCont::addAt(HHistogram *p, Int_t idx)
//
// This function adds to the list of histogrammes a new histogram at
// the position idx.
// 
//---------------------------------------------------------------------

Bool_t HHistoCont::addAt(HHistogram *p, Int_t idx) {

  if(!fHistoAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  if(idx < 0) return kFALSE;
  Int_t i;
  HHistogram *pHisto;
  for(i=0; i < getEntries(); i++) {
    pHisto = at(i);
    if(pHisto) {
      if(!strcmp(pHisto->GetName(),p->GetName())) return kFALSE;
    }
  }
  fHistoAr->AddAt(p,idx);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HHistoCont::removeAt(Int_t idx, Bool_t kSulSerio = kTRUE)
//
// This function removes from the list of histogrammes the one with
// index idx. It also takes care of actual deleting the object in case
// the flaf kSulSerio is true.
// 
//---------------------------------------------------------------------

void HHistoCont::removeAt(Int_t idx, Bool_t kSulSerio) {

  if(!fHistoAr) {
    return;
  }
  if(idx < 0) return;
  HHistogram *pDel;
  pDel = (HHistogram*) fHistoAr->RemoveAt(idx);
  if(kSulSerio) {
    if(pDel) { delete pDel; pDel = 0; }
  }
}


//---------------------------------------------------------------------
//******  Int_t HHistoCont::getEntries()
//
// This function returns number of entries in array fHistoAr.
// Returns -1 if the array is not initialized.
//
//---------------------------------------------------------------------

Int_t HHistoCont::getEntries() {

  if(!fHistoAr) {
    return -1;
  }
  return (fHistoAr->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HHistogram* HHistoCont::at(Int_t idx)
//
// This function returns address of the histogram stored at position
// idx in fHistoAr.
// Returns 0 in case fHistoAr is not initialized or idx is out of bounds.
//
//---------------------------------------------------------------------

HHistogram* HHistoCont::at(Int_t idx) {

  if(!fHistoAr) {
    return 0;
  }
  if(idx < 0) return 0;
  return ((HHistogram*) fHistoAr->At(idx));

}


//---------------------------------------------------------------------
//******  Int_t HHistoCont::find(const Char_t* name)
//
// This function finds (in the list of histogrammes) and returns index 
// of the histogram. Input argument is the name of the histogram to
// be found. In case of no histogram found the function returns -1.
// 
//---------------------------------------------------------------------

Int_t HHistoCont::find(const Char_t* name) {

  if(!fHistoAr) {
    return -1;
  }
  Int_t i;
  HHistogram *pHisto;

  for(i=0; i < getEntries(); i++) {

    pHisto = at(i);
    if(pHisto) {
      if(!strcmp(name,pHisto->GetName())) return i;
    }
  }

  return -1;

}


//---------------------------------------------------------------------
//******  void HHistoCont::checkConsistency()
//
// This function calls checkConsistency() functions of all histogrammes
// that are stored in the list fHisto.
// All found inconsistent histogrammes are stored in array fHiIdx of
// the class HConsistency (fConsistency).
//
//---------------------------------------------------------------------

void HHistoCont::checkConsistency() {

  if(!gOnline) return (HError::message("gOnline not initialized"));
  if(!gOnline->getConsistency()) 
    return (HError::message("fConsistency not initialized"));
  if(!fHistoAr) 
    return (HError::message("Container of histogrammes not initialized"));

  HHistogram *pHisto;

  for(Int_t i=0; i<getEntries(); i++) {

    pHisto = at(i);
    if(pHisto) {
      if(!pHisto->checkConsistency()) 
	(*(gOnline->getConsistency())).fHiIdx[i] = 1;
      else (*(gOnline->getConsistency())).fHiIdx[i] = 0;
    }
  }

}  








