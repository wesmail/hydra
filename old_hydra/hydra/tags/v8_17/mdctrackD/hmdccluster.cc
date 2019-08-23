using namespace std;
#include "hmdccluster.h"
//#include "TObjArray.h"
#include "TMath.h"
#include "hmdcclussim.h"
#include "hmdcgeanttrack.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 15/02/2007 by V. Pechenov

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcCluster
//
// Temporary cluster information for cluster finder
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcCluster)
ClassImp(HMdcClustersArrs)

void HMdcCluster::clear(Char_t seg) {
  status      = kTRUE;
  nMergedClus = 1;
  clusMerg    = 0;
  segment     = seg;
  lCells1.clear();
  lCells2.clear();
  clearBinStat();
}

void HMdcCluster::clearBinStat(void) {
  nBins     = 0;
  meanX     = 0.;
  meanY     = 0.;
  meanXX    = 0.;
  meanYY    = 0.;
  meanYX    = 0.;
  sumWt     = 0.;
  meanXWt   = 0.;
  meanYWt   = 0.;
  meanXXWt  = 0.;
  meanYYWt  = 0.;
  clusMod1  = 0;
  clusMod2  = 0;
  flag      = 0;
  minWt     = 1000;
  numCells1 = -1;
  numCells2 = -1;
}

void HMdcCluster::setModCluster(Int_t im, HMdcCluster* cl) {
  if(im==0)      clusMod1 = cl;
  else if(im==1) clusMod2 = cl;
}

void HMdcCluster::addClus(HMdcCluster& clst2) {
  // seg!=0 and seg!=1 - inner and outer segments
  if(segment != 1) lCells1.add(&(clst2.lCells1));
  if(segment != 0) lCells2.add(&(clst2.lCells2));
  clst2.status   = kFALSE;
  clst2.clusMerg = this;
  sumWt         += clst2.sumWt;
  nBins         += clst2.nBins;
  meanX         += clst2.meanX;
  meanY         += clst2.meanY;
  meanXX        += clst2.meanXX;
  meanYY        += clst2.meanYY;
  meanYX        += clst2.meanYX;
  meanXWt       += clst2.meanXWt;
  meanYWt       += clst2.meanYWt;
  meanXXWt      += clst2.meanXXWt;
  meanYYWt      += clst2.meanYYWt;
  nMergedClus   += clst2.nMergedClus;
  if(minWt > clst2.minWt) minWt = clst2.minWt;
  calcXY();
}

void HMdcCluster::calcXY(void) {
  Double_t norm = minWt-1.;
  if(norm<0.1) {
    x=meanXWt/sumWt;
    y=meanYWt/sumWt;
  } else {
    x=(meanXWt-meanX*norm)/(sumWt-nBins*norm);
    y=(meanYWt-meanY*norm)/(sumWt-nBins*norm);
  }
}

void HMdcCluster::sumClus(HMdcCluster& clst1, HMdcCluster& clst2) {
  lCells1=clst1.lCells1;
  lCells1.add(&(clst2.lCells1));
  if(segment != 0) {
    lCells2=clst1.lCells2;
    lCells2.add(&(clst2.lCells2));
  }
  clst1.flag = 1;
  clst2.flag = 1;
//  clst2.status=kFALSE; ?
//  clst2.clusMerg=this; ?
  sumWt        = clst1.sumWt+clst2.sumWt;
  nBins        = clst1.nBins+clst2.nBins;
  meanX        = clst1.meanX+clst2.meanX;
  meanY        = clst1.meanY+clst2.meanY;
  meanXX       = clst1.meanXX+clst2.meanXX;
  meanYY       = clst1.meanYY+clst2.meanYY;
  meanYX       = clst1.meanYX+clst2.meanYX;
  meanXWt      = clst1.meanXWt+clst2.meanXWt;
  meanYWt      = clst1.meanYWt+clst2.meanYWt;
  meanXXWt     = clst1.meanXXWt+clst2.meanXXWt;
  meanYYWt     = clst1.meanYYWt+clst2.meanYYWt;
  nMergedClus += clst2.nMergedClus;  //??????????????
  
  minWt        = clst1.minWt <= clst2.minWt ? clst1.minWt : clst2.minWt; //???
  calcXY();
}

