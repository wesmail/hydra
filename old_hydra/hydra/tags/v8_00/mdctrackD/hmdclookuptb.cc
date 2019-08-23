using namespace std;
#include "hmdclookuptb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hmdcgetcontainers.h"
#include <iostream>
#include <iomanip>
#include "hmdcgeomstruct.h"
#include "hspecgeompar.h"
#include "hmdcgeompar.h"
#include "hmdcsizescells.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdclayergeompar.h"
#include "TObjArray.h"
#include "TH2.h"
#include "hmdcclussim.h"
#include "hmdctrackdset.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 17/07/2003 by V. Pechenov
//*-- Modified : 05/02/2003 by V. Pechenov
//*-- Modified : 04/06/2002 by V.Pechenov
//*-- Modified : 09/05/2001 by V.Pechenov
//*-- Modified : 12/07/2000 by V.Pechenov
//*-- Modified : 23/05/2000 by V.Pechenov
//*-- Modified : 07/03/2000 by R. Holzmann
//*-- Modified : 02/12/99 by V.Pechenov
//*-- Modified : 26/10/99 by V.Pechenov
//*-- Modified : 20/05/99

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcLookUpTb
//
// Trackfinder for MDC1&2 and MDC3&4 if magnet off
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcLookUpTbCell)
ClassImp(HMdcLookUpTbLayer)
ClassImp(HMdcLookUpTbMod)
ClassImp(HMdcLookUpTbSec)
ClassImp(HMdcLookUpTb)
ClassImp(HMdcCluster)

HMdcLookUpTbCell::HMdcLookUpTbCell(void) {
  nLines=0;
  xBinMin=xBinMax=0;
}

void HMdcLookUpTbCell::init(Int_t yBinMinT, Int_t yBinMaxT) {
  Int_t newNLines=yBinMaxT-yBinMinT+1;
  if(newNLines>nLines) {
    if(xBinMin) delete [] xBinMin;
    if(xBinMax) delete [] xBinMax;
    nLines=newNLines;
    xBinMin=new Short_t [nLines];
    xBinMax=new Short_t [nLines];
  }
  yBinMin=yBinMinT;
  yBinMax=yBinMaxT;
  line=0;
}

HMdcLookUpTbCell::~HMdcLookUpTbCell(void) {
  if(xBinMin) delete [] xBinMin;
  if(xBinMax) delete [] xBinMax;
}

Bool_t HMdcLookUpTbCell::addLine(Short_t nc1, Short_t nc2){
  if(line>=0 && line<nLines) {
    xBinMin[line]=nc1;
    xBinMax[line]=nc2;
    line++;
    return kTRUE;
  }
  else Error("addLine","line number > max. (%i > %i)",line,nLines);
  return kFALSE;
}

//----------Layer-----------------------------

HMdcLookUpTbLayer::HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer) {
  // Geting of pointers to par. cont.
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return;
  HMdcGeomStruct* fMdcGeomStruct=parCont->getMdcGeomStruct();
  if( !fMdcGeomStruct ) return;
  nCells=((*fMdcGeomStruct)[sec][mod])[layer];
  array = new TObjArray(nCells);
  for(Int_t cell=0; cell<nCells; cell++) (*array)[cell]=new HMdcLookUpTbCell();
}

HMdcLookUpTbLayer::~HMdcLookUpTbLayer(void) {
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcLookUpTbLayer::getSize(void) {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//------------Module----------------------------
HMdcLookUpTbMod::HMdcLookUpTbMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++)
    (*array)[layer] = new HMdcLookUpTbLayer(sec, mod, layer);
  nLayers=6;
}

