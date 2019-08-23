//*--- Author : V.Pechenov
//*--- Modified: 16.08.05 V.Pechenov

using namespace std;
#include <iostream>
#include <iomanip>

#include "hmdcwiredata.h"
#include "hmdccal1sim.h"
#include "hmdcdrifttimepar.h"
#include "hmdccal2par.h"
#include "hmdctrackparam.h"
#include "hmdcwirefitsim.h"
#include "hmdcclusfitsim.h"
#include "hmdctrackfitter.h"
#include "hmdcclus.h"
#include "hmatrixcategory.h"
#include "hmdclistcells.h"
#include "hmdctrackdset.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hmdcsizescells.h"

//_HADES_CLASS_DESCRIPTION 
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

HMdcCal2Par* HMdcWireData::pCal2Par=0;
HMdcDriftTimePar* HMdcWireData::pDrTimePar=0;

HMdcWireData::HMdcWireData(void) {
}

void HMdcWireData::setCell(Int_t s, Int_t m, Int_t l, Int_t c, Int_t it,
    Float_t t, Bool_t sSec) {
  sector      = s;
  module      = m;
  layer       = l;
  cell        = c;
  timeIndx    = it;
  tdcTimeCal1 = t;
  tdcTime     = t;
  fMdcCal1    = 0;
  isSecondSec = sSec;
  modInd      = isSecondSec ? module+4 : module;
  setInitVal();
}

void HMdcWireData::setCell(HMdcCal1* cal1, Int_t it, Bool_t isGeant, Bool_t sSec) {
  cal1->getAddress(sector, module,layer,cell);
  timeIndx    = it;
  if(timeIndx==1) tdcTimeCal1 = cal1->getTime1();
  else            tdcTimeCal1 = cal1->getTime2();
  tdcTime     = tdcTimeCal1;
  fMdcCal1    = cal1;
  isSecondSec = sSec;
  modInd      = isSecondSec ? module+4 : module;
  setInitVal();
  if(isGeant) {
    HMdcCal1Sim* c=(HMdcCal1Sim*)cal1;
    if(timeIndx==1) {
      geantTrackNum = c->getNTrack1();
      tof           = c->getTof1();
    } else {
      geantTrackNum = c->getNTrack2();
      tof           = c->getTof2();
    }
  } else geantTrackNum = -1;
}

void HMdcWireData::setInitVal(void) {
  weight           = 1.;
  hitStatus        = 1;
  errTdcTime       = 1.;
  oneDErrTdcTime2  = 1.;
  wtNorm           = weight*oneDErrTdcTime2;
  signalTimeOffset = 0.;
  isWireUsed       = kTRUE;
  distanceSign     = 0;
}

void HMdcWireData::setUnitWeight(void) {
  if(hitStatus==0) return;
  weight           = 1.;
  errTdcTime       = oneDErrTdcTime2 = 1.;
  wtNorm           = weight*oneDErrTdcTime2;
  signalTimeOffset = 0.;
}

void HMdcWireData::setNegTdcTimeTo0(void) {
//  if(tdcTimeCal1<0.) tdcTimeCal1=0.; // dlya CC e+e- ok.; pp???!!!!!!!
  if(tdcTime<0.) tdcTime = 0.;
}

void HMdcWireData::setSignalTimeOffset(Double_t offset) {
  // must be called after setCell !
  signalTimeOffset = offset;
  tdcTime          = tdcTimeCal1-signalTimeOffset;
}

void HMdcWireData::calcTdcErrors(HMdcTrackParam& par) {
  if(hitStatus==0) return;
  if(pCal2Par == 0) {
      errTdcTime      = 1.;
      oneDErrTdcTime2 = 1.;
  } else {
    Double_t distance = pCal2Par->calcDistance(sector,module,alpha,
                                       tdcTime-par.getTimeOffset(modInd));
    if(distance<0) distance=0.;
    errTdcTime        = pDrTimePar->calcTimeErr(sector,module,alpha,distance);
    oneDErrTdcTime2   = 1./(errTdcTime*errTdcTime);
    wtNorm            = weight*oneDErrTdcTime2;
  }
}

Bool_t HMdcWireData::setWeightTo1or0(Double_t maxChi2, Double_t minWeight) {
  isWireUsedPr = isWireUsed;
  if(chi2<maxChi2 || weight>minWeight) {
    setWeightEq1();
    if(hitStatus==1) return kTRUE;
  } else setWeightEq0();
  return kFALSE;
}

void HMdcWireData::setWeightEq1(void) {
  weight     = 1.;
  wtNorm     = oneDErrTdcTime2;
  isWireUsed = kTRUE;
}

void HMdcWireData::setWeightEq0(void) {
  weight     = 0.;
  wtNorm     = 0.;
  isWireUsed = kFALSE;
}

Bool_t HMdcWireData::testWeight1or0(void) {
  return hitStatus!=1 || isWireUsedPr!=isWireUsed;
}

void HMdcWireData::calcDistanceSign(HMdcTrackParam& par) {
  if(hitStatus != 0) distanceSign = pSCLayer->distanceSign(par,cell);
  else distanceSign = 0;
}

void HMdcWireData::calcDriftTime(HMdcTrackParam& par) {
  // calculation of distance to the wire, impact ang. and drift time
  if(hitStatus != 0) {
    distance      = pSCLayer->getImpact(par,cell,alpha,y,z,dirY,dirZ);
//!    driftTime     = pDrTimePar->calcTime(sector,module,alpha,distance);
if(TMath::IsNaN(alpha)) {printf("!!!!!!!!!!! 1. alpha=nan !!!!!!!!!!!!!!!!!\n"); par.printParam();}
    pDrTimeParBin = pDrTimePar->getBin(sector,module,alpha,distance);
    driftTime     = pDrTimeParBin->calcTime(alpha,distance);
    dev0          = driftTime - tdcTime;
    if(hitStatus==1) par.addToSumsDevWt(modInd,dev0,wtNorm);
  }
}

void HMdcWireData::calcDriftTimeAndErr(HMdcTrackParam& par) {
  // calculation of distance to the wire, impact ang., drift time
  // and drift time error
  if(hitStatus != 0) {
    distance        = pSCLayer->getImpact(par,cell,alpha,y,z,dirY,dirZ);
//!    driftTime       = pDrTimePar->calcTime(sector,module,alpha,distance);
if(TMath::IsNaN(alpha)) {printf("!!!!!!!!!!! 2. alpha=nan !!!!!!!!!!!!!!!!!\n"); par.printParam();}
    pDrTimeParBin   = pDrTimePar->getBin(sector,module,alpha,distance);
    driftTime       = pDrTimeParBin->calcTime(alpha,distance);
    dev0            = driftTime - tdcTime;
    
    errTdcTime      = pDrTimeParBin->calcTimeErr(alpha,distance);
    oneDErrTdcTime2 = 1./(errTdcTime*errTdcTime);
    wtNorm          = weight*oneDErrTdcTime2;
    
    if(hitStatus==1) par.addToSumsDevWt(modInd,dev0,wtNorm);
  }
}

void HMdcWireData::calcDriftTimeAndInCell(HMdcTrackParam& par) {
  // calculation of distance to the wire, impact ang. and drift time
  if(hitStatus != 0) {
    inCell        = pSCLayer->getImpact(par,cell,alpha,distance,y,z,dirY,dirZ);
//!    driftTime     = pDrTimePar->calcTime(sector,module,alpha,distance);
if(TMath::IsNaN(alpha)) {printf("!!!!!!!!!!! 3. alpha=nan !!!!!!!!!!!!!!!!!\n"); par.printParam();}
    pDrTimeParBin = pDrTimePar->getBin(sector,module,alpha,distance);
    driftTime     = pDrTimeParBin->calcTime(alpha,distance);
    dev0          = driftTime - tdcTime;
    if(hitStatus==1) par.addToSumsDevWt(modInd,dev0,wtNorm);
  }
}

