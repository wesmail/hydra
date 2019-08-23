#include "hmdc12fit.h"
#include "hdebug.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hmatrixcategory.h"
#include "hlocation.h"
#include <iostream.h>
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hmdchitclus.h"
#include "hmdcgetcontainers.h"
#include "hmdcclussim.h"
#include "hmdcdetector.h"
#include "hmdctrackfitterh.h"
#include "hmdctrackfittera.h"
#include "hmdctrackdset.h"
#include "hmdcclusfit.h"
#include "hmdcwirefit.h"

//*-- Author : V.Pechenov
//*-- Modified: 16.08.01 V.Pechenov
//*-- Modified: 17.10.00 A.Nekhaev

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
  if(trackFitter) {
    delete trackFitter;
    trackFitter=0;
  }
  HMdcTrackDSet::p12Fit=0;
}

void HMdc12Fit::setParContainers() {
  prntSt=kFALSE;
  HMdcTrackDSet::set12Fit(this);
  trackFitter=(HMdcTrackFitter*) ( (HMdcTrackDSet::fitAuthor!=1) ?
      new HMdcTrackFitterH() : new HMdcTrackFitterA() );

  trackFitter->setPrintFlag(HMdcTrackDSet::fprint);
  trackFitter->setVersion(HMdcTrackDSet::distTimeVer);
  fCalCat=0;
  fSegCat=0;
  fSizesCells=0;
  modForSeg[0]=1;
  modForSeg[1]=2;
  cutWeight=0.9;
  connectArrays();
}

void HMdc12Fit::connectArrays() {
  modIndx=trackFitter->getListModules();
  layIndx=trackFitter->getListLayers();
  cellIndx=trackFitter->getListCells();
  timeIndx=trackFitter->getTimeIndxArr();
  hitStatus=trackFitter->getHitStatusArr();
  tdcTime=trackFitter->getTDCTimeArr();
  qualityWeight=trackFitter->getQualityWeightArr();
  xLeft=trackFitter->getXLeftArr();
  yLeft=trackFitter->getYLeftArr();
  zLeft=trackFitter->getZLeftArr();
  xRight=trackFitter->getXRightArr();
  yRight=trackFitter->getYRightArr();
  zRight=trackFitter->getZRightArr();
  fSizeCellsLayer=trackFitter->getSizeCellsLayerArr();
  dev=trackFitter->getDeviationArr();
  distWire=trackFitter->getDistWireArr();
  weight=trackFitter->getWeightArr();
  inCell=trackFitter->getInCellFlag();
  alphaDrDist=trackFitter->getAlphaDrDistArr();
  fullTime=trackFitter->getFullTime();
}

void HMdc12Fit::setModForSeg(Int_t ms1, Int_t ms2) {
  // It's will used for independent track finder mode
  // ms1=0,1 - if track finder has independend mode and sector has
  //           2 modules the HMdcSeg will filled via hit in MDC1 or MDC2
  // ms1<0 -the HMdcSeg will not filled for or sectors!!!
  // ms2=2,3 - MDC3 or MDC4 ...
  modForSeg[0]=(ms1>1) ? 1:ms1;
  modForSeg[1]=(ms2>3) ? 3:ms1;
  if(modForSeg[1]>=0 && modForSeg[1]<2) modForSeg[1]=2;
}

Bool_t HMdc12Fit::init(void) {
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if (!fCalCat) return kFALSE;
  fClusCat=HMdcGetContainers::getCatMdcClus();
  if( !fClusCat ) return kFALSE;

  isGeant=HMdcGetContainers::isGeant();
  HMdcDetector* fMdcDet=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
  if(modForSeg[0]>=0 && modForSeg[1]>0) {
    fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if(!fSegCat) {
      if(isGeant) fSegCat = fMdcDet->buildMatrixCategory("HMdcSegSim",0.5F);
      else fSegCat = fMdcDet->buildCategory(catMdcSeg);
      if (!fSegCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
    }
  }

//   isCatHitClus=kTRUE;
//   fHitCat = HMdcGetContainers::getCatMdcHitClus(kTRUE);
//   if(!fHitCat) {
//     fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
//     if(!fHitCat) return kFALSE;
//     Warning("init",
//         " Category catMdcHit exist already, can't recreate it for HMdcHitClus");
//     isCatHitClus=kFALSE;
//   }  
  isCatHitClus=kFALSE;
  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    if(isGeant) fHitCat = fMdcDet->buildMatrixCategory("HMdcHitSim",0.5F);
    else fHitCat = fMdcDet->buildCategory(catMdcHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }

  iterClus=(HIterator *)fClusCat->MakeIterator();
  
  
  if(HMdcTrackDSet::fntuple) {
    fClusFitCat = HMdcGetContainers::getCatMdcClusFit(kTRUE);
    fWireFitCat = HMdcGetContainers::getCatMdcWireFit(kTRUE);
    if(!fClusFitCat || !fWireFitCat) return kFALSE;
  }
  locClusFit.set(1,0); 
  locWireFit.set(1,0);

  fSizesCells=HMdcSizesCells::getObject();
  if (!fSizesCells) {
    Error("init","HMdcSizesCells is absent");
    return kFALSE;
  }

  loc.set(4,0,0,0,0);
  fGetCont=HMdcGetContainers::getObject();
  fMdcDetector= fGetCont->getMdcDetector();
  if(!fMdcDetector) return kFALSE;
  locSeg.set(2,0,0);
  locHit.set(2,0,0);
  locClus.set(2,0,0);
  trackFitter->init();
  if(!prntSt) printStatus();
  return kTRUE;
}

Bool_t HMdc12Fit::reinit(void) {
  if(!fSizesCells->initContainer()) return kFALSE;
  for(Int_t sec=0; sec<6; sec++) {
    nModsSeg[sec][0]=nModsSeg[sec][1]=0;
    HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sec];
    for(Int_t mod=0; mod<4; mod++) {
      fSCModAr[sec][mod]=0;
      if(fMdcDetector->getModule(sec,mod)) {
        if(!&fSCSec) return kFALSE;
        fSCModAr[sec][mod]=&fSCSec[mod];
        if(!fSCModAr[sec][mod]) return kFALSE;
        mdcExist[sec][mod]=kTRUE;
        nModsSeg[sec][mod/2]++;
      } else mdcExist[sec][mod]=kFALSE;
    }
  }
  if(!trackFitter->reinit()) return kFALSE;
  return kTRUE;
}


