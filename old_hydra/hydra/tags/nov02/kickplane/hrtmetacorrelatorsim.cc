#include "hrtmetacorrelatorsim.h"
#include "showertofinodef.h"
#include "hevent.h"
#include "hcategory.h"

Bool_t HRtMetaCorrelatorSim::getShowerCategory(HEvent *event) {
	Bool_t r=kFALSE;
	fShowerCat = event->getCategory(catShowerHitTofTrack);
  	if (!fShowerCat) {
		Error("init","Shower/Tofino hit category not found!");
		r = kFALSE;
  	} else r = kTRUE;
	return r;
}
	
ClassImp(HRtMetaCorrelatorSim)
