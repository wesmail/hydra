//*--- Author : V.Pechenov
//*--- Modified: 16.08.05 V.Pechenov

using namespace std;
#include <iostream>
#include <iomanip>

#include "hmdcwiredata.h"
#include "hmdccal1sim.h"
#include "hmdccal2parsim.h"
#include "hmdccal2par.h"
#include "hmdctrackparam.h"
#include "hlocation.h"
#include "hmdcwirefitsim.h"
#include "hmdcclusfitsim.h"
#include "hmdctrackfitter.h"
#include "hmdcclus.h"
#include "hmatrixcategory.h"
#include "hmdclistcells.h"
#include "hmdctrackdset.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"

//////////////////////////////////////////////////////////////////////////////
//
//  HMdcWireData
//
//    Class keep one wire data for track fitter.
//
//  HMdcWireArr
//
//    Array of HMdcWireData objects.
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HMdcWireData)
ClassImp(HMdcWiresArr)

HMdcCal2ParSim* HMdcWireData::fCal2ParSim=0;
HMdcCal2Par* HMdcWireData::fCal2Par=0;

HMdcWireData::HMdcWireData(void) {
}

void HMdcWireData::setCell(Int_t s, Int_t m, Int_t l, Int_t c, Int_t it,
    Float_t t) {
  sector=s;
  module=m;
  layer=l;
  cell=c;
  timeIndx=it;
  tdcTimeCal1=t;
  fMdcCal1=0;
  setInitVal();
}

void HMdcWireData::setCell(HMdcCal1* cal1, Int_t it, Bool_t isGeant) {
  cal1->getAddress(sector, module,layer,cell);
  timeIndx=it;
  if(timeIndx==1) tdcTimeCal1=cal1->getTime1();
  else tdcTimeCal1 = cal1->getTime2();
  fMdcCal1=cal1;
  setInitVal();
  if(isGeant) {
    HMdcCal1Sim* c=(HMdcCal1Sim*)cal1;
    if(timeIndx==1) {geantTrackNum=c->getNTrack1();}
    else{geantTrackNum=c->getNTrack2();}
  } else geantTrackNum=-1;
}

void HMdcWireData::setInitVal(void) {
  weight = 1.;
  hitStatus = 1;
  errTdcTime = oneDErrTdcTime2 = 1.;
  wtNorm=weight*oneDErrTdcTime2;
  signalTimeOffset=0.;
}

void HMdcWireData::setUnitWeight(void) {
  if(hitStatus==0) return;
  weight=1.;
  errTdcTime = oneDErrTdcTime2 = 1.;
  wtNorm=weight*oneDErrTdcTime2;
  signalTimeOffset=0.;
}

void HMdcWireData::setCal2Par(HMdcCal2Par* c2par,
    HMdcCal2ParSim* c2ParSim) {
  fCal2Par=c2par;
  fCal2ParSim=c2ParSim;
}

void HMdcWireData::setNegTdcTimeTo0(void) {
  if(tdcTimeCal1<0.) tdcTimeCal1=0.; // !!!!!!!!!!!!!!!!!!!!!!!!
}

void HMdcWireData::setSignalTimeOffset(Double_t offset) {
  // must be called after setCell !
  signalTimeOffset=offset;
  tdcTime=tdcTimeCal1-signalTimeOffset;
}

void HMdcWireData::calcTdcErrors(HMdcTrackParam& par) {
  if(fCal2Par == 0) {
      errTdcTime=1.;
      oneDErrTdcTime2=1.;
  } else {
    Double_t distance=fCal2Par->calcDistance(sector,module,
        alpha,tdcTime-par.getTimeOffset(module));
    if(distance<0) distance=0.;
    errTdcTime=fCal2ParSim->calcTimeErr(sector,module,alpha,distance);
    oneDErrTdcTime2=1./(errTdcTime*errTdcTime);
    wtNorm=weight*oneDErrTdcTime2;
  }
}

void HMdcWireData::setWeightTo1or0(Double_t maxChi2, Double_t minWeight) {
// 1) Nado proveryat' hitStatus<0 .
// 2) Pri vkluchenii wire nado poschitat' dTdA !!!
//
//?         if(hitStatus<0) {
//?           if(chi2<maxChi2) {
//?             hitStatus=1;
//?             weight=1.;
//?           }
//?         } else if(hitStatus>0) {
//?           weight=(chi2<maxChi2 || weight>minWeight) ? 1.:0.;
//?         } else weight=0.;
  weight=(chi2<maxChi2 || weight>minWeight) ? 1.:0.;
  wtNorm=weight*oneDErrTdcTime2;
}

void HMdcWireData::calcDriftTime(HMdcTrackParam& par) {
  // calculation of distance to the wire, impact ang. and drift time
//!?????????????? est' ne ponyatnoe vliyanie iflag na rezul'tat !!!
  if(hitStatus != 0) {
    distance=fSCLayer->getImpact(par,cell,alpha);
    driftTime=fCal2ParSim->calcTime(sector,module,alpha,distance);
    dev0 = driftTime - tdcTime;
    if(hitStatus==1) par.addToSumsDevWt(module,dev0,wtNorm);
  }
}

void HMdcWireData::calcDriftTimeAndInCell(HMdcTrackParam& par) {
  // calculation of distance to the wire, impact ang. and drift time
//!?????????????? est' ne ponyatnoe vliyanie iflag na rezul'tat !!!
  if(hitStatus != 0) {
    inCell=fSCLayer->getImpact(par,cell,alpha,distance);  
    driftTime=fCal2ParSim->calcTime(sector,module,alpha,distance);
    dev0 = driftTime - tdcTime;
    if(hitStatus==1) par.addToSumsDevWt(module,dev0,wtNorm);
  }
}

