#include "hkickstartc.h"

#include "hstartcal.h"
#include "hevent.h"
#include "hcategory.h"
#include "hades.h"
#include "hstartdef.h"

//*-- Author : M. Sanchez

//////////////////////////////////////////////////////////
// HKickStartC
//
//  Dummy class for setting conditions on the Start detector. 
//  Lifetime: Nov2000
/////////////////////////////////////////////////////////


HKickStartC::HKickStartC(Text_t name[], Text_t title[]) : 
  HReconstructor(name,title) {
  fMaxMult = 1;
  fSelectedStrip = 3;
}

HKickStartC::~HKickStartC(void) {
}
 
Bool_t HKickStartC::init(void) {
  if (gHades) {
    fStartCat = gHades->getCurrentEvent()->getCategory(catStartCal);
    if (!fStartCat) {
      Error("init","Start Cal category not found");
      return kFALSE;
    }
  } else {
    Error("init","gHades not found");
    return kFALSE;
  }
  return kTRUE;
}   

Int_t HKickStartC::execute(void) {
  HStartCal *cal=0;
  if (fStartCat->getEntries() > fMaxMult) return 1;
  
/*  cal = (HStartCal *)fStartCat->getObject(0);
  if (cal) {
    if (cal->getStrip() != fSelectedStrip) return 2;
  } else return 3;*/

  return 0;    
}	

Bool_t HKickStartC::finalize(void) {
  return kTRUE;
}

ClassImp(HKickStartC)
