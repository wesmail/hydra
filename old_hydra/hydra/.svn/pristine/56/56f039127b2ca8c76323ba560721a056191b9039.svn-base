/////////////////////////////////////////////////////////////
//  HTriggerParRootFileIo
//
//  Class for TRIGGER parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

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
#include <iostream.h>

ClassImp(HTriggerParRootFileIo)

HTriggerParRootFileIo::HTriggerParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HTriggerParIo"
  fName="HTriggerParIo";
  HDetector* det=gHades->getSetup()->getDetector("Trigger");
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
    if (strcmp(name,"TriggerParMuLep")==0) return read((HTriggerParMuLep*)pPar,set);
    if (strcmp(name,"TriggerParMuDilep")==0) return read((HTriggerParMuDilep*)pPar,set);
    if (strcmp(name,"TriggerParMomMap")==0) return read((HTriggerParMomMap*)pPar,set);
    if (strcmp(name,"TriggerParShowerMap")==0) return read((HTriggerParShowerMap*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParMuLep* pPar,Int_t* set) {
  // reads and fills the container "TriggerParMuLep" for parameters
  //cout << "Enter READ of trigger MU Lepton params" << endl;

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  //cout << "version number " << version << endl;

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  //cout << "name " << name << "  version " << version << endl;
  HTriggerParMuLep* r=(HTriggerParMuLep*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    //cout << "Found TriggerParMuLep in root file" << endl;

  HTriggerParMuLep& rTrigger=*r;
  HTriggerParMuLep& pTrigger=*pPar;

  //cout << "Start copying parameters" << endl;
  //cout << "Root theta " << rTrigger.getMUtheta() << endl;
  pTrigger.setMUtheta(rTrigger.getMUtheta());    

  //cout << "Root phi " << rTrigger.getMUphi() << endl;
  pTrigger.setMUphi(rTrigger.getMUphi());

  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();

  //cout << "TriggerParMuLep: module(s) initialized from Root file" << endl;
  //cout << "Leave READ of trigger MU Lepton params" << endl;

  delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParMuDilep* pPar,Int_t* set) {
  // reads and fills the container "TriggerParMuDilep" for parameters
  //cout << "Enter READ of trigger MU Dilepton params" << endl;

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  //cout << "version number " << version << endl;

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  //cout << "name " << name << "  version " << version << endl;
  HTriggerParMuDilep* r=(HTriggerParMuDilep*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    //cout << "Found TriggerParMuDilep in root file" << endl;
    
    HTriggerParMuDilep& rTrigger=*r;
    HTriggerParMuDilep& pTrigger=*pPar;
    
    //cout << "Start copying parameters" << endl;
    //cout << "Root cutoff " << rTrigger.getMUcutoff() << endl;
    pTrigger.setMUcutoff(rTrigger.getMUcutoff());    
    
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    //cout << "TriggerParMuDilep: module(s) initialized from Root file" << endl;    
    //cout << "Leave READ of trigger MU Dilepton params" << endl;
        
    delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParMomMap* pPar,Int_t* set) {
  // reads and fills the container "TriggerParMomMap" for parameters
  cout << "Enter READ of trigger MU Dilepton params" << endl;

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  //cout << "version number " << version << endl;

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  //cout << "name " << name << "  version " << version << endl;
  HTriggerParMomMap* r=(HTriggerParMomMap*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    //cout << "Found TriggerParMomMap in root file" << endl;
    
    HTriggerParMomMap& rTrigger=*r;
    HTriggerParMomMap& pTrigger=*pPar;
    
    //cout << "Start copying parameters" << endl;
    //cout << " TRIGGERPARROOT READ theta bins " << rTrigger.getTheta_bins() << "  delta theta bins " 
    //<< rTrigger.getDeltatheta_bins() << "  phi bins " << rTrigger.getPhi_bins() << endl;

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
    
    //cout << " TRIGGERPARROOT READ2 theta bins " << pTrigger.getTheta_bins() << "  delta theta bins " 
    //<< pTrigger.getDeltatheta_bins() << "  phi bins " << pTrigger.getPhi_bins() << endl;

    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    //cout << "TriggerParMomMap: module(s) initialized from Root file" << endl;
    //cout << "Leave READ of trigger MU Momentum Mapping params" << endl;
    
    delete r;
  }
  return allFound;
}


Bool_t HTriggerParRootFileIo::read(HTriggerParShowerMap* pPar,Int_t* set) {
  // reads and fills the container "TriggerParShowerMap" for parameters
  //cout << "Enter READ of trigger Shower Map params" << endl;

  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  //cout << "version number " << version << endl;

  if (version==-1) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  //cout << "name " << name << "  version " << version << endl;
  HTriggerParShowerMap* r=(HTriggerParShowerMap*)findContainer(name,version);

  Bool_t allFound=kTRUE;
  if (r){
    //cout << "Found TriggerParShowerMap in root file" << endl;
    
    HTriggerParShowerMap& rTrigger=*r;
    HTriggerParShowerMap& pTrigger=*pPar;
    
    //cout << "Start copying parameters" << endl;
   
    for (Int_t xx=0;xx<32;xx++) {
      for (Int_t yy=0;yy<32;yy++) {
	  pTrigger.setShowerTheta(xx,yy,rTrigger.getShowerTheta(xx,yy));    
	  pTrigger.setShowerPhi(xx,yy,rTrigger.getShowerPhi(xx,yy));    
        }
      }

    
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    //cout << "TriggerParShowerMap: module(s) initialized from Root file" << endl;
    //cout << "Leave READ of trigger MU Shower Mapping params" << endl;
    
    delete r;
  }
  return allFound;
}



































