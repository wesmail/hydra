//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HShowerParRootFileIo
//
//  Class for Mdc parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "hshowerparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hshowergeometry.h"
#include "hshowerdigidetpar.h"
#include "hshowerhitfpar.h"
#include "hshowercalpar.h"
#include "hshowercalasic.h"
#include "hshowerhist.h"
#include "hshowerlookupasic.h"
#include "hshowerasic.h"
#include "hshowerunpackparam.h"

#include <iostream.h>

ClassImp(HShowerParRootFileIo)

HShowerParRootFileIo::HShowerParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HShowerParIo"
  fName="HShowerParIo";
  initModules=new TArrayI(24);
}


HShowerParRootFileIo::~HShowerParRootFileIo() {
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


Bool_t HShowerParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  if (!isActiv) readModules("Shower");
  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("shower par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"ShowerGeometry")==0) return read((HShowerGeometry*)pPar,set);
    if (strcmp(name,"ShowerDigiPar")==0) return read((HShowerDigiDetPar*)pPar,set);
    if (strcmp(name,"ShowerHitFinderParams")==0) return read((HShowerHitFPar*)pPar,set);
    if (strcmp(name,"ShowerCalPar")==0) return read((HShowerCalPar*)pPar,set);
    if (strcmp(name,"ShowerCalAsic")==0) return read((HShowerCalAsic*)pPar,set);
    if (strcmp(name,"ShowerLookupAsic")==0) return read((HShowerLookupAsic*)pPar,set);
    if (strcmp(name,"ShowerHist")==0) return read((HShowerHist*)pPar,set);
    if (strcmp(name,"ShowerUnpackParam")==0) return read((HShowerUnpackParam*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HShowerParRootFileIo::readObject(HShowerParSet *pPar) {
  const Text_t* name = ((TNamed*)pPar)->GetName();
  TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  if (key) {
         pPar->Read(name);
         pPar->setStatic(); 
  }
  return (key != NULL);
}

Bool_t HShowerParRootFileIo::read(HShowerGeometry* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  pPar->attachTables();
//  pPar->transToLab();
   
  return kTRUE;
}

Bool_t HShowerParRootFileIo::read(HShowerDigiDetPar* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;
  
  return kTRUE;
}

Bool_t HShowerParRootFileIo::read(HShowerHitFPar* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  return kTRUE;
}

Bool_t HShowerParRootFileIo::read(HShowerCalPar* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  printf("Shower CalPar initialized from ROOT file.\n"); 
  return kTRUE;
}

Bool_t HShowerParRootFileIo::read(HShowerCalAsic* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  printf("Shower CalAsic initialized from ROOT file.\n"); 
  return kTRUE;
}

Bool_t HShowerParRootFileIo::read(HShowerLookupAsic* pPar,Int_t* set) {
 if (!readObject(pPar))
     return kFALSE;

  printf("Shower LookupAsic initialized from ROOT file.\n"); 
  return kTRUE;
}

Bool_t HShowerParRootFileIo::read(HShowerHist* pPar,Int_t* set) {
  pPar->defaultInit(); //nothing to read
  return kTRUE;  
}

Bool_t HShowerParRootFileIo::read(HShowerUnpackParam* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  printf("Shower Unpack Param initialized from ROOT file.\n"); 
  return kTRUE;
}

  

