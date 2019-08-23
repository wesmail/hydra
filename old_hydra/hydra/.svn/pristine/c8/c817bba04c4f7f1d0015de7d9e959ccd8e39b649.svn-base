//*-- AUTHOR : Ilse Koenig
//*-- Modified : 30/11/2000 by Ilse Koenig

/////////////////////////////////////////////////////////////
//  HTofinoParRootFileIo
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
#include "htofinolookup.h"
#include "htofinogeompar.h"
#include <iostream.h>

ClassImp(HTofinoParRootFileIo)

HTofinoParRootFileIo::HTofinoParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HTofinoParIo"
  fName="HTofinoParIo";
  initModules=new TArrayI(6);
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
  printf("Tofino par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"TofinoCalPar")>=0) return read((HTofinoCalPar*)pPar,set);
    if (strcmp(name,"TofinoShowerMap")>=0) return read((HTofinoShowerMap*)pPar,set);
    if (strcmp(name,"TofinoLookup")>=0) return read((HTofinoLookup*)pPar,set);
    if (strcmp(name,"TofinoGeomPar")>=0)
        return HDetParRootFileIo::read((HTofinoGeomPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

/*
Bool_t HTofinoParRootFileIo::readObject(HTofinoParSet *pPar) {
  const Text_t* name = ((TNamed*)pPar)->GetName();
  TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  if (key) {
         pPar->Read(name);
         pPar->setStatic(); 
  }
  return (key != NULL);
}
*/

Bool_t HTofinoParRootFileIo::readObject(HTofinoParSet *pPar) {

  Text_t* name = (char*)pPar->GetName();
  Int_t version = findInputVersion(name);
 
  if(version<=0){ 
    pPar->setInputVersion(-1,inputNumber);
    Error("readObject","Container %s  not found in RuntimeDB ",name);
	return kFALSE;    // not in ROOT file
  }

  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  //TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  TKey *key = (TKey*)gDirectory->GetKey(name,version);

          // need reinizalization
 
      if(key)
      {
      key->Read(pPar);
      pPar->setInputVersion(version,inputNumber);
      pPar->setChanged();

      } else {

      pPar->setInputVersion(-1,inputNumber);
      }
      return (key != NULL);
}

Bool_t HTofinoParRootFileIo::read(HTofinoCalPar* pPar,Int_t* set) {
  if (!readObject(pPar)) return kFALSE;
  printf("Tofino CalPar initialized from ROOT\n"); 
  return kTRUE;
}

Bool_t HTofinoParRootFileIo::read(HTofinoShowerMap* pPar,Int_t* set) {
  if (!readObject(pPar)) return kFALSE;
  printf("Tofino-Shower Map initialized from ROOT\n"); 
  return kTRUE;
}

Bool_t HTofinoParRootFileIo::read(HTofinoLookup* pPar,Int_t* set) {
  // Reads and fills the container "TofinoLookup" needed by the unpacker.
  // Returns kFalse, when not initialized completely from this input
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) 
  { 
   pPar->setInputVersion(-1,inputNumber);  
   return kFALSE;}    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HTofinoLookup* rTofino=(HTofinoLookup*)findContainer(name,version);
  initModules->Reset();
  pPar->clear();   // no initialization from 2 inputs possible!
  Int_t crate, slot, sector, module, cell;
  Char_t dcType;                      
  for(Int_t t=0;t<rTofino->getSize();t++) {
    HTofinoLookupDConv& rDC=(*rTofino)[t];
    crate=rDC.getCrate();
    slot=rDC.getSlot();                       
    dcType=rDC.getType();                                    
    for(Int_t c=0;c<rDC.getSize();c++) {
      rDC[c].getAddress(sector,module,cell);
      if (sector>=0 && set[sector]) {
        pPar->setAddress(crate,slot,dcType,c,sector,module,cell);
        initModules->AddAt(sector+1,sector);
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("TofinoLookup: module(s) initialized from Root file ");
  delete rTofino;
  Bool_t allFound=kTRUE;
  for(Int_t i=0;i<initModules->GetSize(); i++) {
    if (set[i] && !initModules->At(i)) allFound=kFALSE;
  }
  return allFound;
}
