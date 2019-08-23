#include "hmdcgetcontainers.h"
#include "hkicktask.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"
#include "hmdcdetector.h"
#include "hspecgeompar.h"
#include "hmdcgeomstruct.h"
#include "hmdcrawstruct.h"
#include "hmdclookupgeom.h"
#include "hkickplane2.h"
#include "hgeomcompositevolume.h"
#include "hdetgeompar.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hparset.h"
#include "heventheader.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 07/05/2002 by V.Pechenov
//*-- Modified : 08/05/2001
//*-- Modified : 29/09/2000

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcGetContainers
//
// The table for geting of pointers to param.&data containers
// Using:
//    HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
// If the cont. HMdcGetContainers not exist it will automaticaly 
// created and added to RuntimeDb
//
//    HMdcGeomPar* fgeomPar=fGetCont->getMdcGeomPar(Bool_t init=kFALSE);
// If cont. HMdcGeomPar don't exist & init=kTURUE it will created and initialized
//    ...
//
// Transformations:
// lab <--> sector
//    const HGeomTransform& getLabTransSec(Int_t sec, Bool_t init=kFALSE);
//    Bool_t getLabTransSec(HGeomTransform& trans, Int_t sec, 
//        Bool_t init=kFALSE);
// lab <--> module
//    const HGeomTransform& getLabTransMod(Int_t sec,Int_t mod,
//        Bool_t init=kFALSE);
//    Bool_t getLabTransMod(HGeomTransform& trans, Int_t sec, Int_t mod,
//        Bool_t init=kFALSE);
// lab <--> layer
//    Bool_t getLabTransLayer(HGeomTransform& trans, Int_t sec,Int_t mod,
//        Int_t lay, Bool_t init=kFALSE);
// sector <--> module
//    Bool_t getSecTransMod(HGeomTransform& trans, Int_t sec, Int_t mod,
//        Bool_t init=kFALSE);
// sector <--> layer
//    Bool_t getSecTransLayer(HGeomTransform& trans, Int_t sec,Int_t mod,
//        Int_t lay, Bool_t init=kFALSE);
// module <--> layer
//    const HGeomTransform& getModTransLayer(Int_t mod, Int_t lay,
//        Bool_t init=kFALSE);
//    Bool_t getModTransLayer(HGeomTransform& trans, Int_t mod, Int_t lay,
//        Bool_t init=kFALSE);
//
//  Next finctions do shift of the transfomation
//  vector to real position of the sens. wires plane.
//    Bool_t getModTransLayerZ0(HGeomTransform& trans, Int_t mod, Int_t lay,
//        Bool_t init=kFALSE);
//    Bool_t getLabTransLayerZ0(HGeomTransform& trans, Int_t sec,Int_t mod,
//        Int_t lay, Bool_t init=kFALSE);
//    Bool_t getSecTransLayerZ0(HGeomTransform& trans, Int_t sec,Int_t mod,
//        Int_t lay, Bool_t init=kFALSE);
//
//////////////////////////////////////////////////////////////////

ClassImp(HMdcGetContainers)


HMdcGetContainers* HMdcGetContainers::fMdcGetContainers=0;
    
HMdcGetContainers::HMdcGetContainers() {
//  fName="MdcGetContainers";
//  fTitle="The table for geting of pointers to param.&data containers";
//  strcpy(detName,"Mdc");
  rtdb=0;
  spectr=0;
  isRtdb=kFALSE;
  if(gHades) {
    rtdb=gHades->getRuntimeDb();
//    rtdb->addContainer(this);
    isRtdb=kTRUE;
    spectr=gHades->getSetup(); 
  } else Warning("HMdcGetContainers","gHades=%i",0);
  
  fMdcLayerGeomPar=0;
  isMdcLayerGeomPar=kFALSE;
  fMdcGeomPar=0;
  isMdcGeomPar=kFALSE;
  fMdcDetector=0;
  isMdcDetector=kFALSE;
  fSpecGeomPar=0;
  isSpecGeomPar=kFALSE;
  fMdcGeomStruct=0;
  isMdcGeomStruct=kFALSE;
  fMdcRawStruct=0;
  isMdcRawStruct=kFALSE;
  fMdcLookupGeom=0;
  isMdcLookupGeom=kFALSE;
  fKickPlane=0;
  isKickPlane=kFALSE;
}

void HMdcGetContainers::deleteCont() {
  if(fMdcGetContainers) {
    delete fMdcGetContainers;
    fMdcGetContainers=0;
  }
}

HMdcGetContainers* HMdcGetContainers::getObject() {
  if(!fMdcGetContainers) fMdcGetContainers=new HMdcGetContainers();
  return fMdcGetContainers;
}

