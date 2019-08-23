//*-- AUTHOR : Denis Bertini
//*-- Last modified : 15/12/98 by Ilse Koenig

/////////////////////////////////////////////////////////////
//HShowerParHadAsciiFileIo
//
//  Class for Mdc parameter input/output from/into Asci file
//
/////////////////////////////////////////////////////////////

#include "hshowerparhadasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerdetector.h"
#include "hshowergeometry.h"
#include "hshowercalpar.h"
#include "hshowercalasic.h"
#include "hshowerlookupasic.h"
#include "hshowerdigidetpar.h"
#include "hshowerhitfpar.h"
#include "hshowerhist.h"
#include "hshowerunpackparam.h"
#include <iostream.h>

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
  printf("shower par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"ShowerGeometry")==0) 
                                return read((HShowerGeometry*)pPar,set);
    if (strcmp(name,"ShowerDigiPar")==0) 
                                return read((HShowerDigiDetPar*)pPar,set);
    if (strcmp(name,"ShowerHitFinderParams")==0) 
                                return read((HShowerHitFPar*)pPar,set);
    if (strcmp(name,"ShowerCalPar")==0) 
                                return read((HShowerCalPar*)pPar,set);
    if (strcmp(name,"ShowerCalAsic")==0) 
                                return read((HShowerCalAsic*)pPar,set);
    if (strcmp(name,"ShowerLookupAsic")==0) 
                                return read((HShowerLookupAsic*)pPar,set);
    if (strcmp(name,"ShowerHist")==0) 
                                return read((HShowerHist*)pPar,set);
    if (strcmp(name,"ShowerUnpackParam")==0) 
                                return read((HShowerUnpackParam*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
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
    if (strcmp(name,"ShowerCalAsic")==0) 
                                return writeFile((HShowerCalAsic*)pPar);
    if (strcmp(name,"ShowerLookupAsic")==0) 
                                return writeFile((HShowerLookupAsic*)pPar);
    if (strcmp(name,"ShowerHist")==0) 
                                return kTRUE;
    if (strcmp(name,"ShowerUnpackParam")==0) 
                                return writeFile((HShowerUnpackParam*)pPar);
  }

  cerr<<name<<" could not be written to Asci file"<<endl;
  return kFALSE;
}


Bool_t HShowerParHadAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerGeometry* pPar, Int_t* set) {
  if (!pPar->initAscii(pFile))
    return kFALSE;

  pPar->attachTables();
//  pPar->transToLab();
  pPar->setStatic();
 
  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerDigiDetPar* pPar, Int_t* set) {
  pPar->setStatic();
  return pPar->initAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerHitFPar* pPar, Int_t* set) {
  pPar->setStatic();
  return pPar->initAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerCalPar* pPar, Int_t* set) {
  printf("reading ShowerCalPar container\n");
  int ret;

  pPar->setStatic();
  ret = pPar->initAscii(pFile);

  return ret;
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerCalAsic* pPar, Int_t* set) {
  pPar->setStatic();
  return pPar->initAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerLookupAsic* pPar, Int_t* set) {
  pPar->setStatic();
  return pPar->initAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerHist* pPar, Int_t* set) {
  pPar->setStatic();
  pPar->defaultInit(); //nothing to read
  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::read(HShowerUnpackParam* pPar, Int_t* set) {
  pPar->setStatic();
  return pPar->initAscii(pFile);
}
 
Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerGeometry* pPar) {

  return pPar->writeAscii(pFile);
} 

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerDigiDetPar* pPar) {
  return pPar->writeAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerHitFPar* pPar) {
  return pPar->writeAscii(pFile);
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerCalPar* pPar) {
  printf("writing ShowerCalPar container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerCalAsic* pPar) {
  printf("writing ShowerCalAsic container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerLookupAsic* pPar) {
  printf("writing ShowerLookupAsic container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}

Bool_t HShowerParHadAsciiFileIo::writeFile(HShowerUnpackParam* pPar) {
  printf("writing ShowerUnpackParam container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}        

