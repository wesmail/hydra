using namespace std;
#include "hmdc12fit.h"
#include "hdebug.h"
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
#include "hmdctrackfitterh.h"
#include "hmdctrackfittera.h"
#include "hmdctrackdset.h"
#include "hmdcclusinfsim.h"
#include "hruntimedb.h"
#include "hmdc34clfinder.h"
#include "hmdctrkcand.h"

//*-- Author : V.Pechenov
//*-- Modified: 29.10.03 V.Pechenov
//*-- Modified: 21.07.03 V.Pechenov
//*-- Modified: 04.04.03 V.Pechenov
//*-- Modified: 17.07.02 V.Pechenov
//*-- Modified: 16.08.01 V.Pechenov
//*-- Modified: 17.10.00 A.Nekhaev

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
    if(fitAuthor != 1) fitter=(HMdcTrackFitter*) new HMdcTrackFitterH(fitpar);
    else fitter=(HMdcTrackFitter*) new HMdcTrackFitterA(fitpar);
    fitterArr[nObj]=fitter;
  }
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
  prntSt=kFALSE;
  HMdcTrackDSet::set12Fit(this);
  fSegCat=0;
  iterClus=0;
  modForSeg[0]=1;
  modForSeg[1]=3;
  fitpar.cutWeight=0.01;
  fitpar.signalSpeed=0.0;
  notFillByClus=kFALSE;

  threshold=300.;           // fit threshold
}

