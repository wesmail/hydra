// @(#)$Id: hshowercalIPU.h,v 1.3 2008-05-09 16:19:00 halo Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCal

#ifndef HShowerCalIPU_H
#define HShowerCalIPU_H
#pragma interface

// -----------------------------------------------------------------------------

#include "hlocateddataobject.h"

// -----------------------------------------------------------------------------

class HShowerCalIPU : public HLocatedDataObject
{
public:
    HShowerCalIPU(void)
                : m_nSector(-1), m_nModule(-1), m_nRow(-1), m_nCol(-1),
                    m_bCharge(0) {}

    // initialization could be used e.g. to clear the data element
    HShowerCalIPU(const Byte_t bCharge)
                : m_nSector(-1), m_nModule(-1), m_nRow(-1), m_nCol(-1),
                    m_bCharge(bCharge) {}

    inline Int_t   clear();
    Byte_t  getCharge() const           { return m_bCharge;     }
    void    setCharge(Byte_t bCharge)   { m_bCharge = bCharge;  }

    Int_t   getAddress(void)            { return calcAddress(); }
    Char_t  getSector(void)             { return m_nSector;     }
    Char_t  getModule(void)             { return m_nModule;     }
    Char_t  getRow(void)                { return m_nRow;        }
    Char_t  getCol(void)                { return m_nCol;        }

    void    setSector(Char_t s)         { m_nSector = s; }
    void    setModule(Char_t m)         { m_nModule = m; }
    void    setRow(Char_t r)            { m_nRow = r;    }
    void    setCol(Char_t c)            { m_nCol = c;    }

    inline Int_t   getNLocationIndex(void);
    inline Int_t   getLocationIndex(Int_t i);

    // -------------------------------------------------------------------------

private:
    Int_t calcAddress();

    Char_t m_nSector;   //position of the pad
    Char_t m_nModule;
    Char_t m_nRow;
    Char_t m_nCol;

    Byte_t m_bCharge;  //calibrated charge in the pad

    // -------------------------------------------------------------------------

    ClassDef(HShowerCalIPU, 1) //ROOT extension
};
inline Int_t HShowerCalIPU::clear()
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


#endif
