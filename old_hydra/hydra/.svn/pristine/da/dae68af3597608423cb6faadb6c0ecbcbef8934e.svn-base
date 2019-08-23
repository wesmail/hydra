//*-- AUTHOR : Ilse Koenig
//*-- Modified : 18/02/2000 by I. Koenig

/////////////////////////////////////////////////////////////
// HMdcParRootFileIo
//
//  Class for Mdc parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
// This class replaces the former class HMdcParFileIo.
/////////////////////////////////////////////////////////////

#include "hmdcparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdcmboreadout.h"
#include "hmdccalparraw.h"
#include "hmdclookupgeom.h"
#include "hmdcdigitpar.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"
#include <iostream.h>

ClassImp(HMdcParRootFileIo)

HMdcParRootFileIo::HMdcParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HMdcParIo"
  fName="HMdcParIo";
  initModules=new TArrayI(24);
}


HMdcParRootFileIo::~HMdcParRootFileIo() {
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


Bool_t HMdcParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Mdc");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"MdcRawStruct")==0) return read((HMdcRawStruct*)pPar,set);
    if (strcmp(name,"MdcGeomStruct")==0) return read((HMdcGeomStruct*)pPar,set);
    if (strcmp(name,"MdcMboReadout")==0) return read((HMdcMboReadout*)pPar,set);
    if (strcmp(name,"MdcCalParRaw")==0) return read((HMdcCalParRaw*)pPar,set);
    if (strcmp(name,"MdcLookupGeom")==0) return read((HMdcLookupGeom*)pPar,set);
    if (strcmp(name,"MdcDigitPar")==0) return read((HMdcDigitPar*)pPar,set);
    if (strcmp(name,"MdcLayerGeomPar")==0) return read((HMdcLayerGeomPar*)pPar,set);
    if (strcmp(name,"MdcGeomPar")==0) return HDetParRootFileIo::read((HMdcGeomPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}


Bool_t HMdcParRootFileIo::read(HMdcRawStruct* pPar,Int_t* set) {
  // reads and fills the container "MdcRawStruct"
  // ( names of the motherboards (characterizes the position) and
  //   the maximum number of TDCs on each of it )
  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcRawStruct* r=(HMdcRawStruct*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcRawStruct& rMdc=*r;
  HMdcRawStruct& pMdc=*pPar;
  for(Int_t s=0;s<pMdc.getSize();s++) {
    HMdcRawSecStru& rSec=rMdc[s];
    HMdcRawSecStru& pSec=pMdc[s];
    for(Int_t m=0;m<pSec.getSize();m++) {
      HMdcRawModStru& rMod=rSec[m];
      HMdcRawModStru& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        if (modulesFound->At(pos)) {
          for(Int_t n=0;n<pMod.getSize();n++) {
            pMod[n].SetName(rMod[n].GetName());
            pMod[n].setNTdcs(rMod[n].getNTdcs());
          }
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        } else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcRawStruct: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcMboReadout* pPar,Int_t* set) {
  // reads and fills the container "MdcMboReadout" which holds the
  // readout addresses of the MDC motherboards
  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcMboReadout* r=(HMdcMboReadout*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcMboReadout& rMdc=*r;
  HMdcMboReadout& pMdc=*pPar;
  for(Int_t s=0;s<pMdc.getSize();s++) {
    HMdcMboReadoutSec& rSec=rMdc[s];
    HMdcMboReadoutSec& pSec=pMdc[s];
    for(Int_t m=0;m<pSec.getSize();m++) {
      HMdcMboReadoutMod& rMod=rSec[m];
      HMdcMboReadoutMod& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        Int_t pModSize=pMod.getSize();
        if (pModSize && rMod.getSize()==pModSize) {
          for(Int_t n=0;n<pMod.getSize();n++) pMod[n].fill(rMod[n]);
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        } else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcMboReadout: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcGeomStruct* pPar,Int_t* set) {
  // reads and fills the container "MdcGeomStruct"
  // (number of the cells in each layer)
  Text_t* name=(char*)pPar->GetName();
  Int_t version=getMaxVersion(name);
  if (version<=0) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcGeomStruct* r=(HMdcGeomStruct*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcGeomStruct& pP=*pPar;
  HMdcGeomStruct& rP=*r;
  for(Int_t i=0;i<pP.getSize();i++) {
    HMdcGeomSecStru& pSec=pP[i];
    HMdcGeomSecStru& rSec=rP[i];
    for(Int_t j=0;j<pSec.getSize();j++) {
      pos=i*4+j;
      if (set[pos]) {
        if (modulesFound->At(pos)) {
          HMdcGeomModStru& pMod=pSec[j];
          HMdcGeomModStru& rMod=rSec[j];
          for(Int_t k=0;k<pMod.getSize();k++) pMod.fill(k,rMod[k]);
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        } else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcGeomStruct: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcCalParRaw* pCalPar,Int_t* set) {
  // reads and fills the container "MdcCalParRaw" for calibration parameters
  // on raw level
  Text_t* name=(char*)pCalPar->GetName();
  Int_t version=findInputVersion(name);
  if (version==-1) return kFALSE;    // not in ROOT file
  if (pCalPar->getInputVersion(inputNumber)==version
        && pCalPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcCalParRaw* r=(HMdcCalParRaw*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcCalParRaw& rMdc=*r;
  HMdcCalParRaw& pMdc=*pCalPar;
  for(Int_t s=0;s<pMdc.getSize();s++) {
    HMdcCalParRawSec& rSec=rMdc[s];
    HMdcCalParRawSec& pSec=pMdc[s];
    for(Int_t m=0;m<pSec.getSize();m++) {
      HMdcCalParRawMod& rMod=rSec[m];
      HMdcCalParRawMod& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        for(Int_t l=0;l<pMod.getSize();l++) {
          HMdcCalParMbo& rMbo=rMod[l];
          HMdcCalParMbo& pMbo=pMod[l];
          Int_t pMboSize=pMbo.getSize();
          if (pMboSize && rMbo.getSize()==pMboSize) {
            for(Int_t c=0;c<pMboSize;c++) pMbo[c].fill(rMbo[c]);
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          } else allFound=kFALSE;
        }
      }
    }
  }
  pCalPar->setInputVersion(version,inputNumber);
  pCalPar->setChanged();
  printInfo("MdcCalParRaw: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcLookupGeom* pPar,Int_t* set) {
  // reads and fills the container "MdcLookupGeom" for mapping of
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version==-1) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcLookupGeom* r=(HMdcLookupGeom*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcLookupGeom& rMdc=*r;
  HMdcLookupGeom& pMdc=*pPar;
  for(Int_t s=0;s<pMdc.getSize();s++) {
    HMdcLookupGSec& rSec=rMdc[s];
    HMdcLookupGSec& pSec=pMdc[s];
    for(Int_t m=0;m<pSec.getSize();m++) {
      HMdcLookupGMod& rMod=rSec[m];
      HMdcLookupGMod& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        for(Int_t l=0;l<pMod.getSize();l++) {
          HMdcLookupMoth& rMoth=rMod[l];
          HMdcLookupMoth& pMoth=pMod[l];
          Int_t pMothSize=pMoth.getSize();
          if (pMothSize && rMoth.getSize()==pMothSize) {
            for(Int_t c=0;c<pMothSize;c++) pMoth[c].fill(rMoth[c]);
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          } else allFound=kFALSE;
        }
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcLookupGeom: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcDigitPar* pDigitPar,Int_t* set) {
  // reads and fills the container "MdcDigitPar" for digitisation parameters
  Text_t* name=(char*)pDigitPar->GetName();
  Int_t version=findInputVersion(name);
  if (version==-1) return kFALSE;    // not in ROOT file
  if (pDigitPar->getInputVersion(inputNumber)==version
      && pDigitPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcDigitPar* r=(HMdcDigitPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcDigitPar& rMdc=*r;
  HMdcDigitPar& pMdc=*pDigitPar;
  for(Int_t s=0;s<6;s++) {
    HMdcDigitParSec& rSec=rMdc[s];
    HMdcDigitParSec& pSec=pMdc[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitParMod& rMod=rSec[m];
      HMdcDigitParMod& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        Int_t pModSize=pMod.getSize();
        if (pModSize && rMod.getSize()==pModSize) {
          for(Int_t l=0;l<pModSize;l++) pMod[l].fill(rMod[l]);
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        } else allFound=kFALSE;
      }
    }
  }
  pDigitPar->setInputVersion(version,inputNumber);
  pDigitPar->setChanged();
  printInfo("MdcDigitPar: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcLayerGeomPar* pPar,Int_t* set) {
  // reads and fills the container "MdcLayerGeomPar"
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version==-1) return kFALSE;    // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
      && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcLayerGeomPar* r=(HMdcLayerGeomPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcLayerGeomPar& rMdc=*r;
  HMdcLayerGeomPar& pMdc=*pPar;
  for(Int_t s=0;s<6;s++) {
    HMdcLayerGeomParSec& rSec=rMdc[s];
    HMdcLayerGeomParSec& pSec=pMdc[s];
    for(Int_t m=0;m<4;m++) {
      HMdcLayerGeomParMod& rMod=rSec[m];
      HMdcLayerGeomParMod& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        Int_t pModSize=pMod.getSize();
        if (pModSize && rMod.getSize()==pModSize) {
          for(Int_t l=0;l<pModSize;l++) pMod[l].fill(rMod[l]);
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        } else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcLayerGeomPar: module(s) initialized from Root file ");
  delete r;
  return allFound;
}
