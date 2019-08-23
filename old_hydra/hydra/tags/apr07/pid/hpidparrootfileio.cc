// @(#)$Id: hpidparrootfileio.cc,v 1.2 2006-08-12 12:49:36 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidParRootFileIo
//
// Class for PID parameter input/output from/into ROOT file
//
// It is derived from the base class HDetParRootFileIo and
// inherits from it basic functions e.g. write(...)
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidparrootfileio.h"
#include "hpidparcont.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParRootFileIo)

// -----------------------------------------------------------------------------

HPidParRootFileIo::HPidParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f)
{
// constructor sets the name of the detector I/O "PidParIo"

    fName = PID_PAR_IO_NAME;
}
