#if !defined HGLOBALCONDITION_H
#define HGLOBALCONDITION_H

//-------------------------------------------------------------------------
//
// File name:       hglobalcondition.h
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
// This file contains the declaration of the classes HNameIdx and 
// HGlobalCondition.
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
// 
// count1 = idx*3
// count2 = idx*3 + 1
// count3 = idx*3 + 2
//
// where idx is the index of the appropriate local condition in the container
// of local conditions.
//
//-------------------------------------------------------------------------

#include <TFormula.h>
#include "hcontour.h"

class HNameIdx : public TObject {

 public:

  Int_t   fLCIndex;     // local condition index in array gOnline->getLocal()
  Text_t  fLCName[100]; // local condition name
  Int_t   fLCTrueIdx;   // index of count1 of this condition (lc index*3)

 public:

  HNameIdx() : TObject() {}
  virtual ~HNameIdx() {}
  void setLCName(const char* name) { if(name) sprintf(fLCName,"%s",name); }
  Bool_t isEqual(HNameIdx *anidx);

 public:

  ClassDef(HNameIdx,1)

};


class HExpString;
class HContour;
class HUserParameter;

class HGlobalCondition : public TNamed {

 protected:

  TList        *fLCList;       // list of local conditions (objects HNameIdx)
  Int_t         fNumber;       // number of local conditions
  TFormula     *fExpression;   // expression to be evaluated
  HContour     *fContour;      // contour to be evaluated
  HExpString   *fExpString;    // expression string for browser
  Int_t         fContDS[2];    // array of det set indexes for contour
  Text_t        fDS1Name[100]; // name of the first det set
  Text_t        fDS2Name[100]; // name of the second det set
  TList        *fContParam;    // list of parameters for contour evaluation
  Bool_t        kEvalResult;   //! result of condition evaluation
  Int_t         fMaxParIdx;    // maximum index of parameter for expression
  Bool_t        kConsistent;   // consistency flag

 protected:

  Bool_t setLCList();

 public:
  
  HGlobalCondition();
  virtual ~HGlobalCondition();
  void setExpString(HExpString *p) { fExpString = p; }
  HExpString* getExpString() const { return fExpString; }  
  TFormula* getExpression() const { return fExpression; }
  Bool_t setExpression();
  void setContour(HContour* p) { fContour = p; }
  HContour* getContour() const { return fContour; }
  TList* getContList() const { return fContParam; }
  Bool_t setContList(HUserParameter *p1, HUserParameter *p2);
  Bool_t isTrue() const { return kEvalResult; }
  Bool_t checkConsistency(Bool_t kMsg = kFALSE);
  Bool_t isConsistent() const { return kConsistent; }
  Bool_t isExpression() { return ((fExpression) ? kTRUE : kFALSE); }
  TList* getLCList() const { return fLCList; }
  void evaluate();
  void calcMaxParIdx();
  void setDSName(const char* name, Int_t pos);
  Int_t getDSIdx(Int_t pos) { 
    return ((pos==1 || pos==2) ? fContDS[pos-1] : -1);
  }

 public:

  ClassDef(HGlobalCondition,1)

};

#endif

  
