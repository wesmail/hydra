//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/


//-------------------------------------------------------------------------
//
// File name:       hglobalcondition.cc
// Subject:         Global conditions.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
// Remarks:         The concept of global conditions was initially proposed
//                  and developed by
//                  P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of the classes 
// HNameIdx and HGlobalCondition.
// The HGlobalCondition class provides a set of functions to maintain 
// the online objects of the type "global condition".
// The HNameIdx class is a service class for HGlobalCondition.
//
// Global conditions operate with counters of local conditions. Each local
// condition contains 3 counters:
// count1 - condition was true
// count2 - condition was false
// count3 = count1 + count2
//
// The counters of all local conditions have their "id". It is calculated
// like
// count1 = idx*3
// count2 = idx*3 + 1
// count3 = idx*3 + 2
// where idx is the index of the appropriate local condition in the container
// of local conditions.
//
//-------------------------------------------------------------------------

#include "hglobalcondition.h"
#include "honlinecontrol.h"
#include "hcontour.h"
#include "hexpstring.h"
#include "hlocalcondition.h"
#include "hlocalcont.h"
#include "hconditionout.h"
#include "hdetset.h"
#include "hdetsetcont.h"
#include "hparameter.h"
#include "hcalclocation.h"
#include "herror.h"

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
//
// The HGlobalCondition class provides a set of functions to maintain 
// the online objects of the type "global condition".
// The HNameIdx class is a service class for HGlobalCondition.
//
// Global conditions operate with counters of local conditions. Each local
// condition contains 3 counters:
//
// count1 - condition was true
// count2 - condition was false
// count3 = count1 + count2
//
// The counters of all local conditions have their "id". It is calculated
// like
// 
// count1 = idx*3
// count2 = idx*3 + 1
// count3 = idx*3 + 2
//
// where idx is the index of the appropriate local condition in the container
// of local conditions.
//
// The concept of global conditions was initially proposed and developed by
// P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
///////////////////////////////////////////////////////////////////////////

ClassImp(HGlobalCondition)
ClassImp(HNameIdx)


//---------------------------------------------------------------------
//******  HGlobalCondition::HGlobalCondition()
//
// Constructor of the class HGlobalCondition.
//
//---------------------------------------------------------------------

HGlobalCondition::HGlobalCondition() : TNamed() {
//
// Constructor of the class HGlobalCondition.
//

  fLCList     = 0;
  fNumber     = 0;
  fExpression = 0;
  fContour    = 0;
  fExpString  = 0;
  fContParam  = 0;
  kEvalResult = kFALSE;
  kConsistent = kFALSE;
  fMaxParIdx  = -1;
  for(Int_t i=0; i<2; i++) fContDS[i] = -1;

}


//---------------------------------------------------------------------
//******  HGlobalCondition::~HGlobalCondition()
//
// Destructor of the class HGlobalCondition.
//
//---------------------------------------------------------------------

HGlobalCondition::~HGlobalCondition() {
//
// Destructor of the class HGlobalCondition.
//

  if(fLCList) { fLCList->Delete(); delete fLCList; }
  if(fExpression) delete fExpression;
  if(fContour) delete fContour;
  if(fExpString) delete fExpString;
  if(fContParam) { fContParam->Delete(); delete fContParam; }

}


//---------------------------------------------------------------------
//******  void HGlobalCondition::evaluate()
//
// This function evaluates condition. In general, the procedure of 
// evaluation consists of two steps:
// 1) evaluation of expression (if any)
// 2) evaluation of contour (if any)
// The result of evaluation is 
// kExpressionResult && kContourResult
//
// The result of contour evaluation is true if at least one point from all
// possible combinations of pairs ( detset1 = xaxis, detset2 = yaxis) lies
// inside the contour.
//
//---------------------------------------------------------------------

