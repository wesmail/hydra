//Task for all Matchings, fills hmetamatch
//*-- Author : Anar Rustamov (27.08.2003)
using namespace std;

#include "hmetamatchF.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hspecgeompar.h" 
#include "tofdef.h"
#include "richdef.h"
#include "triggerinfodef.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hmetamatch.h"
#include "hgeomtransform.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdcseg.h"
#include "hkickplane2.h"
#include "hmdctrkcand.h"
#include "hmdctrackgspline.h"
#include "hgeomvector.h"
#include "hlocation.h"
#include "hshowerhittoftrack.h"
#include "hmdcgetcontainers.h"
#include "htofhit.h"
#include "hrichhit.h"
#include "hrichhitIPU.h"
#include <iostream>

#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "kickdef.h"


ClassImp(HMetaMatchF)

HMetaMatchF::HMetaMatchF() {
  setInitParam();
}

HMetaMatchF::HMetaMatchF(Text_t name[], Text_t title[]):
  HReconstructor(name,title) {
  setInitParam();
}

void HMetaMatchF::setInitParam(void) {
  fCatTrkCand=0;
  fCatMdcSeg=0;
  fCatRich=0;
  fCatRichIPU=0;
  Spline=0;
  fTrkCandIter=0;
  IterRich=0;
  IterRichIPU=0;
  condGeant=kFALSE;
  for(Int_t s=0;s<6;s++) {tRans[s]=0;}
  Spline=0;
  rad2deg = 180.0/TMath::Pi();
  
  // Matching parameters. Must be taken from rndb! ...
  dThRich=sqrt(8.);   
  dPhRich=sqrt(8.);   
  dThRichIPU=sqrt(8.);
  dPhRichIPU=sqrt(8.);
}

HMetaMatchF::~HMetaMatchF() {
    for(Int_t s=0;s<6;s++)
    {
	if(tRans[s]) {
	    //delete tRans[s]; // ?????? crash in Hades delete why?
	    //tRans[s]=0;
	}
    }
  if(Spline) delete Spline;
  if(fGetCont) fGetCont->deleteCont();

  if(IterRich){
      delete IterRich;
      IterRich=0;
  }
  if(IterRichIPU){
      delete IterRichIPU;
      IterRichIPU=0;
  }
}

Bool_t HMetaMatchF::init() {
  if (!gHades) return kFALSE;
  HRuntimeDb *rtdb=gHades->getRuntimeDb();
  HSpectrometer *spec=gHades->getSetup();
  HEvent *event=gHades->getCurrentEvent();
  if(rtdb && spec && event) {
    fGetCont=HMdcGetContainers::getObject();
    /*HMdcGeomPar *fMdcGeometry=*/ fGetCont->getMdcGeomPar();
    /*HSpecGeomPar *fSpecGeomPar=*/ fGetCont->getSpecGeomPar();
    kickplane=(HKickPlane2*)(rtdb->getContainer("KickPlane2MDC3"));
    if(!kickplane) cout<<"YESSS DO IT!!!"<<endl;
  }
  Spline=new HMdcTrackGSpline("Spline","Spline"); 
  fCatTrkCand=event->getCategory(catMdcTrkCand);
  if (!fCatTrkCand) return kFALSE;
  fTrkCandIter=(HIterator*)fCatTrkCand->MakeIterator();
  if(!fTrkCandIter) return kFALSE;
  fCatMdcSeg=event->getCategory(catMdcSeg);
  fCatTof=event->getCategory(catTofHit);
  if(!fCatTof) return kFALSE;
  IterTof=(HIterator*)fCatTof->MakeIterator();
  HCategory *fCatKine=event->getCategory(catGeantKine);
  if(!fCatKine) {
    fCatShower=event->getCategory(catShowerHitTof);
    if(!fCatShower) return kFALSE;
  } else {
    condGeant=kTRUE;
    fCatShower=event->getCategory(catShowerHitTofTrack);
    if(!fCatShower) return kFALSE;
  }
  IterShower=(HIterator*)fCatShower->MakeIterator();

  fCatRich=event->getCategory(catRichHit);
  if(!fCatRich)
  {
      Warning("HMetaMatchF::init()","NO RICH catRichHit in input! \n Matching with Rich will be skipped!");
  }
  if(fCatRich)IterRich=(HIterator*)fCatRich->MakeIterator();
  
  fCatRichIPU=event->getCategory(catHardRichHit);
  if(!fCatRichIPU)
  {
      Warning("HMetaMatchF::init()","NO RICH IPU catHardRichHit in input! \n Matching with Rich IPU will be skipped!");
  }
  if(fCatRichIPU)IterRichIPU=(HIterator*)fCatRich->MakeIterator();

  fCatMetaMatch=event->getCategory(catMetaMatch);
  if(!fCatMetaMatch) {
    Int_t size[2]={6,600};
    fCatMetaMatch=new HMatrixCategory("HMetaMatch",2,size,0.5);  
    if(fCatMetaMatch) 
       event->addCategory(catMetaMatch,fCatMetaMatch,"Tracks");
  }
  
  if(!initTrMatch()) return kFALSE;
  
  return kTRUE;
}

