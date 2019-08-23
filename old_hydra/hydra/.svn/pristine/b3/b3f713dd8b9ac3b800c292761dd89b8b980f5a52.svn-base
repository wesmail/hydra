// @(#)$Id: hshowerhitIPU.cc,v 1.1 2002-10-28 14:42:38 jaskula Exp $
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

inline Int_t HShowerHitIPU::clear()
{
    m_bCharge = 0;
    m_nSum0   = 0;
    m_nSum1   = 0;
    m_nSum2   = 0;

    m_nSector = -1;
    m_nRow    = -1;
    m_nCol    = -1;

    m_bFlag   = 0;

    return 1;
}

// -----------------------------------------------------------------------------

inline void HShowerHitIPU::setSum(Int_t i, UShort_t nSum)
{
    switch(i)
    {
        case 0: m_nSum0 = nSum; break;
        case 1: m_nSum1 = nSum; break;
        case 2: m_nSum2 = nSum; break;
        default: break;
    }

    Warning("setSum", "index %d out of bounds", i);
}

// -----------------------------------------------------------------------------

inline UShort_t HShowerHitIPU::getSum(Int_t i)
{
    switch(i)
    {
        case 0: return m_nSum0;
        case 1: return m_nSum1;
        case 2: return m_nSum2;
        default: break;
    }

    Warning("getSum", "index %d out of bounds", i);

    return 0;
}

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

// -----------------------------------------------------------------------------

inline Int_t HShowerHitIPU::getNLocationIndex(void)
{
    return 4;
}

// -----------------------------------------------------------------------------

inline Int_t HShowerHitIPU::getLocationIndex(Int_t i)
{
    switch(i)
    {
        case 0 : return getSector(); break;
        case 1 : return 0; break;
        case 2 : return getRow(); break;
        case 3 : return getCol(); break;
    }

    return -1;
}

