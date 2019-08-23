#include "hmdcgetcontainers.h"
#include "hkickparasciifileio.h"                                                    
#include "hparasciifileio.h"
#include "hpario.h"
#include "hmatrixcategory.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 29/09/2000

////////////////////////////////////////////////////////////////
// HMdcGetContainers
//
// The table for geting of pointers to param.&data containers
// Using:
//    HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
// If the cont. HMdcGetContainers not exist it will automaticaly 
// created and added to RuntimeDb
//
//    HMdcGeomPar* fgeomPar=fGetCont->getMdcGeomPar(Bool_t init=kTRUE);
// If cont. HMdcGeomPar don't exist & init=kTURUE it will created and initialized
//    ...
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcGetContainers)


//const HMdcGetContainers* HMdcGetContainers::fMdcGetContainers=0;   //??? 
    
HMdcGetContainers::HMdcGetContainers() : HParSet() {
  fName="MdcGetContainers";
  fTitle="The table for geting of pointers to param.&data containers";
  strcpy(detName,"Mdc");
  rtdb=0;
  spectr=0;
  Bool_t isRtdb=kFALSE;
  if(gHades) {
    rtdb=gHades->getRuntimeDb();
    rtdb->addContainer(this);
    isRtdb=kTRUE;
    spectr=gHades->getSetup(); 
  } else Warning("HMdcGetContainers","gHades=%i",0);
  
  fMdcLayerGeomPar=0;
  isMdcLayerGeomPar=isRtdb;
  fMdcGeomPar=0;
  isMdcGeomPar=isRtdb;
  fMdcDetector=0;
  isMdcDetector=isRtdb;
  fSpecGeomPar=0;
  isSpecGeomPar=isRtdb;
  fMdcGeomStruct=0;
  isMdcGeomStruct=isRtdb;
  fMdcRawStruct=0;
  isMdcRawStruct=isRtdb;
  fMdcLookupGeom=0;
  isMdcLookupGeom=isRtdb;
  fKickPlane=0;
  isKickPlane=isRtdb;
}

HMdcGetContainers* HMdcGetContainers::getObject() {
  HMdcGetContainers* fGetCont=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) {
      fGetCont=(HMdcGetContainers*)rdb->getContainer("MdcGetContainers");
      if( !fGetCont ) fGetCont=new HMdcGetContainers();
    }
  }
//  fMdcGetContainers=fGetCont; //???
  return fGetCont;
}

Bool_t HMdcGetContainers::initCont(HParSet *par) {
  if( !par ) return kFALSE;
  if( !par->isStatic() ) return par->init();
  return kTRUE;
}

HMdcLayerGeomPar* HMdcGetContainers::getMdcLayerGeomPar(Bool_t init) {
  if( !fMdcLayerGeomPar && isMdcLayerGeomPar ) {
    fMdcLayerGeomPar=(HMdcLayerGeomPar*)rtdb->getContainer("MdcLayerGeomPar");
    if(init) {
      if( !fMdcLayerGeomPar )
        rtdb->addContainer(fMdcLayerGeomPar=new HMdcLayerGeomPar);
      isMdcLayerGeomPar=initCont(fMdcLayerGeomPar);
      if(!isMdcLayerGeomPar) fMdcLayerGeomPar=0;
    }
  }
  return fMdcLayerGeomPar;
}

HMdcGeomPar* HMdcGetContainers::getMdcGeomPar(Bool_t init) {
  if( !fMdcGeomPar && isMdcGeomPar ) {
    fMdcGeomPar=(HMdcGeomPar*)rtdb->getContainer("MdcGeomPar");
    if( !fMdcGeomPar && init ) {
      fMdcGeomPar=(HMdcGeomPar*)(spectr->createDetGeomPar("Mdc"));
      isMdcGeomPar=initCont(fMdcGeomPar);
      if( !isMdcGeomPar ) fMdcGeomPar=0;
    }
  }
  return fMdcGeomPar;
}
    
HMdcDetector* HMdcGetContainers::getMdcDetector() {
  if( !fMdcDetector && isMdcDetector ) {
    fMdcDetector=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
    if( !fMdcDetector ) isMdcDetector=kFALSE;
  }
  return fMdcDetector;
}

HSpecGeomPar* HMdcGetContainers::getSpecGeomPar(Bool_t init) {
  if( !fSpecGeomPar && isSpecGeomPar ) {
    fSpecGeomPar=(HSpecGeomPar*)rtdb->getContainer("SpecGeomPar");
    if( !fSpecGeomPar && init) {
      fSpecGeomPar=(HSpecGeomPar*)(spectr->createSpecGeomPar());
      isSpecGeomPar=initCont(fSpecGeomPar);
      if( !isSpecGeomPar ) fSpecGeomPar=0;
    }
  }
  return fSpecGeomPar;
}


HMdcGeomStruct* HMdcGetContainers::getMdcGeomStruct(Bool_t init) {
  if( !fMdcGeomStruct && isMdcGeomStruct ) {
    fMdcGeomStruct=(HMdcGeomStruct*)rtdb->getContainer("MdcGeomStruct");
    if(init) {
      if(!fMdcGeomStruct) rtdb->addContainer(fMdcGeomStruct=new HMdcGeomStruct);
      isMdcGeomStruct=initCont(fMdcGeomStruct);
      if(!isMdcGeomStruct) fMdcGeomStruct=0;
    }
  }
  return fMdcGeomStruct;
}


