//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*-- Version: $Id: muDilepEmulationExp.h,v 1.4 2009-07-15 11:34:06 halo Exp $

#ifndef HMUDILEPEMULATIONEXP_H
#define HMUDILEPEMULATIONEXP_H

#include "muDilepEmulation.h"

// ***************************************************************************

class HMUDilepEmulationExp : public HMUDilepEmulation {

// ***************************************************************************
 private:
  void prepareInput(void);

 public:
  HMUDilepEmulationExp(void);
  HMUDilepEmulationExp(const Text_t *name,const Text_t *title);
  virtual ~HMUDilepEmulationExp(void);

  Bool_t init(void);
  Bool_t finalize(void);

  ClassDef(HMUDilepEmulationExp,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATIONEXP_H
             



