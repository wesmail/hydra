//*-- Author : Laura Fabbietti
///////////////////////////////////////////////////////////////////////////////////////////
//_________________________________________________________________________________________
//
//  HShowerRawMatrSim
//  this class creates the Matrix Category to store simulated raw data. 
//  It differs from the HShowerRaw (Linear Cat) class because of the 2 additional
//  data members nTrack1, nTrack2.
//
////////////////////////////////////////////////////////////////////////////////
#pragma implementation 
#include "hshowerrawsim.h"

ClassImp(HShowerRawSim)
ClassImp(HShowerRawMatrSim)

 void HShowerRawSim::clear() {
  // clears the object
  HShowerRawSim::clear();
  nTrack1 = nTrack2 = 0;
}
