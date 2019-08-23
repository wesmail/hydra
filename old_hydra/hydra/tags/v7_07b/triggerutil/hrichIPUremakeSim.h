//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/18/2005

#ifndef HRICHIPUREMAKESIM_H
#define HRICHIPUREMAKESIM_H

#include "hreconstructor.h"

class HCategory;
class HIterator;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HSpectrometer;
class HRuntimeDb;
class HLocMaxFilterSim;
// ***************************************************************************

class HRichIPURemakeSim : public HReconstructor {

// ***************************************************************************
protected:
  HCategory *pRich;                       //!
  HRichIPUParThresholds *fRichIPUParThr;  //!
  HRichIPUParLocMax *fRichIPUParLmax;     //!   
  HIterator * iterRich ;                  //!

private:
  Bool_t initParContainer(HSpectrometer *,HRuntimeDb *);

public:
  HRichIPURemakeSim(void);
  HRichIPURemakeSim(Text_t *name, Text_t *title);
  ~HRichIPURemakeSim(void);

  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void prepareInput(void);

  ClassDef(HRichIPURemakeSim,1) // IPU 
};

//============================================================================

#endif  // HRICHIPUREMAKESIM_H