Bool_t HMdcGetContainers::initCont(HParSet *par) {
  if( !par ) return kFALSE;
  if( !isInited(par) || !par->isStatic() ) return kTRUE;
  return par->init();
}

Bool_t HMdcGetContainers::isInited(HParSet *par) {
 if( !par ) return kFALSE;
 return (par->getInputVersion(1)>=0 || par->getInputVersion(2)>=0)
            ? kTRUE:kFALSE;
}

HMdcLayerGeomPar* HMdcGetContainers::getMdcLayerGeomPar(Bool_t init) {
  if( !fMdcLayerGeomPar && isRtdb )
    fMdcLayerGeomPar=(HMdcLayerGeomPar*)rtdb->getContainer("MdcLayerGeomPar");
  if( init && !isMdcLayerGeomPar ) isMdcLayerGeomPar=initCont(fMdcLayerGeomPar);
  return fMdcLayerGeomPar;
}

HMdcGeomPar* HMdcGetContainers::getMdcGeomPar(Bool_t init) {
  if( !fMdcGeomPar && isRtdb )
    fMdcGeomPar=(HMdcGeomPar*)rtdb->getContainer("MdcGeomPar");
  if( init && !isMdcGeomPar ) isMdcGeomPar=initCont(fMdcGeomPar);
  return fMdcGeomPar;
}
    
HMdcDetector* HMdcGetContainers::getMdcDetector() {
  if( !fMdcDetector )
    fMdcDetector=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  return fMdcDetector;
}

HSpecGeomPar* HMdcGetContainers::getSpecGeomPar(Bool_t init) {
  if( !fSpecGeomPar && isRtdb )
    fSpecGeomPar=(HSpecGeomPar*)rtdb->getContainer("SpecGeomPar");
  if( init&& !isSpecGeomPar ) isSpecGeomPar=initCont(fSpecGeomPar);
  return fSpecGeomPar;
}


HMdcGeomStruct* HMdcGetContainers::getMdcGeomStruct(Bool_t init) {
  if( !fMdcGeomStruct && isRtdb )
    fMdcGeomStruct=(HMdcGeomStruct*)rtdb->getContainer("MdcGeomStruct");
  if( init && !isMdcGeomStruct ) isMdcGeomStruct=initCont(fMdcGeomStruct);
  return fMdcGeomStruct;
}


HMdcRawStruct* HMdcGetContainers::getMdcRawStruct(Bool_t init) {
  if( !fMdcRawStruct && isRtdb )
    fMdcRawStruct=(HMdcRawStruct*)rtdb->getContainer("MdcRawStruct");
  if( init && !isMdcRawStruct ) isMdcRawStruct=initCont(fMdcRawStruct);
  return fMdcRawStruct;
}

HMdcLookupGeom* HMdcGetContainers::getMdcLookupGeom(Bool_t init) {
  if( !fMdcLookupGeom && isRtdb )
    fMdcLookupGeom=(HMdcLookupGeom*)rtdb->getContainer("MdcLookupGeom");
  if( init && !isMdcLookupGeom ) isMdcLookupGeom=initCont(fMdcLookupGeom);
  return fMdcLookupGeom;
}

HKickPlane2* HMdcGetContainers::getKickPlane(Bool_t init) {
  if( !fKickPlane ) fKickPlane = HKickPlane2::getMDC3();
  if( init && !isKickPlane ) isKickPlane=initCont(fKickPlane);
  return fKickPlane;
}

//------------------------------------------------------------------------

HGeomCompositeVolume* HMdcGetContainers::getGeomCompositeVolume(
    Int_t mod, Bool_t init) {
  if(!fMdcGeomPar && isMdcGeomPar ) getMdcGeomPar(init);
  return (fMdcGeomPar) ? fMdcGeomPar->getRefVolume(mod) : 0;
}

HMdcLayerGeomParLay* HMdcGetContainers::getMdcLayerGeomParLay(
    Int_t sec,Int_t mod,Int_t lay, Bool_t init) {
  getMdcLayerGeomPar(init);
  if( !fMdcLayerGeomPar ) return 0;
  HMdcLayerGeomParSec* fsec=&((*fMdcLayerGeomPar)[sec]);
  if( !fsec ) return 0;
  HMdcLayerGeomParMod* fmod=&((*fsec)[mod]);
  if( !fmod ) return 0;
  return &((*fmod)[lay]);
}

HModGeomPar* HMdcGetContainers::getModGeomPar(Int_t sec,Int_t mod,Bool_t init) {
  getMdcGeomPar(init);
  return (fMdcGeomPar) ? fMdcGeomPar->getModule(sec,mod) : 0;
}

