//*-- Author : Alberica Toia
//*-- Modified : Joern Wuestenfeld 03/23/2005
//*-- Version: $Id: hmatchurich.cc,v 1.3 2005-07-15 09:00:24 wuestenf Exp $

#include "hmatchurich.h"

//
// This data container holds all information from the MatchingUnit that
// is available for the RICH detector.
//
// The expressions "row" and "column" refer to the hardware
// notation. The standard HADES "row" and "column" are the other
// way round
// The "X", "Y", "Theta" and "Phi" are consistent with
// the standard HADES notation.
//
// The access to the variables is done via the following pulic functions:
// To get the content of a variable:
// Int_t getFifo(void)
//		Get the number of the FIFO that has computed the ring finder
// Int_t getColumnPattern(void)
//		Get the pattern that holds the y information of several hits
// Int_t getSegmentId(void)
//		Get sector number (0 - 5)
// Int_t getSector(void)
//		Get sector number (0 - 5), smae as above, provided for convinience
// Int_t getRowNb(void)
//		Get row + offset
// Int_t getRow(void)
//		Get y coor"muEmulationSim.h"dinate of ring center in hardware notation
// Int_t getColumn(void)
//		Get x coordinate of ring center in hardware notation
// Float_t getTheta(void)
//		Get phi coordinate of ring center in lab coordinate system
// Float_t getPhi(void)
//		Get theta coordinate of ring center in lab coordinate system
// Int_t getX(void)
//		Get x coordinate of ring center in standard detector coordinate system
// Int_t getY(void)
//		Get y coordinate of ring center in standard detector coordinate system
//
// To set a variable:
// void setFifo(Int_t fifo)
//		Set the number of the FIFO tha has computed the ring finder.
// void setColumnPattern(Int_t colPat)
//		Set the pattern that holds the y information of several hits.
// void setSegmentId(Int_t segId)
//		Set the sector number.
// void setRowNb(Int_t row)
//		Set the row + offset address.
// void setColumn(Int_t col)
//		Set the column number.
// void setRow(Int_t row)
//		Set the row number.
// void setTheta(Float_t th)
//		Set the theta coordinate of the ring center in lab coordinate system.
// void setPhi(Float_t ph)
//		Set the phi coordinate of the ring center in lab coordinate system.
// void setX(Int_t px)
//		Set the x coordinate of the ring center in lab coordinate sytem.
// void setY(Int_t py)
//		Set the y coordinate of the ring center in lab coordinate system.
//
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//

HMatchURich::HMatchURich(void)
{
  // Default constructor (empty)
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

HMatchURich::~HMatchURich(void)
{
  // Destructor (empty)
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

ClassImp(HMatchURich)