Int_t HMdc12Fit::execute(void) {
  iterClus->Reset();
  Int_t maxNumSectors=((HMatrixCategory*)fClusCat)->getSize(0);
  Int_t maxNumSegments=((HMatrixCategory*)fClusCat)->getSize(1);
  for(locClus[0]=0; locClus[0]<maxNumSectors; locClus[0]++){
    for(locClus[1]=0; locClus[1]<maxNumSegments; locClus[1]++) {
      
//      iterClus->Reset();
      iterClus->gotoLocation(locClus);
      while((fClst=(HMdcClus*)iterClus->Next())) {
        if(!fClst->getStatus()) continue;
        //-----------------------------------------------------
        HMdcList12GroupCells listCellsOrg=*fClst;
        if(HMdcTrackDSet::fprint) fClst->print();
        if(!fillArrays(listCellsOrg)) continue;
        nMdcHits[0]=nMdcHits[1]=nMdcHits[2]=nMdcHits[3]=-1;
        nMdcSegs[0]=nMdcSegs[1]=0;        
        Int_t mod1=segment*2;
        Int_t mod2=mod1+1;
        Int_t endLoop=1;
        if(nMdcTime[0]==0) fittingMod=1;              // Second MDC only
        else if(nMdcTime[1]==0) fittingMod=endLoop=0; // First MDC only
        else {
          if(HMdcTrackDSet::fitType) fittingMod=0;    // Fit for each MDC
          else fittingMod=endLoop=2;                  // Fit two MDC
        }
        for(; fittingMod<=endLoop; fittingMod++) {
          Int_t mod=(fittingMod<2) ? fittingMod+mod1:-1;
          firstHit=firstTime[fittingMod];
          lastHit=(fittingMod==0) ? nMdcTime[0]:nClTimes;
          nFitedTimes=lastHit-firstHit;
          trackFitter->setFirstTimes(firstHit);
          trackFitter->setNTimes(lastHit);
          trackFitter->setNMDC1Times((fittingMod==1) ? 0:nMdcTime[0]);
          trackFitter->setNMDC2Times((fittingMod==0) ? 0:nMdcTime[1]);
          trackFitter->setParPrPlane(parA[fittingMod],parB[fittingMod],
              parD[fittingMod]);
          trackFitter->setXInitPlane(xPlane[fittingMod]);
          trackFitter->setYInitPlane(yPlane[fittingMod]);
          trackFitter->setZInitPlane(zPlane[fittingMod]);
          //                     MDC1(3)                      MDC2(4)  ???
          while(listCellsOrg.getNCells(0,5)>4||listCellsOrg.getNCells(6,11)>4) {
            listCells=listCellsOrg;
            //??? calcMinFunctional();  // Utochnenie nachal'nogo priblizheniya
            if(fitCluster(1000000000000000.)) { // (threshold???)
              if(mod<0) fillHitSegBySegFit();     // Segment fit
              else {                              // Module fit
                if(fillHitByFit(fittingMod)) {
                  if(nModsSeg[sector][segment]==1 || mod==modForSeg[segment])
                      fillSegByModFit(fittingMod);
                }
              }
              //???   unsetHits(listCellsOrg);
              break;
            } else {
              if(mod<0) {   // Seg. fit
                if(nMdcHits[mod1]<0 && nMdcHits[mod2]<0) fillHitSegByClus();
              } else {      // Mod. fit
                if(nMdcHits[mod]<0) {
                  if(fillHitByClus(fittingMod)) {
                    if(nModsSeg[sector][segment]==1 || mod==modForSeg[segment])
                      fillSegByModClus(fittingMod);
                  }
                }
              }
              break;
            }
          }
        }
        for(Int_t n1=0; n1<nMdcHits[mod1]; n1++) {
          for(Int_t n2=0; n2<nMdcHits[mod2]; n2++) fillSegBy2Hits(n1,n2);
        }
        //-----------------------------------------------------
      }
    }
  }
  return 0;
}

void HMdc12Fit::calcMinFunctional(void) {
//   Double_t xCl=fClst->getX();
//   Double_t yCl=fClst->getY();
//   Double_t errX=fClst->getErrX()*2.; // +/-dX
//   Double_t errY=fClst->getErrY()*2.; // +/-dX
//   Int_t nBinsX=TMath::Max(Int_t(errX*2./2.5),1);  //  / 2.5 mm
//   Int_t nBinsY=TMath::Max(Int_t(errY*2./1.0),1);  //  / 1.0 mm
//   if(nBinsX%2 == 0) nBinsX++;
//   if(nBinsY%2 == 0) nBinsY++;
//   Double_t stepX=errX/nBinsX;
//   Double_t stepY=errY/nBinsY;
//   Double_t xStart=xCl-nBinsX/2*stepX;
//   Double_t yStart=yCl-nBinsY/2*stepY;
//   Double_t minFunc=1.e+20;
// Double_t xMin=xCl;
// Double_t yMin=yCl;
// Printf("xCl=%f, yCl=%f x:%i %f  y:%i %f",xCl,yCl,nBinsX,stepX,nBinsY,stepY);
//   for(Int_t ny=0; ny<nBinsY; ny++) {
//     Double_t yNew=yStart+stepY*ny;
// printf("Y=%7.6g", yNew);
//     for(Int_t nx=0; nx<nBinsX; nx++) {
//       Double_t xNew=xStart+stepX*nx;
//       zPlane = fClst->getZ();
//       trackFitter->setXInitPlane(xNew);
//       trackFitter->setYInitPlane(yNew);
//       trackFitter->setZInitPlane(fClst->getZOnPlane(xNew,yNew));
//       Double_t func=trackFitter->getFunctional();
// printf(" %7.6g",func);
//       if(func<minFunc) {
//         minFunc=func;
//         xMin=xNew;
//         yMin=yNew;
//       }
//     }
// printf("\n");
//   }
// printf("..... xMin=%f, yMin=%f funMin=%f\n\n",xMin,yMin,minFunc);
// trackFitter->setXInitPlane(xMin);
// trackFitter->setYInitPlane(yMin);
// trackFitter->setZInitPlane(fClst->getZOnPlane(xMin,yMin));
// //   trackFitter->setXInitPlane(xPlane);
// //   trackFitter->setYInitPlane(yPlane);
// //   trackFitter->setZInitPlane(zPlane);
}

