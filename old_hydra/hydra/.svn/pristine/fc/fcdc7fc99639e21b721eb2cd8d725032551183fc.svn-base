using namespace std;
#include "hades.h"
#include "hcategory.h"
#include "hevent.h"
#include "hlocation.h"
#include "hrichhitIPUSim.h"
#include "hlocmaxfilterSim.h"

#include <iostream>          
#include <iomanip>

ClassImp(HLocMaxFilterSim)
//----------------------------------------------------------------------------
HLocMaxFilterSim::HLocMaxFilterSim() { 

}
//============================================================================


Bool_t HLocMaxFilterSim::check(TObject *r) {
    if (  ((HRichHitIPUSim*)r)->getX() != 9999 ) {
	return kTRUE;
    } else {
	return kFALSE;
    }
}


//----------------------------------------------------------------------------
HLocMaxFilterSim::~HLocMaxFilterSim() { 
 }

//============================================================================