void HMdcWireData::calcDriftTimeForDeriv(HMdcTrackParam& par) {
  // calculation drift time for derivativatives
  if(hitStatus==1) {
    Double_t alphaD;
    Double_t distD=fSCLayer->getImpact(par,cell,alphaD);
    driftTimeDer=fCal2ParSim->calcTime(sector,module,alphaD,distD);
    devDer = driftTimeDer - tdcTime;
    par.addToSumsDevWt(module,devDer,wtNorm);
  }
}

void HMdcWireData::recalcFunctional(HMdcTrackParam& par) {
  // Recalculation of finctional without calculation of ditances
  // (for the same parameters as before)
  if(hitStatus!=0) {
    dev0 = driftTime - tdcTime;
    if(hitStatus==1) par.addToSumsDevWt(module,dev0,wtNorm);
  }
}

void HMdcWireData::calcFunctional(HMdcTrackParam& par) {
  if(hitStatus!=0) {
    dev = dev0 + par.getTimeOffset(module);
    Double_t chi=dev/errTdcTime;
    chi2 = chi*chi;
    if(hitStatus==1) par.addToFunct(chi2*weight,weight);
  }
}

void HMdcWireData::calcFunctionalForDer(HMdcTrackParam& par) {
  if(hitStatus==1) {
    Double_t chi=(devDer+par.getTimeOffset(module))/errTdcTime;
    par.addToFunct(chi*chi*weight,weight);
  }
}

void HMdcWireData::calcDevAndFunct(HMdcTrackParam& par) {
  if(hitStatus!=0) {
    dev0=driftTime - tdcTime;
    dev = dev0 + par.getTimeOffset(module);
    Double_t chi=dev/errTdcTime;
    chi2 = chi*chi;
    if(hitStatus==1) par.addToFunct(chi2*weight,weight);
  }
}

void HMdcWireData::getAddress(Int_t& s,Int_t& m,Int_t& l,Int_t& c,Int_t& it) {
  s=sector;
  m=module;
  l=layer;
  c=cell;
}

Bool_t HMdcWireData::removeIfWeightLess(Double_t wtCut,
    HMdcList24GroupCells& list) {
  // function return kTRUE if hit is removed
  if(hitStatus==1 && weight<wtCut) {
    removeThisWire(list);
    return kTRUE;
  }
  return kFALSE;
}

Int_t HMdcWireData::unsetFittedHit(HMdcList24GroupCells& list) const {
  if(hitStatus!=1) return 0;
  list.delTime(module*6+layer,cell,timeIndx);
  return 1;
}

void HMdcWireData::getLocation(HLocation& loc) const {
  loc.set(4,sector,module,layer,cell);
}

void HMdcWireData::fillWireFitCont(HMdcWireFit* fWireFit) const {
  // Filling of HMdcWireFit container
  fWireFit->setAddress(sector,module,layer,cell);
  fWireFit->setTimeNum(timeIndx);
  fWireFit->setDev(dev);
  fWireFit->setDriftTime(driftTime);
  fWireFit->setFullTime(tdcTime+dev);
  fWireFit->setTdcTime(tdcTime);
  fWireFit->setMinDist(distance);
  fWireFit->setAlpha(alpha);
  fWireFit->setIsInCell(inCell);
  fWireFit->setTdcTimeErr(errTdcTime);
  fWireFit->setWeight((hitStatus==1) ? weight:0.);
}

void HMdcWireData::fillWireFitSimCont(HMdcWireFit* fWireFit,
    Int_t trackNum) const {
  // Filling of HMdcWireFit container
  if(!fWireFit->isGeant() || fMdcCal1==0) return;
  HMdcCal1Sim* c=(HMdcCal1Sim*)fMdcCal1;
  HMdcWireFitSim* fWFSim=(HMdcWireFitSim*)fWireFit;
  Int_t track=(timeIndx==1) ? c->getNTrack1() : c->getNTrack2();
  fWFSim->setGeantTrackNum(track);
  fWFSim->setGeantMinDist (timeIndx==1 ? c->getMinDist1(): c->getMinDist2());
  fWFSim->setGeantAlpha   (timeIndx==1 ? c->getAngle1()  : c->getAngle2());
  fWFSim->setDigiTimeErr  (timeIndx==1 ? c->getError1()  : c->getError2());
  fWFSim->setGeantTof     (timeIndx==1 ? c->getTof1()    : c->getTof2());
  fWFSim->setClusFitTrFlag(track==trackNum);
}

void HMdcWireData::removeThisWire(HMdcList24GroupCells& list) {
  // Removing of wire from list of cells and setting hitStatus to -1
  list.delTime(layer+module*6,cell,timeIndx);
  hitStatus=-1;
}

Bool_t HMdcWireData::removeOneTimeIfItDoubleTime(HMdcWireData* time1,
    HMdcList24GroupCells& list) {
  // Removing time1 or time2 from one wire for double hit
  // "this" must be for time2 !
  if(hitStatus!=1 || timeIndx!=2 || list.getTime(module*6+layer,cell)!=3)
      return kFALSE;
  if(fabs(time1->dev) <= fabs(dev)) removeThisWire(list);
  else time1->removeThisWire(list);
  return kTRUE;
}

Bool_t HMdcWireData::removeIfOneDistOutCell(HMdcWireData* wire2,
    HMdcList24GroupCells& list) {
  // Removing "this" or wire2 from the same layer in "distance" out of cell
  // return kTRUE if one wire is removed
  if(abs(cell-wire2->cell)<=1 || (inCell && wire2->inCell)) return kFALSE;
  if(wire2->inCell) removeThisWire(list);
  else if(inCell) wire2->removeThisWire(list);
  else if(wire2->distance<distance) removeThisWire(list);
  else wire2->removeThisWire(list);
  return kTRUE;
}

