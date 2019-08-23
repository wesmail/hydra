//*--- Author : V.Pechenov
//*--- Modified: 26.06.07 V.Pechenov
//*--- $Id: hmdc12fit.cc,v 1.64 2008-05-09 16:12:05 halo Exp $
//*--- Modified: 23.01.07 V.Pechenov
//*--- Modified: 17.06.05 V.Pechenov
//*--- Modified: 29.10.03 V.Pechenov
//*--- Modified: 21.07.03 V.Pechenov
//*--- Modified: 04.04.03 V.Pechenov
//*--- Modified: 17.07.02 V.Pechenov
//*--- Modified: 16.08.01 V.Pechenov
//*--- Modified: 17.10.00 A.Nekhaev

using namespace std;
#include "hmdc12fit.h"
#include "hdebug.h"
#include "hevent.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hmatrixcategory.h"
#include <iostream>
#include <iomanip>
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hmdcgetcontainers.h"
#include "hmdcclussim.h"
#include "hmdcdetector.h"
#include "hmdctrackfittera.h"
#include "hmdctrackfitterb.h"
#include "hmdctrackdset.h"
#include "hmdcclusinfsim.h"
#include "hruntimedb.h"
#include "hmdc34clfinder.h"
#include "hmdctrkcand.h"
#include "hmdcgeanttrack.h"

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
// 
// HMdcTrackFittesrArray
//
// Array of HMdcTrackFitterA (B,...) objects
//
//
// HMdc12Fit
//
// Manager class for Dubna track straight piece fitter.
//
//////////////////////////////////////////////////////////////////////////////


ClassImp(HMdc12Fit)
ClassImp(HMdcFittersArray)

HMdcFittersArray::HMdcFittersArray(void) {
  nObj=0;
  fitAuthor=1;
  fitpar=0;
  size=100;
  fitterArr=new HMdcTrackFitter* [size];
  for(Int_t i=0;i<size;i++) fitterArr[i]=0;
}

HMdcFittersArray::~HMdcFittersArray(void) {
  if(fitterArr) {
    for(Int_t i=0;i<size;i++) if(fitterArr[i]!=0) delete fitterArr[i];
    delete [] fitterArr;
    fitterArr=0;
  }
}

void HMdcFittersArray::init(Int_t fitAut, HMdcTrackFitInOut* fpar) {
  fitpar=fpar;
  fitAuthor=fitAut;
}

void HMdcFittersArray::expand(void) {
  Int_t newSize=size+50;
  HMdcTrackFitter** fitterArrOld=fitterArr;
  fitterArr=new HMdcTrackFitter* [newSize];
  for(Int_t i=0;i<size;i++) fitterArr[i]=fitterArrOld[i];
  for(Int_t i=size;i<newSize;i++) fitterArr[i]=0;
  size=newSize;
  delete [] fitterArrOld;
}

HMdcTrackFitter* HMdcFittersArray::getNewFitter(void) {
  if(nObj==size) expand();
  HMdcTrackFitter* fitter=fitterArr[nObj];
  if(fitter==0) {
    if(fitAuthor == 2) fitter=(HMdcTrackFitter*) new HMdcTrackFitterB(fitpar);
    else fitter=(HMdcTrackFitter*) new HMdcTrackFitterA(fitpar);
    fitterArr[nObj]=fitter;
  }
  fitter->getInitParam()->setOneSecData();
  fitter->getFinalParam()->setOneSecData();
  nObj++;
  return fitter;
}

//--------------------------------------------------------------------

HMdc12Fit::HMdc12Fit(Int_t type,Int_t ver,Int_t dp,Bool_t hst,Bool_t prnt) {
  HMdcTrackDSet::setMdc12Fit(type,ver,dp,hst,prnt);
  setParContainers();
}

HMdc12Fit::HMdc12Fit(Text_t *name,Text_t *title,Int_t type, Int_t ver,Int_t dp,
    Bool_t hst, Bool_t prnt) : HReconstructor(name,title) {
  HMdcTrackDSet::setMdc12Fit(type,ver,dp,hst,prnt);
  setParContainers();
}

HMdc12Fit::HMdc12Fit() {
  setParContainers();
}

HMdc12Fit::HMdc12Fit(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  setParContainers();
}

HMdc12Fit::~HMdc12Fit() {
  HMdcSizesCells::deleteCont();
  HMdcTrackDSet::p12Fit=0;
  if(iterClus){delete iterClus;iterClus=0;}
}

void HMdc12Fit::setParContainers(void) {
  HMdcTrackDSet::set12Fit(this);
  prntSt             = kFALSE;
  fSegCat            = 0; 
  iterClus           = 0; 
  modForSeg[0]       = 1;
  modForSeg[1]       = 3;
  fitpar.cutWeight   = 0.01;
  fitpar.signalSpeed = 0.0;
  notFillByClus      = kFALSE;
  isCosmicData       = kFALSE;
  pTrackInfSim       = 0;
}

Bool_t HMdc12Fit::init(void) {
  HMdcGetContainers* fGetCont = HMdcGetContainers::getObject();
  fClusCat = fGetCont->getCatMdcClus();
  if( !fClusCat ) return kFALSE;
  iterClus=(HIterator *)fClusCat->MakeIterator();
  
  if(!fitpar.init()) return kFALSE;
  isGeant = fitpar.geantFlag;
  HMdcDetector* fMdcDet = fGetCont->getMdcDetector();
  modForSeg[0] = HMdcTrackDSet::modForSeg1;
  modForSeg[1] = HMdcTrackDSet::modForSeg2;
  if(modForSeg[0]>=0 && modForSeg[1]>0) {
    fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if(!fSegCat) {
      if(isGeant) fSegCat = fMdcDet->buildMatrixCategory("HMdcSegSim",0.5F);
      else        fSegCat = fMdcDet->buildCategory(catMdcSeg);
      if (!fSegCat) return kFALSE;
      gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
    }
  }

  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    if(isGeant) fHitCat = fMdcDet->buildMatrixCategory("HMdcHitSim",0.5F);
    else        fHitCat = fMdcDet->buildCategory(catMdcHit);
    if (!fHitCat) return kFALSE;
    gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }
  if(isCosmicData) {
    fTrkCandCat = 0;
    fClusInfCat = 0;
  } else {
    fTrkCandCat = fGetCont->getCatMdcTrkCand(kTRUE);
    if(!fTrkCandCat) return kFALSE;
    fClusInfCat = fGetCont->getCatMdcClusInf(kTRUE);
    if(!fClusInfCat) return kFALSE;
  }


  locClInf.set(1,0);
  locSeg.set(2,0,0);
  locHit.set(2,0,0);
  locTrkCand.set(1,0);
  locClus.set(2,0,0);

  if(HMdcTrackDSet::fitAuthor==0) {
    Warning("init",
        "Hejdar's tack fitter not suported more, Alexander's tack fitter will used!");
    HMdcTrackDSet::fitAuthor=1;
  }
      
  fittersArr[0].init(HMdcTrackDSet::fitAuthor,&fitpar);
  fittersArr[1].init(HMdcTrackDSet::fitAuthor,&fitpar);
  if(!prntSt) printStatus();
  f34ClFinder  = HMdc34ClFinder::getExObject();
  if(isGeant) pTrackInfSim = HMdcGeantEvent::getMdcTrackInfSim();
  return kTRUE;
}

