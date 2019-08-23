//*-- AUTHOR : Diego Gonzalez-Diaz 08/06/2006

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HRpcCalSim                                                                //
// this is cal data level for simulated data                                 //
// it stores 2 track numbers                                                 //
///////////////////////////////////////////////////////////////////////////////

#include "hrpccalsim.h"

ClassImp(HRpcCalSim)

void HRpcCalSim::clear(void) {
  // clears the object
  HRpcCalSim::clear();
  nTrackL = nTrackR = -1;

}
