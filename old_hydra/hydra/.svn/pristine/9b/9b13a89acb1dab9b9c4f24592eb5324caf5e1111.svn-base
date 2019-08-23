#if !defined HGLOBALCONT_H
#define HGLOBALCONT_H

//-------------------------------------------------------------------------
//
// File name:       hglobalcont.h
// Subject:         Container of global conditions.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HGlobalCont.
// This class serves as a container of global conditions.
// All global conditions are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

#include <TObjArray.h>

class HGlobalCondition;

class HGlobalCont : public TObject {

 protected:

  TObjArray         *fGlobalAr;    // list of global conditions

 public:

  HGlobalCont() : TObject() { fGlobalAr = 0; }
  virtual ~HGlobalCont();
  Bool_t init();
  Bool_t add(HGlobalCondition *p);
  Bool_t addAt(HGlobalCondition *p, Int_t idx);
  HGlobalCondition* at(Int_t idx);
  Int_t getEntries();
  Int_t find(const Char_t* name);
  void removeAt(Int_t idx, Bool_t kSulSerio = kTRUE);
  void checkConsistency();
  TObjArray* getArray() const { return fGlobalAr; }

 public:

  ClassDef(HGlobalCont,1)

};

#endif



