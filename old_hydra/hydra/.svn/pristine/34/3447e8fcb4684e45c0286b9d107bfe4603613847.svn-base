//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98
//*-- Modified : 26/02/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
//HDetParFileIo
//
//  Interface class to ROOT file for parameter input/output
//  Base class for all detectors
//
//  This class is derived from the common base class HParIo
//  for all inputs/outputs (ROOT file, Oracle, Asci file). 
//  
/////////////////////////////////////////////////////////////
#include <TROOT.h>  
#include "hdetparfileio.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hparfileio.h"
#include "heventfile.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hparset.h"
#include "TKey.h"
#include <iostream.h>

ClassImp(HDetParFileIo)

HDetParFileIo::HDetParFileIo(HParRootFile* f) : HDetParIo() {
  // Constructor
  // takes a pointer to the parameter ROOT file
  pFile=f;
  modulesFound=0;    // modules found in ROOT file
  initModules=0;
  isActiv=kFALSE;
}

HDetParFileIo::~HDetParFileIo() {
  // destructor
  if (modulesFound) {
    delete modulesFound;
    modulesFound=0;
  }
}

Int_t HDetParFileIo::readModules(Text_t* detName) {
  // reads the setup of the detector from ROOT file;
  // compares this setup with the setup of this detector in
  // the current analysis and stores the modules with could be
  // be in the dataelement modulesFound;
  // If a detector cannot be found in the file this pointer is 0.
  // This function must be called each time a file is opened.
  // Sets the flag isAktiv.
  // returns the number of modules not found;
  HDetector* setup=gHades->getSetup()->getDetector(detName);
  Int_t* set=setup->getModules();
  if (pFile) pFile->cd();
  HDetector* det=(HDetector*)gROOT->FindObject(detName);
  Int_t num=0;
  Int_t len=6*(setup->getMaxModules());
  if (det) {
    modulesFound=new TArrayI(len);
    Int_t* mod=det->getModules();
    for(Int_t m=0;m<len;m++) {
      if (set[m]!=0) {
        if (mod[m]!=0) modulesFound->AddAt(m+1,m);
        else num++;
      }
    }
    delete det;
  }
  isActiv=kTRUE;
  return num;
}

Bool_t HDetParFileIo::write(HDetector* p) {
  // writes the setup of a detector to ROOT file
  if (pFile) {
    pFile->cd();
    if (pFile->IsWritable()) {
      p->Write(((char*)p->GetName()));
      return kTRUE;
    }
  }
  cerr<<"Output is not writable"<<endl;
  return kFALSE;
}

Int_t HDetParFileIo::write(HParSet* pPar) {
  // writes a parameter container to ROOT file
  if (pFile) {
    pFile->cd();
    if (pFile->IsWritable()) {
      Text_t* name=(char*)pPar->GetName();
      pPar->Write(name);
      pPar->setChanged(kFALSE);
      gHades->getRuntimeDb()->setVersionsChanged(kTRUE);
      return getMaxVersion(name);
    }
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}

Int_t HDetParFileIo::getMaxVersion(Text_t* name) {
  // returns the maximum version of the container in the ROOT file
  // return -1 if not found
  TKey* key=pFile->GetKey(name);
  if (key) return key->GetCycle();
  else return -1;
}


Int_t HDetParFileIo::findInputVersion(Text_t* name) {
  // finds the input version for initialization
  HParVersion* currVers=
      gHades->getRuntimeDb()->getCurrentEventFile()->getParVersion(name);
  Int_t v=currVers->getInputVersion(inputNumber);
  if (v>0) return v;      // predefined
  HEventFile* f=pFile->eventFile;
  if (!f) return -1;        // event file not in ROOT file
  HParVersion* vers=f->getParVersion(name);
  if (!vers) return -1;     // container not in ROOT file
  return vers->getRootVersion();
}  


TObject* HDetParFileIo::findContainer(Text_t* name, Int_t vers) {
  // finds the parameter container with a special version
  Text_t cn[80];
  sprintf(cn,"%s;%i",name,vers);
  pFile->cd();
  TObject* p=gROOT->FindObject(cn);
  return p;
}

void HDetParFileIo::printInfo(Text_t* msg) {
  // prints the modules in a container initialized from Root file
  // will later go to the log file
  Bool_t first=kTRUE;
  for(Int_t i=0;i<initModules->GetSize();i++) {
    if (initModules->At(i)) {
      if (first) {
        cout<<msg;
        cout<<pFile->GetName()<<": ";
        first=kFALSE;
      }
      cout<<(initModules->At(i)-1)<<" ";
    }
  }
  cout<<'\n';
}


