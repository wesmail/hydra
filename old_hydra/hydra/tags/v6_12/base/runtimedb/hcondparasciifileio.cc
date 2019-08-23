//*-- AUTHOR : Ilse Koenig
//*-- Created : 20/11/2002 by I.Koenig

//////////////////////////////////////////////////////////////////////////////
// HCondParAsciiFileIo
//
// Interface class to ASCII file for input/output of parameters derived
// from HParCond
//
//////////////////////////////////////////////////////////////////////////////

#include "hcondparasciifileio.h"
#include "hparcond.h"

ClassImp(HCondParAsciiFileIo)

HCondParAsciiFileIo::HCondParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor
  // sets the name of the I/O class "HCondParIo"
  // gets the pointer to the ASCII file
  fName="HCondParIo";
}

Bool_t HCondParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls HDetParAsciiFileIo"::readCond(HParCond*,Int_t*)
  if (!pFile) return kFALSE; 
  if (pPar->InheritsFrom("HParCond"))
    return HDetParAsciiFileIo::readCond((HParCond*)pPar);
  Error("HCondParAsciiFileIo::init(HParSet*,Int_t*)",
        "%s does not inherit from HParCond",pPar->GetName());
  return kFALSE;
}

Int_t HCondParAsciiFileIo::write(HParSet* pPar) {
  // calls HDetParAsciiFileIo"::writeCond(HParCond*)
  if (!pFile) return -1; 
  if (pPar->InheritsFrom("HParCond"))
    return HCondParAsciiFileIo::writeCond((HParCond*)pPar);
  Error("HCondParAsciiFileIo::write(HParSet*)",
        "%s does not inherit from HParCond",pPar->GetName());
  return -1;
}
