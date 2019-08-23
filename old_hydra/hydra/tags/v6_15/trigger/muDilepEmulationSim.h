#ifndef HMUDILEPEMULATIONSIM_H
#define HMUDILEPEMULATIONSIM_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptons.h"
#include "muDilepEmulation.h"

class HCategory;
class HMUDilepEmulation;
// ***************************************************************************

class HMUDilepEmulationSim : public HMUDilepEmulation {

// ***************************************************************************
 public:
  HMUDilepEmulationSim();
  HMUDilepEmulationSim(Text_t *name, Text_t *title);
  ~HMUDilepEmulationSim();

  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  
  ClassDef(HMUDilepEmulationSim,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATIONSIM_H
             