void HGlobalCondition::evaluate() {
//
// This function evaluates condition. In general, the procedure of 
// evaluation consists of two steps:
// 1) evaluation of expression (if any)
// 2) evaluation of contour (if any)
// The result of evaluation is 
// kExpressionResult && kContourResult
//
// The result of contour evaluation is true if at least one point from all
// possible combinations of pairs ( detset1 = xaxis, detset2 = yaxis) lies
// inside the contour.
//

  Bool_t kOK = kTRUE;
  Bool_t kExp = kFALSE;
  Bool_t kCont = kFALSE;
  HNameIdx *pNameIdx = 0;
  HConditionOut *pOut = 0;
  Double_t result = -1.;
  Double_t *pArgu = 0;

  kEvalResult = kExp && kCont;
  
  if(!kConsistent) return;
  
  if(!fExpression && !fContour) return;

  if(!fContour) kCont = kTRUE;
  if(!fExpression) kExp = kTRUE;

  if(fExpression) {

    if(fMaxParIdx == -1) calcMaxParIdx();
    if(fMaxParIdx == -1) return;

    pArgu = new Double_t[fMaxParIdx + 1];
    if(!pArgu) return;

    TIter iter(fLCList);
    while((pNameIdx = (HNameIdx*) iter())) {

      if(!gOnline->getLocal()->at(pNameIdx->fLCIndex)->isEvaluated()) {
	kOK = kFALSE;
	break;
      }
      pOut = gOnline->getLocal()->at(pNameIdx->fLCIndex)->getOutput();
      if(pOut) {

	pArgu[pNameIdx->fLCTrueIdx]     = (Double_t) (pOut->getCount(1));
	pArgu[pNameIdx->fLCTrueIdx + 1] = (Double_t) (pOut->getCount(2));
	pArgu[pNameIdx->fLCTrueIdx + 2] = (Double_t) (pOut->getCount(3));

      }
    }
    if(!kOK) {
      if(pArgu) {
	delete [] pArgu;
	pArgu = 0;
      }
      return;
    }
    result = fExpression->EvalPar(0,pArgu);
    if(pArgu) {
      delete [] pArgu;
      pArgu = 0;
    }
    if(result == 1.) kExp = kTRUE;
    else {
      kEvalResult = kFALSE;
      return;
    }

  }

  if(fContour) {

    HDetSet *pSet1 = gOnline->getDetSet()->at(fContDS[0]);
    HDetSet *pSet2 = gOnline->getDetSet()->at(fContDS[1]);

    if(!pSet1 || !pSet2) {
      kEvalResult = kFALSE;
      return;
    }

    TList *pList1 = 0;
    TList *pList2 = 0;

    pList1 = pSet1->getOutput()->getList();
    pList2 = pSet2->getOutput()->getList();

    //    pList1 = HCalcLocation::calcLocList(pSet1);
    //    pList2 = HCalcLocation::calcLocList(pSet2);

    if(!pList1 || !pList2) {

      kEvalResult = kFALSE;
      return;
    }

    HUserParameter *pPar1 = (HUserParameter*) fContParam->First();
    HUserParameter *pPar2 = (HUserParameter*) fContParam->After(pPar1);

    if(!pPar1 || !pPar2) {
      kEvalResult = kFALSE;
      return;
    }

    TIter iter1(pList1);
    TIter iter2(pList2);

    HLocation *pLoc1 = 0;
    HLocation *pLoc2 = 0;

    while((pLoc1 = (HLocation*) iter1())) {

      while((pLoc2 = (HLocation*) iter2())) {

	if(fContour->isInside(pPar1->getParameterValue(*pLoc1), 
			      pPar2->getParameterValue(*pLoc2))) {

	  kCont = kTRUE;
	  break;
	}
      }

      if(kCont) break;
    }
    //    if(pList1) { pList1->Delete(); delete pList1; pList1 = 0; }
    //    if(pList2) { pList2->Delete(); delete pList2; pList2 = 0; }
  }

  kEvalResult = kExp && kCont;

  return;

}



//---------------------------------------------------------------------
//******  Bool_t HGlobalCondition::checkConsistency(Bool_t kMsg = kFALSE)
//
// This function makes consistency check. This means control that all indexes 
// of local conditions and detector sets which are used in this condition are 
// valid.
//
//---------------------------------------------------------------------

