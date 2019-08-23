//*-- AUTHOR : Jochen Markert

/////////////////////////////////////////////////////////////
// HGo4ParRootFileIo
//
//  Class for Go4 parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "hgo4parrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hgo4onlineconfig.h"
#include "hstarthistbooker.h"
#include "hrichhistbooker.h"
#include "hmdcdetector.h"
#include "htofhistbooker.h"
#include "htofinohistbooker.h"
#include "hshowerhistbooker.h"
#include "hmdchistbooker.h"
#include <iostream.h>


ClassImp(HGo4ParRootFileIo)

HGo4ParRootFileIo::HGo4ParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HGo4ParIo"
  fName="HGo4ParIo";
  initModules=new TArrayI(24);
  initModuleTypes=new TArrayI(4);
}


HGo4ParRootFileIo::~HGo4ParRootFileIo() {
  // destructor
  if (modulesFound) {
    delete modulesFound;
    modulesFound=0;
  }
  if (initModules) {
    delete initModules;
    initModules=0;
  }
  if (initModuleTypes) {
    delete initModuleTypes;
    initModuleTypes=0;
  }
}


Bool_t HGo4ParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Mdc");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strncmp(name,"Go4OnlineConfig",strlen("Go4OnlineConfig"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"StartHistBooker",strlen("StartHistBooker"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"RichHistBooker",strlen("RichHistBooker"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"MdcHistBooker",strlen("MdcHistBooker"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"TofHistBooker",strlen("TofHistBooker"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"TofinoHistBooker",strlen("TofinoHistBooker"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"ShowerHistBooker",strlen("ShowerHistBooker"))==0) return HDetParRootFileIo::read(pPar);
  }
  Error("HGo4ParRootFileIo::init()","initialization of %s not possible from ROOT file!",name);
  return kFALSE;
}
