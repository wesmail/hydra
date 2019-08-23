//*-- AUTHOR : Diego Gonzalez-Diaz 17/12/2007

//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HRpcCalSim                                                                //
// this is the cal data level for simulated data                             //
// it stores 4 references to hgeantrpc objects and its corresponding tracks  //
///////////////////////////////////////////////////////////////////////////////


#include "hrpccalsim.h"

ClassImp(HRpcCalSim)

void HRpcCalSim::clear(void) {
  // clears the object
  HRpcCalSim::clear();
  RefL       = RefR       = -1;
  RefLDgtr   = RefRDgtr   = -1;
  TrackL     = TrackR     = -1;
  TrackLDgtr = TrackRDgtr = -1;
}
