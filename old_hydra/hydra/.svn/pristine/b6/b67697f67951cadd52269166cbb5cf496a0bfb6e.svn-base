//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: muEmulationExp.h,v 1.4 2009-07-15 11:34:06 halo Exp $

#ifndef HMUEMULATIONEXP_H
#define HMUEMULATIONEXP_H


#include "hreconstructor.h"
#include "hmatrixcatiter.h"
#include "hmuEmuleptonsExp.h"
#include "muEmulation.h"

class HSpectrometer;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HTriggerParShowerMap;
// ***************************************************************************

class HMUEmulationExp : public HMUEmulation {

// ****************************************************************************
private:
  void prepareInput(void);
  Bool_t initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb);

protected:
  HRichIPUParThresholds *fRichIPUParThr;  //! RICH IPU threshold parameter container
  HRichIPUParLocMax *fRichIPUParLmax;     //! RICH IPU local mximum parametr container
  HTriggerParShowerMap *fShowerMap;       //! SHOWER row/column to theta/phi mapping

public:

  HMUEmulationExp(void);
  HMUEmulationExp(const Text_t *name,const Text_t *title);
  ~HMUEmulationExp(void);

  Bool_t init(void);
  Bool_t finalize(void);

  void setParThresholds(HRichIPUParThresholds *p){fRichIPUParThr=p;}
  void setParLocMax(HRichIPUParLocMax *p){fRichIPUParLmax=p;}

  ClassDef(HMUEmulationExp,1)   //Emulation of the matching unit for real data without MU data
};

//============================================================================

#endif  // HMUEMULATIONEXP_H
