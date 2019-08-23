using namespace std;
#include "htofinocalsimfilter.h"
#include "htofinocalsim.h"
#include <fstream>
#include <iomanip>

ClassImp(HTofinoCalSimFilter)

// adapted from HTofCalSimFilter

//////////////////////////////////////////////////////////////////
//
// HTofinoCalSimFilter is a HFilter to reduce the number of the
// HTofinoCalSim data in the catTofinocal category.
// (If HTofinoCalSim object has Charge==0 && Time==0
// it is deleted from the category.)
//
//////////////////////////////////////////////////////////////////


Bool_t HTofinoCalSimFilter :: check(TObject* cal){
  if ( ((((HTofinoCalSim*)cal)->getTime())==0) &&
       ((((HTofinoCalSim*)cal)->getCharge())==0) ){
    return kFALSE;
  } else {
    return kTRUE;
  }
}