Bool_t HMetaMatchF::reinit() {
  Spline->setKickPointer(kickplane);
  for(Int_t s=0; s<6; s++) {
    if(fGetCont->getMdcDetector()->isSectorActive(s)) {
      tRans[s]=&(fGetCont->getLabTransSec(s));
      if(tRans[s]==0) return kFALSE;
    } else tRans[s]=0;
  }
  
  // Matching parameters. Must be taken from rndb! ...
  if(condGeant) {
    sigmaTofX=26*26;
    sigmaTofY=22*22;
    sigmaShowerX=12*12;
    sigmaShowerY=14*14;
    sTofX=0.;
    sTofY=0.;
    sShowerX=0.;
    sShowerY=0.;
  } else {
    sigmaTofX=43*43;
    sigmaTofY=52*52;
    sigmaShowerX=24*24;
    sigmaShowerY=26*26;
    sTofX=0.;//0.3;
    sTofY=0.;//-13;
    sShowerX=0.;//-4.5;
    sShowerY=0.;//38.;
  }

  dThRich=sqrt(8.);   
  dPhRich=sqrt(8.);   
  dThRichIPU=sqrt(8.);
  dPhRichIPU=sqrt(8.);

  return kTRUE;
}

Bool_t HMetaMatchF::finalize()
{
   return kTRUE;
}
Int_t HMetaMatchF::execute() {
  if(fCatTrkCand->getEntries()<=0) return 0;
  
  collectShowerHits();
  collectTofHits();
  
  fTrkCandIter->Reset();
  HMdcTrkCand *pTrkCand=0;
  while((pTrkCand=(HMdcTrkCand*)(fTrkCandIter->Next()))!=0) { 
    //begin while on HMdcTrkCand
    if(pTrkCand->getNCandForSeg1()<0) continue;
    Short_t firstCandInd = pTrkCand->getFirstCandInd();
    
    Short_t index1=pTrkCand->getSeg1Ind();
    if(index1<0) continue;  // mozhet biyt' nado budet izment' ...
    segments[0]=(HMdcSeg*)fCatMdcSeg->getObject(index1);
    if(!segments[0]) continue;
    
    setCurrentSector(pTrkCand->getSec());
    
    // MDC-RICH matching
    richMatching();
    
    Int_t nMetaMatchObj=0;
    while((trkCandIndex=pTrkCand->getNextCandInd())>=0) {
      pTrkCand=(HMdcTrkCand*)fCatTrkCand->getObject(trkCandIndex);
      // MDC-Meta matching with outer mdc segment:
      nMetaMatchObj+=makeOuterSegMatch(pTrkCand->getSeg2Ind());
    }
    
    // MDC-Meta matching without using outer mdc segment:
    nMetaMatchObj+=makeInnerSegMatch(firstCandInd);
    
    // No outer mdc segment & no meta hits in this sector:
    if(nMetaMatchObj == 0) fillMeta(-1,-1);
  }

  executeTrMatch();  // !!!!!!!!!!!!!
  
  return 0;
}

