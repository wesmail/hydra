//Task for all Matchings, fills hmetamatch
//*-- Author : Anar Rustamov (27.08.2003)
//*-- Modified : 14/10/2004 by V. Pechenov

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
#include "hshowergeometry.h"
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
#include "htofcluster.h"
#include "htofgeompar.h"
#include "hrichhit.h"
#include "hrichhitIPU.h"
#include "hmetamatchpar.h"
#include <iostream>

ClassImp(HMetaMatchF)

HMetaMatchF::HMetaMatchF() {
  setInitParam();
}

HMetaMatchF::HMetaMatchF(Text_t name[], Text_t title[]):
  HReconstructor(name,title) {
  setInitParam();
}

void HMetaMatchF::setInitParam(void) {
  fMatchPar=0;
  fCatTrkCand=0;
  fCatMdcSeg=0;
  fCatRich=0;
  fCatRichIPU=0;
  Spline=0;
  fTrkCandIter=0;
  fTofGeometry=0;
  IterTof=0;
  IterTofCluster=0;
  IterRich=0;
  IterRichIPU=0;
  fTrkCandIter=0;
  IterShower=0;
  fShrGeometry=0;
  condGeant=kFALSE;
  for(Int_t s=0;s<6;s++) {labTrans[s]=0;}
  Spline=0;
  rad2deg = 180.0/TMath::Pi();
  locTofHit.set(3,0,0,0);
}

HMetaMatchF::~HMetaMatchF() {
  if(Spline) delete Spline;
  if(fGetCont) fGetCont->deleteCont();

  if(fTrkCandIter) {
    delete fTrkCandIter;
    fTrkCandIter=0;
  }
  if(IterRich){
    delete IterRich;
    IterRich=0;
  }
  if(IterRichIPU){
    delete IterRichIPU;
    IterRichIPU=0;
  }
  if(IterTof) {
    delete IterTof;
    IterTof=0;
  }
  if(IterTofCluster) {
    delete IterTofCluster;
    IterTofCluster=0;
  }
  if(IterShower) {
    delete IterShower;
    IterShower=0;
  }
}

Bool_t HMetaMatchF::init() {
  if (!gHades) return kFALSE;
  HRuntimeDb *rtdb=gHades->getRuntimeDb();
  if(!rtdb) return kFALSE;
  HEvent *event=gHades->getCurrentEvent();
  if(!event) return kFALSE;
  fGetCont=HMdcGetContainers::getObject();
  fGetCont->getMdcGeomPar();  // for spline ???
  fGetCont->getSpecGeomPar(); // for lab-sec. transf
//  kickplane=(HKickPlane2*)(rtdb->getContainer("KickPlane2MDC3"));
  kickplane = HKickPlane2::getMDC3();
  if(!kickplane) printf("YESSS DO IT!!!\n"); //???????????????
  Spline=new HMdcTrackGSpline("Spline","Spline");
  fCatTrkCand=event->getCategory(catMdcTrkCand);
  if (!fCatTrkCand) {
    Error("init","NO catMdcTrkCand in input! STOP!!!");
    return kFALSE;
  }
  fTrkCandIter=(HIterator*)fCatTrkCand->MakeIterator();
  if(!fTrkCandIter) return kFALSE;
  fCatMdcSeg=event->getCategory(catMdcSeg);

  fCatTof=event->getCategory(catTofHit);
  if(!fCatTof) Warning("init",
      "No catTofHit in input! \n Matching with TofHits will be skipped!");
  else IterTof=(HIterator*)fCatTof->MakeIterator();
  fCatTofCluster=event->getCategory(catTofCluster);
  if(!fCatTofCluster) {
    Warning("init","NO catTofCluster in input! \n Matching with TofClusters will be skipped!");
  } else IterTofCluster=(HIterator*)fCatTofCluster->MakeIterator();
  if(fCatTof || fCatTofCluster) fTofGeometry=
      (HTofGeomPar *)rtdb->getContainer("TofGeomPar");

  HCategory *fCatKine=event->getCategory(catGeantKine);
  if(!fCatKine) {
    fCatShower=event->getCategory(catShowerHitTof);
    if(!fCatShower) Warning("init",
        "NO catShowerHitTof in input! \n Matching with Shower will be skipped!");
  } else {
    condGeant=kTRUE;
    fCatShower=event->getCategory(catShowerHitTofTrack);
    if(!fCatShower) Warning("init",
        "NO catShowerHitTofTrack in input! \n Matching with Shower will be skipped!");
  }
  if(fCatShower) {
    IterShower=(HIterator*)fCatShower->MakeIterator();
    fShrGeometry=(HShowerGeometry*)rtdb->getContainer("ShowerGeometry");
  }

  fCatRich=event->getCategory(catRichHit);
  if(fCatRich) IterRich=(HIterator*)fCatRich->MakeIterator();
  else Warning("HMetaMatchF::init()","NO RICH catRichHit in input! \n Matching with Rich will be skipped!");

  fCatRichIPU=event->getCategory(catHardRichHit);
  if(fCatRichIPU) IterRichIPU=(HIterator*)fCatRich->MakeIterator();
  else Warning("HMetaMatchF::init()","NO RICH IPU catHardRichHit in input! \n Matching with Rich IPU will be skipped!");

  fCatMetaMatch=event->getCategory(catMetaMatch);
  if(!fCatMetaMatch) {
    Int_t size[2]={6,600};
    fCatMetaMatch=new HMatrixCategory("HMetaMatch",2,size,0.5);
    if(fCatMetaMatch)
       event->addCategory(catMetaMatch,fCatMetaMatch,"Tracks");
  }
  fMatchPar=(HMetaMatchPar*)rtdb->getContainer("MetaMatchPar");

  return kTRUE;
}

