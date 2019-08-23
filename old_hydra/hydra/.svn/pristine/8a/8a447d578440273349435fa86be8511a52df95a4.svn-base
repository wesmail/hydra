#if !defined HEXPSTRING_H
#define HEXPSTRING_H

//-------------------------------------------------------------------------
//
// File name:       hexpstring.h
// Subject:         Expression string for a condition.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HExpString.
// This is a helper class which keeps the expression in the form of a string
// and all the related information necessary for evaluation, like for example
// parameters indexes. It is also used by the subpanels for local and
// global conditions, since it contains the helper functions for 
// the interactive definition of an expression by means of GUI.
//
//-------------------------------------------------------------------------

#include <TObject.h>

class TGListBox;

class HExpString : public TObject {

public:

  Int_t index[500];
  Int_t cursor;
  char exp[500];
  Int_t paridx[500]; // indexes of parameters [0], [1] etc
  Int_t parcur;
  Bool_t kClear;
  Bool_t kValid;  // expression is valid

public:

  HExpString() : TObject() {cursor=parcur=-1; kClear=kTRUE; kValid = kFALSE; }
  virtual ~HExpString() {}
  void makeExp();
  void addItem(Int_t idx);
  void addString(const char* strng);
  void undo();
  void updateList(TGListBox *p);
  void copy(HExpString &hes);
  void clear() { cursor = parcur = -1; makeExp(); }
  Bool_t isClear() { return kClear; }
  Bool_t isValid() { return kValid; }
  void check();
  void checkParam();
  const char* getExp() { return exp; }
    
  ClassDef(HExpString,1)

};

#endif