HMdcLookUpTbMod::~HMdcLookUpTbMod(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcLookUpTbMod::getSize(void) {
  // returns the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//----------HMdcCluster-------------------------------
HMdcCluster::HMdcCluster(void) {
  radToDeg=180./acos(Double_t(-1.));
}

void HMdcCluster::clear(void) {
  status=kTRUE;
  flag=0;
  clusMerg=0;
  nMergedClus=1;
  lCells1.clear();
  lCells2.clear();
  nBins=0;
  meanX=meanY=meanXX=meanYY=meanYX=0.;
  sumWt=meanXWt=meanYWt=meanXXWt=meanYYWt=0.;
  clusMod1=clusMod2=0;
}

void HMdcCluster::addClus(HMdcCluster& clst2, Bool_t flag) {
  lCells1.add(&(clst2.lCells1));
  if(flag) lCells2.add(&(clst2.lCells2));
  clst2.status=kFALSE;
  clst2.clusMerg=this;
  sumWt += clst2.sumWt;
  nBins += clst2.nBins;
  meanX += clst2.meanX;
  meanY += clst2.meanY;
  meanXX += clst2.meanXX;
  meanYY += clst2.meanYY;
  meanYX += clst2.meanYX;
  meanXWt += clst2.meanXWt;
  meanYWt += clst2.meanYWt;
  meanXXWt += clst2.meanXXWt;
  meanYYWt += clst2.meanYYWt;
  nMergedClus += clst2.nMergedClus;
  calcXY();
}

void HMdcCluster::sumClus(HMdcCluster& clst1, HMdcCluster& clst2, Bool_t flag) {
  lCells1=clst1.lCells1;
  lCells1.add(&(clst2.lCells1));
  if(flag) {
    lCells2=clst1.lCells2;
    lCells2.add(&(clst2.lCells2));
  }
  clst1.flag=1;
  clst2.flag=1;
//  clst2.status=kFALSE; ?
//  clst2.clusMerg=this; ?
  sumWt = clst1.sumWt+clst2.sumWt;
  nBins = clst1.nBins+clst2.nBins;
  meanX = clst1.meanX+clst2.meanX;
  meanY = clst1.meanY+clst2.meanY;
  meanXX = clst1.meanXX+clst2.meanXX;
  meanYY = clst1.meanYY+clst2.meanYY;
  meanYX = clst1.meanYX+clst2.meanYX;
  meanXWt = clst1.meanXWt+clst2.meanXWt;
  meanYWt = clst1.meanYWt+clst2.meanYWt;
  meanXXWt = clst1.meanXXWt+clst2.meanXXWt;
  meanYYWt = clst1.meanYYWt+clst2.meanYYWt;
//  nMergedClus += clst2.nMergedClus;
  calcXY();
}
void HMdcCluster::calcClusParam(Double_t xSt2, Double_t ySt2) {
  errX=sqrt(meanXXWt/sumWt-x*x + xSt2);
  errY=sqrt(meanYYWt/sumWt-y*y + ySt2);
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
    alpha=atan2(sqrt(eYX2+e1*e1),sqrt(eYX2+e2*e2))*radToDeg;
    if(eYX<0.) alpha=180.-alpha;
  }
}

void HMdcCluster::fillClus(HMdcClus* fClus, Int_t nLst, Bool_t fillTrList) {
  fClus->setSumWt(sumWt);
  fClus->setNBins(nBins);
  fClus->setXY(x,errX,y,errY);
  fClus->setNMergClust(nMergedClus);
  HMdcList12GroupCells& list=(nLst==0) ? lCells1:lCells2;
  if(clusMod1==0) {
    Int_t nDrTm=list.getNDrTimes(0,5);
    if(nDrTm>0 && fClus->getMinCl(0)>0) {
      fClus->setClusSizeM1(nBins);
      fClus->setNMergClustM1(nMergedClus);
      fClus->setShapeM1(sigma1,sigma2,alpha);
      if(fillTrList && fClus->isGeant()) 
        ((HMdcClusSim*)fClus)->calcTrListMod(list,0);
    } else fClus->clearMod1Par();
    fClus->setNDrTimesM1(nDrTm);
  } else {
    fClus->setClusSizeM1(clusMod1->nBins);
    fClus->setNDrTimesM1(clusMod1->lCells1.getNDrTimes(0,5));
    fClus->setNMergClustM1(clusMod1->nMergedClus);
    fClus->setShapeM1(clusMod1->sigma1,clusMod1->sigma2,clusMod1->alpha);
    if(fillTrList && fClus->isGeant()) 
      ((HMdcClusSim*)fClus)->calcTrListMod(clusMod1->lCells1,0);
  }
  if(clusMod2==0) {
    Int_t nDrTm=list.getNDrTimes(6,11);
    if(nDrTm>0 && fClus->getMinCl(1)>0) {
      fClus->setClusSizeM2(nBins);
      fClus->setNMergClustM2(nMergedClus);
      fClus->setShapeM2(sigma1,sigma2,alpha);
      if(fillTrList && fClus->isGeant()) 
        ((HMdcClusSim*)fClus)->calcTrListMod(list,1);
    } else fClus->clearMod2Par();
    fClus->setNDrTimesM2(nDrTm);
  } else {
    fClus->setClusSizeM2(clusMod2->nBins);
    fClus->setNDrTimesM2(clusMod2->lCells1.getNDrTimes(6,11));
    fClus->setNMergClustM2(clusMod2->nMergedClus);
    fClus->setShapeM2(clusMod2->sigma1,clusMod2->sigma2,clusMod2->alpha);
    if(fillTrList && fClus->isGeant()) 
      ((HMdcClusSim*)fClus)->calcTrListMod(clusMod2->lCells1,1);
  }
}

//----------Sector------------------------------------

HMdcClFnStack* HMdcLookUpTbSec::stack=0;
Int_t HMdcLookUpTbSec::hPlModsSize=0;
UChar_t* HMdcLookUpTbSec::hPlMod[4]={0,0,0,0};
Int_t HMdcLookUpTbSec::sizeBArSt=0;
UChar_t* HMdcLookUpTbSec::plotBArSc=0;
UChar_t* HMdcLookUpTbSec::plotBArM[4]={0,0,0,0};
Short_t* HMdcLookUpTbSec::clusIndM1=0;
Int_t HMdcLookUpTbSec::clIndArrSzM1=0;
Short_t* HMdcLookUpTbSec::clusIndM2=0;
Int_t HMdcLookUpTbSec::clIndArrSzM2=0;
Int_t HMdcLookUpTbSec::clusArrSize=0;
HMdcCluster* HMdcLookUpTbSec::clusArr=0;
HMdcCluster* HMdcLookUpTbSec::clusArrM1=0;
HMdcCluster* HMdcLookUpTbSec::clusArrM2=0;

HMdcLookUpTbSec::HMdcLookUpTbSec(Int_t sec, Int_t nSegs,
                                 Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  sector=sec;
  nSegments=nSegs;
  hist=0;
  nBinX=(inBinX%2 == 0) ? inBinX:inBinX+1;
  nBinY=(inBinY%2 == 0) ? inBinY:inBinY+1;
  xBinsPos=new Double_t [nBinX];
  yBinsPos=new Double_t [nBinY];
  size=nBinX*nBinY;
  size=(size/32 + ((size%32 > 0) ? 1:0))*32;
  
  Bool_t resize=(size>hPlModsSize) ? kTRUE:kFALSE;
  if(resize) hPlModsSize=size;
  array = new TObjArray(4);
  HMdcDetector* fMdcDet= HMdcGetContainers::getObject()->getMdcDetector();
  const Int_t* nLM=HMdcTrackDSet::getTrFnNLayersInMod()+sector*4;
  for (Int_t mod = 0; mod < nSegs*2; mod++) {
    if(fMdcDet->getModule(sec,mod) && nLM[mod]>0) {
      HMdcLookUpTbMod* pMod=new HMdcLookUpTbMod(sec,mod);
      (*array)[mod]=pMod;
      pMod->setNLayers(nLM[mod]);
    }
    if(hPlMod[mod] && !resize) continue;
    if(hPlMod[mod])  delete [] hPlMod[mod];
    hPlMod[mod]=new UChar_t [hPlModsSize];
    memset(hPlMod[mod],0,hPlModsSize);
  }

  if(plotBArSc==0 || sizeBArSt<size/8) {
    if(plotBArSc) delete [] plotBArSc;
    for(Int_t mod=0;mod<4;mod++) if(plotBArM[mod]) delete [] plotBArM[mod];
    sizeBArSt=size/8;
    plotBArSc=new UChar_t [sizeBArSt];
    memset(plotBArSc,0,sizeBArSt);
    for(Int_t mod=0;mod<4;mod++) {
      plotBArM[mod]=new UChar_t [sizeBArSt];
      memset(plotBArM[mod],0,sizeBArSt);
    }
  }
  sizeBAr=size/8;
  typeClFinder=0;
  setLUpTb=kFALSE;
  neighbBins[0]=-1;
  neighbBins[1]=+1;
  neighbBins[2]=-nBinX;
  neighbBins[3]=+nBinX;
  neighbBins[4]=-1-nBinX;
  neighbBins[5]=-1+nBinX;
  neighbBins[6]=1-nBinX;
  neighbBins[7]=1+nBinX;
  if(stack==0) stack=new HMdcClFnStack(10000);
  isGeant = HMdcGetContainers::isGeant();
  fillTrackList = (isGeant) ? kTRUE:kFALSE;
  locClus.set(3,sec,0,0);
  if(clusArrSize==0) {
    clusArrSize=500;    // Clusters array size!
    clusArr=new HMdcCluster [clusArrSize];
    clusArrM1=new HMdcCluster [clusArrSize];
    clusArrM2=new HMdcCluster [clusArrSize];
  }
  if(size>clIndArrSzM1) {
    if(clusIndM1) delete [] clusIndM1;
    clusIndM1=new Short_t [size];
    clIndArrSzM1=size;
  }
  if(size>clIndArrSzM2) {
    if(clusIndM2) delete [] clusIndM2;
    clusIndM2=new Short_t [size];
    clIndArrSzM2=size;
  }
  for(Int_t mod=0;mod<4;mod++) {
    xMin[mod]=new Int_t [nBinY];
    xMax[mod]=new Int_t [nBinY];
    for(Int_t y=0;y<nBinY;y++) {
      xMin[mod][y]=size;
      xMax[mod][y]=-1;
    }
  }
  xMaxCl=new Short_t [nBinY];
  xMinCl=new Short_t [nBinY];
}

HMdcLookUpTbSec::~HMdcLookUpTbSec(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  for(Int_t mod=0; mod<4; mod++) {
    if(hPlMod[mod]) delete [] hPlMod[mod];
    hPlMod[mod]=0;
  }
  hPlModsSize=0;
  if(stack) delete stack;
  stack=0;
  if(hist && hist->IsOnHeap()) delete hist;
  hist=0;
  if(plotBArSc) {
    delete [] plotBArSc;
    plotBArSc=0;
    sizeBArSt=0;
  }
  for(Int_t mod=0;mod<4;mod++) {
    if(plotBArM[mod]) {
      delete [] plotBArM[mod];
      plotBArM[mod]=0;
    }
  }
  delete [] xBinsPos;
  delete [] yBinsPos;
  for(Int_t mod=0;mod<4;mod++) {
    delete [] xMin[mod];
    delete [] xMax[mod];
  }

  if(clusArr)   delete [] clusArr;
  if(clusArrM1) delete [] clusArrM1;
  if(clusArrM2) delete [] clusArrM2;
  clusArrSize=0;
  clusArr=clusArrM1=clusArrM2=0;

  if(clusIndM1) {
    delete [] clusIndM1;
    clusIndM1=0;
    clIndArrSzM1=0;
  }
  if(clusIndM2) {
    delete [] clusIndM2;
    clusIndM2=0;
    clIndArrSzM2=0;
  }
  delete [] xMaxCl;
  delete [] xMinCl;
}

void HMdcLookUpTbSec::clearPrArrs(void) {
  if(hPlMod[0]) memset(hPlMod[0],0,size);
  if(hPlMod[1]) memset(hPlMod[1],0,size);
  if(hPlMod[2]) memset(hPlMod[2],0,size);
  if(hPlMod[3]) memset(hPlMod[3],0,size);
}

void HMdcLookUpTbSec::clearPrMod(Int_t mod) {
  UChar_t* hPlModM=hPlMod[mod];
  if(!hPlModM) return;
  Int_t* xMaxM=xMax[mod];
  Int_t* xMinM=xMin[mod];
  for(Int_t y=0;y<nBinY;y++) {
    if(xMaxM[y]<0) continue;
    memset(hPlModM+xMinM[y],0,xMaxM[y]-xMinM[y]+1);
    xMinM[y]=size;
    xMaxM[y]=-1;
  }
}

Int_t HMdcLookUpTbSec::getSize(void) {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//May be numhits[i]=0 ... in another sub. ?????????????????
void HMdcLookUpTbSec::clearwk(void) {
  for(Int_t i=0; i<24; i++) nHits[i]=0;
  nHitsTot=0;
  setLUpTb=kFALSE;
  counter[0]=counter[1]=0;
}


void HMdcLookUpTbSec::setCell(Short_t mod, Short_t layer,
                              Short_t cell, Short_t nTimes){
  HMdcLookUpTbMod& fLUpTbMod=(*this)[mod];
  if(&fLUpTbMod == 0) return;
  HMdcLookUpTbLayer& fLUpTbLayer=fLUpTbMod[layer];
  if(cell<0 || cell>=fLUpTbLayer.nCells) return;
  HMdcLookUpTbCell& fLUpTbCell=fLUpTbLayer[cell];
  if( &fLUpTbCell && fLUpTbCell.line>0 ) {
    Int_t ind1=mod*6+layer;
    Int_t ind2=nHits[ind1];
    if (ind2<250) {
      hits[ind1][ind2]=cell;
      hitsNTime[ind1][ind2]=nTimes;
      hitsDel[ind1][ind2]=0;
      nHits[ind1]++;
      nHitsTot++;
    } else Error("setCell","Too many hits in layer.");
  }
}

void HMdcLookUpTbSec::deleteHit(Short_t mod, Short_t layer,
                                Short_t cell, Short_t nTime){

 if(mod<2) {
   Short_t ind1=mod*6+layer;
   Short_t ind2=-1;
   for(Int_t i=0; i<nHits[ind1]; i++) {
     if(hits[ind1][i]==cell) { ind2=i; break;}
   }
   if (ind2>=0) {
     //hitsNTime[ind1][ind2]=1 - time1; =2 - tim2; =3 - time1&tim2
     if(hitsNTime[ind1][ind2]==1 || hitsNTime[ind1][ind2]==2 || nTime==0 )
       hitsDel[ind1][ind2]=-1;
     else if(hitsDel[ind1][ind2]==0) hitsDel[ind1][ind2]=3-nTime;
     else hitsDel[ind1][ind2]=-1;
   }
   else Error("deleteHit","Hit %iM%iL%iC is absent.\n",mod,layer,cell);
 }
/*
 HMdcLookUpTbLayer& fLUpTbLayer=(*this)[mod][layer];
 HMdcLookUpTbCell& fCell=fLUpTbLayer[cell];
 UChar_t nBin=(UChar_t) nCell;
 const UChar_t one=1;
 for(Int_t l=0; l<fCell.line; l++) {
   for(Int_t nb=fCell.xBinMin[l]; nb<=fCell.xBinMax[l]; nb++) {
     UChar_t bit=(one<<(nBin-fLUpTbLayer.nfcell[nb]));
     if(fLUpTbLayer.ncellbin[nb]&bit != 0) {
       fLUpTbLayer.ncellbin[nb]=fLUpTbLayer.ncellbin[nb]&(~bit);
       if(fLUpTbLayer.ncellbin[nb] == 0) hitPlS1[nb]--;
     }
   }
 }
*/
}

void HMdcLookUpTbSec::calcMaxAmp(void) {
  for(Int_t mod=0;mod<4;mod++) {
    maxAmp[mod]=0;
    for(Int_t layer=0;layer<6;layer++) if(nHits[mod*6+layer]>0) maxAmp[mod]++;
  }
}

void HMdcLookUpTbSec::makeSPlot(void) {
  // Making proj.plot in sector(magnetoff) or segment(magneton).
  // Number of MDCs in sector or segment must be >1 !!!
  maxBinBAr4Sc=0;
  minBinBAr4Sc=size;
  UChar_t addm=1;

  Int_t lmod=-1;
  if(minAmp[3]>0) lmod=3;
  else if(minAmp[2]>0) lmod=2;
  else if(minAmp[1]>0) lmod=1;
  else if(minAmp[0]>0) lmod=0;
  if(lmod<0) return;
  Int_t fmod=3;
  if(minAmp[0]>0) fmod=0;
  else if(minAmp[1]>0) fmod=1;
  else if(minAmp[2]>0) fmod=2;
  if(fmod==lmod) return;

  Int_t lay[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling
  Char_t minAm0=minAmp[0];
  Char_t minAm1=minAmp[1];
  Char_t minAm2=minAmp[2];
  Char_t minAmL=minAmp[lmod];
  for(Int_t mod=fmod; mod<=lmod; mod++) {
    cFMod=&((*this)[mod]);
    if( !cFMod ) continue;
    if(minAmp[mod]==0) continue;
    cHPlModM=hPlMod[mod];
    if(!cHPlModM) continue;
    cXMinM=xMin[mod];
    cXMaxM=xMax[mod];

    Int_t iLay=mod*6;
    Int_t nFiredLay=0;
    for(Int_t indLine=0; indLine<6; indLine++) {
      Int_t layer=lay[indLine];
      Int_t indLay=iLay+layer;
      if(nHits[indLay]==0) continue;
      nFiredLay++;
      if(maxAmp[mod]-nFiredLay+1>=minAmp[mod]) {
        if(mod==fmod) makeLayProjV1(layer,indLay);
        else if(mod<lmod || nFiredLay<minAmp[mod]) makeLayProjV1b(layer,indLay);
        else {
          UChar_t add=addm<<layer;
          HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
          for(Int_t n=0; n<nHits[indLay]; n++) {
            if(hitsDel[indLay][n]<0) continue;
            HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
            for(Int_t ln=0; ln<fCell.line; ln++) {
              Int_t y=fCell.yBinMin+ln;
              if(pXMaxM[y]<0) continue;
              Int_t shift=y * nBinX;
              Int_t nbL=fCell.xBinMax[ln]+shift;
              Int_t nbF=fCell.xBinMin[ln]+shift;
              if(nbL>pXMaxM[y]) nbL=pXMaxM[y];
              if(nbF<pXMinM[y]) nbF=pXMinM[y];
              if(nbF>nbL) continue;
              if(nbF<cXMinM[y]) cXMinM[y]=nbF;
              if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
              UChar_t* bt=cHPlModM+nbF;
              for(Int_t nb=nbF; nb<=nbL; nb++) {
                *bt |= add;
                Char_t wt=HMdcBArray::getNSet(bt);
                bt++;
                if( wt<minAmL ) continue;
                if(fmod==0&&HMdcBArray::getNSet(hPlMod[0][nb])<minAm0) continue;
                if(mod>1) {
                  if(HMdcBArray::getNSet(hPlMod[1][nb])<minAm1) continue;
                  if(mod>2&&HMdcBArray::getNSet(hPlMod[2][nb])<minAm2) continue;
                }
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
                if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
              }
            }
          }
        }
      } else {
        if(mod<lmod || nFiredLay<minAmp[mod]) makeLayProjV2(layer,indLay);
        else {
          UChar_t add=addm<<layer;
          HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
          for(Int_t n=0; n<nHits[indLay]; n++) {
            if(hitsDel[indLay][n]<0) continue;
            HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
            for(Int_t ln=0; ln<fCell.line; ln++) {
              Int_t y=fCell.yBinMin+ln;
              if(cXMaxM[y]<0) continue;
              Int_t shift=y * nBinX;
              Int_t nbL=fCell.xBinMax[ln]+shift;
              Int_t nbF=fCell.xBinMin[ln]+shift;
              if(nbL>cXMaxM[y]) nbL=cXMaxM[y];
              if(nbF<cXMinM[y]) nbF=cXMinM[y];
              UChar_t* bt=cHPlModM+nbF;
              for(Int_t nb=nbF; nb<=nbL; nb++) {
                *bt |= add;
                Char_t wt=HMdcBArray::getNSet(bt);
                bt++;
                if( wt<minAmL ) continue;
                if(fmod==0&&HMdcBArray::getNSet(hPlMod[0][nb])<minAm0) continue;
                if(mod>1) {
                  if(HMdcBArray::getNSet(hPlMod[1][nb])<minAm1) continue;
                  if(mod>2&&HMdcBArray::getNSet(hPlMod[2][nb])<minAm2) continue;
                }
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
                if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
              }
            }
          }
        }
      }
    }
    pXMinM=cXMinM;
    pXMaxM=cXMaxM;
  }
  maxBinBAr4Sc/=32;
  minBinBAr4Sc/=32;
}

void HMdcLookUpTbSec::makeModPlot(Int_t mod,Int_t minAm) {
  if(minAm<2) return;
  cFMod=&((*this)[mod]);
  if( !cFMod ) return;
  cHPlModM=hPlMod[mod];
  cXMinM=xMin[mod];
  cXMaxM=xMax[mod];

  Int_t iLay=mod*6;
  Int_t lay[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling

  UChar_t* cPlotBAr=plotBArM[mod];
  Int_t& minBin=minBinBAr4M[mod];
  Int_t& maxBin=maxBinBAr4M[mod];
  maxBin=0;
  minBin=size;
  Int_t nFiredLay=0;
  for(Int_t indLine=0; indLine<6; indLine++) {
    Int_t layer=lay[indLine];
    Int_t indLay=iLay+layer;
    if(nHits[indLay]==0) continue;
    nFiredLay++;
    if(maxAmp[mod]-nFiredLay+1>=minAm) {  // determination min(max)Bin[y]
      if(nFiredLay<minAm) makeLayProjV1(layer,indLay);  // filling pr.plot
      else {                                                // ...+ amp.checking
        // filling, min, max and bits seting
        UChar_t add=1<<layer;
        HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
        for(Int_t n=0; n<nHits[indLay]; n++) {
          if(hitsDel[indLay][n]<0) continue;
          HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
          for(Int_t ln=0; ln<fCell.line; ln++) {
            Int_t y=fCell.yBinMin+ln;
            if(cXMaxM[y]<0) continue;
            Int_t shift=y * nBinX;
            Int_t nbL=fCell.xBinMax[ln]+shift;
            Int_t nbF=fCell.xBinMin[ln]+shift;
            if(nbF<cXMinM[y]) cXMinM[y]=nbF;
            if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
            UChar_t* bt=cHPlModM+nbF;
            for(Int_t nb=nbF; nb<=nbL; nb++) {
              *bt |= add;
              Char_t wt=HMdcBArray::getNSet(bt);
              bt++;
              if(wt<minAm) continue;
              HMdcBArray::set(cPlotBAr,nb);
              if(nb<minBin) minBin=nb;
              if(nb>maxBin) maxBin=nb;
            }
          }
        }
      }
    } else {                         // filling in minBin[y]-maxBin[y] only
      if(nFiredLay<minAm) makeLayProjV2(layer,indLay);  // filling pr.plot
      else {                                                // ...+ amp.checking
        UChar_t add=1<<layer;
        HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
        for(Int_t n=0; n<nHits[indLay]; n++) {
          if(hitsDel[indLay][n]<0) continue;
          HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
          for(Int_t ln=0; ln<fCell.line; ln++) {
            Int_t y=fCell.yBinMin+ln;
            if(cXMaxM[y]<0) continue;
            Int_t shift=y * nBinX;
            Int_t nbL=fCell.xBinMax[ln]+shift;
            Int_t nbF=fCell.xBinMin[ln]+shift;
            if(nbL>cXMaxM[y]) nbL=cXMaxM[y];
            if(nbF<cXMinM[y]) nbF=cXMinM[y];
            UChar_t* bt=cHPlModM+nbF;
            for(Int_t nb=nbF; nb<=nbL; nb++) {
              *bt |= add;
              Char_t wt=HMdcBArray::getNSet(bt);
              bt++;
              if(wt<minAm) continue;
              HMdcBArray::set(cPlotBAr,nb);
              if(nb<minBin) minBin=nb;
              if(nb>maxBin) maxBin=nb;
            }
          }
        }
      }
    }
  }
  maxBin/=32;
  minBin/=32;
}

void HMdcLookUpTbSec::makeLayProjV1(Int_t layer, Int_t indLay) {
  // plot filling and filled region determination
  UChar_t add=1<<layer;
  HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
  for(Int_t n=0; n<nHits[indLay]; n++) {
    if(hitsDel[indLay][n]<0) continue;
    HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
    for(Int_t ln=0; ln<fCell.line; ln++) {
      Int_t y=fCell.yBinMin+ln;
      Int_t shift=y * nBinX;
      Int_t nbL=fCell.xBinMax[ln]+shift;
      Int_t nbF=fCell.xBinMin[ln]+shift;
      if(nbF<cXMinM[y]) cXMinM[y]=nbF;
      if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
      UChar_t* hPlModE=cHPlModM+nbL;
      for(UChar_t* bt=cHPlModM+nbF; bt<=hPlModE; bt++) *bt |= add;
    }
  }
}

void HMdcLookUpTbSec::makeLayProjV1b(Int_t layer, Int_t indLay) {
  // plot filling and filled region determination in region determined
  // in previous mdc
  UChar_t add=1<<layer;
  HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
  for(Int_t n=0; n<nHits[indLay]; n++) {
    if(hitsDel[indLay][n]<0) continue;
    HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
    for(Int_t ln=0; ln<fCell.line; ln++) {
      Int_t y=fCell.yBinMin+ln;
      if(pXMaxM[y]<0) continue;
      Int_t shift=y * nBinX;
      Int_t nbL=fCell.xBinMax[ln]+shift;
      Int_t nbF=fCell.xBinMin[ln]+shift;
      if(nbL>pXMaxM[y]) nbL=pXMaxM[y];
      if(nbF<pXMinM[y]) nbF=pXMinM[y];
      if(nbF>nbL) continue;
      if(nbF<cXMinM[y]) cXMinM[y]=nbF;
      if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
      UChar_t* hPlModE=cHPlModM+nbL;
      for(UChar_t* bt=cHPlModM+nbF; bt<=hPlModE; bt++) *bt |= add;
    }
  }
}

void HMdcLookUpTbSec::makeLayProjV2(Int_t layer, Int_t indLay) {
  // plot filling in filled regions only
  UChar_t add=1<<layer;
  HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
  for(Int_t n=0; n<nHits[indLay]; n++) {
    if(hitsDel[indLay][n]<0) continue;
    HMdcLookUpTbCell& fCell=fLayer[hits[indLay][n]];
    for(Int_t ln=0; ln<fCell.line; ln++) {
      Int_t y=fCell.yBinMin+ln;
      if(cXMaxM[y]<0) continue;
      Int_t shift=y * nBinX;
      Int_t nbL=fCell.xBinMax[ln]+shift;
      Int_t nbF=fCell.xBinMin[ln]+shift;
      if(nbL>cXMaxM[y]) nbL=cXMaxM[y];
      if(nbF<cXMinM[y]) nbF=cXMinM[y];
      UChar_t* hPlModE=cHPlModM+nbL;
      for(UChar_t* bt=cHPlModM+nbF; bt<=hPlModE; bt++) *bt |= add;
    }
  }
}

Int_t HMdcLookUpTbSec::findClusters(Int_t *imax){
  nClusters=0;
  if(nHitsTot==0) return 0;
  calcMaxAmp();
  //isCoilOff - !!!??? ---------------------------------------
  nMods=0;
  for(Int_t mod=0; mod<4; mod++) {
    minAmp[mod]=(mod<nSegments*2) ? imax[mod]:0;
    if(minAmp[mod]>0) nMods++;
  }
  if(typeClFinder==1) {
    // chamber clust. finding
    for(Int_t m=0; m<4; m++) if(minAmp[m]>0) findClusInMod(m,minAmp[m]);
  } else {
    // combyned clust. finding
    nModSeg[0]=nModSeg[1]=0;
    for(Int_t m=0;m<4;m++) if(minAmp[m]>0) nModSeg[m/2] |= m%2+1;
    if(nModSeg[0]>0 && nModSeg[1]>0)
        findClusInSec(minAmp[0]+minAmp[1]+minAmp[2]+minAmp[3]);
    else {
      if(nModSeg[0]==3) findClusInSeg1();
        // findClusInSeg(0,minAmp[0]+minAmp[1]);
      else if(nModSeg[0]>0) findClusInMod(nModSeg[0]-1,minAmp[nModSeg[0]-1]);
      if(nModSeg[1]==3) findClusInSeg(1,minAmp[2]+minAmp[3]);
      else if(nModSeg[1]>0) findClusInMod(nModSeg[1]+1,minAmp[nModSeg[1]+1]);
    }
  }
  return nClusters;
}

Int_t HMdcLookUpTbSec::getClusterSlot(Int_t seg, HMdcList12GroupCells& list) {
  locClus[1]=seg;
  locClus[2]=counter[seg];
  Int_t index;
  fClus = (HMdcClus*)fClusCat->getSlot(locClus,&index);
  if(!fClus) {
    Warning("getClusterSlot","Sec.%i Segment %i No slot HMdcClus available",
        sector+1,seg+1);
    return -1;
  }
  if(isGeant) fClus=(HMdcClus*)(new(fClus) HMdcClusSim(list));
  else fClus=new(fClus) HMdcClus(list);
  fClus->setAddress(locClus);
  counter[seg]++;
  return index;
}

void HMdcLookUpTbSec::fillWiresList(Int_t mod, HMdcList12GroupCells& list) {
  HMdcLookUpTbMod& fMod=(*this)[mod];
  if( !(&fMod) ) return;
  Int_t layAdd=(mod%2)*6;
  for(Int_t layer=0; layer<6; layer++) {
    Int_t iLayer=mod*6+layer;
    if(nHits[iLayer]<=0) continue;
    Int_t clLay=layAdd+layer;
    HMdcLookUpTbLayer& fLayer=fMod[layer];
    for(Int_t i=0; i<nHits[iLayer]; i++) {
      if(hitsDel[iLayer][i]<0) continue;
      Int_t cell=hits[iLayer][i];
      HMdcLookUpTbCell& fCell=fLayer[cell];
      Short_t yBinMax=fCell.yBinMax;
      if( nLMinCl > yBinMax ) continue;
      Short_t yBinMin=fCell.yBinMin;
      if( nLMaxCl < yBinMin ) continue;
//    if( nLMaxCl < yBinMin ) break;    //        Hits are not sorted !
      Short_t lMax=(yBinMax < nLMaxCl) ? yBinMax : nLMaxCl;
      Short_t lMin=(yBinMin > nLMinCl) ? yBinMin : nLMinCl;
      for (Short_t nl=lMin; nl<=lMax; nl++) {
        if( fCell.xBinMin[nl-yBinMin] > xMaxCl[nl] ||
            fCell.xBinMax[nl-yBinMin] < xMinCl[nl] ) continue;
        list.setTime(clLay,cell,hitsNTime[iLayer][i]);
	break;
      }
    }
  }
}

void HMdcLookUpTbSec::findClusInSeg(Int_t seg, Short_t minAm){
  if(maxAmp[seg*2]<minAmp[seg*2] || maxAmp[seg*2+1]<minAmp[seg*2+1]) return;
  if(minAm<3) return;
  nClsArr=0;
  makeSPlot();
  UChar_t *hPlModF=(seg==0) ? hPlMod[0]:hPlMod[2];
  UChar_t *hPlModS=(seg==0) ? hPlMod[1]:hPlMod[3];
  Char_t maxClS=minAm-1;
  Int_t nmo=(seg+1)*2;
  cPlotBAr=plotBArSc;
  Int_t* plotBAr4b=(Int_t*) cPlotBAr;
  //---Cluster finding-------------------------------------
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      clus=&clusArr[nClsArr];
      Int_t nBin=nBin4+nLBin;
      initCluster(nBin);
      while ((nBin=stack->pop()) >= 0) addBinInCluster(nBin,
            HMdcBArray::getNSet(hPlModF[nBin])+
            HMdcBArray::getNSet(hPlModS[nBin])-maxClS);
      //-Filling of cluster---------------------------------------------------
      clus->calcXY();
      for(Int_t mod=seg*2; mod<nmo; mod++) fillWiresList(mod,clus->lCells1);
      nClsArr++;
      if(nClsArr >= clusArrSize) break;
    }
    if(nClsArr >= clusArrSize) {
      Warning("findClusInSeg"," Num. of clusters in sector %i > max\n",sector);
      memset(cPlotBAr,0,sizeBAr);
      break;
    }
  }

  if(nClsArr>0) {
    setCurrentArray(clusArr,&nClsArr);
    if(nClsArr>1) mergeClusInSeg();
    calcClParam();
    fillClusCat(-2,seg,0);
  }
  for(Int_t mod=seg*2;mod<(seg+1)*2;mod++) if(minAmp[mod]>0) clearPrMod(mod);
}

void HMdcLookUpTbSec::findClusInSeg1(void){
  if(typeClFinder==0) {
    if(maxAmp[0]<minAmp[0] || maxAmp[1]<minAmp[1]) return;
  } else if(maxAmp[0]<minAmp[0] && maxAmp[1]<minAmp[1]) return;
  if(minAmp[0]+minAmp[1]<3) return;
  makeModPlot(0,minAmp[0]);
  makeModPlot(1,minAmp[1]);

  minBinBAr4Sc=(minBinBAr4M[0]>=minBinBAr4M[1])?minBinBAr4M[0]:minBinBAr4M[1];
  maxBinBAr4Sc=(maxBinBAr4M[0]<=maxBinBAr4M[1])?maxBinBAr4M[0]:maxBinBAr4M[1];
  Int_t* plotBAr4bM1=(Int_t *)(plotBArM[0]);
  Int_t* plotBAr4bM2=(Int_t *)(plotBArM[1]);
  Int_t* plotBArSc4b=(Int_t *)plotBArSc;
  for(Int_t n4=minBinBAr4Sc; n4<=maxBinBAr4Sc; n4++)
      plotBArSc4b[n4]=plotBAr4bM1[n4] & plotBAr4bM2[n4];

  setCurrentArray(clusArrM1,&nClsArrM1);
  scanPlotInMod(0);
  if(nClsArrM1>1) mergeClusInMod(0);
  calcClParam();

  setCurrentArray(clusArrM2,&nClsArrM2);
  scanPlotInMod(1);
  if(nClsArrM2>1) mergeClusInMod(1);
  calcClParam();

  setCurrentArray(clusArr,&nClsArr);
  scanPlotInSeg1(0,plotBArSc);

  if(nClsArr>1) mergeClusInSeg();
  testClusMod12toSeg();
  if(nClsArrM1>0 && nClsArrM2>0) mergeClusMod1to2();
  if(nClsArr>0) {
    calcClParam();
    fillClusCat(-2,0,0);
  }
  
  if(typeClFinder==2) {
    setCurrentArray(clusArrM1,&nClsArrM1);
    fillClusCat(0,0,2);
    setCurrentArray(clusArrM2,&nClsArrM2);
    fillClusCat(1,0,2);
  }
  
  clearPrMod(0);
  clearPrMod(1);
}

void HMdcLookUpTbSec::scanPlotInMod(Int_t mod) {
  // Scan proj.plot in one module but determination of wires list in segment(!)
  Char_t maxClS=minAmp[mod]-1;
  (*cNClusArr)=0;
  UChar_t* hPlModM=hPlMod[mod];
  cPlotBAr=plotBArM[mod];
  Int_t* plotBAr4b=(Int_t*) cPlotBAr;
  Int_t minBin=minBinBAr4M[mod];
  Short_t* clusInd=(mod==0) ? clusIndM1:clusIndM2;
  for(Int_t n4=maxBinBAr4M[mod]; n4>=minBin; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      clus=&cClusArr[(*cNClusArr)];
      Int_t nBin=nBin4+nLBin;
      initCluster(nBin);
      while ((nBin=stack->pop()) >= 0) {
        clusInd[nBin]=(*cNClusArr);
        addBinInCluster(nBin, HMdcBArray::getNSet(hPlModM[nBin])-maxClS);
      }
      //-Filling of cluster---------------------------------------------------
      clus->calcXY();
      fillWiresList(mod,clus->lCells1);
      fillWiresList(mod^1,clus->lCells1); // for another module in segment
      (*cNClusArr)++;
      if((*cNClusArr) >= clusArrSize) break;
    }
    if((*cNClusArr) >= clusArrSize) {
      Warning("scanClusInMod"," Num. of clusters in sector %i > max\n",sector);
      memset(cPlotBAr,0,sizeBAr);
      break;
    }
  }
}

void HMdcLookUpTbSec::scanPlotInSeg1(Int_t seg, UChar_t* plotBAr) {
  Int_t m1=seg*2;
  Int_t m2=m1+1;
  Char_t maxClS=minAmp[m1]+minAmp[m2]-1;
  (*cNClusArr)=0;
  UChar_t *hPlModF=(seg==0) ? hPlMod[0]:hPlMod[2];
  UChar_t *hPlModS=(seg==0) ? hPlMod[1]:hPlMod[3];
  cPlotBAr=plotBAr;
  Int_t* plotBAr4b=(Int_t*) cPlotBAr;
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      clus=&cClusArr[(*cNClusArr)];
      Int_t nBin=nBin4+nLBin;
      initClusterT2(nBin);
      while ((nBin=stack->pop()) >= 0) addBinInCluster(nBin,
          HMdcBArray::getNSet(hPlModF[nBin])+
          HMdcBArray::getNSet(hPlModS[nBin])-maxClS);
      //-Filling of cluster---------------------------------------------------
      clus->calcXY();
      for(Int_t mod=m1;mod<=m2;mod++) fillWiresList(mod,clus->lCells1);
      (*cNClusArr)++;
      if((*cNClusArr) >= clusArrSize) break;
    }
    if((*cNClusArr) >= clusArrSize) {
      Warning("scanClusInSeg"," Num. of clusters in sector %i > max\n",sector);
      memset(cPlotBAr,0,sizeBAr);
      break;
    }
  }
}