Bool_t HMetaMatchF::reinit() {
  Spline->initMiddleParams(Spline->takeMiddleParams(fGetCont,0,2));
  Spline->initMiddleParamsP4(Spline->takeMiddleParams(fGetCont,0,3));
  Spline->setKickPointer(kickplane);
  for(Int_t s=0; s<6; s++) {
     if(fGetCont->getMdcDetector()->isSectorActive(s)) {
	labTrans[s]=&(fGetCont->getLabTransSec(s));
	if(labTrans[s]==0) return kFALSE;
     } else labTrans[s]=0;
  }
  if(fTofGeometry && !HMdcGetContainers::isInited(fTofGeometry)) return kFALSE;
  if(fShrGeometry && !HMdcGetContainers::isInited(fShrGeometry)) return kFALSE;

  if(fMatchPar==0 || !HMdcGetContainers::isInited(fMatchPar)) {
    Error("reinit","no parameters for matching!");
    return kFALSE;
  }
  setMatchingParam();
  return kTRUE;
}

void HMetaMatchF::setMatchingParam(void) {
  // Matching parameters.
  for(Int_t s=0;s<6;s++) {
    dThRich[s]=fabs( fMatchPar->getRichThetaMaxCut(s) -
                     fMatchPar->getRichThetaMinCut(s) )/2.;
//    dThRichOff[s]=( fMatchPar->getRichThetaMinCut(s) + 
//                    fMatchPar->getRichThetaMaxCut(s) )/2.;
    dPhRich[s]=fMatchPar->getRichSigmaPhi(s);
    dPhRichOff[s]=fMatchPar->getRichSigmaPhiOffset(s);
    qualityRichCut[s]=fMatchPar->getRichQualityCut(s);

    dThRichIPU[s]=fabs( fMatchPar->getRichIPUThetaMaxCut(s) -
                        fMatchPar->getRichIPUThetaMinCut(s) )/2.;
    //dThRichIPUOff[s]=( fMatchPar->getRichIPUThetaMinCut(s) +
    //                   fMatchPar->getRichIPUThetaMaxCut(s) )/2.;
    dPhRichIPU[s]=fMatchPar->getRichIPUSigmaPhi(s);
    dPhRichIPUOff[s]=fMatchPar->getRichIPUSigmaPhiOffset(s);
    qualityRichIPUCut[s]=fMatchPar->getRichIPUQualityCut(s);

    sigma2MdcInShrX[s]=
        fMatchPar->getShowerSigmaXMdc(s)*fMatchPar->getShowerSigmaXMdc(s);
    sigma2MdcInShrY[s]=
        fMatchPar->getShowerSigmaYMdc(s)*fMatchPar->getShowerSigmaYMdc(s);
    sShowerX[s]=fMatchPar->getShowerSigmaXOffset(s);
    sShowerY[s]=fMatchPar->getShowerSigmaYOffset(s);
    qualitySHOWERCut[s]=fMatchPar->getShowerQualityCut(s);

    sigmaTofX[s]=fMatchPar->getTofSigmaX(s);
    sigmaTofY[s]=fMatchPar->getTofSigmaY(s);
    sTofX[s]=fMatchPar->getTofSigmaXOffset(s);
    sTofY[s]=fMatchPar->getTofSigmaYOffset(s);
    qualityTOFCut[s]=fMatchPar->getTofQualityCut(s);
    richThetaMinCut[s]=fMatchPar->getRichThetaMinCut(s);
    richThetaMaxCut[s]=fMatchPar->getRichThetaMaxCut(s);
    richIPUThetaMinCut[s]=fMatchPar->getRichIPUThetaMinCut(s);
    richIPUThetaMaxCut[s]=fMatchPar->getRichIPUThetaMaxCut(s);
  }
//   for(Int_t s=0;s<6;s++) {
//     if(condGeant) {
//       sigmaTofX[s]=26;
//       sigmaTofY[s]=22;
//       sigmaShowerX[s]=12;
//       sigmaShowerY[s]=14;
//       sTofX[s]=0.;
//       sTofY[s]=0.;
//       sShowerX[s]=0.;
//       sShowerY[s]=0.;
//     } else {
//       sigmaTofX[s]=43;
//       sigmaTofY[s]=52;
//       sigmaShowerX[s]=24;
//       sigmaShowerY[s]=26;
//       sTofX[s]=0.;//0.3;
//       sTofY[s]=0.;//-13;
//       sShowerX[s]=0.;//-4.5;
//       sShowerY[s]=0.;//38.;
//     }
//     qualityTOFCut[s]=5.;    //25.;
//     qualitySHOWERCut[s]=5.; //25.;
// 
//     dThRich[s]=sqrt(8.);
//     dPhRich[s]=sqrt(8.); // ???
//     dThRichOff[s]=0.;
//     dPhRichOff[s]=0.;
//     qualityRichCut[s]=1.;
// 
//     dThRichIPU[s]=sqrt(8.);
//     dPhRichIPU[s]=sqrt(8.); // ???
//     dThRichIPUOff[s]=0.;
//     dPhRichIPUOff[s]=0.;
//     qualityRichIPUCut[s]=1.;
//   }
}

