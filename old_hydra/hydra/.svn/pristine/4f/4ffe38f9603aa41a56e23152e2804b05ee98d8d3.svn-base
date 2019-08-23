//*-- AUTHOR : Ilse Koenig
//*-- Modified : 18/02/2000 by I. Koenig

/////////////////////////////////////////////////////////////
//  HTofParRootFileIo
//
//  Class for TOF parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "htofparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "htofdetector.h"
#include "htofcalpar.h"
#include "htofgeompar.h"
#include <iostream.h>

ClassImp(HTofParRootFileIo)

HTofParRootFileIo::HTofParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HTofParIo"
  fName="HTofParIo";
  HDetector* det=gHades->getSetup()->getDetector("Tof");
  Int_t n=(det->getMaxSectors())*(det->getMaxModules());
  initModules=new TArrayI(n);
}


HTofParRootFileIo::~HTofParRootFileIo() {
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

Bool_t HTofParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Tof");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"TofCalPar")==0) return read((HTofCalPar*)pPar,set);
    if (strcmp(name,"TofGeomPar")==0)
        return HDetParRootFileIo::read((HTofGeomPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}


Bool_t HTofParRootFileIo::read(HTofCalPar* pPar,Int_t* set) {
  // reads and fills the container "TofCalPar" for calibration parameters
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version==-1) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HTofCalPar* r=(HTofCalPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HTofCalPar& rTof=*r;
  HTofCalPar& pTof=*pPar;
  for(Int_t s=0;s<pTof.getSize();s++) {
    HTofCalParSec& rSec=rTof[s];
    HTofCalParSec& pSec=pTof[s];
    for(Int_t m=0;m<pSec.getSize();m++) {
      HTofCalParMod& rMod=rSec[m];
      HTofCalParMod& pMod=pSec[m];
      pos=s*22+m;
      if (set[pos]) {
        Int_t pModSize=pMod.getSize();
        if (pModSize && rMod.getSize()==pModSize) {
          for(Int_t c=0;c<pModSize;c++) pMod[c].fill(rMod[c]);
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        } else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("TofCalPar: module(s) initialized from Root file:\n ");
  delete r;
  return allFound;
}
 