void HMdcLookUpTbSec::findClusInSec(Short_t minAm){
  for(Int_t mod=0;mod<4;mod++)
      if(minAmp[mod]>0 && maxAmp[mod]<minAmp[mod]) return;
  if(minAm<3) return;
  makeSPlot();
  Char_t maxClS=minAm-1;
  nClsArr=0;
  cPlotBAr=plotBArSc;
  Int_t* plotBAr4b=(Int_t*) cPlotBAr;
  //---Cluster finding-------------------------------------
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      clus=&clusArr[nClsArr];
      Int_t nBin=nBin4+nLBin;
      initCluster(nBin);
      while ((nBin=stack->pop()) >= 0) addBinInCluster(nBin,
          HMdcBArray::getNSet(hPlMod[0][nBin])+
          HMdcBArray::getNSet(hPlMod[1][nBin])+
          HMdcBArray::getNSet(hPlMod[2][nBin])+
          HMdcBArray::getNSet(hPlMod[3][nBin])-maxClS);
      //-End cluster------------------------------------------------------
      clus->calcXY();
      for(Int_t mod=0;mod<4;mod++) {
        if(minAmp[mod]==0) continue;
        if(mod<2) fillWiresList(mod,clus->lCells1);
        else  fillWiresList(mod,clus->lCells2);
      }
      nClsArr++;
      if(nClsArr >= clusArrSize) break;
    }
    if(nClsArr >= clusArrSize) {
      Warning("findClusInSec"," Num. of clusters in sector %i > max\n",sector);
      memset(cPlotBAr,0,sizeBAr);
      break;
    }
  }
  if(nClsArr>0) {
    setCurrentArray(clusArr,&nClsArr);
    if(nClsArr>1) mergeClusInSec();
    calcClParam();
    fillClusCat(-nMods,-1,0);
  }
  for(Int_t mod=0;mod<4;mod++) if(minAmp[mod]>0) clearPrMod(mod);
}