Bool_t HMetaMatchF::finalize() {
  return kTRUE;
}

Int_t HMetaMatchF::execute() {
  if(fCatTrkCand->getEntries()<=0) return 0;

  collectShowerHits();
  collectTofHits();

  fTrkCandIter->Reset();
  HMdcTrkCand *pTrkCand=0;

  for(Int_t sec=0;sec<6;sec++) {
    setCurrentSector(sec);
    if(!fTrkCandIter->gotoLocation(sectorLoc)) continue;

    for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) nTrCndForTHit[nTof]=0;
    for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) nTrCndForSHit[nSh]=0;
    Bool_t isCandInSec=kFALSE;
    while((pTrkCand=(HMdcTrkCand*)(fTrkCandIter->Next()))!=0) {
      if(pTrkCand->getNCandForSeg1()<0) continue;
      Short_t index1=pTrkCand->getSeg1Ind();
      if(index1<0) continue;  // mozhet biyt' nado budet izment' ...
      segments[0]=(HMdcSeg*)fCatMdcSeg->getObject(index1);
      if(!segments[0]) continue;

      makeRichMatching();            // MDC-RICH matching
      makeOuterSegMatch(pTrkCand);
      isCandInSec=kTRUE;
    }
    if(isCandInSec) {
      fillTofClstInd();
      fillMetaHitsInd();
    }
  }
  return 0;
}

