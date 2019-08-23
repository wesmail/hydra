//-------------------------------------------------------------------------
//
// File name:       hconsistency.cc
// Subject:         Online objects consistency check.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HConsistency.
// This class contains functions to perform consistency check for any online
// object of the HTOP. That means for example that if there is a histogram
// bound to a condition the condition must exist otherwise that histogram
// will be declared inconsistent and during event loop no action will be
// applied to it.
//
//-------------------------------------------------------------------------

#include "hconsistency.h"
#include "herror.h"
#include "honlinecontrol.h"
#include "hlocalcont.h"
#include "hglobalcont.h"
#include "hhistocont.h"
#include "hconsistencywarn.h"
#include <iostream.h>

ClassImp(HConsistency)


//---------------------------------------------------------------------
//******  HConsistency::HConsistency()
//
// Constructor of the class HConsistency.
//
//---------------------------------------------------------------------

HConsistency::HConsistency() : TObject() {

  fLCIdx.Set(500);
  fGCIdx.Set(500);
  fHiIdx.Set(500);

  clear();

}


//---------------------------------------------------------------------
//******  void HConsistency::clear()
//
// This function clears arrays of indexes of inconsistent objects.
//
//---------------------------------------------------------------------

void HConsistency::clear() {

  Int_t i;
  for(i=0; i<fLCIdx.fN; i++) fLCIdx[i] = 0;
  for(i=0; i<fGCIdx.fN; i++) fGCIdx[i] = 0;
  for(i=0; i<fHiIdx.fN; i++) fHiIdx[i] = 0;

  kConsistent = kTRUE;

}


//---------------------------------------------------------------------
//******  void HConsistency::checkConsistency()
//
// This function calls checkConsistency functions of containers of local
// and global conditions and histogrammes.
//
//---------------------------------------------------------------------

void HConsistency::checkConsistency() {

  //  cout << "HConsistency::checkConsistency()" << endl;

  if(!gOnline) return;
  clear();
  gOnline->getLocal()->checkConsistency();
  gOnline->getGlobal()->checkConsistency();
  gOnline->getHisto()->checkConsistency();

  for(Int_t i=0; i<fLCIdx.fN; i++) {
    if(fLCIdx[i] != 0) {
      kConsistent = kFALSE;
      return;
    }
  }

  for(Int_t i=0; i<fGCIdx.fN; i++) {
    if(fGCIdx[i] != 0) {
      kConsistent = kFALSE;
      return;
    }
  }

  for(Int_t i=0; i<fHiIdx.fN; i++) {
    if(fHiIdx[i] != 0) {
      kConsistent = kFALSE;
      return;
    }
  }

}


//---------------------------------------------------------------------
//******  void HConsistency::delInc()
//
// This function removes all found inconsistent objects.
//
//---------------------------------------------------------------------

void HConsistency::delInc() {

  if(!gOnline) return;
  Int_t i;

  for(i=0; i<fLCIdx.fN; i++) {
    if(fLCIdx[i] == 1) {
      gOnline->getLocal()->removeAt(i);
    }
  }
  for(i=0; i<fGCIdx.fN; i++) {
    if(fGCIdx[i] == 1) {
      gOnline->getGlobal()->removeAt(i);
    }
  }
  for(i=0; i<fHiIdx.fN; i++) {
    if(fHiIdx[i] == 1) {
      gOnline->getHisto()->removeAt(i);
    }
  }

}


//---------------------------------------------------------------------
//******  void HConsistency::warnPanel()
//
// This function calls a warning panel to inform the user about the presence
// of inconsistent objects in the program.
//
//---------------------------------------------------------------------

void HConsistency::warnPanel() {

  Int_t retval;
  new HConsistencyWarn(gClient->GetRoot(),1,1,&retval);
  if(retval == 1) delInc();

}


//---------------------------------------------------------------------
//******  void HConsistency::action()
//
// This function is called from the main control class HOnlineControl to
// perform consistency check of all existing HTOP objects.
//
//---------------------------------------------------------------------

void HConsistency::action() {

  checkConsistency();
  if(kConsistent) return;
  warnPanel();
  checkConsistency();

}




