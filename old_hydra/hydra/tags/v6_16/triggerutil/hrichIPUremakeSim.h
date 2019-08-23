#ifndef HRICHIPUREMAKESIM_H
#define HRICHIPUREMAKESIM_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hrichhitIPUSim.h"
#include "hlocmaxfilterSim.h"

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
 public:
    HCategory *pRich; 
    
    HRichIPUParThresholds *fRichIPUParThr; //!
    HRichIPUParLocMax *fRichIPUParLmax; //!   
    
    HRichIPURemakeSim();
    HRichIPURemakeSim(Text_t *name, Text_t *title);
    ~HRichIPURemakeSim();
    
    HIterator * iterRich ; //!
    
    Bool_t init();
    void initParContainer(HSpectrometer *,HRuntimeDb *);
    Bool_t finalize();
    Int_t execute();
    void prepareInput();
    
    ClassDef(HRichIPURemakeSim,1) 
  };
	
//============================================================================
 
#endif  // HRICHIPUREMAKESIM_H
             




