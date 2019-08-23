// @(#)$Id: hshowercalIPU.cc,v 1.1 2002-10-28 14:42:38 jaskula Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCal

#pragma implementation
#include "hshowercalIPU.h"

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

inline int HShowerCalIPU::clear()
{
//clearing parameters

   m_bCharge = 0;
   m_nSector = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;

   return 1;
}

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

// -----------------------------------------------------------------------------

inline Int_t HShowerCalIPU::getNLocationIndex(void)
{
    return 4;
}

// -----------------------------------------------------------------------------

inline Int_t HShowerCalIPU::getLocationIndex(Int_t i)
{
    switch(i)
    {
        case 0 : return getSector(); break;
        case 1 : return getModule(); break;
        case 2 : return getRow(); break;
        case 3 : return getCol(); break;
    }

    return -1;
}