void HMdcWireData::calcDriftTimeAndFunct(HMdcTrackParam& par) {
  // Calculation of functional without recalculation of time offset!
  // (for investigation of finctional)
  if(hitStatus != 0) {
    distance=fSCLayer->getImpact(par,cell,alpha);
    driftTime=fCal2ParSim->calcTime(sector,module,alpha,distance);
    dev0= driftTime - tdcTime;
    dev = dev0 + par.getTimeOffset(module);
    Double_t chi=dev/errTdcTime;
    chi2 = chi*chi;
    if(hitStatus==1) par.addToFunct(chi2*weight,weight);
  }
}

void HMdcWireData::calcDTdPar(Int_t par, Double_t oneDv2Step) {
  if(hitStatus!=1) return;
  dTdPar[par]-=driftTimeDer;
  dTdPar[par]*=oneDv2Step;
}

Double_t HMdcWireData::getHijEll(Int_t i, Int_t j) const {
  return (hitStatus==1) ? 4.0*dTdPar[i]*dTdPar[j]*wtNorm : 0.;
}

void HMdcWireData::printIfDeleted(void) const {
  if(weight>0.01 && hitStatus<0)
        printf("Warn.!  HIT %i is deleted\n",sequentialIndex);
}

void HMdcWireData::subtractWireOffset(HMdcTrackParam& par, Double_t sigSpeed) {
  // Subtraction of wire time offset
//! Function getSignPath(,,,,,,,Float_t outside) can be used for fake finding
  signalTimeOffset=fSCLayer->getSignPath(par,cell) * sigSpeed;
  tdcTime=tdcTimeCal1-signalTimeOffset;
}

void HMdcWireData::addToTOffsetErr(HMdcTrackParam& par) {
  if(hitStatus==1) par.addToTOffsetErr(module,dTdPar,wtNorm);
}

void HMdcWireData::printTime(Int_t iflag,HMdcTrackParam& par,Bool_t isGeant) {
  printf("%c%3i)%c %iM%iL%3iC %5.2fmm",(iflag==2) ? '!':' ',sequentialIndex,
      (hitStatus==1)?'+':'-',module+1,layer+1,cell+1,distance);
  if(iflag==2) printf(" %c",inCell ? 'I':'O');
  printf(" dev=%5.1f%+4.1f%+6.1f=%5.1fns",driftTime,par.getTimeOffset(module),
      -tdcTime,dev);
  printf(" dT=%4.1f chi2=%6.1f WT=%6.4f",errTdcTime,chi2,weight);
  if(geantTrackNum>0) printf("%5i trk.",geantTrackNum);
  printf("\n");
}

void HMdcWireData::setTukeyWeight(Double_t cwSig,Double_t c2Sig,Double_t c4Sig,
    Double_t& sumWt, Double_t& funct) {
  if(hitStatus==0) return;
  if(hitStatus!=1 || chi2 >= c2Sig) weight=wtNorm=0.;
  else {
    if(chi2 < cwSig) {
      weight=chi2/c4Sig;
      weight = 1. - weight*weight;
    } else weight = 1. - chi2/c2Sig;
    weight*=weight;
    wtNorm=weight*oneDErrTdcTime2;
  }
  if(hitStatus==1) {
    sumWt+=weight;
    funct+=chi2*weight;
  }
}

Bool_t HMdcWireData::isAddressEq(Int_t s, Int_t m, Int_t l, Int_t c) {
  if(s>=0 && s!=sector) return kFALSE;
  if(m>=0 && m!=module) return kFALSE;
  if(l>=0 && l!=layer)  return kFALSE;
  if(c>=0 && c!=cell)   return kFALSE;
  return kTRUE;
}

//-----------------------------------------------------------

HMdcWiresArr::HMdcWiresArr(void) {
  arraySize=0;
  wires=0;
  fClst1 = 0;
  fClst2 = 0;
  setNumDriftTimes(500);
  fitInOut=0;
  locCal1.set(4,0,0,0,0);
}

HMdcWiresArr::~HMdcWiresArr(void) {
  if(wires) delete [] wires;
  wires=0;
}

void HMdcWiresArr::setNumDriftTimes(Int_t nDrTm) {
  nDriftTimes=nDrTm;
  if(nDriftTimes>arraySize) {
    if(wires) delete [] wires;
    arraySize=(arraySize<=0) ? nDriftTimes : (Int_t)(1.3*nDriftTimes);
    wires=new HMdcWireData [arraySize];
    for(Int_t i=0;i<arraySize;i++) wires[i].setSequentialIndex(i);
  }
}

Bool_t HMdcWiresArr::fillListHits(HMdcClus* fCl1, HMdcClus* fCl2) {
  // Filling list wire for fitting from cluster(s)
  setListCells(fCl1,fCl2);
  HCategory* fCal1Cat=fitInOut->getMdcCal1Cat();
  HMdcSizesCellsMod** fSCModAr=fitInOut->getSCellsModArr(sector);
  locCal1[0]=sector;
  Int_t nClTimes=0;
  for(Int_t layer=0;layer<24;layer++) {
    Int_t nDrTimes = lInputCells.getNDrTimes(layer);
    if(nDrTimes<=0) {
      firstWireInLay[layer]=lastWireInLay[layer]=0;
      continue;
    }
    firstWireInLay[layer]=wires+nClTimes;
    lastWireInLay[layer] =wires+nClTimes+nDrTimes-1;
    Int_t modr = layer/6;
    Int_t lay=layer%6;
    locCal1[1]=modr;
    locCal1[2]=lay;
    Int_t cell=-1;
    if(fSCModAr[modr]==0) return kFALSE;
    HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
    while((cell=lInputCells.next(layer,cell)) > -1) {
      Int_t nTimes=lInputCells.getTime(layer,cell);
      locCal1[3]=cell;
      HMdcCal1* cal=(HMdcCal1*)fCal1Cat->getObject(locCal1);
      if(!cal) {
        Error("fillListHits","S.%i M.%i L.%i  Cell %i is not fired",
            sector+1,modr+1,lay+1,cell+1);
        return kFALSE;
      }
      for(Int_t time=1; time<3; time++) {
        if((time&nTimes) == 0) continue;
        wires[nClTimes].setCell(cal,time,fitInOut->isGeant());
        wires[nClTimes].setSizesCellsLayer(&fSCLay);
	wires[nClTimes].setNegTdcTimeTo0(); //!!!!!!!!!!!!!!???????????
        nClTimes++;
      }
    }
  }
  if(nDriftTimes!=nClTimes) return kFALSE;
  return kTRUE;
}