void HMdcWireData::calcDriftTimeAndErrAndInCell(HMdcTrackParam& par) {
  // calculation of distance to the wire, impact ang., drift time
  // and drift time error
  if(hitStatus != 0) {
    inCell          = pSCLayer->getImpact(par,cell,alpha,distance,y,z,dirY,dirZ);
//!    driftTime       = pDrTimePar->calcTime(sector,module,alpha,distance);
if(TMath::IsNaN(alpha)) {printf("!!!!!!!!!!! 4. alpha=nan !!!!!!!!!!!!!!!!!\n"); par.printParam();}
    pDrTimeParBin   = pDrTimePar->getBin(sector,module,alpha,distance);
    driftTime       = pDrTimeParBin->calcTime(alpha,distance);
    dev0            = driftTime - tdcTime;
    errTdcTime      = pDrTimeParBin->calcTimeErr(alpha,distance);
    oneDErrTdcTime2 = 1./(errTdcTime*errTdcTime);
    wtNorm          = weight*oneDErrTdcTime2;
    if(hitStatus==1) par.addToSumsDevWt(modInd,dev0,wtNorm);
  }
}

void HMdcWireData::calcDriftTimeForDeriv(HMdcTrackParam& par) {
  // calculation drift time for derivatives
  if(hitStatus==1) {
    Double_t alphaD;
    Double_t distD = pSCLayer->getImpact(par,cell,alphaD,y,z,dirY,dirZ);
//!    driftTimeDer=pDrTimePar->calcTime(sector,module,alphaD,distD);
//!!!???    pDrTimeParBin=pDrTimePar->getBin(sector,module,alpha,distD);
if(TMath::IsNaN(alpha)) {printf("!!!!!!!!!!! 5. alpha=nan !!!!!!!!!!!!!!!!!\n"); par.printParam();}
    driftTimeDer   = pDrTimeParBin->calcTime(alphaD,distD);
    devDer         = driftTimeDer - tdcTime;
    par.addToSumsDevWt(modInd,devDer,wtNorm);
  }
}

Double_t HMdcWireData::calcDriftTimeForAlign(const HGeomVector& p1,
    const HGeomVector& p2) {
  // p1,p2 - in ROTATED LAYER COOR.SYS.!!!
  // For calculation of alignment parameters derivatives only
  if(hitStatus != 1) return 0;
  Double_t alphaD;
  Double_t distD  = pSCLayer->getImpactLSys(p1,p2,cell,alphaD);
if(TMath::IsNaN(alphaD)) {printf("!!!!!!!!!!! 6. alpha=nan !!!!!!!!!!!!!!!!!\n");}
  return pDrTimeParBin->calcTime(alphaD,distD);
}

void HMdcWireData::fillLookupTableForDer(HMdcTrackParam& par) {
  Int_t parSec = par.getSec();
  if(parSec==sector || parSec<0)
      fillLookupTableForDer(pSCLayer->getRotLaySysRSec(),par);
  else {
    HGeomTransform trans;
    pSCLayer->getRotLSysForOtherSecSys(parSec,trans);
    fillLookupTableForDer(&trans,par);
  }
}

void HMdcWireData::fillLookupTableForDer(const HGeomTransform* trans,
    HMdcTrackParam& par) { 
  const HGeomRotation& rot=trans->getRotMatrix();
  dYdP[0] = rot(1)+rot(7)*par.dZ1dX1();
  dYdP[1] = rot(4)+rot(7)*par.dZ1dY1();
  dYdP[2] = dYdP[3] = 0.;
  dZdP[0] = rot(2)+rot(8)*par.dZ1dX1();
  dZdP[1] = rot(5)+rot(8)*par.dZ1dY1();
  dZdP[2] = dZdP[3] = 0.;
  
  dDirYdP[0]=rot(1)*par.dDirXdX1()+rot(7)*par.dDirZdX1();
  dDirYdP[1]=rot(4)*par.dDirYdY1()+rot(7)*par.dDirZdY1();
  dDirYdP[2]=rot(1)*par.dDirXdX2()+rot(7)*par.dDirZdX2();
  dDirYdP[3]=rot(4)*par.dDirYdY2()+rot(7)*par.dDirZdY2();
  
  dDirZdP[0]=rot(2)*par.dDirXdX1()+rot(8)*par.dDirZdX1();
  dDirZdP[1]=rot(5)*par.dDirYdY1()+rot(8)*par.dDirZdY1();
  dDirZdP[2]=rot(2)*par.dDirXdX2()+rot(8)*par.dDirZdX2();
  dDirZdP[3]=rot(5)*par.dDirYdY2()+rot(8)*par.dDirZdY2();

  for(Int_t k=0; k<4; k++) {
    cd2DsdP2[k]=dDirZdP[k]*dYdP[k]-dDirYdP[k]*dZdP[k];
    Int_t kn=(k<3) ? k-1:k;
    for(Int_t l=0; l<k; l++) {
      Int_t i=kn+l;
      cd2DsdPkdPl[i]=dDirZdP[l]*dYdP[k]-dDirYdP[l]*dZdP[k];
      cdVkdPl[i]=dDirYdP[l]*dDirZdP[k] - dDirZdP[l]*dDirYdP[k];
      cdUkdPl[i]=dDirZdP[l]*dDirZdP[k] + dDirYdP[l]*dDirYdP[k];
    }
  }
}