Bool_t HMdc12Fit::reinit(void) {
  if(!fitpar.reinit()) return kFALSE;
  if(isCosmicData) {
    clusFitAlg=4;
    fitpar.doTargScan=kFALSE;
  } else {
    clusFitAlg=1;
    if(HMdcTrackDSet::isCoilOff()) {
      if(HMdcTrackDSet::getClFinderType()==1) clusFitAlg=2;
      else clusFitAlg=3;
    }
  }
  return kTRUE;
}

Int_t HMdc12Fit::execute(void) {
  if(HMdcTrackDSet::fprint) {
    fitpar.setPrintFlag(gHades->getEventCounter());
    if(fitpar.fprint) printf(
      "============================== Event %i =============================\n",
      gHades->getEventCounter());
  }
  iterClus->Reset();
  if(clusFitAlg==1) fitAlgorithm1();        // magnet on                       
  else if(clusFitAlg==2) fitAlgorithm2();   // magnet off + chanmer clusters   
  else if(clusFitAlg==3) fitAlgorithm3b();   // magnet off + sector clusters    
  else if(clusFitAlg==4) fitAlgorithm4();   // cosmic data 
  if(isGeant) {
    iterClus->Reset();
    HMdcClusSim* clusSim;
    while ((clusSim=(HMdcClusSim*)iterClus->Next())!=0) 
        if(clusSim->getIOSeg()==1) clusSim->sortTrListByContr();
  }
  return 0;
}

void HMdc12Fit::fitAlgorithm1(void) {
  Int_t maxNumSectors=((HMatrixCategory*)fClusCat)->getSize(0);
  locClus[1]=0;
  for(Int_t sec=0; sec<maxNumSectors; sec++){
     //      iterClus->Reset();
    locClus[0]=sec;
    locTrkCand[0]=sec;
    iterClus->gotoLocation(locClus);
    Int_t nMdcsSeg1=fitpar.fSizesCells->nModInSeg(sec,0);
    Int_t nMdcsSeg2=fitpar.fSizesCells->nModInSeg(sec,1);
    HMdcClus * fClstSeg1;
    while((fClstSeg1=(HMdcClus*)iterClus->Next())) {
      // Segment 1:
      Bool_t flag=kFALSE;
      Int_t typeClFnSeg1=fClstSeg1->getTypeClFinder();
      fittersArr[0].reset();
      if(nMdcsSeg1==1) flag=fitMod(fClstSeg1);         // One mdc per segment
      else if(typeClFnSeg1==0) {                       // Two mdc per segment
        if(HMdcTrackDSet::fitType==0) flag=fitSeg(fClstSeg1);
        else if(HMdcTrackDSet::fitType==1) flag=fitCombClusIndFit(fClstSeg1);
      } else if(typeClFnSeg1==1) flag=fitChambClus(fClstSeg1);
      else if(typeClFnSeg1==2) flag=fitMixedClus(fClstSeg1);
      if(!flag) continue;
      // HMdcTrkCand filling:
      fitter=fittersArr[0].getFitter(0); // ???
      if(fitter->segIndex<0) continue;
      HMdcTrkCand* fTrkCand=fillTrkCandISeg();
      if(fTrkCand==0) continue;
      // Segment 2:
      if(nMdcsSeg2>0) {
        // Cluster finder in segment 2.
        if(findSeg2Clusters(fitter,/*kFALSE*/kTRUE) <= 0) continue;
      }

      Int_t firstCh,lastCh;
      fClstSeg1->getIndexRegChilds(firstCh,lastCh);
      fittersArr[1].reset();
      for(Int_t ind=firstCh;ind<=lastCh;ind++) {
        HMdcClus* fClstSeg2=(HMdcClus*)fClusCat->getObject(ind);
        if(fClstSeg2==0) {
          Error("fitAlgorithm1","Absent cluster with ind=%i!",ind);
          continue;
        }
        Int_t typeClFnSeg2=fClstSeg2->getTypeClFinder();
        if(nMdcsSeg2==1) flag=fitMod(fClstSeg2,1);        // One mdc per segment
        else if(typeClFnSeg2==0) {                        // Two mdc per segment
          if(HMdcTrackDSet::fitType==0) flag=fitSeg(fClstSeg2,1);
          else if(HMdcTrackDSet::fitType==1) flag=fitCombClusIndFit(fClstSeg2,1);
        } else if(typeClFnSeg2==1) flag=fitChambClus(fClstSeg2,1);
        else if(typeClFnSeg2==2) flag=fitMixedClus(fClstSeg2,1);
        if(!flag || fitter->segIndex<0) continue;
        // HMdcTrkCand filling:
        fTrkCand=fillTrkCandOSeg(fTrkCand);
        if(!fTrkCand) break;
      }
    }
  }
}