void HMdcCluster::calcClusParam(Double_t xSt2, Double_t ySt2) {
  Double_t norm = minWt-1.;
  if(minWt<0.1) {
    errX=sqrt(meanXXWt/sumWt-x*x + xSt2);
    errY=sqrt(meanYYWt/sumWt-y*y + ySt2);
  } else {
    errX=sqrt((meanXXWt-meanXX*norm)/(sumWt-nBins*norm)-x*x + xSt2);
    errY=sqrt((meanYYWt-meanYY*norm)/(sumWt-nBins*norm)-y*y + ySt2);
  }
  // Cluster shape:-------------------------------------------------
  //  eXX,eYY,eXY=eYX - covariance matrix componets
  //
  //    | eXX-sigma   aXY       |           | E1 |
  // A= |                       |         B=|    |
  //    | eYX         eYY-sigma |           | E2 |
  //
  // A^2=0;  ==> sigma1 & sigma2  - (sigma1 > sigma2)
  //
  //    | 0 |        | E1 |
  // AB=|   |;   ==> |    | - direction of main axis
  //    | 0 |        | E2 |
  if(nBins<2) {
    sigma1=sigma2=0.;
    alpha=0.;
  } else {
    Double_t nBn=nBins;
    Double_t eXX = meanXX*nBn- meanX*meanX;
    Double_t eYX = meanYX*nBn- meanY*meanX;
    Double_t eYY = meanYY*nBn- meanY*meanY;
    Double_t norm = (eXX+eYY)/2.;
    Double_t eYX2 = eYX*eYX;
    Double_t c=eXX*eYY-eYX2;
    Double_t sigma1D=norm+sqrt(norm*norm-c);
    Double_t sigma2D=norm-sqrt(norm*norm-c);
    Double_t e1=sigma1D-eXX;
    Double_t e2=sigma1D-eYY;
    sigma1=sqrt(sigma1D)/nBn;
    sigma2=(sigma2D<10e-5) ? 0. : sqrt(sigma2D)/nBn;
    alpha=atan2(sqrt(eYX2+e1*e1),sqrt(eYX2+e2*e2))*TMath::RadToDeg();
    if(eYX<0.) alpha=180.-alpha;
  }
}

void HMdcCluster::fillClus(HMdcClus* fClus, Int_t nLst, Bool_t fillTrList) {
  fClus->setSumWt(sumWt);
  fClus->setNBins(nBins);
  fClus->setXY(x,errX,y,errY);
  fClus->setNMergClust(nMergedClus);
  fClus->setRealLevel((UChar_t)(minWt+0.1));
  HMdcList12GroupCells& list=(nLst==0) ? lCells1:lCells2;
  HMdcTrackInfSim* clusSimInf = 0;
  if(fillTrList && fClus->isGeant())
      clusSimInf = HMdcGeantEvent::getMdcTrackInfSim();
  if(clusSimInf) clusSimInf->fillClusTrackInf((HMdcClusSim*)fClus);
  if(clusMod1==0) {
    Int_t nDrTm=list.getNDrTimes(0,5);
    if(nDrTm>0 && fClus->getMinCl(0)>0) {
      fClus->setClFnLevelM1(fClus->getMinCl(0));
      fClus->setClusSizeM1(nBins);
      fClus->setNMergClustM1(nMergedClus);
      fClus->setShapeM1(sigma1,sigma2,alpha);
      if(clusSimInf) clusSimInf->fillClusModTrackInf((HMdcClusSim*)fClus,
                                                     &list,0);
    } else fClus->clearMod1Par();
    fClus->setNDrTimesM1(nDrTm);
  } else {
    fClus->setClFnLevelM1((UChar_t)(clusMod1->minWt+0.1));
    fClus->setClusSizeM1(clusMod1->nBins);
    fClus->setNDrTimesM1(clusMod1->lCells1.getNDrTimes(0,5));
    fClus->setNMergClustM1(clusMod1->nMergedClus);
    fClus->setShapeM1(clusMod1->sigma1,clusMod1->sigma2,clusMod1->alpha);
    if(clusSimInf) clusSimInf->fillClusModTrackInf((HMdcClusSim*)fClus,
                                                   &(clusMod1->lCells1),0);
  }
  if(clusMod2==0) {
    Int_t nDrTm=list.getNDrTimes(6,11);
    if(nDrTm>0 && fClus->getMinCl(1)>0) {
      fClus->setClFnLevelM2(fClus->getMinCl(1));
      fClus->setClusSizeM2(nBins);
      fClus->setNMergClustM2(nMergedClus);
      fClus->setShapeM2(sigma1,sigma2,alpha);
      if(clusSimInf) clusSimInf->fillClusModTrackInf((HMdcClusSim*)fClus,
                                                     &list,1);
    } else fClus->clearMod2Par();
    fClus->setNDrTimesM2(nDrTm);
  } else {
    fClus->setClFnLevelM2((UChar_t)(clusMod2->minWt+0.1));
    fClus->setClusSizeM2(clusMod2->nBins);
    fClus->setNDrTimesM2(clusMod2->lCells1.getNDrTimes(6,11));
    fClus->setNMergClustM2(clusMod2->nMergedClus);
    fClus->setShapeM2(clusMod2->sigma1,clusMod2->sigma2,clusMod2->alpha);
    if(clusSimInf) clusSimInf->fillClusModTrackInf((HMdcClusSim*)fClus,
                                                   &(clusMod2->lCells1),1);
  }
}

void HMdcCluster::print(void) {
  // Debug print:
  printf("Cluster: status=%s, flag=%i,nMergedClus=%i  x=%.1f y=%.1f\n",
      status ? "kTRUE" : "kFALSE",flag,nMergedClus,x,y);
  if(lCells1.getNCells()>0) {
    printf(" - - - - - - - lCells1:\n");
    lCells1.print();
  }
  if(lCells2.getNCells()>0) {
    printf(" - - - - - - - lCells2:\n");
    lCells2.print();
  }
}

