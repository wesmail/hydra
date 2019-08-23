using namespace std;
#include "hmdctrackfitter.h"
#include "hdebug.h"
#include "hades.h"
//#include <iostream>
//#include <iomanip>
#include "TStopwatch.h"
#include "TMath.h"
#include "hmdctrackfitpar.h"
#include "hruntimedb.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdccal2parsim.h"
#include "hmdccal2par.h"
#include "hmdcgetcontainers.h"
#include "hmdclistcells.h"
#include "hmdcwirefit.h"
#include "hmdcclusfit.h"
#include "hmdcclus.h"
#include "hmdctrackdset.h"
#include "hmdcdigitpar.h"

//*-- Author : G.Agakichiev
//*-- Modified: 21.07.2003 by V.Pechenov
//*-- Modified: 28.07.2002 by V.Pechenov
//*-- Modified: 07.05.2002 by V.Pechenov

ClassImp(HMdcTrackFitInOut)
ClassImp(HMdcTrackFitter)

Bool_t HMdcTrackFitInOut::init(void) {
  // parameters:
  isGeant=HMdcGetContainers::isGeant();
  version=HMdcTrackDSet::getFitVersion();
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->
      getContainer("MdcTrackFitPar");
  if(version != 0) {
    fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->
        getContainer("MdcCal2ParSim");
    fCal2Par=(HMdcCal2Par*)gHades->getRuntimeDb()->
        getContainer("MdcCal2Par");
    if(!fCal2ParSim || !fCal2Par) return kFALSE;
  } else {
    fCal2ParSim=0;
    fCal2Par=0;
  }
  useWireOffset=HMdcTrackDSet::getUseWireOffset();
  if(useWireOffset) {
    fDigitPar=(HMdcDigitPar*)gHades->getRuntimeDb()->getContainer("MdcDigitPar");
    if(!fDigitPar) {
      Error("init:","Zero pointer for HMdcDigitPar recieved!");
      return kFALSE;
    }
  } else fDigitPar=0;
  fSizesCells=HMdcSizesCells::getObject();
  if (!fSizesCells) {
    Error("init","HMdcSizesCells is absent");
    return kFALSE;
  }
  
  // categoryes:
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if (!fCalCat) return kFALSE;
  if(HMdcTrackDSet::fNTuple()) {
    fClusFitCat = HMdcGetContainers::getCatMdcClusFit(kTRUE);
    fWireFitCat = HMdcGetContainers::getCatMdcWireFit(kTRUE);
    if(!fClusFitCat || !fWireFitCat) return kFALSE;
  } else {
    fClusFitCat = 0;
    fWireFitCat = 0;
  }
  locClusFit.set(1,0);
  locWireFit.set(1,0);
  loc.set(4,0,0,0,0);
  return kTRUE;
}

Bool_t HMdcTrackFitInOut::reinit(void) {
  if(!fSizesCells->initContainer()) return kFALSE;
  for(Int_t sec=0; sec<6; sec++) {
    HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sec];
    for(Int_t mod=0; mod<4; mod++)
        fSCModAr[sec][mod]=(fSizesCells->modStatus(sec,mod)) ? &fSCSec[mod] : 0;
  }
  if(version!=0 && (!HMdcGetContainers::isInited(fCal2ParSim)
                 || !HMdcGetContainers::isInited(fCal2Par))) return kFALSE;
  if( !HMdcGetContainers::isInited(fitPar) ) return kFALSE;
  if(useWireOffset) {
    if( !HMdcGetContainers::isInited(fDigitPar) ) return kFALSE;
    signalSpeed=fDigitPar->getSignalSpeed();
//     if(isGeant) signalSpeed=fDigitPar->getSignalSpeed();
//     else signalSpeed=0.004; //!!!!!!!!!!!!!!!!!!!!!!!!!!
  } else signalSpeed=0.;
  return kTRUE;
}

void HMdcTrackFitter::init(void) {
  fCal2ParSim=fitInOut->fCal2ParSim;
  fCal2Par=fitInOut->fCal2Par;
  fprint=HMdcTrackDSet::fPrint();
  version=HMdcTrackDSet::getFitVersion();
}

