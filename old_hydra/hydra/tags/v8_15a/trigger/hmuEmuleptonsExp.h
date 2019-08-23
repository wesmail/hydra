//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: hmuEmuleptonsExp.h,v 1.3 2005-07-15 09:00:24 wuestenf Exp $

#ifndef HMUEMULEPTONSEXP_H
#define HMUEMULEPTONSEXP_H

#include "hmuEmuleptons.h"

class HMUEMULeptonsExp : public HMUEMULeptons
{
 public: 
  HMUEMULeptonsExp(void);
  ~HMUEMULeptonsExp(void); 

 ClassDef(HMUEMULeptonsExp,1) // Leptons from matching unit emulation for real data without MU

};

class HMUEMUDiLeptonsExp : public HMUEMUDiLeptons
{
 public:
  HMUEMUDiLeptonsExp(void);
  ~HMUEMUDiLeptonsExp(void);

 ClassDef(HMUEMUDiLeptonsExp,1) // Dileptons from Matching unit emulation for real data without MU

};

#endif /* !HMUEMULEPTONSEXP_H */













