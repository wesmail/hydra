using namespace std;
#include "hades.h"
#include "hcategory.h"
#include "hevent.h"
#include "hlocation.h"
#include "hrichhitIPU.h"
#include "hlocmaxfilter.h"

#include <iostream>          
#include <iomanip>

ClassImp(HLocMaxFilter)
//----------------------------------------------------------------------------
HLocMaxFilter::HLocMaxFilter() { 

}
//============================================================================


Bool_t HLocMaxFilter::check(TObject *r) {
    if (  ((HRichHitIPU*)r)->getX() != 9999 ) {
	return kTRUE;
    } else {
	return kFALSE;
    }
}


//----------------------------------------------------------------------------
HLocMaxFilter::~HLocMaxFilter() { 
 }

//============================================================================