void HMdcWireData::calcAnalytDeriv(HMdcTrackParam& par,Int_t flag) {
  // calculation of the first and second derivatives
  // flag=0 - grad calculation only
  // flag=1 - calculation first derivatives only
  // flag=2 - calculation first and second derivatives
  // flag=3 - calculation first and second derivatives +  errors
  if(hitStatus!=1) return;
  Double_t dirZY2    = 1./(dirZ*dirZ+dirY*dirY);
  Double_t dirZY     = TMath::Sqrt(dirZY2); 
  if(y*dirZ < z*dirY) dirZY=-dirZY;        // +sign 
  Double_t yDrYzDrZ  = y*dirY+z*dirZ;
  Double_t c2wtNr    = 2.*wtNorm;
  Double_t c2wtNrDev = c2wtNr*dev;
  Double_t radToDeg  = TMath::RadToDeg();
  if(dirY*dirZ <0.) radToDeg=-radToDeg;    // +sign
  
  Double_t cdAl,cdDs,cdAldDs;
  pDrTimeParBin->coeffForDer(alpha,distance,cdAl,cdDs,cdAldDs);
  Double_t cDb=2*cdAldDs;
  
  Double_t u[4],w[4],dDsdP[4],dAldP[4];
  Double_t dirYn=dirY*dirZY2;
  Double_t dirZn=dirZ*dirZY2;

  for(Int_t k=0;k<4;k++) {
    Double_t& dDsdPk=dDsdP[k];
    Double_t& uk=u[k];
    Double_t& dAldPk=dAldP[k];
    Double_t& dTdPk=dTdP[k];
    
    Double_t vk     = dirYn*dDirZdP[k] - dirZn*dDirYdP[k];
    dDsdPk = (dirZ*dYdP[k]-dirY*dZdP[k]+yDrYzDrZ*vk)*dirZY;      // dDist/dPk^2
    dAldPk = vk*radToDeg;                                        // dAlpha/dPk^2
    dTdPk  = cdDs*dDsdPk + cdAl*dAldPk;                          // dTime/dPk
    if(flag!=3) {
      grad[k] += c2wtNrDev*dTdPk;
      if(flag==0) continue;
    } else (*matrH)(k,k) += 4.*wtNorm*dTdPk*dTdPk;
    par.addToTOffsetDer(modInd,k,dTdPk*wtNorm);
    uk=dirZn*dDirZdP[k]+dirYn*dDirYdP[k];
    Double_t dVdP=-2.*vk*uk;                                     // dV/dPk
    w[k]=dirY*dYdP[k]+dirZ*dZdP[k]+y*dDirYdP[k]+z*dDirZdP[k];    // d(yDrYzDrZ)/dPk
    Double_t d2DsdP=(cd2DsdP2[k]+vk*w[k]+yDrYzDrZ*dVdP)*dirZY - 
        dDsdPk*uk;                                               // d2Dist/dPk^2
    Double_t d2AldP=dVdP*radToDeg;                               // d2Alpha/dPk^2
    Double_t d2TdP2=cdDs*d2DsdP+cdAl*d2AldP+cDb*dAldPk*dDsdPk;   // d2Tm/dPk^2
    (*grad2)(k,k) += c2wtNr*(dev*d2TdP2 + dTdPk*dTdPk);
    
    if(flag<2) continue;
    
    Int_t kn=(k<3) ? k-1:k;
    for(Int_t l=0;l<k;l++) {
      Int_t i=kn+l;
      Double_t dVkdPl     = cdVkdPl[i]*dirZY2 - 2.*vk*u[l];    // dVk/dPl
      Double_t d2DsdPkdPl = (cd2DsdPkdPl[i] + w[l]*vk + 
          yDrYzDrZ*dVkdPl)*dirZY - dDsdPk*u[l];                // d2Dist/dPkdPl
      Double_t dUkdPl     = cdUkdPl[i]*dirZY2 - 2.*vk*u[l];    // dUk/dPl
      Double_t dVkdPkdPl  = -2.*(uk*dVkdPl+vk*dUkdPl);         // d2Vk/dPkdPl
      Double_t d2AlPkdPl  = dVkdPkdPl*radToDeg;                // d2Alpha/dPkdPl
      Double_t d2TdPkdPl  = cdDs*d2DsdPkdPl + cdAl*d2AlPkdPl + 
          cdAldDs*(dAldPk*dDsdP[l] + dDsdPk*dAldP[l]);         // d2Tm/dPkdPl
      (*grad2)(k,l) += c2wtNr*(dev*d2TdPkdPl + dTdPk*dTdP[l]);
      if(flag==3) (*matrH)(k,l) += 4.*wtNorm*dTdPk*dTdP[l];
    }
  }
}

Bool_t HMdcWireData::getAnalytDeriv(Float_t* der,HMdcTrackParam* par) {
  // This funct. is used in alignment only & must be called after track seg.fit.
  // "par" - track parameters after track seg.fit.
  // Return the derivatives dTcalc/dPk. k = 0,1,2,3
  // If par==0 - time offset is assumed constant,
  // else - dTof/dPk is added to dTcalc/dPk.
  // Size of "der" must be >=4 !
  if(hitStatus!=1 || der==0 ) return kFALSE;
  if(par) {
    Double_t dTofdP[4];
    par->getTimeOffsetDer(dTofdP);
    for(Int_t k=0;k<4;k++) der[k] = dTdP[k] + dTofdP[k];
  } else for(Int_t k=0;k<4;k++) der[k] = dTdP[k];
  return kTRUE;
}

void HMdcWireData::recalcFunctional(HMdcTrackParam& par) {
  // Recalculation of finctional without calculation of ditances
  // (for the same parameters as before)
  if(hitStatus!=0) {
    dev0 = driftTime - tdcTime;
    if(hitStatus==1) par.addToSumsDevWt(modInd,dev0,wtNorm);
  }
}

void HMdcWireData::calcFunctional(HMdcTrackParam& par) {
  if(hitStatus!=0) {
    dev = dev0 + par.getTimeOffset(modInd);
    Double_t chi=dev/errTdcTime;
    chi2 = chi*chi;
    if(hitStatus==1) par.addToFunct(chi2*weight,weight);
  }
}

void HMdcWireData::calcFunctionalForDer(HMdcTrackParam& par) {
  if(hitStatus==1) {
    Double_t chi=(devDer+par.getTimeOffset(modInd))/errTdcTime;
    par.addToFunct(chi*chi*weight,weight);
  }
}

void HMdcWireData::calcDevAndFunct(HMdcTrackParam& par) {
  if(hitStatus!=0) {
    dev0         = driftTime - tdcTime;
    dev          = dev0 + par.getTimeOffset(modInd);
    Double_t chi = dev/errTdcTime;
    chi2         = chi*chi;
    if(hitStatus==1) par.addToFunct(chi2*weight,weight);
  }
}

void HMdcWireData::getAddress(Int_t& s,Int_t& m,Int_t& l,Int_t& c,Int_t& it) {
  s = sector;
  m = module;
  l = layer;
  c = cell;
}

Bool_t HMdcWireData::removeIfWeightLess(Double_t wtCut) {
  // function return kTRUE if hit is removed
  if(hitStatus==1 && weight<wtCut) {
    removeThisWire();
    return kTRUE;
  }
  return kFALSE;
}