Bool_t HGlobalCondition::checkConsistency(Bool_t kMsg) {
//
// This function makes consistency check. This means control that all indexes 
// of local conditions and detector sets which are used in this condition are 
// valid.
//

  if(!gOnline) {
    if(kMsg) HError::message("Object of the main control class not found.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  if(!fExpression && !fContour) {
    if(kMsg) HError::message("Expression and contour not defined.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  if(!gOnline->getLocal()) {
    if(kMsg) HError::message("Container of local conditions not found.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  HDetSet *pSet = 0;
  Int_t idx = -1;

  if(fExpression) {

    if(!setLCList()) {
      kConsistent = kFALSE;
      return kFALSE;
    }

  }

  if(fContour) {

    if(!gOnline->getDetSet()) {
      if(kMsg) HError::message("Container of detector sets not found.");
      kConsistent = kFALSE;
      return kFALSE;
    }

    idx = gOnline->getDetSet()->find(fDS1Name);
    if(idx == -1) {
      if(kMsg) HError::message("1st detector set not found in the container.");
      kConsistent = kFALSE;
      return kFALSE;
    }
    if(fContDS[0] != idx) fContDS[0] = idx;

    idx = gOnline->getDetSet()->find(fDS2Name);
    if(idx == -1) {
      if(kMsg) HError::message("2nd detector set not found in the container.");
      kConsistent = kFALSE;
      return kFALSE;
    }
    if(fContDS[1] != idx) fContDS[1] = idx;
 
    pSet = gOnline->getDetSet()->at(fContDS[0]);
    if(!pSet) {
      kConsistent = kFALSE;
      return kFALSE;
    }

    HUserParameter *pPar1 = (HUserParameter*) fContParam->First();
    if(!pPar1) {
      if(kMsg) HError::message("1st parameter of the contour not found.");
      kConsistent = kFALSE;
      return kFALSE;
    }
    if(pPar1->getCategory() != pSet->getCategory()) {
      if(kMsg) HError::message("Parameter and det set categories are different.");
      kConsistent = kFALSE;
      return kFALSE;
    }

    pSet = gOnline->getDetSet()->at(fContDS[1]);
    if(!pSet) {
      kConsistent = kFALSE;
      return kFALSE;
    }

    HUserParameter *pPar2 = (HUserParameter*) fContParam->After(pPar1);
    if(!pPar2) {
      if(kMsg) HError::message("2nd parameter of the contour not found.");
      kConsistent = kFALSE;
      return kFALSE;
    }
    if(pPar2->getCategory() != pSet->getCategory()) {
      if(kMsg) HError::message("Parameter and det set categories are different.");
      kConsistent = kFALSE;
      return kFALSE;
    }
  }

  char nulstr[50] = "";
  if(!strcmp(GetName(),nulstr)) {
    if(kMsg) HError::message("Global condition name not defined.");
    kConsistent = kFALSE;
    return kFALSE;
  }

  kConsistent = kTRUE;
  return kTRUE;

}



//---------------------------------------------------------------------
//******  void HGlobalCondition::calMaxParIdx()
//
// This function scans the list fLCList and finds the maximum of indexes
// fLCTrueIdx. This information is used by the evaluation function.
//
//---------------------------------------------------------------------

void HGlobalCondition::calcMaxParIdx() {
//
// This function scans the list fLCList and finds the maximum of indexes
// fLCTrueIdx. This information is used by the evaluation function.
//

  fMaxParIdx = -1;
  if(!fLCList) return;
  HNameIdx *pNameIdx = 0;
  TIter iter(fLCList);
  Int_t tempidx = -1;

  while((pNameIdx = (HNameIdx*) iter())) {
    tempidx = pNameIdx->fLCTrueIdx + 2;
    if(tempidx > fMaxParIdx) fMaxParIdx = tempidx;
  }

}


//---------------------------------------------------------------------
//******  Bool_t HGlobalCondition::setExpression()
//
// This function sets fExpression using expression stored in fExpString.
//
//---------------------------------------------------------------------

Bool_t HGlobalCondition::setExpression() {
//
// This function sets fExpression using expression stored in fExpString.
//

  if(!fExpString) return kFALSE;
  if(fExpression) {
    delete fExpression;
    fExpression = 0;
  }
  if(fLCList) { fLCList->Delete(); delete fLCList; fLCList = 0; }
  fLCList = new TList();
  if(!fLCList) return kFALSE;

  char nullstr[100] = "";
  char expname[10] = "formula";
  if(!strcmp(nullstr,GetName())) 
    fExpression = new TFormula(expname,fExpString->exp);
  else fExpression = new TFormula(GetName(),fExpString->exp);

  if(!fExpression) return kFALSE;
  if(!setLCList()) return kFALSE;
  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HGlobalCondition::setLCList()
//
// This function sets the list of HNameIdx objects.
//
//---------------------------------------------------------------------

Bool_t HGlobalCondition::setLCList() {
//
// This function sets the list of HNameIdx objects.
//

  if(!fExpString) return kFALSE;
  if(fLCList) fLCList->Delete();
  else fLCList = new TList();

  if(!fLCList) return kFALSE;

  fNumber = 0;

  HNameIdx *pNameIdx = 0;
  HNameIdx *pElement = 0;
  HLocalCondition *pLocal = 0;

  TIterator *it = 0;
  Int_t i;

  for(i=0; i < (fExpString->parcur + 1); i++) {

    pNameIdx = new HNameIdx();
    if(!pNameIdx) return kFALSE;

    pNameIdx->fLCTrueIdx = (*fExpString).paridx[i];
    while(((pNameIdx->fLCTrueIdx % 3) != 0)) (pNameIdx->fLCTrueIdx)--;

    pNameIdx->fLCIndex = pNameIdx->fLCTrueIdx / 3;
    
    pLocal = gOnline->getLocal()->at(pNameIdx->fLCIndex);
    if(!pLocal) return kFALSE; 
    
    pNameIdx->setLCName(pLocal->GetName());

    if(it) { delete it; it = 0; }
    it = fLCList->MakeIterator();
    while((pElement = (HNameIdx*) it->Next())) {
      if(pNameIdx->isEqual(pElement)) {
	delete pNameIdx;
	pNameIdx = 0;
	break;
      }
    }

    if(pNameIdx) {
      fLCList->Add(pNameIdx);
      fNumber++;
    }
  }

  calcMaxParIdx();

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Bool_t HGlobalCondition::setContList(HUserParameter *p1,
//                                             HUserParameter *p2)
//
// This function sets the list of parameters for the contour. 
//
//---------------------------------------------------------------------

Bool_t HGlobalCondition::setContList(HUserParameter *p1, HUserParameter *p2) {
//
// This function sets the list of parameters for the contour. 
//

  if(!p1 || !p2) return kFALSE;

  if(fContParam) { fContParam->Delete(); delete fContParam; fContParam = 0; }
  fContParam = new TList();
  if(!fContParam) return kFALSE;

  fContParam->AddFirst(p1);
  fContParam->AddAfter(p1,p2);

  return kTRUE;

}


//---------------------------------------------------------------------
//******  void HGlobalCondition::setDSName(const char *name, Int_t pos)
//         
// This function sets the detector set name for the contour. 
//
//---------------------------------------------------------------------

void HGlobalCondition::setDSName(const char *name, Int_t pos) {
//         
// This function sets the detector set name for the contour. 
//

  if(!name) return;

  if(pos == 1) sprintf(fDS1Name,"%s",name);
  if(pos == 2) sprintf(fDS2Name,"%s",name);

}


//---------------------------------------------------------------------
//******  Bool_t HNameIdx::isEqual(HNameIdx *anidx)
//
// This function checks if two objects of the class HNameIdx are equal.
//
//---------------------------------------------------------------------

Bool_t HNameIdx::isEqual(HNameIdx *anidx) {
//
// This function checks if two objects of the class HNameIdx are equal.
//

  if(!anidx) return kFALSE;
  if(anidx->fLCIndex != fLCIndex) return kFALSE;
  if(anidx->fLCTrueIdx != fLCTrueIdx) return kFALSE;
  return kTRUE;

}












