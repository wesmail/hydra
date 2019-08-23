//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: hmuEmuleptonsSim.h,v 1.2 2005-07-15 09:00:24 wuestenf Exp $

#ifndef HMUEMULEPTONSSIM_H
#define HMUEMULEPTONSSIM_H

#include "hmuEmuleptons.h"

class HMUEMULeptonsSim : public HMUEMULeptons
{
 public: 
  HMUEMULeptonsSim(void);
  ~HMUEMULeptonsSim(void); 

 ClassDef(HMUEMULeptonsSim,1) // Leptons from matching unit emulation for simulation data
};

class HMUEMUDiLeptonsSim : public HMUEMUDiLeptons
{
 public:
  HMUEMUDiLeptonsSim(void);
  ~HMUEMUDiLeptonsSim(void);

 ClassDef(HMUEMUDiLeptonsSim,1) // Dileptons from matching unit emulation from simulation data
};

#endif /* !HMUEMULEPTONSSIM_H */













