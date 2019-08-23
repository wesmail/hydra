//*-- AUTHOR : Denis Bertini
//*-- Last modified : 15/12/98 by Ilse Koenig

/////////////////////////////////////////////////////////////
//HShowerParAsciiFileIo
//
//  Class for Mdc parameter input/output from/into Asci file
//
/////////////////////////////////////////////////////////////

#include "hshowerparasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerdetector.h"
#include <iostream.h>

ClassImp(HShowerParAsciiFileIo)

HShowerParAsciiFileIo::HShowerParAsciiFileIo(fstream* f) 
                            : HDetParAsciiFileIo(f) {
  fName="HShowerParIo";
}


Bool_t HShowerParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();

  if (pFile) {
  }
  cerr<<"initialization of "<<name<<" not possible from ASCII file!"<<endl;
  return kFALSE;
}


Int_t HShowerParAsciiFileIo::write(HParSet* pPar) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
  }
  cerr<<name<<" could not be written to Asci file"<<endl;
  return kFALSE;
}


Bool_t HShowerParAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}