void HMetaMatchF::makeOuterSegMatch(HMdcTrkCand* pTrkCand) {
  // Matching outer mdc segments with meta hits.
  // Inner mdc segment is the same.
  Short_t firstCandInd = pTrkCand->getFirstCandInd();
  Bool_t isMMatch=kFALSE;
  for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) {
    Int_t& nTrCndForT=nTrCndForTHit[nTof];
    if(nTrCndForT>=256) continue;
    mmTof[nTof][nTrCndForT]=-1;
    nTrCndForT++;
  }
  for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++)  {
    Int_t& nTrCndForS=nTrCndForSHit[nSh];
    if(nTrCndForS>=256) continue;
    mmShr[nSh][nTrCndForS]=-1;
    nTrCndForS++;
  }

  while((trkCandIndex=pTrkCand->getNextCandInd())>=0) {
    pTrkCand=(HMdcTrkCand*)fCatTrkCand->getObject(trkCandIndex);
    // MDC-Meta matching with outer mdc segment:
    meta=0;
    Short_t index2=pTrkCand->getSeg2Ind();
    if(index2<0) continue;               // It can happen by error only.
    segments[1]=(HMdcSeg*)fCatMdcSeg->getObject(index2);
    if(!segments[1]) continue;           // It can happen by error only.
    if(segments[1]->getHitInd(0)<0)
    {
       Spline->calcSegPoints123(segments,mdcTrackPar);
       }
       else if(segments[1]->getHitInd(1)<0)
       {
       Spline->calcSegPoints123P4(segments,mdcTrackPar);
       }
    else
    {
       Spline->calcSegPoints(segments,mdcTrackPar);
    }
    for(Int_t p=0;p<4;p++) mdcTrackPar[p] *= 10.; // cm => mm
    mdcTrackPar[2]=secLabTrans->transFrom(mdcTrackPar[2]);
    mdcTrackPar[3]=secLabTrans->transFrom(mdcTrackPar[3]);

    Bool_t isNotMatch=kTRUE;;
    for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) {
      Int_t& nTrCndForT=nTrCndForTHit[nTof];
      if(nTrCndForT>=256) continue;

      //Variables to store x/y-deviation of META hit and segment normalized to META resolution
      qualTof=qualityTof(nTof,meta_dx,meta_dy);
      if(qualTof>qualityTOFCut[(Int_t)sector]) continue;
      if(mmTof[nTof][nTrCndForT-1] < 0) nTrCndForT--;
      mmTof[nTof][nTrCndForT]=fillMeta(-1,nTof,meta);
      nTrCndForT++;
      isNotMatch=kFALSE;
    }
    for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
      Int_t& nTrCndForS=nTrCndForSHit[nSh];
      if(nTrCndForS>=256) continue;
      qualShower=qualityShower(nSh,meta_dx,meta_dy);
      if(qualShower>qualitySHOWERCut[(Int_t)sector]) continue;
      if(mmShr[nSh][nTrCndForS-1] < 0) nTrCndForS--;
      mmShr[nSh][nTrCndForS]=fillMeta(nSh,-1,meta);
      nTrCndForS++;
      isNotMatch=kFALSE;
    }
    // Creating of MetaMatch object for MdcTrkCand which has not have matching to Meta
    if(isNotMatch) getMetaMatchSlot(0);
    isMMatch=kTRUE;
  }
  
  // No outer segment for meta hits:
  meta_dx=meta_dy=0.;
  qualShower=qualTof=0.;
  xSegCrShr=ySegCrShr=0.;
  xSegCrTof=ySegCrTof=0.;
  trkCandIndex=firstCandInd;
  meta=0;
  for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) {
    Int_t nTrCnd=nTrCndForTHit[nTof]-1;
    if(mmTof[nTof][nTrCnd] < 0) mmTof[nTof][nTrCnd]=fillMeta(-1,nTof,meta);
  }
  for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
    Int_t nTrCnd=nTrCndForSHit[nSh]-1;
    if(mmShr[nSh][nTrCnd] < 0) mmShr[nSh][nTrCnd] = fillMeta(nSh,-1,meta);
  }
  // No outer segment and no meta hits:
  if(!isMMatch && meta==0) getMetaMatchSlot(0);
}

Int_t HMetaMatchF::getMetaMatchSlot(HMetaMatch* prevMM) {
  // For TrackCand with inner MDC segments only
  Int_t metaIndex=-1;
  HMetaMatch * metaNew=(HMetaMatch*)fCatMetaMatch->getNewSlot(sectorLoc,
      &metaIndex);
  if(!metaNew) {
    Warning("getMetaMatchSlot",
        "No slot available in sector %i. size of catMetaMatch is %i !",
        sectorLoc[0]+1,fCatMetaMatch->getEntries());
    return -1;
  }
  if(prevMM==0) {
    HMdcTrkCand* pTrkCand=(HMdcTrkCand*)fCatTrkCand->getObject(trkCandIndex);
    if(pTrkCand) pTrkCand->setMetaMatchInd((Short_t)metaIndex);
    meta=new(metaNew) HMetaMatch(sector,trkCandIndex,metaIndex);
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
  } else meta=new(metaNew) HMetaMatch(prevMM,metaIndex); // (meta,metaIndex) == meta->nextCandForMeta=metaIndex

  HMetaMatch *metaFirst=
      (HMetaMatch*)(fCatMetaMatch->getObject(meta->getFirstMMForSameTrCnd()));
  metaFirst->incrNumMMForSameTrkCnd();
  return metaIndex;
}