Int_t HMdc12Fit::findSeg2Clusters(HMdcTrackFitter* fitter, Bool_t flag) {
  // Cluster finder in segment 2.
  // flag=kTRUE - use seg1 cluster; =kFALSE - use fitted cluster (if fit ok.)
  if(f34ClFinder==0) return 0;
  HMdc34ClFinderSec& f34CFSc=(*f34ClFinder)[fitter->wires.getSector()];
  if(!&f34CFSc || f34CFSc.notEnoughWires()) return 0;
  if(!fitter->fitStatus || flag) 
    return f34CFSc.findClustersSeg2(fitter->wires.getClust1());
  HMdcSeg* fSeg=(HMdcSeg*)fSegCat->getObject(fitter->segIndex);
  if(fSeg==0) return 0;
  return f34CFSc.findClustersSeg2(fSeg,fitter->wires.getClust1());
}

void HMdc12Fit::fitAlgorithm2(void) {
  // Magnet off + chamber clusters
  Int_t maxNumSectors=((HMatrixCategory*)fClusCat)->getSize(0);
  Int_t maxNumSegments=((HMatrixCategory*)fClusCat)->getSize(1);
  for(locClus[0]=0; locClus[0]<maxNumSectors; locClus[0]++){
    for(locClus[1]=0; locClus[1]<maxNumSegments; locClus[1]++) {
       //      iterClus->Reset();
      iterClus->gotoLocation(locClus);
      Int_t nMdcs=fitpar.fSizesCells->nModInSeg(locClus[0],locClus[1]);
      HMdcClus * fClst;
      while((fClst=(HMdcClus*)iterClus->Next())) {
        Bool_t flag=kFALSE;
        fittersArr[0].reset();
        if(nMdcs==1) flag=fitMod(fClst);             // One mdc per segment
        else flag=fitChambClus(fClst);               // Two mdc per segment
      }
    }
  }
}

void HMdc12Fit::fitAlgorithm3(void) {
  // Segment fit
  // Magnet off + combined clusters in sector
  Int_t maxNumSectors=((HMatrixCategory*)fClusCat)->getSize(0);
  locClus[1]=0;
  for(Int_t sec=0; sec<maxNumSectors; sec++){
     //      iterClus->Reset();
    locClus[0]=sec;
    locTrkCand[0]=sec;
    iterClus->gotoLocation(locClus);
    Int_t nMdcsSeg1=fitpar.fSizesCells->nModInSeg(sec,0);
    Int_t nMdcsSeg2=fitpar.fSizesCells->nModInSeg(sec,1);
    HMdcClus * fClstSeg1;
    while((fClstSeg1=(HMdcClus*)iterClus->Next())) {
      // Segment 1:
      Bool_t flag=kFALSE;
      Int_t typeClFnSeg1=fClstSeg1->getTypeClFinder();
      fittersArr[0].reset();
      if(nMdcsSeg1==1) flag=fitMod(fClstSeg1);           // One mdc per segment
      else if(typeClFnSeg1==0) {                         // Two mdc per segment
        if(HMdcTrackDSet::fitType==0) flag=fitSeg(fClstSeg1);
        else if(HMdcTrackDSet::fitType==1) flag=fitCombClusIndFit(fClstSeg1);
      } else if(typeClFnSeg1==2) flag=fitMixedClus(fClstSeg1);
      
      // Segment 2:
      if(!flag) continue;
      
      // HMdcTrkCand filling:
      fitter=fittersArr[0].getFitter(0); // ???
      if(fitter->segIndex<0) continue;
      HMdcTrkCand* fTrkCand=fillTrkCandISeg();
      if(fTrkCand==0) continue;
      
      Int_t firstCh,lastCh;
      fClstSeg1->getIndexRegChilds(firstCh,lastCh);
      fittersArr[1].reset();
      for(Int_t ind=firstCh;ind<=lastCh;ind++) {
        HMdcClus* fClstSeg2=(HMdcClus*)fClusCat->getObject(ind);
        if(fClstSeg2==0) {
          Error("fitAlgorithm3","Absent cluster with ind=%i!",ind);
          continue;
        }
        flag=kFALSE;
        Int_t typeClFnSeg2=fClstSeg2->getTypeClFinder();
        if(nMdcsSeg2==1) flag=fitMod(fClstSeg2,1);       // One mdc per segment
        else if(typeClFnSeg2==0) {                       // Two mdc per segment
          if(HMdcTrackDSet::fitType==0) flag=fitSeg(fClstSeg2,1);
          else if(HMdcTrackDSet::fitType==1) flag=fitCombClusIndFit(fClstSeg2,1);
        }
        if(!flag || fitter->segIndex<0) continue;
        // HMdcTrkCand filling:
        fTrkCand=fillTrkCandOSeg(fTrkCand);
        if(!fTrkCand) break;
      }
    }
  }
}


void HMdc12Fit::fitAlgorithm3b(void) {
  // Sector fit
  // Magnet off + combined clusters in sector
  HMdcClus* fClst1;
  while((fClst1=(HMdcClus*)iterClus->Next())) {
    if(fClst1->getIndexParent() >= 0) continue;
    fittersArr[0].reset();
    Int_t first,last;
    fClst1->getIndexRegChilds(first,last);
    HMdcClus* fClst2 = (first<0) ? 0:(HMdcClus*)fClusCat->getObject(first);
    Int_t sec=fClst1->getSec();
    Int_t seg=fClst1->getIOSeg();
    Bool_t flag=kFALSE;
    if(fClst2==0) {
      Int_t typeClFn=fClst1->getTypeClFinder();
      Int_t nMdcsSeg=fitpar.fSizesCells->nModInSeg(sec,seg);
      if(nMdcsSeg==1) flag=fitMod(fClst1);              // One mdc per segment
      else if(typeClFn==0) {                         // Two mdc per segment
        if(HMdcTrackDSet::fitType==0) flag=fitSeg(fClst1);
        else if(HMdcTrackDSet::fitType==1) flag=fitCombClusIndFit(fClst1);
      } else if(typeClFn==2) flag=fitMixedClus(fClst1);
      if(!flag) continue;
      fitter=fittersArr[0].getFitter(0); // ???
      if(fitter->segIndex<0) continue;
      fillTrkCandISeg();
    } else {
      flag=fitSec(fClst1,fClst2);  //??????????????
    }
  }
}