const HGeomTransform& HMdcGetContainers::getLabTransSec(Int_t sec,Bool_t init) {
  // return transformation sector<-->lab.
  HSpecGeomPar* fSpecGeomPar=getSpecGeomPar(init);
  if(fSpecGeomPar) {
    HGeomVolume* fVol=fSpecGeomPar->getSector(sec);
    if(fVol) return fVol->getTransform();
  }
  Error("getLabTransSec","can't get the transformation lab.<-->sec. (S%i)",
      sec+1);
  return *((HGeomTransform*)0);
}

Bool_t HMdcGetContainers::getLabTransSec(HGeomTransform& trans, Int_t sec,
    Bool_t init) {
  const HGeomTransform& tr=getLabTransSec(sec,init);
  if( !&tr ) return kFALSE;
  trans=tr;
  return kTRUE;
}

const HGeomTransform& HMdcGetContainers::getLabTransMod(Int_t sec, Int_t mod,
    Bool_t init) {
  //return transformation module<-->lab.
  HModGeomPar* fMod=getModGeomPar(sec,mod,init);
  if(fMod) return fMod->getLabTransform();
  Error("getLabTransMod","can't get the transformation lab.<-->mod. (S%iM%i)",
      sec+1,mod+1);
  return *((HGeomTransform*)0);
}

Bool_t HMdcGetContainers::getLabTransMod(HGeomTransform& trans, Int_t sec,
    Int_t mod, Bool_t init) {
  const HGeomTransform& tr=getLabTransMod(sec,mod,init);
  if( !&tr ) return kFALSE;
  trans=tr;
  return kTRUE;
}

Bool_t HMdcGetContainers::getSecTransMod(HGeomTransform& trans, Int_t sec, 
    Int_t mod, Bool_t init) {
  //return transformation module<-->sector.
  const HGeomTransform& tr=getLabTransMod(sec,mod,init);
  if( !&tr ) return kFALSE;
  const HGeomTransform& fSec=getLabTransSec(sec,init);
  if( &fSec==0 ) return kFALSE;
  trans=tr;
  trans.transTo(fSec);
  return kTRUE;
}

const HGeomTransform& HMdcGetContainers::getModTransLayer( Int_t mod, Int_t lay, 
    Bool_t init) {
  //return transformation layer<-->module.
  HGeomCompositeVolume* fMod=getGeomCompositeVolume(mod,init);
  if(fMod) {
    HGeomVolume* fVol=fMod->getComponent(lay);
    if(fVol) {
      const HGeomTransform& trans=fVol->getTransform();
      if(&trans) return trans;
    }
  }
  Error("getModTransLayer",
    "can't get the transformation mod.<-->lay. (M%iL%i)",mod+1,lay+1);
  return *((HGeomTransform*)0);
}

Bool_t HMdcGetContainers::getModTransLayer(HGeomTransform& trans, Int_t mod, 
    Int_t lay, Bool_t init) {
  const HGeomTransform& tr=getModTransLayer(mod,lay,init);
  if( !&tr ) return kFALSE;
  trans=tr;
  return kTRUE;
}

Bool_t HMdcGetContainers::getLabTransLayer(HGeomTransform& trans, 
    Int_t sec,Int_t mod, Int_t lay, Bool_t init) {
  //return transformation layer<-->lab.
  const HGeomTransform& tr=getModTransLayer(mod,lay,init);
  if( !&tr ) return kFALSE;
  const HGeomTransform& fLabMod=getLabTransMod(sec,mod,init);
  if( !&fLabMod ) return kFALSE;
  trans=tr;
  trans.transFrom(fLabMod);
  return kTRUE;
}

Bool_t HMdcGetContainers::getSecTransLayer(HGeomTransform& trans, 
    Int_t sec,Int_t mod, Int_t lay, Bool_t init) {
  //return transformation layer<-->sector.
  if( !getLabTransLayer(trans,sec,mod,lay,init) )return kFALSE;
  const HGeomTransform& fSec=getLabTransSec(sec,init);
  if(&fSec==0) return kFALSE;
  trans.transTo(fSec); 
  return kTRUE;
}

