// File: hrichparasciifileio.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hades.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hrichparasciifileio.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichParAsciiFileIo
//
//  For plain i/o ascii. Not used in RICH.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichParAsciiFileIo)

//----------------------------------------------------------------------------
HRichParAsciiFileIo::HRichParAsciiFileIo(fstream* f) 
                            : HDetParAsciiFileIo(f) {
  fName="HRichParIo";
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();

  if (pFile) {
  }
  cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichParAsciiFileIo::write(HParSet* pPar) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
  }
  cerr<<name<<" could not be written to Asci file"<<endl;
  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}
//============================================================================