void HMdc12Fit::fitAlgorithm4(void) {
  // For cosmic data !!!
  HMdcClus* fClstSeg;
  Bool_t flag=kFALSE;
  while((fClstSeg=(HMdcClus*)iterClus->Next())) {
    Int_t sec=fClstSeg->getSec();
    Int_t seg=fClstSeg->getIOSeg();
    Int_t typeClFnSeg=fClstSeg->getTypeClFinder();
    fittersArr[0].reset();
    Int_t nMdcsSeg=fitpar.fSizesCells->nModInSeg(sec,seg);
    if(nMdcsSeg==1) flag=fitMod(fClstSeg);            // One mdc per segment
    else if(typeClFnSeg==0) {                         // Two mdc per segment
      if(HMdcTrackDSet::fitType==0) flag=fitSeg(fClstSeg);
      else if(HMdcTrackDSet::fitType==1) flag=fitCombClusIndFit(fClstSeg);
    } else if(typeClFnSeg==2) flag=fitMixedClus(fClstSeg);
  }
}

Bool_t HMdc12Fit::fitMod(HMdcClus* fClst, Int_t arrInd) {
  // fit of segment with one mdc
  Int_t modClus=fClst->getMod();
  Int_t iMod;
  if(modClus<0) iMod=(fClst->getNDrTimesMod(0)) ? 0:1; // !!! nado uprostit'
  else iMod=modClus&1;
    
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fitter->setClustAndFill(fClst)) return kFALSE;
  fitter->setRegionOfWires();
  Bool_t isFitted;
  if(fClst->getNLayersMod(iMod)<5) isFitted=kFALSE;
  else isFitted=fitter->fitCluster();
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
    fillHitSegByClus(iMod);
  } else while(isFitted) {
    hitSegFiller.setFitParam(fitter->finalParam); //!
    fillHitSegByFit(iMod);
    isFitted=kFALSE; //???
    //???   nFitedTimes=fitter->getWiresArr().unsetHits();
    //???   isFitted=fitter->fitCluster();
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitSeg(HMdcClus* fClst, Int_t arrInd) {
  // fit of one mdc in segment
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fitter->setClustAndFill(fClst)) return kFALSE;
  fitter->setRegionOfWires();
  Bool_t isFitted=fitter->fitCluster();
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
    fillHitSegByClus();
  } else /*while(isFitted)*/ {
    hitSegFiller.setFitParam(fitter->finalParam); //!
    fillHitSegByFit();
    //???   nFitedTimes=fitter->getWiresArr().unsetHits();
    //???   isFitted=fitter->fitCluster();
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitSec(HMdcClus* fClst1, HMdcClus* fClst2) {
  // fit of all mdcs in segment
  fitter=fittersArr[0].getNewFitter();
  if(!fitter->setClustAndFill(fClst1,fClst2)) return kFALSE;
  fitter->setRegionOfWires();
  HMdcTrkCand* fTrkCand = 0;
  Bool_t isFitted=fitter->fitCluster();
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst1,HMdcTrackDSet::isCoilOff());
    if(fillHitSegByClus(-1) && fTrkCandCat) {
      fTrkCand=fillTrkCandISeg();
      if(fTrkCand==0) return kFALSE;
    }
    if(fillHitSegByClus(-2) && fTrkCand) fTrkCand=fillTrkCandOSeg(fTrkCand);
  } else /*while(isFitted)*/ {
    hitSegFiller.setFitParam(fitter->finalParam); //!
    if(fillHitSegByFit(-1) && fTrkCandCat) {
      fTrkCand=fillTrkCandISeg();
      if(fTrkCand==0) return kFALSE;
    }
    if(fillHitSegByFit(-2) && fTrkCand) fTrkCand=fillTrkCandOSeg(fTrkCand);
    //???   nFitedTimes=fitter->getWiresArr().unsetHits();
    //???   isFitted=fitter->fitCluster();
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fit2Sectors(HMdcClus* fClst1, HMdcClus* fClst2,
                              HMdcClus* fClst3, HMdcClus* fClst4) {
  // For cosmic data only.
  // Fit of all mdcs in 2 sectors.
  fitter=fittersArr[0].getNewFitter();
  if(!fitter->setClustAndFill(fClst1,fClst2,fClst3,fClst4)) return kFALSE;
  fitter->setRegionOfWires();
  Bool_t isFitted=fitter->fitCluster();
  if(!isFitted) {
    //???????????!!!!!!!!!!!!!!!!!!+++
    hitSegFiller.setClusParam(fClst1,HMdcTrackDSet::isCoilOff());
    fillHitSegByClus(-1);
    fillHitSegByClus(-2);
    
    if(fClst3) hitSegFiller.setClusParam(fClst3,HMdcTrackDSet::isCoilOff());
    else       hitSegFiller.setClusParam(fClst4,HMdcTrackDSet::isCoilOff());
    fillHitSegByClus(-3);
    fillHitSegByClus(-4);
  } else /*while(isFitted)*/ {
    hitSegFiller.setFitParam(fitter->finalParam); //!
    fillHitSegByFit(-1);
    fillHitSegByFit(-2);
    if(fitter->getWiresArr().getOutputListCells2().getNDrTimes() > 0) {
      HMdcTrackParam parInSec2(fitter->finalParam);
      Int_t sec2 = fitter->wires.getSector2();
      fitpar.fSizesCells->transLineToAnotherSec(parInSec2,sec2);
      hitSegFiller.setFitParamForSecondSec(parInSec2);
      fillHitSegByFit(-3);
      fillHitSegByFit(-4);
    }
    //???   nFitedTimes=fitter->getWiresArr().unsetHits();
    //???   isFitted=fitter->fitCluster();
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitCombClusIndFit(HMdcClus* fClst, Int_t arrInd) {
  // CombinedClusters + Independent fit
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fitter->setClustAndFill(fClst)) return kFALSE;
  Int_t fstIndex[2];
  Int_t lstIndex[2];
  Int_t seg=fClst->getIOSeg();
  for(Int_t iMod=0; iMod<2; iMod++) {
    Int_t mod=iMod+seg*2;
    fitter->setRegionOfWires(mod);
    fstIndex[iMod]=-1;
    lstIndex[iMod]=-2;
     Bool_t isFitted=(fClst->getNLayersMod(iMod)<5) ? kFALSE:
         fitter->fitCluster(mod);
    if(!isFitted) {
      hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
      fstIndex[iMod]=lstIndex[iMod]=fillHitByClus(mod);
    } else /*while(isFitted)*/ {
      hitSegFiller.setFitParam(fitter->finalParam); //!
      lstIndex[iMod]=fillHitByFit(mod);
      if(fstIndex[iMod]<0) fstIndex[iMod]=lstIndex[iMod];
      //???   nFitedTimes=fitter->getWiresArr().unsetHits();
      //???   isFitted=fitter->fitCluster(mod);
    }
  }
  for(Int_t i1=fstIndex[0]; i1<=lstIndex[0]; i1++) 
    for(Int_t i2=fstIndex[1]; i2<=lstIndex[1]; i2++) fillSegBy2Hits(i1,i2);
  return kTRUE;
}

Bool_t HMdc12Fit::fitChambClus(HMdcClus* fClst, Int_t arrInd) {
  // fit of ChamberClusters:
  Int_t modClus=fClst->getMod();
  if(modClus<0) return kFALSE;
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fitter->setClustAndFill(fClst)) return kFALSE;
  Int_t iMod=modClus&1;
  fitter->setRegionOfWires();
  Bool_t isFitted=fitter->fitCluster();
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
    if(modClus==modForSeg[fitter->wires.getSegment()]) fillHitSegByClus(iMod);
    else fillHitByClus(modClus);
  } else /*while(isFitted)*/ {
    hitSegFiller.setFitParam(fitter->finalParam); //!
    if(modClus==modForSeg[fitter->wires.getSegment()]) fillHitSegByFit(iMod);
    else fillHitByFit(modClus);
    //???   nFitedTimes=fitter->getWiresArr().unsetHits();
    //???   isFitted=fitter->fitCluster();
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitMixedClus(HMdcClus* fClst, Int_t arrInd) {
  // fit of MixedClusters:
  Int_t modClus=fClst->getMod();
  if(modClus<0) return kFALSE;
  Int_t iMod=modClus&1;
  Int_t addMod=fClst->getIOSeg()*2 + (iMod^1);
  if(fClst->getNLayersMod(iMod^1)<HMdcTrackDSet::mixFitCut) return kFALSE;
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fitter->setClustAndFill(fClst)) return kFALSE;
  fitter->setRegionOfWires();
  Bool_t isFitted=fitter->fitCluster();
  if(!isFitted) {
    if(fClst->getNLayersMod(iMod^1) >= HMdcTrackDSet::mixClusCut) {
      hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
      fillHitSegByClus(iMod);
    }
  } else /*while(isFitted)*/ {
    if(fitter->getWiresArr().getOutputListCells().getNLayersMod(addMod) >=
        HMdcTrackDSet::mixFitCut) {
//    if(fitter->listCells.getNLayersMod(addMod) >= HMdcTrackDSet::mixFitCut) {
      hitSegFiller.setFitParam(fitter->finalParam); //!
      fillHitSegByFit(iMod); // ??? esli cut ne proshol mozhet zaolnit' clust?
    }
    //???   nFitedTimes=fitter->getWiresArr().unsetHits();
    //???   isFitted=fitter->fitCluster();
  }
  return kTRUE;
}

void HMdc12Fit::fillClusInf(HMdcHit* fHit, Int_t iMod, Char_t fitSt,
    Int_t indClusFit) {
  fHit->setTrackFinder(1);
  if(fitSt==0) fHit->setChi2(-1.);
  if(fClusInfCat==0) return; // Cosmic data
  Int_t index;
  HMdcClusInf* fClusInf=(HMdcClusInf*)fClusInfCat->getNewSlot(locClInf,&index);
  if(!fClusInf) {
    Warning("fillClusInf","No slot HMdcClusInf available");
    return;
  }
  fClusInf = (isGeant) ? (HMdcClusInf*)(new(fClusInf) HMdcClusInfSim) :
                                        new(fClusInf) HMdcClusInf;
  fHit->setClusInfIndex(index);
  fClusInf->setFitStat(fitSt);
  Int_t mod = fHit->getModule();
  HMdcClus* fClst=(fitter->wires.getClust2() && mod>1) ? 
      fitter->wires.getClust2() : fitter->wires.getClust1();
  
  Int_t nmods=fClst->getMod();
  fClusInf->setNModInCl((nmods<0) ? -nmods:1); //?????????peredelat!
//   nmods=0;
//   for(Int_t m=0;m<4;m++) if(fitter->nTimesInFit[m]>0) nmods++;
  fClusInf->setNModInFit(fitter->getWiresArr().getNModulesInFit());
  fClusInf->setClusIndex(fClusCat->getIndex(fClst));
  fClusInf->setClusFitIndex(fitter->indClusFit);
  
  fClusInf->setXYHit(fHit->getX(),fHit->getY());
  
  HMdcSizesCellsMod* fSCMod = fitpar.fSCModAr[fitter->wires.getSector()][mod];
  Double_t xm,ym,zm;
  fSCMod->calcIntersection(
      fClst->getXTarg(),fClst->getYTarg(),fClst->getZTarg(),
      fClst->getX(),    fClst->getY(),    fClst->getZ(),    xm,ym,zm);
  fSCMod->transTo(xm,ym,zm);
  fClusInf->setXYClus(xm,ym);
  
  if(iMod==0) {
    fClusInf->setLevelClFinding(fClst->getClFnLevelM1());
    fClusInf->setClusSize(fClst->getClusSizeM1());
    fClusInf->setNMergClust(fClst->getNMergClustM1());
    fClusInf->setNDrTimes(fClst->getNDrTimesM1());
    fClusInf->setSigma1(fClst->getSigma1M1());
    fClusInf->setSigma2(fClst->getSigma2M1());
    fClusInf->setAlpha(fClst->getAlphaM1());
    fClusInf->setNLayers(fClst->getNLayersMod(0));
    if(!fitSt) {
      fClusInf->setNLayersFitted(fClst->getNLayersMod(0));
      fClusInf->setNDrTimesFitted(fClst->getNDrTimesM1());
    }
  } else {
    fClusInf->setLevelClFinding(fClst->getClFnLevelM2());
    fClusInf->setClusSize(fClst->getClusSizeM2());
    fClusInf->setNMergClust(fClst->getNMergClustM2());
    fClusInf->setNDrTimes(fClst->getNDrTimesM2());
    fClusInf->setSigma1(fClst->getSigma1M2());
    fClusInf->setSigma2(fClst->getSigma2M2());
    fClusInf->setAlpha(fClst->getAlphaM2());
    fClusInf->setNLayers(fClst->getNLayersMod(1));
    if(!fitSt) {
      fClusInf->setNLayersFitted(fClst->getNLayersMod(1));
      fClusInf->setNDrTimesFitted(fClst->getNDrTimesM2());
    }
  }
  if(fitSt) {
    fClusInf->setNLayersFitted(
      fitter->getWiresArr().getOutputListCells().getNLayersMod(mod));
    fClusInf->setNDrTimesFitted(
      fitter->getWiresArr().getOutputListCells().getNDrTimesMod(mod));
  }
  if(isGeant) {
    HMdcClusSim* fClSim=(HMdcClusSim*)fClst;
    ((HMdcClusInfSim*)fClusInf)->setTracksList(fClSim->getNTracksMod(iMod),
        fClSim->getListTrM(iMod),fClSim->getNTimesM(iMod));
  }
}

void HMdc12Fit::fillClusInf(HMdcSeg* fSeg,Bool_t secondSec) {
  HMdcClus* fClst1=fitter->wires.getClust1();
  HMdcClus* fClst2=fitter->wires.getClust2();
  if(secondSec) {
    fClst1=fitter->wires.getClust3();
    fClst2=fitter->wires.getClust4();
  }
  Int_t nMrCl = fClst1->getNMergClust();
  if(nMrCl>99) nMrCl=99;
  HMdcClus* fClst=(fClst2 && fSeg->getIOSeg()==1) ? fClst2 : fClst1;
  Int_t flag=(fClst->getNBins()&32767) <<16;
  flag += (fClst->getNDrTimes()*100 + nMrCl) & 65535;
  fSeg->setFlag(flag);
}

Bool_t HMdc12Fit::fillSegBy2Hits(Int_t ind1, Int_t ind2) {
  Int_t segment = fitter->wires.getSegment();
  Int_t mod1    = fitter->wires.getSegment()*2;
  Int_t mod2    = mod1+1;
  Int_t sector  = fitter->wires.getSector();
  HMdcSizesCellsMod* fSCMod1=fitpar.fSCModAr[sector][mod1];
  HMdcSizesCellsMod* fSCMod2=fitpar.fSCModAr[sector][mod2];
  if(!fSCMod1 || !fSCMod2) return kFALSE;
  HMdcHit* h1=(HMdcHit*)fHitCat->getObject(ind1);
  HMdcHit* h2=(HMdcHit*)fHitCat->getObject(ind2);
  if(h1==0 || h2==0) return kFALSE;
  Float_t x1,y1,z1,ex1,ey1;
  Float_t x2,y2,z2,ex2,ey2;
  h1->getXY(x1,ex1,y1,ey1);
  h2->getXY(x2,ex2,y2,ey2);
  fSCMod1->transFromZ0(x1,y1,z1);
  fSCMod2->transFromZ0(x2,y2,z2);
  ey1 /= sqrt(1.+fSCMod1->B()*fSCMod1->B()); // simple error recalculation
  ey2 /= sqrt(1.+fSCMod2->B()*fSCMod2->B()); // (let eX_sec=eX_mod,...)
  HMdcSeg* fSeg = getSegSlot(sector,segment);
  if(!fSeg) return kFALSE;
  for(Int_t l=0; l<6; l++) {
    copySignId(h1,0,l,fSeg);
    copySignId(h2,1,l,fSeg);
  }
  Double_t zm, r0, theta, phi;
  Double_t eZm, eR0, eTheta, ePhi;
  HMdcSizesCells::calcMdcSeg(x1,y1,z1,x2,y2,z2,ex1,ey1,0.,ex2,ey2,
      -fSCMod1->A(),-fSCMod1->B(),-fSCMod2->A(),-fSCMod2->B(),
      zm,eZm, r0,eR0,theta,eTheta, phi,ePhi);
  fSeg->setPar(zm, eZm, r0, eR0, theta, eTheta, phi, ePhi);
  
  HMdcSizesCellsSec& fSCSec=(*(fitpar.fSizesCells))[sector];
  Double_t zPrime=zm; 
  Double_t rPrime=r0;
  if(&fSCSec) fSCSec.calcRZToTargLine(x1,y1,z1,x2,y2,z2,zPrime,rPrime);
  fSeg->setZprime(zPrime);
  fSeg->setRprime(rPrime);
  
  fSeg->setHitInd(0,ind1);
  fSeg->setHitInd(1,ind2);
  fSeg->setChi2(-2.);
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
  
  if(h1->getChi2()>=0 || h2->getChi2()>=0) fitter->fitStatus=kTRUE; // ??? podumat'!!!

  // Cluster param:
  //fSeg->setFlag(fitter->fClst1->getNDrTimes()*100 + fitter->fClst1->getNMergClust());
  //fSeg->setChi2(fitter->fClst1->getNBins());  // NBins in clust. --> chi2 in Seg.
  return kTRUE;
}

void HMdc12Fit::copySignId(HMdcHit* hit,Int_t mod,Int_t lay,HMdcSeg* seg) {
  // do Signals Id copy from Hit to Seg
  Int_t nHits=hit->getNCells(lay);
  if(nHits<=0||nHits>4) return;
  Int_t id[4]={0,0,0,0};
  Int_t cell=hit->getCell(lay,0);
  for(Int_t i=0; i<nHits; i++) {
    Int_t n=hit->getCell(lay,i) - cell;
    if(n>=0&&n<4) id[n]=hit->getSignId(lay,i);
  }
  seg->setSignId(lay+mod*6,cell,id[0],id[1],id[2],id[3]);
}

Int_t HMdc12Fit::fillHitByFit(Int_t mod) {
  HMdcList24GroupCells* lCells = &(fitter->getWiresArr().getOutputListCells());
  Int_t sector = fitter->wires.getSector();
  if(mod>=4) {  // Cosmic
    lCells = &(fitter->getWiresArr().getOutputListCells2());
    sector = fitter->wires.getSector2();
    if(sector<0) return -1;
    mod -= 4; 
  }
  if(lCells->getNDrTimesMod(mod) == 0) return -1;
  HMdcSizesCellsMod* fSCMod=fitpar.fSCModAr[sector][mod];
  if(!fSCMod) return -1;
  Int_t index=-1;
  HMdcHit* fHit = getHitSlot(sector,mod,index);
  if(!fHit) return -1;
  hitSegFiller.fillMdcHit(fSCMod,fHit);
  setGroup(lCells, fHit, mod);
  fHit->setChi2(fitter->getChi2()); // getFunMin())/NDF
  fillClusInf(fHit,mod&1,1,fitter->indClusFit); // Cluster inform.
  return index;
}

Bool_t HMdc12Fit::fillHitSegByFit(Int_t iMod) {
  // iMod==0   - first MDC in segment
  // iMod==1   - second MDC in segment
  // iMod==-99 - two MDC in segment
  // iMod==-1  - first segment of sector track
  // iMod==-2  - second segment of sector track
  // iMod==-3  - first segment of second sector in track (cosmic)
  // iMod==-4  - second segment of second sector in track (cosmic)
  Int_t segInd = fitter->wires.getSegment();
  if(iMod<=-1 && iMod>=-4) segInd = -iMod-1;  // sector fit !!!???
  Int_t segment = segInd&1;
  HMdcList24GroupCells* lCells = &(fitter->getWiresArr().getOutputListCells());
  Int_t sector = fitter->wires.getSector();
  if(segInd>1) {
    lCells = &(fitter->getWiresArr().getOutputListCells2());
    sector = fitter->wires.getSector2();
    if(sector<0) return kFALSE;
  }
  if(lCells->getNDrTimes(segment*12,segment*12+11) == 0) return kFALSE;
  
  HMdcSeg* fSeg = getSegSlot(sector,segment);
  if(!fSeg) return kFALSE;
  Int_t indx1=-1;
  if(iMod!=1) indx1=fillHitByFit(segInd*2);
  Int_t indx2=-1;
  if(iMod!=0) indx2=fillHitByFit(segInd*2+1);
  if(indx1>=0) fSeg->setHitInd(0,indx1);
  if(indx2>=0) fSeg->setHitInd(1,indx2);
  setGroup(lCells, fSeg, segment);
  HMdcSizesCellsSec& fSCSec=(*(fitpar.fSizesCells))[sector];
  const HGeomVector* targ=0;
  if(&fSCSec) targ=&(fSCSec.getTargetMiddlePoint());
 
  hitSegFiller.fillMdcSeg(fSeg,targ);
  fSeg->setChi2(fitter->getChi2()); // getFunMin());
  // Cluster param:
  fillClusInf(fSeg,segInd>1);
  return kTRUE;
}

Int_t HMdc12Fit::fillHitByClus(Int_t mod) {
  HMdcList24GroupCells* lCells = &(fitter->getWiresArr().getInputListCells());
  Int_t sector = fitter->wires.getSector();
  if(mod>=4) {  // Cosmic
    lCells = &(fitter->getWiresArr().getInputListCells2());
    sector = fitter->wires.getSector2();
    if(sector<0) return -1;
    mod -= 4;
  }
  if(lCells->getNDrTimesMod(mod) == 0) return -1;
  HMdcSizesCellsMod* fSCMod = fitpar.fSCModAr[sector][mod];
  if(!fSCMod) return -1;
  Int_t index=-1;
  HMdcHit* fHit = getHitSlot(sector,mod,index);
  if(!fHit) return -1;
  hitSegFiller.fillMdcHitByClus(fSCMod,fHit);
  setGroup(lCells, fHit,mod);
  fillClusInf(fHit,mod&1,0); // Cluster inform.
  return index;
}

Bool_t HMdc12Fit::fillHitSegByClus(Int_t iMod) {
  // iMod==0   - first MDC in segment
  // iMod==1   - second MDC in segment
  // iMod==-99 - two MDC in segment
  // iMod==-1  - first segment of sector track
  // iMod==-2  - second segment of sector track
  // iMod==-3  - first segment of second sector in track (cosmic)
  // iMod==-4  - second segment of second sector in track (cosmic)
  Int_t segInd = fitter->wires.getSegment();
  if(iMod<=-1 && iMod>=-4) segInd = -iMod-1;  // sector fit !!!???
  Int_t segment = segInd&1;
  HMdcList24GroupCells* lCells = &(fitter->getWiresArr().getInputListCells());
  Int_t sector = fitter->wires.getSector();
  if(segInd>1) {
    lCells = &(fitter->getWiresArr().getInputListCells2());
    sector = fitter->wires.getSector2();
    if(sector<0) return kFALSE;
  }
  if(lCells->getNDrTimes(segment*12,segment*12+11) == 0) return kFALSE;
  
  HMdcSeg* fSeg = getSegSlot(sector,segment);
  if(!fSeg) return kFALSE;
  Int_t indx1=-1;
  if(iMod!=1) indx1 = fillHitByClus(segInd*2);
  Int_t indx2=-1;
  if(iMod!=0) indx2 = fillHitByClus(segInd*2+1);
  if(indx1>=0) fSeg->setHitInd(0,indx1);
  if(indx2>=0) fSeg->setHitInd(1,indx2);
  setGroup(lCells, fSeg, segment);
  HMdcSizesCellsSec& fSCSec=(*(fitpar.fSizesCells))[sector];
  const HGeomVector* targ=(&fSCSec) ? &(fSCSec.getTargetMiddlePoint()) : 0;
  hitSegFiller.fillMdcSegByClus(fSeg,targ);
  fSeg->setChi2(-1.);
  // Cluster param:
  fillClusInf(fSeg,segInd>1);
  return kTRUE;
}

HMdcHit* HMdc12Fit::getHitSlot(Int_t sec, Int_t mod, Int_t& index) {
  // Geting a slot and setting address
  locHit[0] = sec;;
  locHit[1] = mod;
  HMdcHit* fHit = (HMdcHit*)fHitCat->getNewSlot(locHit,&index);
  if(!fHit) {
    Warning("getHitSlot"," No slot in catMdcHit available");
    index=-1;
    return 0;
  }
  if(isGeant) fHit=(HMdcHit*) new(fHit) HMdcHitSim;
  else fHit= new(fHit) HMdcHit;
  fHit->setSecMod(sec,mod);
  return fHit;
}

HMdcSeg* HMdc12Fit::getSegSlot(Int_t sec,Int_t segment) {
  // Geting a slot and setting address
  if(sec<0) return 0;
  locSeg[0] = sec; 
  locSeg[1] = segment;
  Int_t index;
  HMdcSeg* fSeg = (HMdcSeg*)fSegCat->getNewSlot(locSeg,&index);
  if(!fSeg) {
    Warning("getSegSlot"," No slot in catMdcSeg available");
    fitter->segIndex=-1;
    return 0;
  }
  fitter->segIndex = index;      // For cosmic data it is not used
  fSeg = (isGeant) ? (HMdcSeg*)(new(fSeg) HMdcSegSim) : new(fSeg) HMdcSeg;
  fSeg->setSec(sec);
  fSeg->setIOSeg(segment);
  return fSeg;
}

HMdcTrkCand* HMdc12Fit::fillTrkCandISeg(void) {
  if(fTrkCandCat==0) return 0;
  Int_t index;
  HMdcTrkCand* fTrkCandNew = 
      (HMdcTrkCand*)fTrkCandCat->getNewSlot(locTrkCand,&index);
  if(!fTrkCandNew) {
    Warning("fillTrkCandISeg"," No slot available in catMdcTrkCand");
    fitter->segIndex=-1;
    return 0;
  }
  return new(fTrkCandNew) HMdcTrkCand(fitter->wires.getSector(),
      fitter->segIndex,index);
}

HMdcTrkCand* HMdc12Fit::fillTrkCandOSeg(HMdcTrkCand* fTrkCand) {
  if(fTrkCandCat==0 || fTrkCand==0) return 0;
  Int_t index;
  HMdcTrkCand* fTrkCandNew =
      (HMdcTrkCand*)fTrkCandCat->getNewSlot(locTrkCand,&index);
  if(!fTrkCandNew) {
    Warning("fillTrkCandOSeg"," No slot available in catMdcTrkCand");
    fitter->segIndex=-1;
    return 0;
  }
  return new(fTrkCandNew) HMdcTrkCand(fTrkCand,fitter->segIndex,index);
}

Double_t HMdc12Fit::calcVertexDist( Double_t x0, Double_t y0, Double_t z0,
			      Double_t x1, Double_t y1, Double_t z1,
			      Double_t x2, Double_t y2, Double_t z2) {
  // Calculeting a min distance betveen point x0, y0, z0 and line
  return sqrt( ((y2-y1)*(z1-z0) - (z2-z1)*(y1-y0))*((y2-y1)*(z1-z0) - (z2-z1)*(y1-y0)) +
	       ((z2-z1)*(x1-x0) - (x2-x1)*(z1-z0))*((z2-z1)*(x1-x0) - (x2-x1)*(z1-z0)) +
               ((x2-x1)*(y1-y0) - (y2-y1)*(x1-x0))*((x2-x1)*(y1-y0) - (y2-y1)*(x1-x0)) )/
    sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

void HMdc12Fit::setGroup(HMdcList24GroupCells* lCl, HMdcSeg* fSeg, Int_t seg) {
  // Filling list of cells in HMdcSeg
  Int_t lay1=seg*12; //fitter->wires.getSegment()*12;
  Int_t lay2=lay1+12;
  Int_t t[4];
  for(Int_t layer=lay1; layer<lay2; layer++) {
    Int_t cell=lCl->get4FirstCells(layer,t);
    if(cell>=0) fSeg->setSignId(layer-lay1,cell,t[0],t[1],t[2],t[3]);
  }
  if(pTrackInfSim) pTrackInfSim->fillSegTrackInf((HMdcSegSim*)fSeg,lCl);
}

void HMdc12Fit::setGroup(HMdcList24GroupCells* lCl, HMdcHit* fHit, Int_t mod) {
  // Filling list of cells in HMdcHit
  Int_t lay1=mod*6;
  Int_t lay2=lay1+6;
  Int_t t[4];
  for(Int_t layer=lay1; layer<lay2; layer++) {
    Int_t cell=lCl->get4FirstCells(layer,t);
    if(cell>=0) fHit->setSignId(layer-lay1,cell,t[0],t[1],t[2],t[3]);
  }
  if(pTrackInfSim) pTrackInfSim->fillHitTrackInf((HMdcHitSim*)fHit,lCl);
}

void HMdc12Fit::printStatus(void) {
  // prints the parameters to the screen
  Char_t* hAuth ="1 = Alexander, 2 = tuned ver. 1";
  Char_t* hTmVer="0 = polynomial,  1 = HMdcCal2ParSim::calcTime";
  Char_t* hFType="0 = segment fit,  1 = independent mdc fit";
  Char_t* hDCat ="1 = create and fill catMdcClusFit&catMdcWireFit 0 = don't cr.";
  Char_t* hDPr  ="1 = print debug information, 0 = don't print";
  Char_t* hUWOff="1 = remove signal time on wire from drift time, 0 = don't remove";
  printf("--------------------------------------------------------------------------------------------\n");
  printf("HMdc12FitSetup:\n");
  printf("FitVersion      =  %i :  %s\n",HMdcTrackDSet::fitAuthor,hAuth);
  printf("DrTimeCalcVers  =  %i :  %s\n",HMdcTrackDSet::distTimeVer,hTmVer);
  printf("FitType         =  %i :  %s\n",HMdcTrackDSet::fitType,hFType);
  printf("useWireOffset   =  %i :  %s\n",HMdcTrackDSet::useWireOffset,hUWOff);
  printf("DebugCategorie  =  %i :  %s\n",HMdcTrackDSet::fntuple,hDCat);
  printf("DebugPrint      =  %i :  %s\n",HMdcTrackDSet::fprint,hDPr);
  printf ("--------------------------------------------------------------------------------------------\n");
  prntSt=kTRUE;
}

Bool_t HMdc12Fit::finalize(void) {
  return kTRUE;
}

