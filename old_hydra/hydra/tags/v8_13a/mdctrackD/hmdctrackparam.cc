//*--- Author : V.Pechenov
//*--- Modified: 16.08.05 V.Pechenov

using namespace std;
#include <iostream>
#include <iomanip>

#include "hmdctrackparam.h"
#include "hmdcclusfit.h"
#include "hmdcgeomobj.h"
#include "hmdcwiredata.h"


//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HMdcTrackParam
//
//    Class keep track fit parameters
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HMdcTrackParam)

HMdcTrackParam::HMdcTrackParam(void) {
  nParam         = 4;
  timeOffsetFlag = 3;
  numOfGoodWires = 0;
  isTmoffFixed   = 0;
  iterNumb       = 0;
  chi2perDF      = -1.;
  funct          = -1.;
  oldFunct       = -1.;
}


HMdcTrackParam::HMdcTrackParam(HMdcTrackParam& tp) {
  numOfGoodWires=0;
  copyParam(tp);
  copyPlanes(tp);
  nParam       = tp.nParam;
  chi2perDF    = tp.chi2perDF;
  isTmoffFixed = tp.isTmoffFixed;
  iterNumb     = tp.iterNumb;
}

HMdcTrackParam::~HMdcTrackParam(void) {
}

void HMdcTrackParam::copyNewParam(const HMdcTrackParam& tp) {
  copyParam(tp);
  copyTimeOffsets(tp);
  funct          = tp.funct;
  iterNumb       = tp.iterNumb;
  numOfGoodWires = tp.numOfGoodWires;
  chi2perDF      = tp.chi2perDF;
  sumWeight      = tp.sumWeight;
  for(Int_t ip=0;ip<nParam;ip++) sWht[ip]=tp.sWht[ip]; //!!!!!!!!!!!!! mozhet perenesti v wires arr.
}

void HMdcTrackParam::copyParAndAdd(const HMdcTrackParam& tp,
    Int_t ip, Double_t add) {
  copyParam(tp);
  addToParam(ip,add);
  isTmoffFixed = tp.isTmoffFixed;
}

void HMdcTrackParam::copyParAndAdd(const HMdcTrackParam& tp,
    Int_t ip1, Double_t add1, Int_t ip2, Double_t add2) {
  copyParam(tp);
  addToParam(ip1,add1,ip2,add2);
  isTmoffFixed = tp.isTmoffFixed;
}

void HMdcTrackParam::clearFunct(void) {
  funct        = 0.;
  sumWeight    = 0.;
  if(isTmoffFixed>0) isTmoffFixed = 0;
  for(Int_t m=0;m<4;m++) {
    sDev[m]=0.;
    sWht[m]=0.;
  }
}

void HMdcTrackParam::calcTimeOffsets(Int_t tofFlag) {
  // tofFlag=1 - calcul. timeOffsets for each Mdc
  // tofFlag=2 - calcul. timeOffsets for each segment
  // else      - calcul. one timeOffset for all mdc's
  if(isTmoffFixed<0) return;
  timeOffsetFlag=tofFlag;
  if(timeOffsetFlag==1) for(Int_t m=0;m<4;m++) timeOffset[m]=(sWht[m]>0.) ?
      -sDev[m]/sWht[m]:0.;
  else if(timeOffsetFlag==2) {
    Double_t tos=(sWht[0]>0. || sWht[1]>0.) ?
        -(sDev[0]+sDev[1])/(sWht[0]+sWht[1]) : 0.;
    timeOffset[0]=(sWht[0]>0.) ? tos : 0.;
    timeOffset[1]=(sWht[1]>0.) ? tos : 0.;
    tos=(sWht[2]>0. || sWht[3]>0.) ?
        -(sDev[2]+sDev[3])/(sWht[2]+sWht[3]) : 0.;
    timeOffset[2]=(sWht[2]>0.) ? tos : 0.;
    timeOffset[3]=(sWht[3]>0.) ? tos : 0.;
  } else {
    Double_t tos=-(sDev[0]+sDev[1]+sDev[2]+sDev[3]) /
                  (sWht[0]+sWht[1]+sWht[2]+sWht[3]);
    for(Int_t m=0;m<4;m++) timeOffset[m]=(sWht[m]>0.) ? tos : 0.; //! ???
  }
  isTmoffFixed=0;
}

void HMdcTrackParam::correctMinTimeOffsets(Double_t minTos) {
  // if timeOffset[m]<minTos timeOffset[m]=minTos
  if(isTmoffFixed<0) return;
  for(Int_t m=0;m<4;m++) if(sWht[m]>0. && timeOffset[m]<minTos) {
    timeOffset[m]=minTos;
    isTmoffFixed |= 1<<m;
  }
}

