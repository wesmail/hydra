//*-- AUTHOR : Denis Bertini
//*-- Last modified : 15/12/98 by Ilse Koenig

/////////////////////////////////////////////////////////////
//HTofinoParHadAsciiFileIo
//
//  Class for Tofino parameter input/output from/into Asci file
//
/////////////////////////////////////////////////////////////

#include "htofinoparhadasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "htofinodetector.h"
#include "htofinocalpar.h"
#include "htofinoshowermap.h"
#include <iostream.h>

ClassImp(HTofinoParHadAsciiFileIo)

HTofinoParHadAsciiFileIo::HTofinoParHadAsciiFileIo(HParHadAsciiFileIo* f) 
                            : HDetParHadAsciiFileIo(f) {
  fName="HTofinoParIo";
}


Bool_t HTofinoParHadAsciiFileIo::init(HParSet* pPar, Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("Tofino par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"TofinoCalPar")>=0) 
                                return read((HTofinoCalPar*)pPar,set);
    if (strcmp(name,"TofinoShowerMap")>=0) 
                                return read((HTofinoShowerMap*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ASCII file!"<<endl;
  return kFALSE;
}


Int_t HTofinoParHadAsciiFileIo::write(HParSet* pPar) {
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
    if (strcmp(name,"TofinoCalPar")>=0) 
                                return writeFile((HTofinoCalPar*)pPar);
    if (strcmp(name,"TofinoShowerMap")>=0) 
                                return writeFile((HTofinoShowerMap*)pPar);
  }

  cerr<<name<<" could not be written to ASCII file"<<endl;
  return kFALSE;
}


Bool_t HTofinoParHadAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}

Bool_t HTofinoParHadAsciiFileIo::read(HTofinoCalPar* pPar, Int_t* set) {
  printf("reading TofinoCalPar container\n");
  int ret;

  pPar->setStatic();
  ret = pPar->initAscii(pFile);

  return ret;
}

Bool_t HTofinoParHadAsciiFileIo::read(HTofinoShowerMap* pPar, Int_t* set) {
  printf("reading TofinoShowerMap container\n");
  int ret;

  pPar->setStatic();
  ret = pPar->initAscii(pFile);

  return ret;
}

Bool_t HTofinoParHadAsciiFileIo::writeFile(HTofinoCalPar* pPar) {
  printf("writing TofinoCalPar container\n");
  
  pPar->writeAscii(pFile);      
  
  return kTRUE;
}   

Bool_t HTofinoParHadAsciiFileIo::writeFile(HTofinoShowerMap* pPar) {
  printf("writing TofinoShowerMap container\n");

  pPar->writeAscii(pFile);

  return kTRUE;
}

