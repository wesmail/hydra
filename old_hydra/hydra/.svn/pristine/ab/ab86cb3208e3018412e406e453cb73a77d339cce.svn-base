//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 06/20/2005
//*-- Version: $Id: hmuEmuleptons.cc,v 1.6 2006-08-12 13:05:56 halo Exp $

#include "hmuEmuleptons.h"

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
//
//HMUEMULeptons
//
// Datacontainer for leptons from Matching Unit emulation
//
// The access to the variables is done via the following public functions.
// The names of the function are selfexplanatory.
// To get the content of a variable:
//  Int_t getDetBit(void)
//		Get flag for META detector. (=0 for TOF, =1 for SHOWER)
//  Int_t getSector(void)
//		Get sector number (0 - 5).
//  Float_t getThetaRich(void)
//		Get theta coordinate of RICH hit in lab coordinate system
//  Float_t getPhiRich(void)
//		Get phi coordinate of RICH hit in lab coordinate sytem.
//  Float_t getThetaMeta(void)
//		Get theta coordinate of META in lab coordinate sytem.
//  Float_t getPhiMeta(void)
//		Get phi coordinate of META in lab coordinate system.
//  Int_t getFlag(void)
//		Get lepton flag (=0 for positrons, =1 for electrons).
//  Float_t getMom(void)
//		Get momentum (0 - 255).
//
// To set the contents of a variable:
//  void setDetBit( Int_t db )
//		Set flag for META detector (db=0 for TOF, db=1 for SHOWER).
//  void setSector( Int_t sec )
//		Set sector (0 - 5).
//  void setThetaRich( Float_t thetaRich )
//		Set theta coordinate of RICH in lab coordinate system.
//  void setPhiRich( Float_t phiRich )
//		Set phi coordinate of RICH in lab coordinate system.
//  void setThetaMeta( Float_t thetaMeta )
//		Set theta coordinate of META in lab coordinate system.
//  void setPhiMeta( Float_t phiMeta )
//		Set phi coordinate of META in lab coordinate system.
//  void setEFlag( Int_t flag)
//		Set lepton flag (flag=0 for positrons, flag=1 for electrons).
//  void setMomentum (Float_t mom)
//		Set momentum (0 - 255).
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//
///////////////////////////////////////////////////////////////////////////

HMUEMULeptons::HMUEMULeptons(){
// Default constructor. Sets all variables to zero.
//
// Input parameters:
//	none
//
// Output parameters:
//	none
//
// Return code:
//	none
  fPhiRich=fThetaRich=fPhiMeta=fThetaMeta=0.0;
  iDetBit=0; 
  iSector=0;
  iFlag=0; 
  fMom=0.; 
}

HMUEMULeptons::~HMUEMULeptons(){
// Destructor
//
// Input parameters:
//	none
//
// Output parameters:
//	none
//
// Return code:
//	none
}

///////////////////////////////////////////////////////////////////////////
//
//HMUEMUDiLeptons
//
// Datacontainer for leptons from Matching Unit emulation.
//
// The access to the variables is done via the following public functions.
// The names of the function are selfexplanatory.
// To get the content of a variable:
//  Int_t getN1(void)
//		Get the index of the first lepton used for this dilepton.
//  Int_t getN2(void)
//		Get the index of the second lepton used for this dilepton.
//  Float_t getMass(void)
//		Get the invariant mass of the dilepton.
//  Float_t getOpangle(void)
// Get the opening angle between the two leptons.
//
// To set the contents of a variable:
//  void setN1( Int_t n )
//		Set the index of the first lepton used for this dilepton.
//  void setN2( Int_t n ) 
//		Set the index of the second lepton used by this dilepton.
//  void setMass( Float_t m )
//		Set the invariant mass of this dilepton.
//  void setOpangle( Float_t op ) 
//		Set the opening angle of the two leptons.
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//
///////////////////////////////////////////////////////////////////////////

HMUEMUDiLeptons::HMUEMUDiLeptons(void)
{
// Default constructor. Sets all variables to zero.
//
// Input parameters:
//	none
//
// Output parameters:
//	none
//
// Return code:
//	none
  iN2=iN1=0;
  fMass=0;
  opangle=0;
}
HMUEMUDiLeptons::~HMUEMUDiLeptons(void)
{
// Destructor
//
// Input parameters:
//	none
//
// Output parameters:
//	none
//
// Return code:
//	none
}



ClassImp(HMUEMULeptons)
ClassImp(HMUEMUDiLeptons)