Bool_t HMdcWiresArr::fillListHits(HMdcEvntListCellsAndTimes* store,
    HMdcClus* fCl1, HMdcClus* fCl2) {
  // Filling list wire for fitting from cluster(s),
  // drift times from HMdcEvntListCellsAndTimes
  setListCells(fCl1,fCl2);
  HMdcSecListCells& storeSec=(*store)[sector];
  HMdcSizesCellsMod** fSCModAr=fitInOut->getSCellsModArr(sector);
  locCal1[0]=sector;
  Int_t nClTimes=0;
  for(Int_t layer=0;layer<24;layer++) {
    Int_t nDrTimes = lInputCells.getNDrTimes(layer);
    if(nDrTimes<=0) {
      firstWireInLay[layer]=lastWireInLay[layer]=0;
      continue;
    }
    firstWireInLay[layer]=wires+nClTimes;
    lastWireInLay[layer] =wires+nClTimes+nDrTimes-1;
    Int_t modr = layer/6;
    Int_t lay=layer%6;
    locCal1[1]=modr;
    locCal1[2]=lay;
    Int_t cell=-1;
    if(fSCModAr[modr]==0) return kFALSE;
    HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
    HMdcLayListCells& storeLay=storeSec[modr][lay];
    while((cell=lInputCells.next(layer,cell)) > -1) {
      if(lInputCells.getTime(layer,cell) == 0) continue;
      wires[nClTimes].setCell(sector,modr,lay,cell,1,storeLay.getTimeValue(cell)); // index of time =1 !!!!???
      wires[nClTimes].setSizesCellsLayer(&fSCLay);
      wires[nClTimes].setNegTdcTimeTo0(); //!!!!!!!!!!!!!!???????????
      nClTimes++;
    }
  }
  if(nDriftTimes!=nClTimes) return kFALSE;
  return kTRUE;
}

void HMdcWiresArr::setListCells(HMdcClus* fCl1, HMdcClus* fCl2) {
  sigma2=-1.;
  fClst1=fCl1;
  fClst2=fCl2;
  sector=fClst1->getSec();
  segment=fClst1->getIOSeg();
  if(fClst2 != 0) segment = 3;
  if(segment==0) lInputCells.set(fClst1,0);
  else if(segment==1) lInputCells.set(0,fClst1);
  else lInputCells.set(fClst1,fClst2);
  xClst=fClst1->getX();
  yClst=fClst1->getY();
  zClst=fClst1->getZ();
  lOutputCells=lInputCells;
  nDriftTimes=0;
  for(Int_t m=0;m<4;m++) nDriftTimes+=nMdcTimes[m]=lInputCells.getNDrTimesMod(m);
  setNumDriftTimes(nDriftTimes);
  firstTime=0;
  lastTime=nDriftTimes;
  firstWire=wires;
  lastWire=wires+nDriftTimes-1;
}

Bool_t HMdcWiresArr::fillListHits(HMdcEvntListCellsAndTimes* store) {
  // Filling list wire for fitting from cluster(s),
  // drift times from HMdcEvntListCellsAndTimes
  setListCells(store);
  HMdcSecListCells& storeSec=(*store)[sector];
  HMdcSizesCellsMod** fSCModAr=fitInOut->getSCellsModArr(sector);
  locCal1[0]=sector;
  Int_t nClTimes=0;
  for(Int_t layer=0;layer<24;layer++) {
    Int_t nDrTimes = lInputCells.getNDrTimes(layer);
    if(nDrTimes<=0) {
      firstWireInLay[layer]=lastWireInLay[layer]=0;
      continue;
    }
    firstWireInLay[layer]=wires+nClTimes;
    lastWireInLay[layer] =wires+nClTimes+nDrTimes-1;
    Int_t modr = layer/6;
    Int_t lay=layer%6;
    locCal1[1]=modr;
    locCal1[2]=lay;
    Int_t cell=-1;
    if(fSCModAr[modr]==0) return kFALSE;
    HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
    HMdcLayListCells& storeLay=storeSec[modr][lay];
    while((cell=lInputCells.next(layer,cell)) > -1) {
      if(lInputCells.getTime(layer,cell) == 0) continue;
      wires[nClTimes].setCell(sector,modr,lay,cell,1,storeLay.getTimeValue(cell)); // index of time =1 !!!!???
      wires[nClTimes].setSizesCellsLayer(&fSCLay);
      wires[nClTimes].setNegTdcTimeTo0(); //!!!!!!!!!!!!!!???????????
      nClTimes++;
    }
  }
  if(nDriftTimes!=nClTimes) return kFALSE;
  return kTRUE;
}

