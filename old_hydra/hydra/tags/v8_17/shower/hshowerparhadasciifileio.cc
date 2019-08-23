//*-- AUTHOR : Denis Bertini
//*-- Last modified : 15/12/98 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//HShowerParHadAsciiFileIo
//
//  Class for Shower parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hshowerparhadasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerdetector.h"
#include "hshowergeometry.h"
#include "hshowercalpar.h"
#include "hshowerdigidetpar.h"
#include "hshowerhitfpar.h"
#include "hshowerhist.h"
#include "hshowerunpackparam.h"
#include <iostream> 
#include <iomanip>

ClassImp(HShowerParHadAsciiFileIo)

HShowerParHadAsciiFileIo::HShowerParHadAsciiFileIo(HParHadAsciiFileIo* f) 
                            : HDetParHadAsciiFileIo(f) {
  fName="HShowerParIo";
}


Bool_t HShowerParHadAsciiFileIo::init(HParSet* pPar, Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("Shower par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"ShowerGeometry")==0) 
                                return read((HShowerGeometry*)pPar,set); // always the same 
    if (strcmp(name,"ShowerDigiPar")==0) 
                                return read((HShowerDigiDetPar*)pPar,set);
    if (strcmp(name,"ShowerHitFinderParams")==0) 
                                return read((HShowerHitFPar*)pPar,set);
    if (strcmp(name,"ShowerCalPar")==0) 
                                return read((HShowerCalPar*)pPar,set);
    if (strcmp(name,"ShowerHist")==0) 
                                return read((HShowerHist*)pPar,set);
    if (strcmp(name,"ShowerUnpackParam")==0) 
                                return read((HShowerUnpackParam*)pPar,set);
  }
  cerr<<"Initialization of "<<name<<" not possible from ASCII file!"<<endl;
  return kFALSE;
}


Int_t HShowerParHadAsciiFileIo::write(HParSet* pPar) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
    if (strcmp(name,"ShowerGeometry")==0) 
                                return writeFile((HShowerGeometry*)pPar);
    if (strcmp(name,"ShowerDigiPar")==0) 
                                return writeFile((HShowerDigiDetPar*)pPar);
    if (strcmp(name,"ShowerHitFinderParams")==0) 
                                return writeFile((HShowerHitFPar*)pPar);
    if (strcmp(name,"ShowerCalPar")==0) 
                                return writeFile((HShowerCalPar*)pPar);
    if (strcmp(name,"ShowerHist")==0) 
                                return kTRUE;
    if (strcmp(name,"ShowerUnpackParam")==0) 
                                return writeFile((HShowerUnpackParam*)pPar);
  }

  cerr<<name<<" Could not be written to ASCII file"<<endl;
  return kFALSE;
}


Bool_t HShowerParHadAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerGeometry* pPar, Int_t* set) {
  printf("Reading ShowerGeometry container\n");
  if (!pPar->initAscii(pFile))
    return kFALSE;

  pPar->attachTables();
//  pPar->transToLab();
  pPar->setStatic();
 
  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerDigiDetPar* pPar, Int_t* set) {
  printf("Reading ShowerDigiDetPar container\n");
  pPar->setStatic();
  return pPar->initAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerHitFPar* pPar, Int_t* set) {
  printf("Reading ShowerHitFPar container\n");
  pPar->setStatic();
  return pPar->initAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerCalPar* pPar, Int_t* set) {
  printf("Reading ShowerCalPar container\n");
  Int_t ret;

  pPar->setStatic();
  ret = pPar->initAscii(pFile);

  return ret;
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerHist* pPar, Int_t* set) {
  printf("Reading ShowerHist container\n");
  pPar->setStatic();
  pPar->defaultInit(); //nothing to read
  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerUnpackParam* pPar, Int_t* set) {
  printf("Reading ShowerUnpackParam container\n");
  pPar->setStatic();
  return pPar->initAscii(pFile);
}
 
Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerGeometry* pPar) {
  printf("Writing HShowerGeometry container\n");

  return pPar->writeAscii(pFile);
} 

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerDigiDetPar* pPar) {
  printf("Writing HShowerDigiDetPar container\n");
  return pPar->writeAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerHitFPar* pPar) {
  printf("Writing ShowerHitFPar container\n");
  return pPar->writeAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerCalPar* pPar) {
  printf("Writing ShowerCalPar container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerUnpackParam* pPar) {
  printf("Writing ShowerUnpackParam container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}        