Double_t HMetaMatchF::qualityTof(Int_t hit,Float_t& dX_n, Float_t& dY_n) {
  HGeomTransform &tofModSys =
      fTofGeometry->getModule(sector,tofModuleSec[hit])->getLabTransform();
  HGeomVector p1=tofModSys.transTo(mdcTrackPar[2]);
  HGeomVector p2=tofModSys.transTo(mdcTrackPar[3]);
  xSegCrTof=(p1(2)*p2(0)-p1(0)*p2(2))/(p1(2)-p2(2));
  ySegCrTof=(p1(2)*p2(1)-p1(1)*p2(2))/(p1(2)-p2(2));
  Double_t Xtof=tofHitsSec[hit].getX();
  Double_t Ytof=tofHitsSec[hit].getY();
  Double_t dX=(Xtof-xSegCrTof-sTofX[(Int_t)sector])/sigmaTofX[(Int_t)sector];
  Double_t dY=(Ytof-ySegCrTof-sTofY[(Int_t)sector])/sigmaTofY[(Int_t)sector];
  dX_n=Xtof-xSegCrTof-sTofX[(Int_t)sector];
  dY_n=Ytof-ySegCrTof-sTofY[(Int_t)sector];
  return sqrt(dX*dX + dY*dY);
}

void HMetaMatchF::collectTofHits(void) {
  if(fCatTof) IterTof->Reset();
  HTofHit *pTofHit;
  for(Int_t s=0;s<6;s++) nTofHits[s]=0;
  if(!fCatTofCluster) {
    // No TofCluster category:
    if(fCatTof)
      while((pTofHit=(HTofHit*)(IterTof->Next()))!=0 ) addTofHit(pTofHit,0);
  } else {
    // TofCluster category exist:
    IterTofCluster->Reset();
    HTofCluster *pTofCluster;
    while((pTofCluster=(HTofCluster*)(IterTofCluster->Next()))!=0 ) {
      Int_t tofClSize=pTofCluster->getClusterSize();
      if(tofClSize>2) continue;                  // tofClSize<=2 only !
      addTofCluster(pTofCluster);
      if(tofClSize<2) continue;
      if(fCatTof==0) continue;
      Int_t sec = pTofCluster->getSector();
      Int_t mod=pTofCluster->getModule();
      Int_t cell=pTofCluster->getCell();
      while((pTofHit=(HTofHit*)(IterTof->Next()))!=0 ) {
        if(sec!=pTofHit->getSector() || mod!=pTofHit->getModule() ||
            cell!=pTofHit->getCell()) continue;
        if(tofClSize==2) {  // two TofHits adding, tofClSize==2 only !
          addTofHit(pTofHit,0);                  // first TofHit for TofCluster
          pTofHit=(HTofHit*)(IterTof->Next());   // second ...
          addTofHit(pTofHit,0);                  // clust.size==2 selected !!!
        } else {            // tofClSize>2 only HTofHits can be used
          addTofHit(pTofHit,-1);
          for(Int_t h=0;h<tofClSize-1;h++) {
            pTofHit=(HTofHit*)(IterTof->Next());
            addTofHit(pTofHit,-1);
          }
        }
        break;
      }
    }
  }
}

void HMetaMatchF::addTofCluster(HTofCluster* pTofCluster) {
  addTof(pTofCluster,fCatTofCluster->getIndex(pTofCluster),
      pTofCluster->getClusterSize());
}

void HMetaMatchF::addTofHit(HTofHit* pTofHit,Int_t clSize) {
  if(pTofHit==0) Error("addTofHit"," Pointer to HTofHit == 0 !");
  else addTof(pTofHit,fCatTof->getIndex(pTofHit),clSize);
}