Bool_t HMdcGetContainers::getModTransLayerZ0(HGeomTransform& trans, 
    Int_t mod, Int_t lay, Bool_t init) {
  //return transformation layer<-->module (do shift of the transfomation
  // vector along Z to real position of the sens. wires plane).
  HGeomCompositeVolume* fMod=getGeomCompositeVolume(mod,init);
  if(fMod) {
    HGeomVolume* fVol=fMod->getComponent(lay);
    if(fVol) {
      Double_t zShift=
          (fVol->getPoint(0)->getZ()+fVol->getPoint(4)->getZ())*0.5;
      const HGeomTransform& tr=fVol->getTransform();
      if( !&tr ) return kFALSE;
      trans=tr;
      HGeomVector trVec(trans.getTransVector());
      trVec.setZ(trVec.getZ() + zShift);
      trans.setTransVector(trVec);
      return kTRUE;
    }
  }
  Error("getModTransLayerZ0",
    "can't get the transformation mod.<-->lay. (M%iL%i)",mod+1,lay+1);
  return kFALSE;
}

Bool_t HMdcGetContainers::getLabTransLayerZ0(HGeomTransform& trans, 
    Int_t sec,Int_t mod, Int_t lay, Bool_t init) {
  //return transformation layer<-->lab. (do shift of the transfomation
  // vector along Z to real position of the sens. wires plane).
  const HGeomTransform& fLabMod=getLabTransMod(sec,mod,init);
  if( !&fLabMod ) return kFALSE;
  if( !getModTransLayerZ0(trans, mod,lay,init) ) return kFALSE;
  trans.transFrom(fLabMod); 
  return kTRUE;
}


Bool_t HMdcGetContainers::getSecTransLayerZ0(HGeomTransform& trans, 
    Int_t sec,Int_t mod, Int_t lay, Bool_t init) {
  //return transformation layer<-->sector (do shift of the transfomation
  // vector along Z to real position of the sens. wires plane).
  const HGeomTransform& fSec=getLabTransSec(sec,init);
  if( !&fSec ) return kFALSE;
  if( !getLabTransLayerZ0(trans,sec,mod,lay,init) )return kFALSE;
  trans.transTo(fSec); 
  return kTRUE;
}

Bool_t HMdcGetContainers::getSecTransCathLayerZ0(HGeomTransform& trans1, 
    HGeomTransform& trans2, Int_t sec,Int_t mod, Int_t lay, Bool_t init) {
  //return transformations of two cathode layers<-->sector (do shift of the transfomation
  // vector along Z to real position of the sens. wires plane).
  const HGeomTransform& fSec=getLabTransSec(sec,init);
  if( !&fSec ) return kFALSE;
  const HGeomTransform& fLabMod=getLabTransMod(sec,mod,init);
  if( !&fLabMod ) return kFALSE;
  HMdcLayerGeomPar* fLayerGeomPar = getMdcLayerGeomPar(init);
  if(!fLayerGeomPar) return kFALSE;
  HMdcLayerGeomParLay& fLayerGeomParLay = (*fLayerGeomPar)[sec][mod][lay];
  if(!&fLayerGeomParLay) {
    Error("getSecTransCathLayerZ0",
        "can't get MdcLayerGeomParLay cont. (%iS %iM %iL)",sec+1,mod+1,lay+1);
    return kFALSE;
  }
  Double_t halfCatDist=fLayerGeomParLay.getCatDist()*0.5;
  HGeomCompositeVolume* fMod=getGeomCompositeVolume(mod,init);
  if(!fMod) {
    Error("getSecTransCathLayerZ0",
        "can't get the transformation mod.<-->lay. (M%iL%i)",mod+1,lay+1);
  }
  HGeomVolume* fVol=fMod->getComponent(lay);
  if(!fVol) return kFALSE;
  Double_t zShift=(fVol->getPoint(0)->getZ()+fVol->getPoint(4)->getZ())*0.5;
  const HGeomTransform& tr=fVol->getTransform();
  if( !&tr ) return kFALSE;
  trans1=tr;
  trans2=tr;
  HGeomVector trVec1(tr.getTransVector());
  trVec1.setZ(trVec1.getZ() + zShift);
  HGeomVector trVec2(trVec1);
  trVec1.setZ(trVec1.getZ() - halfCatDist);
  trVec2.setZ(trVec2.getZ() + halfCatDist);
  trans1.setTransVector(trVec1);
  trans2.setTransVector(trVec2);
  
  trans1.transFrom(fLabMod);
  trans2.transFrom(fLabMod);
  
  trans1.transTo(fSec);
  trans2.transTo(fSec);
  return kTRUE;
}

//------Categories:-------------------------
HCategory* HMdcGetContainers::getCatMdcClus(Bool_t create) {
  HCategory *fClusCat = gHades->getCurrentEvent()->getCategory(catMdcClus);
  if(!fClusCat && create) {
    Int_t sizes[3]={6,2,500};   // sec, seg, clust.(maximum 500 clusters in seg)
    if(isGeant()) fClusCat = new HMatrixCategory("HMdcClusSim",3,sizes,0.2);
    else fClusCat = new HMatrixCategory("HMdcClus",3,sizes,0.5);
    gHades->getCurrentEvent()->addCategory(catMdcClus,fClusCat,"Mdc");
  }
  return fClusCat;
}

