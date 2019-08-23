//*-- AUTHOR : Ilse Koenig
//*-- Created : 18/05/2005

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HHodoParRootFileIo
//
//  Parameter I/O from ROOT file for the Pion Hodoscopes
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhodoparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hhododetector.h"
#include "hhodolookup.h"
#include "hhodocalpar.h"
#include "hhodorefhitpar.h"
#include <iostream>
#include <iomanip>

ClassImp(HHodoParRootFileIo)

HHodoParRootFileIo::HHodoParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HHodoParIo"
  fName="HHodoParIo";
  Int_t n=gHades->getSetup()->getDetector("Hodo")->getMaxModules();
  initModules=new TArrayI(n);
}

HHodoParRootFileIo::~HHodoParRootFileIo() {
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

Bool_t HHodoParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // Initializes a container called by name, but only the modules defined
  //   in the array 'set.
  // Calls the special read function for this container.
  // If it is called the first time, it reads the setup found in the file.
  if (!isActiv) readModules("Hodo");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"HodoLookup")==0) return read((HHodoLookup*)pPar,set);
    if (strcmp(name,"HodoCalPar")==0) return read((HHodoCalPar*)pPar,set);
    if (strcmp(name,"HodoRefHitPar")==0) return read((HHodoRefHitPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HHodoParRootFileIo::read(HHodoLookup* pPar,Int_t* set) {
  // Reads and fills the container "HodoLookup" needed by the unpacker.
  // Returns an error, when not initialized completely from this input.
  Text_t* name=(Char_t*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HHodoLookup* rHodo=(HHodoLookup*)findContainer(name,version);
  initModules->Reset();
  pPar->clear();   // no initialization from 2 inputs possible!
  Int_t module, fiber;
  for(Int_t c=0;c<rHodo->getSize();c++) {
    for(Int_t s=0;s<(*rHodo)[c].getSize();s++) {
      for(Int_t i=0;i<(*rHodo)[c][s].getSize();i++) {
        (*rHodo)[c][s][i].getAddress(module,fiber);
        if (module>=0 && set[module]) {
          pPar->fill(c,s,i,module,fiber);
          initModules->AddAt(module+1,module);
        }
      }
    }
  }
  delete rHodo;
  Bool_t allFound=kTRUE;
  for(Int_t i=0;i<initModules->GetSize(); i++) {
    if (set[i] && !initModules->At(i)) allFound=kFALSE;
  }
  if (allFound) {
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    printInfo("HodoLookup: module(s) initialized from Root file ");
  }
  return allFound;
}

Bool_t HHodoParRootFileIo::read(HHodoCalPar* pPar,Int_t* set) {
  // Reads and fills the container "HodoCalPar"
  Text_t* name=(Char_t*)pPar->GetName();
  Int_t version=findInputVersion(name);
    if (version<=0)
    {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
    }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HHodoCalPar* rHodo=(HHodoCalPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  initModules->Reset();
  pPar->clear();   // no initialization from 2 inputs possible!
  for(Int_t m=0;m<rHodo->getSize();m++) {
    if (set[m]) {
      HHodoCalParMod& r=(*rHodo)[m];
      HHodoCalParMod& p=(*pPar)[m];
      Int_t pModSize=p.getSize();
      if (pModSize && r.getSize()==pModSize) {
        for(Int_t c=0;c<pModSize;c++) p[c].fill(r[c]);
        initModules->AddAt(m+1,m);
      } else allFound=kFALSE;
    }
  }
  for(Int_t i=0;i<initModules->GetSize(); i++) {
    if (set[i] && !initModules->At(i)) allFound=kFALSE;
  }
  if (allFound) {
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    printInfo("HodoCalPar: module(s) initialized from Root file ");
  }
  delete rHodo;
  return allFound;
}

Bool_t HHodoParRootFileIo::read(HHodoRefHitPar* pPar,Int_t* set) {
  // Reads and fills the container "HodoRefHitPar"
  Text_t* name=(Char_t*)pPar->GetName();
  Int_t version=findInputVersion(name);
    if (version<=0)
    {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
    }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HHodoRefHitPar* rHodo=(HHodoRefHitPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  initModules->Reset();
  pPar->clear();   // no initialization from 2 inputs possible!
  for(Int_t m=0;m<rHodo->getSize();m++) {
    if (set[m]) {
      HHodoRefHitParMod& r=(*rHodo)[m];
      HHodoRefHitParMod& p=(*pPar)[m];
      p.fill(r);
      initModules->AddAt(m+1,m);
    }
  }
  for(Int_t i=0;i<initModules->GetSize(); i++) {
    if (set[i] && !initModules->At(i)) allFound=kFALSE;
  }
  if (allFound) {
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    printInfo("HodoRefHitPar: module(s) initialized from Root file ");
  }
  delete rHodo;
  return allFound;
}
