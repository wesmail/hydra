#if !defined HCONTOURCONT_H
#define HCONTOURCONT_H

//-------------------------------------------------------------------------
//
// File name:       hcontourcont.h
// Subject:         Container of contours.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HContourCont.
// This class stores all run time defined contours.
//
//-------------------------------------------------------------------------

#include <TObjArray.h>

class HContour;

class HContourCont : public TObject {

 protected:

  TObjArray         *fContourAr;    // list of contours

 public:

  HContourCont();
  virtual ~HContourCont();
  Bool_t init();
  Bool_t add(HContour *p);
  HContour* at(Int_t idx);
  Int_t getEntries();
  Int_t find(const Char_t* name);
  void removeAt(Int_t idx);
  TObjArray* getArray() const { return fContourAr; }
  void compress() { if(fContourAr) fContourAr->Compress(); }

 public:

  ClassDef(HContourCont,1) //Container of contours

};

#endif


