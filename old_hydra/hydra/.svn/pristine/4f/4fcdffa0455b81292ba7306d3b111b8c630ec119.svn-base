/////////////////////////////////////////////////////////////
//HRichParAsciiFileIo
/////////////////////////////////////////////////////////////

#include "hrichparasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include <iostream.h>

ClassImp(HRichParAsciiFileIo)

HRichParAsciiFileIo::HRichParAsciiFileIo(fstream* f) 
                            : HDetParAsciiFileIo(f) {
  fName="HRichParIo";
}


Bool_t HRichParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();

  if (pFile) {
  }
  cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
  return kFALSE;
}


Int_t HRichParAsciiFileIo::write(HParSet* pPar) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
  }
  cerr<<name<<" could not be written to Asci file"<<endl;
  return kFALSE;
}


Bool_t HRichParAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}


