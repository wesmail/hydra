//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: hmuEmuleptonsSim.cc,v 1.4 2006-08-12 13:05:56 halo Exp $

#include "hmuEmuleptonsSim.h"

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
//
// HMUEMULeptonsSim
//
///////////////////////////////////////////////////////////////////////////

HMUEMULeptonsSim::HMUEMULeptonsSim(void)
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

HMUEMULeptonsSim::~HMUEMULeptonsSim(void)
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
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0;
  iSector=0;
  iFlag=0;
  fMom=0.;
}

HMUEMUDiLeptonsSim::HMUEMUDiLeptonsSim(void)
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

HMUEMUDiLeptonsSim::~HMUEMUDiLeptonsSim(void)
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
}

ClassImp(HMUEMULeptonsSim)
ClassImp(HMUEMUDiLeptonsSim)
