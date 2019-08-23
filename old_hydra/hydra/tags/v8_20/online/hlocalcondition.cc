//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hlocalcondition.cc
// Subject:         Local conditions.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
// Remarks:         The concept of local conditions was initially proposed
//                  and developed by
//                  P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HLocalCondition.
// This class provides all functions to maintain the online objects of
// the type "local condition".
//
//-------------------------------------------------------------------------
using namespace std;
#include "hlocalcondition.h"
#include "honlinecontrol.h"
#include "hexpstring.h"
#include "hdetsetcont.h"
#include "hdetset.h"
#include "hparameter.h"
#include "hcalclocation.h"
#include "herror.h"

#include "hlocation.h"
#include "hindextransform.h"

#include <iostream> 
#include <iomanip>

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
// This class provides all functions to maintain the online objects of
// the type "local condition".
//
// The concept of local conditions was initially proposed and developed by
// P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//////////////////////////////////////////////////////////////////////

ClassImp(HLocalCondition)


//---------------------------------------------------------------------
//******  HLocalCondition::HLocalCondition()
//
// Constructor of the class HLocalCondition.
//
//---------------------------------------------------------------------

HLocalCondition::HLocalCondition() : TNamed() {
//
// Constructor of the class HLocalCondition.
//

  fExpression =  0;
  fContour    =  0;
  fExpList    =  0;
  fExpNumber  =  0;
  fContList   =  0;
  fContNumber =  0;
  fDetSet     = -2;
  fOut        =  0;
  fExpString  =  0;
  kAlwaysTrue =  kTRUE;
  kConsistent =  kFALSE;
  kEvaluated  =  kFALSE;

}


//---------------------------------------------------------------------
//******  HLocalCondition::~HLocalCondition()
//
// Destructor of the class HLocalCondition.
//
//---------------------------------------------------------------------

HLocalCondition::~HLocalCondition() {
//
// Destructor of the class HLocalCondition.
//

  if(fExpression) delete fExpression;
  if(fContour)    delete fContour;
  if(fExpList)    { fExpList->Delete(); delete fExpList; }
  if(fContList)   { fContList->Delete(); delete fContList; }
  if(fOut)        delete fOut;
  if(fExpString)  delete fExpString;

}


//---------------------------------------------------------------------
//******  Bool_t HLocalCondition::init()
//
// This function makes initialization of the local condition object.
//
//---------------------------------------------------------------------

