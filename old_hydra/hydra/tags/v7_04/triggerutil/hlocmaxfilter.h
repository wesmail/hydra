#ifndef HLOCMAXFILTER_H
#define HLOCMAXFILTER_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hfilter.h" 
#include "hrichhitIPU.h"

// ***************************************************************************

class HLocMaxFilter : public HFilter {

// ***************************************************************************
 public:
    
    HLocMaxFilter(); 
    ~HLocMaxFilter();
    Bool_t check(TObject *obj);

    ClassDef(HLocMaxFilter,1) 
  };
	
//============================================================================
 
#endif  // HLOCMAXFILTER_H
             




