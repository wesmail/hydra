//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hcalclocation.cc
// Subject:         Evaluation of data hit locations for detector sets.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HCalcLocation.
// This class contains a set of static functions to be called whenever
// it is necessary to calculate a list of physical locations of data
// hits inside a particular category using the geometrical information
// from a detector set. This class is mainly used by detector sets.
//
//-------------------------------------------------------------------------

#include "hcalclocation.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hdataobject.h"
#include "hiterator.h"

#include "htofraw.h"
#include "htofhit.h"
#include "hshowerraw.h"
#include "hshowercal.h"
#include "hshowerhit.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "hrichhit.h"
#include "hmdccal1.h"
#include "hmdccal2.h"
#include "hmdchit.h"

#include "hdetset.h"

#include <iostream.h>

//////////////////////////////////////////////////////////////////////
// This class contains a set of static functions to be called whenever
// it is necessary to calculate a list of physical locations of data
// hits inside a particular category using the geometrical information
// from a detector set. This class is mainly used by detector sets.
/////////////////////////////////////////////////////////////////////

ClassImp(HCalcLocation)

//---------------------------------------------------------------------
//**  TList* HCalcLocation::transform(Int_t acat, TList* pList, Bool_t kill)
//
// This function calculates the list of physical locations inside category
// defined by acat, using pList as a list of geometrical locations. Right
// now it is used only by histogrammes when a detector set of a histogram 
// and a local condition is identical only in the geometrical part. 
// In the future this function will be probably replaced by a more efficient 
// one.
// If the flag kill is true the input list is destroyed. 
//
//---------------------------------------------------------------------

TList* HCalcLocation::transform(Int_t acat, TList *pList, Bool_t kill) {
//
// This function calculates the list of physical locations inside category
// defined by acat, using pList as a list of geometrical locations. Right
// now it is used only by histogrammes when a detector set of a histogram 
// and a local condition is identical only in the geometrical part. 
// In the future this function will be probably replaced by a more efficient 
// one.
// If the flag kill is true, the input list is destroyed. 
//

  if(!pList) return 0;
  if(!gHades) {
    if(kill) { pList->Delete(); delete pList; pList = 0; }
    return 0;
  }
  if(!gHades->getCurrentEvent()) {
    if(kill) { pList->Delete(); delete pList; pList = 0; }
    return 0;
  }

  HCategory *pCateg = gHades->getCurrentEvent()->getCategory(acat);
  if(!pCateg) {
    if(kill) { pList->Delete(); delete pList; pList = 0;}
    return 0;
  }

  HLocation *pLoc = 0;
  HLocation *pOutLoc = 0;
  HIterator *pIt = 0;
  TIterator *pIter = 0;

  HDataObject *pDataObj = 0;

  TList *pOutList = new TList();
  if(!pOutList) {
    if(kill) { pList->Delete(); delete pList; pList = 0; }
    return 0;
  }

  Int_t dim1 = -1;
  Int_t dim2 = -1;
  Int_t dim3 = -1;
  Int_t dim4 = -1;

  Bool_t kFound = kFALSE;

  if(!pIt) pIt = (HIterator*) pCateg->MakeIterator();
  pIt->Reset();

  while((pDataObj = (HDataObject*) pIt->Next()) != NULL) {

    kFound = kFALSE;
    if(!pIter) pIter = (TIterator*) pList->MakeIterator();
    pIter->Reset();

    while((pLoc = (HLocation*) pIter->Next()) != NULL) {

      switch(acat) {

      case 96: // catTofRaw

	dim1 = (Int_t) (((HTofRaw*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HTofRaw*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HTofRaw*) pDataObj) -> getCell());


	//	cout << dim1 << "  " << dim2 << "  " << dim3 << endl;

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 97: // catTofHit

	dim1 = (Int_t) (((HTofHit*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HTofHit*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HTofHit*) pDataObj) -> getCell());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 64: // catShowerRaw

	dim1 = (Int_t) (((HShowerRaw*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HShowerRaw*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HShowerRaw*) pDataObj) -> getRow());
	dim4 = (Int_t) (((HShowerRaw*) pDataObj) -> getCol());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2) && dim4==pLoc->getIndex(3)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 66: // catShowerCal

	dim1 = (Int_t) (((HShowerCal*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HShowerCal*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HShowerCal*) pDataObj) -> getRow());
	dim4 = (Int_t) (((HShowerCal*) pDataObj) -> getCol());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2) && dim4==pLoc->getIndex(3)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 65: // catShowerHit

	dim1 = (Int_t) (((HShowerHit*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HShowerHit*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HShowerHit*) pDataObj) -> getRow());
	dim4 = (Int_t) (((HShowerHit*) pDataObj) -> getCol());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2) && dim4==pLoc->getIndex(3)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 32: // catRichRaw

	dim1 = (Int_t) (((HRichRaw*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HRichRaw*) pDataObj) -> getRow());
	dim3 = (Int_t) (((HRichRaw*) pDataObj) -> getCol());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 34: // catRichCal

	dim1 = (Int_t) (((HRichCal*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HRichCal*) pDataObj) -> getRow());
	dim3 = (Int_t) (((HRichCal*) pDataObj) -> getCol());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 33: // catRichHit

	dim1 = (Int_t) (((HRichHit*) pDataObj) -> getSector());

	if(dim1==pLoc->getIndex(0)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 2: // catMdcCal1

	dim1 = (Int_t) (((HMdcCal1*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HMdcCal1*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HMdcCal1*) pDataObj) -> getLayer());
	dim4 = (Int_t) (((HMdcCal1*) pDataObj) -> getCell());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2) && dim4==pLoc->getIndex(3)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 4: // catMdcCal2

	dim1 = (Int_t) (((HMdcCal2*) pDataObj) -> getSector());
	dim2 = (Int_t) (((HMdcCal2*) pDataObj) -> getModule());
	dim3 = (Int_t) (((HMdcCal2*) pDataObj) -> getLayer());
	dim4 = (Int_t) (((HMdcCal2*) pDataObj) -> getCell());

	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1) &&
	   dim3==pLoc->getIndex(2) && dim4==pLoc->getIndex(3)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      case 1: // catMdcHit

	((HMdcHit*) pDataObj) -> getSecMod(dim1, dim2);
	//	dim1 = (Int_t) (((HMdcHit*) pDataObj) -> getSector());
	//	dim2 = (Int_t) (((HMdcHit*) pDataObj) -> getModule());

       	if(dim1==pLoc->getIndex(0) && dim2==pLoc->getIndex(1)) {

	  pOutLoc = new HLocation();
	  if(!pOutLoc) break;
	  (*pOutLoc) = pIt->getLocation();
	  pOutList->Add(pOutLoc);
	  kFound = kTRUE;
	}
	break;

      default:
	break;
      }

      if(kFound) break;
    }
  }

  if(pIt) { delete pIt; pIt = 0; }
  if(pIter) { delete pIter; pIter = 0; }
  if(kill) { pList->Delete(); delete pList; pList = 0; }

  return pOutList;
}


