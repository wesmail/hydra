//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 04/01/2005
//*-- Version: $Id: hmatchushower.cc,v 1.3 2005-07-15 09:00:24 wuestenf Exp $

#include "hmatchushower.h"

//
// HMatchUShower
//
// This data container holds all information from the MatchingUnit that
// is available for the SHOWER detector.
//
// The access to the variables is done via the following pulic functions:
// To get the content of a variable:
//  Int_t getBuildId(void)
//		Get the number of the IPC module.
//  Int_t getColumn(void)
//		Get the x coordinate of the hit in detector coordinate system.
//  Int_t getRowPattern(void)
//		Get the pattern taht encodes the y information of several hits.
//  Int_t getRow(void) 
//		Get the y coordinate of the hit in detector coordinate system.
//  Int_t getRowOffset(void)}
//		Get the row offset.
//  Int_t getSector(void)
//		Get the sector number (0 - 5).
//  Float_t getTheta(void)
//		Get the theta coordinate of the hit in lab coordinate system.
//  Float_t getPhi(void)
//		Get the phi coordinate of the hit in lab coordinate system.
//
// To set a variable:
//  void setBuildID(Int_t buildId)
//		Set the number of the IPC module.
//  void setColumn(Int_t col)
//		Set the x coordinate of the hit in detector coordinate system.
//  void setRowPattern(Int_t rowPat)
//		Set the pattern taht encodes the y information of several hits.
//  void setRow(Int_t row)
//		Set the y coordinate of the hit in detector coordinate system.
//  void setRowOffset(Int_t row)
//		Set the row offset.
//  void setSector(Int_t i)
//		Set the sector number (0 - 5).
//  void setTheta(Float_t th)
// 		Set the theta coordinate of the hit in lab coordinate system.
//  void setPhi(Float_t ph)
//	Set the phi coordinate of the hit in lab coordinate system.
//
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//

HMatchUShower::HMatchUShower(void) {
// Default constructor.
//
// Input parameters:
//		none
//
// Output parameters:
// 		none
//
//Return code:
//		none
}

HMatchUShower::~HMatchUShower(void) {
// Destructor
//
// Input parameters:
//		none
//
// Output parameters:
// 		none
//
//Return code:
//		none
}

ClassImp(HMatchUShower)