void HMdcWiresArr::setListCells(HMdcEvntListCellsAndTimes* store) {
  Int_t s=-1, m, l, c;
  UChar_t t;
  nDriftTimes = 0;
  lInputCells.clear();
  while(store->nextCell(s, m, l, c, t)) {
    sector = s;
    lInputCells.setTime(m*6+l,c,t);
  }
  for(Int_t m=0;m<4;m++) nDriftTimes+=nMdcTimes[m]=lInputCells.getNDrTimesMod(m);
  if(nMdcTimes[2]+nMdcTimes[3] == 0) segment=0;
  else if(nMdcTimes[0]+nMdcTimes[1] == 0) segment=1;
  else segment=3;  
  setNumDriftTimes(nDriftTimes);
  firstTime = 0;
  lastTime = nDriftTimes;
  firstWire = wires;
  lastWire = wires+nDriftTimes-1;
  lOutputCells = lInputCells;
}

Double_t HMdcWiresArr::testFitResult(void) {
  // Removing cells which has small weight or
  // if min.distance outside of this cell for case when more than on wires
  // in this layer passed fit and so on
// Can be tuned and checked ???).
  Double_t sumDelWeight=0.;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    // Test for Tukey weight:
    if(w->removeIfWeightLess(fitInOut->getWeightCut(),lOutputCells)) {
        sumDelWeight+=w->getWeight();
        numOfGoodWires--;
      //Test on double time (time==2 - weight are tested already):
    } else if(w>firstWire && w->removeOneTimeIfItDoubleTime(w-1,lOutputCells)) {
      sumDelWeight+=1.;  // +=weight[dHit] ???
      numOfGoodWires--;
    }
  }

  // Test for layers which have > 2 wires:
// !!!!!!!! Nado perepisat' i sdelat' proverku tol'ko na inCell !!!
  Int_t l1 = firstWire->getSequentialLayNum();
  Int_t l2 =  lastWire->getSequentialLayNum();
  for(Int_t lay=l1;lay<=l2;lay++) {
    HMdcWireData* fWire=firstWireInLay[lay];
    if(fWire==0) continue;
    HMdcWireData* lWire=lastWireInLay[lay];
    while(fWire<lWire) {
      if(fWire->getHitStatus()==1) {
        if(lWire->getHitStatus()==1) {
          if(fWire->removeIfOneDistOutCell(lWire,lOutputCells)) {
            sumDelWeight+=1.;
            numOfGoodWires--;
          } else break;
        } else lWire--;
      } else fWire++;
    }
  }
  if(fprint) for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->printIfDeleted();
  return sumDelWeight;
}

void HMdcWiresArr::subtractWireOffset(HMdcTrackParam& par) {
  // Must be called after setCell !
  if(!fitInOut->useWireOffset()) return;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->subtractWireOffset(par,fitInOut->getSignalSpeed());
}

void HMdcWiresArr::setHitStatM1toP1(void) {
  // if(hitStatus==-1) hitStatus=1
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->setHitStatM1toP1();
}

void HMdcWiresArr::switchOff(Int_t sec, Int_t mod, Int_t lay, Int_t cell) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      if(w->isAddressEq(sec,mod,lay,cell)) w->setHitStatus(-2);
}

Int_t HMdcWiresArr::unsetHits(void) {
  // removing cells passed fit
  Int_t nFitedTimes=0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      nFitedTimes +=w->unsetFittedHit(lInputCells);
  return nFitedTimes;
}

Int_t HMdcWiresArr::getNCellsInInput(Int_t mod) {
  // Return number of cells in mdc module =mod in input list
  // if mod<0 return number of cells in all mdc's of segment
  if(mod<0) return lInputCells.getNCells();
  return lInputCells.getNCells(mod*6,mod*6+5);
}

Int_t HMdcWiresArr::getNCellsInOutput(Int_t mod) {
  // Return valid value after calling testFitResult() only !
  // Return number of cells in mdc module =mod passed fit
  // if mod<0 return number of cells in all mdc's of segment
  if(mod<0) return lOutputCells.getNCells();
  return lOutputCells.getNCells(mod*6,mod*6+5);
}

Bool_t HMdcWiresArr::calcNGoodWiresAndChi2(HMdcTrackParam& par) {
  numOfGoodWires=0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      if(w->isPassedFit()) numOfGoodWires++;
  return (par.calcChi2PerDF(numOfGoodWires)<0.) ? kFALSE:kTRUE;
}

void HMdcWiresArr::fillClusFitAndWireFit(HMdcClusFit* fClusFit) {
  fClusFit->setSec(sector);
  fClusFit->setIOSeg(segment);
  fClusFit->setFitAuthor(HMdcTrackDSet::getFitAuthor());
  fClusFit->setDistTimeVer(fitInOut->getDrTimeCalcVer());
  fClusFit->setFitType(HMdcTrackDSet::getFitType());
  fClusFit->setClustIndex(fClst1->getIndex()); // ???? for 2 segm. ???
  Int_t firstMod=firstWire->getModule();
  Int_t lastMod=lastWire->getModule();
  fClusFit->setMod((firstMod==lastMod) ? firstMod:-1);
  Int_t nLayers=0;
  for(Int_t m=firstMod;m<=lastMod;m++) nLayers += lOutputCells.getNLayersMod(m);
  fClusFit->setNumOfLayers(nLayers);
  Int_t indFirstWireFit=-1;
  Int_t indLastWireFit=-1;
  
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    if(w->getHitStatus() == 0) continue;
    HMdcWireFit *fWireFit=fitInOut->getNewWireFitSlot(&indLastWireFit);
    if(!fWireFit) break;
    if(indFirstWireFit<0) indFirstWireFit=indLastWireFit;
    w->fillWireFitCont(fWireFit);
    if(fClusFit->isGeant()) w->fillWireFitSimCont(fWireFit,
          ((HMdcClusFitSim*)fClusFit)->getGeantTrackNum());
  }
  fClusFit->setFirstWireFitInd(indFirstWireFit);
  fClusFit->setLastWireFitInd(indLastWireFit);
  fClusFit->setSigmaChi2(sigma2>=0. ? sqrt(sigma2):-1.);
}

