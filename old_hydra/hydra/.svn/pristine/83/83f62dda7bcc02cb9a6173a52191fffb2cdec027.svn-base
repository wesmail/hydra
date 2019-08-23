#if !defined HDETSETCONT_H
#define HDETSETCONT_H

//-------------------------------------------------------------------------
//
// File name:       hdetsetcont.h
// Subject:         Container of detector sets.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HDetSetCont.
// This class serves as a container of detector sets.
// All detector sets are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

#include "TObjArray.h"

class HDetSet;

class HDetSetCont : public TObject {

 protected:

  TObjArray      *fDetSetAr;

 public:

  HDetSetCont();
  virtual ~HDetSetCont();
  Bool_t init();
  Bool_t add(HDetSet *p);
  Bool_t addAt(HDetSet *p, Int_t idx);
  HDetSet* at(Int_t idx);
  Int_t getEntries();
  Int_t find(const Char_t* name);
  void removeAt(Int_t idx, Bool_t kSulSerio = kTRUE);
  TObjArray* getArray() const { return fDetSetAr; }

 public:
  
  ClassDef(HDetSetCont, 1) //Container of detector sets

};

#endif


































