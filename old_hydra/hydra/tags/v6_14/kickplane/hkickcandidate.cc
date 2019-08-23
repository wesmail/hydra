//*-- Author : M.Sanchez
//*-- Modified : 22.08.2000
#include "hkickcandidate.h"

////////////////////////////////////////////////////////////
//HKickCandidate
//
//   Each of the candidate combinations  evaluated by the software 
//algorithm. This class stores the results of the evaluation
/////////////////////////////////////////////////////////////
HKickCandidate::HKickCandidate(void) {
  fSector = 0;
  fP = -1;      //momentum
  fPolarity = 0;    //Polarity 
  fErrP = 0; //Error in p
  fPTof = -1;
  fPTofErr = 0;
  fAssigned = kFALSE; //Candidate was accepted?
  fMass = -10; //Assigned mass in MeV/c^2 or -1.
  fXPull = 1000; //DeltaX / SigmaX
  fPID = 0; 
  fDenomin = 0; //Deniminator.
  fDist = -1;
  fMETAdir.setX(0.0);
  fMETAdir.setY(0.0);
  fMETAdir.setZ(1.0);
  fFlag = -1;
  fClAccept = kTRUE;
  fMETAoverlap = kFALSE;
  fEloss = -1.0;
  beta = -1.;
  setPosition1(0.,0.,0.);
}

HKickCandidate::HKickCandidate(HKickCandidate &can) {
  //Copy constructor
  fSector = can.fSector;
  fP = can.fP;      //momentum
  fPolarity = can.fPolarity;    //Polarity 
  fErrP = can.fErrP; //Error in p
  fPTof = can.fPTof;
  fPTofErr = can.fPTofErr;
  fAssigned = can.fAssigned; //Candidate was accepted?
  fMass = can.fMass; //Assigned mass in MeV/c^2 or -1.
  fXPull = can.fXPull; //DeltaX / SigmaX
  fPID = can.fPID; 
  fDenomin = can.fDenomin; //Deniminator.
  fDist = can.fDist;
  fMETAdir = can.fMETAdir;
  fClAccept = can.fClAccept;
  fMETAoverlap = can.fMETAoverlap;
  fEloss = can.fEloss;
  beta = can.beta;

  setPosition1(can.fX,can.fY,can.fZ);
}

ClassImp(HKickCandidate)

