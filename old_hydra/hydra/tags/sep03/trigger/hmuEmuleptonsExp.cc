#include "hmuEmuleptonsExp.h"

///////////////////////////////////////////////////////////////////////////
//HMUEMULeptonsExp
// 
///////////////////////////////////////////////////////////////////////////

HMUEMULeptonsExp::HMUEMULeptonsExp(){
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0; 
  iSector=0;
  iFlag=0; 
  fMom=0.; 
}

HMUEMULeptonsExp::~HMUEMULeptonsExp(){
   fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
   iDetBit=0; 
   iSector=0;
   iFlag=0; 
   fMom=0.; 
}

HMUEMUDiLeptonsExp::HMUEMUDiLeptonsExp(){
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}
HMUEMUDiLeptonsExp::~HMUEMUDiLeptonsExp(){
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}



ClassImp(HMUEMULeptonsExp)
ClassImp(HMUEMUDiLeptonsExp)