Int_t HMdcWireData::unsetFittedHit(void) {
  if(hitStatus!=1) return 0;
  lInCl->delTime(module*6+layer,cell,timeIndx);
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
  fWireFit->setCal1Time(tdcTimeCal1);
  fWireFit->setTdcTime(tdcTime);
  fWireFit->setMinDist(distanceSign==-1 ? -distance : distance);
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

void HMdcWireData::removeThisWire(void) {
  // Removing of wire from list of cells and setting hitStatus to -1
  lOutCl->delTime(layer+module*6,cell,timeIndx);
  hitStatus=-1;
}

Bool_t HMdcWireData::removeOneTimeIfItDoubleTime(HMdcWireData* time1) {
  // Removing time1 or time2 from one wire for Double_t hit
  // "this" must be for time2 !
  if(hitStatus!=1 || timeIndx!=2 || lOutCl->getTime(module*6+layer,cell)!=3)
      return kFALSE;
  if(TMath::Abs(time1->dev) <= TMath::Abs(dev)) removeThisWire();
  else time1->removeThisWire();
  return kTRUE;
}

Bool_t HMdcWireData::removeIfOneDistOutCell(HMdcWireData* wire2) {
  // Removing "this" or wire2 from the same layer in "distance" out of cell
  // return kTRUE if one wire is removed
  if(abs(cell-wire2->cell)<=1 || (inCell && wire2->inCell)) return kFALSE;
  if(wire2->inCell) removeThisWire();
  else if(inCell) wire2->removeThisWire();
  else if(wire2->distance<distance) removeThisWire();
  else wire2->removeThisWire();
  return kTRUE;
}

void HMdcWireData::calcDriftTimeAndFunct(HMdcTrackParam& par) {
  // Calculation of functional without recalculation of time offset!
  // (for investigation of finctional)
  if(hitStatus != 0) {
    distance      = pSCLayer->getImpact(par,cell,alpha,y,z,dirY,dirZ);
//    driftTime      =pDrTimePar->calcTime(sector,module,alpha,distance);
if(TMath::IsNaN(alpha)) {printf("!!!!!!!!!!! 7. alpha=nan !!!!!!!!!!!!!!!!!\n"); par.printParam();}
    pDrTimeParBin = pDrTimePar->getBin(sector,module,alpha,distance);
    driftTime     = pDrTimeParBin->calcTime(alpha,distance);
    dev0          = driftTime - tdcTime;
    dev           = dev0 + par.getTimeOffset(modInd);
    Double_t chi  = dev/errTdcTime;
    chi2          = chi*chi;
    if(hitStatus==1) par.addToFunct(chi2*weight,weight);
  }
}

void HMdcWireData::calcDTdPar(Int_t par, Double_t oneDv2Step) {
  if(hitStatus!=1) return;
  dTdPar[par] -= driftTimeDer;
  dTdPar[par] *= oneDv2Step;
}

void HMdcWireData::printIfDeleted(void) const {
  if(weight>0.01 && hitStatus<0)
        printf("Warn.!  HIT %i is deleted\n",sequentialIndex);
}

void HMdcWireData::subtractWireOffset(HMdcTrackParam& par, Double_t sigSpeed) {
  // Subtraction of wire time offset
//! Function getSignPath(,,,,,,,Float_t outside) can be used for fake finding
  signalTimeOffset = pSCLayer->getSignPath(par,cell) * sigSpeed;
  tdcTime          = tdcTimeCal1-signalTimeOffset;
}

void HMdcWireData::addToTOffsetErr(HMdcTrackParam& par) {
  if(hitStatus==1) par.addToTOffsetErr(modInd,dTdPar,wtNorm);
}

void HMdcWireData::printTime(Int_t iflag,HMdcTrackParam& par,Bool_t isGeant) {
  printf("%c%2i)%c %iS%iM%iL%3iC %5.2fmm",(iflag==2) ? '!':' ',sequentialIndex,
      (hitStatus==1)?'+':'-',sector+1,module+1,layer+1,cell+1,distance);
  if(iflag==2) printf(" %c",inCell ? 'I':'O');
  printf(" dev=%5.1f%+4.1f%+6.1f=%5.1fns dT=%4.1f chi2=%6.1f WT=",
      driftTime,par.getTimeOffset(modInd),-tdcTime,dev,errTdcTime,chi2);
  if(weight==0.) printf("0    ");
  else if(weight==1.) printf("1    ");
  else printf("%4.3f",weight);
  
  if(geantTrackNum>0) printf("%5i trk. TOF=%.2f",geantTrackNum,tof);
  printf("\n");
}

Int_t HMdcWireData::setTukeyWeight(Double_t cwSig,Double_t c2Sig,Double_t c4Sig,
    Double_t& sumWt, Double_t& funct) {
  if(hitStatus==0) return 0;
  if(hitStatus!=1 || chi2 >= c2Sig) {
    weight=wtNorm=0.;
    return 0;
  }
  if(chi2 < cwSig) {
    weight = chi2/c4Sig;
    weight = 1. - weight*weight;
  } else weight = 1. - chi2/c2Sig;
  weight *= weight;
  wtNorm  = weight*oneDErrTdcTime2;
  sumWt  += weight;
  funct  += chi2*weight;
  return 1;
}

Int_t HMdcWireData::setTukeyWeight(Double_t cwSig,Double_t c2Sig,Double_t c4Sig) {
  if(hitStatus==0) return 0;
  if(hitStatus!=1 || chi2 >= c2Sig) {
    weight=wtNorm=0.;
    return 0;
  }
  if(chi2 < cwSig) {
    weight = chi2/c4Sig;
    weight = 1. - weight*weight;
  } else weight = 1. - chi2/c2Sig;
  weight *= weight;
  wtNorm  = weight*oneDErrTdcTime2;
  return 1;
}

// Bool_t HMdcWireData::testTukeyWeight(Double_t cwSig,Double_t c2Sig,
//     Double_t c4Sig, Double_t maxChi2, Double_t minWeight) {
//   if(hitStatus!=1 || weight>0.5) return kFALSE;
//   if(chi2 >= c2Sig || chi2>maxChi2) return kFALSE;
//   Double_t wt=0.;
//   if(chi2 < cwSig) {
//     wt=chi2/c4Sig;
//     wt = 1. - wt*wt;
//   } else wt = 1. - chi2/c2Sig;
//   wt*=wt;
//   if(wt>minWeight) return kFALSE;
//   weight=1.;
//   wtNorm=oneDErrTdcTime2;
//   return kTRUE;
// }

// Bool_t HMdcWireData::testTukeyWeight(Double_t cwSig,Double_t c2Sig,
//     Double_t c4Sig, Double_t maxChi2, Double_t minWeight) {
//   if(hitStatus==0) return kFALSE;
//   Double_t wtOld=weight;
// //  if(hitStatus!=1 || weight>0.5) return kFALSE;
//   if(chi2 >= c2Sig || chi2>maxChi2) return kFALSE;
//   Double_t wt=0.;
//   if(chi2 < cwSig) {
//     wt=chi2/c4Sig;
//     wt = 1. - wt*wt;
//   } else wt = 1. - chi2/c2Sig;
//   wt*=wt;
//   if(wt>minWeight && wtOld<0.5) {
//     wtNorm=oneDErrTdcTime2;
//     weight=1.;
// printf("add wire %iM %iL %iC\n",module+1,layer+1,cell+1);
//     return kFALSE;
//   } else if(wt<minWeight && wtOld>0.5) {
//     weight=wtNorm=0.;
// printf("remove wire %iM %iL %iC\n",module+1,layer+1,cell+1);
//     return kFALSE;
//   }
//   return kTRUE;
// }

Bool_t HMdcWireData::isAddressEq(Int_t s, Int_t m, Int_t l, Int_t c) {
  if(s>=0 && s!=sector) return kFALSE;
  if(m>=0 && m!=module) return kFALSE;
  if(l>=0 && l!=layer)  return kFALSE;
  if(c>=0 && c!=cell)   return kFALSE;
  return kTRUE;
}

//-----------------------------------------------------------

HMdcWiresArr::HMdcWiresArr(void) {
  arraySize = 0;
  wires     = 0;
  fClst1    = 0;
  fClst2    = 0;
  fClst3    = 0;
  fClst4    = 0;
  setNumDriftTimes(500);
  fitInOut  = 0;
  sector2   = -1;
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
    for(Int_t i=0;i<arraySize;i++) {
      wires[i].setSequentialIndex(i);
      wires[i].setGradP(grad,&grad2,&matrH);
    }
  }
}