void HMdc12Fit::unsetHits(HMdcList12GroupCells& lCells) {
  for(Int_t lay=0;lay<12;lay++) {
    Int_t cell=-1;
    while((cell=listCells.next(lay,cell)) > -1) {
      Int_t time=listCells.getTime(lay,cell); // time=1||2, not 3 !!!
      // ??? check on weight ???
      if(time>0 && time<3) {
        hitStatus[hitIndex[lay][cell][time-1]]=0;
        lCells.delTime(lay,cell,time);
        nFitedTimes--;
      } else if(time == 3) {  //???
        hitStatus[hitIndex[lay][cell][0]]=0;
        hitStatus[hitIndex[lay][cell][1]]=0;
        lCells.delTime(lay,cell,3);
        nFitedTimes-=2;
      }
    }
  }
}

Bool_t HMdc12Fit::fillArrays(HMdcList12GroupCells& lCells) {
  Double_t xVertex, yVertex, zVertex;
  memset(hitIndex,-1,sizeof(hitIndex));

  parA[2] = fClst->A();
  parB[2] = fClst->B();
  parD[2] = fClst->D();
  xPlane[2] = fClst->getX();
  yPlane[2] = fClst->getY();
  zPlane[2] = fClst->getZ();
  xVertex = fClst->getXTarg();
  yVertex = fClst->getYTarg();
  zVertex = fClst->getZTarg();

  nMdcTime[0] = nMdcTime[1] = 0;
  nClTimes = 0;
  segment = fClst->getIOSeg();
  sector = fClst->getSec();

  loc[0] = sector;
  for(Int_t mod=0; mod<2; mod++) {       // relative number of module in segment
    loc[1] = mod  + segment*2;
    if(!mdcExist[sector][loc[1]]) continue;
    HMdcSizesCellsMod* fSCMod=fSCModAr[sector][loc[1]];
    fSCMod->getMdcPlanePar(parA[mod],parB[mod],parD[mod]);
    fSCMod->calcIntersection(xVertex,yVertex,zVertex,
        xPlane[2],yPlane[2],zPlane[2],xPlane[mod],yPlane[mod],zPlane[mod]);

    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
      Int_t nLayer=mod*6+loc[2];
      Int_t nWires=lCells.getNCells(nLayer); // num. cells in layer
      if(nWires<=0) continue;
      loc[3]=-1;
      while((loc[3]=lCells.next(nLayer,loc[3])) > -1) {
	Int_t nTimes=lCells.getTime(nLayer,loc[3]); // num. times for nWire
	HMdcCal1* cal=(HMdcCal1*)fCalCat->getObject(loc);
	if(!cal) {
          Error("fillArrays","S.%i M.%i L.%i  Cell %i is not fired",
              loc[0]+1,loc[1]+1,loc[2]+1,loc[3]+1);
          continue;
        }
        HMdcSizesCellsLayer& fSCLay=(*fSCMod)[loc[2]];
//        HMdcSizesCellsCell& fSCCell=fSCLay[loc[3]];

//         const HGeomVector* wP1=0;
//         const HGeomVector* wP2=0;
//         if(HMdcTrackDSet::distTimeVer==0) {
// 	  wP1 = fSCCell.getWirePoint(0);
// 	  wP2 = fSCCell.getWirePoint(1);
//         }
//---------------------------------------------------------------
        for(Int_t time=1; time<3; time++) {
          if((time&nTimes) == 0) continue;
          if(time==1) tdcTime[nClTimes] = cal->getTime1();
          else {
//            if(isGeant && cal->getTime2() - cal->getTime1() <= 50.) {
//              lCells.delTime(loc[2]+mod*2,loc[3],2); //???
//              continue;
//            }
            tdcTime[nClTimes] = cal->getTime2();
          }
          nMdcTime[mod]++;
          fSizeCellsLayer[nClTimes]=&fSCLay;
          modIndx[nClTimes]=loc[1];
          layIndx[nClTimes]=loc[2];
          cellIndx[nClTimes]=loc[3];
          timeIndx[nClTimes]=time;
          hitIndex[mod*6+loc[2]][loc[3]][time-1]=nClTimes;

          qualityWeight[nClTimes] = 1.; // /nWires;
//          if(HMdcTrackDSet::distTimeVer==0) {
//            xLeft [nClTimes] = wP1->getX();
//            xRight[nClTimes] = wP2->getX();
//            yLeft [nClTimes] = wP1->getY();
//            yRight[nClTimes] = wP2->getY();
//            zLeft [nClTimes] = wP1->getZ();
//            zRight[nClTimes] = wP2->getZ();
//          }
          hitStatus[nClTimes] = 1;

          nClTimes++;
        }
//---------------------------------------------------------------
      }  // end of hit loop
    }    // end of layer loop
  }      // end of MDC loop
  if(nClTimes <=0) return kFALSE;

  firstTime[0]=firstTime[3]=0;
  firstTime[1]=nMdcTime[0];

  trackFitter->setAddress(sector,segment);

  trackFitter->setXVertex(xVertex);
  trackFitter->setYVertex(yVertex);
  trackFitter->setZVertex(zVertex);

