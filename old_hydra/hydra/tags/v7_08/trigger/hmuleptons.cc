//*-- Author : Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: hmuleptons.cc,v 1.5 2005-07-15 09:00:24 wuestenf Exp $

#include "hmuleptons.h"

 //////////////////////////////////////////////////////////////////////////////
 //
 // HMULeptons
 //
 // Datacontainer holding all MatchingUnit information abaout found leptons.
 //
 // The access to the variables is done via the following public functions:
 // To get the content of a variable:
 // Int_t getRichNr(void)
 //		Get the index into HMatchURich category
 // Int_t getMetaNr(void)
 //		Get index into HMatchUTof category
 // Int_t getDetBit(void)
 //		Get flag for META detector (=0 for TOF, =1 for SHOWER)
 // Int_t getSector(void) 
 //		Get sector number in stadndard notation (0 - 5)
 // Float_t getThetaRich(void)
 //		Get theta coordinate of RICH hit in lab coordinate system
 // Float_t getPhiRich(void)
 //		Get phi coordinate of RICH hit in lab coordinate system
 // Float_t getThetaMeta(void)
 //		Get theta coordinate of META hit in lab coordinate system
 // Float_t getPhiMeta(void)
 //		Get phi coordinate of META hit in lab coordinate system
 // Int_t getFlag(void)
 //		Get lepton flag (=0 for positrons, =1 for electrons)
 // Float_t getMom(void)
 //		Get momentum (0 - 255)
 //
 // To set a variable:
 // void setRichNr( Int_t rnr )
 //		Set the index of the corresponding HMatchURich category entry
 // void setMetaNr( Int_t rmr )
 //		Set the index of the corresponding HMatchUTof category entry
 // void setDetBit( Int_t db )
 //		Set flag for Meta system (=0 for TOF, =1 for Shower)
 // void setSector( Int_t sec )
 //		Set the sector (0 - 5)
 // void setThetaRich( Float_t theta )
 //		Set the theta angle of the RICH hit in lab coordinate system
 // void setPhiRich( Float_t phi )
 //		Set the phi angle of the RICH hit in lab coordinate system
 // void setThetaMeta( Float_t theta )
 //		Set theta angle of the META hit in lab coordinate system
 // void setPhiMeta( Float_t phi ) { fPhiMeta = phi; }
 //		Set the phi angle of the META hit in lab coordinate system
 // void setFlag( Int_t flag)
 //		Set lepton flag (=0 for positron, =1 for electron)
 // void setMomentum (Float_t mom)
 //		Set momentum (0 - 255)
 // If you use these functions, make sure your input parameter is in the rigth
 // coordinate system and units! No checking or transformations are done.
 //
 //////////////////////////////////////////////////////////////////////////////
HMULeptons::HMULeptons(void){
//Default constructor. Sets theta and phi variables to 0.0.
  fPhiRich=fThetaRich=0.0;
  fPhiMeta=fThetaMeta=0.0;
}

///////////////////////////////////////////////////////////////////////////////
//
// HMUDiLeptons
//
// Datacontainer holding all MatchingUnit information about DiLeptons.
// 
// The access to the variables is done via the following pulic functions:
// To get the content of a variable:
// Int_t getN1(void)
//		Get index of first lepton from HMULepton category used for thi dilepton
// Int_t getN2(void)
//		Get index of second lepton from HMULepton category used for thi dilepton
// Float_t getMass(void)
//		Get invariant mass of dilepton
//
//To set a variable:
// void setN1( Int_t n )
// void setN2( Int_t n )
// void setMass( Float_t x )  
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//
///////////////////////////////////////////////////////////////////////////////
HMUDiLeptons::HMUDiLeptons(void){
// Default constructor. Sets indexes to 0.
  iN2=iN1=0;
}



ClassImp(HMULeptons)
ClassImp(HMUDiLeptons)