void HMdcTrackFitter::calcTdcErrors(Double_t* par) {
  if(fCal2Par == 0) {
    for(int time = firstTime; time < lastTime; time++) {
      errTdcTime[time]=1.;
      oneDErrTdcTime2[time]=1.;
    }
  } else {
    Double_t distance,alpha;
    Double_t xV = par[0];
    Double_t yV = par[1];
    Double_t zV = firstPl->getZOnPlane(xV,yV); //zVertex;
    Double_t xPlane = par[2];
    Double_t yPlane = par[3];
    Double_t zPlane = secondPl->getZOnPlane(xPlane,yPlane); //!!! zInitPlane;
    for(int time = firstTime; time < lastTime; time++) {
      distance=fSizeCellsLayer[time]->getImpact(xV, yV, zV,
        xPlane, yPlane, zPlane, lCell[time], alpha);
      distance=fCal2Par->calcDistance(sector,lModule[time],
          alpha,tdcTime[time]-par[4]);
      if(distance<0) distance=0.;
      errTdcTime[time]=fCal2ParSim->calcTimeErr(sector,lModule[time],alpha,distance);
      oneDErrTdcTime2[time]=1./(errTdcTime[time]*errTdcTime[time]);
    }
  }
}

Bool_t HMdcTrackFitter::fillListHits(HMdcList12GroupCells& lCells) {
//  filler=kNoFill;
  segIndex=-1;
  HMdcSizesCellsMod** fSCModAr=fitInOut->fSCModAr[sector];
  indClusFit=-1; //??? mozhet v drugoe mesto?
  nMdcTimes[0] = nMdcTimes[1] = 0;
  nClTimes = 0;
  lastTime=0;
  Int_t nDrTimes=lCells.getNDrTimes();
  if(nDrTimes>arrSize || nDrTimes<=0) {
    Error("fillListHits","num.of drift times in claster=%i. Not in 1-%i!",
        nDrTimes,arrSize);
    return kFALSE;
  }
  listCells=lCells;
  listCellsOrg=lCells;
  fitInOut->loc[0]=sector;
  for(Int_t nLayer=0;nLayer<12;nLayer++) {
    Int_t nDrTimes = lCells.getNDrTimes(nLayer);
    if(nDrTimes<=0) {
      indFWireInLay[nLayer]=-1;
      indLWireInLay[nLayer]=-2;
      continue;
    }
    indFWireInLay[nLayer]=nClTimes;
    indLWireInLay[nLayer]=nClTimes+nDrTimes-1;
    Int_t mod=nLayer/6;
    Int_t modr = mod  + segment*2;
    Int_t lay=nLayer%6;
    fitInOut->loc[1]=modr;
    fitInOut->loc[2]=lay;
    Int_t cell=-1;
    if(fSCModAr[modr]==0) return kFALSE;
    HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
    while((cell=lCells.next(nLayer,cell)) > -1) {
      Int_t nTimes=lCells.getTime(nLayer,cell);
      fitInOut->loc[3]=cell;
      HMdcCal1* cal=(HMdcCal1*)fitInOut->fCalCat->getObject(fitInOut->loc);
      if(!cal) {
        Error("fillListHits","S.%i M.%i L.%i  Cell %i is not fired",
            sector+1,modr+1,lay+1,cell+1);
        continue;
      }
      for(Int_t time=1; time<3; time++) {
        if((time&nTimes) == 0) continue;
        nMdcTimes[mod]++;
        lModule[nClTimes]=modr;
        lLayer[nClTimes]=lay;
        lCell[nClTimes]=cell;
        timeIndx[nClTimes]=time;
        qualityWeight[nClTimes] = 1.; // /nWires;
        hitStatus[nClTimes] = 1;
        fSizeCellsLayer[nClTimes]=&fSCLay;
        tdcTimeCal1[nClTimes] = (time==1) ? cal->getTime1():cal->getTime2();
        nClTimes++;
      }
    }
  }
  firstPl=&((*(fSCModAr[lModule[0]]))[0]);
  secondPl=&((*(fSCModAr[lModule[nClTimes-1]]))[5]);
  lastTime=nClTimes;
  return kTRUE;
}