void HMdcWiresArr::fillClusFitSim(HMdcClusFit* fClusFit,
    HMdcTrackParam& par) {
  if(!fClusFit->isGeant()) return;
  Int_t nTrcksClus=0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    if(w->getHitStatus() == 0) continue;
    Int_t track=w->getGeantTrackNum();
    Int_t indx=0;
    for(;indx<nTrcksClus;indx++) if(trackNum[indx]==track) break;
    if(indx==nTrcksClus) {
      if(indx==200) continue; //!!!!!!!!!
      numWiresClus[indx]=numWiresFit[indx]=0;
      trackNum[indx]=track;
      nTrcksClus++;
    }
    numWiresClus[indx]++;
    if(w->getHitStatus()==1) numWiresFit[indx]++;
  }
  Int_t indxMax=0;
  Int_t nTrcksFit=0;
  for(Int_t i=0;i<nTrcksClus;i++) {
    if(numWiresFit[i]==0) continue;
    nTrcksFit++;
    if(numWiresFit[indxMax]<numWiresFit[i]) indxMax=i;
  }
  Int_t geantTrack=trackNum[indxMax];
  HMdcClusFitSim* fClusFitSim=(HMdcClusFitSim*)fClusFit;
  fClusFitSim->setNumTracks(nTrcksFit);  
  fClusFitSim->setNumTracksClus(nTrcksClus);
  fClusFitSim->setGeantTrackNum(geantTrack);
  fClusFitSim->setNumWiresTrack(numWiresFit[indxMax]);
  fClusFitSim->setNumWiresTrClus(numWiresClus[indxMax]);
  if(fClusFitSim->isFake()) {
    fClusFitSim->setFakeTrack();
    return;
  }
  if(fitInOut->getGeantKineCut()==0) return;
  HGeantKine* fGeantKine=(HGeantKine*)fitInOut->getGeantKineCut()->
      getObject(trackNum[indxMax]-1);
  if(fGeantKine==0) return;
  fClusFitSim->setPrimaryFlag(fGeantKine->isPrimary());
  if(fitInOut->getGeantMdcCut()==0) return;

  HMdcWireData* firstTrWire=NULL;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    if(w->getGeantTrackNum()!=geantTrack || w->getHitStatus()!=1) continue;
    firstTrWire=w;
    break;
  }
  HMdcWireData* lastTrWire=NULL;
  for(HMdcWireData* w=lastWire;w>=firstWire;w--) {
    if(w->getGeantTrackNum()!=geantTrack || w->getHitStatus()!=1) continue;
    lastTrWire=w;
    break;
  }

  fGeantKine->resetMdcIter();
  HGeantMdc* hit = NULL;
  HGeantMdc* firstHit = NULL;
  HGeantMdc* lastHit  = NULL;
  Int_t modF=firstTrWire->getModule();
  Int_t modL=lastTrWire ->getModule();
  Int_t layF=firstTrWire->getLayer();
  Int_t layL=lastTrWire ->getLayer();
  while((hit = (HGeantMdc*) fGeantKine->nextMdcHit()) != NULL) {
    if(sector!=hit->getSector()) break;
    if(firstHit==NULL && hit->getModule()==modF &&
        hit->getLayer()==layF) firstHit=hit;
    if(hit->getModule()<modL) continue;
    if(hit->getModule()==modL) {
      if(hit->getLayer()!=layL) continue;
      lastHit=hit;
    }
    break;
  }
  if(firstHit==0 || lastHit==0 || firstHit==lastHit) {
    fClusFitSim->setFakeTrack(kFALSE);
    return;
  }
  Double_t x1,y1,z1;
  getGeantHitPoint(firstTrWire,firstHit,x1,y1,z1);
  Double_t x2,y2,z2;
  getGeantHitPoint(lastTrWire,lastHit,x2,y2,z2);
  Double_t xc,yc,zc;
  par.getFirstPlane()->calcIntersection(x1,y1,z1,x2,y2,z2, xc,yc,zc);
  fClusFitSim->setXYZ1Geant(xc,yc,zc);
  par.getSecondPlane()->calcIntersection(x1,y1,z1,x2,y2,z2, xc,yc,zc);
  fClusFitSim->setXYZ2Geant(xc,yc,zc);
}

void HMdcWiresArr::getGeantHitPoint(HMdcWireData* w, HGeantMdc* hit,
    Double_t& x,Double_t& y,Double_t& z) {
  // Transformation of geant mdc hit in coor.sys. of sector.
  // x,y,z - [mm]
  Float_t ax,ay,atof,ptof;
    hit->getHit(ax,ay,atof,ptof);
    x=ax;
    y=ay;
    z=0.;
    w->getSCLayer()->transFrom(x,y,z);
}

void HMdcWiresArr::calcTdcErrorsAndFunct(HMdcTrackParam& par, Int_t iflag) {
  // alphaDrDist and timeOffsets must be calculated befor this function!
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcTdcErrors(par);
  if(fprint) par.saveFunct();
  recalcFunctional(par,iflag);
  if(fprint && iflag>=0) par.printFunctChange(" TdcErr.Recalc.:");
  par.saveFunct();
}

Double_t HMdcWiresArr::valueOfFunctional(HMdcTrackParam& par, Int_t iflag) {
  // iflag:
  //   0 - setting minimal val. of time offset    and no print (default value)
  //   1 - setting minimal val. of time offset    and print if fprint=kTRUE
  //   2 - no setting minimal val. of time offset, calculation of inCell[time]
  //                                              and print if fprint=kTRUE
  calcDriftTime(par,iflag);
  par.calcTimeOffsets(fitInOut->getTofFlag());
  calcFunctional(par,iflag);
  return par.functional();
}