void HMdcLookUpTbSec::mergeClusInMod(Int_t mod) {
  Int_t nClus=*cNClusArr;
  Int_t fLay=(mod==0 || mod==2) ? 0:6;
  Int_t lLay=fLay+5;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<(*cNClusArr)-1; cl1++) {
      HMdcCluster& cls1=cClusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      for(Int_t cl2=cl1+1; cl2<(*cNClusArr); cl2++) {
        HMdcCluster& cls2=cClusArr[cl2];
        if(!cls2.status) continue;
        HMdcList12GroupCells& listCells2=cls2.lCells1;
        Float_t dY=cls1.y-cls2.y;
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 30.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 100.) continue;        // 40. mm !???
        if(listCells1.compare(&listCells2,fLay,lLay)<4) continue;
        cls1.addClus(cls2,kFALSE);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::mergeClusMod1to2(void) {
  for(Int_t cl1=0; cl1<nClsArrM1; cl1++) {
    HMdcCluster& cls1=clusArrM1[cl1];
    if(!cls1.status || cls1.flag>0) continue;
    HMdcList12GroupCells& listCells1=cls1.lCells1;
    for(Int_t cl2=0; cl2<nClsArrM2; cl2++) {
      HMdcCluster& cls2=clusArrM2[cl2];
      if(!cls2.status || cls2.flag>0) continue;
      HMdcList12GroupCells& listCells2=cls2.lCells1;
      Float_t dY=cls1.y-cls2.y;
      if(dY>100.) break;  //????????? biylo zakomentirovano ???
      if(fabs(dY) > 30.) continue;                   // 10. mm !???
      if(fabs(cls1.x-cls2.x) > 100.) continue;        // 40. mm !???
      if(listCells1.compare(&listCells2)<4) continue;
//      if(listCells1.compare(&listCells2,0, 5)<4) continue;
//      if(listCells1.compare(&listCells2,6,11)<4) continue;
      if(nClsArr >= clusArrSize) break;
      HMdcCluster& cls=clusArr[nClsArr++];
      cls.clear();
      cls.sumClus(cls1,cls2,kFALSE);
    }
  }
}

void HMdcLookUpTbSec::testClusMod12toSeg(void) {
  // excluding clusters in MOD with <4 non identical wires to segment clusters
  for(Int_t cl=0; cl<nClsArr; cl++) {
    HMdcCluster& cls=clusArr[cl];
    if(!cls.status || cls.flag>0) continue;
    HMdcList12GroupCells& lCells=cls.lCells1;
    for(Int_t cl1=0; cl1<nClsArrM1; cl1++) {
      HMdcCluster& cls1=clusArrM1[cl1];
      if(!cls1.status || cls1.flag>0) continue;
      HMdcList12GroupCells& lM1Cells1=cls1.lCells1;
      Float_t dY=cls.y-cls1.y;
      //if(dY>100.) break;  //????????? biylo zakomentirovano ???
      if(fabs(dY) > 30.) continue;                   // 10. mm !???
      if(fabs(cls.x-cls1.x) > 100.) continue;        // 40. mm !???
      if(lM1Cells1.getNCells()-lCells.nIdentDrTimes(&lM1Cells1) >= 4) continue; 
      cls1.flag=1;
    }
    for(Int_t cl2=0; cl2<nClsArrM2; cl2++) {
      HMdcCluster& cls2=clusArrM2[cl2];
      if(!cls2.status || cls2.flag>0) continue;
      HMdcList12GroupCells& lM1Cells2=cls2.lCells1;
      Float_t dY=cls.y-cls2.y;
      // if(dY>100.) break;  //????????? biylo zakomentirovano ???
      if(fabs(dY) > 30.) continue;                   // 10. mm !???
      if(fabs(cls.x-cls2.x) > 100.) continue;        // 40. mm !??
      if(lM1Cells2.getNCells()-lCells.nIdentDrTimes(&lM1Cells2) >= 4) continue; 
      cls2.flag=1;
    }
  }
}

void HMdcLookUpTbSec::mergeClusInSeg(void) {
  Int_t nClus=nClsArr;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      HMdcCluster& cls1=clusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        HMdcCluster& cls2=clusArr[cl2];
        if(!cls2.status) continue;

        if(cls1.clusMod1 != cls2.clusMod1) continue;
        if(cls1.clusMod2 != cls2.clusMod2) continue;

        HMdcList12GroupCells& listCells2=cls2.lCells1;
        Float_t dY=cls1.y-cls2.y;
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 30.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 100.) continue;       // 40. mm !???
        Int_t n1=listCells1.compare(&listCells2,0, 5);
        if(n1<3) continue;
        Int_t n2=listCells1.compare(&listCells2,6,11);
        if(n2<3 || n1+n2<7) continue;
//        if(listCells1.compare(&listCells2,0, 5)<4) continue;
//        if(listCells1.compare(&listCells2,6,11)<4) continue;
        cls1.addClus(cls2,kFALSE);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::mergeClusInSec(void) {
  Int_t nClus=nClsArr;
  Int_t m1=minAmp[0];
  Int_t m2=minAmp[1];
  Int_t m3=minAmp[2];
  Int_t m4=minAmp[3];
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      HMdcCluster& cls1=clusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      HMdcList12GroupCells& listCells2=cls1.lCells2;
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        HMdcCluster& cls2=clusArr[cl2];
        if(!cls2.status) continue;
        HMdcList12GroupCells* listCells1s=&(cls2.lCells1);
        HMdcList12GroupCells* listCells2s=&(cls2.lCells2);
        Float_t dY=cls1.y-cls2.y;
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 30.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 100.) continue;       // 40. mm !???
        if(m1>0 && listCells1.compare(listCells1s,0, 5)<4) continue;
        if(m2>0 && listCells1.compare(listCells1s,6,11)<4) continue;
        if(m3>0 && listCells2.compare(listCells2s,0, 5)<4) continue;
        if(m4>0 && listCells2.compare(listCells2s,6,11)<4) continue;
        cls1.addClus(cls2,kTRUE);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::calcClParam(void) {
  for(Int_t nCl=0; nCl<(*cNClusArr); nCl++)
    if(cClusArr[nCl].status) cClusArr[nCl].calcClusParam(xHStep2,yHStep2);
}

void HMdcLookUpTbSec::fillClusCat(Int_t mod, Int_t segp, Int_t tpClFndr) {
  Int_t seg=(segp>=0) ? segp : 0;
  Int_t m1=minAmp[seg*2];
  Int_t m2=minAmp[seg*2+1];
  Int_t m3=0;
  Int_t m4=0;
  if(mod>=0) {
    if(mod==seg*2) m2=0;
    else if(mod==seg*2+1) m1=0;
  } else if(segp<0) {
    m3=minAmp[2];
    m4=minAmp[3];
  }
  // Filling containers:
  for(Int_t cl=0; cl<(*cNClusArr); cl++) {
    HMdcCluster& cls=cClusArr[cl];
    if(!cls.status || cls.flag>0) continue;
    Int_t index1=getClusterSlot(seg,cls.lCells1);
    if(!fClus) return;
    nClusters++;
    fClus->setMod(mod);
    fClus->setTypeClFinder(tpClFndr); // !!!
    fClus->setMinCl(m1,m2);
    fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
    fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],target[2],eTarg[2]);
    cls.fillClus(fClus,0,fillTrackList);
    if(fillTrackList && fClus->isGeant()) ((HMdcClusSim*)fClus)->calcTrList();
    if(segp<0) {
      HMdcClus* fClus0=fClus;
      Int_t index2=getClusterSlot(1,cls.lCells2);
      if(!fClus) return;
      fClus->setMod(mod);
      fClus->setTypeClFinder(tpClFndr);
      fClus->setMinCl(m3,m4);
      fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
      fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],
          target[2],eTarg[2]);
      cls.fillClus(fClus,1,fillTrackList);
      fClus->setIndexParent(index1);
      fClus0->setIndexChilds(index2,index2);
      if(fillTrackList && fClus->isGeant()) ((HMdcClusSim*)fClus)->calcTrList();
    }
  }
}

