#if !defined HCONDITIONOUT_H
#define HCONDITIONOUT_H

//-------------------------------------------------------------------------
//
// File name:       hconditionout.h
// Subject:         Output of the local condition.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HConditionOut.
// This class is an output of the local condition. It stores the list of 
// locations for the hits which passed the condition, the list of geometrical
// locations for the hits which passed the condition and the condition 
// counters.
// count1 - condition was true
// count2 - condition was false
// countS = count1 + count2
//
//-------------------------------------------------------------------------

#include "TList.h"

class HConditionOut : public TObject {

 protected:

  Bool_t     kTrueFlag;      // at least once condition was true
  TList     *fLocList;       // list of locations in the category for the hits
                             // which pass condition 
  TList     *fIdxList;       // list of indexes (sec, mod, cell) for the hits
                             // which pass condition (ONLY for TOF and SHOWER:
                             // one to one correspondence between raw-cal-hit)
  Int_t      count1;         // condition was true
  Int_t      count2;         // condition was false
  Int_t      countS;         // count1 + count2

 public:

  HConditionOut();
  virtual ~HConditionOut();
  Bool_t getTrueFlag() { return kTrueFlag; }  
  void setTrueFlag() { kTrueFlag = kTRUE; }
  TList* getLocList() const { return fLocList; }
  TList* getIdxList() const { return fIdxList; }
  Bool_t init();
  Int_t  getCount(Int_t number);
  void   incCount(Int_t number);
  void   clearCounts() { count1 = count2 = countS = 0; kTrueFlag = kFALSE; }
  void   clearList();

 public:

  ClassDef(HConditionOut,0) //Output of the local condition

};


#endif



