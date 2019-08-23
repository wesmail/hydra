//*-- AUTHOR : Jochen Markert

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HGo4ParAsciiFileIo
//
// Class for Mdc parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////

#include "hgo4parasciifileio.h"
#include "hgo4onlineconfig.h"
#include "htriggerhistbooker.h"
#include "hstarthistbooker.h"
#include "hhodohistbooker.h"
#include "hrichhistbooker.h"
#include "hmdchistbooker.h"
#include "htofhistbooker.h"
#include "htofinohistbooker.h"
#include "hshowerhistbooker.h"

ClassImp(HGo4ParAsciiFileIo)

HGo4ParAsciiFileIo::HGo4ParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HGo4ParIo";
}

Bool_t HGo4ParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (!strncmp(name,"Go4OnlineConfig",strlen("Go4OnlineConfig"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"TriggerHistBooker",strlen("TriggerHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"StartHistBooker",strlen("StartHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"HodoHistBooker",strlen("HodoHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"RichHistBooker",strlen("RichHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"MdcHistBooker",strlen("MdcHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"TofHistBooker",strlen("TofHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"TofinoHistBooker",strlen("TofinoHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"ShowerHistBooker",strlen("ShowerHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}
    if (!strncmp(name,"PhysicsHistBooker",strlen("ShowerHistBooker"))){return HDetParAsciiFileIo::readCond((HParCond*)pPar);}

    Error("HGo4ParAsciiFileIo::init()","initialization of %s not possible from file!",name);
    return kFALSE;
  }
  Error("HGo4ParAsciiFileIo::init()","no input file open");
  return kFALSE;
}

Int_t HGo4ParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strncmp(name,"Go4OnlineConfig",strlen("Go4OnlineConfig"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"TriggerHistBooker",strlen("TriggerHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"StartHistBooker",strlen("StartHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"HodoHistBooker",strlen("HodoHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"RichHistBooker",strlen("RichHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"MdcHistBooker",strlen("MdcHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"TofHistBooker",strlen("TofHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"TofinoHistBooker",strlen("TofinoHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"ShowerHistBooker",strlen("ShowerHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}
    if (!strncmp(name,"PhysicsHistBooker",strlen("ShowerHistBooker"))){return HDetParAsciiFileIo::writeCond((HParCond*)pPar);}

    Error("HGo4ParAsciiFileIo::write()","%s could not be written to Ascii file",name);
    return -1;
  }
  Error("HGo4ParAsciiFileIo::write()","no output file open");
  return -1;
}
