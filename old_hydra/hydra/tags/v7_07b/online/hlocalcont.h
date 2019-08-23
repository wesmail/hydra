#if !defined HLOCALCONT_H
#define HLOCALCONT_H

//-------------------------------------------------------------------------
//
// File name:       hlocalcont.h
// Subject:         Container of local conditions.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HLocalCont.
// This class serves as a container of local conditions.
// All detector sets are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

#include <TObjArray.h>

class HLocalCondition;

class HLocalCont : public TObject {

 protected:

  TObjArray         *fLocalAr;    // list of local conditions

 public:

  HLocalCont();
  virtual ~HLocalCont();
  Bool_t init();
  Bool_t add(HLocalCondition *p);
  Bool_t addAt(HLocalCondition *p, Int_t idx);
  HLocalCondition* at(Int_t idx);
  Int_t getEntries();
  Int_t find(const Char_t* name);
  void removeAt(Int_t idx, Bool_t kSulSerio = kTRUE);
  void checkConsistency();
  TObjArray* getArray() const { return fLocalAr; }

 public:

  ClassDef(HLocalCont,1) //Container of local conditions

};

#endif