void HMetaMatchF::addTof(HTofHit* pTofHit,Int_t index, Int_t clSize) {
  Int_t sec = pTofHit->getSector();
  Int_t& nTofHSec=nTofHits[sec];
  Float_t Xtof,Ytof,Ztof;
  pTofHit->getXYZLab(Xtof,Ytof,Ztof);
  HGeomVector& point=tofHits[sec][nTofHSec];
  point.setXYZ(Xtof,Ytof,Ztof);
  HModGeomPar *module=fTofGeometry->getModule(sec,pTofHit->getModule());
  if(module==0) {
    Error("addTof"," Can't get transformation for tof. %i sec. %imod",
        sec,pTofHit->getModule());
    return;
  }
  HGeomTransform &trans = module->getLabTransform();
  point=trans.transTo(point);    // transformation to coor.sys. of tof module

  indexTofHit[sec][nTofHSec]=index;
  tofClustSize[sec][nTofHSec]=clSize;
  tofModule[sec][nTofHSec]=pTofHit->getModule();
  nTofHSec++;
}

Double_t HMetaMatchF::qualityShower(Int_t hit,Float_t& dX_n, Float_t& dY_n)  {
  const HGeomTransform& showerModSys=
    fShrGeometry->getTransform(sector,fShowerHitsSec[hit]->getModule());
  HGeomVector p1=showerModSys.transTo(mdcTrackPar[2]);
  HGeomVector p2=showerModSys.transTo(mdcTrackPar[3]);
  xSegCrShr=(p1(2)*p2(0)-p1(0)*p2(2))/(p1(2)-p2(2));
  ySegCrShr=(p1(2)*p2(1)-p1(1)*p2(2))/(p1(2)-p2(2));
  Float_t Xshr,Yshr;
  fShowerHitsSec[hit]->getXY(&Xshr,&Yshr);
  Double_t dX=Xshr-xSegCrShr-sShowerX[(Int_t)sector];
  Double_t dY=Yshr-ySegCrShr-sShowerY[(Int_t)sector];
  Double_t dXsigma2=fShowerHitsSec[hit]->getSigmaX();
  dXsigma2= dXsigma2*dXsigma2 + sigma2MdcInShrX[(Int_t)sector];
  Double_t dYsigma2=fShowerHitsSec[hit]->getSigmaY();
  dYsigma2= dYsigma2*dYsigma2 + sigma2MdcInShrY[(Int_t)sector];
  dX_n = dX;
  dY_n = dY;
  return sqrt(dX*dX/dXsigma2 +dY*dY/dYsigma2);
}

void HMetaMatchF::collectShowerHits(void) {
  for(Int_t s=0;s<6;s++) nShowerHits[s]=0;
  if(!fCatShower) return;
  IterShower->Reset();
  HShowerHitTof *pShowerHitTof;
  while((pShowerHitTof=(HShowerHitTof*)(IterShower->Next()))!=0) {
    Int_t sec = pShowerHitTof->getSector();
    if(nShowerHits[sec]==200) continue; //!!!
    fShowerHits[sec][nShowerHits[sec]]=pShowerHitTof;
    indexShrHit[sec][nShowerHits[sec]]=fCatShower->getIndex(pShowerHitTof);
    nShowerHits[sec]++;
  }
}

// Int_t HMetaMatchF::indShrOverlap(Int_t nTof) {
//   //checking overlap with Shower Detector;
//   HGeomVector& point=tofHitsSec[nTof];
//   metaDir=Spline->calcMetaDir(segments[0],point);
//   for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
//     metaDirShower=Spline->calcMetaDir(segments[0],showerHitsSec[nSh]);
//     if(metaDir.scalarProduct(metaDirShower) > 0.999) {
//       qualitySHOWERCut=qualityShower(nSh);
//       return nSh;
//     }
//   }
//   return -1;
// }