Bool_t HMdcWiresArr::fillListHits(HMdcClus* fCl1, HMdcClus* fCl2,
                                  HMdcClus* fCl3, HMdcClus* fCl4) {
  // Filling list wire for fitting from cluster(s)
  setListCells(fCl1,fCl2,fCl3,fCl4);
  if(getNLayOrientInput()<3) return kFALSE;
  HCategory* fCal1Cat=fitInOut->getMdcCal1Cat();
  Int_t nClTimes=0;
  
  nDrTmFirstSec = 0;
  for(Int_t isec=0;isec<2;isec++) {
    Int_t sec = isec==0 ? sector : sector2;
    if(sec<0) break;
    HMdcSizesCellsMod** fSCModAr=fitInOut->getSCellsModArr(sec);
    locCal1[0] = sec;
    HMdcList24GroupCells& lInCells  = isec==0 ? lInputCells : lInputCells2;
    HMdcList24GroupCells& lOutCells = isec==0 ? lOutputCells : lOutputCells2;
    Bool_t sSecFlag = isec==0 ? kFALSE : kTRUE;
    for(Int_t layer=0;layer<24;layer++) {
      Int_t ilay = layer + 24*isec;
      Int_t nDrTimes = lInCells.getNDrTimes(layer);
      if(nDrTimes<=0) {
        firstWireInLay[ilay]=lastWireInLay[ilay]=0;
        continue;
      }
      firstWireInLay[ilay] = wires+nClTimes;
      lastWireInLay[ilay]  = wires+nClTimes+nDrTimes-1;
      Int_t modr = layer/6;
      Int_t lay  = layer%6;
      locCal1[1] = modr;
      locCal1[2] = lay;
      Int_t cell = -1;
      if(fSCModAr[modr]==0) return kFALSE;
      HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
      while((cell=lInCells.next(layer,cell)) > -1) {
        Int_t nTimes=lInCells.getTime(layer,cell);
        locCal1[3]    = cell;
        HMdcCal1* cal = (HMdcCal1*)fCal1Cat->getObject(locCal1);
        if(!cal) {
          Error("fillListHits","S.%i M.%i L.%i  Cell %i is not fired",
              sec+1,modr+1,lay+1,cell+1);
          return kFALSE;
        }
        for(Int_t time=1; time<3; time++) {
          if((time&nTimes) == 0) continue;
          wires[nClTimes].setCell(cal,time,fitInOut->isGeant(),sSecFlag);
          wires[nClTimes].setSizesCellsLayer(&fSCLay);
          wires[nClTimes].setCellsLists(&lInCells,&lOutCells);
          nClTimes++;
          if(isec==0) nDrTmFirstSec++;
        }
      }
    }
  }
  
  if(nDriftTimes!=nClTimes) return kFALSE;
  return kTRUE;
}

void HMdcWiresArr::setNegTdcTimeTo0(void) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->setNegTdcTimeTo0(); 
}

Bool_t HMdcWiresArr::fillListHits(HMdcEvntListCells* store,
    HMdcClus* fCl1, HMdcClus* fCl2) {
  // Filling list wire for fitting from cluster(s),
  // drift times from HMdcEvntListCells
  setListCells(fCl1,fCl2);
  HMdcSecListCells& storeSec=(*store)[sector];
  HMdcSizesCellsMod** fSCModAr=fitInOut->getSCellsModArr(sector);
  locCal1[0] = sector;
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
    Int_t lay  = layer%6;
    locCal1[1] = modr;
    locCal1[2] = lay;
    Int_t cell = -1;
    if(fSCModAr[modr]==0) return kFALSE;
    HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
    HMdcLayListCells& storeLay=storeSec[modr][lay];
    while((cell=lInputCells.next(layer,cell)) > -1) {
      if(lInputCells.getTime(layer,cell) == 0) continue;
      wires[nClTimes].setCell(sector,modr,lay,cell,1,storeLay.getTimeValue(cell)); // index of time =1 !!!!???
      wires[nClTimes].setSizesCellsLayer(&fSCLay);
      wires[nClTimes].setCellsLists(&lInputCells,&lOutputCells);
      nClTimes++;
    }
  }
  if(nDriftTimes!=nClTimes) return kFALSE;
  return kTRUE;
}

void HMdcWiresArr::setListCells(HMdcClus* fCl1, HMdcClus* fCl2,
                                HMdcClus* fCl3, HMdcClus* fCl4) {
  sigma2  = -1.;
  fClst1  = fCl1;
  fClst2  = fCl2;
  sector  = fClst1->getSec();
  segment = fClst1->getIOSeg();
  if(fClst2 != 0) segment = 3;
  if(segment==0) lInputCells.set(fClst1,0);
  else if(segment==1) lInputCells.set(0,fClst1);
  else lInputCells.set(fClst1,fClst2);
  xClst        = fClst1->getX();
  yClst        = fClst1->getY();
  zClst        = fClst1->getZ();
  lOutputCells = lInputCells;
  nDriftTimes  = 0;
  for(Int_t m=0;m<4;m++) nDriftTimes +=
                            nMdcTimes[m] = lInputCells.getNDrTimesMod(m);
  
  // For cosmic:
  fClst3  = fCl3;
  fClst4  = fCl4;
  sector2 = -1;
  if(fClst3 != 0) {
    sector2  = fClst3->getSec();
    segment2 = fClst3->getIOSeg();
    if(fClst4 != 0) segment2 = 3;
    if(segment2==0) lInputCells2.set(fClst3,0);
    else if(segment2==1) lInputCells2.set(0,fClst3);
    else lInputCells2.set(fClst3,fClst4);
    lOutputCells2 = lInputCells2;
    for(Int_t m=0;m<4;m++) nDriftTimes +=
                            nMdcTimes[m+4] = lInputCells2.getNDrTimesMod(m);
  }
  
  setNumDriftTimes(nDriftTimes);
  firstTime = 0;
  lastTime  = nDriftTimes;
  firstWire = wires;
  lastWire  = wires+nDriftTimes-1;
}

Bool_t HMdcWiresArr::fillListHits(HMdcEvntListCells* store) {
  // Filling list wire for fitting from cluster(s),
  // drift times from HMdcEvntListCells
  setListCells(store);
  HMdcSecListCells& storeSec=(*store)[sector];
  HMdcSizesCellsMod** fSCModAr=fitInOut->getSCellsModArr(sector);
  locCal1[0]     = sector;
  Int_t nClTimes = 0;
  for(Int_t layer=0;layer<24;layer++) {
    Int_t nDrTimes = lInputCells.getNDrTimes(layer);
    if(nDrTimes<=0) {
      firstWireInLay[layer]=lastWireInLay[layer]=0;
      continue;
    }
    firstWireInLay[layer] = wires+nClTimes;
    lastWireInLay[layer]  = wires+nClTimes+nDrTimes-1;
    Int_t modr = layer/6;
    Int_t lay  = layer%6;
    locCal1[1] = modr;
    locCal1[2] = lay;
    Int_t cell = -1;
    if(fSCModAr[modr]==0) return kFALSE;
    HMdcSizesCellsLayer& fSCLay=(*(fSCModAr[modr]))[lay];
    HMdcLayListCells& storeLay=storeSec[modr][lay];
    while((cell=lInputCells.next(layer,cell)) > -1) {
      if(lInputCells.getTime(layer,cell) == 0) continue;
      wires[nClTimes].setCell(sector,modr,lay,cell,1,storeLay.getTimeValue(cell)); // index of time =1 !!!!???
      wires[nClTimes].setSizesCellsLayer(&fSCLay);
      wires[nClTimes].setCellsLists(&lInputCells,&lOutputCells);
      nClTimes++;
    }
  }
  if(nDriftTimes!=nClTimes) return kFALSE;
  return kTRUE;
}

void HMdcWiresArr::setListCells(HMdcEvntListCells* store) {
  Int_t s,m,l,c;
  s=m=l=c=-1;
  UChar_t t   = 0;
  nDriftTimes = 0;
  sector2     = -1;
  lInputCells.clear();
  while(store->nextCell(s, m, l, c, t)) {
    sector = s;
    lInputCells.setTime(m*6+l,c,t);
  }
  for(Int_t m=0;m<4;m++) nDriftTimes+=nMdcTimes[m]=lInputCells.getNDrTimesMod(m);
  if(nMdcTimes[2]+nMdcTimes[3] == 0)      segment = 0;
  else if(nMdcTimes[0]+nMdcTimes[1] == 0) segment = 1;
  else                                    segment = 3;  
  setNumDriftTimes(nDriftTimes);
  nDrTmFirstSec = nDriftTimes;
  firstTime     = 0;
  lastTime      = nDriftTimes;
  firstWire     = wires;
  lastWire      = wires+nDriftTimes-1;
  lOutputCells  = lInputCells;
}

