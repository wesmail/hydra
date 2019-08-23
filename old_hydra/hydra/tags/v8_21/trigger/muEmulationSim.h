//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: muEmulationSim.h,v 1.4 2009-07-15 11:34:06 halo Exp $

#ifndef HMUEMULATIONSIM_H
#define HMUEMULATIONSIM_H


#include "muEmulation.h"

class HSpectrometer;
class HRuntimeDb;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HTriggerParShowerMap;
// ***************************************************************************

class HMUEmulationSim : public HMUEmulation
{

// ****************************************************************************
protected:
  HRichIPUParThresholds *fRichIPUParThr;  //! Pointer to RICH IPU threshold parameters
  HRichIPUParLocMax *fRichIPUParLmax;     //! Pointer to RICH IPU local maximum parameters
  HTriggerParShowerMap *fShowerMap;       //! Pointer to SHOWER  mapping parameters

private:
  void prepareInput(void);
  Bool_t initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb);

public:
  HMUEmulationSim(void);
  HMUEmulationSim(const Text_t *name,const Text_t *title);
  virtual ~HMUEmulationSim();

  Bool_t init(void);
  Bool_t finalize(void);

  void setParThresholds(HRichIPUParThresholds *p){fRichIPUParThr=p;}
  void setParLocMax(HRichIPUParLocMax *p){fRichIPUParLmax=p;}

  ClassDef(HMUEmulationSim,1)   // Emulation of matching unit for simulation data
};

//============================================================================

#endif  // HMUEMULATIONSIM_H
