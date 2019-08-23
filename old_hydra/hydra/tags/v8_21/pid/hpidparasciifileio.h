// @(#)$Id: hpidparasciifileio.h,v 1.2 2002-11-04 02:42:23 jaskula Exp $
#ifndef HPIDPARASCIIFILEIO_H
#define HPIDPARASCIIFILEIO_H

// -----------------------------------------------------------------------------

#include "hdetparasciifileio.h"

// -----------------------------------------------------------------------------

class HPidParAsciiFileIo : public HDetParAsciiFileIo
{
public:
    HPidParAsciiFileIo(fstream *f);

    virtual Bool_t init(HParSet *pPar);
    virtual Bool_t init(HParSet *pPar, Int_t *) { return init(pPar);      }

    virtual Int_t  write(HParSet *pPar);

    // -------------------------------------------------------------------------

    ClassDef(HPidParAsciiFileIo, 0) // Pid parameter I/O from/to ASCII file
};

// -----------------------------------------------------------------------------

#endif  /*!HPIDPARASCIIFILEIO_H*/







