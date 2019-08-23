#include "hmuEmuleptons.h"

///////////////////////////////////////////////////////////////////////////
//HMUEMULeptons
// 
///////////////////////////////////////////////////////////////////////////

HMUEMULeptons::HMUEMULeptons(){
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0; 
  iSector=0;
  iFlag=0; 
  fMom=0.; 
}

HMUEMULeptons::~HMUEMULeptons(){
   fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
   iDetBit=0; 
   iSector=0;
   iFlag=0; 
   fMom=0.; 
}

HMUEMUDiLeptons::HMUEMUDiLeptons(){
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}
HMUEMUDiLeptons::~HMUEMUDiLeptons(){
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}



ClassImp(HMUEMULeptons)
ClassImp(HMUEMUDiLeptons)
