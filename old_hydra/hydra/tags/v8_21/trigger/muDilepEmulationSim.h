//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*--Version: $Id: muDilepEmulationSim.h,v 1.3 2009-07-15 11:34:06 halo Exp $

#ifndef HMUDILEPEMULATIONSIM_H
#define HMUDILEPEMULATIONSIM_H


#include "muDilepEmulation.h"


// ***************************************************************************

class HMUDilepEmulationSim : public HMUDilepEmulation {

// ***************************************************************************
 public:
  HMUDilepEmulationSim();
  HMUDilepEmulationSim(const Text_t *name,const Text_t *title);
  ~HMUDilepEmulationSim();

  Bool_t init();
  Bool_t finalize();
  
  ClassDef(HMUDilepEmulationSim,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATIONSIM_H
             



