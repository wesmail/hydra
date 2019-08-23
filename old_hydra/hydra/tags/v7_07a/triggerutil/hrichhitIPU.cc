//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005

#include "hrichhitIPU.h"

ClassImp(HRichHitIPU)

HRichHitIPU::HRichHitIPU(void)
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
  //   none
  iRingX = 0;
  iRingY = 0;
  iRingQuality = 0;
  iVetoQuality = 0;
  nSector = 0;
  nPads = 0;
  fTheta =0.;
  fPhi =0.;
  DataWord=999;
}

HRichHitIPU::HRichHitIPU(const HRichHitIPU& source)
{
  // Copy constructor
  //
  // Input parameters:
  //   const HRichHitIPU& source
  //      Pointer to object from which to copy.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  iRingX = source.iRingX;
  iRingY = source.iRingY;
  iRingQuality = source.iRingQuality;
  iVetoQuality = source.iVetoQuality;
  nSector = source.nSector;
  nPads = source.nPads;
  fTheta =source.fTheta;
  fPhi =source.fPhi;
  DataWord=source.DataWord;
}

HRichHitIPU& HRichHitIPU::operator=(const HRichHitIPU& source)
{
  // Operator =
  // Assigns the values from one object to this one.
  //
  // Input parameters:
  //   const HRichHitIPU& source
  //      Pointer to object from which to copy.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Pointer to this object.
  if (this != &source)
    {
      iRingX = source.iRingX;
      iRingY = source.iRingY;
      iRingQuality = source.iRingQuality;
      iVetoQuality = source.iVetoQuality;
      nSector = source.nSector;
      nPads = source.nPads;
      fTheta =source.fTheta;
      fPhi =source.fPhi;
      DataWord=source.DataWord;
    }
  return *this;
}


void HRichHitIPU::Reset(void)
{
  // Resets all variables to the defaults.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  iRingX = 0;
  iRingY = 0;
  iRingQuality = 0;
  iVetoQuality = 0;
  nSector = 0;
  nPads = 0;
  fTheta =0.;
  fPhi =0.;
  DataWord=0;
}
