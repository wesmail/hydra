// @(#)$Id: hpidparasciifileio.cc,v 1.3 2008-09-18 13:06:21 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidParAsciiFileIo
//
// Class for PID parameter input/output from/into ASCII file
//
// It is derived from the base class HDetParAsciiFileIo
// and uses interface of HParCond to write and read data
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidparasciifileio.h"
#include "hpidparcont.h"

#include "TClass.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParAsciiFileIo)

// -----------------------------------------------------------------------------

HPidParAsciiFileIo::HPidParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f)
{
// constructor sets the name of the detector I/O "PidParIo"

    fName = PID_PAR_IO_NAME;
}

// -----------------------------------------------------------------------------

Bool_t HPidParAsciiFileIo::init(HParSet* pPar)
{
// Load data from ASCII file using interface of HParCond
// if pPar inherits from this class

    if(pPar == NULL)
    {
        Error("init", "pPar == NULL");
        return kFALSE;
    }

    if(pPar->IsA()->InheritsFrom("HParCond") == kFALSE)
    {
        Error("init", "%s does not inherits from HParCond",
                        pPar->IsA()->GetName());
        return kFALSE;
    }

    return readCond((HParCond *) pPar);
}

// -----------------------------------------------------------------------------

Int_t HPidParAsciiFileIo::write(HParSet* pPar)
{
// Write data from ASCII file using interface of HParCond
// if pPar inherits from this class

    if(pPar == NULL)
    {
        Error("write", "pPar == NULL");
        return 0;
    }

    if(pPar->IsA()->InheritsFrom("HParCond") == kFALSE)
    {
        Error("write", "%s does not inherits from HParCond",
                        pPar->IsA()->GetName());
        return 0;
    }

    return writeCond((HParCond *) pPar);
}