void HMdcLookUpTbSec::findClusInMod(Int_t mod, Short_t minAm){
  if(maxAmp[mod]<minAmp[mod]) return;
  if(minAm<3) return;
  makeModPlot(mod,minAm);
  Char_t maxClS=minAmp[mod]-1;
  nClsArr=0;
  UChar_t* hPlModM=hPlMod[mod];
  cPlotBAr=plotBArM[mod];
  Int_t* plotBAr4b=(Int_t*) cPlotBAr;
  Int_t minBin=minBinBAr4M[mod];
  for(Int_t n4=maxBinBAr4M[mod]; n4>=minBin; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      clus=&clusArr[nClsArr];
      Int_t nBin=nBin4+nLBin;
      initCluster(nBin);
      while ((nBin=stack->pop()) >= 0) addBinInCluster(nBin,
            HMdcBArray::getNSet(hPlModM[nBin])-maxClS);
      //-Filling of cluster---------------------------------------------------
      clus->calcXY();
      fillWiresList(mod,clus->lCells1);
      nClsArr++;
      if(nClsArr >= clusArrSize) break;
    }
    if(nClsArr >= clusArrSize) {
      Warning("scanClusInMod"," Num. of clusters in sector %i > max\n",sector);
      memset(cPlotBAr,0,sizeBAr);
      break;
    }
  }
  if(nClsArr>0) {
    setCurrentArray(clusArr,&nClsArr);
    if(nClsArr>1) mergeClusInMod(mod);
    calcClParam();
    fillClusCat(mod,mod/2,(typeClFinder==1) ? 1:0);
  }
  clearPrMod(mod);
}