void HMdcTrackParam::addToTOffsetErr(Int_t m, Double_t* dTdA, Double_t wtNorm) {
  for(Int_t ip=0;ip<nParam;ip++) dTdPar[ip][m]+=dTdA[ip]*wtNorm;
}

void HMdcTrackParam::clearTOffsetDer(void) {
  for(Int_t ip=0;ip<nParam;ip++) for(Int_t m=0;m<4;m++) dTdPar[ip][m]=0.;
}

void HMdcTrackParam::addTimeOffsetDer1(TMatrixD& grad2) {
  if(isTmoffFixed<0) return;
  if(timeOffsetFlag==1) {
    for(Int_t m=0;m<4;m++) if(sWht[m]>0. && !isMdcTimeOffsetFixed(m)) {
      for(Int_t k=0;k<nParam;k++) 
          grad2(k,k) -= 2.*dTdPar[k][m]*dTdPar[k][m]/sWht[m];
    }
  } else if(timeOffsetFlag==2) {
    Double_t sW=sWht[0]+sWht[1];
    if(sW>0. && !isSegTimeOffsetFixed(0)) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP=dTdPar[k][0]+dTdPar[k][1];
        grad2(k,k) -= 2.*dTdP*dTdP/sW;
      }
    }
    sW=sWht[2]+sWht[3];
    if(sW>0. && !isSegTimeOffsetFixed(1)) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP=dTdPar[k][2]+dTdPar[k][3];
        grad2(k,k) -= 2.*dTdP*dTdP/sW;
      }
    }
  } else {
    Double_t sW=sWht[0]+sWht[1]+sWht[2]+sWht[3];
    if(sW>0. && !isTrackTimeOffsetFixed()) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP=dTdPar[k][0]+dTdPar[k][1]+dTdPar[k][2]+dTdPar[k][3];
        grad2(k,k) -= 2.*dTdP*dTdP/sW;
      }
    }
  }
}

void HMdcTrackParam::addTimeOffsetDer2(TMatrixD& grad2) {
  if(isTmoffFixed<0) return;
  if(timeOffsetFlag==1) {
    for(Int_t m=0;m<4;m++) if(sWht[m]>0. && !isMdcTimeOffsetFixed(m)) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP= -2.*dTdPar[k][m]/sWht[m];
        for(Int_t l=0;l<=k;l++) grad2(k,l) += dTdP*dTdPar[l][m];
      }
    }
  } else if(timeOffsetFlag==2) {
    Double_t sW=sWht[0]+sWht[1];
    if(sW>0. && !isSegTimeOffsetFixed(0)) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP= -2.*(dTdPar[k][0]+dTdPar[k][1])/sW;
        for(Int_t l=0;l<=k;l++) grad2(k,l) += dTdP*(dTdPar[l][0]+dTdPar[l][1]);
      }
    }
    sW=sWht[2]+sWht[3];
    if(sW>0. && !isSegTimeOffsetFixed(1)) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP= -2.*(dTdPar[k][2]+dTdPar[k][3])/sW;
        for(Int_t l=0;l<=k;l++) grad2(k,l) += dTdP*(dTdPar[l][2]+dTdPar[l][3]);
      }
    }

  } else {
    Double_t sW=sWht[0]+sWht[1]+sWht[2]+sWht[3];
    if(sW>0. && !isTrackTimeOffsetFixed()) {
      for(Int_t k=0;k<nParam;k++) {
        Double_t dTdP=
            -2.*(dTdPar[k][0]+dTdPar[k][1]+dTdPar[k][2]+dTdPar[k][3])/sW;
        for(Int_t l=0;l<=k;l++) grad2(k,l) +=
            dTdP*(dTdPar[l][0]+dTdPar[l][1]+dTdPar[l][2]+dTdPar[l][3]);
      }
    }
  }
}

void HMdcTrackParam::calcTimeOffsetsErr(void) {
  if(isTmoffFixed<0) return;
  if(timeOffsetFlag==1) for(Int_t m=0;m<4;m++) errTimeOffset[m] = calcTosErr(m);
  else if(timeOffsetFlag==2) {
    Double_t err=calcTosErr(0,1);
    errTimeOffset[0]=(sWht[0]>0.) ? err : 0.;
    errTimeOffset[1]=(sWht[1]>0.) ? err : 0.;
    err=calcTosErr(2,3);
    errTimeOffset[2]=(sWht[2]>0.) ? err : 0.;
    errTimeOffset[3]=(sWht[3]>0.) ? err : 0.;
  } else {
    Double_t err=calcTosErr();
    for(Int_t m=0;m<4;m++) errTimeOffset[m]=(sWht[m]>0.) ? err : 0.;
  }
}

