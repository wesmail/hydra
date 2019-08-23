#if !defined HLOCALCONDITION_H
#define HLOCALCONDITION_H

//-------------------------------------------------------------------------
//
// File name:       hlocalcondition.h
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
// This file contains the declaration of the class HLocalCondition.
// This class provides all functions to maintain the online objects of
// the type "local condition".
//
//-------------------------------------------------------------------------

#include <TFormula.h>
#include "hconditionout.h"
#include "hcontour.h"

class HExpString;
class HDetSet;
class HUserParameter;

class HLocalCondition : public TNamed {

 protected:

  TFormula       *fExpression;      //  expression to be evaluated
  HContour       *fContour;         //  contour to be evaluated
  TList          *fExpList;         //  list of parameters for expression
                                    //  (HParameter objects)
  Int_t           fExpNumber;       //  number of parameters for expression
  TList          *fContList;        //  list of parameters for contour
  Int_t           fContNumber;      //  number of parameters for contour
  Int_t           fDetSet;          //  index of det set in array of det sets
  Text_t          fDetSetName[100]; //  det set name (for consistency check)
  HConditionOut  *fOut;             //! evaluation results
  Bool_t          kAlwaysTrue;      //  if true, condition is not evaluated
  Bool_t          kConsistent;      //  condition consistent
  HExpString     *fExpString;       //  expression string (for browser)
  Bool_t          kEvaluated;       //  true if condition was evaluated

 public:

  Bool_t          setExpList(Int_t acateg);

 public:

  HLocalCondition();
  virtual ~HLocalCondition();
  Bool_t          checkConsistency(Bool_t kMsg = kFALSE);
  void            evaluate();
  void            setExpString(HExpString *p) { fExpString = p; }
  HExpString*     getExpString() const { return fExpString; }
  void            setContour(HContour* p) { fContour = p; }
  HContour*       getContour() const { return fContour; }
  Bool_t          setExpression(Int_t acateg);
  TFormula*       getExpression() const { return fExpression; }
  TList*          getExpList() const { return fExpList; }
  Int_t           getExpNumber() const { return fExpNumber; }
  TList*          getContList() const { return fContList; }
  Bool_t          setContList(HUserParameter *p1, HUserParameter *p2);
  Int_t           getContNumber() const { return fContNumber; }
  void            setDetSet(const char* detsetname);
  Int_t           getDetSet() const { return fDetSet; }
  HConditionOut*  getOutput() const { return fOut; }
  void            SetName(const char* name);
  Bool_t          init();
  Bool_t          isAlwaysTrue() const { return kAlwaysTrue; }
  void            setAlwaysTrue(Bool_t aflag = kTRUE) { kAlwaysTrue = aflag; }
  Bool_t          isConsistent() const { return kConsistent; }
  Bool_t          isEvaluated() const { return kEvaluated; }

 public:

  ClassDef(HLocalCondition, 1)

};

#endif



