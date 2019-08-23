//*-- AUTHOR : Ilse Koenig
//*-- Modified : 06/10/2000 by I.Koenig

/////////////////////////////////////////////////////////////
//  HStartParRootFileIo
//
//  Class for Start parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "hstartparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hstartdetector.h"
#include "hstartlookup.h"
#include "hstartcalpar.h"
#include "hstartgeompar.h"
#include <iostream.h>

ClassImp(HStartParRootFileIo)

HStartParRootFileIo::HStartParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HStartParIo"
  fName="HStartParIo";
  Int_t n=gHades->getSetup()->getDetector("Start")->getMaxModules();
  initModules=new TArrayI(n);
}


HStartParRootFileIo::~HStartParRootFileIo() {
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

Bool_t HStartParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Start");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"StartLookup")==0) return read((HStartLookup*)pPar,set);
    if (strcmp(name,"StartCalPar")==0) return read((HStartCalPar*)pPar,set);
    if (strcmp(name,"StartGeomPar")==0)
        return HDetParRootFileIo::read((HStartGeomPar*)pPar,set);
   }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HStartParRootFileIo::read(HStartLookup* pPar,Int_t* set) {
  // reads and fills the container "StartLookup"
  // the Start detector raw data
  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HStartLookup* rStart=(HStartLookup*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  initModules->Reset();
  for(Int_t t=0;t<pPar->getSize();t++) {
    HStartLookupTdc& r=(*rStart)[t];
    HStartLookupTdc& p=(*pPar)[t];
    Int_t pTdcSize=p.getSize();
    if (pTdcSize && r.getSize()==pTdcSize) {
      for(Int_t c=0;c<pTdcSize;c++) p[c].fill(r[c]);
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("StartLookup: module(s) initialized from Root file ");
  delete rStart;
  return allFound;
}

Bool_t HStartParRootFileIo::read(HStartCalPar* pPar,Int_t* set) {
  // reads and fills the container "StartCalPar"
  // the Start detector raw data
  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HStartCalPar* rStart=(HStartCalPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  initModules->Reset();
  for(Int_t m=0;m<pPar->getSize();m++) {
    if (set[m]) {
      HStartCalParMod& r=(*rStart)[m];
      HStartCalParMod& p=(*pPar)[m];
      Int_t pModSize=p.getSize();
      if (pModSize && r.getSize()==pModSize) {
        for(Int_t c=0;c<pModSize;c++) p[c].fill(r[c]);
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("StartCalPar: module(s) initialized from Root file ");
  delete rStart;
  return allFound;
}

