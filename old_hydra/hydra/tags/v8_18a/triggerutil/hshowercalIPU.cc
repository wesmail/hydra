// @(#)$Id: hshowercalIPU.cc,v 1.4 2006-08-12 13:06:27 halo Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCal

#pragma implementation
#include "hshowercalIPU.h"

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HShowerCalIPU
//
// HShowerCalIPU contains calibrated data of the pre-SHOWER as it is done
// by the IPU.
//
///////////////////////////////////////////////////////////////////////////////

ClassImp(HShowerCalIPU)

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

Int_t HShowerCalIPU::calcAddress(void)
{
//calculation of address of the pad to SMRRCC format
//S - sector; 6 for sector==0
//M - module
//RR - row
//CC - col

  Int_t  nAddress;
  Char_t s = (m_nSector) ? m_nSector : 6;

  nAddress  = 100000 * s;
  nAddress += 10000 * m_nModule;
  nAddress += 100 * m_nRow;
  nAddress += m_nCol;

  return nAddress;
}