void HMdcWiresArr::fillLookupTableForDer(HMdcTrackParam& par) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->fillLookupTableForDer(par);
}
    
Double_t HMdcWiresArr::testFitResult(void) {
  // Removing cells which has small weight or
  // if min.distance outside of this cell for case when more than on wires
  // in this layer passed fit and so on
// Can be tuned and checked ???).
  Double_t sumDelWeight=0.;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    // Test for Tukey weight:
    if(w->removeIfWeightLess(fitInOut->getWeightCut())) {
        sumDelWeight+=w->getWeight();
        numOfGoodWires--;
      //Test on Double_t time (time==2 - weight are tested already):
    } else if(w>firstWire && w->removeOneTimeIfItDoubleTime(w-1)) {
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
          if(fWire->removeIfOneDistOutCell(lWire)) {
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
    if(w->isAddressEq(sec,mod,lay,cell)) {
      w->setHitStatus(-2);
      w->removeThisWire();
  }
}

Int_t HMdcWiresArr::unsetHits(void) {
  // removing cells passed fit
  Int_t nFitedTimes=0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      nFitedTimes +=w->unsetFittedHit();
  return nFitedTimes;
}

Int_t HMdcWiresArr::getNCellsInInput(Int_t mod,Int_t isec) {
  // Return number of cells in mdc module =mod in input list
  // if mod<0 return number of cells in all mdc's of segment
  // isec - sector index (0 or 1) for cosmic data only
  // isec<0 && mod<0 return number of cells in all mdc's of two sectors
  if(mod<0) {
    if(isec < 0 && sector2 >= 0)  return lInputCells.getNCells()+
                                         lInputCells2.getNCells();
    if(isec == 1 && sector2 >= 0) return lInputCells2.getNCells();
    return lInputCells.getNCells();
  }
  if(isec == 1) return lInputCells2.getNCells(mod*6,mod*6+5);
  return lInputCells.getNCells(mod*6,mod*6+5);
}

UChar_t HMdcWiresArr::getNLayOrientInput(void) {
  // Return list of layer: 6 bits, one bit per layer
  UChar_t listLayers = 0;
  for(Int_t mod=0;mod<4;mod++) {
    listLayers                |= lInputCells.getListLayers(mod);
    if(sector2>=0) listLayers |= lInputCells2.getListLayers(mod);
  }
  return HMdcBArray::getNLayOrientation(listLayers);
}

UChar_t HMdcWiresArr::getNLayOrientOutput(void) {
  // Return list of layer: 6 bits, one bit per layer
  UChar_t listLayers = 0;
  for(Int_t mod=0;mod<4;mod++) {
    listLayers                |= lOutputCells.getListLayers(mod);
    if(sector2>=0) listLayers |= lOutputCells2.getListLayers(mod);
  }
  return HMdcBArray::getNLayOrientation(listLayers);
}

Int_t HMdcWiresArr::getNCellsInOutput(Int_t mod,Int_t isec) {
  // Return valid value after calling testFitResult() only !
  // Return number of cells in mdc module =mod passed fit
  // if mod<0 return number of cells in all mdc's of segment
  if(mod<0) {
    if(isec < 0 && sector2 >= 0)  return lOutputCells.getNCells()+
                                         lOutputCells2.getNCells();
    if(isec == 1 && sector2 >= 0) return lOutputCells2.getNCells();
    return lOutputCells.getNCells();
  }
  if(isec == 1) return lOutputCells2.getNCells(mod*6,mod*6+5);
  return lOutputCells.getNCells(mod*6,mod*6+5);
}

Bool_t HMdcWiresArr::calcNGoodWiresAndChi2(HMdcTrackParam& par) {
  numOfGoodWires=0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      if(w->isPassedFit()) numOfGoodWires++;
  return par.calcChi2PerDF(numOfGoodWires) >= 0.;
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
  fClusFit->setSigmaChi2(sigma2>=0. ? TMath::Sqrt(sigma2):-1.);
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

  if(firstTrWire==0 || lastTrWire==0) {
    fClusFitSim->setXYZ1Geant(-1000.,-1000.,-1000.);
    fClusFitSim->setXYZ2Geant(-1000.,-1000.,-1000.);
    Error("fillClusFitSim","Sec.%i Seg.%i bad track parameters or weights!",
        sector+1,segment+1);
    par.printParam("Bad track:");
    return;
  }
  
  if(fitInOut->getGeantKineCat()==0 || trackNum[indxMax]<1) return;
  HGeantKine* fGeantKine=(HGeantKine*)fitInOut->getGeantKineCat()->
      getObject(trackNum[indxMax]-1);
  if(fGeantKine==0) return;
  fClusFitSim->setParticleID(fGeantKine->getID());
  fClusFitSim->setMomentum(fGeantKine->getTotalMomentum());
  fClusFitSim->setPrimaryFlag(fGeantKine->isPrimary());
  if(fitInOut->getGeantMdcCat()==0) return;

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
  if(fprint) fClusFitSim->printSimVsRec();
}

void HMdcWiresArr::getGeantHitPoint(HMdcWireData* w, HGeantMdc* hit,
    Double_t& x,Double_t& y,Double_t& z) {
  // Transformation of geant mdc hit in coor.sys. of sector.
  // x,y,z - [mm]
  Float_t ax,ay,atof,ptof;
  hit->getHit(ax,ay,atof,ptof);
  x = ax;
  y = ay;
  z = w->getSCLayer()->getZGeantHits();
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

Double_t HMdcWiresArr::valueOfFunctAndErr(HMdcTrackParam& par, Int_t iflag) {
  // iflag:
  //   0 - setting minimal val. of time offset    and no print (default value)
  //   1 - setting minimal val. of time offset    and print if fprint=kTRUE
  //   2 - no setting minimal val. of time offset, calculation of inCell[time]
  //                                              and print if fprint=kTRUE
  calcDriftTimeAndErr(par,iflag);
  par.calcTimeOffsets(fitInOut->getTofFlag());
  calcFunctional(par,iflag);
  return par.functional();
}

Bool_t HMdcWiresArr::setRegionOfWires(Int_t mod,Int_t isec) {
  if(mod<0 || mod>3) {
    Int_t mInd1 = 0;
    if(isec==1 && sector2>=0) mInd1 = 4;
    Int_t mInd2 = 4;
    if(isec!=0 && sector2>=0) mInd2 = 8;
    
    firstWire  = wires;
    if(mInd1==4) firstWire += nMdcTimes[0]+nMdcTimes[1]+
                              nMdcTimes[2]+nMdcTimes[3];
    lastWire   = wires+nDriftTimes-1;
    if(isec==0 && sector2>=0) lastWire -= nMdcTimes[4]+nMdcTimes[5]+
                                          nMdcTimes[6]+nMdcTimes[7];
    nModsInFit = 0;
    for(Int_t m=mInd1;m<mInd2;m++) if(nMdcTimes[m]>0) nModsInFit++;
  } else {
    Int_t mInd = mod;
    if(isec == 1 && sector2>=0) mInd +=4;
    firstWire=wires;
    for(Int_t m=0;m<mInd;m++) firstWire += nMdcTimes[m];
    lastWire   = firstWire + nMdcTimes[mInd]-1;
    nModsInFit = 1;
  }
  if(firstWire==0 || lastWire==0) {
    Warning("setFittingTimesList","No fired wires in MDC %i",mod+1);
    return kFALSE;
  }
  firstTime = firstWire->getSequentialIndex();
  lastTime  = lastWire->getSequentialIndex()+1;
  return kTRUE;
}

void HMdcWiresArr::setWeightsTo1or0(Double_t maxChi2, Double_t minWeight) {
  Int_t   nDrTimes = 0;
  UChar_t nLayList= 0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    if(w->setWeightTo1or0(maxChi2,minWeight)) {
      nDrTimes++;
      nLayList |= 1<<w->getLayer();
    }
  }
  if(nDrTimes<5) {
    Int_t ntm=5-nDrTimes;
    for(Int_t nw=0;nw<ntm;nw++) {
      HMdcWireData* wmin=findMinChi2();
      if(wmin==0) {
        Error("setWeightsTo1or0","Number of wires for fit=%i",nDrTimes);
        return;
      }
      wmin->setWeightEq1();
      nLayList |= 1<<wmin->getLayer();
      nDrTimes++;
    }
  }
  
  while(HMdcBArray::getNLayOrientation(nLayList) < 3) {
    HMdcWireData* wmin = findMinChi2();
    if(wmin==0) {
      Error("setWeightsTo1or0","Number of wires orientations = %i (<3)",
          HMdcBArray::getNLayOrientation(nLayList));
      return;
    }
    wmin->setWeightEq1();
    nLayList |= 1<<wmin->getLayer();
  }
}

Bool_t HMdcWiresArr::testTukeyWeights(HMdcTrackParam& par) {
  filterOfHitsV2(par); 
  setWeightsTo1or0(fitInOut->getMaxChi2(),fitInOut->getMinWeight());
  Bool_t exitFlag=kFALSE;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) if(w->testWeight1or0()) {
    if(fprint) printf("Reset: %is%im%il%3ic %s\n",w->getSector()+1,w->getModule()+1,
        w->getLayer()+1,w->getCell()+1,w->getIsWireUsed() ? "ON":"OFF");
    exitFlag=kTRUE;
  }
  recalcFunctional(par);
  par.saveFunct();
  return exitFlag;
}

HMdcWireData* HMdcWiresArr::findMinChi2(void) {
  // finding wire with minimal chi2
  HMdcWireData* wmin = 0; 
  Double_t chi2min   = 1.e+200;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    if(w->getIsWireUsed() || w->getHitStatus()!=1) continue;
    Double_t chi2=w->getChi2();
    if(chi2>chi2min) continue;
    wmin    = w;
    chi2min = chi2;
  }
  return wmin;
}

