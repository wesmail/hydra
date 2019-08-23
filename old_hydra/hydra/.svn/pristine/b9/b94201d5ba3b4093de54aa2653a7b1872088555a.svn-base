//*-- Author : V.Pechenov
//*--Modified : 12.05.2002 by V.Pechenov
//*--Modified : 17.01.2002 by V.Pechenov
//*--Modified : 31.05.2001
//*--Modified : 09.03.2001
//*--Modified : 10.08.2000

///////////////////////////////////////////////////////////////////////////////
//
//  HMdcClusterToHit("","", Int_t flagSeg1, Int_t flagSeg2)
//
//  fills the data objects of HMdcHit and HMdcSeg
//  after track candidate search task (HMdcTrackFinder).
//
// flagSeg1=-1;0;1
//   -1 - don't fill HMdcSeg
//   0 (or 1) - at the typeClustFinder=1 (see hmdctrackfinder.cc)
//     HMdcSeg will filled by hits from MDC1 (or MDC2)
// If in segment 1 is only one MDC (1 or 2) and
// flagSeg1=0 or 1  HMdcSeg will filled by Hit in this MDC.
// flagSeg2= -1;2;3
//    -/- -/-
//
//  HMdcHit and HMdcSeg:
//  Chi2 - size of cluster (in proj. plot bins)
//  user flag - "number of wires in cluster"*100 + "number of merged clusters"
//
//  Using:
//  Insert this task in task list after HMdcTrackFinder.
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcclustertohit.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hmdchitsim.h"
#include "hmdcsegsim.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"
#include "hmdcgetcontainers.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hmdcclussim.h"
#include "hmdcclusinf.h"
#include "hmdctrackdset.h"
#include "hmdcsizescells.h"
#include <stdlib.h>

HMdcClusterToHit::HMdcClusterToHit() {
  setParContainers();
}

HMdcClusterToHit::HMdcClusterToHit(Text_t *name, Text_t *title) :
    HReconstructor(name,title) {
  setParContainers();
}

HMdcClusterToHit::HMdcClusterToHit(Int_t fSeg1, Int_t fSeg2) {
  HMdcTrackDSet::setMdcClusterToHit(fSeg1,fSeg2);
  setParContainers();
}

HMdcClusterToHit::HMdcClusterToHit(Text_t *name, Text_t *title,
    Int_t fSeg1, Int_t fSeg2) : HReconstructor(name,title) {
  HMdcTrackDSet::setMdcClusterToHit(fSeg1,fSeg2);
  setParContainers();
}

HMdcClusterToHit::~HMdcClusterToHit(void) {
  HMdcTrackDSet::pClusterToHit=0;
}

void HMdcClusterToHit::setParContainers(void) {
  prntSt=kFALSE;
  HMdcTrackDSet::setClusterToHit(this);
  tFillSeg[0]=HMdcTrackDSet::modForSeg1;
  tFillSeg[1]=HMdcTrackDSet::modForSeg2;
}

Bool_t HMdcClusterToHit::init(void) {
  fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return kFALSE;
  fmdc=fGetCont->getMdcDetector();
  fClustCat = gHades->getCurrentEvent()->getCategory(catMdcClus);
  if( !fmdc || !fClustCat) return kFALSE;
  isGeant=HMdcGetContainers::isGeant();
  HMdcDetector* fMdcDet=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
  if(!fMdcDet) return kFALSE;

  fClusInfCat = HMdcGetContainers::getCatMdcClusInf(kTRUE);
  
  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    if(isGeant) fHitCat = fMdcDet->buildMatrixCategory("HMdcHitSim",0.5F);
    else fHitCat = fMdcDet->buildCategory(catMdcHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }

  fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fSegCat) {
    if(isGeant) fSegCat = fMdcDet->buildMatrixCategory("HMdcSegSim",0.5F);
    else fSegCat = fMdcDet->buildCategory(catMdcSeg);
    if (!fSegCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
  }
  iter=(HIterator *)fClustCat->MakeIterator("native");
  fSizesCells=HMdcSizesCells::getObject();

  locHit.set(3,0,0,0);
  locSeg.set(3,0,0,0);
  loc.set(1,0);
  if(!prntSt) printStatus();
  return kTRUE;
}

Bool_t HMdcClusterToHit::reinit(void) {
  if(!fSizesCells->initContainer()) return kFALSE;
  if(!fClusInfCat) return kFALSE;
  return kTRUE;
}