Bool_t HMdcWiresArr::setRegionOfWires(Int_t mod) {
  if(mod<0 || mod>3) {
    firstWire=wires;
    lastWire=wires+nDriftTimes-1;
    nModsInFit=0;
    for(Int_t m=0;m<4;m++) if(nMdcTimes[m]>0) nModsInFit++;
  } else {
    firstWire=wires;
    for(Int_t m=0;m<mod;m++) firstWire += nMdcTimes[m];
    lastWire=firstWire + nMdcTimes[mod]-1;
    nModsInFit=1;
  }
  if(firstWire==0 || lastWire==0) {
    Warning("setFittingTimesList","No fired wires in MDC %i",mod+1);
    return kFALSE;
  }
firstTime=firstWire->getSequentialIndex();
lastTime=lastWire->getSequentialIndex()+1;
  return kTRUE;
}

void HMdcWiresArr::setWeightsTo1or0(Double_t maxChi2, Double_t minWeight) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->setWeightTo1or0(maxChi2,minWeight);
}

void HMdcWiresArr::initDTdPar(Int_t k) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->initDTdPar(k);
}

Int_t HMdcWiresArr::getNWiresInFit(void) {
  return lastWire->getSequentialIndex() - firstWire->getSequentialIndex() + 1;
}

void HMdcWiresArr::calcDTdPar(Int_t k, Double_t oneDv2StepD) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcDTdPar(k,oneDv2StepD);
}

void HMdcWiresArr::calcDriftTime(HMdcTrackParam& par, Int_t iflag) {
  par.clearFunct();
  if(iflag != 2) for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->calcDriftTime(par);
  else for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->calcDriftTimeAndInCell(par);
}

void HMdcWiresArr::calcFunctional(HMdcTrackParam& par, Int_t iflag) {
  if(iflag!=2) par.correctMinTimeOffsets(fitInOut->getMinTOffsetIter());
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcFunctional(par);
  if(fprint && (iflag==1 || iflag==2)) printTimes(iflag,par);
}

void HMdcWiresArr::recalcFunctional(HMdcTrackParam& par, Int_t iflag) {
  // Recalculation of finctional without calculation of ditances
  // (for the same parameters as before)
  // iflag:
  //   0 - setting minimal val. of time offset    and no print (default value)
  //   1 - setting minimal val. of time offset    and print if fprint=kTRUE
  //   2 - no setting minimal val. of time offset and print if fprint=kTRUE
  par.clearFunct();
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->recalcFunctional(par);
  par.calcTimeOffsets(fitInOut->getTofFlag());
  calcFunctional(par,iflag);
}

Double_t HMdcWiresArr::functForDeriv(HMdcTrackParam& par, Int_t iflag) {
  // Calculation of finctional for derivativatives.
  // functional for par must be calc. before this !!!
  // iflag:
  //   !=2 - setting minimal val. of time offset
  //    =2 - no setting minimal val. of time offset
  par.clearFunct();
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcDriftTimeForDeriv(par);
  par.calcTimeOffsets(fitInOut->getTofFlag());
//??? Nado li korektirovat' timeOffset dlya proizvodniyh ???
  if(iflag!=2) par.correctMinTimeOffsets(fitInOut->getMinTOffsetIter());
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcFunctionalForDer(par);
  return par.functional();
}

Double_t HMdcWiresArr::getHijEll(Int_t i,Int_t j) {
  Double_t Hij = 0.0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) Hij += w->getHijEll(i,j);
  return Hij;
}

void HMdcWiresArr::calculateErrors(HMdcTrackParam& par) {
  // calculation of fit parameters errors
  calcSecondDeriv(par);
  Int_t nPar=par.getNumParam();
  for(int i=0;i<nPar;i++) {
    for(int j=i;j<nPar;j++) matrH(i,j)=matrH(j,i)=getHijEll(i,j);
    if(fabs(grad2(i,i))<3.e-16) {
      grad2(i,i)=3.e-16;
      if(matrH(i,i)==0.) matrH(i,i)=1.;
    }
  }
  grad2.InvertFast();
    
  matrH=grad2*matrH*grad2.T(); //!!!  matrH.Similarity(grad2);
  par.fillErrorsMatr(matrH);
  calcTimeOffsetsErr(par);
}

void HMdcWiresArr::calcTimeOffsetsErr(HMdcTrackParam& par) {
  // Calculation of time offset error
  par.clearTimeOffsetErr();
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->addToTOffsetErr(par);
  par.calcTimeOffsetsErr();
  if(fprint) par.printErrors();
}

void HMdcWiresArr::printTimes(Int_t iflag, HMdcTrackParam& par) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->printTime(iflag,par,fitInOut->isGeant());
}

Double_t HMdcWiresArr::getSumOfWeights(void) {
  Double_t sum=0.;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      if(w->getHitStatus()==1) sum+=w->getWeight();
  return sum;
}

Double_t HMdcWiresArr::setTukeyWeightsAndCalcSigma2(Double_t sigma2) {
  Double_t cwSig=sigma2*fitInOut->getTukeyConstWs();
  Double_t c2Sig=sigma2*fitInOut->getTukeyConst2s();
  Double_t c4Sig=sigma2*fitInOut->getTukeyConst4s();
  Double_t sumWt=0.;
  Double_t funct=0.;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->setTukeyWeight(cwSig,c2Sig,c4Sig,sumWt,funct);
  return funct/sumWt;
}

