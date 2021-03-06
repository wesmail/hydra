//*-- AUTHOR : Ilse Koenig
//*-- Modified : 21/05/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
// HDetParRootFileIo
//
// Interface class to ROOT file for parameter input/output
// Base class for all detectors
//
// This class is derived from the common base class HDetParIo
// for all inputs/outputs (ROOT file, Oracle, Ascii file).
// It contains all common functionality for all detectors which can be
// implemeted without knowing the exact type of the detector or of the detector
// dependend parameter containers using only the base class types.
//
// The data element modulesFound stores the indices of all modules in a
// detector which are are defined in the actual analysis which can be
// initialized from the ROOT file. If a detector cannot be found in the file
// this pointer is 0. The array ic created and filled via the function
// readModules(Text_t* detectorName) which is called when the first init()
// function of a parameter container belonging to this detector is called.
//
// The data element initModules stores the  indices of all modules for which a
// container has been initialized from the ROOT file.
// 
///////////////////////////////////////////////////////////////////////////////
#include <TROOT.h>  
#include "hdetparrootfileio.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hparrootfileio.h"
#include "heventfile.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hparset.h"
#include "hdetgeompar.h"
#include "hgeomcompositevolume.h"
#include "TKey.h"
#include <iostream.h>

ClassImp(HDetParRootFileIo)

HDetParRootFileIo::HDetParRootFileIo(HParRootFile* f) : HDetParIo() {
  // Constructor takes a pointer to the parameter ROOT file
  pFile=f;
  modulesFound=0;    // modules found in ROOT file
  initModules=0;
  isActiv=kFALSE;
}


HDetParRootFileIo::~HDetParRootFileIo() {
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


Int_t HDetParRootFileIo::readModules(Text_t* detName) {
  // reads the setup of the detector with given name from the ROOT file,
  // compares it with the setup of this detector in the current analysis and
  // creates and fills the data element modulesFound;
  // Sets the flag isAktiv kTRUE.
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


Bool_t HDetParRootFileIo::write(HDetector* p) {
  // writes the setup of a detector to the ROOT file
  // returns kFALSE if the file is not writable
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


Int_t HDetParRootFileIo::write(HParSet* pPar) {
  // writes a parameter container to the ROOT file and returns the new version
  // number (returns -1 if the file is not writable)
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


Int_t HDetParRootFileIo::getMaxVersion(Text_t* name) {
  // returns the maximum version of the container given by name in the ROOT
  // file (return -1 if not found)
  TKey* key=pFile->GetKey(name);
  if (key) return key->GetCycle();
  else return -1;
}


Int_t HDetParRootFileIo::findInputVersion(Text_t* name) {
  // finds the input version to initialize the container given by name;
  // returns -1 if the version cannot be determined
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


TObject* HDetParRootFileIo::findContainer(Text_t* name, Int_t vers) {
  // finds the parameter container given by its name with a special version in
  // the ROOT file (returns 0 if not successful)
  // This funtion uses internally the ROOT function FindObject(Text_t*), which
  // creates a new object. The calling function must therefore delete the
  // object after usage!
  Text_t cn[80];
  sprintf(cn,"%s;%i",name,vers);
  pFile->cd();
  TObject* p=gROOT->FindObject(cn);
  return p;
}


void HDetParRootFileIo::printInfo(const Text_t* msg) {
  // prints the module indices for which the container was initialized from
  // the ROOT file (will later go to the log file)
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


Bool_t HDetParRootFileIo::read(HDetGeomPar* pPar,Int_t* set) {
  // reads and fills the container for the basic geometry parameters of the
  // detectors
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);  // ROOT version needed
  if (version==-1) return kFALSE;        // not in ROOT file
  Int_t cv=pPar->getInputVersion(inputNumber); // actual container version
  if (cv==version) return kTRUE;         // no reinitialization needed
//  if ((inputNumber==1 && cv!=-1) || (inputNumber==2 && cv!=-1))
  if (pPar->getInputVersion(1)==-1 && pPar->getInputVersion(2)==-1)
      pPar-> clear();  
  HDetGeomPar* rGeom=(HDetGeomPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  initModules->Reset();
  Int_t nMod=pPar->getNumModules();
  Int_t nRef=pPar->getNumRefModules();
  Int_t nComp=pPar->getNumComponents();
  for(Int_t m=0;m<nMod;m++) {
    if (set[m]) {
      HModGeomPar* pMod=pPar->getModule(m);
      HModGeomPar* rMod=rGeom->getModule(m);
      if (rMod) {
        pMod->SetName(rMod->GetName());
        pMod->setRefName(rMod->getRefName());
        HGeomTransform& tp=pMod->getLabTransform();
        tp=rMod->getLabTransform();
        const Text_t* ref=rMod->getRefName();
        pMod->setRefName(ref);
        Int_t mr=pPar->getModNumInMod(ref);
        HGeomCompositeVolume* refMod=pPar->getRefVolume(mr);
        if (refMod==0) {
          refMod=new HGeomCompositeVolume(nComp);
          refMod->SetName(ref);
          pPar->addRefVolume(refMod,mr);
        }
        pMod->setVolume(refMod);
        set[m]=0;
        initModules->AddAt(m+1,m);
      } else allFound=kFALSE;
    }
  }
  for(Int_t m=0;m<nRef;m++) {
    HGeomCompositeVolume* pMod=pPar->getRefVolume(m);
    HGeomCompositeVolume* rMod=rGeom->getRefVolume(m);
    if (pMod && rMod && pMod->getNumPoints()==0) {
      pMod->setShape(rMod->getShape());
      pMod->setMother(rMod->getMother());
      Int_t np=rMod->getNumPoints();
      pMod->createPoints(np);
      for(Int_t i=0;i<np;i++) pMod->setPoint(i,*(rMod->getPoint(i)));
      HGeomTransform& tp=pMod->getTransform();
      tp=rMod->getTransform();
      for(Int_t c=0;c<nComp;c++) {
        HGeomVolume* pComp=pMod->getComponent(c);
        HGeomVolume* rComp=rMod->getComponent(c);
        pComp->SetName(rComp->GetName());
        pComp->setShape(rComp->getShape());
        pComp->setMother(rComp->getMother());
        Int_t npc=rComp->getNumPoints();
        pComp->createPoints(npc);
        for(Int_t i=0;i<npc;i++) pComp->setPoint(i,*(rComp->getPoint(i)));
        HGeomTransform& tpc=pComp->getTransform();
        tpc=rComp->getTransform();
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  TString msg(name);
  msg.Append(": module(s) initialized from Root file ");
  printInfo((const char*)msg);
  delete rGeom;
  return allFound;
}
