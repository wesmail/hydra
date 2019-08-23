//*-- AUTHOR : Ilse Koenig
//*-- Modified : 19/01/2001 by I. Koenig

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
#include "hmdcmodulegeometry.h"
#include "hmdchitfpar.h"
#include "hmdctargetgeometry.h"
#include "hmdcsegmentfpar.h"
#include "hmdcleverarmgeometry.h"
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
    if (strcmp(name,"MdcHitFPar")==0) return read((HMdcHitFPar*)pPar,set);
    if (strcmp(name,"MdcModuleGeometry")==0) return read((HMdcModuleGeometry*)pPar,set);
    if (strcmp(name,"MdcTargetGeometry")==0) return read((HMdcTargetGeometry*)pPar,set);
    if (strcmp(name,"MdcSegmentFPar")==0) return read((HMdcSegmentFPar*)pPar,set);
    if (strcmp(name,"MdcLeverArmGeometry")==0) return read((HMdcLeverArmGeometry*)pPar,set);
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
  Int_t version=findInputVersion(name);
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
          if (pMboSize>0) {
            if (rMbo.getSize()==pMboSize) {
              for(Int_t c=0;c<pMboSize;c++) pMbo[c].fill(rMbo[c]);
              set[pos]=0;
              initModules->AddAt(pos+1,pos);
            } else allFound=kFALSE;
          }
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
          if (pMothSize>0) {
            if (rMoth.getSize()==pMothSize) {
              for(Int_t c=0;c<pMothSize;c++) pMoth[c].fill(rMoth[c]);
              set[pos]=0;
              initModules->AddAt(pos+1,pos);
            } else allFound=kFALSE;
          }
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