void HMetaMatchF::makeRichMatching(void) {
  Float_t qualTheta;
  Bool_t qualityTheta;
  Float_t mdcPhi=segments[0]->getPhi()*rad2deg + (sector!=5 ? sector*60.:-60.);
  Float_t mdcTheta=segments[0]->getTheta(); // *rad2deg;
  // Matching with HRichHit's:
  nRichId = 0;
  if(fCatRich) {
    HRichHit* pRichHit;
    IterRich->Reset();
    while((pRichHit=(HRichHit*)(IterRich->Next()))!=0) {
      if(pRichHit->getSector() != sector) continue;
      Float_t quality=qualityRich(mdcTheta,mdcPhi, pRichHit,qualTheta);
      
      if(qualTheta>richThetaMinCut[(Int_t)sector]&&qualTheta<richThetaMaxCut[(Int_t)sector])
      {
      qualityTheta=kFALSE;
      }
      else qualityTheta=kTRUE;
      
      if(quality>qualityRichCut[(Int_t)sector] || qualityTheta) continue;
      Short_t ind=fCatRich->getIndex(pRichHit);
      addRing(quality,ind,aRichIndTable, nRichId);
    }
  }
  // Matching with HRichHitIPU's:
  nRichIPUId = 0;
  if(fCatRichIPU) {
    HRichHitIPU* pRichHitIPU;
    IterRichIPU->Reset();
    while((pRichHitIPU=(HRichHitIPU*)(IterRichIPU->Next()))!=0) {
      if(pRichHitIPU->getSector() != sector) continue;
      Float_t quality=qualityRichIPU(mdcTheta,mdcPhi, pRichHitIPU,qualTheta);
      if(qualTheta>richIPUThetaMinCut[(Int_t)sector]&&qualTheta<richIPUThetaMaxCut[(Int_t)sector])
        {
	qualityTheta=kFALSE;
        }
    else qualityTheta=kTRUE;
      if(quality>qualityRichIPUCut[(Int_t)sector] || qualityTheta) continue;
      Short_t ind=fCatRichIPU->getIndex(pRichHitIPU);
      addRing(quality,ind,aRichIPUIndTable, nRichIPUId);
    }
  }
}

void HMetaMatchF::addRing(Float_t quality, Short_t ind,
    Short_t* indTable,Int_t& nRich) {
  // Adding matched ring in sorted list
  if(nRich==0 || quality>=qualRich[nRich-1]) {
    // if ring is first in the list or ring must be added to the end of list:
    if(nRich>=RICH_TAB_SIZE) return;
    indTable[nRich] = ind;
    qualRich[nRich] = quality;
    nRich++;
  } else {
    // if ring must be inserted in list:
    for(Int_t i=0;i<nRich;i++) {
      if(quality>=qualRich[i]) continue;
      if(nRich<RICH_TAB_SIZE) nRich++;
      for(Int_t ish=nRich-1;ish>i;ish--) {  // Shift of rings in list
        indTable[ish]=indTable[ish-1];      // ...
        qualRich[ish]=qualRich[ish-1];      // ...
      }
      indTable[i]=ind;                      // Inserting ring in list
      qualRich[i]=quality;                  // ...
      return;
    }
  }
}

Float_t HMetaMatchF::qualityRich(Float_t mdcTheta, Float_t mdcPhi,
    HRichHit *pRich, Float_t &qualTheta) {
  // mdcTheta - in rad.!
  // mdcPhi - in deg.!
  Double_t dPhi=pRich->getPhi() - mdcPhi - dPhRichOff[(Int_t)sector];
  dPhi *= TMath::Sin(mdcTheta)/dPhRich[(Int_t)sector];
  Double_t dTheta=pRich->getTheta()-mdcTheta*rad2deg;
 // dTheta=(dTheta-dThRichOff[sector])/dThRich[sector];
  qualTheta=dTheta;
  return sqrt(dPhi*dPhi);
}

  Float_t HMetaMatchF::qualityRichIPU(Float_t mdcTheta,Float_t mdcPhi,
    HRichHitIPU *pRich,Float_t &qualTheta) {
  // mdcTheta - in rad.!
  // mdcPhi - in deg.!
  Double_t dPhi = pRich->getPhi() - mdcPhi - dPhRichIPUOff[(Int_t)sector];
  dPhi *= TMath::Sin(mdcTheta)/dPhRichIPU[(Int_t)sector];
  Double_t dTheta=pRich->getTheta()-mdcTheta*rad2deg;
  //dTheta=(dTheta-dThRichIPUOff[sector])/dThRichIPU[sector];
  qualTheta=dTheta;
  return sqrt(dPhi*dPhi);
}

void HMetaMatchF::setCurrentSector(Int_t sec) {
  sector=sec;
  sectorLoc.set(1,sector);
  secLabTrans=labTrans[(Int_t)sector];

  nShowerHitsSec=nShowerHits[sec];
  nTofHitsSec=nTofHits[sec];

//  showerHitsSec=showerHits[sec];
  indexShrHitSec=indexShrHit[sec];
  fShowerHitsSec=fShowerHits[sec];
//  showerModuleSec=showerModule[sec];

  tofHitsSec=tofHits[sec];
  indexTofHitSec=indexTofHit[sec];
  tofModuleSec=tofModule[sec];

  tofClustSizeSec=tofClustSize[sec];
}

