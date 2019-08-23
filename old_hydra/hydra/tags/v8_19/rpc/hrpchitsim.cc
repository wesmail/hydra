//*-- AUTHOR : Diego Gonzalez-Diaz 17/12/2007

//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// HRpcHitSim                                                                //
// this is the hit data level for simulated data                             //
// it stores 4 references to hgeantrpc objects and the corresponding track   //
///////////////////////////////////////////////////////////////////////////////


#include "hrpchitsim.h"

ClassImp(HRpcHitSim)

void HRpcHitSim::clear(void) {
  HRpcHit::clear();
  RefL       = RefR       = -1;
  RefLDgtr   = RefRDgtr   = -1;
  TrackL     = TrackR     = -1;
  TrackLDgtr = TrackRDgtr = -1;
}
