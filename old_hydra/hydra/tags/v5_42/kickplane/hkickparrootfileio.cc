//*-- AUTHOR : M. Sanchez (12.03.2001)
//*-- Modified : 12/03/2001 by M. Sanchez

/////////////////////////////////////////////////////////////
// HKickParRootFileIo
//
//  Class for KickPlane parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
// This class replaces the former class HMdcParFileIo.
/////////////////////////////////////////////////////////////

#include "hkickparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hkicktrackfpar.h"
#include "hkickplane.h"
#include <iostream.h>


HKickParRootFileIo::HKickParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HKickParIo"
  fName="HKickParIo";
}

HKickParRootFileIo::~HKickParRootFileIo() {
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


Bool_t HKickParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  //  if (!isActiv) readModules("Mdc");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"KickPlaneMeta")==0) return read((HKickPlane*)pPar);
    if (strcmp(name,"KickPlaneMDC3")==0) return read((HKickPlane*)pPar);
    if (strcmp(name,"KickTrackFPar")==0) return read((HKickTrackFPar*)pPar);
  }
  Warning("init","Initialization of %s not possible from ROOT file!\n",name);
  return kFALSE;
}


Bool_t HKickParRootFileIo::read(HKickPlane* pPar) {
  // reads and fills the container "KickPlane"

  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0)
  {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HKickPlane* r=(HKickPlane*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  delete r;
  return allFound;
}


Bool_t HKickParRootFileIo::read(HKickTrackFPar* pPar) {
  // reads and fills the container "KickTrackFPar"
  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0)
  {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HKickTrackFPar* r=(HKickTrackFPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  delete r;
  return allFound;
}

ClassImp(HKickParRootFileIo)











