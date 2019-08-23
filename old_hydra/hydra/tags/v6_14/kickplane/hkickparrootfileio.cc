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
using namespace std;
#include "hkickparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hkicktrackfpar.h"
#include "hkickplane.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "hrtmatchingpar.h"
#include "hrtfunctional.h"
#include "hmessagemgr.h"
#include <iostream> 
#include <iomanip>


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
    if (strncmp(name,"KickPlaneMeta",strlen("KickPlaneMeta"))==0)
      return read((HKickPlane*)pPar);
    if (strncmp(name,"KickPlaneMDC3",strlen("KickPlaneMDC3"))==0)
      return read((HKickPlane*)pPar);
    if (strncmp(name,"KickPlane2MDC3",strlen("KickPlane2MDC3"))==0)
      return read((HKickPlane2*)pPar);
    if (strncmp(name,"KickPlane2Meta",strlen("KickPlane2Meta"))==0)
      return read((HKickPlane2*)pPar);
    if (strncmp(name,"KickTrackFPar",strlen("KickTrackFPar"))==0)
      return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"RtSavGolPar",strlen("RtSavGolPar"))==0)
      return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"KickMatchParMeta",strlen("KickMatchParMeta"))==0) 
      return read((HKickMatchPar *)pPar);
    if (strncmp(name,"KickMatchParMDC3",strlen("KickMatchParMDC3"))==0) 
      return read((HKickMatchPar *)pPar);
    if (strncmp(name,"RtMatchingParMDC3",strlen("RtMatchingParMDC3"))==0) 
      return read((HRtMatchingPar *)pPar);
    if (strncmp(name,"RtMatchingParMeta",strlen("RtMatchingParMeta"))==0) 
      return read((HRtMatchingPar *)pPar);
    if (strncmp(name,"RtData",strlen("RtData"))==0) 
      return read((HRtData *)pPar);
  }
  Warning("init","Initialization of %s not possible from ROOT file!\n",name);
  return kFALSE;
}


Bool_t HKickParRootFileIo::read(HKickPlane* pPar) {
  // reads and fills the container "KickPlane"

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
  HKickPlane* r=(HKickPlane*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  if (allFound) gHades->getMsg()->info(10,DET_KICKPLANE,"HKickParRootFileIo","%s correctly initialized!",name);
  delete r;
  return allFound;
}

Bool_t HKickParRootFileIo::read(HKickPlane2* pPar) {
  // reads and fills the container "KickPlane2"

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HKickPlane2* r=(HKickPlane2*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  if (allFound) gHades->getMsg()->info(10,DET_KICKPLANE,"HKickParRootFileIo","%s correctly initialized!",name);
  delete r;
  return allFound;
}

Bool_t HKickParRootFileIo::read(HKickTrackFPar* pPar) {
  // reads and fills the container "KickTrackFPar"
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
  HKickTrackFPar* r=(HKickTrackFPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  if (allFound) gHades->getMsg()->info(10,DET_KICKPLANE,"HKickParRootFileIo","%s correctly initialized!",name);
  delete r;
  return allFound;
}

Bool_t HKickParRootFileIo::read(HRtMatchingPar* pPar) {
  // reads and fills the container "KickTrackFPar"
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
  HRtMatchingPar* r=(HRtMatchingPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  if (allFound) gHades->getMsg()->info(10,DET_KICKPLANE,"HKickParRootFileIo","%s correctly initialized!",name);
  delete r;
  return allFound;
}

Bool_t HKickParRootFileIo::read(HKickMatchPar *pPar) {
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HKickMatchPar* r=(HKickMatchPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->readFrom(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  if (allFound) gHades->getMsg()->info(10,DET_KICKPLANE,"HKickParRootFileIo","%s correctly initialized!",name);
  delete r;
  return allFound;
}

Bool_t HKickParRootFileIo::read(HRtData *pPar) {
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HRtData* r=(HRtData*)findContainer(name,version);
  Bool_t allFound=kTRUE;

  pPar->stealData(*r);

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  if (allFound) gHades->getMsg()->info(10,DET_KICKPLANE,"HKickParRootFileIo","%s correctly initialized!",name);
  delete r;
  return allFound;
}

ClassImp(HKickParRootFileIo)