//---------------------------------------------------------------------
//******  TList* HCalcLocation::calcLocList(HDetSet *pSet)
//
// Obsolete function. No longer in use.
//
//---------------------------------------------------------------------

TList* HCalcLocation::calcLocList(HDetSet *pSet) {
//
// Obsolete function. No longer in use.
//

  if(!pSet) return 0;

  HLocation *pLoc = 0;
  Int_t *pIdx = new Int_t[4];
  if(!pIdx) return 0;
  Int_t n = 0;
  TList *pList = new TList();
  if(!pList) return 0;

  if(pSet->isMulti()) {

  }
  else {

    pLoc = new HLocation();
    if(!pLoc) {
      delete [] pIdx;
      pIdx = 0;
      return 0;
    }
    n = (pSet->getSingleAddress()).fN;
    pLoc->set(n);
    for(Int_t i=0; i<n; i++) 
      pLoc->operator [] (i) = (pSet->getSingleAddress()).operator [] (i);
    pList->Add(pLoc);
  }

  delete [] pIdx;
  pIdx = 0;

  return (transform(pSet->getCategory(),pList,kTRUE));

}


//---------------------------------------------------------------------
//******  void HCalcLocation::calcLoc(HDetSet *pSet)
//
// This function makes "evaluation" of the detector set and creates
// a list of physical locations for the category stored inside the detector
// set.
//
//---------------------------------------------------------------------