//---------------------------------------------------------
HMdcLookUpTb* HMdcLookUpTb::fMdcLookUpTb=0;

HMdcLookUpTb::HMdcLookUpTb(const char* name,const char* title,
                           const char* context)
            : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HMdcLookUpTbSec
  strcpy(detName,"Mdc");
  fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return;
  array = new TObjArray(6);
  fMdcDet       = fGetCont->getMdcDetector();
  fMdcGeomPar   = fGetCont->getMdcGeomPar();
  fSpecGeomPar  = fGetCont->getSpecGeomPar();
  fLayerGeomPar = fGetCont->getMdcLayerGeomPar();
  fSizesCells   =  HMdcSizesCells::getObject();
  setPar(319, kFALSE);   // Project plot size is the same for all sectors!!!
  fMdcClusCat = HMdcGetContainers::getCatMdcClus(kTRUE);
  targLenInc[0]=targLenInc[1]=0.;
  quietMode=kFALSE;
}

void HMdcLookUpTb::setPar(Int_t inBinX, Bool_t isCOff) {
  nBinX=(inBinX%2 == 0) ? inBinX:inBinX+1;
  if(nBinX<100) nBinX=320;
  nBinY=(nBinX*278)/100;
  if(nBinY%2 == 0) nBinY++;
  isCoilOff=isCOff;
}

Bool_t HMdcLookUpTb::initContainer(void) {
  // It is called from "reinit" of reconstractor!
  if( !fMdcDet || !HMdcGetContainers::isInited(fMdcGeomPar) ||
      !HMdcGetContainers::isInited(fSpecGeomPar) ||
      !HMdcGetContainers::isInited(fLayerGeomPar) ||
      !fSizesCells->initContainer()) return kFALSE;

  if( !status && (fSizesCells->hasChanged() || fSpecGeomPar->hasChanged() ||
      fMdcGeomPar->hasChanged() || fLayerGeomPar->hasChanged()) ) {
    changed=kTRUE;
    if(!fMdcClusCat) return kFALSE;
    for (Int_t sec = 0; sec < 6; sec++) {
      if(!fMdcDet->isSectorActive(sec))  continue;
      if( !(*array)[sec] ) {
        const Int_t* nLM=HMdcTrackDSet::getTrFnNLayersInMod()+sec*4;
        Int_t nSegs=((fMdcDet->getModule(sec,0) && nLM[0]>0) ||
                     (fMdcDet->getModule(sec,1) && nLM[1]>0)) ?1:0;
        if(isCoilOff && ((fMdcDet->getModule(sec,2) && nLM[2]>0) || 
                        (fMdcDet->getModule(sec,3) && nLM[3]>0))) nSegs=2;
//          nSegs=(fMdcDet->getModule(sec,2) || fMdcDet->getModule(sec,3)) ?2:1;
        if(nSegs==0) continue;
        (*array)[sec] = new HMdcLookUpTbSec(sec,nSegs,nBinX,nBinY);
        (*this)[sec].fClusCat=fMdcClusCat;
      }
      // initialization of container ---
      if(!calcPrPlane(sec)) return kFALSE;
      if(!calcTarget(sec)) return kFALSE;
      if(!calcPlotSize(sec)) return kFALSE;
      if(!calcLookUpTb(sec)) return kFALSE;
      // --------------------------------
    }
    if(versions[1]<0 || versions[2]<0) versions[1]=versions[2]=0;
    else versions[2]++;
  } else changed=kFALSE;
  return kTRUE;
}

HMdcLookUpTb* HMdcLookUpTb::getExObject(void) {
  return fMdcLookUpTb;
}

HMdcLookUpTb* HMdcLookUpTb::getObject(void) {
  if(!fMdcLookUpTb) fMdcLookUpTb=new HMdcLookUpTb();
  return fMdcLookUpTb;
}

void HMdcLookUpTb::deleteCont(void) {
  if(fMdcLookUpTb) {
    delete fMdcLookUpTb;
    fMdcLookUpTb=0;
  }
}

