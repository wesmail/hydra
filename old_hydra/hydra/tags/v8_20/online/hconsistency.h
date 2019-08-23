#if !defined HCONSISTENCY_H
#define HCONSISTENCY_H

//-------------------------------------------------------------------------
//
// File name:       hconsistency.h
// Subject:         Online objects consistency check.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HConsistency.
// This class contains functions to perform consistency check for any online
// object of the HTOP. That means for example that if there is a histogram
// bound to a condition the condition must exist otherwise that histogram
// will be declared inconsistent and during event loop no action will be
// applied to it.
//
//-------------------------------------------------------------------------

#include "TArrayI.h"
#include "TObject.h"

class HOnlineControl;

class HConsistency : public TObject {

friend class HOnlineControl;

 protected:

  Bool_t kConsistent;

 public:

  TArrayI   fLCIdx; // list of inconsistent local conditions
  TArrayI   fGCIdx; // list of inconsistent global conditions
  TArrayI   fHiIdx; // list of inconsistent histogrammes

 protected:

  HConsistency();

 public:

  virtual ~HConsistency() {}
  void checkConsistency();
  Bool_t isConsistent() { return kConsistent; }
  void clear();
  void warnPanel();
  void delInc();
  void action();

 public:

  ClassDef(HConsistency,0) //Online objects consistency check

};

#endif