void HCalcLocation::calcLoc(HDetSet *pSet) {
//
// This function makes "evaluation" of the detector set and creates
// a list of physical locations for the category stored inside the detector
// set.
//


  if(!pSet) return;
  if(!pSet->getOutput()) return;
  if(!gHades) return;
  if(!gHades->getCurrentEvent()) return;

  Int_t acat = pSet->getCategory();

  HCategory *pCateg = gHades->getCurrentEvent()->getCategory(acat);
  if(!pCateg) return;

  TList *pOutList = pSet->getOutput()->getList();
  if(!pOutList) return;

  HLocation *pOutLoc = 0;
  HIterator *pIt = 0;

  HDataObject *pDataObj = 0;

  Int_t dim1 = -1;
  Int_t dim2 = -1;
  Int_t dim3 = -1;
  Int_t dim4 = -1;

  if(!pIt) pIt = (HIterator*) pCateg->MakeIterator();
  pIt->Reset();

  while((pDataObj = (HDataObject*) pIt->Next()) != NULL) {

    switch(acat) {

    case 96: // catTofRaw

      dim1 = (Int_t) (((HTofRaw*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HTofRaw*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HTofRaw*) pDataObj) -> getCell());

      if(pSet->belongs(dim1,dim2,dim3)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 97: // catTofHit

      dim1 = (Int_t) (((HTofHit*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HTofHit*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HTofHit*) pDataObj) -> getCell());

      if(pSet->belongs(dim1,dim2,dim3)) {
       	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 64: // catShowerRaw

      dim1 = (Int_t) (((HShowerRaw*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HShowerRaw*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HShowerRaw*) pDataObj) -> getRow());
      dim4 = (Int_t) (((HShowerRaw*) pDataObj) -> getCol());

      //      cout << dim1 << ":"<<dim2<<":"<<dim3<<":"<<dim4<<endl;

      if(pSet->belongs(dim1,dim2,dim3,dim4)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 66: // catShowerCal

      dim1 = (Int_t) (((HShowerCal*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HShowerCal*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HShowerCal*) pDataObj) -> getRow());
      dim4 = (Int_t) (((HShowerCal*) pDataObj) -> getCol());

      if(pSet->belongs(dim1,dim2,dim3,dim4)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;
      
    case 65: // catShowerHit

      dim1 = (Int_t) (((HShowerHit*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HShowerHit*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HShowerHit*) pDataObj) -> getRow());
      dim4 = (Int_t) (((HShowerHit*) pDataObj) -> getCol());


      if(pSet->belongs(dim1,dim2,dim3,dim4)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 32: // catRichRaw

      dim1 = (Int_t) (((HRichRaw*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HRichRaw*) pDataObj) -> getRow());
      dim3 = (Int_t) (((HRichRaw*) pDataObj) -> getCol());
      
      if(pSet->belongs(dim1,dim2,dim3)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 34: // catRichCal

      dim1 = (Int_t) (((HRichCal*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HRichCal*) pDataObj) -> getRow());
      dim3 = (Int_t) (((HRichCal*) pDataObj) -> getCol());

      if(pSet->belongs(dim1,dim2,dim3)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 33: // catRichHit

      dim1 = (Int_t) (((HRichHit*) pDataObj) -> getSector());

      if(pSet->belongs(dim1)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 2: // catMdcCal1

      dim1 = (Int_t) (((HMdcCal1*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HMdcCal1*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HMdcCal1*) pDataObj) -> getLayer());
      dim4 = (Int_t) (((HMdcCal1*) pDataObj) -> getCell());

      if(pSet->belongs(dim1,dim2,dim3)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 4: // catMdcCal2

      dim1 = (Int_t) (((HMdcCal2*) pDataObj) -> getSector());
      dim2 = (Int_t) (((HMdcCal2*) pDataObj) -> getModule());
      dim3 = (Int_t) (((HMdcCal2*) pDataObj) -> getLayer());
      dim4 = (Int_t) (((HMdcCal2*) pDataObj) -> getCell());

      if(pSet->belongs(dim1,dim2,dim3,dim4)) {
	pOutLoc = new HLocation();
	if(!pOutLoc) break;
	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;

    case 1: // catMdcHit
      
      ((HMdcHit*) pDataObj) -> getSecMod(dim1, dim2);
      //      dim1 = (Int_t) (((HMdcXY*) pDataObj) -> getSector());
      //      dim2 = (Int_t) (((HMdcXY*) pDataObj) -> getModule());

      if(pSet->belongs(dim1,dim2)) {
       	pOutLoc = new HLocation();
	if(!pOutLoc) break;
       	(*pOutLoc) = pIt->getLocation();
	pOutList->Add(pOutLoc);
      }
      break;
      
    default:
      break;
    }
  }

  if(pIt) { delete pIt; pIt = 0; }

}