Bool_t HMdc12Fit::init(void) {
  fClusCat=HMdcGetContainers::getCatMdcClus();
  if( !fClusCat ) return kFALSE;
  if(!fitpar.init()) return kFALSE;
  isGeant=fitpar.isGeant;
  HMdcDetector* fMdcDet=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
  modForSeg[0]=HMdcTrackDSet::modForSeg1;
  modForSeg[1]=HMdcTrackDSet::modForSeg2;
  if(modForSeg[0]>=0 && modForSeg[1]>0) {
    fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if(!fSegCat) {
      if(isGeant) fSegCat = fMdcDet->buildMatrixCategory("HMdcSegSim",0.5F);
      else fSegCat = fMdcDet->buildCategory(catMdcSeg);
      if (!fSegCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
    }
  }

  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    if(isGeant) fHitCat = fMdcDet->buildMatrixCategory("HMdcHitSim",0.5F);
    else fHitCat = fMdcDet->buildCategory(catMdcHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }

  fTrkCandCat = HMdcGetContainers::getCatMdcTrkCand(kTRUE);
  if(!fTrkCandCat) return kFALSE;

  iterClus=(HIterator *)fClusCat->MakeIterator();

  fClusInfCat = HMdcGetContainers::getCatMdcClusInf(kTRUE);
  if(!fClusInfCat) return kFALSE;
  locClInf.set(1,0);
  locSeg.set(2,0,0);
  locHit.set(2,0,0);
  locTrkCand.set(1,0);
  locClus.set(2,0,0);

  fittersArr[0].init(HMdcTrackDSet::fitAuthor,&fitpar);
  fittersArr[1].init(HMdcTrackDSet::fitAuthor,&fitpar);
  if(!prntSt) printStatus();
  f34ClFinder=HMdc34ClFinder::getExObject();
  return kTRUE;
}

Bool_t HMdc12Fit::reinit(void) {
  if(!fitpar.reinit()) return kFALSE;
  clusFitAlg=1;
  if(HMdcTrackDSet::isCoilOff()) {
    if(HMdcTrackDSet::getClFinderType()==1) clusFitAlg=2;
    else clusFitAlg=3;
  }
  return kTRUE;
}

Int_t HMdc12Fit::execute(void) {
//fitter->setPrintFlag(HMdcTrackDSet::fprint);
  iterClus->Reset();
  if(clusFitAlg==1) fitAlgorithm1();
  else if(clusFitAlg==2) fitAlgorithm2();
  else if(clusFitAlg==3) fitAlgorithm3();
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
  HMdc34ClFinderSec& f34CFSc=(*f34ClFinder)[fitter->sector];
  if(!&f34CFSc || f34CFSc.notEnoughWires()) return 0;
  if(!fitter->fitStatus || flag) return f34CFSc.findClustersSeg2(fitter->fClst);
  HMdcSeg* fSeg=(HMdcSeg*)fSegCat->getObject(fitter->segIndex);
  if(fSeg==0) return 0;
  return f34CFSc.findClustersSeg2(fSeg,fitter->fClst);
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
  // Magnet off + combined clusters
// ??? peredelat' v fit vsego treka !!!
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

Bool_t HMdc12Fit::fillArrays(HMdcClus * fClst, HMdcList12GroupCells& lCells) {
  if(HMdcTrackDSet::fprint) fClst->print();
  fitter->fClst=fClst;
  fitter->setAddress(fClst->getSec(),fClst->getIOSeg());
  if(!fitter->fillListHits(lCells)) return kFALSE;
  fitter->setTrackPar(fClst->getXTarg(),fClst->getYTarg(),fClst->getZTarg(),
                      fClst->getX(),fClst->getY(),fClst->getZ());
  return kTRUE;
}

Bool_t HMdc12Fit::fitMod(HMdcClus* fClst, Int_t arrInd) {
  // fit of segment with one mdc
  Int_t modClus=fClst->getMod();
  Int_t iMod;
  if(modClus<0) iMod=(fClst->getNDrTimesMod(0)) ? 0:1; // !!! nado uprostit'
  else iMod=modClus&1;
    
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fillArrays(fClst,(HMdcList12GroupCells&)(*fClst))) return kFALSE;
  fitter->setFittingTimesList();
  Bool_t isFitted=(fClst->getNLayersMod(iMod)<5) ? kFALSE:
      fitter->fitCluster(threshold);  // (threshold ???)
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
    fillHitSegByClus(iMod);
  } else while(isFitted) {
    hitSegFiller.setFitParam(fitter->firstPl,fitter->secondPl,fitter->parameter,
        &(fitter->errMatr),fitter->errTimeOffset);
    fillHitSegByFit(iMod);
    isFitted=kFALSE; //???
    //???   nFitedTimes=fitter->unsetHits();
    //???   isFitted=fitter->fitCluster(threshold);
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitSeg(HMdcClus* fClst, Int_t arrInd) {
  // fit of one mdc in segment
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fillArrays(fClst,(HMdcList12GroupCells&)(*fClst))) return kFALSE;
  fitter->setFittingTimesList();
  Bool_t isFitted=fitter->fitCluster(threshold);  // (threshold ???)
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
    fillHitSegByClus();
  } else /*while(isFitted)*/ {
    hitSegFiller.setFitParam(fitter->firstPl,fitter->secondPl,fitter->parameter,
        &(fitter->errMatr),fitter->errTimeOffset);
    fillHitSegByFit();
    //???   nFitedTimes=fitter->unsetHits();
    //???   isFitted=fitter->fitCluster(threshold);
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitCombClusIndFit(HMdcClus* fClst, Int_t arrInd) {
  // CombinedClusters + Independent fit
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fillArrays(fClst,(HMdcList12GroupCells&)(*fClst))) return kFALSE;
  Int_t fstIndex[2];
  Int_t lstIndex[2];
  for(Int_t iMod=0; iMod<2; iMod++) {
    fitter->setFittingTimesList(iMod);
    fstIndex[iMod]=-1;
    lstIndex[iMod]=-2;
    Bool_t isFitted=fitter->fitCluster(threshold,iMod);  // (threshold ???)
    if(!isFitted) {
      hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
      fstIndex[iMod]=lstIndex[iMod]=fillHitByClus(iMod);
    } else /*while(isFitted)*/ {
      hitSegFiller.setFitParam(fitter->firstPl,fitter->secondPl,
          fitter->parameter,&(fitter->errMatr),fitter->errTimeOffset);
      lstIndex[iMod]=fillHitByFit(iMod);
      if(fstIndex[iMod]<0) fstIndex[iMod]=lstIndex[iMod];
      //???   nFitedTimes=fitter->unsetHits();
      //???   isFitted=fitter->fitCluster(threshold,iMod);
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
  if(!fillArrays(fClst,(HMdcList12GroupCells&)(*fClst))) return kFALSE;
  Int_t iMod=modClus&1;
  fitter->setFittingTimesList();
  Bool_t isFitted=fitter->fitCluster(threshold);  // (threshold ???)
  if(!isFitted) {
    hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
    if(modClus==modForSeg[fitter->segment]) fillHitSegByClus(iMod);
    else fillHitByClus(iMod);
  } else /*while(isFitted)*/ {
    hitSegFiller.setFitParam(fitter->firstPl,fitter->secondPl,fitter->parameter,
        &(fitter->errMatr),fitter->errTimeOffset);
    if(modClus==modForSeg[fitter->segment]) fillHitSegByFit(iMod);
    else fillHitByFit(iMod);
    //???   nFitedTimes=fitter->unsetHits();
    //???   isFitted=fitter->fitCluster(threshold);
  }
  return kTRUE;
}

Bool_t HMdc12Fit::fitMixedClus(HMdcClus* fClst, Int_t arrInd) {
  // fit of MixedClusters:
  Int_t modClus=fClst->getMod();
  if(modClus<0) return kFALSE;
  Int_t iMod=modClus&1;
  if(fClst->getNLayersMod(iMod^1)<HMdcTrackDSet::mixFitCut) return kFALSE;
  fitter=fittersArr[arrInd].getNewFitter();
  if(!fillArrays(fClst,(HMdcList12GroupCells&)(*fClst))) return kFALSE;
  fitter->setFittingTimesList();
  Bool_t isFitted=fitter->fitCluster(threshold);  // (threshold ???)
  if(!isFitted) {
    if(fClst->getNLayersMod(iMod^1) >= HMdcTrackDSet::mixClusCut) {
      hitSegFiller.setClusParam(fClst,HMdcTrackDSet::isCoilOff());
      fillHitSegByClus(iMod);
    }
  } else /*while(isFitted)*/ {
    if(fitter->listCells.getNLayersMod(iMod^1) >= HMdcTrackDSet::mixFitCut) {
      hitSegFiller.setFitParam(fitter->firstPl,fitter->secondPl,
          fitter->parameter,&(fitter->errMatr),fitter->errTimeOffset);
      fillHitSegByFit(iMod); // ??? esli cut ne proshol mozhet zaolnit' clust?
    }
    //???   nFitedTimes=fitter->unsetHits();
    //???   isFitted=fitter->fitCluster(threshold);
  }
  return kTRUE;
}

void HMdc12Fit::fillClusInf(HMdcHit* fHit, Int_t iMod, Char_t fitSt,
    Int_t indClusFit) {
  Int_t index;
  HMdcClusInf* fClusInf=(HMdcClusInf*)fClusInfCat->getNewSlot(locClInf,&index);
  if(!fClusInf) {
    Error("fillClusInf","No slot HMdcClusInf available");
    return;
  }
  fClusInf = (isGeant) ? (HMdcClusInf*)(new(fClusInf) HMdcClusInfSim) :
                                        new(fClusInf) HMdcClusInf;
  fHit->setTrackFinder(1);
  fHit->setClusInfIndex(index);
  if(fitSt==0) fHit->setChi2(-1.);
  fClusInf->setFitStat(fitSt);
  fClusInf->setLevelClFinding(fitter->fClst->getMinCl(iMod));
  Int_t nmods=fitter->fClst->getMod();
  nmods= (nmods<0) ? -nmods:1;
  fClusInf->setNModInCl(nmods);
  nmods=(fitter->nMDC1Times && fitter->nMDC2Times) ? 2:1;
  fClusInf->setNModInFit(nmods);
  fClusInf->setClusIndex(fClusCat->getIndex(fitter->fClst));
  fClusInf->setClusFitIndex(fitter->indClusFit);
  if(iMod==0) {
    fClusInf->setClusSize(fitter->fClst->getClusSizeM1());
    fClusInf->setNMergClust(fitter->fClst->getNMergClustM1());
    fClusInf->setNDrTimes(fitter->fClst->getNDrTimesM1());
    fClusInf->setSigma1(fitter->fClst->getSigma1M1());
    fClusInf->setSigma2(fitter->fClst->getSigma2M1());
    fClusInf->setAlpha(fitter->fClst->getAlphaM1());
  } else {
    fClusInf->setClusSize(fitter->fClst->getClusSizeM2());
    fClusInf->setNMergClust(fitter->fClst->getNMergClustM2());
    fClusInf->setNDrTimes(fitter->fClst->getNDrTimesM2());
    fClusInf->setSigma1(fitter->fClst->getSigma1M2());
    fClusInf->setSigma2(fitter->fClst->getSigma2M2());
    fClusInf->setAlpha(fitter->fClst->getAlphaM2());
  }
  if(isGeant) {
    HMdcClusSim* fClSim=(HMdcClusSim*)fitter->fClst;
    ((HMdcClusInfSim*)fClusInf)->setTracksList(fClSim->getNTracksMod(iMod),
        fClSim->getListTrM(iMod),fClSim->getNTimesM(iMod));
  }
}

void HMdc12Fit::fillClusInf(HMdcSeg* fSeg, Bool_t fitSt) {
  Int_t nMrCl = fitter->fClst->getNMergClust();
    if(nMrCl>99) nMrCl=99;
    Int_t flag=(fitter->fClst->getNBins()&32767) <<16;
    flag += (fitter->fClst->getNDrTimes()*100 + nMrCl) & 65535;
    fSeg->setFlag(flag);
    if(!fitSt) fSeg->setChi2(-1.);
}

Bool_t HMdc12Fit::fillSegBy2Hits(Int_t ind1, Int_t ind2) {
  Int_t mod1=fitter->segment*2;
  Int_t mod2=mod1+1;
  HMdcSizesCellsMod* fSCMod1=fitpar.fSCModAr[fitter->sector][mod1];
  HMdcSizesCellsMod* fSCMod2=fitpar.fSCModAr[fitter->sector][mod2];
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
  HMdcSeg* fSeg = getSegSlot();
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
  
  HMdcSizesCellsSec& fSCSec=(*(fitpar.fSizesCells))[fitter->sector];
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
  //fSeg->setFlag(fitter->fClst->getNDrTimes()*100 + fitter->fClst->getNMergClust());
  //fSeg->setChi2(fitter->fClst->getNBins());  // NBins in clust. --> chi2 in Seg.
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

Int_t HMdc12Fit::fillHitByFit(Int_t iMod) {
  Int_t mod=iMod+fitter->segment*2;
  Int_t modClus=fitter->fClst->getMod();
  if(modClus>=0 && modClus!=mod) return -1;
  HMdcSizesCellsMod* fSCMod=fitpar.fSCModAr[fitter->sector][mod];
  if(!fSCMod) return -1;
  Int_t index=-1;
  HMdcHit* fHit = getHitSlot(mod,index);
  if(!fHit) return -1;
  hitSegFiller.fillMdcHit(fSCMod,fHit);
  setGroup(&(fitter->listCells), fHit,iMod);
  fHit->setChi2(fitter->chi2perDF); // getFunMin());  /deg.of_fr.num.
  if(isGeant) ((HMdcHitSim*)fHit)->calcNTracks();
  fillClusInf(fHit,iMod,1,fitter->indClusFit); // Cluster inform.
  return index;
}

Bool_t HMdc12Fit::fillHitSegByFit(Int_t iMod) {
  // iMod==0 - first MDC in segment
  // iMod==1 - second MDC in segment
  // iMod==-1 - two MDC in segment
  HMdcSeg* fSeg = getSegSlot();
  if(!fSeg) return kFALSE;
  Int_t indx1=(iMod!=1) ? fillHitByFit(0):-1;
  Int_t indx2=(iMod!=0) ? fillHitByFit(1):-1;
  if(indx1>=0) fSeg->setHitInd(0,indx1);
  if(indx2>=0) fSeg->setHitInd(1,indx2);
  setGroup(&(fitter->listCells), fSeg);
  HMdcSizesCellsSec& fSCSec=(*(fitpar.fSizesCells))[fitter->sector];
  const HGeomVector* targ=(&fSCSec) ? &(fSCSec.getTargetMiddlePoint()) : 0;
  hitSegFiller.fillMdcSeg(fSeg,targ);
  fSeg->setChi2(fitter->chi2perDF); // getFunMin());
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
  // Cluster param:
  fillClusInf(fSeg,kTRUE);
  return kTRUE;
}

Int_t HMdc12Fit::fillHitByClus(Int_t iMod) {
  Int_t mod=iMod+fitter->segment*2;
  Int_t modClus=fitter->fClst->getMod();
  if(modClus>=0 && modClus!=mod) return -1;
  HMdcSizesCellsMod* fSCMod=fitpar.fSCModAr[fitter->sector][mod];
  if(!fSCMod) return -1;
  Int_t index=-1;
  HMdcHit* fHit = getHitSlot(mod,index);
  if(!fHit) return -1;
  hitSegFiller.fillMdcHitByClus(fSCMod,fHit);
  setGroup(&(fitter->listCellsOrg), fHit,iMod);
  if(isGeant) ((HMdcHitSim*)fHit)->calcNTracks();
  fillClusInf(fHit,iMod,0); // Cluster inform.
  return index;
}

Bool_t HMdc12Fit::fillHitSegByClus(Int_t iMod) {
  // iMod==0 - first MDC in segment
  // iMod==1 - second MDC in segment
  // iMod==-1 - two MDC in segment
  HMdcSeg* fSeg = getSegSlot();
  if(!fSeg) return kFALSE;
  Int_t indx1=(iMod!=1) ? fillHitByClus(0):-1;
  Int_t indx2=(iMod!=0) ? fillHitByClus(1):-1;
  if(indx1>=0) fSeg->setHitInd(0,indx1);
  if(indx2>=0) fSeg->setHitInd(1,indx2);
  setGroup(&(fitter->listCellsOrg), fSeg);
  HMdcSizesCellsSec& fSCSec=(*(fitpar.fSizesCells))[fitter->sector];
  const HGeomVector* targ=(&fSCSec) ? &(fSCSec.getTargetMiddlePoint()) : 0;
  hitSegFiller.fillMdcSegByClus(fSeg,targ);
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
  // Cluster param:
  fillClusInf(fSeg,kFALSE);
  return kTRUE;
}

HMdcHit* HMdc12Fit::getHitSlot(Int_t mod, Int_t& index) {
  // Geting a slot and setting address
  locHit[0]=fitter->sector;
  locHit[1]=mod;
  HMdcHit* fHit = (HMdcHit*)fHitCat->getNewSlot(locHit,&index);
  if(!fHit) {
    Error("getHitSlot"," No slot in catMdcHit available");
    index=-1;
    return 0;
  }
  if(isGeant) fHit=(HMdcHit*) new(fHit) HMdcHitSim;
  else fHit= new(fHit) HMdcHit;
  fHit->setSecMod(fitter->sector,mod);
  return fHit;
}

HMdcSeg* HMdc12Fit::getSegSlot(void) {
  // Geting a slot and setting address
  locSeg[0]=fitter->sector;
  locSeg[1]=fitter->segment;
  Int_t index;
  HMdcSeg* fSeg = (HMdcSeg*)fSegCat->getNewSlot(locSeg,&index);
  if(!fSeg) {
    Error("getSegSlot"," No slot in catMdcSeg available");
    fitter->segIndex=-1;
    return 0;
  }
  fitter->segIndex=index;
  fSeg = (isGeant) ? (HMdcSeg*)(new(fSeg) HMdcSegSim) : new(fSeg) HMdcSeg;
  fSeg->setSec(fitter->sector);
  fSeg->setIOSeg(fitter->segment);
  return fSeg;
}

HMdcTrkCand* HMdc12Fit::fillTrkCandISeg(void) {
  Int_t index;
  HMdcTrkCand* fTrkCandNew = 
      (HMdcTrkCand*)fTrkCandCat->getNewSlot(locTrkCand,&index);
  if(!fTrkCandNew) {
    Error("fillTrkCandISeg"," No slot available in catMdcTrkCand");
    fitter->segIndex=-1;
    return 0;
  }
  return new(fTrkCandNew) HMdcTrkCand(fitter->sector,fitter->segIndex,index);
}

HMdcTrkCand* HMdc12Fit::fillTrkCandOSeg(HMdcTrkCand* fTrkCand) {
//   if(fTrkCand->getSeg2Ind() <0) {
//     fTrkCand->addSeg2Ind(fitter->segIndex);
//     return fTrkCand;
//   }
  Int_t index;
  HMdcTrkCand* fTrkCandNew =
      (HMdcTrkCand*)fTrkCandCat->getNewSlot(locTrkCand,&index);
  if(!fTrkCandNew) {
    Error("fillTrkCandOSeg"," No slot available in catMdcTrkCand");
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

void HMdc12Fit::setGroup(HMdcList12GroupCells* lCl, HMdcCellGroup* lHit,
    Int_t iMod) {
  // For HMdcSeg: setGroup(fClst,fSeg);
  // For HMdcHit: setGroup(fClst,fSeg,iMod);  iMod=0 or 1 - index in segment!
  Int_t lay1=(iMod!=1) ? 0: 6;
  Int_t lay2=(iMod==0) ? 6:12;
  Int_t t[4];
  for(Int_t layer=lay1; layer<lay2; layer++) {
    Int_t cell=lCl->get4FirstCells(layer,t);
    if(cell>=0) lHit->setSignId(layer-lay1,cell,t[0],t[1],t[2],t[3]);
  }
}

void HMdc12Fit::printStatus(void) {
  // prints the parameters to the screen
  char* hAuth ="0 = Hejdar,  1 = Alexander";
  char* hTmVer="0 = polynomial,  1 = HMdcCal2ParSim::calcTime";
  char* hFType="0 = segment fit,  1 = independent mdc fit";
  char* hDCat ="1 = create and fill catMdcClusFit&catMdcWireFit 0 = don't cr.";
  char* hDPr  ="1 = print debug information, 0 = don't print";
  char* hUWOff="1 = remove signal time on wire from drift time, 0 = don't remove";
  printf("--------------------------------------------------------------------------------------------\n");
  printf("HMdc12FitSetup:\n");
  printf("FitAuthor       =  %i :  %s\n",HMdcTrackDSet::fitAuthor,hAuth);
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