//  Not used yet ----------------------
//   HGeomVector vtmp;
//
//   HGeomVector trP1(0., 0., 0.);
//   HGeomVector trP2(xPlane, yPlane, zPlane);
//
//   HGeomVector pointCros1, pointCros2;
//   HGeomVector pointCros3, pointCros4;
//   Double_t dist1, dist2, dist3, dist4;
//
//   if(&(fSCSec[0])) {
//     calcCross( &trP1, &trP2, fSCSec[0].getSecTrans(), &pointCros1);
//     dist1 = sqrt(pointCros1.getX()*pointCros1.getX()+pointCros1.getY()*
// 		 pointCros1.getY()+pointCros1.getZ()*pointCros1.getZ());
//   }
//   else dist1 = 0;
//
//   if(&(fSCSec[1])) {
//     calcCross( &trP1, &trP2, fSCSec[1].getSecTrans(), &pointCros2);
//     dist2 =  sqrt(pointCros2.getX()*pointCros2.getX()+pointCros2.getY()*
// 		  pointCros2.getY()+pointCros2.getZ()*pointCros2.getZ());
//   }
//   else dist2 = 0;
//
//   if(&(fSCSec[2])) {
//     calcCross( &trP1, &trP2, fSCSec[2].getSecTrans(), &pointCros3);
//     dist3 =  sqrt(pointCros3.getX()*pointCros3.getX()+pointCros3.getY()*
// 		  pointCros3.getY()+pointCros3.getZ()*pointCros3.getZ());
//   }
//   else dist3 = 0;
//
//   if(&(fSCSec[3])) {
//     calcCross( &trP1, &trP2, fSCSec[3].getSecTrans(), &pointCros4);
//     dist4 =  sqrt(pointCros4.getX()*pointCros4.getX()+pointCros4.getY()*
// 		  pointCros4.getY()+pointCros4.getZ()*pointCros4.getZ());
//   }
//   else dist4 = 0;
//
//   trackFitter->setDistVertexMDC1(dist1);
//   trackFitter->setDistVertexMDC2(dist2);
//   trackFitter->setDistVertexMDC3(dist3);
//   trackFitter->setDistVertexMDC4(dist4);
// --------------------------

  return kTRUE;
}

Bool_t HMdc12Fit::fitCluster(Double_t threshold) {
  Int_t iter=0;
  Int_t nCellM1=0;
  Int_t nCellM2=0;
  for(Int_t n=firstHit; n<lastHit; n++) if(hitStatus[n]<0) hitStatus[n]=1;
  while(kTRUE) {
    if(trackFitter->execute(threshold,iter++)==0) {
      // fill cl ?????????????????????????????????????????
      return kFALSE;
    }
    Double_t delW=testFitResult();
    nCellM1=(fittingMod!=1) ? listCells.getNCells(0,5) : 0;
    nCellM2=(fittingMod!=0) ? listCells.getNCells(6,11) : 0;
    if( delW<0.5 || (nCellM1<5 && nCellM2<5) ) break;
    if(HMdcTrackDSet::fprint) Printf(
      "\n Sum.of weights of deleted times=%f>0.5, start new fit for this cluster! \n"
      ,delW);
  }
  if(nCellM1+nCellM2 < 5) return kFALSE; // ????????????????
  if(HMdcTrackDSet::fntuple) fillFitCat();
//  fillMdcHitSeg();
  return kTRUE;
}

Double_t HMdc12Fit::testFitResult() {
  // Selection of hits (needs in checking ???).
  Double_t sumDelWeight=0.;
  for(int hit=firstHit;hit<lastHit;hit++) {
    if(hitStatus[hit] != 1) continue;
    Int_t mod=modIndx[hit]%2;
    Int_t lay=layIndx[hit];
    Int_t cell=cellIndx[hit];
    Int_t time=timeIndx[hit];
    // Test for qualityWeight:
    if(qualityWeight[hit]<.01) { // Hits sel. by weight
      // new fit ???
      listCells.delTime(mod*6+lay,cell,time);
      hitStatus[hit]=-1;
      continue;
    }
    // Test for Tukey wiht:
    if(weight[hit]<cutWeight) {
      listCells.delTime(mod*6+lay,cell,time);
      hitStatus[hit]=-1;
      sumDelWeight+=weight[hit]; // +=weight[dHit]*qual.W[dHit] ?
      continue;
    }
    //Test on double hit:
    // time==2 - weights and qualityWeights are tested already
    if(time==2 && listCells.getTime(mod*6+lay,cell)==3) {  // Double hit ...
      // new fit ???
      Double_t dev1=fabs(dev[hit-1]);
      Double_t dev2=fabs(dev[hit]);
      Int_t dHit=(dev1 > dev2) ? hit-1:hit;
//       Int_t dHit=(weigh[hit] < weight[hit+1]) ? hit:hit+1;
      listCells.delTime(mod*6+lay,cell,timeIndx[dHit]);
      sumDelWeight+=1.;  // +=weight[dHit]*qualityWeight[dHit] ???
      hitStatus[dHit]=-1;
    }
  }
  // Test for layers which have > 2 wires:
  Int_t l1=(fittingMod!=1) ? 0:6;
  Int_t l2=(fittingMod==0) ? 6:12;
  for(Int_t lay=l1;lay<l2;lay++) {
    while(listCells.getLastCell(lay)-listCells.getFirstCell(lay)>1) {
      // new fit i mozhet vikid. po odnoy, maksimal'noy ???
      Int_t fCell=listCells.getFirstCell(lay);
      Int_t lCell=listCells.getLastCell(lay);
      if(HMdcTrackDSet::fprint) Printf(
        "Region of fired wires in layer %i: cells %i - %i > 2",lay,lCell,fCell);
      Int_t t1=listCells.getTime(lay,fCell);
      Int_t t2=listCells.getTime(lay,lCell);
      Int_t hit1=hitIndex[lay][fCell][t1-1];
      Int_t hit2=hitIndex[lay][lCell][t2-1];
      Double_t dev1=fabs(dev[hit1]);
      Double_t dev2=fabs(dev[hit2]);
      Int_t dHit=(dev1 > dev2) ? hit1:hit2;
//       Int_t dHit=(weigh[hit1] < weight[hit2]) ? hit2:hit1;
      listCells.delTime(lay,fCell,timeIndx[dHit]);  // new fit ???
      sumDelWeight+=1.;   // +=weight[dHit]*qualityWeight[dHit] ???
      hitStatus[dHit]=-1;
    }
  }
  return sumDelWeight;
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
    Int_t fmod) {
  // For HMdcSeg: setGroup(fClst,fSeg);
  // For HMdcHit: setGroup(fClst,fSeg,mod);  mod=0 or 1 !
  Int_t lay1,lay2;
  if(fmod<0) {
    lay1=0;
    lay2=12;
  } else {
    lay1=(fmod%2)*6;
    lay2=lay1+6;
  }
  Int_t t[4];
  for(Int_t layer=lay1; layer<lay2; layer++) {
    Int_t cell=lCl->get4FirstCells(layer,t);
    if(cell>=0) lHit->setSignId(layer-lay1,cell,t[0],t[1],t[2],t[3]);
  }
}

