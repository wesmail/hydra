#if !defined HCALCLOCATION_H
#define HCALCLOCATION_H

//-------------------------------------------------------------------------
//
// File name:       hcalclocation.h
// Subject:         Evaluation of data hit locations for detector sets.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HCalcLocation.
// This class contains a set of static functions to be called whenever
// it is necessary to calculate a list of physical locations of data
// hits inside a particular category using the geometrical information
// from a detector set. This class is mainly used by detector sets.
//
//-------------------------------------------------------------------------

#include <TList.h>

class HDetSet;

class HCalcLocation : public TObject {

 protected:

  HCalcLocation() : TObject() {}

 public:

  virtual ~HCalcLocation() {}
  static TList* transform(Int_t acat, TList* pList, Bool_t kill=kFALSE);
  static TList* calcLocList(HDetSet *pSet);
  static void calcLoc(HDetSet *pSet);

 public:

  ClassDef(HCalcLocation,0)

};

#endif

