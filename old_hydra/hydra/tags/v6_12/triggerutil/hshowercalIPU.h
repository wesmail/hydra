// @(#)$Id: hshowercalIPU.h,v 1.1.1.1 2002-11-28 17:49:45 halo Exp $
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

    Int_t   clear();
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

    Int_t   getNLocationIndex(void);
    Int_t   getLocationIndex(Int_t i);

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

#endif