Bool_t HMdc12Fit::fillHitByFit(Int_t iMod) {
  Int_t mod=iMod+segment*2;
  HMdcSizesCellsMod* fSCMod=fSCModAr[sector][mod];
  if(!fSCMod) return kFALSE;
  HMdcHit* fHit = getHitSlot(mod);
  if(!fHit) return kFALSE;
  isHitFitted[mod][nMdcHits[mod]-1]=kTRUE; // must be after getHitSlot(mod)
  setGroup(&listCells, fHit,iMod);
  Double_t x,eX,y,eY,xDir,eXDir,yDir,eYDir;
  eX=eY=eXDir=eYDir=0;  
  fSCMod->calcMdcHit(trackFitter->getXOutVertex(),trackFitter->getYOutVertex(),
      trackFitter->getZOutVertex(),trackFitter->getXOutPlane(),
      trackFitter->getYOutPlane(),trackFitter->getZOutPlane(), x,y,xDir,yDir); 
//   Double_t dZ1dX1, dZ1dY1,zErrTag; 
//   fSCMod->calcMdcHit(trackFitter->getXOutVertex(),trackFitter->getYOutVertex(),
//       trackFitter->getZOutVertex(),trackFitter->getXOutPlane(),
//       trackFitter->getYOutPlane(),trackFitter->getZOutPlane(),
//       trackFitter->getErrXTarg(),trackFitter->getErrYTarg(),zErrTag,
//       trackFitter->getErrX(),trackFitter->getErrY(),
//       dZ1dX1, dZ1dY1, -fClst->A()??, -fClst->B()??,
//       x,eX, y,eY, xDir,eXDir, yDir,eYDir);
  fHit->setXY(x,eX,y,eY);
  fHit->setXYDir(xDir,eXDir,yDir,eYDir);
  fHit->setChi2(trackFitter->getFunMin());   // /deg.of_fr.num. ???
  fHit->setOff(trackFitter->getArriveTime(),0.);  // Error ?
  if(isGeant) ((HMdcHitSim*)fHit)->calcNTracks();
  fillClusInf(fHit,iMod,kTRUE); // Cluster inform.
  return kTRUE;
}

void HMdc12Fit::fillClusInf(HMdcHit* fHit, Int_t iMod, Bool_t fitSt) {
  if(isCatHitClus) {
    HMdcClusInf* inf=(isGeant) ? ((HMdcHitSimClus*)fHit)->getClusInf() :
                                 ((HMdcHitClus*)   fHit)->getClusInf();
    inf->setFitStatus(fitSt);
    inf->setClustSize(fClst->getNBins());
    inf->setNMergClust(fClst->getNMergClust());
    inf->setNDrTimes(fClst->getNDrTimes(iMod*6,iMod*6+5));
    // inf->setShapePar(p1,p2);
  } else {
    Int_t nMrCl = fClst->getNMergClust();
    if(nMrCl>99) nMrCl=99;
    Int_t flag=(fClst->getNBins()&32767) <<16;
    flag += (fClst->getNDrTimes(iMod*6,iMod*6+5)*100 + nMrCl) & 65535;
    fHit->setFlag(flag);
    if(!fitSt) fHit->setChi2(-1.);
  }  
}

void HMdc12Fit::fillClusInf(HMdcSeg* fSeg, Bool_t fitSt) {
  Int_t nMrCl = fClst->getNMergClust();
    if(nMrCl>99) nMrCl=99;
    Int_t flag=(fClst->getNBins()&32767) <<16;
    flag += (fClst->getNDrTimes()*100 + nMrCl) & 65535;
    fSeg->setFlag(flag);
    if(!fitSt) fSeg->setChi2(-1.);
}

Bool_t HMdc12Fit::fillHitSegBySegFit() {
  Int_t mod1=segment*2;
  Int_t mod2=mod1+1;  
  Int_t indx1=(fillHitByFit(mod1&1)) ? --nMdcHits[mod1] : -1;
  Int_t indx2=(fillHitByFit(mod2&1)) ? --nMdcHits[mod2] : -1;
  HMdcSeg* fSeg = getSegSlot();
  if(!fSeg) return kFALSE; 
  setGroup(&listCells, fSeg);
  Double_t theta, phi, zm, r0;
  Double_t eZm, eR0, eTheta, ePhi;
  eZm=eR0=eTheta=ePhi=0.;
  HMdcSizesCells::calcMdcSeg(trackFitter->getXOutVertex(),
      trackFitter->getYOutVertex(),trackFitter->getZOutVertex(),
      trackFitter->getXOutPlane(),trackFitter->getYOutPlane(),
      trackFitter->getZOutPlane(),zm,r0,theta,phi);
//   Double_t eTheta,ePhi,eZm,eR0;
//   Double_t dZ1dX1, dZ1dY1,zErrTag;
//   HMdcSizesCells::calcMdcSeg(trackFitter->getXOutVertex(),
//       trackFitter->getYOutVertex(),trackFitter->getZOutVertex(),
//       trackFitter->getXOutPlane(),trackFitter->getYOutPlane(),
//       trackFitter->getZOutPlane(),???,???,???,???,???,???,
//       ???,???,???,???,
//       zm,eZm, r0,eR0,theta,eTheta, phi,ePhi);
  fSeg->setPar(zm, eZm, r0, eR0, theta, eTheta, phi, ePhi);
  if(indx1>=0) fSeg->setHitInd(0,indx1);
  if(indx2>=0) fSeg->setHitInd(1,indx2);
  fSeg->setChi2(trackFitter->getFunMin());
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
  // Cluster param:
  fillClusInf(fSeg,kTRUE);
  return kTRUE; 
}

