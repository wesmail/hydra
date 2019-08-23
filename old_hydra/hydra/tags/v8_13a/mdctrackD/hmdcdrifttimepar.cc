//*--- AUTHOR : Vladimir Pechenov
//*--- Modified : 18/08/05 V.Pechenov

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcDriftTimePar
//
// Container class for the distance->time parameterization.
// Contains functions for calculating distance->time for track fitter.
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcdrifttimepar.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hruntimedb.h"
#include "hmdccal2parsim.h"
#include "hmdcgetcontainers.h"

ClassImp(HMdcDriftTimeParBin)
ClassImp(HMdcDriftTimeParAngle)
ClassImp(HMdcDriftTimeParMod)
ClassImp(HMdcDriftTimeParSec)
ClassImp(HMdcDriftTimePar)



HMdcDriftTimeParBin::HMdcDriftTimeParBin(void) {
  c0=c1=c2=c3=0.;
}

void HMdcDriftTimeParBin::setParam(Double_t k0, Double_t k1, Double_t k2,
    Double_t k3) {
  c0=k0;
  c1=k1;
  c2=k2;
  c3=k3;
}

void HMdcDriftTimeParBin::print(void) const {
  printf("DriftTime = %.5g + %.5g*alpha + %.5g*dist + %.5g*alpha*dist\n",
      c0,c1,c2,c3);
  printf("ErrDriftTime = %.5g + %.5g*alpha + %.5g*dist + %.5g*alpha*dist\n",
      c0err,c1err,c2err,c3err);
}

void HMdcDriftTimeParBin::printTable(void) const {
  printf("| %10.4g | %10.4g | %10.4g | %10.4g || %10.4g | %10.4g | %10.4g | %10.4g |\n",
      c0,c1,c2,c3,c0err,c1err,c2err,c3err);
}

HMdcDriftTimeParAngle::HMdcDriftTimeParAngle(Int_t nb) : TObjArray(nb) {
  for(Int_t i=0; i<nb; ++i) AddAt(new HMdcDriftTimeParBin(),i);
}

HMdcDriftTimeParAngle::~HMdcDriftTimeParAngle(void) {
  // destructor
  Delete();
}

HMdcDriftTimeParMod::HMdcDriftTimeParMod(Int_t nAnBins, Int_t nDsBins) :
    TObjArray(nAnBins) {
  // Constructor create array of HMdcDriftTimeParAngle objects.
  // One object per one impact angle bin.
  for(Int_t i=0; i<nAnBins; ++i) AddAt(new HMdcDriftTimeParAngle(nDsBins),i);
  nAngleBins=nAnBins;
  lAngleBin=nAngleBins-1;
  angleBinSize=90./nAngleBins;
  nDistBins=nDsBins;
  lDistBin=nDistBins-1;
  setDistBinSize(0.1); // default bin size
}

void HMdcDriftTimeParMod::setDistBinSize(Double_t dBSize) {
  distBinSize=dBSize;
  lastBinDist=lDistBin*distBinSize;
}

Bool_t HMdcDriftTimeParMod::initContainer(HMdcCal2ParModSim& fC2PModS,
    Double_t slOut) {
  slopeOutside=slOut;
  for(Int_t anBin=0; anBin<nAngleBins; anBin++) {
    for(Int_t dsBin=0; dsBin<nDistBins; dsBin++) {
      HMdcDriftTimeParBin* bin=at(anBin)->at(dsBin);
      if(bin==0) return kFALSE;
      HMdcCal2ParAngleSim& rAng1=fC2PModS[anBin];// pointer to the first set
      HMdcCal2ParAngleSim& rAng2=(anBin<lAngleBin) ? fC2PModS[anBin+1]:rAng1;
      
      fillDriftTime(rAng1,rAng2,bin,anBin,dsBin);
      fillDriftTimeErr(rAng1,rAng2,bin,anBin,dsBin);
    }
  }
  return kTRUE;
}

void HMdcDriftTimeParMod::print(void) {
  for(Int_t anBin=0; anBin<nAngleBins; anBin++) {
    for(Int_t dsBin=0; dsBin<nDistBins; dsBin++) {
      HMdcDriftTimeParBin* bin=at(anBin)->at(dsBin);
      if(bin==0) continue;
//      printf("ang.bin=%i dist.bin=%i: ",anBin,dsBin);
      printf("%3i   %3i ",anBin,dsBin);
      bin->printTable();
    }
  }
}

void HMdcDriftTimeParMod::fillDriftTime(HMdcCal2ParAngleSim& rAng1,
    HMdcCal2ParAngleSim& rAng2, HMdcDriftTimeParBin* bin,
    Int_t anBin, Int_t dsBin) {
  Double_t k0=90./angleBinSize-anBin;
  if(dsBin==lDistBin) {  // distance outside matrix
    Double_t y1=rAng1.getDriftTime1(lDistBin);
    Double_t y2=rAng2.getDriftTime1(lDistBin);
    Double_t k1=y1-y2;
    bin->setC0(y1-lastBinDist*slopeOutside - k0*k1);
    bin->setC1(k1/angleBinSize);
    bin->setC2(slopeOutside);
    bin->setC3(0.);
  } else {
    Double_t y1=rAng1.getDriftTime1(dsBin);
    Double_t y2=rAng2.getDriftTime1(dsBin);
    Double_t y3=rAng2.getDriftTime1(dsBin+1);
    Double_t y4=rAng1.getDriftTime1(dsBin+1);
    Double_t k1=y1-y2;
    Double_t k2=y1-y4;
    Double_t k3=k1+y3-y4;
    k1+=dsBin*k3;
    bin->setC0(y1 + dsBin*k2 - k0*k1);
    bin->setC1(k1/angleBinSize);
    bin->setC2((k0*k3-k2)/distBinSize);
    bin->setC3(-k3/distBinSize/angleBinSize);
  }
}

