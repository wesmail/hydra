//*-- AUTHOR : Ilse Koenig
//*-- Modified : 06/10/2000 by I.Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//  HStartParRootFileIo
//
//  Class for Start parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hstartparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hstartdetector.h"
#include "hstartlookup.h"
#include "hstartcalpar.h"
#include "hstartselfcopar.h"
#include "hstartgeompar.h"
#include <iostream> 
#include <iomanip>

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
  // Initializes a container called by name, but only the modules defined
  //   in the array 'set.
  // Calls the special read function for this container.
  // If it is called the first time, it reads the setup found in the file.
  if (!isActiv) readModules("Start");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"StartLookup")==0) return read((HStartLookup*)pPar,set);
    if (strcmp(name,"StartCalPar")==0) return read((HStartCalPar*)pPar,set);
    if (strcmp(name,"StartSelfCoPar")==0) return readStartModule((HStartSelfCoPar*)pPar,set);
    if (strcmp(name,"StartGeomPar")==0) return HDetParRootFileIo::read((HStartGeomPar*)pPar,set);
   }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HStartParRootFileIo::read(HStartLookup* pPar,Int_t* set) {
  // Reads and fills the container "StartLookup" needed by the unpacker.
  // Returns an error, when not initialized completely from this input
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
    if (version<=0)
    {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
    }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HStartLookup* rStart=(HStartLookup*)findContainer(name,version);
  initModules->Reset();
  pPar->clear();   // no initialization from 2 inputs possible!
  Int_t crate, slot, module, strip;
  for(Int_t t=0;t<rStart->getSize();t++) {
    HStartLookupTdc& rTdc=(*rStart)[t];
    crate=rTdc.getCrate();
    slot=rTdc.getSlot();
    for(Int_t c=0;c<rTdc.getSize();c++) {
      rTdc[c].getAddress(module,strip);
      if (module>=0 && set[module]) {
        pPar->setAddress(crate,slot,c,module,strip,rTdc[c].getType());
        initModules->AddAt(module+1,module);
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("StartLookup: module(s) initialized from Root file ");
  delete rStart;
  Bool_t allFound=kTRUE;
  for(Int_t i=0;i<initModules->GetSize(); i++) {
    if (set[i] && !initModules->At(i)) allFound=kFALSE;
  }
  return allFound;
}

Bool_t HStartParRootFileIo::read(HStartCalPar* pPar,Int_t* set) {
  // Reads and fills the container "StartCalPar"
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
    if (version<=0)
    {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
    }
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
        initModules->AddAt(m+1,m);
      } else allFound=kFALSE;
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("StartCalPar: module(s) initialized from Root file ");
  delete rStart;
  return allFound;
}

Bool_t HStartParRootFileIo::readStartModule(HStartSelfCoPar* pPar,Int_t* set) {
  // Reads and fills the container "StartSelfCoPar"
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
    if (version<=0)
    {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
    }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HStartSelfCoPar* rStart=(HStartSelfCoPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  initModules->Reset();
  if (set[0]) {
    HStartSelfCoParMod& r=(*rStart)[0];
    HStartSelfCoParMod& p=(*pPar)[0];
    Int_t pModSize=p.getSize();
    if (pModSize && r.getSize()==pModSize) {
      for(Int_t c=0;c<pModSize;c++) p[c].fill(r[c]);
      initModules->AddAt(1,0);
    } else allFound=kFALSE;
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("StartSelfCoPar: module initialized from Root file ");
  delete rStart;
  return allFound;
}
