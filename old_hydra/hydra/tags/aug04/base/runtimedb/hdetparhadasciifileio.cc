//*-- AUTHOR : Ilse Koenig
//*-- modified : 27/05/99 by Ilse Koenig


/////////////////////////////////////////////////////////////
// HDetParHadAsciiFileIo
//
// Class for parameter input/output from/into Ascii file
// Base class for all detectors 
//
/////////////////////////////////////////////////////////////

#include "hparhadasciifileio.h"
#include "hdetparhadasciifileio.h"
#include "hdetgeompar.h"
#include "hgeomcompositevolume.h"
#include "hgeomshapes.h"

ClassImp(HDetParHadAsciiFileIo)

HDetParHadAsciiFileIo::HDetParHadAsciiFileIo(HParHadAsciiFileIo* f) {
  // constructor
  pFile=f;
}