Bool_t HMdc12Fit::fillSegBy2Hits(Int_t nHit1, Int_t nHit2) {
  Int_t mod1=segment*2;
  Int_t mod2=mod1+1;
  HMdcSizesCellsMod* fSCMod1=fSCModAr[sector][mod1];
  HMdcSizesCellsMod* fSCMod2=fSCModAr[sector][mod2];
  if(!fSCMod1 || !fSCMod2) return kFALSE;
  HMdcHit* h1=fMdcHit[mod1][nHit1];
  HMdcHit* h2=fMdcHit[mod2][nHit1];
  Float_t x1,y1,z1,ex1,ey1;
  Float_t x2,y2,z2,ex2,ey2;
  h1->getXY(x1,ex1,y1,ey1);
  h2->getXY(x2,ex2,y2,ey2);
  fSCMod1->transFromZ0(x1,y1,z1);
  fSCMod2->transFromZ0(x2,y2,z2);
  ey1 /= sqrt(1.+parB[0]*parB[0]); // simple error recalculation (let
  ey2 /= sqrt(1.+parB[1]*parB[1]); // eX_sec=eX_mod,...)
  HMdcSeg* fSeg = getSegSlot();
  if(!fSeg) return kFALSE;
  for(Int_t l=0; l<6; l++) {
    if(h1->getNCells(l)>0) fSeg->setSignId(  l,h1->getCell(l,0),
      h1->getSignId(l,0),h1->getSignId(l,1),h1->getSignId(l,2),
      h1->getSignId(l,3));
    if(h2->getNCells(l)>0) fSeg->setSignId(6+l,h2->getCell(l,0),
      h2->getSignId(l,0),h2->getSignId(l,1),h2->getSignId(l,2),
      h2->getSignId(l,3));
  }
  Double_t zm, r0, theta, phi;
  Double_t eZm, eR0, eTheta, ePhi;
  // HMdcSizesCells::calcMdcSeg(x1,y1,z1,x2,y2,z2, zm,r0,theta,phi);
  HMdcSizesCells::calcMdcSeg(x1,y1,z1,x2,y2,z2,ex1,ey1,0.,ex2,ey2,
      -parA[0],-parB[0],-parA[1],-parB[1],
      zm,eZm, r0,eR0,theta,eTheta, phi,ePhi); 
  fSeg->setPar(zm, eZm, r0, eR0, theta, eTheta, phi, ePhi);
  fSeg->setHitInd(0,indexHit[mod1][nHit1]);
  fSeg->setHitInd(1,indexHit[mod2][nHit2]);
  // fSeg->setChi2(trackFitter->getFunMin());  ???
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
  
  // Cluster param:
  //fSeg->setFlag(fClst->getNDrTimes()*100 + fClst->getNMergClust());
  //fSeg->setChi2(fClst->getNBins());  // NBins in clust. --> chi2 in Seg.
  return kTRUE; 
}

Bool_t HMdc12Fit::fillSegByModFit(Int_t iMod) {
  Int_t mod=segment*2+iMod;
  nMdcHits[mod]--;             // hit exist and should be excl. from list
  HMdcSeg* fSeg = getSegSlot();
  if(!fSeg) return kFALSE;
  Int_t nHit=nMdcHits[mod];    // last hit!
  setGroup(&listCells, fSeg);
  Double_t theta, phi, zm, r0;
  Double_t eZm, eR0, eTheta, ePhi;
  eZm=eR0=eTheta=ePhi=0.;
  HMdcSizesCells::calcMdcSeg(trackFitter->getXOutVertex(),
      trackFitter->getYOutVertex(),trackFitter->getZOutVertex(),
      trackFitter->getXOutPlane(),trackFitter->getYOutPlane(),
      trackFitter->getZOutPlane(),zm,r0,theta,phi);
//   Double_t eTheta,ePhi,eZm,eR0;
//   Double_t dZ1dX1, dZ1dY1,zErrTag;
//   HMdcSizesCells::calcMdcSeg(trackFitter->getXOutVertex(),
//       trackFitter->getYOutVertex(),trackFitter->getZOutVertex(),
//       trackFitter->getXOutPlane(),trackFitter->getYOutPlane(),
//       trackFitter->getZOutPlane(),???,???,???,???,???,???,
//       ???,???,???,???,
//       zm,eZm, r0,eR0,theta,eTheta, phi,ePhi);
  fSeg->setPar(zm, eZm, r0, eR0, theta, eTheta, phi, ePhi);
  fSeg->setHitInd(iMod,indexHit[mod][nHit]);
  fSeg->setChi2(trackFitter->getFunMin());
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();
  // Cluster param:
  fillClusInf(fSeg,kTRUE);
  return kTRUE;
}

// void HMdc12Fit::calcCross( HGeomVector* vc1, HGeomVector* vc2,
//             const HGeomTransform* trans, HGeomVector* vcNew){
// // Calculeting a cross of line (vc1-vc2) with plane HGeomTransform
//   Double_t x1,x2,y1,y2,z1,z2;
//   x1=vc1->getX();   x2=vc2->getX();
//   y1=vc1->getY();   y2=vc2->getY();
//   z1=vc1->getZ();   z2=vc2->getZ();
//   Double_t A,B,C,D;
//   A=(trans->getTransVector()).getX();
//   B=(trans->getTransVector()).getY();
//   C=(trans->getTransVector()).getZ();
//   D=-(A*A+B*B+C*C);
//   Double_t point[3];
//   point[0]=(x1/(x1-x2)*(B*(y1-y2)+C*(z1-z2))-B*y1-C*z1-D)/
//            (A+(B*(y1-y2)+C*(z1-z2))/(x1-x2));
//   point[1]=(point[0]-x1)*(y1-y2)/(x1-x2)+y1;
//   point[2]=(-A*point[0]-B*point[1]-D)/C;
//   vcNew->setVector(point);
// }

Bool_t HMdc12Fit::finalize(void) {
  return kTRUE;
}