void HMdcTrackFitter::setTrackPar(Double_t x1,Double_t y1,Double_t z1,
                                  Double_t x2,Double_t y2,Double_t z2) {
  firstPl->calcIntersection(x1,y1,z1,x2,y2,z2,xVertex,yVertex,zVertex);
  secondPl->calcIntersection(x1,y1,z1,x2,y2,z2,
      xInitPlane,yInitPlane,zInitPlane);
  // Wires time offset subtraction
  for(Int_t hit=0;hit<nClTimes;hit++) {
    HMdcSizesCellsMod* fSCMod=fitInOut->fSCModAr[sector][lModule[hit]];
    HMdcSizesCellsLayer& fSCLay=(*fSCMod)[lLayer[hit]];
    Int_t cell=lCell[hit];
    Float_t outside=0;
    if(fitInOut->useWireOffset) {
      // Wire offset is different for GEANT and REAL !!!
      // testing "outside" for fakes???
//      if(fitInOut->isGeant) {
      timeOffset[hit]=fSCLay.getSignPath(x1,y1,z1,x2,y2,z2,cell,outside) 
                        * fitInOut->signalSpeed;
//       } else { // 0.5 needs in new testing !!!
//         timeOffset[hit]=(fSCLay.getSignPath(x1,y1,z1,x2,y2,z2,cell,outside)-
//             0.5*fSCLay[cell].getWireLength())*fitInOut->signalSpeed;
//       }
      tdcTime[hit]=tdcTimeCal1[hit]-timeOffset[hit];
    } else {
      timeOffset[hit]=0.;
      tdcTime[hit]=tdcTimeCal1[hit];
    }
  }
}

