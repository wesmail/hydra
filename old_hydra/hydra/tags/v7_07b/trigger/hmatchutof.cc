//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 04/01/2005
//*-- Version: $Id: hmatchutof.cc,v 1.3 2005-07-15 09:00:24 wuestenf Exp $

#include "hmatchutof.h"

//
// HMatchUTof
//
// This data container holds all information from the MatchingUnit that
// is available for the SHOWER detector.
//
// The access to the variables is done via the following pulic functions:
// To get the content of a variable:
//  Int_t getPID(void)
//		Get the identifier of the particle (=1 for leptons, =2 for other particles).
//  Int_t getSector(void)
//		Get the sector number (0 - 6)
//  Float_t getTheta()
//		Get the theta coordinate of the hit in lab coordinate system.
//  Float_t getTime()
//		Get the time of flight (ns).
//  Int_t getErrTheta()
//		Get the errror on the theta coordinate.
//  Float_t getPhi()
//		Get the phi coordinate of the hit in lab coordinate system.
//  Int_t getErrPhi()
//		Get the error on the phi coordinate.
//  Int_t getiTheta()
//		Get the theta coordinate of the hit in lab coordinate system (0 - 255).
//  Int_t getiPhi()
//		Get the phi coordinate of the hit in lab coordinate system (0 - 255).
//
// To set a variable:
//  void setTheta(Float_t val,Int_t error)
//		Set the theta coordinate and the error on theta in lab coordinate system.
//  void setPhi(Float_t val,Int_t error) 
//		Set the phi coordinate and the error on phi in lab coordinate system.
//  void setiTheta(Int_t ith) 
//		Set the theta coordinate in lab coordinate system (0 - 255).
//  void setiPhi(Int_t iph) 
//		Set the phi coordinate in lab coordinate system (0 - 255).
//  void setTime(Float_t t)
// 		Set the time of flight (ns).
//  void setSec(Int_t s)
//		Set the sector number (0 - 5)
//  void setPID(Int_t p)
//		Set the identifier of the particle (=1 for leptons, =2 for other particles)-
//
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//

ClassImp(HMatchUTof)
