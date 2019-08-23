#ifndef HRICHIPUREMAKE_H
#define HRICHIPUREMAKE_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hrichhitIPU.h"
#include "hlocmaxfilter.h"

class HCategory;
class HIterator;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HSpectrometer;
class HRuntimeDb;
class HLocMaxFilter;
// ***************************************************************************

class HRichIPURemake : public HReconstructor {

// ***************************************************************************
 public:
    HCategory *pRich; 
    
    HRichIPUParThresholds *fRichIPUParThr; //!
    HRichIPUParLocMax *fRichIPUParLmax; //!   
    
    HRichIPURemake();
    HRichIPURemake(Text_t *name, Text_t *title);
    ~HRichIPURemake();
    
    HIterator * iterRich ; //!
    
    Bool_t init();
    void initParContainer(HSpectrometer *,HRuntimeDb *);
    Bool_t finalize();
    Int_t execute();
    void prepareInput();
    
    ClassDef(HRichIPURemake,1) 
  };
	
//============================================================================
 
#endif  // HRICHIPUREMAKE_H
             




