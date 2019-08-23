#ifndef HMUEMULATIONSIM_H
#define HMUEMULATIONSIM_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptons.h"
#include "muEmulation.h"

class HCategory;
class HMUEmulationSim;
class HSpectrometer;
class HRuntimeDb;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
// ***************************************************************************

class HMUEmulationSim : public HMUEmulation {
  
// ****************************************************************************   
 public:
  HRichIPUParThresholds *fRichIPUParThr; //!
  HRichIPUParLocMax *fRichIPUParLmax; //!

  HMUEmulationSim();
  HMUEmulationSim(Text_t *name, Text_t *title);
  ~HMUEmulationSim();

  Bool_t init();
  Bool_t finalize();
  Int_t execute();

  void prepareInput();
  void initParContainer(HSpectrometer *, HRuntimeDb *);

  void setParThresholds(HRichIPUParThresholds *p){fRichIPUParThr=p;}
  void setParLocMax(HRichIPUParLocMax *p){fRichIPUParLmax=p;}

  ClassDef(HMUEmulationSim,1) 
};
    
//============================================================================
 
#endif  // HMUEMULATIONSIM_H
             