Int_t HMdcClusterToHit::execute(void) {
  iter->Reset();
  Int_t indexHit[6][4];
  Int_t indexSeg[6][2];
  for(Int_t s=0; s<6; s++) {
    indexSeg[s][0]=indexSeg[s][1]=0;
    for(Int_t m=0; m<4; m++) indexHit[s][m]=0;
  }
  while ((fClst=(HMdcClus *)iter->Next())!=0) {
    clusIndex=fClustCat->getIndex(fClst);
    Short_t sec=fClst->getSec();
    Short_t modCl=fClst->getMod();
    Short_t seg=fClst->getIOSeg();
    Short_t nMods=0;
    if(fmdc->getModule(sec,seg*2)) nMods++;
    if(fmdc->getModule(sec,seg*2+1)) nMods++;
    locHit[0]=sec;
    Int_t typeClFn=fClst->getTypeClFinder();
    fSeg = 0;
    if(tFillSeg[seg]!=-1 && (typeClFn==0 || nMods==1 || modCl==tFillSeg[seg])) {
      locSeg[0]=sec;
      locSeg[1]=seg;
      locSeg[2]=indexSeg[sec][seg]++;
      fSeg = (HMdcSeg*)fSegCat->getSlot(locSeg);
      if(!fSeg) {
        Error("execute","S.%i Cluster %i No slot HMdcSeg available",
            sec+1,indexSeg[sec][seg]+1);
        continue;
      }
      fSeg = (isGeant) ? (HMdcSeg*)(new(fSeg) HMdcSegSim) : new(fSeg) HMdcSeg;
      fSeg->setSec(sec);
      fSeg->setIOSeg(seg);
      fillSeg();
    }
    for(Int_t imod=0; imod<2; imod++) {
      Int_t mod=imod+seg*2;
      if(modCl>=0 && modCl!=mod) continue;
      if(!fmdc->getModule(sec,mod)) continue;

      locHit[1]=mod;
      locHit[2]=indexHit[sec][mod]++;
      Int_t index;
      fHit = (HMdcHit*)fHitCat->getSlot(locHit,&index);
      if(!fHit) {
        Error("execute","S.%i No slot HMdcHit available",sec+1);
        continue;
      }
      if(isGeant) fHit=(HMdcHit*) new(fHit) HMdcHitSim;
      else fHit= new(fHit) HMdcHit;
      fHit->setSecMod(sec,mod);
      if(!fillHit(mod)) return 1;

      Int_t t[4];
      for(Int_t layer=0; layer<6; layer++) {
        Int_t iLayer=layer+imod*6;
        Int_t firstCell=fClst->get4FirstCells(iLayer,t);
        if(firstCell<0) continue;
        fHit->setSignId(layer,firstCell,t[0],t[1],t[2],t[3]);
        if(fSeg) fSeg->setSignId(iLayer,firstCell,t[0],t[1],t[2],t[3]);
      }
      if(fSeg) fSeg->setHitInd(imod,index);
      if(isGeant) ((HMdcHitSim*)fHit)->calcNTracks();
    }
    if(fSeg) {
      Int_t nMrCls=fClst->getNMergClust();
      if(nMrCls>99) nMrCls=99;
      Int_t flag=(fClst->getNBins()&32767) <<16;
      flag += (fClst->getNDrTimes()*100 + nMrCls) & 65535;
      fSeg->setFlag(flag);
      fHit->setChi2(-1.);
      if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
    }
  }
  return 0;
}

void HMdcClusterToHit::fillSeg(void) {
  Double_t dZ1dX1, dZ1dY1,zErrTag;
  if(fClst->getIOSeg()==0 || HMdcTrackDSet::isCoiloff) {
    // target
    zErrTag=fClst->getErrZTarg();
    dZ1dX1=dZ1dY1=0.;
  } else {
    // kick plane
    zErrTag=0.;
    dZ1dX1=0.;
    dZ1dY1=fClst->getErrYTarg();
    dZ1dY1=(dZ1dY1>0.0001) ? -fClst->getErrZTarg()/dZ1dY1 : -1.274;
  }
  Double_t theta, phi, zm, r0;
  Double_t eTheta,ePhi,eZm,eR0;
  HMdcSizesCells::calcMdcSeg(fClst->getXTarg(),fClst->getYTarg(),fClst->getZTarg(),
      fClst->getX(),fClst->getY(),fClst->getZ(),fClst->getErrXTarg(),
      fClst->getErrYTarg(),zErrTag,fClst->getErrX(),fClst->getErrY(),
      dZ1dX1, dZ1dY1, -fClst->A(), -fClst->B(),
      zm,eZm, r0,eR0,theta,eTheta, phi,ePhi);
  fSeg->setPar(zm,eZm, r0,eR0, theta,eTheta, phi,ePhi);
}

