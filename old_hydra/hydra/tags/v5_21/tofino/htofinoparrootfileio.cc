//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HTofinoParRootFileIo
//
//  Class for Tofino parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "htofinoparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "htofinocalpar.h"
#include "htofinoshowermap.h"

#include <iostream.h>

ClassImp(HTofinoParRootFileIo)

HTofinoParRootFileIo::HTofinoParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HTofinoParIo"
  fName="HTofinoParIo";
  initModules=new TArrayI(24);
}


HTofinoParRootFileIo::~HTofinoParRootFileIo() {
  // destructor
  if (modulesFound) {
    delete modulesFound;
    modulesFound=0;
  }
  if (initModules) {
    delete initModules;
    initModules=0;
  }
}


Bool_t HTofinoParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  if (!isActiv) readModules("Tofino");
  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("tofino par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"TofinoCalPar")==0) return read((HTofinoCalPar*)pPar,set);
    if (strcmp(name,"TofinoShowerMap")==0) return read((HTofinoShowerMap*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HTofinoParRootFileIo::readObject(HTofinoParSet *pPar) {
  const Text_t* name = ((TNamed*)pPar)->GetName();
  TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  if (key) {
         pPar->Read(name);
         pPar->setStatic(); 
  }
  return (key != NULL);
}


Bool_t HTofinoParRootFileIo::read(HTofinoCalPar* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  printf("Tofino CalPar initialized from ROOT\n"); 
  return kTRUE;
}

Bool_t HTofinoParRootFileIo::read(HTofinoShowerMap* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  printf("Tofino-Shower Map initialized from ROOT\n"); 
  return kTRUE;
}
