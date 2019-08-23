/////////////////////////////////////////////////////////////
//  HTriggerParRootFileIo
//
//  Class for TRIGGER parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "htriggerparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "htriggerdetector.h"
#include "htriggerparmulep.h"
#include "htriggerparmudilep.h"
#include "htriggerparmommap.h"
#include "htriggerparshowermap.h"
#include "htriggerparrichmap.h"
#include "hrichIPUparpattern.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include <iostream> 
#include <iomanip>

ClassImp(HTriggerParRootFileIo)

HTriggerParRootFileIo::HTriggerParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HTriggerParIo"
  fName="HTriggerParIo";
}


HTriggerParRootFileIo::~HTriggerParRootFileIo() {
  // destructor
}

Bool_t HTriggerParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Trigger");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"TriggerParMomMap")==0) return read((HTriggerParMomMap*)pPar,set);
    if (strcmp(name,"TriggerParShowerMap")==0) return read((HTriggerParShowerMap*)pPar,set);
    if (strcmp(name,"TriggerParRichMap")==0) return read((HTriggerParRichMap*)pPar,set);
    if (strncmp(name,"TriggerParMuLep",strlen("TriggerParMuLep"))==0)
        return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"TriggerParMuDilep",strlen("TriggerParMuDilep"))==0)
        return HDetParRootFileIo::read(pPar);
    if (strcmp(name,"RichIPUParPattern")==0) return read((HRichIPUParPattern*)pPar,set);
    if (strcmp(name,"RichIPUParThresholds")==0) return read((HRichIPUParThresholds*)pPar,set);
    if (strncmp(name,"RichIPUParLocMax",strlen("RichIPUParLocMax"))==0)
      return HDetParRootFileIo::read(pPar);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParMuLep* pPar,Int_t* set) {
  // reads and fills the container "TriggerParMuLep" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HTriggerParMuLep* r=(HTriggerParMuLep*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
  HTriggerParMuLep& rTrigger=*r;
  HTriggerParMuLep& pTrigger=*pPar;

  pTrigger.setMUtheta(rTrigger.getMUtheta());    
  pTrigger.setMUphiOffset(rTrigger.getMUphiOffset());
  pTrigger.setMUphiSlope(rTrigger.getMUphiSlope());

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();

  delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParMuDilep* pPar,Int_t* set) {
  // reads and fills the container "TriggerParMuDilep" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HTriggerParMuDilep* r=(HTriggerParMuDilep*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HTriggerParMuDilep& rTrigger=*r;
    HTriggerParMuDilep& pTrigger=*pPar;
    
    pTrigger.setMUcutoff(rTrigger.getMUcutoff());    
    
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
        
    delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParMomMap* pPar,Int_t* set) {
  // reads and fills the container "TriggerParMomMap" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HTriggerParMomMap* r=(HTriggerParMomMap*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HTriggerParMomMap& rTrigger=*r;
    HTriggerParMomMap& pTrigger=*pPar;
    
    pTrigger.setTheta_bins(rTrigger.getTheta_bins());    
    pTrigger.setDeltatheta_bins(rTrigger.getDeltatheta_bins());    
    pTrigger.setPhi_bins(rTrigger.getPhi_bins());    

    for (Int_t xx=0;xx<rTrigger.getTheta_bins();xx++) {
      for (Int_t yy=0;yy<rTrigger.getDeltatheta_bins();yy++) {
        for (Int_t zz=0;zz<rTrigger.getPhi_bins();zz++) {
	  pTrigger.setMom(xx,yy,zz,rTrigger.getMom(xx,yy,zz));    
        }
      }
    }
    
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    
    delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParShowerMap* pPar,Int_t* set) {
  // reads and fills the container "TriggerParShowerMap" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HTriggerParShowerMap* r=(HTriggerParShowerMap*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HTriggerParShowerMap& rTrigger=*r;
    HTriggerParShowerMap& pTrigger=*pPar;
    
    for (Int_t xx=0;xx<32;xx++) {
      for (Int_t yy=0;yy<32;yy++) {
	  pTrigger.setShowerTheta(xx,yy,rTrigger.getShowerTheta(xx,yy));    
	  pTrigger.setShowerPhi(xx,yy,rTrigger.getShowerPhi(xx,yy));    
        }
      }

    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    
    delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParRichMap* pPar,Int_t* set) {
  // reads and fills the container "TriggerParRichMap" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HTriggerParRichMap* r=(HTriggerParRichMap*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HTriggerParRichMap& rTrigger=*r;
    HTriggerParRichMap& pTrigger=*pPar;
    

    for (Int_t xx=0;xx<96;xx++) {
      for (Int_t yy=0;yy<96;yy++) {
	  pTrigger.setRichTheta(xx,yy,rTrigger.getRichTheta(xx,yy));    
	  pTrigger.setRichPhi(xx,yy,rTrigger.getRichPhi(xx,yy)); 
        }
      }

    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    
    delete r;
  }
  return allFound;
}

Bool_t HTriggerParRootFileIo::read(HRichIPUParPattern* pPar,Int_t* set) {
  // reads and fills the container "RichIPUParPattern" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HRichIPUParPattern* r=(HRichIPUParPattern*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HRichIPUParPattern& rRichIPU=*r;
    HRichIPUParPattern& pRichIPU=*pPar;

    pRichIPU.setNGroups(rRichIPU.getNGroups());
    pRichIPU.setNPadsinGroup(rRichIPU.getNPadsinGroup());
    pRichIPU.setNPadDimensions(rRichIPU.getNPadDimensions());

    for (Int_t xx=0;xx<rRichIPU.getNGroups();xx++) {
      for (Int_t yy=0;yy<rRichIPU.getNPadsinGroup();yy++) {
        for (Int_t zz=0;zz<rRichIPU.getNPadDimensions();zz++) {
          pRichIPU.setPattern(xx,yy,zz,rRichIPU.getPattern(xx,yy,zz));
        }
      }
    }

    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();

    delete r;
  }
  return allFound;
}

Bool_t HTriggerParRootFileIo::read(HRichIPUParThresholds* pPar,Int_t* set) {
  // reads and fills the container "RichIPUParThresholds" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HRichIPUParThresholds* r=(HRichIPUParThresholds*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HRichIPUParThresholds& rRichIPU=*r;
    HRichIPUParThresholds& pRichIPU=*pPar;

    for (Int_t xx=0;xx<2;xx++) {
      for (Int_t yy=0;yy<6;yy++) {
        pRichIPU.setThresholds(xx,yy,rRichIPU.getThresholds(xx,yy));
      }
    }

    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();

    delete r;
  }
  return allFound;
}

Bool_t HTriggerParRootFileIo::read(HRichIPUParLocMax* pPar,Int_t* set) {
  // reads and fills the container "RichIPUParLocMax" for parameters

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization

  HRichIPUParLocMax* r=(HRichIPUParLocMax*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    HRichIPUParLocMax& rRichIPU=*r;
    HRichIPUParLocMax& pRichIPU=*pPar;

    pRichIPU.setLocMax(rRichIPU.getLocMax());
    pRichIPU.setLocMaxFlag(rRichIPU.getLocMaxFlag());

    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();

    delete r;
  }
  return allFound;
}
