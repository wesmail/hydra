//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#ifndef HLOCMAXFILTERSIM_H
#define HLOCMAXFILTERSIM_H

#include "hfilter.h" 

// ***************************************************************************
class TObject;

class HLocMaxFilterSim : public HFilter
{
// ***************************************************************************
 public:    
    HLocMaxFilterSim(void); 
    ~HLocMaxFilterSim(void);
    Bool_t check(TObject *obj);

    ClassDef(HLocMaxFilterSim,1)   // Local maximum filter for RICH simuylated data 
};

//============================================================================
 
#endif  // HLOCMAXFILTERSIM_H