Int_t HMetaMatchF::fillMeta(Int_t hShower, Int_t hTof,
    HMetaMatch* metaOld) {
  Int_t metaIndex=getMetaMatchSlot(metaOld);
  if(metaIndex>=0) {
    meta->setdxMeta(meta_dx);
    meta->setdyMeta(meta_dy);
    if(hShower>=0) {
      meta->setShowerHitInd(indexShrHitSec[hShower]);
      meta->setQualitySHOWER(qualShower);
      meta->setMdcSegCross(xSegCrShr,ySegCrShr);
    }
    if(hTof>=0) {
      meta->setTofHitInd(indexTofHitSec[hTof]);
      meta->setTofClusterSize(tofClustSizeSec[hTof]);
      meta->setQualityTOF(qualTof);
      meta->setMdcSegCross(xSegCrTof,ySegCrTof);
    }
  }
  return metaIndex;
}

void HMetaMatchF::fillTofClstInd(void) {
  for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) {
    if(nTrCndForTHit[nTof]<1) {
      Error("fillTofClstInd","num. of track cand. for tof hit <1 !");
      continue;
    }
    if(tofClustSizeSec[nTof]<2) continue;
    Int_t mmClstInd=mmTof[nTof][0];
    if(mmClstInd<0) {
      Error("fillTofClstInd","mmClstInd <0 !");
      continue;
    }
    HMetaMatch* meta=0;
    for(Int_t i=0;i<=tofClustSizeSec[nTof];i++) {
      if(nTof+i>=nTofHitsSec) {
        Error("fillTofClstInd","tof hit index out of range!");
        break;
      }
      Int_t mmInd=mmTof[nTof+i][0];
      if(mmInd<0) {
        Error("fillTofClstInd","mmInd <0 !");
        break;
      }
      if(meta) meta->setNextMMForTofHit(mmInd);
      meta=getMMObj(mmInd);
      if(meta==0) break;
      meta->setIndMMForTofClst(mmClstInd);
    }
  }
}

void HMetaMatchF::fillMetaHitsInd(void) {
  for(Int_t nTof=0; nTof<nTofHitsSec;nTof++) {
    Int_t& nTrCndForT=nTrCndForTHit[nTof];
    if(nTrCndForT<1) {
      Error("fillMetaHitsInd","num. of track cand. for tof hit <1 !");
      continue;
    }
    Int_t mmFirstInd=mmTof[nTof][0];
    HMetaMatch* meta=0;
    for(Int_t nTrCnd=0;nTrCnd<nTrCndForT;nTrCnd++) {
      meta=setMetaHitsInd(meta,mmFirstInd,mmTof[nTof][nTrCnd],nTrCndForT);
      if(meta==0) break;
    }
  }
  for(Int_t nSh=0; nSh<nShowerHitsSec;nSh++) {
    Int_t& nTrCndForS=nTrCndForSHit[nSh];
    if(nTrCndForS<1) {
      Error("fillMetaHitsInd","num. of track cand. for shower hit <1 !");
      continue;
    }
    Int_t mmFirstInd=mmShr[nSh][0];
    HMetaMatch* meta=0;
    for(Int_t nTrCnd=0;nTrCnd<nTrCndForS;nTrCnd++) {
      meta=setMetaHitsInd(meta,mmFirstInd,mmShr[nSh][nTrCnd],nTrCndForS);
      if(meta==0) break;
    }
  }
}

HMetaMatch* HMetaMatchF::setMetaHitsInd(HMetaMatch* meta, Int_t mmFirstInd,
    Int_t mmInd, Int_t totNTrCnd) {
  if(mmInd<0 || mmFirstInd<0) {
    Error("fillMetaHitsInd","mmInd or mmFistInd <0 !");
    return 0;
  }
  if(meta) meta->setNextMMForSameMeta(mmInd);
  HMetaMatch* metaNext=getMMObj(mmInd);
  if(metaNext==0) return 0;
  if(meta==0) metaNext->setNumMMForSameMeta(totNTrCnd);
  else metaNext->setNumMMForSameMeta(-1);
  metaNext->setFirstMMForSameMeta(mmFirstInd);
  return metaNext;
}

HMetaMatch* HMetaMatchF::getMMObj(Int_t ind) {
  HMetaMatch* meta=0;
  if(ind>=0) meta=(HMetaMatch*)(fCatMetaMatch->getObject(ind));
  if(meta==0) Error("getMMObj","no HMetaMatch object with index =%i !",ind);
  return meta;
}