HMdcLookUpTb::~HMdcLookUpTb(void) {
  // destructor
  HMdcGetContainers::deleteCont();
  HMdcSizesCells::deleteCont();
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcLookUpTb::getSize(void) {
  // return the size of the pointer array
  return array->GetEntries();
}

void HMdcLookUpTb::clear(void) {
  for(Int_t s=0;s<6;s++) {
   if( (*array)[s] ) {
      HMdcLookUpTbSec& sec=(*this)[s];
      sec.clearwk();
    }
  }
}

Int_t  HMdcLookUpTb::findClusters(Int_t *imax){
  Int_t ntot=0;
  for(Int_t sec=0;sec<6;sec++) {
    if( (*array)[sec] ) {
      HMdcLookUpTbSec& fSec=(*this)[sec];
      ntot+=fSec.findClusters(imax+4*sec);
    }
  }
  return ntot;
}

TH2C* HMdcLookUpTbSec::fillTH2C(Char_t* name, Char_t* title, Int_t type,
    Int_t bining){
  // Proj.plot filling without cluster finding.
  for(Int_t mod=0; mod<4; mod++) {
    cFMod=&((*this)[mod]);
    if( !cFMod ) continue;
    cHPlModM=hPlMod[mod];
    if(!hPlMod) continue;
    cXMinM=xMin[mod];
    cXMaxM=xMax[mod];
    Int_t iLay=mod*6;
    for(Int_t layer=0;layer<6;layer++) makeLayProjV1(layer,iLay+layer);
  }

  if(!hist) {
    plBining=(bining) ? 2:1;
    hist=new TH2C(name,title,nBinX/plBining,xLow,xUp,nBinY/plBining,yLow,yUp);
  } else {
    hist->Reset();
    hist->SetName(name);
    hist->SetTitle(title);
  }
  hist->SetMinimum(0.);
  hist->Fill(0.,0.,0);
  Float_t maximum=6.;
  if(type==2) maximum=4.5;
  else if(type==0) maximum=(hPlMod[2]) ? 24.:12.;
  else if(type==1) maximum=(hPlMod[2]) ? 60.:18.;
  hist->SetMaximum(maximum);
  if(nHitsTot==0) return hist;
  for (Int_t nx=0; nx<nBinX; nx+=plBining) {
    for (Int_t ny=0; ny<nBinY; ny+=plBining) {
      Int_t nBin=ny*nBinX+nx;
      Int_t nBin2=nBin+nBinX;
      UChar_t binM1=hPlMod[0][nBin];
      UChar_t binM2=hPlMod[1][nBin];
      if(plBining==2) {
        binM1 |= hPlMod[0][nBin+1] | hPlMod[0][nBin2] | hPlMod[0][nBin2+1];
        binM2 |= hPlMod[1][nBin+1] | hPlMod[1][nBin2] | hPlMod[1][nBin2+1];
      }
      UChar_t binM3=0;
      UChar_t binM4=0;
      if(hPlMod[2]) {
        binM3=hPlMod[2][nBin];
        binM4=hPlMod[3][nBin];
        if(plBining==2) {
          binM3|=hPlMod[2][nBin+1] | hPlMod[2][nBin2] | hPlMod[2][nBin2+1];
          binM4|=hPlMod[3][nBin+1] | hPlMod[3][nBin2] | hPlMod[3][nBin2+1];
        }
      }
      if( binM1==0 && binM2==0 && binM3==0 && binM4==0) continue;
      Int_t color=0;
      if(type==1) {
        if(binM4) color=HMdcBArray::getNSet(binM4)+18;
        else if(binM3) color+=HMdcBArray::getNSet(binM3)+12;
        else if(binM2) color+=HMdcBArray::getNSet(binM2)+6;
        else if(binM1) color+=HMdcBArray::getNSet(binM1);

// Int_t m1=HMdcBArray::getNSet(binM1);
// Int_t m2=HMdcBArray::getNSet(binM2);
// color=(m1>m2) ? m2:m1; //m1:m2;
	
      } else if(type==2) {
        if(binM4) color=4;
        else if(binM3) color=3;
        else if(binM2) color=2;
        else if(binM1) color=1;
      } else {
        color=HMdcBArray::getNSet(binM1)+HMdcBArray::getNSet(binM2)+
              HMdcBArray::getNSet(binM3)+HMdcBArray::getNSet(binM4);
      }
      hist->Fill(xBinsPos[nx],yBinsPos[ny], color);
    }
  }
  for(Int_t mod=0;mod<4;mod++) clearPrMod(mod);
  return hist;
}

Bool_t HMdcLookUpTb::calcLookUpTb(Int_t sec) {
  HMdcLookUpTbSec& fsec=(*this)[sec];
  HMdcSizesCellsSec& fSCSec = (*fSizesCells)[sec];
  HMdcTrap cellSize;
  //Modules:
  for(Int_t mod=0; mod<4; mod++) {
    if( !&(fsec[mod]) ) continue;
    for(Int_t layer=0; layer<6; layer++) {
      HMdcSizesCellsLayer& fSCLayer = fSCSec[mod][layer];
      Int_t nCells=fSCLayer.getSize();
      if(nCells<1) continue;
      HMdcLookUpTbLayer& fLUpTbLayer=fsec[mod][layer];
      //Cells:
      for(Int_t cell=0; cell<nCells; cell++) {
        HMdcLookUpTbCell& fLUpTbCell=fLUpTbLayer[cell];
        if( !&fLUpTbCell ) return kFALSE;
        fLUpTbCell.clear();
        HMdcSizesCellsCell& fSizesCellsCell = fSCLayer[cell];
        if( !&fSizesCellsCell || !fSizesCellsCell.getStatus()) continue;
        if(!fSizesCells->getCellVol(sec,mod,layer,cell,cellSize)) continue;
        HMdcTrapPlane cellProj;
        calcCellProj(sec,cellSize,cellProj);
        //Lines 0,1,2,3 - lines from points 0-1, 1-2, 2-3, 3-0 respectively
        Int_t yBinMinT=0;
        Int_t yBinMaxT=0;
        for(Int_t i=1; i<4; i++) {
          if(cellProj[yBinMinT].gety() > cellProj[i].gety()) yBinMinT=i;
	  if(cellProj[yBinMaxT].gety() < cellProj[i].gety()) yBinMaxT=i;
        }
        yBinMinT=(Int_t)((cellProj[yBinMinT].gety()-fsec.yLow)/fsec.yStep);
        yBinMaxT=(Int_t)((cellProj[yBinMaxT].gety()-fsec.yLow)/fsec.yStep);
        if(!quietMode && (yBinMinT<1 || yBinMinT>fsec.nBinY-2)) {
          Warning("calcLookUpTb","S%iM%iL%iC%i yBinMinT=%i isn't in 1 - %i !",
              sec+1,mod+1,layer+1,cell+1,yBinMinT,fsec.nBinY-2);
          yBinMinT=(yBinMinT<1) ? 1:fsec.nBinY-2;
        }
        if(!quietMode && (yBinMaxT<1 || yBinMaxT>fsec.nBinY-1)) {
          Warning("calcLookUpTb","S%iM%iL%iC%i  yBinMaxT=%i isn't in 1 - %i !",
              sec+1,mod+1,layer+1,cell+1,yBinMaxT,fsec.nBinY-2);
          yBinMaxT=(yBinMaxT<1) ? 1:fsec.nBinY-2;
        }
        fLUpTbCell.init(yBinMinT,yBinMaxT);
        Int_t nx1t,nx2t;
        nx1t=+1000000000;
        nx2t=-1000000000;
        Int_t nxiMin=1;
        Int_t nxiMax=fsec.nBinX-2;
        for(Int_t ny=yBinMinT; ny<=yBinMaxT; ny++) {
          Double_t y=(ny+1)*fsec.yStep+fsec.yLow;
          Double_t x1,x2;
          calcX(cellProj, y, x1, x2);	
          Int_t nx1i=(Int_t)((x1-fsec.xLow)/fsec.xStep);
	  Int_t nx2i=(Int_t)((x2-fsec.xLow)/fsec.xStep);
	  if(nx1i < nx1t) nx1t=nx1i;
	  if(nx2i > nx2t) nx2t=nx2i;
          if(nx1i<1 || nx1i>fsec.nBinX-2) {
            if(nx1i<nxiMin) nxiMin=nx1i;
            else if(nx1i>nxiMax) nxiMax=nx1i;
            nx1i=(nx1i<1) ? 1:fsec.nBinX-2;
          }
          if(nx2i<1 || nx2i>fsec.nBinX-2) {
            if(nx2i<nxiMin) nxiMin=nx2i;
            else if(nx2i>nxiMax) nxiMax=nx2i;
            nx2i=(nx2i<1) ? 1:fsec.nBinX-2;
          }
          //Fill look up table:
          fLUpTbCell.addLine(nx1t,nx2t);
	  if(ny==yBinMaxT-1 && yBinMinT!=yBinMaxT) {
            if(!fLUpTbCell.addLine(nx1t,nx2t)) return kFALSE;
            ny++;
          }
	  nx1t=nx1i;
	  nx2t=nx2i;	
        }
        if(!quietMode && (nxiMin<1 || nxiMax>nBinX-2)) {
          Warning("calcLookUpTb","S%iM%iL%iC%i  nx1/2i bins %i - %i out of 1 - %i !",
            sec+1,mod+1,layer+1,cell+1,nxiMin,nxiMax,fsec.nBinX-2);
        }
      }
    }
  }
  return kTRUE;
}

void HMdcLookUpTb::calcX(HMdcTrapPlane& pr, Double_t y,
                           Double_t &xLow, Double_t &xUp) {
  xLow=1.0e+30;
  xUp=-1.0e+30;
  for(Int_t i=0; i<4; i++){
    Int_t u1=i;
    Int_t u2=(u1<3) ? u1+1:0;
    if(fabs(pr[u1].gety()-pr[u2].gety())<1.e-4) continue;
    if(pr[u1].getx()>pr[u2].getx()) {
      Int_t u=u1;  u1=u2;  u2=u;
    }
    Double_t x=(y-pr[u1].gety())/(pr[u1].gety()-pr[u2].gety())*
              (pr[u1].getx()-pr[u2].getx())+pr[u1].getx();
    if(x<pr[u1].getx() || x>pr[u2].getx()) continue;
    if(x<xLow) xLow=x;
    if(x>xUp) xUp=x;
  }
  //Attention pr[0]&pr[1] mast be >= pr[2]&pr[3] (0-3; 1-2)!!!!
  if(xLow<-2000) xLow=(pr[2].getx() < pr[3].getx()) ? pr[2].getx():pr[3].getx();
  if(xUp> 2000) xUp=(pr[0].getx() > pr[1].getx()) ? pr[0].getx():pr[1].getx();
}


Bool_t HMdcLookUpTb::calcPrPlane(Int_t sec){
  //  Calc. of projection plane:
  //  It's a plane between first and last MDC in sector
  //  (without magnetic field) or in inner segment (with magnetic fild). 
  //  par=0.0 -> plane=layer 6 of first MDC
  //  par=1.0 -> plane=layer 1 of last MDC
  //  If there is one MDC only - middle plane of MDC will used.
  Int_t firstMod=-1;
  Int_t lastMod=0;
  Int_t nMaxMods=(isCoilOff) ? 4:2;
  Int_t nmod=0;
  const Int_t* nLM=HMdcTrackDSet::getTrFnNLayersInMod()+sec*4;
  for(Int_t m=0; m<nMaxMods; m++) {
    if(fMdcDet->getModule(sec,m) && nLM[m]>0) {
      nmod++;
      if(firstMod<0) firstMod=m;
      lastMod=m;
    }
  }
  if(nmod==0) return kFALSE;
  if(nmod>1) {
    //  Double_t par=0.0;   // <<<===================== use MDC1
    //  Double_t par=1.0;   // <<<===================== use MDC2
    Double_t par=0.553; //0.551232;
    HMdcPlane m1((*fSizesCells)[sec][firstMod][5]);
    HMdcPlane m2((*fSizesCells)[sec][ lastMod][0]);
    Double_t dNew=m1.D()*(1.-par)+m2.D()*par;
    Double_t bNew=dNew/(m1.D()*(1.-par)/m1.B()+m2.D()*par/m2.B());
    ((*this)[sec]).prPlane.setPlanePar( 0., bNew, 1., dNew);
    if(!quietMode) printf(
       "\n===> Sec.%i: Using plane between MDC%i and MDC%i (p=%f) as projection plane\n",
        sec+1,firstMod+1,lastMod+1,par);
  } else {
    ((*this)[sec]).prPlane.setPlanePar((*fSizesCells)[sec][firstMod]);
    if(!quietMode) printf(
        "\n===> Sec.%i: Using middle plane of MDC%i as projection plane\n",
        sec+1,firstMod+1);
  }
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcTarget(Int_t sec){
  // Calculation of target parameters
  HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sec];
  HMdcLookUpTbSec& fsec=(*this)[sec];
  if(&fSCSec==0 || &fsec==0) return kFALSE;
  fsec.targVc[0]=fSCSec.getTargetFirstPoint();
  fsec.targVc[1]=fSCSec.getTargetLastPoint();
  fsec.targVc[0].setZ( fsec.targVc[0].getZ() - targLenInc[0]);
  fsec.targVc[1].setZ( fsec.targVc[1].getZ() + targLenInc[1]);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// fsec.targVc[0].setX(0.);
// fsec.targVc[0].setY(0.);
// fsec.targVc[1].setX(0.);
// fsec.targVc[1].setY(0.);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for(Int_t i=0;i<3;i++) {
    fsec.target[i]=(fsec.targVc[0](i)+fsec.targVc[1](i))*0.5;
    fsec.eTarg[i]=fabs(fsec.targVc[0](i)-fsec.targVc[1](i))*0.5;
  }
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcPlotSize(Int_t sec){
  HMdcLookUpTbSec& fsec=(*this)[sec];
  HGeomVector senVol;
  HGeomVector cross;
  fsec.xLow=fsec.yLow=1.0e+30;
  fsec.xUp=fsec.yUp=-1.0e+30;
  for(Int_t mod=0; mod<4; mod++) {
    if( &(fsec[mod]) == 0 ) continue;
    HGeomCompositeVolume* fComVol=fGetCont->getGeomCompositeVolume(mod);
    if(!fComVol) {
      Error("calcPlotSize","GeomCompositeVolume for MDC%i Sec.%i is absent.",
          mod+1,sec+1);
      return kFALSE;
    }

    for(Int_t layer=0; layer<6; layer+=5) {
      HGeomVolume* fGeomVolLay=fComVol->getComponent(layer);
      if(!fGeomVolLay || fGeomVolLay->getNumPoints()!=8) return kFALSE;
      HMdcSizesCellsLayer& fSizesCellsLay=(*fSizesCells)[sec][mod][layer];
      if( !&fSizesCells ) return kFALSE;
      HMdcLayerGeomParLay& fLayerGeomParLay=(*fLayerGeomPar)[sec][mod][layer];
      if( !&fLayerGeomParLay )return kFALSE;
      Double_t dstCathPl=(layer/2-1)*fLayerGeomParLay.getCatDist()*0.5;
      for(Int_t point=0; point<4; point++) {
        senVol=*(fGeomVolLay->getPoint(point)); // mm!
        senVol.setZ(dstCathPl);
        senVol=fSizesCellsLay.getSecTrans()->transFrom(senVol);
        for(Int_t targ=0; targ<2; targ++) {
          fsec.prPlane.calcIntersection(fsec.targVc[targ],
                       senVol-fsec.targVc[targ],cross);
          if(cross(0)<fsec.xLow) fsec.xLow=cross(0);
          else if(cross(0)>fsec.xUp) fsec.xUp=cross(0);
          if(cross(1)<fsec.yLow) fsec.yLow=cross(1);
          else if(cross(1)>fsec.yUp) fsec.yUp=cross(1);
        }
      }
    }
  }
  Double_t del=(fsec.xUp-fsec.xLow)*0.02;
  fsec.xLow -= del;
  fsec.xUp  += del;
  del=(fsec.yUp-fsec.yLow)*0.01;
  fsec.yLow -= del;
  fsec.yUp  += del;
  fsec.xStep=(fsec.xUp-fsec.xLow)/(Double_t)(fsec.nBinX-3);
  fsec.yStep=(fsec.yUp-fsec.yLow)/(Double_t)(fsec.nBinY-3);
  fsec.yLow -= fsec.yStep*1.5;            //must be bin=0 at limits of plot
  fsec.yUp += fsec.yStep*1.5;
  fsec.xLow -= fsec.xStep*1.5;
  fsec.xUp += fsec.xStep*1.5;
  fsec.xStep=(fsec.xUp-fsec.xLow)/(Double_t)(fsec.nBinX);
  fsec.yStep=(fsec.yUp-fsec.yLow)/(Double_t)(fsec.nBinY);
  Double_t xHStep=fsec.xStep/2;
  Double_t yHStep=fsec.yStep/2;
  fsec.xHStep2=xHStep*xHStep;
  fsec.yHStep2=yHStep*yHStep;
  for(Int_t n=0; n<fsec.nBinX; n++)
      fsec.xBinsPos[n]=((Double_t)n)*fsec.xStep+fsec.xLow+xHStep;
  for(Int_t n=0; n<fsec.nBinY; n++)
      fsec.yBinsPos[n]=((Double_t)n)*fsec.yStep+fsec.yLow+yHStep;
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcCellProj(Int_t sec,HMdcTrap& cellSize,
    HMdcTrapPlane& cellProj) {
  // Calculation of cell projection on the proj. plane.
  // (For sector coordinat system only !!!)
  HGeomVector pProj[16];
  HMdcLookUpTbSec& fsec=(*this)[sec];
  for (Int_t i=0; i<8; i++) {
    for (Int_t j=0; j<2; j++) fsec.prPlane.calcIntersection(fsec.targVc[j],
      cellSize[i]-fsec.targVc[j],pProj[j*8+i]);
  }
  Double_t dist[2][8];
  Double_t sig=0.001;
  //Lines 1,5 for minmum:
  Bool_t indb=(fabs(pProj[0](0)-pProj[3](0)) > sig) ? kTRUE:kFALSE;
  for(Int_t i=0; i<2; i++) {
    for(Int_t j=0; j<2; j++) {
      dist[i][j*4]=pProj[i*8+j*4](1);
      if(indb) dist[i][j*4]=calcDist(pProj[i*8+j*4],pProj[i*8+j*4+3]);	
    }
  }
  //Lines 1,5 for maximum:
  Bool_t indu=(fabs(pProj[1](0)-pProj[2](0)) > sig) ? kTRUE:kFALSE;
  for(Int_t i=0; i<2; i++) {
    for(Int_t j=0; j<2; j++) {
      dist[i][j*4+1]=pProj[i*8+j*4+1](1);
      if(indu) dist[i][j*4+1]=calcDist(pProj[i*8+j*4+1],pProj[i*8+j*4+2]);
    }
  }
  // Minimum y:
  Int_t tb=0; Int_t pb=0;
  if(dist[1][0] < dist[tb][pb])  tb=1;
  if(dist[1][4] < dist[tb][pb]) {tb=1; pb=4;}
  if(dist[0][4] < dist[tb][pb]) {tb=0; pb=4;}
  //Maximum y:	
  Int_t tu=0; Int_t pu=1;
  if(dist[1][1] > dist[tu][pu])  tu=1;
  if(dist[1][5] > dist[tu][pu]) {tu=1; pu=5;}
  if(dist[0][5] > dist[tu][pu]) {tu=0; pu=5;}
  for(Int_t i=0; i<2; i++) {
   dist[i][0]=fabs(calcDist(pProj[i*8+0],pProj[i*8+1])); //line 0-1;
   dist[i][4]=fabs(calcDist(pProj[i*8+4],pProj[i*8+5])); //line 4-5;
   dist[i][2]=fabs(calcDist(pProj[i*8+2],pProj[i*8+3])); //line 2-3;
   dist[i][6]=fabs(calcDist(pProj[i*8+6],pProj[i*8+7])); //line 2-3
  }
  // Minimum-minmum for lines 0-1;4-5;  2-3;6-7:
  Int_t tl=0; Int_t pl=0;
  if(dist[1][0] > dist[tl][pl])  tl=1;
  if(dist[1][4] > dist[tl][pl]) {tl=1; pl=4;}
  if(dist[0][4] > dist[tl][pl]) {tl=0; pl=4;}
  Int_t tr=0; Int_t pr=2;
  if(dist[1][2] > dist[tr][pr])  tr=1;
  if(dist[1][6] > dist[tr][pr]) {tr=1; pr=6;}
  if(dist[0][6] > dist[tr][pr]) {tr=0; pr=6;}
  if(indb) {
    calcPoint(cellProj[0],pProj[tb*8+pb],pProj[tb*8+pb+3],
	                  pProj[tl*8+pl],pProj[tl*8+pl+1]);
    calcPoint(cellProj[3],pProj[tb*8+pb],pProj[tb*8+pb+3],
	                  pProj[tr*8+pr],pProj[tr*8+pr+1]);
  }
  if(indu) {
    calcPoint(cellProj[1],pProj[tu*8+pu],pProj[tu*8+pu+1],
	                  pProj[tl*8+pl],pProj[tl*8+pl+1]);
    calcPoint(cellProj[2],pProj[tu*8+pu],pProj[tu*8+pu+1],
	                  pProj[tr*8+pr],pProj[tr*8+pr+1]);
  }
  Double_t x,y;
  if(!indb) {
    x=(pProj[tb*8+pb](0)+pProj[tb*8+pb+3](0))*0.5;
    y=(pProj[tb*8+pb](1)+pProj[tb*8+pb+3](1))*0.5;
    if(y<0. || y>10000.) {
      Error("calcCellProj","Sec.%i p0,3  x=%f y=%f(!)",sec+1,x,y);
      return kFALSE;
    }
    cellProj[0].set(x,y);
    cellProj[3].set(x,y);
  }
  if(!indu) {
    x=(pProj[tb*8+pu](0)+pProj[tb*8+pu+1](0))*0.5;
    y=(pProj[tb*8+pu](1)+pProj[tb*8+pu+1](1))*0.5;
    if(y<0. || y>10000.) {
      Error("calcCellProj","Sec.%i p1,2  x=%f y=%f(!)",sec+1,x,y);
      return kFALSE;
    }
    cellProj[1].set(x,y);
    cellProj[2].set(x,y);
  }
  return kTRUE;
}

Double_t HMdcLookUpTb::calcDist(HGeomVector& p1, HGeomVector& p2) {
  return p1(1)-p1(0)*(p1(1)-p2(1))/(p1(0)-p2(0));
}

void HMdcLookUpTb::calcPoint(HMdcPointPlane& proj,
  HGeomVector& p1l1, HGeomVector& p2l1, HGeomVector& p1l2, HGeomVector& p2l2) {
  Double_t al1=(p1l1(1)-p2l1(1))/(p1l1(0)-p2l1(0));
  Double_t bl1=p1l1(1)-al1*p1l1(0);
  Double_t al2=(p1l2(1)-p2l2(1))/(p1l2(0)-p2l2(0));
  Double_t bl2=p1l2(1)-al2*p1l2(0);
  Double_t x=(bl2-bl1)/(al1-al2);
  Double_t y=al2*x+bl2;
  proj.set(x,y);
}


void HMdcLookUpTb::donotFillTrackList(void) {
  for(Int_t sec=0;sec<6;sec++) {
    if( !(*array)[sec] ) continue;
    (*this)[sec].fillTrackList=kFALSE;
  }
}
