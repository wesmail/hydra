// @(#)$Id: hshowerhitIPU.cc,v 1.2 2003-11-21 15:07:28 halo Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCalIPU

#pragma implementation
#include "hshowerhitIPU.h"

////////////////////////////////////////////////////////////////////////////////
//
// HShowerHitIPU
//
// Structure for Hit Level found by the IPU
//
////////////////////////////////////////////////////////////////////////////////

ClassImp(HShowerHitIPU)




// -----------------------------------------------------------------------------

Int_t HShowerHitIPU::calcAddress(void)
{
Char_t s = (m_nSector) ? m_nSector : 6;
Int_t  nAddress;

    nAddress = 100000 * s;
    //nAddress += 10000 * m_nModule; // module := 0 !!!
    nAddress += 100 * m_nRow;
    nAddress += m_nCol;

    return nAddress;
}