Bool_t HMdcTrackFitter::fillClusFitCont(void) {
  if(lastTime-firstTime <1) return kFALSE;
  HMdcClusFit *fClusFit=(HMdcClusFit*)fitInOut->fClusFitCat->
      getNewSlot(fitInOut->locClusFit,&indClusFit);
  if(!fClusFit) {
    Error("fillClusFitCont","No slot HMdcClusFit available");
    return kFALSE;
  }
  fClusFit = new(fClusFit) HMdcClusFit;
  fClusFit->setSec(sector);
  fClusFit->setIOSeg(segment);
  fClusFit->setFitAuthor(HMdcTrackDSet::getFitAuthor());
  fClusFit->setDistTimeVer(HMdcTrackDSet::getFitVersion());
  fClusFit->setFitType(HMdcTrackDSet::getFitType());
  fClusFit->setNParam(numOfParam);
  fClusFit->setFunMin(funMin);
  fClusFit->setTimeOff(arriveTime);
  fClusFit->setX1(xOutVertex);
  fClusFit->setY1(yOutVertex);
  fClusFit->setZ1(zOutVertex);
  fClusFit->setX2(xOutPlane);
  fClusFit->setY2(yOutPlane);
  fClusFit->setZ2(zOutPlane);
  fClusFit->setNumIter(iteration);
  fClusFit->setClustIndex(fClst->getIndex()); // ????
  
  Int_t nGWeight = 0;
  Int_t indWireFit=-1;
  Int_t nmdc[4];
  Int_t nLayers=0;
  Int_t lay=-1;
  nmdc[0]=nmdc[1]=nmdc[2]=nmdc[3]=0;
  for(Int_t hit=firstTime; hit<lastTime; hit++) {
    if(hitStatus[hit] == 0) continue;  // this hit was fited befor it (dubl.tr.)
    HMdcWireFit *fWireFit=(HMdcWireFit*)fitInOut->fWireFitCat->
        getNewSlot(fitInOut->locWireFit,&indWireFit);
    if(!fWireFit) {
      Error("fillFitCat","No slot HMdcWireFit available");
      break;
    }
    if(hit==firstTime) fClusFit->setIndf(indWireFit);
    nmdc[lModule[hit]]++;
    if(fillWireFitCont(fWireFit,hit)) {
      nGWeight++;
      if(lay != lLayer[hit]) {
        lay=lLayer[hit];
        nLayers++;
      }
    }
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
  
  return kTRUE;
}

Bool_t HMdcTrackFitter::fillWireFitCont(HMdcWireFit* fWireFit,Int_t hit) const {
  fWireFit = new(fWireFit) HMdcWireFit;
  fWireFit->setAddress(sector, lModule[hit], lLayer[hit], lCell[hit]);
  fWireFit->setTimeNum(timeIndx[hit]);
  fWireFit->setDev(dev[hit]);
  fWireFit->setWeight((hitStatus[hit]<0 || weight[hit]<0.001) ? 0.:weight[hit]);
  fWireFit->setDriftTime(drTime[hit]);
  fWireFit->setFullTime(drTime[hit]+arriveTime);
  fWireFit->setTdcTime(tdcTime[hit]);
  fWireFit->setMinDist(distWire[hit]);
  fWireFit->setAlpha(alphaDrDist[hit]);
  fWireFit->setIsInCell(inCell[hit]);
  fWireFit->setTdcTimeErr(errTdcTime[hit]);
  if(hitStatus[hit]==1 && weight[hit]>0.1) {  // 0.01 ???
    fWireFit->setWeight( weight[hit]);
    return kTRUE;
  }
  fWireFit->setWeight(0.);
  return kFALSE;
}

Double_t HMdcTrackFitter::testFitResult(void) {
  // Selection of hits (needs in checking ???).
  Double_t sumDelWeight=0.;
  for(int hit=firstTime;hit<lastTime;hit++) {
    if(hitStatus[hit] != 1) continue;
    Int_t mod=lModule[hit]%2;
    Int_t lay=lLayer[hit];
    Int_t cell=lCell[hit];
    Int_t time=timeIndx[hit];
    // Test for qualityWeight:
    if(qualityWeight[hit]<.1) { // Hits sel. by weight
      // new fit ???
      listCells.delTime(mod*6+lay,cell,time);
      hitStatus[hit]=-1;
      continue;
    }
    // Test for Tukey wiht:
    if(weight[hit]<fitInOut->cutWeight) {
      listCells.delTime(mod*6+lay,cell,time);
      hitStatus[hit]=-1;
      sumDelWeight+=weight[hit]; // +=weight[dHit]*qual.W[dHit] ?
      continue;
    }
    //Test on double hit:
    // time==2 - weights and qualityWeights are tested already
    if(time==2 && listCells.getTime(mod*6+lay,cell)==3) { // Double hit
      // new fit ???
      Double_t dev1=fabs(dev[hit-1]);
      Double_t dev2=fabs(dev[hit]);
      Int_t dHit=(dev1 > dev2) ? hit-1:hit;
      listCells.delTime(mod*6+lay,cell,timeIndx[dHit]);
      sumDelWeight+=1.;  // +=weight[dHit]*qualityWeight[dHit] ???
      hitStatus[dHit]=-1;
    }
  }
  // Test for layers which have > 2 wires:
  // !!!!!!!! Nado perepisat' i sdelat' proverku tol'ko na inCell !!! 
//  Int_t l1=(fittingMod!=1) ? 0:6;
//  Int_t l2=(fittingMod==0) ? 6:12;
  Int_t l1=lLayer[firstTime]+(lModule[firstTime]&1)*6;
  Int_t l2=lLayer[lastTime-1]+(lModule[lastTime-1]&1)*6;
  for(Int_t lay=l1;lay<l2;lay++) {
    Int_t fIndCell=indFWireInLay[lay];
    if(fIndCell<0) continue;
    Int_t lIndCell=indLWireInLay[lay];
    while(lCell[lIndCell]-lCell[fIndCell]>1) { // >1 ?????
      if(hitStatus[fIndCell]<1) {
        fIndCell++;
        continue;
      }
      if(hitStatus[lIndCell]<1) {
        lIndCell--;
        continue;
      }
      if(inCell[fIndCell] && inCell[lIndCell]) break;
      if(HMdcTrackDSet::fPrint()) printf(
          "Region of fired wires in layer %i: cells %i - %i > 1\n",lay+1,
        lCell[lIndCell],lCell[fIndCell]);
      Int_t dHit=fIndCell;
      if(inCell[fIndCell] || (!inCell[lIndCell] && 
        distWire[fIndCell] < distWire[lIndCell])) dHit=lIndCell;
      listCells.delTime(lay,lCell[dHit],timeIndx[dHit]);  // new fit ???
      sumDelWeight+=1.;   // +=weight[dHit]*qualityWeight[dHit] ???
      hitStatus[dHit]=-1;
    }
  }
  return sumDelWeight;
}

Int_t HMdcTrackFitter::unsetHits(void) {
  Int_t nFitedTimes=0;
  for(Int_t hit=firstTime;hit<lastTime;hit++) {
    if(hitStatus[hit]<1) continue;
    listCellsOrg.delTime(lLayer[hit],lCell[hit],timeIndx[hit]);
    nFitedTimes++;
  }
  return nFitedTimes;
}


Bool_t HMdcTrackFitter::fitCluster(Double_t threshold, Int_t fittingMod) {
  //??? calcMinFunctional();  // Utochnenie nachal'nogo priblizheniya
  fitStatus=kFALSE;
  Int_t ncells=(fittingMod<0) ? listCellsOrg.getNCells() :
      listCellsOrg.getNCells(fittingMod*6,fittingMod*6+5);
  if(ncells<5) return kFALSE;
  Int_t iter=0;
  Int_t nCellM1=0;
  Int_t nCellM2=0;
  for(Int_t n=firstTime; n<lastTime; n++) if(hitStatus[n]<0) hitStatus[n]=1;
  while(kTRUE) {
    if(execute(threshold*100.,iter++)==0) {
      // fill cl ?????????????????????????????????????????
      return kFALSE;
    }
    Double_t delW=testFitResult();
    nCellM1=(fittingMod!=1) ? listCells.getNCells(0,5) : 0;
    nCellM2=(fittingMod!=0) ? listCells.getNCells(6,11) : 0;
    if( delW<0.5 || (nCellM1+nCellM2<6) ) break;  // 5.0 ??? (mnogo?)
    if(HMdcTrackDSet::fPrint()) printf(
      "\n Sum.of weights of deleted times=%f>0.5, start new fit for this cluster! \n"
      ,delW);
  }
  if(funMin>threshold) return kFALSE;
  if(parameter[4] < -15.0 || parameter[4] > 30.0) return kFALSE;  // Too large t-off !!!
  if(nCellM1+nCellM2 < 5) return kFALSE; // ????????????????
  if(fitInOut->fClusFitCat && HMdcTrackDSet::fNTuple()) fillClusFitCont();
  fitStatus=kTRUE;
  return kTRUE;
}

void HMdcTrackFitter::calcMinFunctional(void) {
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
//       setXInitPlane(xNew);
//       setYInitPlane(yNew);
//       setZInitPlane(fClst->getZOnPlane(xNew,yNew));
//       Double_t func=getFunctional();
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
// setXInitPlane(xMin);
// setYInitPlane(yMin);
// setZInitPlane(fClst->getZOnPlane(xMin,yMin));
// //   setXInitPlane(xPlane);
// //   setYInitPlane(yPlane);
// //   setZInitPlane(zPlane);
}

void HMdcTrackFitter::setFittingTimesList(Int_t iMod) {
  firstTime= (iMod!=1) ?            0 : nMdcTimes[0];
  lastTime=  (iMod==0) ? nMdcTimes[0] : nClTimes;
  nMDC1Times=(iMod==1) ?            0 : nMdcTimes[0];
  nMDC2Times=(iMod==0) ?            0 : nMdcTimes[1];
}
