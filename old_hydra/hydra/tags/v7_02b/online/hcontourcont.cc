//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hcontourcont.cc
// Subject:         Container of contours.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HContourCont.
// This class stores all runtime defined contours.
//
//-------------------------------------------------------------------------
using namespace std;
#include "hcontourcont.h"
#include "hcontour.h"
#include "herror.h"
#include "honlinecontrol.h"
#include "hlocalcont.h"
#include "hlocalcondition.h"
#include "hglobalcont.h"
#include "hglobalcondition.h"

#include <iostream> 
#include <iomanip>

///////////////////////////////////////////////////////////////
// This class stores all runtime defined contours.
///////////////////////////////////////////////////////////////
ClassImp(HContourCont)


//---------------------------------------------------------------------
//******  HContourCont::HcontourCont()
//
// Constructor of the class HContourCont.
//
//---------------------------------------------------------------------

HContourCont::HContourCont() : TObject() {
//
// Constructor of the class HContourCont.
//

  fContourAr = 0;

}


//---------------------------------------------------------------------
//******  HContourCont::~HContourCont()
//
// Destructor of the class HContourCont.
//
//---------------------------------------------------------------------

HContourCont::~HContourCont() {
//
// Destructor of the class HContourCont.
//

  if(fContourAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fContourAr;
  }

}


//---------------------------------------------------------------------
//******  Bool_t HContourCont::init()
//
// This function makes initialization of array fContourAr (actual container
// of contours).
//
//---------------------------------------------------------------------

Bool_t HContourCont::init() {
//
// This function makes initialization of array fContourAr (actual container
// of contours).
//

  if(fContourAr) {

    for(Int_t i=0; i<getEntries(); i++) removeAt(i);
    delete fContourAr;
    fContourAr = 0;

  }
  fContourAr = new TObjArray(500);
  if(!fContourAr) return kFALSE;
  return kTRUE;

}  


//---------------------------------------------------------------------
//******  Bool_t HContourCont::add(HContour *p)
//
// This function adds to the list of contours a new contour.
// 
//---------------------------------------------------------------------

Bool_t HContourCont::add(HContour *p) {
//
// This function adds to the list of contours a new contour.
// 

  if(!fContourAr) {
    return kFALSE;
  }
  if(!p) return kFALSE;
  Int_t i;
  HContour *pContour;
  for(i=0; i < getEntries(); i++) {
    pContour = at(i);
    if(pContour) {
      if(!strcmp(pContour->GetName(),p->GetName())) {
	HError::message("Contour with this name already exists.");
	return kFALSE;
      }
    }
  }
  fContourAr->Add(p);
  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HContourCont::removeAt(Int_t idx)
//
// This function removes from the list of contours the one with
// index idx. It also takes care of actual deleting the object.
// If there is some condition using this contour, the contour is not deleted.
// 
//---------------------------------------------------------------------

void HContourCont::removeAt(Int_t idx) {
//
// This function removes from the list of contours the one with
// index idx. It also takes care of actual deleting the object.
// If there is some condition using this contour, the contour is not deleted.
// 

  if(!fContourAr) {
    return;
  }
  if(idx < 0) return;
  HContour *pDel;
  pDel = at(idx);

  HLocalCondition *pLocal = 0;
  HGlobalCondition *pGlobal = 0;

  if(gOnline) {

    for(Int_t i=0; i<gOnline->getLocal()->getEntries(); i++) {

      pLocal = gOnline->getLocal()->at(i);
      if(pLocal && pLocal->getContour()) {
	if(!strcmp(pDel->GetName(),pLocal->getContour()->GetName())) {
	  HError::message("Found condition using this contour. \
Remove operation refused.");
	  return;
	}
      }
    }

    for(Int_t i=0; i<gOnline->getGlobal()->getEntries(); i++) {

      pGlobal = gOnline->getGlobal()->at(i);
      if(pGlobal && pGlobal->getContour()) {
	if(!strcmp(pDel->GetName(),pGlobal->getContour()->GetName())) {
	  HError::message("Found condition using this contour. \
Remove operation refused.");
	  return;
	}
      }
    }
  }
  fContourAr->RemoveAt(idx);
  if(pDel) { delete pDel; pDel = 0; }
  compress();

}



//---------------------------------------------------------------------
//******  Int_t HContourCont::getEntries()
//
// This function returns number of entries in array fContourAr.
// Returns -1 if the array is not initialized.
//
//---------------------------------------------------------------------

Int_t HContourCont::getEntries() {
//
// This function returns number of entries in array fContourAr.
// Returns -1 if the array is not initialized.
//

  if(!fContourAr) {
    return -1;
  }
  return (fContourAr->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HContour* HContourCont::at(Int_t idx)
//
// This function returns address of the contour stored at position
// idx in fContourAr.
// Returns 0 in case fContourAr is not initialized or idx is out of bounds.
//
//---------------------------------------------------------------------

HContour* HContourCont::at(Int_t idx) {
//
// This function returns address of the contour stored at position
// idx in fContourAr.
// Returns 0 in case fContourAr is not initialized or idx is out of bounds.
//

  if(!fContourAr) {
    return 0;
  }
  if(idx < 0) return 0;
  return ((HContour*) fContourAr->At(idx));

}


//---------------------------------------------------------------------
//******  Int_t HContourCont::find(const Char_t* name)
//
// This function finds (in the list of contours) and returns index 
// of the contour. Input argument is the name of the contour to
// be found. In case of no contour found the function returns -1.
// 
//---------------------------------------------------------------------

Int_t HContourCont::find(const Char_t* name) {
//
// This function finds (in the list of contours) and returns index 
// of the contour. Input argument is the name of the contour to
// be found. In case of no contour found the function returns -1.
// 

  if(!fContourAr) {
    return -1;
  }
  Int_t i;
  HContour *pContour;

  for(i=0; i < getEntries(); i++) {

    pContour = at(i);
    if(pContour) {
      if(!strcmp(name,pContour->GetName())) return i;
    }
  }

  return -1;

}