void HMdc12Fit::fillFitCat() {
  if(lastHit-firstHit <1) return;
  Int_t indClusFit;   
  HMdcClusFit *fClusFit=(HMdcClusFit*)fClusFitCat->
      getNewSlot(locClusFit,&indClusFit);
//Printf("indClusFit=%i",indClusFit);
  if(!fClusFit) {
    Error("fillFitCat","No slot HMdcClusFit available"); 
    return;
  }
  fClusFit = new(fClusFit) HMdcClusFit;
  
  fClusFit->setSec(sector);
  fClusFit->setIOSeg(segment);
  fClusFit->setFitAuthor(HMdcTrackDSet::fitAuthor);
  fClusFit->setDistTimeVer(HMdcTrackDSet::distTimeVer);
  fClusFit->setFitType(HMdcTrackDSet::fitType);
  fClusFit->setNParam(trackFitter->getNumOfParam());
  fClusFit->setFunMin(trackFitter->getFunMin());
  Double_t arrTm=trackFitter->getArriveTime();
  fClusFit->setTimeOff(arrTm);
  fClusFit->setX1(trackFitter->getXOutVertex());
  fClusFit->setY1(trackFitter->getYOutVertex());
  fClusFit->setZ1(trackFitter->getZOutVertex());
  fClusFit->setX2(trackFitter->getXOutPlane());
  fClusFit->setY2(trackFitter->getYOutPlane());
  fClusFit->setZ2(trackFitter->getZOutPlane());
  fClusFit->setNumIter(trackFitter->getIterationNumber());
  fClusFit->setClustIndex(fClst->getIndex());
  
  Int_t nGWeight = 0;
  Int_t indWireFit;
  Int_t nmdc[4];
  Int_t nLayers=0;
  Int_t lay=-1;
  nmdc[0]=nmdc[1]=nmdc[2]=nmdc[3]=0;
  for(Int_t hit=firstHit; hit<lastHit; hit++) {
    if(hitStatus[hit] == 0) continue;  // this hit was fited befor it (dubl.tr.)
    HMdcWireFit *fWireFit=(HMdcWireFit*)fWireFitCat->
        getNewSlot(locWireFit,&indWireFit);
    if(!fWireFit) {
      Error("fillFitCat","No slot HMdcWireFit available"); 
      return;
    }
    fWireFit = new(fWireFit) HMdcWireFit;
    if(hit==firstHit) fClusFit->setIndf(indWireFit);
    nmdc[modIndx[hit]]++;
    if(hitStatus[hit]==1 && weight[hit]>0.01) {  // 0.01 ???
      nGWeight++;
      fWireFit->setWeight( weight[hit]);
      if(lay != layIndx[hit]) {
        lay=layIndx[hit];
        nLayers++;
      }
    } else fWireFit->setWeight(0.);
    fWireFit->setAdsress(sector, modIndx[hit], layIndx[hit], cellIndx[hit]);
    fWireFit->setTimeNum(timeIndx[hit]);
    fWireFit->setDev(dev[hit]);
    fWireFit->setWeight(
        (hitStatus[hit]<0 || weight[hit]<0.001) ? 0. : weight[hit]); //0.001 ???
    fWireFit->setDriftTime(fullTime[hit]-arrTm);
    fWireFit->setFullTime(fullTime[hit]);
    fWireFit->setTdcTime(tdcTime[hit]);
    fWireFit->setMinDist(distWire[hit]);
    fWireFit->setAlpha(alphaDrDist[hit]);
    fWireFit->setIsInCell(inCell[hit]);
  }
  fClusFit->setIndl(indWireFit);
  fClusFit->setNumOfWires(nGWeight);
  fClusFit->setNumOfLayers(nLayers);
  Int_t mod=-2;
  for(Int_t m=0;m<4;m++) {
    if(nmdc[m]==0) continue;
    if(mod==-2) mod=m;
    else mod=-1;
  }
  fClusFit->setMod(mod);
}

Bool_t HMdc12Fit::fillHitByClus(Int_t iMod) {
  Int_t mod=iMod+segment*2;
  HMdcSizesCellsMod* fSCMod=fSCModAr[sector][mod];
  if(!fSCMod) return kFALSE;
  HMdcHit* fHit = getHitSlot(mod);
  if(!fHit) return kFALSE;
  isHitFitted[mod][nMdcHits[mod]]=kFALSE;  // must be after getHitSlot(mod)
  setGroup(&listCells, fHit,iMod);
  Double_t dZ1dX1=0., dZ1dY1=0.,zErrTag=0.;
  if(mod<2 || HMdcTrackDSet::isCoiloff) zErrTag=fClst->getErrZTarg();
  else {           // kick plane
    dZ1dX1=0.;
    dZ1dY1=fClst->getErrYTarg();
    dZ1dY1=(dZ1dY1>0.0001) ? -fClst->getErrZTarg()/dZ1dY1 : -1.274;
  }
  Double_t x,eX,y,eY,xDir,eXDir,yDir,eYDir;
  fSCMod->calcMdcHit(fClst->getXTarg(),fClst->getYTarg(),fClst->getZTarg(),
      fClst->getX(),fClst->getY(),fClst->getZ(),fClst->getErrXTarg(),
      fClst->getErrYTarg(),zErrTag,fClst->getErrX(),fClst->getErrY(),
      dZ1dX1, dZ1dY1, -fClst->A(), -fClst->B(),
      x,eX, y,eY, xDir,eXDir, yDir,eYDir);
  fHit->setXY(x,eX,y,eY);
  fHit->setXYDir(xDir,eXDir,yDir,eYDir);
  if(isGeant) ((HMdcHitSim*)fHit)->calcNTracks();
  fillClusInf(fHit,iMod,kFALSE); // Cluster inform.
  return kTRUE;
}

Bool_t HMdc12Fit::fillSegByModClus(Int_t iMod) {
  Int_t mod=segment*2+iMod;
  HMdcSeg* fSeg = getSegSlot();
  nMdcHits[mod]--;             // hit exist and should be excl. from list
  if(!fSeg) return kFALSE;
  Int_t nHit=nMdcHits[mod];   // last hit!
  setGroup(&listCells, fSeg);
  Double_t dZ1dX1=0., dZ1dY1=0.,zErrTag=0.;
  if(segment==0 || HMdcTrackDSet::isCoiloff) zErrTag=fClst->getErrZTarg();
  else {                                  // kick plane
    dZ1dX1=0.;
    dZ1dY1=fClst->getErrYTarg();
    dZ1dY1=(dZ1dY1>0.0001) ? -fClst->getErrZTarg()/dZ1dY1 : -1.274;
  }
  Double_t theta, phi, zm, r0;
  Double_t eTheta,ePhi,eZm,eR0;
  HMdcSizesCells::calcMdcSeg(fClst->getXTarg(),fClst->getYTarg(),
      fClst->getZTarg(),fClst->getX(),fClst->getY(),fClst->getZ(),
      fClst->getErrXTarg(),fClst->getErrYTarg(),zErrTag,fClst->getErrX(),
      fClst->getErrY(),dZ1dX1, dZ1dY1, -fClst->A(), -fClst->B(),
      zm,eZm, r0,eR0,theta,eTheta, phi,ePhi);
  fSeg->setPar(zm,eZm, r0,eR0, theta,eTheta, phi,ePhi);
  fSeg->setHitInd(iMod,indexHit[mod][nHit]);
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();  
  // Cluster param:
  fillClusInf(fSeg,kFALSE);
  return kTRUE;
}

