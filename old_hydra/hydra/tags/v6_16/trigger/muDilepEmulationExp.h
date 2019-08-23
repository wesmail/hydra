#ifndef HMUDILEPEMULATIONEXP_H
#define HMUDILEPEMULATIONEXP_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptonsExp.h"
#include "muDilepEmulation.h"

class HMUDilepEmulation;
// ***************************************************************************

class HMUDilepEmulationExp : public HMUDilepEmulation {

// ***************************************************************************
 public:
  HMUDilepEmulationExp();
  HMUDilepEmulationExp(Text_t *name, Text_t *title);
  ~HMUDilepEmulationExp();

  Bool_t init();
  Bool_t finalize();
  Int_t execute();

  void prepareInput();

  ClassDef(HMUDilepEmulationExp,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATIONEXP_H
             