void HMdcWiresArr::initDTdPar(Int_t k) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->initDTdPar(k);
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

void HMdcWiresArr::calcDriftTimeAndErr(HMdcTrackParam& par, Int_t iflag) {
  par.clearFunct();
  if(iflag != 2) for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->calcDriftTimeAndErr(par);
  else for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      w->calcDriftTimeAndErrAndInCell(par);
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
  // Calculation of finctional for derivatives.
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

void HMdcWiresArr::calcDistanceSign(HMdcTrackParam& par) {
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcDistanceSign(par);
}

Bool_t HMdcWiresArr::calcErrorsAnalyt(HMdcTrackParam& par) {
  // calculation of fit parameters errors analyticaly
  matrH.Zero();
  grad2.Zero();
  par.clearTOffsetDer();
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcAnalytDeriv(par,3);
  par.addTimeOffsetDer2(grad2);
  if(!calcErrs(par)) return kFALSE;
  par.calcTimeOffsetsErr();
  if(fprint) par.printErrors();
  return kTRUE;
}

Bool_t HMdcWiresArr::calculateErrors(HMdcTrackParam& par) {
  // calculation of fit parameters errors numer.
  calcSecondDeriv(par);
  if(!calcErrs(par)) return kFALSE;
  calcTimeOffsetsErr(par);
  if(fprint) par.printErrors();
  return kTRUE;
}

Bool_t HMdcWiresArr::calcErrs(HMdcTrackParam& par) {
  Int_t numOfParam=par.getNumParam();
  for(Int_t k=0; k<numOfParam; k++) {
    if(TMath::Abs(grad2(k,k))<3.e-16) {
      grad2(k,k)=3.e-16;
      if(matrH(k,k)==0.) matrH(k,k)=1.;
    }
    for(Int_t l=0; l<k; l++) {
      grad2(l,k) = grad2(k,l);
      matrH(l,k) = matrH(k,l);
    }
  }
  grad2.InvertFast();
  if(!grad2.IsValid()) {
    Error("calcErrs","Inverted matrix is not valid!");
    grad2.MakeValid();
    return kFALSE;
  }
  matrH=grad2*matrH*grad2.T(); //!!!  matrH.Similarity(grad2);
  if(!matrH.IsValid()) {
    Error("calcErrs","Errors matrix is not valid!");
    matrH.MakeValid();
    return kFALSE;
  }
  par.fillErrorsMatr(matrH);
  return kTRUE;
}

void HMdcWiresArr::calcTimeOffsetsErr(HMdcTrackParam& par) {
  // Calculation of time offset error
  par.clearTOffsetDer();
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
      if(w->getHitStatus()==1) sum += w->getWeight();
  return sum;
}

Double_t HMdcWiresArr::findNewSigm2(Int_t ntm) {
  Double_t chi2buf[ntm];
  Int_t nwr=0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) {
    if(w->getWeight()>0. || w->getHitStatus()!=1) continue;
    Double_t chi2 = w->getChi2();
    if(nwr==0 || chi2>=chi2buf[nwr-1]) {
      if(nwr<ntm) {
        chi2buf[nwr++] = chi2;
      }
    } else {
      for(Int_t i=0;i<nwr;i++) if(chi2<chi2buf[i]) {
        Int_t j=(nwr<ntm) ? nwr:ntm-1;
        for(;j>i;j--) chi2buf[j]=chi2buf[j-1];
        chi2buf[i]=chi2;
        if(nwr<ntm) nwr++;
        break;
      }
    }
  }
  Double_t sigma2 = (nwr==ntm) ? chi2buf[nwr-1]*1.1 : 1000000; // +10%
  return sigma2/fitInOut->getTukeyConst2s();
}

Double_t HMdcWiresArr::setTukeyWeightsAndCalcSigma2(Double_t sigma2) {
  Double_t cwSig  = sigma2*fitInOut->getTukeyConstWs();
  Double_t c2Sig  = sigma2*fitInOut->getTukeyConst2s();
  Double_t c4Sig  = sigma2*fitInOut->getTukeyConst4s();
  Double_t sumWt  = 0.;
  Double_t funct  = 0.;
  Int_t    nwires = 0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      nwires += w->setTukeyWeight(cwSig,c2Sig,c4Sig,sumWt,funct);
  
  if(nwires>=5) return funct/sumWt;
  return setTukeyWeightsAndCalcSigma2(findNewSigm2(5-nwires)); //???
}

Bool_t HMdcWiresArr::setTukeyWeights(Double_t sigma2) {
  // return kTRUE if sigma2 was increased due to small amount of wires
  // after filtering (<5)
  Double_t cwSig  = sigma2*fitInOut->getTukeyConstWs();
  Double_t c2Sig  = sigma2*fitInOut->getTukeyConst2s();
  Double_t c4Sig  = sigma2*fitInOut->getTukeyConst4s();
  Int_t    nwires = 0;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++)
      nwires += w->setTukeyWeight(cwSig,c2Sig,c4Sig);
  
  if(nwires>=5) return kFALSE;
  setTukeyWeights(findNewSigm2(5-nwires)); //???
  return kTRUE;
}