Int_t HMetaMatchF::makeOuterSegMatch(Short_t index2) {
  // Meta matching with outer mdc segment
  totalNumber=0;
  if(index2<0) return 0;
  segments[1]=(HMdcSeg*)fCatMdcSeg->getObject(index2);
  if(!segments[1]) return 0;
  
  if(segments[1]->getHitInd(0)<0 || segments[1]->getHitInd(1)<0) 
     Spline->calcSegPoints123(segments,POINTS);
  else
     Spline->calcSegPoints(segments,POINTS);

  clearShrInOverlap();
  for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) {
    qualityTOF=qualityTof(nTof);  
    if(qualityTOF<25.) fillMeta(indShrOverlap(nTof),nTof);
  }
  for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
    qualitySHOWER=qualityShower(nSh);
    if(!showerIsInOverSec[nSh] && qualitySHOWER<25.) fillMeta(nSh,-1);
  }

  if(totalNumber ==0) fillMeta(-1,-1);

  return totalNumber;
}

Int_t HMetaMatchF::makeInnerSegMatch(Int_t firstCandInd) {
  // Meta matching without using outer mdc segment.
  totalNumber=0;
  trkCandIndex=firstCandInd;
  qualitySHOWER=qualityTOF=0.;
  clearShrInOverlap();
  for(Int_t nTof=0;nTof<nTofHitsSec;nTof++) {
    if(!tofIsUsedSec[nTof]) fillMeta(indShrOverlap(nTof),nTof);
  }
  for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
    if(!showerIsUsedSec[nSh]) fillMeta(nSh,-1);
  }
  return totalNumber;
}

Double_t HMetaMatchF::qualityTof(Int_t hit) {
  Float_t Xtof=tofHitsSec[hit].getX();
  Float_t Ytof=tofHitsSec[hit].getY();
  Float_t Ztof=tofHitsSec[hit].getZ();
//	metaDistance=Spline->getMetaDistance(0.1*Xtof,0.1*Ytof,0.1*Ztof);
  Double_t Xcalc,Ycalc;
  Spline->equationofLine3D(POINTS[2],POINTS[3],0.1*Ztof,Xcalc,Ycalc);
  return (Xtof-10*Xcalc-sTofX)*(Xtof-10*Xcalc-sTofX)/sigmaTofX +
         (Ytof-10*Ycalc-sTofY)*(Ytof-10*Ycalc-sTofY)/sigmaTofY;
}

void HMetaMatchF::collectTofHits(void) {
  IterTof->Reset();
  for(Int_t s=0;s<6;s++) nTofHits[s]=0;
  HTofHit *pTofHit;
  while((pTofHit=(HTofHit*)(IterTof->Next()))!=0 ) {
    Float_t Xtof,Ytof,Ztof;
    Char_t sec = pTofHit->getSector();
    pTofHit->getXYZLab(Xtof,Ytof,Ztof);
    HGeomVector& point=tofHits[sec][nTofHits[sec]];
    point.setXYZ(Xtof,Ytof,Ztof);
    if(tRans[sec]==0) continue; //???
    point=tRans[sec]->transTo(point);
    indexTofHit[sec][nTofHits[sec]]=fCatTof->getIndex(pTofHit);
    tofIsUsed[sec][nTofHits[sec]]=0;
    nTofHits[sec]++;
  }
}

Double_t HMetaMatchF::qualityShower(Int_t hit) {
  Float_t Xshr=showerHitsSec[hit].getX();
  Float_t Yshr=showerHitsSec[hit].getY();
  Float_t Zshr=showerHitsSec[hit].getZ();
//	metaDistance=Spline->getMetaDistance(0.1*Xshr,0.1*Yshr,0.1*Zshr);
  Double_t Xcalc,Ycalc;
  Spline->equationofLine3D(POINTS[2],POINTS[3],0.1*Zshr,Xcalc,Ycalc);
  return (Xshr-10*Xcalc-sShowerX)*(Xshr-10*Xcalc-sShowerX)/sigmaShowerX +
         (Yshr-10*Ycalc-sShowerY)*(Yshr-10*Ycalc-sShowerY)/sigmaShowerY;
}