HCategory* HMdcGetContainers::getCatMdcTrkCand(Bool_t create) {
  if(gHades==0) return 0; 
  HCategory *fTrkCandCat =gHades->getCurrentEvent()->getCategory(catMdcTrkCand);
  if(!fTrkCandCat && create) {
    HMdcDetector* fMdcDet=
        (HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
    if(!fMdcDet) return 0;
    fTrkCandCat=fMdcDet->buildCategory(catMdcTrkCand);
    if (!fTrkCandCat) return 0;
    gHades->getCurrentEvent()->addCategory(catMdcTrkCand,fTrkCandCat,"Mdc");
  }
  return fTrkCandCat;
}

HCategory* HMdcGetContainers::getCatMdcClusFit(Bool_t create) {
  HCategory *fClFitCat = gHades->getCurrentEvent()->getCategory(catMdcClusFit);
  if(!fClFitCat && create) {  
    if(isGeant()) fClFitCat = new HLinearCategory("HMdcClusFitSim",4000);  
    else fClFitCat = new HLinearCategory("HMdcClusFit",4000);
    gHades->getCurrentEvent()->addCategory(catMdcClusFit,fClFitCat,"Mdc");
  }
  return fClFitCat;
}

HCategory* HMdcGetContainers::getCatMdcWireFit(Bool_t create) {
  HCategory *fClWireCat = gHades->getCurrentEvent()->getCategory(catMdcWireFit);
  if(!fClWireCat && create) {
    if(isGeant()) fClWireCat = new HLinearCategory("HMdcWireFitSim",40000);  
    else          fClWireCat = new HLinearCategory("HMdcWireFit",   40000);
    gHades->getCurrentEvent()->addCategory(catMdcWireFit,fClWireCat,"Mdc");
  }
  return fClWireCat;
}

HCategory* HMdcGetContainers::getCatMdcClusInf(Bool_t create) {
  HCategory *fClusInf = gHades->getCurrentEvent()->getCategory(catMdcClusInf);
  if(!fClusInf && create) {    
    if(isGeant()) fClusInf = new HLinearCategory("HMdcClusInfSim",4000);
    else fClusInf = new HLinearCategory("HMdcClusInf",4000);
    gHades->getCurrentEvent()->addCategory(catMdcClusInf,fClusInf,"Mdc");
  }
  return fClusInf;
}

void HMdcGetContainers::getFileName(TString& fileName) {
  // Return file name without dir.name and extension (.hld, .root, ...)
  getFileNameWInd(fileName);
  Int_t ind=fileName.Index(".");
  if(ind>0) fileName=fileName.Remove(ind);
}

void HMdcGetContainers::getFileNameWInd(TString& fileName) {
  // Return file name without dir.name
  fileName=gHades->getRuntimeDb()->getCurrentFileName();
  Int_t ind=fileName.Index("/");
  while(ind>=0) {
    fileName=fileName.Remove(0,ind+1);
    ind=fileName.Index("/");
  }
}

void HMdcGetContainers::getRunId(TString& runId) {
  // Return runId
  HEventHeader* evHeader=gHades->getCurrentEvent()->getHeader();
  if(!evHeader) {
    runId="";
    return;
  }
  UInt_t id=evHeader->getEventRunNumber();
  char buf[20];
  sprintf(buf,"%i",id);
  runId=buf;
}

void HMdcGetContainers::getEventDate(TString& eventDate) {
  // Return current event date
  HEventHeader* evHeader=gHades->getCurrentEvent()->getHeader();
  if(!evHeader) {
    eventDate="";
    return;
  }
  UInt_t date=evHeader->getDate();
  UChar_t* dt=(UChar_t*)&date;
  char buf[20];
  sprintf(buf,"%02i-%02i-%4i",dt[0],dt[1]+1,1900+dt[2]);
  eventDate=buf;
}

void HMdcGetContainers::getEventTime(TString& eventTime) {
  // Return current event time
  HEventHeader* evHeader=gHades->getCurrentEvent()->getHeader();
  if(!evHeader) {
    eventTime="";
    return;
  }
  UInt_t time=evHeader->getTime();
  UChar_t* tm=(UChar_t*)&time;
  char buf[20];
  sprintf(buf,"%02i:%02i:%02i",tm[2],tm[1],tm[0]);
  eventTime=buf;
}
