#include "hmuEmuleptonsSim.h"

///////////////////////////////////////////////////////////////////////////
//HMUEMULeptonsSim
// 
///////////////////////////////////////////////////////////////////////////

HMUEMULeptonsSim::HMUEMULeptonsSim(){
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0; 
  iSector=0;
  iFlag=0; 
  fMom=0.; 
}

HMUEMULeptonsSim::~HMUEMULeptonsSim(){
   fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
   iDetBit=0; 
   iSector=0;
   iFlag=0; 
   fMom=0.; 
}

HMUEMUDiLeptonsSim::HMUEMUDiLeptonsSim(){
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}
HMUEMUDiLeptonsSim::~HMUEMUDiLeptonsSim(){
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}



ClassImp(HMUEMULeptonsSim)
ClassImp(HMUEMUDiLeptonsSim)