Int_t HMetaMatchF::indShrOverlap(Int_t nTof) {
  //checking overlap with Shower Detector;
  HGeomVector& point=tofHitsSec[nTof];
  metaDir=Spline->calcMetaDir(segments[0],point);
  for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
    metaDirShower=Spline->calcMetaDir(segments[0],showerHitsSec[nSh]);
    if(metaDir.scalarProduct(metaDirShower) > 0.999) {
      qualitySHOWER=qualityShower(nSh);
      return nSh;
    }
  }
  return -1;
}

void HMetaMatchF::collectShowerHits(void) {
  IterShower->Reset();
  for(Int_t s=0;s<6;s++) nShowerHits[s]=0;
  HShowerHitTofTrack *pShowerHitTofTrack;
  while((pShowerHitTofTrack=(HShowerHitTofTrack*)(IterShower->Next()))!=0) {
    Char_t sec = pShowerHitTofTrack->getSector();
    if(nShowerHits[sec]==200) continue; //!!!
    HGeomVector& point=showerHits[sec][nShowerHits[sec]];
    Float_t Xtof,Ytof,Ztof;
    pShowerHitTofTrack->getLabXYZ(&Xtof,&Ytof,&Ztof);
    point.setXYZ(Xtof,Ytof,Ztof);
    if(tRans[sec]==0) continue; //???
    point=tRans[sec]->transTo(point);
    indexShrHit[sec][nShowerHits[sec]]=fCatShower->getIndex(pShowerHitTofTrack);
    nShowerHits[sec]++;
  }
}

void HMetaMatchF::clearShrInOverlap(void) {
  for(Int_t n=0;n<nShowerHitsSec;n++) showerIsInOverSec[n]=0;
}

void HMetaMatchF::clearIsUsedArr(void) {
  for(Int_t n=0;n<nShowerHitsSec;n++) showerIsUsedSec[n]=0;
  for(Int_t n=0;n<nTofHitsSec;n++) tofIsUsedSec[n]=0;
}

void HMetaMatchF::richMatching(void) {  
  Float_t mdcPhi = segments[0]->getPhi()*rad2deg;
  if(sector != 5) mdcPhi = mdcPhi+sector*60.;
  else mdcPhi = mdcPhi-60.;
  Float_t mdcTheta = segments[0]->getTheta()*rad2deg;
  
  nRichId = 0;
  if(fCatRich)
  {
      IterRich->Reset();
      HRichHit* pRichHit;
      while((pRichHit=(HRichHit*)(IterRich->Next()))!=0) {
	  if(pRichHit->getSector() != sector) continue;
	  if(isRichMdcCorrelation(mdcTheta,mdcPhi, pRichHit)) {
	      if(nRichId < RICH_TAB_SIZE) aRichIndTable[nRichId] = 
                  fCatRich->getIndex(pRichHit);
	      nRichId++;
	  }
      }
  }
  if(fCatRichIPU)
  {
      HRichHitIPU* pRichHitIPU;
      nRichIPUId = 0;
      IterRichIPU->Reset();
      while((pRichHitIPU=(HRichHitIPU*)(IterRichIPU->Next()))!=0) {
	  if(pRichHitIPU->getSector() != sector) continue;
	  if(isRichMdcCorrelation(mdcTheta,mdcPhi, pRichHitIPU)) {
	      if(nRichIPUId < RICH_TAB_SIZE) aRichIPUIndTable[nRichIPUId] = 
                  fCatRichIPU->getIndex(pRichHitIPU);
	      nRichIPUId++;
	  }
      }
  }

}


Bool_t HMetaMatchF::isRichMdcCorrelation(Float_t mdcTheta,Float_t mdcPhi, 
    HRichHit *pRich) {
  Float_t dPhi = (pRich->getPhi() - mdcPhi)/dPhRich;
  Float_t dTheta = (pRich->getTheta() - mdcTheta)/dThRich;
  if( dTheta*dTheta+dPhi*dPhi <= 1.0) return kTRUE;
  return kFALSE;
}

