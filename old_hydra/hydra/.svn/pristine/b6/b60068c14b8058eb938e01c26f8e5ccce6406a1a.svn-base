//*-- Author : M.Sanchez
//*-- Modified : 22.08.2000
#include "hkickcandidate.h"

////////////////////////////////////////////////////////////
//HKickCandidate
//
//   Each of the candidate combinations  evaluated by the software 
//algorithm. This class stores the results of the evaluation
/////////////////////////////////////////////////////////////
 
HKickCandidate::HKickCandidate(HKickCandidate &can) {
  //Copy constructor
  fP=can.fP;                 //momentum
  fDPhi=can.fDPhi;           //Deflection in azimuthal
  fDTheta=can.fDTheta;       //Deflection in theta
  fPolarity=can. fPolarity;  //Polarity 
  fErrP=can.fErrP;           //Error in p
  fAssigned=can.fAssigned;
  fMass=can.fMass;           //Assigned mass in MeV/c^2 or -1.
  fPhi1=can.fPhi1;           //Phi before magnet
  fGhi1=can.fGhi1;           //Ghi before magnet
  fSector = can.fSector;
  setPosition1(can.fX,can.fY,can.fZ);
  fTheta1 = can.fTheta1;
}

ClassImp(HKickCandidate)
