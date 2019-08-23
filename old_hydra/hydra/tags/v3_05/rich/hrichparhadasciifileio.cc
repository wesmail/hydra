//*-- AUTHOR : Denis Bertini
//*-- Last modified : 15/12/98 by Ilse Koenig

/////////////////////////////////////////////////////////////
//HRichParHadAsciiFileIo
//
//  Class for Mdc parameter input/output from/into Asci file
//
/////////////////////////////////////////////////////////////

#include "hrichparhadasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hrichgeometry.h"
#include "hrichcalpar.h"
#include "hrichdigidetpar.h"
#include "hrichhitfpar.h"
#include "hrichanalysispar.h"
#include <iostream.h>

ClassImp(HRichParHadAsciiFileIo)

HRichParHadAsciiFileIo::HRichParHadAsciiFileIo(HParHadAsciiFileIo* f) 
                            : HDetParHadAsciiFileIo(f) {
  fName="HRichParIo";
}


Bool_t HRichParHadAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("rich par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"RichGeometry")==0) 
                                return read((HRichGeometry*)pPar,set);
    if (strcmp(name,"RichDigiPar")==0) 
                                return read((HRichDigiDetPar*)pPar,set);
    if (strcmp(name,"RichHitFinderParams")==0) 
                                return read((HRichHitFPar*)pPar,set);
    if (strcmp(name,"RichCalPar")==0) 
                                return read((HRichCalPar*)pPar,set);
    if (strcmp(name,"RichAnalysisParameters")==0)
                                return read((HRichAnalysisPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}


Int_t HRichParHadAsciiFileIo::write(HParSet* pPar) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
    if (strcmp(name,"RichGeometry")==0) 
                                return writeFile((HRichGeometry*)pPar);
    if (strcmp(name,"RichDigiPar")==0) 
                                return writeFile((HRichDigiDetPar*)pPar);
    if (strcmp(name,"RichHitFinderParams")==0) 
                                return writeFile((HRichHitFPar*)pPar);
    if (strcmp(name,"RichCalPar")==0) 
                                return writeFile((HRichCalPar*)pPar);
    if (strcmp(name,"RichAnalysisParameters")==0)
                                return writeFile((HRichAnalysisPar*)pPar);
  }

  cerr<<name<<" could not be written to Asci file"<<endl;
  return kFALSE;
}


Bool_t HRichParHadAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}

Bool_t HRichParHadAsciiFileIo::read(HRichGeometry* pPar,Int_t* set) {
  return pPar->initAscii(pFile);
}

Bool_t HRichParHadAsciiFileIo::read(HRichDigiDetPar* pPar,Int_t* set) {
  return pPar->initAscii(pFile);
}

Bool_t HRichParHadAsciiFileIo::read(HRichHitFPar* pPar,Int_t* set) {
  return pPar->initAscii(pFile);
}

Bool_t HRichParHadAsciiFileIo::read(HRichCalPar* pPar,Int_t* set) {
  printf("reading RichCalPar container\n");
  int ret;

  ret = pPar->initAscii(pFile);

  return ret;
}

Bool_t HRichParHadAsciiFileIo::read(HRichAnalysisPar* pPar, Int_t* set) {
 return pPar->initAscii(pFile);
}





Bool_t HRichParHadAsciiFileIo::writeFile(HRichGeometry* pPar) {

  return kTRUE;
} 

Bool_t HRichParHadAsciiFileIo::writeFile(HRichDigiDetPar* pPar) {

  return kTRUE;
}

Bool_t HRichParHadAsciiFileIo::writeFile(HRichHitFPar* pPar) {

  return kTRUE;
}

Bool_t HRichParHadAsciiFileIo::writeFile(HRichCalPar* pPar) {
  printf("writing RichCalPar container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}

Bool_t HRichParHadAsciiFileIo::writeFile(HRichAnalysisPar* pPar) {

 return kTRUE;
}