Bool_t HMdcClusterToHit::fillHit(Short_t mod) {
  HMdcSizesCellsMod& fSCMod=(*fSizesCells)[fClst->getSec()][mod];
  if(!&fSCMod) return kFALSE;
  Double_t dZ1dX1, dZ1dY1,zErrTag;
  if(mod<2 || HMdcTrackDSet::isCoiloff) {
    // target
    zErrTag=fClst->getErrZTarg();
    dZ1dX1=dZ1dY1=0.;
  } else {
    // kick plane
    zErrTag=0.;
    dZ1dX1=0.;
    dZ1dY1=fClst->getErrYTarg();
    dZ1dY1=(dZ1dY1>0.0001) ? -fClst->getErrZTarg()/dZ1dY1 : -1.274;
  }
  Double_t x,eX,y,eY,xDir,eXDir,yDir,eYDir;
  fSCMod.calcMdcHit(fClst->getXTarg(),fClst->getYTarg(),fClst->getZTarg(),
      fClst->getX(),fClst->getY(),fClst->getZ(),fClst->getErrXTarg(),
      fClst->getErrYTarg(),zErrTag,fClst->getErrX(),fClst->getErrY(),
      dZ1dX1, dZ1dY1, -fClst->A(), -fClst->B(),
      x,eX, y,eY, xDir,eXDir, yDir,eYDir);
  fHit->setXY(x,eX,y,eY);
  fHit->setXYDir(xDir,eXDir,yDir,eYDir);
  fillClusInf(mod&1);
  return kTRUE;
}

void HMdcClusterToHit::fillClusInf(Int_t iMod) {
  Int_t index;
  HMdcClusInf* fClusInf=(HMdcClusInf*)fClusInfCat->getNewSlot(loc,&index);
  if(!fClusInf) {
    Error("fillFitCat","No slot HMdcClusInf available");
    return;
  } 
  fHit->setTrackFinder(1);
  fHit->setClusInfIndex(index);
  fHit->setChi2(-1.);
  fClusInf->setIsNotFitted();
  fClusInf->setLevelClFinding(fClst->getMinCl(iMod));
  Int_t nmods=fClst->getMod();
  fClusInf->setNModInCl( (nmods<0) ? -nmods:1 );
  fClusInf->setClusSize(fClst->getNBins());
  fClusInf->setNMergClust(fClst->getNMergClust());
  fClusInf->setNDrTimes(fClst->getNDrTimes(iMod*6,iMod*6+5));
  fClusInf->setClusIndex(clusIndex);
  fClusInf->setSigma1(fClst->getSigma1());
  fClusInf->setSigma2(fClst->getSigma2());
  fClusInf->setAlpha(fClst->getAlpha());
}

void HMdcClusterToHit::printStatus(void) {
  // prints the parameters to the screen
  char* hFSeg1a ="-1 = don't fill HMdcSeg for segment 1,";
  char* hFSeg1b =" 0 = fill HMdcSeg by cluster in MDC1 if typeClFinder=1";
  char* hFSeg1c ="     and sector has mdc1&2, else by cluster in existing MDC,";
  char* hFSeg1d =" 1 =  -/- in mdc2";
  char* hFSeg2a ="-1 = don't fill HMdcSeg for segment 2,";
  char* hFSeg2b =" 2 = fill HMdcSeg by cluster in MDC3 if typeClFinder=1";
  char* hFSeg2c ="     and sector has mdc3&4 and MagnetOff,";
  char* hFSeg2d ="     else by cluster in existing MDC,";
  char* hFSeg2e =" 3 =  -/- in MDC4";
  printf("--------------------------------------------------------------------------------------------\n");
  printf("HMdcClusterToHitSetup:\n");
  printf("ModForSeg1      = %2i :  %s\n",tFillSeg[0],hFSeg1a);
  printf("                        %s\n",hFSeg1b);
  printf("                        %s\n",hFSeg1c);
  printf("                        %s\n",hFSeg1d);
  printf("ModForSeg2      = %2i :  %s\n",tFillSeg[1],hFSeg2a);
  printf("                        %s\n",hFSeg2b);
  printf("                        %s\n",hFSeg2c);
  printf("                        %s\n",hFSeg2d);
  printf("                        %s\n",hFSeg2e);
  printf ("--------------------------------------------------------------------------------------------\n");
  prntSt=kTRUE;
}

ClassImp(HMdcClusterToHit)
