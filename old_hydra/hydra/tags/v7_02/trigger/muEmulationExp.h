#ifndef HMUEMULATIONEXP_H
#define HMUEMULATIONEXP_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptonsExp.h"
#include "muEmulation.h"

class HCategory;
class HMUEmulationExp;
class HSpectrometer;
class HRuntimeDb;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
// ***************************************************************************

class HMUEmulationExp : public HMUEmulation {
  
// ****************************************************************************   
 public:
  HRichIPUParThresholds *fRichIPUParThr; //!
  HRichIPUParLocMax *fRichIPUParLmax; //!

  HMUEmulationExp();
  HMUEmulationExp(Text_t *name, Text_t *title);
  ~HMUEmulationExp();

  Bool_t init();
  Bool_t finalize();
  Int_t execute();

  void prepareInput();
  void initParContainer(HSpectrometer *, HRuntimeDb *);

  void setParThresholds(HRichIPUParThresholds *p){fRichIPUParThr=p;}
  void setParLocMax(HRichIPUParLocMax *p){fRichIPUParLmax=p;}

  ClassDef(HMUEmulationExp,1) 
};
    
//============================================================================
 
#endif  // HMUEMULATIONEXP_H
             




