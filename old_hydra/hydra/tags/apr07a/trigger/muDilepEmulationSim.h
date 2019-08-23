//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*--Version: $Id: muDilepEmulationSim.h,v 1.2 2005-07-15 09:00:25 wuestenf Exp $

#ifndef HMUDILEPEMULATIONSIM_H
#define HMUDILEPEMULATIONSIM_H


#include "muDilepEmulation.h"


// ***************************************************************************

class HMUDilepEmulationSim : public HMUDilepEmulation {

// ***************************************************************************
 public:
  HMUDilepEmulationSim();
  HMUDilepEmulationSim(Text_t *name, Text_t *title);
  ~HMUDilepEmulationSim();

  Bool_t init();
  Bool_t finalize();
  
  ClassDef(HMUDilepEmulationSim,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATIONSIM_H
             