Bool_t HMdcWiresArr::filterOfHits(HMdcTrackParam& par, Int_t iflag) {
  // Filtering of bad hits using Tukey weights
  // return kTRUE new weights was calculated
// seychas eto ne tak!!!
//? Kak chasto nado fil'trovat' ?????
  Double_t sigma2n = par.getNormFunctional();
  if(sigma2n <= fitInOut->getTukeyWtMinSigma2()) {
    if(fprint) printf(" sigma=%f => Not filtering!\n",sqrt(sigma2n));
    return kFALSE;
  }
  Bool_t exitFlag=kTRUE;
  for(Int_t j=0; j<fitInOut->maxNumFilterIter(); j++) {
//    A esli sigma2n <fitInOut->getTukeyWtMinSigma2() pri pervom zhe prohode ???
    if(sigma2n < fitInOut->getTukeyWtMinSigma2()) {
      exitFlag=kFALSE;
//????            =kFALSE - eto bug??? //????? Kak chasto nado fil'trovat' ?????
      break;
    }
//    Mozhet nado delat' tak???
//    if(sigma2n < fitInOut->getTukeyWtMinSigma2()) {
//      sigma2n=fitInOut->getTukeyWtMinSigma2();
//      j=4;
//    }
    sigma2=sigma2n;
    sigma2n=setTukeyWeightsAndCalcSigma2(sigma2);
//Pochemu ne pereschitiyvaetsya funcional dlya kazhdogo j ??? Proverit' !!!
  }
  par.saveFunct();
  recalcFunctional(par,iflag);
  if(fprint) {
    printf(" sigma=%f => FILTER! ",sqrt(sigma2n));
    par.printFunctChange();
  }
  par.saveFunct();
  return exitFlag;
}

void HMdcWiresArr::filterOfHitsConstSig(HMdcTrackParam& par, Double_t sig) {
  // Filtering of bad hits using Tukey weights
  // return kTRUE new weights was calculated
  setTukeyWeightsAndCalcSigma2(sig*sig);
  recalcFunctional(par);
  par.saveFunct();
}

void HMdcWiresArr::setWeightsTo1or0(HMdcTrackParam& par, Int_t iflag) {
  setWeightsTo1or0(fitInOut->getMaxChi2(),fitInOut->getMinWeight());

  if(fprint && iflag>=0) par.saveFunct();
  recalcFunctional(par);
  if(fprint && iflag>=0) par.printFunctChange(" Weights=>0/1:");
  par.saveFunct();

//? poka rabotaet huzhe ???
// recalcFunctional(par);
// calcTdcErrorsAndFunct(par);
// ....
//-----------------------
}

void HMdcWiresArr::setUnitWeights(void) {
  // Setting all weights to 1.
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->setUnitWeight();
}

void HMdcWiresArr::setSizeGrad2Matr(HMdcTrackParam& par) {
  // Seting of sizes of gradient matrix.
  grad2.ResizeTo(par.getNumParam(),par.getNumParam());
  matrH.ResizeTo(par.getNumParam(),par.getNumParam());
}

void HMdcWiresArr::calcDerivatives(HMdcTrackParam& par,Int_t iflag) {
  // Calculation of the fit parameters derivatives
  // iflag<2 - calculate the diagonal elements of the grad2 matrix only
  // else    - calculate the full grad2 matrix
  Double_t funMin=par.functional();
  Double_t stepD=fitInOut->getStepDer(funMin);
  Double_t oneDv2StepD=1./(2.*stepD);
  Double_t oneDvStepD2=1./(stepD*stepD);
  HMdcTrackParam tPar;
  tPar.copyPlanes(par);
  Int_t numOfParam=par.getNumParam();
  agrad=0;
  for(Int_t k = 0; k < numOfParam; k++) {
    Double_t func0 = functForDeriv(tPar(par,k, stepD));
    Double_t func1 = functForDeriv(tPar(par,k,-stepD));
    Double_t grad2kk=(func0 + func1 - 2.0*funMin)*oneDvStepD2;
    grad2(k,k)=grad2kk;
    grad[k] = (func0 - func1)*oneDv2StepD;
    agrad += grad[k]*grad[k];
    if(iflag < 2) continue;
    for(Int_t l = k+1; l < numOfParam; l++) {
      Double_t func2 = functForDeriv(tPar(par,k,stepD,l,stepD));
      Double_t func3 = functForDeriv(tPar(par,l,stepD));
      grad2(k,l)=grad2(l,k)=(funMin - func0 + func2 - func3) * oneDvStepD2;
    }
  }
  agrad=sqrt(agrad);
}

void HMdcWiresArr::calcSecondDeriv(HMdcTrackParam& par) {
  // For errors calculation
  Double_t funMin=par.functional();
  Double_t stepD=fitInOut->getStepDer(funMin);
  Double_t oneDv2StepD=1./(2.*stepD);
  Double_t oneDvStepD2=1./(stepD*stepD);
  Int_t numOfParam=par.getNumParam();
  HMdcTrackParam tPar;
  tPar.copyPlanes(par);
  for(Int_t k = 0; k < numOfParam; k++) {
    Double_t func0 = functForDeriv(tPar(par,k, stepD),2);//functForDeriv(tPar,2);
    initDTdPar(k);
    Double_t func1  = functForDeriv(tPar(par,k,-stepD),2);//functForDeriv(tPar,2);
    calcDTdPar(k,oneDv2StepD);
    grad2(k,k)= (func0 + func1 - 2.0*funMin)*oneDvStepD2;
    for(Int_t l=k+1; l<numOfParam; l++) {
      Double_t func2 = functForDeriv(tPar(par,k,stepD,l,stepD),2);
      Double_t func3 = functForDeriv(tPar(par,l,stepD),2);
      grad2(k,l)=grad2(l,k)=(funMin - func0 + func2 - func3) * oneDvStepD2;
    }
  }
}