Bool_t HMdcCluster::testForInclude(HMdcCluster& cl2) {
  // Function test are wires from one cluster fully included in another cluster.
  // Index of "this" obj. in array must be < index cl2 !!!
  // return kTRUE if "cl2" is included to "this" (STOP LOOP)
  // return kFALSE cl2 is not included to "this" or "this" is included to "cl2"
  if(numCells1<0)         numCells1 = lCells1.getNCells();
  if(numCells2<0)         numCells2 = lCells2.getNCells();
  if(cl2.numCells1<0) cl2.numCells1 = cl2.lCells1.getNCells();
  if(cl2.numCells2<0) cl2.numCells2 = cl2.lCells2.getNCells();
  Int_t inclFlag = 0;
  if(segment == 0) {
    if(numCells1 == cl2.numCells1) {
      if(lCells1.isIncluded(cl2.lCells1))      inclFlag=1; // add cl2 to "this"
    } else if(numCells1 < cl2.numCells1) {
      if(lCells1.isIncluded(cl2.lCells1))      inclFlag=2; // replace "this"
    } else if(cl2.lCells1.isIncluded(lCells1)) inclFlag=3; // delete cl2
  } else if(segment == 1) {
    if(numCells2 == cl2.numCells2) {
      if(lCells2.isIncluded(cl2.lCells2))      inclFlag=1; // add cl2 to "this"
    } else if(numCells2 < cl2.numCells2) {
      if(lCells2.isIncluded(cl2.lCells2))      inclFlag=2; // replace "this"
    } else if(cl2.lCells2.isIncluded(lCells2)) inclFlag=3; // delete cl2
  } else {
    if(numCells1+numCells2 == cl2.numCells1+cl2.numCells2) {
      if(lCells1.isIncluded(cl2.lCells1) && 
        lCells2.isIncluded(cl2.lCells2))        inclFlag=1; // add cl2 to "this"
    } else if(numCells1+numCells2 < cl2.numCells1+cl2.numCells2) {
      if(lCells1.isIncluded(cl2.lCells1) &&
         lCells2.isIncluded(cl2.lCells2))       inclFlag=2; // replace "this"
    } else if(cl2.lCells1.isIncluded(lCells1) &&
              cl2.lCells2.isIncluded(lCells2))  inclFlag=3; // delete cl2
  }
  
  if(inclFlag==0) return kFALSE; // "cl2" cluster will used.      Continue loop!
  if(inclFlag==3) {              // "cl2" cluster will not used.  Stop loop!
    cl2.status   = kFALSE;
    cl2.clusMerg = this;
    return kTRUE;
  }
  if(inclFlag==2) {              // "this" cluster will not used. Continue loop!
    status   = kFALSE;
    clusMerg = &cl2;
    return kFALSE;
  }
  // inclFlag==1 :
  addClus(cl2);                  // "cl2" is added to "this".     Stop loop!
  return kTRUE;
}

//===HMdcClustersArrs===========================================================

HMdcClustersArrs* HMdcClustersArrs::pMdcClustersArrs = 0;

HMdcClustersArrs* HMdcClustersArrs::getObject(void) {
  if(!pMdcClustersArrs) pMdcClustersArrs = new HMdcClustersArrs();
  return pMdcClustersArrs;
}

void HMdcClustersArrs::deleteCont(void) {
  if(pMdcClustersArrs != 0) delete pMdcClustersArrs;
}

HMdcClustersArrs::HMdcClustersArrs(void) {
  for(Int_t i=0;i<3;i++) {
    clusArr[i] = 0;
    arrSize[i] = 0;
  }
}

HMdcClustersArrs::~HMdcClustersArrs(void) {
  if(this == pMdcClustersArrs) pMdcClustersArrs = 0;
  for(Int_t i=0;i<3;i++) if(clusArr[i]) {
    delete [] clusArr[i];
    clusArr[i] = 0;
  }
}

Int_t HMdcClustersArrs::createAllArrays(Int_t size) {
  if(createArray(0,size) == 0) return 0;
  size = arrSize[0];
  if(createArray(1,size) == 0) return 0;
  size = arrSize[1];
  if(createArray(2,size) == 0) return 0;
  return size;
}

HMdcCluster* HMdcClustersArrs::createArray(Int_t ind,Int_t size) {
  // return pointer to the array of HMdcCluster objects
  // if array is not created it will be created
  if(clusArr[ind] != 0) size = arrSize[ind];
  else if(size>0) {
    clusArr[ind] = new HMdcCluster [size];
    arrSize[ind] = size;
  }
  return clusArr[ind];
}

HMdcCluster* HMdcClustersArrs::getCluster(Int_t indarr,Int_t  indClus) {
  if(clusArr[indarr]==0 || indClus>=arrSize[indarr] || indClus<0) return 0;
  return &(clusArr[indarr][indClus]);
}

void HMdcClustersArrs::testCluster(Int_t indArr,Int_t indClus) {
  HMdcCluster* clArr = clusArr[indArr];
  for(Int_t iCl=0;iCl<indClus;iCl++) if(clArr[iCl].getStatus()) {
    if( clArr[iCl].testForInclude(clArr[indClus]) ) break;
  }
}
