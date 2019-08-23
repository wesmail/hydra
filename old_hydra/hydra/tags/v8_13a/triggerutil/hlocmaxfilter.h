//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#ifndef HLOCMAXFILTER_H
#define HLOCMAXFILTER_H

#include "hfilter.h" 

class TObject;
// ***************************************************************************

class HLocMaxFilter : public HFilter
{
// ***************************************************************************
 public:    
    HLocMaxFilter(void); 
    ~HLocMaxFilter(void);
    Bool_t check(TObject *obj);

    ClassDef(HLocMaxFilter,1) // Local maximum filter for RICH data
  };

//============================================================================
 
#endif  // HLOCMAXFILTER_H
