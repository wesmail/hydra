//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HMdcParFileIo
//
//  Class for Mdc parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "hmdcparfileio.h"
#include "hparfileio.h"
#include "heventfile.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdccalpar.h"
#include "hmdclookupgeom.h"
#include <iostream.h>

ClassImp(HMdcParFileIo)

HMdcParFileIo::HMdcParFileIo(HParRootFile* f) : HDetParFileIo(f) {
  // constructor sets the name of the detector I/O "HMdcParIo"
  fName="HMdcParIo";
  initModules=new TArrayI(24);
}


HMdcParFileIo::~HMdcParFileIo() {
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


Bool_t HMdcParFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Mdc");
  Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"MdcRawStruct")==0) return read((HMdcRawStruct*)pPar,set);
    if (strcmp(name,"MdcGeomStruct")==0) return read((HMdcGeomStruct*)pPar,set);
    if (strcmp(name,"MdcCalPar")==0) return read((HMdcCalPar*)pPar,set);
    if (strcmp(name,"MdcLookupGeom")==0) return read((HMdcLookupGeom*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}


Bool_t HMdcParFileIo::read(HMdcRawStruct* pPar,Int_t* set) {
  // reads and fills the container "MdcRawStruct"
  // ( names of the motherboards (characterizes the position) and
  //   the maximum number of TDCs on each of it )
  Text_t* name=pPar->GetName();
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
  for(Int_t s=0;s<6;s++) {
    HMdcRawSecStru& rSec=rMdc[s];
    HMdcRawSecStru& pSec=pMdc[s];
    for(Int_t m=0;m<4;m++) {
      HMdcRawModStru& rMod=rSec[m];
      HMdcRawModStru& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        if (modulesFound->At(pos)) {
          for(Int_t n=0;n<16;n++) {
            pMod[n].SetName(rMod[n].GetName());
            pMod[n].setNTdcs(rMod[n].getNTdcs());
          }
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        }
        else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  pPar->setStatic();
  printInfo("MdcRawStruct: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParFileIo::read(HMdcGeomStruct* pPar,Int_t* set) {
  // reads and fills the container "MdcGeomStruct"
  // (number of the cells in each layer)
  Text_t* name=pPar->GetName();
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
  for(Int_t i=0;i<6;i++) {
    for(Int_t j=0;j<4;j++) {
      pos=i*4+j;
      if (set[pos]) {
        if (modulesFound->At(pos)) {
          for(Int_t k=0;k<6;k++) {
            pP[i][j].fill(k,(*r)[i][j][k]);
          }
          set[pos]=0;
          initModules->AddAt(pos+1,pos);
        }
        else allFound=kFALSE;
      }
    }
  }
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  pPar->setStatic();
  printInfo("MdcGeomStruct: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParFileIo::read(HMdcCalPar* pCalPar,Int_t* set) {
  // reads and fills the container "MdcCalPar" for calibration parameters
  Text_t* name=pCalPar->GetName();
  Int_t version=findInputVersion(name);
  if (version==-1) return kFALSE;    // not in ROOT file
  if (pCalPar->getInputVersion(inputNumber)==version
        && pCalPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HMdcCalPar* r=(HMdcCalPar*)findContainer(name,version);
  Bool_t allFound=kTRUE;
  Int_t pos;
  initModules->Reset();
  HMdcCalPar& rMdc=*r;
  HMdcCalPar& pMdc=*pCalPar;
  for(Int_t s=0;s<pMdc.getSize();s++) {
    HMdcCalParSec& rSec=rMdc[s];
    HMdcCalParSec& pSec=pMdc[s];
    for(Int_t m=0;m<pSec.getSize();m++) {
      HMdcCalParMod& rMod=rSec[m];
      HMdcCalParMod& pMod=pSec[m];
      pos=s*4+m;
      if (set[pos]) {
        for(Int_t l=0;l<pMod.getSize();l++) {
          HMdcCalParLayer& rLay=rMod[l];
          HMdcCalParLayer& pLay=pMod[l];
          if (rLay.getSize()) {
            for(Int_t c=0;c<pLay.getSize();c++) {
              pLay[c].setSlope(rLay[c].getSlope());
              pLay[c].setOffset(rLay[c].getOffset());
            }
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          }
          else allFound=kFALSE;
        }
      }
    }
  }
  pCalPar->setInputVersion(version,inputNumber);
  pCalPar->setChanged();
  printInfo("MdcCalPar: module(s) initialized from Root file ");
  delete r;
  return allFound;
}


Bool_t HMdcParFileIo::read(HMdcLookupGeom* pPar,Int_t* set) {
  // reads and fills the container "MdcLookupGeom" for mapping of
  Text_t* name=pPar->GetName();
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
          if (pMoth.getSize()) {
            if (rMoth.getSize()) {
              for(Int_t c=0;c<pMoth.getSize();c++) {
                pMoth[c].setNLayer(rMoth[c].getNLayer());
                pMoth[c].setNCell(rMoth[c].getNCell());
              }
              set[pos]=0;
              initModules->AddAt(pos+1,pos);
            }
            else allFound=kFALSE;
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
