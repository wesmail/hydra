//-------------------------------------------------------------------------
//
// File name:       honlinecontrol.cc
// Subject:         Main control class of HTOP
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it" 
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code of the member functions of the main 
// control class HOnlineControl. This class maintains containers of detector 
// sets, histogrammes, local and global conditions, and contours. It also
// contains pointers to the objects of classes HConsistency and HMemoryCheck
// which are responsible of performing control that all run-time defined
// objects are self consistent and do not exceed memory limit defined by
// a user for a particular analysis session. 
// Only one object of this class can be created during the session. This
// object can be always addressed by means of a global pointer gOnline.
//
//-------------------------------------------------------------------------

#include "honlinecontrol.h"
#include "hdetsetcont.h"
#include "hlocalcont.h"
#include "hglobalcont.h"
#include "hhistocont.h"
#include "hcontourcont.h"
#include "hconsistency.h"
#include "hmemorycheck.h"
#include "herror.h"

#include <TFile.h>
#include <TKey.h>
#include <TCollection.h>

ClassImp(HOnlineControl)

//---------------------------------------------------------------------
//******  HOnlineControl::HOnlineControl()
//
// Constructor of the class HOnlineControl.
//
//---------------------------------------------------------------------

HOnlineControl::HOnlineControl() : TObject() {

  if(gOnline) {
    delete gOnline;
    gOnline = 0;
  }
  fDetSet      = 0;
  fLocal       = 0;
  fGlobal      = 0;
  fHisto       = 0;
  fContour     = 0;
  fConsistency = 0;
  fMemCheck    = 0;
  setSaveFile("honline.root");
  gOnline = this;

}


//---------------------------------------------------------------------
//******  HOnlineControl::~HOnlineControl()
//
// Destructor of the class HOnlineControl.
//
//---------------------------------------------------------------------

HOnlineControl::~HOnlineControl() {

  write();
  if(fDetSet)      delete fDetSet;
  if(fLocal)       delete fLocal;
  if(fGlobal)      delete fGlobal;
  if(fHisto)       delete fHisto;
  if(fContour)     delete fContour;
  if(fConsistency) delete fConsistency;
  if(fMemCheck)    delete fMemCheck;

}


//---------------------------------------------------------------------
//******  Bool_t HOnlineControl::read()
//
// This function tries to find and read the file 'honline.root' which must 
// contain the last saved configuration of the HTOP run. In case of a fail it
// calls init() function for default initialization.
//
//---------------------------------------------------------------------

Bool_t HOnlineControl::read() {

  TFile *pFile = new TFile("honline.root");
  if(!pFile) return init();
  if(pFile->IsZombie()) {
      delete pFile;
      pFile = 0;
      return init();
  }

  TIter it(pFile->GetListOfKeys());
  TKey *key;

  while((key = (TKey*) it())) {
    if(!strcmp(key->GetClassName(),"HDetSetCont")) 
      fDetSet = (HDetSetCont*) key->ReadObj();

    if(!strcmp(key->GetClassName(),"HLocalCont"))
      fLocal = (HLocalCont*) key->ReadObj();

    if(!strcmp(key->GetClassName(),"HGlobalCont"))
      fGlobal = (HGlobalCont*) key->ReadObj();

    if(!strcmp(key->GetClassName(),"HHistoCont"))
      fHisto = (HHistoCont*) key->ReadObj();

    if(!strcmp(key->GetClassName(),"HContourCont"))
      fContour = (HContourCont*) key->ReadObj();
  }

  if(!fConsistency) fConsistency = new HConsistency();
  if(!fMemCheck) fMemCheck = new HMemoryCheck();
  if(!fDetSet || !fLocal || !fGlobal || !fHisto || !fContour ||
     !fConsistency || !fMemCheck) 
    return kFALSE;

  fConsistency->action();
  if(!fConsistency->isConsistent()) return kFALSE;

  fMemCheck->check();
    
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HOnlineControl::write()
//
// This function writes containers of detector sets, histogrammes, local and
// global conditions, and contours to the file with the name 'savefile', which
// is user defined at run time. 
//
//---------------------------------------------------------------------

void HOnlineControl::write() {

  TFile *pFile = new TFile(savefile,"RECREATE");
  if(!pFile) return;

  if(!fDetSet || !fLocal || !fGlobal || !fHisto || !fContour ||!fConsistency) {
    delete pFile;
    pFile = 0;
    return;
  }

  fConsistency->action();
  
  if(!fConsistency->isConsistent()) {
    delete pFile;
    pFile = 0;
    return;
  }

  fDetSet->Write();
  fLocal ->Write();
  fGlobal->Write();
  fHisto ->Write();
  fContour->Write();

}


//---------------------------------------------------------------------
//******  Bool_t HOnlineControl::init()
//
// This function makes default initialization of the HTOP stuff.
//
//---------------------------------------------------------------------

Bool_t HOnlineControl::init() {

  if(fDetSet) {
    delete fDetSet;
    fDetSet = 0;
  }
  fDetSet = new HDetSetCont();
  if(!fDetSet->init()) return kFALSE;

  if(fLocal) {
    delete fLocal;
    fLocal = 0;
  }
  fLocal = new HLocalCont();
  if(!fLocal->init()) return kFALSE;

  if(fHisto) {
    delete fHisto;
    fHisto = 0;
  }
  fHisto = new HHistoCont();
  if(!fHisto->init()) return kFALSE;

  if(fContour) {
    delete fContour;
    fContour = 0;
  }
  fContour = new HContourCont();
  if(!fContour->init()) return kFALSE;

  if(fGlobal) {
    delete fGlobal;
    fGlobal = 0;
  }
  fGlobal = new HGlobalCont();
  if(!fGlobal->init()) return kFALSE;

  if(fMemCheck) {
    delete fMemCheck;
    fMemCheck = 0;
  }

  fMemCheck = new HMemoryCheck();
  if(!fMemCheck) return kFALSE;

  if(fConsistency) {
    delete fConsistency;
    fConsistency = 0;
  }

  fConsistency = new HConsistency();
  if(!fConsistency) return kFALSE;

  fConsistency->action();
  if(!fConsistency->isConsistent()) return kFALSE;

  return kTRUE;

}


HOnlineControl *gOnline;






