#if !defined HHISTOCONT_H
#define HHISTOCONT_H

//-------------------------------------------------------------------------
//
// File name:       hhistocont.h
// Subject:         Container of histogrammes.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HHistoCont.
// This class serves as a container of histogrammes.
// All histogrammes are stored in an array which is a TObjArray object.
//
//-------------------------------------------------------------------------

#include <TObjArray.h>

class HHistogram;

class HHistoCont : public TObject {

 protected:

  TObjArray         *fHistoAr;    // list of histogrammes

 public:

  HHistoCont();
  virtual ~HHistoCont();
  Bool_t init();
  Bool_t add(HHistogram *p);
  Bool_t addAt(HHistogram *p, Int_t idx);
  HHistogram* at(Int_t idx);
  Int_t getEntries();
  Int_t find(const Char_t* name);
  void removeAt(Int_t idx, Bool_t kSulSerio = kTRUE);
  void checkConsistency();
  TObjArray* getArray() const { return fHistoAr; }

 public:

  ClassDef(HHistoCont,1) //Container of histogrammes

};

#endif



