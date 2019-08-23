#if !defined HONLPROCESSEVENT_H
#define HONLPROCESSEVENT_H

//-------------------------------------------------------------------------
//
// File name:       honlprocessevent.h
// Subject:         Event processing inside HTOP
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HOnlProcessEvent.
// This class maintains processing of an event inside HTOP. That means
// 1) evaluation of detector sets
// 2) evaluation of local conditions
// 3) evaluation of global conditions
// 4) filling of the histogrammes
//
// Being derived from the HReconstructor class, HOnlProcessEvent can be easily
// added to the HYDRA task list.
// Init() function of the HOnlProcessEvent class performs all necessary 
// initialization for the whole HTOP.
//
//-------------------------------------------------------------------------


#include "hreconstructor.h"

class HOnlProcessEvent : public HReconstructor {

 public:

  HOnlProcessEvent() : HReconstructor() {}
  HOnlProcessEvent(Text_t *name,Text_t *title) : HReconstructor(name,title) {}
  ~HOnlProcessEvent() {}
  Bool_t init();
  Int_t execute();
  Bool_t finalize() { return kTRUE; }

 public:

  ClassDef(HOnlProcessEvent,1) //Event processing inside HTOP

};

#endif