Bool_t HMetaMatchF::isRichMdcCorrelation(Float_t mdcTheta,Float_t mdcPhi, 
    HRichHitIPU *pRich) {
  Float_t dPhi = (pRich->getPhi() - mdcPhi)/dPhRichIPU;
  Float_t dTheta = (pRich->getTheta() - mdcTheta)/dThRichIPU;
  if( dTheta*dTheta+dPhi*dPhi <= 1.0 ) return kTRUE;
  return kFALSE;
}

void HMetaMatchF::setCurrentSector(Int_t sec) {
  sector=sec;
  sectorloc.set(1,sector);
  
  showerIsUsedSec=showerIsUsed[sec];
  nShowerHitsSec=nShowerHits[sec];
  tofIsUsedSec=tofIsUsed[sec];
  nTofHitsSec=nTofHits[sec];
  clearIsUsedArr();
  
  showerIsInOverSec=showerIsInOver[sec];
  showerHitsSec=showerHits[sec];
  indexShrHitSec=indexShrHit[sec];
  
  tofHitsSec=tofHits[sec];
  indexTofHitSec=indexTofHit[sec];
} 

HMetaMatch *HMetaMatchF::fillMeta(Int_t hShower, Int_t hTof) {
  Int_t index=-1;
  HMetaMatch * metaNew=(HMetaMatch*)fCatMetaMatch->getNewSlot(sectorloc,&index);
  if(!metaNew) {
    Error("fillMeta","No slot available in sector %i. size of catMetaMatch is %i !",sectorloc[0]+1,fCatMetaMatch->getEntries());
//    cout<<"No slot available,,,"<<endl;
    return 0;
  }
  if(totalNumber==0) {
    HMdcTrkCand* pTrkCand=(HMdcTrkCand*)fCatTrkCand->getObject(trkCandIndex);
    if(pTrkCand) pTrkCand->setMetaMatchInd((Short_t)index);
    meta=new(metaNew) HMetaMatch(sector,trkCandIndex,index);
    if( nRichId > 0) {
      if(nRichId>3) nRichId=3;
      meta->setNCandForRich(nRichId);
      for(Int_t i = 0; i < nRichId; i++) meta->setARichInd(i,aRichIndTable[i]);
    }
    if(nRichIPUId > 0) {
      if(nRichIPUId>3) nRichIPUId=3;
      meta->setNCandForIPU(nRichIPUId);
      for(Int_t i = 0; i < nRichIPUId; i++) meta->setARichIPUInd(i,aRichIPUIndTable[i]);
    }    
  } else meta=new(metaNew) HMetaMatch(meta,index);
  
  if(hShower>=0) {
    if(hTof>=0) showerIsInOverSec[hShower]++;
    showerIsUsedSec[hShower]++;
    meta->setShowerHitInd(indexShrHitSec[hShower]);
    meta->setQualitySHOWER(qualitySHOWER);
  }
  if(hTof>=0) {
    tofIsUsedSec[hTof]++;
    meta->setTofHitInd(indexTofHitSec[hTof]);
    meta->setQualityTOF(qualityTOF);
  }
  
  totalNumber++;
  HMetaMatch *metaFirst=
      (HMetaMatch*)(fCatMetaMatch->getObject(meta->getFirstCandForMeta()));
  metaFirst->setNCandForMeta(totalNumber);
  return meta;
}


