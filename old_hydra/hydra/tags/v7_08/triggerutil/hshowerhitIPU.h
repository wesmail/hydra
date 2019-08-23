// @(#)$Id: hshowerhitIPU.h,v 1.2 2003-11-21 15:07:28 halo Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCalIPU

#ifndef HShowerHitIPU_H
#define HShowerHitIPU_H
#pragma interface

// -----------------------------------------------------------------------------

#include "hlocateddataobject.h"

// -----------------------------------------------------------------------------

class HShowerHitIPU : public HLocatedDataObject
{
public:
    HShowerHitIPU(void)                 { clear();              }

    inline Int_t  clear();
    Byte_t   getCharge() const          { return m_bCharge;     }
    void     setCharge(Byte_t bCharge)  { m_bCharge = bCharge;  }

    Char_t   getSector(void)            { return m_nSector;     }
    Char_t   getRow(void)               { return m_nRow;        }
    Char_t   getCol(void)               { return m_nCol;        }

    void     setSector(Char_t s)        { m_nSector = s;        }
    void     setRow(Char_t r)           { m_nRow = r;           }
    void     setCol(Char_t c)           { m_nCol = c;           }

    UShort_t getSum0() const            { return m_nSum0;       }
    void     setSum0(UShort_t nSum0)    { m_nSum0 = nSum0;      }
    UShort_t getSum1() const            { return m_nSum1;       }
    void     setSum1(UShort_t nSum1)    { m_nSum1 = nSum1;      }
    UShort_t getSum2() const            { return m_nSum2;       }
    void     setSum2(UShort_t nSum2)    { m_nSum2 = nSum2;      }

    inline void     setSum(Int_t i, UShort_t nSum);
    inline UShort_t getSum(Int_t i);

    Byte_t   getFlag() const            { return m_bFlag;       }
    void     setFlag(Byte_t bFlag)      { m_bFlag = bFlag;      }

    Int_t    calcAddress(void);
    inline Int_t    getNLocationIndex(void);
    inline Int_t    getLocationIndex(Int_t i);

    // -------------------------------------------------------------------------

private:

    Char_t   m_nSector;  // position of the pad
    Char_t   m_nRow;
    Char_t   m_nCol;

    Byte_t   m_bCharge;  // charge on the pad
    UShort_t m_nSum0;    // sums from 9 pads around
    UShort_t m_nSum1;
    UShort_t m_nSum2;

    Byte_t   m_bFlag;    // logical sum of positive conditions

    // -------------------------------------------------------------------------

    ClassDef(HShowerHitIPU, 1) //ROOT extension
};
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

#endif
