#ifndef HPIDPARROOTFILEIO_H
#define HPIDPARROOTFILEIO_H

// -----------------------------------------------------------------------------

#include "hdetparrootfileio.h"

// -----------------------------------------------------------------------------

class HParRootFile;

// -----------------------------------------------------------------------------

class HPidParRootFileIo : public HDetParRootFileIo
{
public:
    HPidParRootFileIo(HParRootFile *f);

    virtual Bool_t init(HParSet *pPar)          { return read(pPar); }
    virtual Bool_t init(HParSet *pPar, Int_t *) { return init(pPar); }

    // -------------------------------------------------------------------------

    ClassDef(HPidParRootFileIo, 0) // Pid parameter I/O from/to ROOT file
};

// -----------------------------------------------------------------------------

#endif  /*!HPIDPARROOTFILEIO_H*/