Bool_t HMetaMatchF::initTrMatch() {
  HEvent *event=gHades->getCurrentEvent();
  fCatKickTrack=event->getCategory(catKickTrack);
  if(!fCatKickTrack) return kFALSE;
  
  iterMetaMatch=(HIterator*)fCatMetaMatch->MakeIterator();
  if(!iterMetaMatch) return kFALSE;
  iterKickTrack=(HIterator*)fCatKickTrack->MakeIterator("native");
  if(!iterKickTrack) return kFALSE;
  
  fCatKickTrackB=event->getCategory(catKickTrackB);
  if(!fCatKickTrackB) {
    Int_t ssize[2]={6,100};
    fCatKickTrackB = new HMatrixCategory("HKickTrackB",2,ssize,0.5);
    if(fCatKickTrackB) event->addCategory(catKickTrackB,fCatKickTrackB,"Tracks");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  } else {
    Error("init","catKickTrackB created already");
    return kFALSE;
  }
  
  locTofHit.set(3,0,0,0);
  
  return kTRUE;
}

Int_t HMetaMatchF::executeTrMatch() {
  if(fCatKickTrack->getEntries()<=0) return 0;
  if(fCatMetaMatch->getEntries()<=0) return 0;

  iterKickTrack->Reset();
  iterMetaMatch->Reset();
  HKickTrackB* pKickTrackB;
  HKickTrack* pKickTrack;
  while((pKickTrack=(HKickTrack*)(iterKickTrack->Next()))!=0) {
    Char_t sec = pKickTrack->getSector();
    if(sec<0||sec>5) continue;
    locKickTrack.set(1,sec);
    Int_t indexKickTrackB=-1;
    pKickTrackB=(HKickTrackB*)
        (fCatKickTrackB->getNewSlot(locKickTrack,&indexKickTrackB));
    if (pKickTrackB) pKickTrackB = new(pKickTrackB) HKickTrackB(pKickTrack);
    else {
      Error("execute","No slot available");
      return 0;
    }
    
    Int_t indexKickTrack=fCatKickTrack->getIndex(pKickTrack);
    if(indexKickTrack!=indexKickTrackB) {
      Error("execute","Mismatching HKickTrack<->HKickTrackB");
      return 0;
    }
    Short_t tofInd=pKickTrackB->getTofHitInd();
    if(tofInd>=0) {
      if(!fCatTof) {
        pKickTrackB->setTofHitInd(-1);
      } else {
        Short_t c=tofInd%100;
        Short_t m=(tofInd/100)%10;
        Short_t s=tofInd/1000;
        locTofHit.set(3,s,m,c);
        Int_t tofInd=fCatTof->getIndex(locTofHit);
        if(tofInd<0) {
          Error("execute","No TofHit for location!");
        } else {
// HTofHit* pTofHit1=(HTofHit*)fCatTof->getObject(tofInd);
// HTofHit* pTofHit2=(HTofHit*)fCatTof->getObject(locTofHit);
// if(pTofHit1!=pTofHit2) Printf("uuuuuuuuuuuuuu");
          pKickTrackB->setTofHitInd(tofInd);
        }
      }
    }

    Int_t system = pKickTrackB->getSystem();
    if(system<0) continue;
    Int_t indexMdcSeg = pKickTrackB->getSegment();
    segLoc.set(3,sec,0,indexMdcSeg);
    indexMdcSeg=fCatMdcSeg->getIndex(segLoc);
    if(indexMdcSeg<0) {
      Error("executeTrMatch","No HMdcSeg object for HKickTrack");
      continue;
    }
    
    locMetaMatch.set(1,sec);
    //? iterMetaMatch->Reset();
    iterMetaMatch->gotoLocation(locMetaMatch);
    HMetaMatch* pMetaMatch;
    while((pMetaMatch=(HMetaMatch*)(iterMetaMatch->Next()))!=0) {
      Short_t trkCandInd = pMetaMatch->getTrkCandInd();
      HMdcTrkCand* pTrkCand=(HMdcTrkCand*)fCatTrkCand->getObject(trkCandInd);
      if(pTrkCand->getSeg1Ind() != indexMdcSeg) continue;
      if(system==0 && pMetaMatch->getShowerHitInd() != 
          pKickTrackB->getShowerHitInd()) continue;
      if(system>0  && pMetaMatch->getTofHitInd()    != 
          pKickTrackB->getTofHitInd()) continue;
      pMetaMatch->setKickInd(indexKickTrackB);
    }
  }
  return 0;
}