Bool_t HMdcParRootFileIo::read(HMdcHitFPar* pPar,Int_t* set) {
  // reads and fills the container "MdcHitFPar" for reconstruction parameters
  Text_t* name = (char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;                       // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
      && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  // needs reinitialization
  HMdcHitFPar* r=(HMdcHitFPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  //  HMdcHitFPar& rMdc=*r;
  HMdcHitFPar& pMdc=*pPar;
  Int_t sectors, modules;
  sectors = r->getNSectors();
  modules = r->getNModules();

  for(Int_t s=0; s<sectors; s++){
    for(Int_t m=0; m< modules; m++){
      pos = s*4+m;
      if(set[pos]){
        if(modulesFound->At(pos)){
          pMdc.setMinNumHits(s,m,r->getMinNumHits(s,m));
          pMdc.setMaxNumCommonHits(s,m,r->getMaxNumCommonHits(s,m));
          pMdc.setMaxChi(s,m,r->getMaxChi(s,m));
          pMdc.setRoad(s,m,r->getRoad(s,m));
          pMdc.setMaxSlopeX(s,m,r->getMaxSlopeX(s,m));
          pMdc.setMinSlopeX(s,m,r->getMinSlopeX(s,m));
          pMdc.setMaxSlopeY(s,m,r->getMaxSlopeY(s,m));
          pMdc.setMinSlopeY(s,m,r->getMinSlopeY(s,m));
          pMdc.setDistZAxis(s,m,r->getDistZAxis(s,m));
          pMdc.setZHit(s,m,r->getZHit(s,m));
        }
        set[pos]=0;
        initModules->AddAt(pos+1,pos);
        }
        else allFound=kFALSE;
    }

  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcHitFPar: module(s) initialized from Root file ");
  delete r;
  return allFound;
}



Bool_t HMdcParRootFileIo::read(HMdcModuleGeometry* pGeoPar,Int_t* set) {
  // reads and fills the container "MdcModuleGeometry" for digitisation parameters
  Text_t* name = (char*)pGeoPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;                       // not in ROOT file
  if (pGeoPar->getInputVersion(inputNumber)==version
      && pGeoPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  // needs reinitialization
  HMdcModuleGeometry* r=(HMdcModuleGeometry*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  //HMdcModuleGeometry& rMdc=*r;
  HMdcModuleGeometry& pMdc=*pGeoPar;
  Int_t sectors, modules, layers;
  sectors = r->getNSectors();
  modules = r->getNModules();
  layers  = r->getNLayers();
  //  pMdc.setNSectors(sectors);
  //  pMdc.setNModules(modules);
  //  pMdc.setNLayers(layers);

  for(Int_t s=0; s<sectors; s++){
    for(Int_t m=0; m< modules; m++){
      pos = s*4+m;
      if(set[pos]){
        if(modulesFound->At(pos)){
          for(Int_t l=0; l< layers; l++){
            pMdc.setZPlane(s,m,l,r->getZPlane(s,m,l));
	    pMdc.setWireOrient(s,m,l,r->getWireOrient(s,m,l));
            pMdc.setCosn(s,m,l,r->getCosn(s,m,l));
            pMdc.setSinn(s,m,l,r->getSinn(s,m,l));
            pMdc.setSgpln(s,m,l,r->getSgpln(s,m,l));
            pMdc.setPitch(s,m,l,r->getPitch(s,m,l));
	    pMdc.setCatDistance(s,m,l,r->getCatDistance(s,m,l));
            pMdc.setFirstWirePos(s,m,l,r->getFirstWirePos(s,m,l));
	    pMdc.setCentralWire(s,m,l,r->getCentralWire(s,m,l));
            pMdc.setMaxNumCells(s,m,l,r->getMaxNumCells(s,m,l));           
          }
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        }
        else allFound=kFALSE;
      }
    }
  }
  pGeoPar->setInputVersion(version,inputNumber);
  pGeoPar->setChanged();
  printInfo("MdcModuleGeometry: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParRootFileIo::read(HMdcTargetGeometry* pGeoPar,Int_t* set) {
  // reads and fills the container "MdcTargetGeometry" for mdc reconstruction
  Text_t* name = (char*)pGeoPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;                       // not in ROOT file
  if (pGeoPar->getInputVersion(inputNumber)==version
      && pGeoPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  // needs reinitialization
  HMdcTargetGeometry* r=(HMdcTargetGeometry*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  //HMdcTargetGeometry& rMdc=*r;
  HMdcTargetGeometry& pMdc=*pGeoPar;
  Int_t sectors, modules;
  sectors = r->getNSectors();
  modules = r->getNModules();
  //  pMdc.setNSectors(sectors);
  //  pMdc.setNModules(modules);
  //  pMdc.setNLayers(layers);

  pMdc.setXDim(r->getXDim());
  pMdc.setYDim(r->getYDim());

  for(Int_t s=0; s<sectors; s++){
    for(Int_t m=0; m< modules; m++){
      pos = s*4+m;
      if(set[pos]){
        if(modulesFound->At(pos)){
            pMdc.setZPos(s,m,r->getZPos(s,m));
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        }
        else allFound=kFALSE;
      }
    }
  }
  pGeoPar->setInputVersion(version,inputNumber);
  pGeoPar->setChanged();
  printInfo("MdcTargetGeometry: module(s) initialized from Root file ");
  delete r;
  return allFound;
}




Bool_t HMdcParRootFileIo::read(HMdcSegmentFPar* pPar,Int_t* set) {
  // reads and fills the container "HMdcSegmentFPar" for mdc reconstruction
  Text_t* name = (char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;                       // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
      && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  // needs reinitialization
  HMdcSegmentFPar* r=(HMdcSegmentFPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos1, pos2;
  initModules->Reset();
  //HMdcSegmentFPar& rMdc=*r;
  HMdcSegmentFPar& pMdc=*pPar;
  Int_t sectors, leverarms;
  sectors = r->getNSectors();
  leverarms = r->getNLeverArms();
  //  pMdc.setNSectors(sectors);
  //  pMdc.setNModules(modules);
  //  pMdc.setNLayers(layers);
  Int_t module1;

  for(Int_t s=0; s<sectors; s++){
    for(Int_t l=0; l< leverarms; l++){
      if(l==0) module1 = 0;
      else module1 = 2;
      pos1 = s*4+module1;
      pos2 = s*4+module1+1;
      if(set[pos1] && set[pos2]){
        if(modulesFound->At(pos1) && modulesFound->At(pos2)){
	  pMdc.setCorrX(s,l,r->getCorrX(s,l));
	  pMdc.setCorrY(s,l,r->getCorrY(s,l));
	  pMdc.setErrDiffX(s,l,r->getErrDiffX(s,l));
	  pMdc.setErrDiffY(s,l,r->getErrDiffY(s,l));
	  pMdc.setErrDiffXSlope(s,l,r->getErrDiffXSlope(s,l));
	  pMdc.setErrDiffYSlope(s,l,r->getErrDiffYSlope(s,l));
	  pMdc.setAlpha(s,l,r->getAlpha(s,l));
          set[pos1]=0;
          set[pos2]=0;
          initModules->AddAt(pos1+1,pos1);
          initModules->AddAt(pos2+1,pos2);
        }
        else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcSegmentFPar: module(s) initialized from Root file ");
  delete r;
  return allFound;
}




Bool_t HMdcParRootFileIo::read(HMdcLeverArmGeometry* pPar,Int_t* set) {
  // reads and fills the container "HMdcLeverArmGeometry" for mdc reconstruction
  Text_t* name = (char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) return kFALSE;                       // not in ROOT file
  if (pPar->getInputVersion(inputNumber)==version
      && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  // needs reinitialization
  HMdcLeverArmGeometry* r=(HMdcLeverArmGeometry*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos1, pos2;
  initModules->Reset();
  //HMdcLeverArmGeometry& rMdc=*r;
  HMdcLeverArmGeometry& pMdc=*pPar;
  Int_t sectors, leverarms;
  sectors = r->getNSectors();
  leverarms = r->getNLeverArms();
  //  pMdc.setNSectors(sectors);
  //  pMdc.setNModules(modules);
  //  pMdc.setNLayers(layers);

  Int_t module1;
  for(Int_t s=0; s<sectors; s++){
    for(Int_t l=0; l< leverarms; l++){
      if(l==0) module1 = 0;
      else module1 = 2;
      pos1 = s*4+module1;
      pos2 = s*4+module1+1;
      if(set[pos1] && set[pos2]){
        if(modulesFound->At(pos1) && modulesFound->At(pos2)){
            pMdc.setCosn(s,l,r->getCosn(s,l));
            pMdc.setSinn(s,l,r->getSinn(s,l));
            pMdc.setTrasElement(s,l,0,r->getTrasElement(s,l,0));
            pMdc.setTrasElement(s,l,1,r->getTrasElement(s,l,1));
            pMdc.setTrasElement(s,l,2,r->getTrasElement(s,l,2));
          set[pos1]=0;
          set[pos2]=0;
          initModules->AddAt(pos1+1,pos1);
          initModules->AddAt(pos2+1,pos2);
        }
        else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcLeverArmGeometry: module(s) initialized from Root file ");
  delete r;
  return allFound;
}