Bool_t HMdcWiresArr::filterOfHits(HMdcTrackParam& par, Int_t iflag) {
  // Filtering of bad hits using Tukey weights
  // return kTRUE new weights was calculated
// seychas eto ne tak!!!
//? Kak chasto nado fil'trovat' ?????
  if(getNWiresInFit()<=5) {
    if(fprint) printf(" num.wires=%i => No filtering!\n",getNWiresInFit());
    return kFALSE;
  }
  Double_t sigma2n = par.getNormFunctional();
  if(sigma2n <= fitInOut->getTukeyWtMinSigma2()) {
    if(fprint) printf(" sigma=%f => Not filtering!\n",TMath::Sqrt(sigma2n));
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
    sigma2  = sigma2n;
    sigma2n = setTukeyWeightsAndCalcSigma2(sigma2);
    if(sigma2n>sigma2) break;
//Pochemu ne pereschitiyvaetsya funcional dlya kazhdogo j ??? Proverit' !!!
  }
  par.saveFunct();
  recalcFunctional(par,iflag);
  if(fprint) {
    printf(" sigma=%f => FILTER! ",TMath::Sqrt(sigma2n));
    par.printFunctChange();
  }
  par.saveFunct();
  return exitFlag;
}

Bool_t HMdcWiresArr::filterOfHitsV2(HMdcTrackParam& par, Int_t iflag) {
  // Filtering of bad hits using Tukey weights
  // return kTRUE new weights was calculated
// seychas eto ne tak!!!
//? Kak chasto nado fil'trovat' ?????
  if(getNWiresInFit()<=5) {
    if(fprint) printf(" num.wires=%i => No filtering!\n",getNWiresInFit());
    return kFALSE;
  }
  Double_t sigma2n = par.getNormFunctional();
  if(sigma2n <= fitInOut->getTukeyWtMinSigma2()) {
    if(fprint) printf(" sigma=%f => No filtering!\n",TMath::Sqrt(sigma2n));
    return kFALSE;
  }
  Int_t maxIter=fitInOut->maxNumFilterIter()-1;
  Int_t iter=0;
  par.saveFunct();
  for(; iter<=maxIter; iter++) {
    sigma2=sigma2n;
    Bool_t exit = setTukeyWeights(sigma2);
    recalcFunctional(par);
    sigma2n=par.getNormFunctional();
    if(sigma2n < fitInOut->getTukeyWtMinSigma2() || exit) break;
  }
  if(fprint) {
    if(iflag>0) printTimes(iflag,par);
    printf(" sigma=%f => FILTER! ",TMath::Sqrt(sigma2n));
    par.printFunctChange();
  }
  return iter >= maxIter;
}

void HMdcWiresArr::filterOfHitsConstSig(HMdcTrackParam& par, Double_t sig) {
  // Filtering of bad hits using Tukey weights
  // return kTRUE new weights was calculated
  if(getNWiresInFit()<=5) {
    if(fprint) printf(" num.wires=%i => No filtering!\n",getNWiresInFit());
    return;
  }
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
  // Numerical calculation of the fit parameters derivatives
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
    grad2(k,k)=(func0 + func1 - 2.0*funMin)*oneDvStepD2;
    grad[k] = (func0 - func1)*oneDv2StepD;
    agrad += grad[k]*grad[k];
    if(iflag < 2) continue;
    for(Int_t l = k+1; l < numOfParam; l++) {
      Double_t func2 = functForDeriv(tPar(par,k,stepD,l,stepD));
      Double_t func3 = functForDeriv(tPar(par,l,stepD));
      grad2(k,l)=(funMin - func0 + func2 - func3) * oneDvStepD2;
    }
  }
  agrad=TMath::Sqrt(agrad);
}

void HMdcWiresArr::calcAnalyticDerivatives1(HMdcTrackParam& par) {
  // Analytical calculation of the fit parameters derivatives 
  // for the first min.method:
  // calculate the first der. and the diagonal elements of the grad2 matrix
  Int_t numOfParam=par.getNumParam();
  for(Int_t n=0; n<numOfParam; n++) grad[n]=0.;
  grad2.Zero();
  par.clearTOffsetDer();
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcAnalytDeriv(par,1);
  par.addTimeOffsetDer1(grad2);
  // dTof/dP == 0 - no need to correct grad!
  agrad=0;
  for(Int_t k=0; k<numOfParam; k++) agrad += grad[k]*grad[k];
  agrad=TMath::Sqrt(agrad);
}

Double_t HMdcWiresArr::calcAGradAnalyt(HMdcTrackParam& par) {
  for(Int_t k=0; k<4; k++) grad[k]=0.;
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcAnalytDeriv(par,0);
  Double_t agrd=0;
  for(Int_t k=0; k<4; k++) agrd += grad[k]*grad[k];
  return TMath::Sqrt(agrd);
}

void HMdcWiresArr::calcAnalyticDerivatives2(HMdcTrackParam& par) {
  // Analytical calculation of the fit parameters derivatives 
  // for the second min.method.
  // Calculate the first der. and all elements of the grad2 matrix
  Int_t numOfParam=par.getNumParam();
  for(Int_t n=0; n<numOfParam; n++) grad[n]=0.;
  grad2.Zero();
  par.clearTOffsetDer();
  for(HMdcWireData* w=firstWire;w<=lastWire;w++) w->calcAnalytDeriv(par,2);
  par.addTimeOffsetDer2(grad2);
  agrad=0;
  for(Int_t k=0; k<numOfParam; k++) {
    agrad += grad[k]*grad[k];
    for(Int_t l=0; l<k; l++) grad2(l,k)=grad2(k,l);
  }
  agrad=TMath::Sqrt(agrad);
}

void HMdcWiresArr::calcSecondDeriv(HMdcTrackParam& par) {
  // For errors calculation
  Double_t funMin      = par.functional();
  Double_t stepD       = fitInOut->getStepDer(funMin);
  Double_t oneDv2StepD = 1./(2.*stepD);
  Double_t oneDvStepD2 = 1./(stepD*stepD);
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
      grad2(k,l) = grad2(l,k) = (funMin - func0 + func2 - func3) * oneDvStepD2;
    }
  }
}

void HMdcWiresArr::setInitWeghts(HMdcTrackParam& par) {
  Int_t layMax = sector2<0 ? 24 : 48;
  for(Int_t layer=0;layer<layMax;layer++) {
    if(firstWireInLay[layer]==0 || lastWireInLay[layer]==0) continue;
    if(firstWireInLay[layer]==lastWireInLay[layer]) continue;
    Int_t nWires=0;
    HMdcWireData* fWr=firstWireInLay[layer];
    HMdcWireData* lWr=lastWireInLay[layer];
    for(HMdcWireData* w=fWr;w<=lWr;w++) if(w->getHitStatus() == 1) nWires++;
    if(nWires<=2) continue;
    HMdcSizesCellsLayer* pSCLayer=fWr->getSCLayer();
    Float_t cell1,cell2;
    pSCLayer->calcCrossedCells(par.x1(),par.y1(),par.z1(),
        par.x2(),par.y2(),par.z2(),cell1,cell2);
    Double_t cell=(cell1+cell2)/2.;
    for(HMdcWireData* w=fWr;w<=lWr;w++) {
      if(w->getHitStatus() != 1) continue;
      Double_t dCell=TMath::Abs(w->getCell()-cell);
      if(dCell>1.) w->setWeightAndWtNorm(w->getWeight()/dCell);
    }
  }
}

Int_t HMdcWiresArr::getOutputNLayers(void) const {
  Int_t nLays = lOutputCells.getNLayers();
  if(sector2>=0) nLays += lOutputCells2.getNLayers();
  return nLays;
}
