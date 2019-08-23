//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: hmuEmuleptonsExp.cc,v 1.3 2005-07-15 09:00:24 wuestenf Exp $

#include "hmuEmuleptonsExp.h"

///////////////////////////////////////////////////////////////////////////
//
// HMUEMULeptonsExp
//
//
///////////////////////////////////////////////////////////////////////////

HMUEMULeptonsExp::HMUEMULeptonsExp(void)
  : HMUEMULeptons()
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0; 
  iSector=0;
  iFlag=0;
  fMom=0.;
}

HMUEMULeptonsExp::~HMUEMULeptonsExp(void)
{
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0;
  iSector=0;
  iFlag=0;
  fMom=0.;
}

HMUEMUDiLeptonsExp::HMUEMUDiLeptonsExp(void)
  : HMUEMUDiLeptons()
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}
HMUEMUDiLeptonsExp::~HMUEMUDiLeptonsExp(void)
{
  // Dstructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}

ClassImp(HMUEMULeptonsExp)
ClassImp(HMUEMUDiLeptonsExp)