HMdcRawStruct* HMdcGetContainers::getMdcRawStruct(Bool_t init) {
  if( !fMdcRawStruct && isMdcRawStruct ) {
    fMdcRawStruct=(HMdcRawStruct*)rtdb->getContainer("MdcRawStruct");
    if(init) {
      if( !fMdcRawStruct ) rtdb->addContainer(fMdcRawStruct=new HMdcRawStruct);
      isMdcRawStruct=initCont(fMdcRawStruct);
      if(!isMdcRawStruct) fMdcRawStruct=0;
    }
  }
  return fMdcRawStruct;
}

HMdcLookupGeom* HMdcGetContainers::getMdcLookupGeom(Bool_t init) {
  if( !fMdcLookupGeom && isMdcLookupGeom ) {
    fMdcLookupGeom=(HMdcLookupGeom*)rtdb->getContainer("MdcLookupGeom");
    if(init) {
      if(!fMdcLookupGeom) rtdb->addContainer(fMdcLookupGeom=new HMdcLookupGeom);
      isMdcLookupGeom=initCont(fMdcLookupGeom);
      if(!isMdcLookupGeom) fMdcLookupGeom=0;
    }
  }
  return fMdcLookupGeom;
}

HKickPlane* HMdcGetContainers::getKickPlane(Bool_t init) {
  if( !fKickPlane && isKickPlane ) {
    fKickPlane=(HKickPlane*)rtdb->getContainer("KickPlane");
    if(init) {
      //Adds kickplane's Parameter Io to the runtime database.
      HParIo *io=0;
      io=rtdb->getFirstInput();                                                         
      if (io != 0) {                                                                    
        if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {                        
          HKickParAsciiFileIo* p=                                                       
            new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());                 
          io->setDetParIo(p);                                                           
        }                                                                               
      }                                                                                 
      io=rtdb->getSecondInput();                                                        
      if (io != 0) {                                                                    
        if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {                        
          HKickParAsciiFileIo* p=                                                       
            new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());                 
          io->setDetParIo(p);                                                           
        }                                                                               
      }                                                                                 
      io=rtdb->getOutput();                                                             
      if (io != 0) {                                                                    
        if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {                        
          HKickParAsciiFileIo* p=                                                       
            new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());                 
          io->setDetParIo(p);                                                           
        }                                                                               
      }
      //--------------------------------
      if(!fKickPlane) rtdb->addContainer(fKickPlane=new HKickPlane);
//      isKickPlane=initCont(fKickPlane);
//      if(!isKickPlane) fKickPlane=0;
        isKickPlane=kTRUE; //?????????????????????
    }
  }
  return fKickPlane;
}

//------------------------------------------------------------------------

HGeomCompositeVolume* HMdcGetContainers::getGeomCompositeVolume(
    Int_t mod, Bool_t init) {
  if( !fMdcGeomPar && isMdcGeomPar ) getMdcGeomPar(init);
  return (isMdcGeomPar) ? fMdcGeomPar->getRefVolume(mod) : 0;
}

HMdcLayerGeomParLay* HMdcGetContainers::getMdcLayerGeomParLay(
    Int_t sec,Int_t mod,Int_t lay, Bool_t init) {
  if( !fMdcLayerGeomPar && isMdcLayerGeomPar ) getMdcLayerGeomPar(init);
  if( !isMdcLayerGeomPar ) return 0;
  HMdcLayerGeomParSec* fsec=&((*fMdcLayerGeomPar)[sec]);
  if( !fsec ) return 0;
  HMdcLayerGeomParMod* fmod=&((*fsec)[mod]);
  if( !fmod ) return 0;
  return &((*fmod)[lay]);
}

//------Categories:-------------------------
HCategory* HMdcGetContainers::getCatMdcClus(Bool_t create) {
//  HCategory *fCalCat=getCatMdcCal1();
//  if( !fCalCat ) return 0;
  HCategory *fClusCat = gHades->getCurrentEvent()->getCategory(catMdcClus);
  if(!fClusCat && create) {
    HMdcGetContainers* parCont=getObject();
    HMdcGeomStruct* p=parCont->getMdcGeomStruct();
    TArrayI* ind = new TArrayI(4);
    p->getMaxIndices(ind);
    Int_t sizes[3];   // sec, seg, clust.
    sizes[0]=ind->At(0)+1;
    sizes[1]=ind->At(1)/2+1;
    sizes[2]=500;            //??! 500 v buduschem nado umen'shit'
    if(isGeant()) fClusCat = new HMatrixCategory("HMdcClusSim",3,sizes,0.5);
    else fClusCat = new HMatrixCategory("HMdcClus",3,sizes,0.5);
    gHades->getCurrentEvent()->addCategory(catMdcClus,fClusCat,"Mdc");
    delete ind;
  }
  return fClusCat;
}