void HMdcDriftTimeParMod::fillDriftTimeErr(HMdcCal2ParAngleSim& rAng1,
    HMdcCal2ParAngleSim& rAng2, HMdcDriftTimeParBin* bin,
    Int_t anBin, Int_t dsBin) {
  Double_t k0=90./angleBinSize-anBin;
  if(dsBin==lDistBin) {  // distance outside matrix
    Double_t y1=rAng1.getDriftTime1Error(lDistBin);
    Double_t y2=rAng2.getDriftTime1Error(lDistBin);
    Double_t k1=y1-y2;
    bin->setC0Err(y1 - k0*k1);
    bin->setC1Err(k1/angleBinSize);
    bin->setC2Err(0.);
    bin->setC3Err(0.);
  } else {
    Double_t y1=rAng1.getDriftTime1Error(dsBin);
    Double_t y2=rAng2.getDriftTime1Error(dsBin);
    Double_t y3=rAng2.getDriftTime1Error(dsBin+1);
    Double_t y4=rAng1.getDriftTime1Error(dsBin+1);
    Double_t k1=y1-y2;
    Double_t k2=y1-y4;
    Double_t k3=k1+y3-y4;
    k1+=dsBin*k3;
    bin->setC0Err(y1 + dsBin*k2 - k0*k1);
    bin->setC1Err(k1/angleBinSize);
    bin->setC2Err((k0*k3-k2)/distBinSize);
    bin->setC3Err(-k3/distBinSize/angleBinSize);
  }
}

Bool_t HMdcDriftTimeParMod::testSizes(Int_t anSz, Int_t dsSz) const {
  return (anSz!=nAngleBins || dsSz!=nDistBins) ? kFALSE:kTRUE;
}

HMdcDriftTimeParMod::~HMdcDriftTimeParMod(void) {
  // destructor
  Delete();
}

HMdcDriftTimeParSec::HMdcDriftTimeParSec(void) : TObjArray(4) {
  // Constructor create TObjArray for HMdcDriftTimeParMod objects.
}

Bool_t HMdcDriftTimeParSec::initContainer(HMdcCal2ParSecSim& fC2PSecS,
    Int_t sec, Double_t slOut) {
  sector=sec;
  HMdcDetector* pMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  for(Int_t m=0; m<4; m++) {
    Int_t nDistBins=100;         //!!!!!
    Double_t distBinSize=0.1;    //!!!!!
    if(pMdcDet->getModule(sec,m) <= 0) continue;
    HMdcCal2ParModSim& fC2PModS=fC2PSecS[m];
    if(&fC2PModS == 0) return kFALSE;
    Int_t nAngleBins=fC2PModS.getSize();
    if(At(m)==0) AddAt(new HMdcDriftTimeParMod(nAngleBins,nDistBins),m);
    else if(!at(m)->testSizes(nAngleBins,nDistBins)) {
      delete at(m);
      AddAt(new HMdcDriftTimeParMod(nAngleBins,nDistBins),m);
    }
    at(m)->setDistBinSize(distBinSize);
    if(!at(m)->initContainer(fC2PModS,slOut)) return kFALSE;
  }
  return kTRUE;
}

void HMdcDriftTimeParSec::print(void) {
  HMdcDetector* pMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  for(Int_t m=0; m<4; m++) {
    if(pMdcDet->getModule(sector,m) <= 0) continue;
    printf("\n HMdcDriftTimePar: %i sector  %i module \n",sector+1,m+1);
    printf("---bin---------------------------------------------------------");
    printf(          "-----------------------------------------------------\n");
    printf("ang. dis. |     c0     |     c1     |     c2     |     c3     |");
    printf(          "|    c0err   |    c1err   |    c2err   |    c3err   |\n");
    printf("----------|------------|------------|------------|------------|");
    printf(          "|------------|------------|------------|------------|\n");
    at(m)->print();
  }
}

HMdcDriftTimeParSec::~HMdcDriftTimeParSec(void) {
  // destructor
  Delete();
}

HMdcDriftTimePar::HMdcDriftTimePar(void) : TObjArray(6) { // 6 sectors
  // Constructor create TObjArray for HMdcDriftTimeParSec objects.
  pCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->
      getContainer("MdcCal2ParSim");
}

Bool_t HMdcDriftTimePar::initContainer(void) {
  if(!pCal2ParSim || !HMdcGetContainers::isInited(pCal2ParSim)) return kFALSE;
  HMdcDetector* pMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if(pMdcDet==0) return kFALSE;
  Double_t slopeOutside=pCal2ParSim->getSlopeOutside();
  for(Int_t s=0; s<6; s++) {
    if( !pMdcDet->isSectorActive(s) ) continue;
    HMdcCal2ParSecSim& fC2PSecS=(*pCal2ParSim)[s];
    if(&fC2PSecS == 0) return kFALSE;
    if(At(s)==0) AddAt(new HMdcDriftTimeParSec(),s);
    if(!at(s)->initContainer(fC2PSecS,s,slopeOutside)) return kFALSE;
  }
  return kTRUE;
}

void HMdcDriftTimePar::print(void) {
  HMdcDetector* pMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if(pMdcDet==0) return;
  for(Int_t s=0; s<6; s++) {
    if( !pMdcDet->isSectorActive(s) ) continue;
    at(s)->print();
  }
}

HMdcDriftTimePar::~HMdcDriftTimePar(void) {
  // destructor
  Delete();
}