Bool_t HLocalCondition::init() {
//
// This function makes initialization of the local condition object.
//

  if(fExpList) {
    delete fExpList;
    fExpList = 0;
  }
  if(fContList) {
    delete fContList;
    fContList = 0;
  }
  if(fOut) {
    delete fOut;
    fOut = 0;
  }
  fDetSet = -2;
  fExpList = new TList();
  fContList = new TList();
  fOut     = new HConditionOut();
  if(!fExpList || !fContList || !fOut) return kFALSE;
  if(fOut) {
    if(!fOut->init()) return kFALSE;
  }
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HLocalCondition::checkConsistency(Bool_t kMsg)
//
// This function makes a check that the object is consistent.
// Consistency means that all detector sets bound to this local
// condition exist and consistent in their turn.
// If kMsg is true the system does not simply perform the consistency
// check but also reports about found problems. 
//
//---------------------------------------------------------------------

Bool_t HLocalCondition::checkConsistency(Bool_t kMsg) {
//
// This function makes a check that the object is consistent.
// Consistency means that all detector sets bound to this local
// condition exist and are consistent in their turn.
// If kMsg is true the system does not simply perform the consistency
// check but also reports about found problems. 
//

  if(!gOnline) {
    if(kMsg) HError::message("Object of the main control class not found.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  if(!fExpression && !fContour && !kAlwaysTrue) {
    if(kMsg) HError::message("Expression and contour not found and condition is not always true.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  HDetSet *pSet = 0;
  Int_t categ = -1;

  //
  // make check of detector sets consistency
  //

  if(!gOnline->getDetSet()) {
    if(kMsg) HError::message("Container of detector sets not found.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  Int_t idx = -1;
  Char_t nullstr[100] = "";

  if(!strcmp(fDetSetName,nullstr)) {
    if(kMsg) HError::message("Detector set name not defined.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  idx = gOnline->getDetSet()->find(fDetSetName);
  if(idx != -1) fDetSet = idx;
  if(idx == -1 && fDetSet != -1) { // changed det set name
    pSet = gOnline->getDetSet()->at(fDetSet);
    if(pSet) setDetSet(pSet->GetName());
    else {
      kConsistent = kFALSE;
      return kFALSE;
    }
  }
  if(fDetSet == -1) {
    if(kMsg) HError::message("Detector set not found in the container."); 
    kConsistent = kFALSE;
    return kFALSE;
  }
  
  //
  // check if the name of the local condition is set
  //

  if(!strcmp(nullstr,GetName())) {
    if(kMsg) HError::message("Name of the local condition is not defined.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  //
  // check if in case of a contour category of the det set and parameters
  // is the same
  //

  if(fContour) {

    pSet = gOnline->getDetSet()->at(fDetSet);

    if(!pSet) {
      if(kMsg) HError::message("Detector set not found.");
      kConsistent = kFALSE;
      return kFALSE;
    }
    categ = pSet->getCategory();

    if(!fContList) {
      if(kMsg) HError::message("List of contour parameters not found.");
      kConsistent = kFALSE;
      return kFALSE;
    }

    HUserParameter *pPar1 = (HUserParameter*) fContList->First();
    HUserParameter *pPar2 = (HUserParameter*) fContList->After(pPar1);

    if(!pPar1 || !pPar2) {
      if(kMsg) HError::message("Contour parameters not found.");
      kConsistent = kFALSE;
      return kFALSE;
    }

    if((categ == -1) || (categ != pPar1->getCategory()) || 
       (categ != pPar2->getCategory())) {

      if(kMsg) HError::message("Detector set and contour parameters have different categories.");
      kConsistent = kFALSE;
      return kFALSE;
    }
  }

  kConsistent = kTRUE;
  return kTRUE;

}
 

//---------------------------------------------------------------------
//******  Bool_t HLocalCondition::setExpression(Int_t acateg)
//
// This function sets the expression for the evaluation and also
// takes care of calling the function which creates the list of parameters 
// needed for the expression.
//
//---------------------------------------------------------------------

Bool_t HLocalCondition::setExpression(Int_t acateg) {
//
// This function sets the expression for the evaluation and also
// takes care of calling the function which creates the list of parameters 
// needed for the expression.
//

  if(!fExpString) return kFALSE;
  if(fExpression) {
    delete fExpression;
    fExpression = 0;
    init();
  }

  Char_t nullstr[100] = "";
  Char_t expname[10] = "formula";
  if(!strcmp(nullstr,GetName())) 
    fExpression = new TFormula(expname,fExpString->exp);
  else fExpression = new TFormula(GetName(),fExpString->exp);

  if(!fExpression) return kFALSE;
  if(!setExpList(acateg)) return kFALSE;
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HLocalCondition::setExpList(Int_t acateg)
//
// This function creates a list of parameters bound to the expression.
//
//---------------------------------------------------------------------

Bool_t HLocalCondition::setExpList(Int_t acateg) {
//
// This function creates a list of parameters bound to the expression.
//

  if(!fExpString) return kFALSE;
  if(fExpList) fExpList->Delete();
  else fExpList = new TList();

  if(!fExpList) return kFALSE;
  fExpNumber = 0;

  HDefaultParameter *pParam;
  HDefaultParameter *pElement;
  TIterator *it = 0;
  Int_t i;

  for(i=0; i < (fExpString->parcur + 1); i++) {

    pParam = new HDefaultParameter();
    if(!pParam) return kFALSE;

    pParam->setCategory(acateg);
    pParam->setIndex((*fExpString).paridx[i] + 1);

    it = fExpList->MakeIterator();
    while((pElement = (HDefaultParameter*) it->Next())) {
      if(pParam->isEqual(pElement)) {
	delete pParam;
	pParam = 0;
	break;
      }
    }
    
    if(pParam) {
      fExpList->Add(pParam);
      fExpNumber++;
    }
  }
 
  return kTRUE;
}


//---------------------------------------------------------------------
//******  Bool_t HLocalCondition::setContList(HUserParameter *p1,
//                                            HUserParameter *p2)
//
// This function creates a list of parameters for the contour.
//
//---------------------------------------------------------------------

Bool_t HLocalCondition::setContList(HUserParameter *p1, 
				    HUserParameter *p2) {
//
// This function creates a list of parameters for the contour.
//

  if(fContList) { fContList->Delete(); delete fContList; fContList = 0; }
  fContList = new TList();
  if(!fContList) return kFALSE;

  fContNumber = 0;

  if(!p1 || !p2) return kFALSE;

  fContList->AddFirst(p1);
  fContNumber++;

  fContList->AddAfter(p1,p2);
  fContNumber++;

  return kTRUE;
}


//---------------------------------------------------------------------
//******  void HLocalCondition::setDetSet(const Char_t* detsetname)
//
// This function sets detector set for the condition.
//
//---------------------------------------------------------------------

void HLocalCondition::setDetSet(const Char_t* detsetname) {
//
// This function sets the detector set for the condition.
//

  if(detsetname) {
    sprintf(fDetSetName,"%s",detsetname);
    checkConsistency();
  }

}


//---------------------------------------------------------------------
//******  void HLocalCondition::setName(const Char_t* name)
//
// This function sets the name of the condition.
//
//---------------------------------------------------------------------

void HLocalCondition::SetName(const Char_t* name) {
//
// This function sets the name of the condition.
//

  fName = name;
  if(fExpression) fExpression->SetName(name);

}


//---------------------------------------------------------------
//******* void HLocalCondition::evaluate()
//
// This function evaluates the condition and stores in fOut the results
// of evaluation:
// 1. list of hits which passed the condition
// 2. condition counters
//
//---------------------------------------------------------------

void HLocalCondition::evaluate() {
//
// This function evaluates the condition and stores in fOut the results
// of evaluation:
// 1. list of hits which passed the condition
// 2. condition counters
//

  if(!kConsistent) return;

  if(!fOut) {
    fOut = new HConditionOut();
    if(!fOut) return;
    if(!fOut->init()) return;
  }

  fOut->clearList();
  fOut->clearCounts();

  if(!fExpression && !fContour && !kAlwaysTrue) return;

  HDetSet *pSet = gOnline->getDetSet()->at(fDetSet);
  if(!pSet) return;

  TList *pList = pSet->getOutput()->getList();
  if(!pList) return;

  TIterator *iter = pList->MakeIterator();
  TIterator *iter2 = 0;
  TIterator *it = 0;
  HLocation *pLoc;
  Double_t  *pArgu = 0;
  HDefaultParameter *pParam;
  Double_t result1 = 0.;
  Double_t result2 = 0.;
  Int_t *maxind = 0;
  HLocation *pIdx = 0;
  Int_t *idx = 0;
  Int_t detname = -1;

  
  if(kAlwaysTrue) fOut->setTrueFlag();

  while((pLoc = (HLocation*) iter->Next()) != NULL) {

    if(iter2) { delete iter2; iter2 = 0; }

    if(!kAlwaysTrue) {

      if(fExpression) {

	if(pArgu) {
	  delete [] pArgu;
	  pArgu = 0;
	}

	Int_t cate = pSet->getCategory();

	pArgu = new Double_t[HDefaultParameter::getMaxDefIdx(cate)];

	iter2 = fExpList->MakeIterator();

	while((pParam = (HDefaultParameter*) iter2->Next()) != NULL) {

       	  pArgu[pParam->getIndex() - 1] = (Double_t) (pParam
       	    ->getParameterValue(*pLoc));
	}	

      	result1 = fExpression->EvalPar(0,pArgu);

      }

      if(pArgu) {
	delete [] pArgu;
	pArgu = 0;
      }

      if(!fExpression) result1 = 1.;

      if(fContour) {

	HUserParameter *pPar1 = (HUserParameter*) fContList->First();
	HUserParameter *pPar2 = (HUserParameter*) fContList->After(pPar1);

	if(pPar1 && pPar2) {

	  if(fContour->isInside(pPar1->getParameterValue(*pLoc), 
				pPar2->getParameterValue(*pLoc))) result2 = 1.;
	  else result2 = 0.;
	}
      }

      if(!fContour) result2 = 1.;

      if((result1*result2) == 0.) {
	//	if(pLoc) { delete pLoc; pLoc = 0; }
	fOut->incCount(2);
      }
      if((result1*result2) == 1.) {
	fOut->incCount(1);
	fOut->getLocList()->Add(pLoc);
      }
	  
    }
    else {
      fOut->incCount(1);
      fOut->getLocList()->Add(pLoc);
    }
  }

  kEvaluated = kTRUE;

  if(iter) { delete iter; iter = 0; }
  if(iter2) { delete iter2; iter2 = 0; }

  detname = gOnline->getDetSet()->at(fDetSet)->getDetName();
  if(maxind) { delete [] maxind; maxind = 0; }

  pParam = new HDefaultParameter();
  if(!pParam) return;
  pParam->setCategory(gOnline->getDetSet()->at(fDetSet)->getCategory());


  idx = new Int_t[4];
  maxind = new Int_t[4];
  if(!idx || !maxind) {
    delete pParam;
    pParam = 0;
    return;
  }
  it = fOut->getLocList()->MakeIterator();

  switch(detname) {

  case TOF:
    maxind[0] = 6;
    maxind[1] = 8;
    maxind[2] = 8;

    while((pLoc = (HLocation*) it->Next()) != 0) {

      if(HIndexTransform::lin2mult(3,maxind,
	(Int_t) (pParam->getParameterValue(*pLoc)),idx)) {

	pIdx = new HLocation();
	if(!pIdx) break;

	pIdx->set(3,0,0,0);
	pIdx->operator [] (0) = idx[0];
	pIdx->operator [] (1) = idx[1];
	pIdx->operator [] (2) = idx[2];
	fOut->getIdxList()->Add(pIdx);
      }
    }
    break;

  case SHOWER:
    maxind[0] = 6;
    maxind[1] = 3;
    maxind[2] = 32;
    maxind[3] = 32;

    while((pLoc = (HLocation*) it->Next()) != 0) {

      if(HIndexTransform::lin2mult(4,maxind,
	(Int_t) (pParam->getParameterValue(*pLoc)),idx)) {

	pIdx = new HLocation();
	if(!pIdx) break;

	pIdx->set(4,0,0,0);
	pIdx->operator [] (0) = idx[0];
	pIdx->operator [] (1) = idx[1];
	pIdx->operator [] (2) = idx[2];
	pIdx->operator [] (3) = idx[3];

	fOut->getIdxList()->Add(pIdx);
      }
    }
    break;

  case MDC:
    maxind[0] = 6;
    maxind[1] = 4;
    maxind[2] = 6;
    maxind[3] = 200;

    while((pLoc = (HLocation*) it->Next()) != 0) {

      if(HIndexTransform::lin2mult(4,maxind,
	(Int_t) (pParam->getParameterValue(*pLoc)),idx)) {

	pIdx = new HLocation();
	if(!pIdx) break;

	pIdx->set(4,0,0,0);
	pIdx->operator [] (0) = idx[0];
	pIdx->operator [] (1) = idx[1];
	pIdx->operator [] (2) = idx[2];
	pIdx->operator [] (3) = idx[3];

	fOut->getIdxList()->Add(pIdx);
      }
    }
    break;

  case RICH:
    maxind[0] = 6;
    maxind[1] = 96;
    maxind[2] = 96;

    while((pLoc = (HLocation*) it->Next()) != 0) {

      if(HIndexTransform::lin2mult(3,maxind,
	(Int_t) (pParam->getParameterValue(*pLoc)),idx)) {

	pIdx = new HLocation();
	if(!pIdx) break;

	pIdx->set(3,0,0,0);
	pIdx->operator [] (0) = idx[0];
	pIdx->operator [] (1) = idx[1];
	pIdx->operator [] (2) = idx[2];

	fOut->getIdxList()->Add(pIdx);
      }
    }
    break;

  default:
    break;
  }

  if(maxind) { delete [] maxind; maxind = 0; }
  if(pParam) { delete pParam; pParam = 0; }
  if(idx) { delete [] idx; idx = 0; }
  if(it) { delete it; it = 0; }

}