Bool_t HMdc12Fit::fillHitSegByClus(void) {
  Int_t mod1=segment*2;
  Int_t mod2=mod1+1;
  Int_t indx1=(fillHitByClus(mod1&1)) ? --nMdcHits[mod1] : -1;
  Int_t indx2=(fillHitByClus(mod2&1)) ? --nMdcHits[mod2] : -1;
  HMdcSeg* fSeg = getSegSlot();
  if(!fSeg) return kFALSE;
  setGroup(&listCells, fSeg);
  Double_t dZ1dX1=0., dZ1dY1=0.,zErrTag=0.;
  if(segment==0 || HMdcTrackDSet::isCoiloff) zErrTag=fClst->getErrZTarg();
  else {                                  // kick plane
    dZ1dX1=0.;
    dZ1dY1=fClst->getErrYTarg();
    dZ1dY1=(dZ1dY1>0.0001) ? -fClst->getErrZTarg()/dZ1dY1 : -1.274;
  }
  Double_t theta, phi, zm, r0;
  Double_t eTheta,ePhi,eZm,eR0;
  HMdcSizesCells::calcMdcSeg(fClst->getXTarg(),fClst->getYTarg(),
      fClst->getZTarg(),fClst->getX(),fClst->getY(),fClst->getZ(),
      fClst->getErrXTarg(),fClst->getErrYTarg(),zErrTag,fClst->getErrX(),
      fClst->getErrY(),dZ1dX1, dZ1dY1, -fClst->A(), -fClst->B(),
      zm,eZm, r0,eR0,theta,eTheta, phi,ePhi);
  fSeg->setPar(zm,eZm, r0,eR0, theta,eTheta, phi,ePhi);
  if(indx1>=0) fSeg->setHitInd(0,indexHit[mod1][indx1]);
  if(indx2>=0) fSeg->setHitInd(1,indexHit[mod2][indx2]);
  if(isGeant) ((HMdcSegSim*)fSeg)->calcNTracks();  
  // Cluster param:
  fillClusInf(fSeg,kFALSE);
  return kTRUE;
}

HMdcHit* HMdc12Fit::getHitSlot(Int_t mod) {
  // Geting a slot and setting address  
  if(nMdcHits[mod]>99) {
    Error("getHitSlot","Num. of HMdcHit cont.>100 from one cluster.");
    return 0;
  }
  if(nMdcHits[mod]<0) nMdcHits[mod]=0;
  locHit[0]=sector;
  locHit[1]=mod;
  Int_t index;
  HMdcHit* fHit = (HMdcHit*)fHitCat->getNewSlot(locHit,&index);
  if(!fHit) {
    Error("getHitSlot"," No slot in catMdcHit available");
    return 0;  
  }
  if(!isCatHitClus) fHit=(isGeant) ? new(fHit) HMdcHitSim : new(fHit) HMdcHit;
  else fHit=(isGeant) ? new(fHit) HMdcHitSimClus : new(fHit) HMdcHitClus;
  fHit->setSecMod(sector,mod);
  fMdcHit[mod][nMdcHits[mod]]=fHit;
  indexHit[mod][nMdcHits[mod]]=index;
  nMdcHits[mod]++;
  return fHit;
}


HMdcSeg* HMdc12Fit::getSegSlot(void) {
  // Geting a slot and setting address
  if(nMdcSegs[segment]>99) {
    Error("getSegSlot","Num. of HMdcSeg cont.>100 from one cluster.");
    return 0;
  }
  locSeg[0]=sector;  
  locSeg[1]=segment;
  Int_t index;
  HMdcSeg* fSeg = (HMdcSeg*)fSegCat->getNewSlot(locSeg,&index);
  if(!fSeg) {
    Error("getSegSlot"," No slot in catMdcSeg available");
    return 0;  
  }
  fSeg = (isGeant) ? (HMdcSeg*)(new(fSeg) HMdcSegSim) : new(fSeg) HMdcSeg;
  fSeg->setSec(sector);
  fSeg->setIOSeg(segment);
  fMdcSeg[segment][nMdcSegs[segment]]=fSeg;
  indexSeg[segment][nMdcSegs[segment]]=index;
  nMdcSegs[segment]++;
  return fSeg;
}

void HMdc12Fit::printStatus(void) {
  // prints the parameters to the screen
  char* hAuth ="0 = Hejdar,  1 = Alexander";
  char* hTmVer="0 = polynomial,  1 = HMdcCal2ParSim::calcTime";
  char* hFType="0 = segment fit,  1 = independent mdc fit";
  char* hDCat ="1 = create and fill catMdcClusFit&catMdcWireFit 0 = don't cr.";
  char* hDPr  ="1 = print debug information, 0 = don't print";
  printf("--------------------------------------------------------------------------------------------\n");
  printf("HMdc12FitSetup:\n");
  printf("FitAuthor       =  %i :  %s\n",HMdcTrackDSet::fitAuthor,hAuth);
  printf("DrTimeCalcVers  =  %i :  %s\n",HMdcTrackDSet::distTimeVer,hTmVer);
  printf("FitType         =  %i :  %s\n",HMdcTrackDSet::fitType,hFType);
  printf("DebugCategorie  =  %i :  %s\n",HMdcTrackDSet::fntuple,hDCat);
  printf("DebugPrint      =  %i :  %s\n",HMdcTrackDSet::fprint,hDPr);
  printf ("--------------------------------------------------------------------------------------------\n");
  prntSt=kTRUE;
}

ClassImp(HMdc12Fit)

