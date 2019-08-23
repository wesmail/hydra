//*--- Author : V.Pechenov
//*--- Modified: 16.08.05 V.Pechenov

using namespace std;
#include <iostream>
#include <iomanip>

#include "hmdctrackparam.h"
#include "hmdcclusfit.h"
#include "hmdcgeomobj.h"
#include "hmdcwiredata.h"
#include <TMatrixD.h>


//////////////////////////////////////////////////////////////////////////////
//
//  HMdcTrackParam
//
//    Class keep track fit parameters
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HMdcTrackParam)

HMdcTrackParam::HMdcTrackParam(void) {
  nParam=4;
  timeOffsetFlag=3;
  chi2perDF=-1.;
}


HMdcTrackParam::HMdcTrackParam(HMdcTrackParam& tp) {
  copyParam(tp);
  copyPlanes(tp);
  nParam=tp.nParam;
  chi2perDF=tp.chi2perDF;
}

HMdcTrackParam::~HMdcTrackParam(void) {
}

void HMdcTrackParam::copyParAndAdd(const HMdcTrackParam& tp,
    Int_t ip, Double_t add) {
  copyParam(tp);
  addToParam(ip,add);
}

void HMdcTrackParam::copyParAndAdd(const HMdcTrackParam& tp,
    Int_t ip1, Double_t add1, Int_t ip2, Double_t add2) {
  copyParam(tp);
  addToParam(ip1,add1,ip2,add2);
}

void HMdcTrackParam::clearFunct(void) {
  funct=0.;
  sumWeight=0.;
  for(Int_t m=0;m<nParam;m++) {
    sDev[m]=0.;
    sWht[m]=0.;
  }
}

void HMdcTrackParam::calcTimeOffsets(Int_t tofFlag) {
  // tofFlag=1 - calcul. timeOffsets for each Mdc
  // tofFlag=2 - calcul. timeOffsets for each segment
  // else      - calcul. one timeOffset for all mdc's
  timeOffsetFlag=tofFlag;
  if(timeOffsetFlag==1) for(Int_t m=0;m<nParam;m++) timeOffset[m]=(sWht[m]>0.) ?
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
    for(Int_t m=0;m<nParam;m++) timeOffset[m]=(sWht[m]>0.) ? tos : 0.; //! ???
  }
}

void HMdcTrackParam::correctMinTimeOffsets(Double_t minTos) {
  // if timeOffset[m]<minTos timeOffset[m]=minTos
  for(Int_t m=0;m<nParam;m++) if(sWht[m]>0. && timeOffset[m]<minTos)
    timeOffset[m]=minTos;
}

void HMdcTrackParam::clearTimeOffsetErr(void) {
  for(Int_t m=0;m<nParam;m++) for(Int_t n=0;n<nParam;n++) sc2m[n][m]=0.;
}

void HMdcTrackParam::addToTOffsetErr(Int_t m, Double_t* dTdA, Double_t wtNorm) {
  for(Int_t ip=0;ip<nParam;ip++) sc2m[ip][m]+=dTdA[ip]*wtNorm;
}

void HMdcTrackParam::addToTOffsetErr(HMdcWireData& wire, Double_t* dTdA) {
  if(wire.getHitStatus()==1)
      addToTOffsetErr(wire.getModule(),dTdA,wire.getWtNorm());
}

void HMdcTrackParam::addToTOffsetErr(HMdcWireData& wire) {
  if(wire.getHitStatus()==1)
      addToTOffsetErr(wire.getModule(),wire.getDTdParArr(),wire.getWtNorm());
}

void HMdcTrackParam::calcTimeOffsetsErr(void) {
  if(timeOffsetFlag==1) for(Int_t m=0;m<nParam;m++)
      errTimeOffset[m] = calcTosErr(m);
  else if(timeOffsetFlag==2) {
    Double_t err=calcTosErr(0,1);
    errTimeOffset[0]=(sWht[0]>0.) ? err : 0.;
    errTimeOffset[1]=(sWht[1]>0.) ? err : 0.;
    err=calcTosErr(2,3);
    errTimeOffset[2]=(sWht[2]>0.) ? err : 0.;
    errTimeOffset[3]=(sWht[3]>0.) ? err : 0.;
  } else {
    Double_t err=calcTosErr();
    for(Int_t m=0;m<nParam;m++) errTimeOffset[m]=(sWht[m]>0.) ? err : 0.;
  }
}

Double_t HMdcTrackParam::calcTosErr(Int_t m) {
  if(sWht[m]==0.) return 0.;
  Double_t sum=0.;
  for(int ip=0; ip<nParam; ip++) sum +=
      calcTosErr(sWht[m],sc2m[ip][m],errMatr(ip,ip));
  return sqrt(sum);
}

Double_t HMdcTrackParam::calcTosErr(Int_t m1, Int_t m2) {
  if(sWht[m1]==0. && sWht[m2]==0.) return 0.;
  Double_t sum=0.;
  if(sWht[m1]>0. || sWht[m2]>0.) for(int ip=0; ip<nParam; ip++) sum +=
      calcTosErr(sWht[m1]+sWht[m2],sc2m[ip][m1]+sc2m[ip][m2],errMatr(ip,ip));
  return sqrt(sum);
}

Double_t HMdcTrackParam::calcTosErr(void) {
  Double_t sum=0.;
  for(int ip=0; ip<nParam; ip++) sum +=
      calcTosErr(sWht[0]+sWht[1]+sWht[2]+sWht[3],
        sc2m[ip][0]+sc2m[ip][1]+sc2m[ip][2]+sc2m[ip][3],errMatr(ip,ip));
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
  printf("%3i%c fun.=%-6g->%-6g Par.=%-6g %-6g %-6g %-6g TOF=%-6g %-6g %-6g %-6g\n",
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
  for(Int_t m=0;m<nParam;m++) if(errTimeOffset[m]>0)
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

Bool_t HMdcTrackParam::testParameters(Double_t threshold, 
    Double_t tosMin, Double_t tosMax) {
  if(funct>threshold) return kFALSE;
  for(Int_t m=0;m<nParam;m++)
      if(timeOffset[m]<tosMin||timeOffset[m]>tosMax) return kFALSE;
  return kTRUE;
}