Double_t HMdcTrackParam::calcTosErr(Int_t m) {
  if(isTmoffFixed<0) return 0.;
  if(sWht[m]==0.) return 0.;
  Double_t sum=0.;
  for(int ip=0; ip<nParam; ip++) sum +=
      calcTosErr(sWht[m],dTdPar[ip][m],errMatr(ip,ip));
  return sqrt(sum);
}

Double_t HMdcTrackParam::calcTosErr(Int_t m1, Int_t m2) {
  if(isTmoffFixed<0) return 0.;
  if(sWht[m1]==0. && sWht[m2]==0.) return 0.;
  Double_t sum=0.;
  if(sWht[m1]>0. || sWht[m2]>0.) for(int ip=0; ip<nParam; ip++) sum +=
      calcTosErr(sWht[m1]+sWht[m2],dTdPar[ip][m1]+dTdPar[ip][m2],errMatr(ip,ip));
  return sqrt(sum);
}

Double_t HMdcTrackParam::calcTosErr(void) {
  if(isTmoffFixed<0) return 0.;
  Double_t sum=0.;
  for(int ip=0; ip<nParam; ip++) sum +=
      calcTosErr(sWht[0]+sWht[1]+sWht[2]+sWht[3],
        dTdPar[ip][0]+dTdPar[ip][1]+dTdPar[ip][2]+dTdPar[ip][3],errMatr(ip,ip));
  return sqrt(sum);
}

void HMdcTrackParam::fillErrorsMatr(TMatrixD& matrH) {
  for(Int_t i=0; i<nParam; i++) for(Int_t j=i; j<nParam; j++)
      errMatr.setElement(i,j,matrH(i,j));
}

Double_t HMdcTrackParam::calcChi2PerDF(Int_t nGWires) {
  if(nGWires>0) numOfGoodWires=nGWires;
  chi2perDF = (numOfGoodWires>nParam) ? funct/(numOfGoodWires - nParam) : -1.;
  return chi2perDF;
}

void HMdcTrackParam::printParam(const char* title) const {
  if(title) printf("%s ",title);
  char zf=(funct<=oldFunct) ? '+':'-';
  printf("%3i%c fun.=%5.4g=>%5.4g Par.=%6.2f %6.2f %6.2f %6.2f TOF=%5.1f %5.1f %5.1f %5.1f\n",
      iterNumb,zf,oldFunct,funct,
      point1.X(),point1.Y(),point2.X(),point2.Y(),
      timeOffset[0],timeOffset[1],timeOffset[2],timeOffset[3]);
}

void HMdcTrackParam::printFunctChange(const char* title) const {
  if(title) printf("%s ",title);
  printf("fun.=%-6g->%-6g\n",oldFunct,funct);
}

void HMdcTrackParam::printErrors(void) {
  errMatr.print();
  printf("  timeoffsets:");
  for(Int_t m=0;m<4;m++) if(errTimeOffset[m]>0)
      printf(" mod%i=%g+/-%g",m+1,timeOffset[m],errTimeOffset[m]);
  printf("\n");
}

void HMdcTrackParam::fillClusFit(HMdcClusFit* fClusFit) const {
  fClusFit->setNParam(nParam);
  fClusFit->setFunMin(funct);
  fClusFit->setTimeOff(timeOffset);
  fClusFit->setX1(point1.X());
  fClusFit->setY1(point1.Y());
  fClusFit->setZ1(point1.Z());
  fClusFit->setX2(point2.X());
  fClusFit->setY2(point2.Y());
  fClusFit->setZ2(point2.Z());
  fClusFit->setNumIter(iterNumb);
  fClusFit->setNumOfWires(numOfGoodWires);
}

Bool_t HMdcTrackParam::testParameters(Double_t tosMin, Double_t tosMax) {
  if(isTmoffFixed<0) return kTRUE;
  for(Int_t m=0;m<4;m++)
      if(timeOffset[m]<tosMin||timeOffset[m]>tosMax) return kFALSE;
  return kTRUE;
}

void HMdcTrackParam::setFixedTimeOffset(Double_t o1,Double_t o2,
    Double_t o3,Double_t o4) {
  timeOffset[0]=o1;
  timeOffset[1]=o2;
  timeOffset[2]=o3;
  timeOffset[3]=o4;
  isTmoffFixed=-1;
}
