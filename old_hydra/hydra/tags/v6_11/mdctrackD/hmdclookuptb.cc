#include "hmdclookuptb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hmdcgetcontainers.h"
#include <iostream.h>
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

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 04/06/2002 by V.Pechenov
//*-- Modified : 09/05/2001 by V.Pechenov
//*-- Modified : 12/07/2000 by V.Pechenov
//*-- Modified : 23/05/2000 by V.Pechenov
//*-- Modified : 07/03/2000 by R. Holzmann
//*-- Modified : 02/12/99 by V.Pechenov
//*-- Modified : 26/10/99 by V.Pechenov
//*-- Modified : 20/05/99

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
ClassImp(HMdcClFnStack)

HMdcLookUpTbCell::HMdcLookUpTbCell() {
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

HMdcLookUpTbCell::~HMdcLookUpTbCell() {
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
  Int_t nCells=((*fMdcGeomStruct)[sec][mod])[layer];
  array = new TObjArray(nCells);
  for(Int_t cell=0; cell<nCells; cell++) (*array)[cell]=new HMdcLookUpTbCell();
}

HMdcLookUpTbLayer::~HMdcLookUpTbLayer() {
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

HMdcLookUpTbMod::~HMdcLookUpTbMod() {
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

//----------HMdcClFnStack-----------------------------
HMdcClFnStack::HMdcClFnStack(Int_t size) {
  stackSize=(size>0) ? size:1000;
  stack=new Int_t [stackSize+100];
  stackPos=0;
}

HMdcClFnStack::~HMdcClFnStack() {
  if(stack) delete [] stack;
  stack=0;
}

void HMdcClFnStack::increaseStack(void) {
  Warning("increaseStack","Stack increased on 50\%");
  stackSize += stackSize/2;
  Int_t* stackNew=new Int_t [stackSize+50];
  for(Int_t n=0; n<=stackPos; n++) stackNew[n]=stack[n];
  delete [] stack;
  stack=stackNew;
}

//----------HMdcCluster-------------------------------
HMdcCluster::HMdcCluster() {
  radToDeg=180./acos(Double_t(-1.));
  xMaxCl=xMinCl=0;
}

void HMdcCluster::initArr(Int_t size) {
  xMaxCl=new Int_t [size];
  xMinCl=new Int_t [size];
}

HMdcCluster::~HMdcCluster() {
  if(xMaxCl) delete [] xMaxCl;
  if(xMinCl) delete [] xMinCl;
  xMaxCl=xMinCl=0;
}
    
void HMdcCluster::clear(void) {
  status=kTRUE;
  nMergedClus=1;
  lCells1.clear();
  lCells2.clear();
  nBins=0;
  meanX=meanY=meanXX=meanYY=meanYX=0.;
  sumWt=meanXWt=meanYWt=meanXXWt=meanYYWt=0.;
  nLMax=-1;
  nLMin=1000000;
  clusMod1=clusMod2=0;
  numSegCl=0;
}

void HMdcCluster::addClus(HMdcCluster& clst2, Bool_t flag) {
  lCells1.add(&(clst2.lCells1));
  if(flag) lCells2.add(&(clst2.lCells2));
  clst2.status=kFALSE;
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
  Int_t lMin=(nLMin<clst2.nLMin) ? nLMin:clst2.nLMin;
  Int_t lMax=(nLMax>clst2.nLMax) ? nLMax:clst2.nLMax;
  for(Int_t l=lMin; l<=lMax; l++) {
    if( (l<nLMin && l>clst2.nLMax) || (l>nLMax && l<clst2.nLMin) ) xMinCl[l]=-1;
    else if(l<nLMin || l>nLMax || xMinCl[l]<0) {
      xMinCl[l]=clst2.xMinCl[l];
      xMaxCl[l]=clst2.xMaxCl[l];
    } else if(l>=clst2.nLMin && l<=clst2.nLMax && clst2.xMinCl[l]>=0) {
      if(clst2.xMinCl[l]<xMinCl[l]) xMinCl[l]=clst2.xMinCl[l];
      if(clst2.xMaxCl[l]>xMaxCl[l]) xMaxCl[l]=clst2.xMaxCl[l];
    }
  }
  nLMin=lMin;
  nLMax=lMax;
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
    sigma1=norm+sqrt(norm*norm-c);
    sigma2=norm-sqrt(norm*norm-c);
    if(sigma2<0.) sigma2=0.;
    Double_t e1=sigma1-eXX;
    Double_t e2=sigma1-eYY;
    sigma1=sqrt(sigma1)/nBn;
    sigma2=sqrt(sigma2)/nBn;
    alpha=atan2(sqrt(eYX2+e1*e1),sqrt(eYX2+e2*e2))*radToDeg;
    if(eYX<0.) alpha=180.-alpha;
  }
}

void HMdcCluster::fillClus(HMdcClus* fClus, Int_t nLst) {
  fClus->setSumWt(sumWt);
  fClus->setNBins(nBins);
  fClus->setXY(x,errX,y,errY);
  fClus->setNMergClust(nMergedClus);
  if(clusMod1==0) {
    Int_t nDrTm=(nLst==0) ? lCells1.getNDrTimes(0,5):lCells2.getNDrTimes(0,5);
    if(nDrTm>0) {
      fClus->setClusSizeM1(nBins);
      fClus->setNDrTimesM1(nDrTm);
      fClus->setNMergClustM1(nMergedClus);
      fClus->setShapeM1(sigma1,sigma2,alpha);
    } else fClus->clearMod1Par();
  } else {
    fClus->setClusSizeM1(clusMod1->nBins);
    fClus->setNDrTimesM1(clusMod1->lCells1.getNDrTimes(0,5));
    fClus->setNMergClustM1(clusMod1->nMergedClus);
    fClus->setShapeM1(clusMod1->sigma1,clusMod1->sigma2,clusMod1->alpha);
  }
  if(clusMod2==0) {
    Int_t nDrTm=(nLst==0) ? lCells1.getNDrTimes(6,11):lCells2.getNDrTimes(6,11);
    if(nDrTm>0) {
      fClus->setClusSizeM2(nBins);
      fClus->setNDrTimesM2(nDrTm);
      fClus->setNMergClustM2(nMergedClus);
      fClus->setShapeM2(sigma1,sigma2,alpha);
    } else fClus->clearMod2Par();
  } else {
    fClus->setClusSizeM2(clusMod2->nBins);
    fClus->setNDrTimesM2(clusMod2->lCells1.getNDrTimes(6,11));
    fClus->setNMergClustM2(clusMod2->nMergedClus);
    fClus->setShapeM2(clusMod2->sigma1,clusMod2->sigma2,clusMod2->alpha);
  }
}

//----------Sector------------------------------------

HMdcClFnStack* HMdcLookUpTbSec::stack=0;
Int_t HMdcLookUpTbSec::sizeBArSt=0;
UChar_t* HMdcLookUpTbSec::plotBArSc=0;
UChar_t* HMdcLookUpTbSec::plotBArM1=0;
UChar_t* HMdcLookUpTbSec::plotBArM2=0;

HMdcLookUpTbSec::HMdcLookUpTbSec(Int_t sec, Int_t nSegs,
                                 Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  sector=sec;
  nSegments=nSegs;
  array = new TObjArray(4);
  HMdcDetector* fMdcDet= HMdcGetContainers::getObject()->getMdcDetector();
  for (Int_t mod = 0; mod < nSegs*2; mod++)
    if(fMdcDet->getModule(sec,mod)) (*array)[mod]=new HMdcLookUpTbMod(sec,mod);
  nBinX=(inBinX%2 == 0) ? inBinX:inBinX+1;
  nBinY=(inBinY%2 == 0) ? inBinY:inBinY+1;
  xBinsPos=new Double_t [nBinX];
  yBinsPos=new Double_t [nBinY];
  size=nBinX*nBinY;
  size=(size/32 + ((size%32 > 0) ? 1:0))*32;
  size4=size/2;
  hitPlS1=new Short_t [size4*2];
  hitPl4S1=(Int_t*) hitPlS1;
  hPlMod1=(UChar_t*) hitPlS1;
  hPlMod2=hPlMod1+1;
  hist=0;
  if(nSegs==2) {
    hitPlS2=new Short_t [size4*2];
    hitPl4S2=(Int_t*) hitPlS2;
    hPlMod3=(UChar_t*) hitPlS2;
    hPlMod4=hPlMod3+1;
  } else {
    hitPlS2=0;
    hitPl4S2=0;
    hPlMod3=0;
    hPlMod4=0;
  }
  if(plotBArSc==0 || sizeBArSt<size/8) {
    if(plotBArSc) delete [] plotBArSc;
    if(plotBArM1) delete [] plotBArM1;
    if(plotBArM2) delete [] plotBArM2;
    sizeBArSt=size/8;
    plotBArSc=new UChar_t [sizeBArSt];
    plotBArM1=new UChar_t [sizeBArSt];
    plotBArM2=new UChar_t [sizeBArSt];
    memset(plotBArSc,0,sizeBArSt);
    memset(plotBArM1,0,sizeBArSt);
    memset(plotBArM2,0,sizeBArSt);
  }
  sizeBAr=size/8;
  sizeBAr4b=sizeBAr/4;
  plotBArSc4b=(Int_t *)plotBArSc;
  plotBAr4bM1=(Int_t *)plotBArM1;
  plotBAr4bM2=(Int_t *)plotBArM2;
  typeClFinder=0;
  setLUpTb=kFALSE;
  nearbyBins[0]=-1;
  nearbyBins[1]=+1;
  nearbyBins[2]=-nBinX;
  nearbyBins[3]=+nBinX;
  nearbyBins[4]=-1-nBinX;
  nearbyBins[5]=-1+nBinX;
  nearbyBins[6]=1-nBinX;
  nearbyBins[7]=1+nBinX;
  if(stack==0) stack=new HMdcClFnStack(1000);
  isGeant = HMdcGetContainers::isGeant();
  locClus.set(3,sec,0,0);
  Int_t lnArr=sizeof(clusArr)/sizeof(clusArr[0]);
  for(Int_t i=0;i<lnArr; i++) clusArr[i].initArr(nBinY);
  lnArr=sizeof(clusArrM1)/sizeof(clusArrM1[0]);
  for(Int_t i=0;i<lnArr; i++) clusArrM1[i].initArr(nBinY);
  lnArr=sizeof(clusArrM2)/sizeof(clusArrM1[0]);
  for(Int_t i=0;i<lnArr; i++) clusArrM2[i].initArr(nBinY);
}

HMdcLookUpTbSec::~HMdcLookUpTbSec() {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  delete [] hitPlS1;
  if(stack) delete stack;
  stack=0;
  if(hitPlS2) delete [] hitPlS2;
  hitPlS2=0;
  if(hist && hist->IsOnHeap()) delete hist;
  hist=0;
  if(plotBArSc) {
    delete [] plotBArSc;
    plotBArSc=0;
    sizeBArSt=0;
  }
  if(plotBArM1) {
    delete [] plotBArM1;
    plotBArM1=0;
  }
  if(plotBArM2) {
    delete [] plotBArM2;
    plotBArM2=0;
  }
  delete [] xBinsPos;
  delete [] yBinsPos;
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
  HMdcLookUpTbCell& fLUpTbLayer=(*this)[mod][layer][cell];
  if( &fLUpTbLayer && fLUpTbLayer.line>0 ) {
    Short_t ind1=mod*6+layer;
    Short_t ind2=nHits[ind1];
    if (ind2<250) {
      hits[ind1][ind2]=cell;
      hitsNTime[ind1][ind2]=nTimes;
      hitsDel[ind1][ind2]=0;
      nHits[ind1]++;
      nHitsTot++;
    }
    else Error("setCell","Too many hits in layer.");
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

void HMdcLookUpTbSec::makePlot(void) {
  // Can be used for proj.plot filling without cluster finding!
  for(Int_t i=0; i<size4; i++) hitPl4S1[i]=0;
  if(hitPlS2) for(Int_t i=0; i<size4; i++) hitPl4S2[i]=0;
  
  for(Int_t mod=0; mod<2; mod++) {
    HMdcLookUpTbMod& fMod=(*this)[mod];
    if( !(&fMod) ) continue;
    Int_t nlay=mod*6;
    Short_t addm=1<<(mod*8);  // !!!
    for(Int_t layer=0; layer<6; layer++) {
      Int_t i=nlay+layer;
      Short_t add=addm<<layer;
      HMdcLookUpTbLayer& fLayer=fMod[layer];
      for(Int_t n=0; n<nHits[i]; n++) {
        if(hitsDel[i][n]<0) continue;
        HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
        for(Int_t ln=0; ln<fCell.line; ln++) {
          Int_t shift=(fCell.yBinMin+ln) * nBinX;
          Int_t nbL=fCell.xBinMax[ln]+shift;
          for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
            hitPlS1[nb] |= add;
          }
        }
      }
    }
  }
  // === Segment 2 (mdc3, mdc4) ==========================
  if(!hitPlS2) return;
  for(Int_t mod=2; mod<4; mod++) {
    HMdcLookUpTbMod& fMod=(*this)[mod];
    if( !(&fMod) ) continue;
    Int_t nlay=mod*6;
    Short_t addm=1<<((mod&1)*8);  // !!!
    for(Int_t layer=0; layer<6; layer++) {
      if(nHits[layer + mod*6]<=0) continue;
      Short_t add=addm<<layer;
      HMdcLookUpTbLayer& fLayer=fMod[layer];
      Int_t i=nlay+layer;
      for(Int_t n=0; n<nHits[i]; n++) {
        if(hitsDel[i][n]<0) continue;
        HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
        for(Int_t ln=0; ln<fCell.line; ln++) {
          Int_t shift=(fCell.yBinMin+ln) * nBinX;
          Int_t nbL=fCell.xBinMax[ln]+shift;
          for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
            hitPlS2[nb] |= add;
          }
        }
      }
    }
  }
}

void HMdcLookUpTbSec::makeSecPlot(Int_t minAm) {
  Int_t nLayers=0;
  maxBinBAr4Sc=-1;
  minBinBAr4Sc=sizeBAr4b*32;
  for(Int_t mod=0; mod<4; mod++) {
    HMdcLookUpTbMod& fMod=(*this)[mod];
    if( !(&fMod) ) continue;
    if(minAmp[mod]==0) continue;
    Short_t* hPlot=(mod<2) ? hitPlS1:hitPlS2;
    if(!hPlot) break;
    Int_t iLay=mod*6;
    Short_t addm=1<<((mod&1)*8);  // !!!
    for(Int_t layer=0; layer<6; layer++) {
      Int_t i=iLay+layer;
      if(nHits[i]==0) continue;
      nLayers++; // layers with fired wires only
      Short_t add=addm<<layer;
      HMdcLookUpTbLayer& fLayer=fMod[layer];
      for(Int_t n=0; n<nHits[i]; n++) {
        if(hitsDel[i][n]<0) continue;
        HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
        for(Int_t ln=0; ln<fCell.line; ln++) {
          Int_t shift=(fCell.yBinMin+ln) * nBinX;
          Int_t nbL=fCell.xBinMax[ln]+shift;
          for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
            hPlot[nb] |= add;
            if( nLayers>=minAm && HMdcBArray::get(plotBArSc,nb)==0 ) {
              Int_t nb2=nb<<1;  // =nb*2;
              if( HMdcBArray::getNSet(hPlMod1[nb2])>=minAmp[0] &&
                  HMdcBArray::getNSet(hPlMod2[nb2])>=minAmp[1] && 
                  HMdcBArray::getNSet(hPlMod3[nb2])>=minAmp[2] &&
                  HMdcBArray::getNSet(hPlMod4[nb2])>=minAmp[3]) {
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
                if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
              }
            }
          }
        }
      }
    }
  }
  maxBinBAr4Sc/=32;
  minBinBAr4Sc/=32;
}

void HMdcLookUpTbSec::makeSegPlot(Int_t seg, Int_t minAm) {
  Int_t m1=seg*2;
  Int_t m2=m1+1;
  Short_t* hPlot;
  UChar_t* hPlModF;
  UChar_t* hPlModS;
  if(seg==0) {
    hPlot=hitPlS1;
    hPlModF=hPlMod1;
    hPlModS=hPlMod2;
  } else {
    hPlot=hitPlS2;
    hPlModF=hPlMod3;
    hPlModS=hPlMod4;
  }
  if(!hPlot) return;
  maxBinBAr4Sc=-1;
  minBinBAr4Sc=sizeBAr4b*32;
  Int_t nLayers=0;
  for(Int_t mod=m1; mod<=m2; mod++) {
    HMdcLookUpTbMod& fMod=(*this)[mod];
    if( !(&fMod) ) continue;
    if(minAmp[mod]==0) continue;
    Int_t iLay=mod*6;
    Short_t addm=1<<((mod&1)*8);  // !!!
    for(Int_t layer=0; layer<6; layer++) {
      Int_t i=iLay+layer;
      if(nHits[i]==0) continue;
      nLayers++;   // layers with fired wires only
      Short_t add=addm<<layer;
      HMdcLookUpTbLayer& fLayer=fMod[layer];
      for(Int_t n=0; n<nHits[i]; n++) {
        if(hitsDel[i][n]<0) continue;
        HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
        for(Int_t ln=0; ln<fCell.line; ln++) {
          Int_t shift=(fCell.yBinMin+ln) * nBinX;
          Int_t nbL=fCell.xBinMax[ln]+shift;
          for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
            hPlot[nb] |= add;
            if( nLayers>=minAm && (layer+1)>=minAmp[mod] &&
                HMdcBArray::get(plotBArSc,nb)==0 ) {
              Int_t nb2=(nb<<1);
              if( HMdcBArray::getNSet(hPlModF[nb2])>=minAmp[m1] &&
                  HMdcBArray::getNSet(hPlModS[nb2])>=minAmp[m2] ) {
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
                if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
              }
            }
          }
        }
      }
    }
  }
  maxBinBAr4Sc/=32;
  minBinBAr4Sc/=32;
}

void HMdcLookUpTbSec::makeModPlot(Int_t mod,Int_t minAm, Short_t* hPlot) {
  HMdcLookUpTbMod& fMod=(*this)[mod];
  if( !(&fMod) ) return;
  UChar_t* hPlMod=(UChar_t*) hPlot;
  if(mod&1) hPlMod++;
  Int_t iLay=mod*6;
  Short_t addm=((mod&1)==0) ? 1:256;
  Int_t nLayers=0;
  maxBinBAr4Sc=-1;
  minBinBAr4Sc=sizeBAr4b*32;
  for(Int_t layer=0; layer<6; layer++) {
    Int_t i=iLay+layer;
    if(nHits[i]==0) continue;
    nLayers++;   // layers with fired wires only
    Short_t add=addm<<layer;
    HMdcLookUpTbLayer& fLayer=fMod[layer];
    for(Int_t n=0; n<nHits[i]; n++) {
      if(hitsDel[i][n]<0) continue;
      HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
      for(Int_t ln=0; ln<fCell.line; ln++) {
        Int_t shift=(fCell.yBinMin+ln) * nBinX;
        Int_t nbL=fCell.xBinMax[ln]+shift;
        for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
          hPlot[nb] |= add;
          if( nLayers>=minAm && HMdcBArray::get(plotBArSc,nb)==0 &&
              HMdcBArray::getNSet(hPlMod[nb<<1])>=minAm ) {
            HMdcBArray::set(plotBArSc,nb);
            if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
            if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
          }
        }
      }
    }
  }
  maxBinBAr4Sc/=32;
  minBinBAr4Sc/=32;
}

void HMdcLookUpTbSec::makeMod1Plot(Int_t minAm) {
  HMdcLookUpTbMod& fMod=(*this)[0];
  if( !(&fMod) ) return;
  Short_t addm=1;
  Int_t nLayers=0;
  maxBinBAr4M1=-1;
  minBinBAr4M1=sizeBAr4b*32;
  for(Int_t layer=0; layer<6; layer++) {
    Int_t i=layer;
    if(nHits[i]==0) continue;
    nLayers++;   // layers with fired wires only
    Short_t add=addm<<layer;
    HMdcLookUpTbLayer& fLayer=fMod[layer];
    for(Int_t n=0; n<nHits[i]; n++) {
      if(hitsDel[i][n]<0) continue;
      HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
      for(Int_t ln=0; ln<fCell.line; ln++) {
        Int_t shift=(fCell.yBinMin+ln) * nBinX;
        Int_t nbL=fCell.xBinMax[ln]+shift;
        for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
          hitPlS1[nb] |= add;
          if( nLayers>=minAm && HMdcBArray::get(plotBArM1,nb)==0 &&
              HMdcBArray::getNSet(hPlMod1[nb<<1])>=minAm ) {
            HMdcBArray::set(plotBArM1,nb);
            if(nb<minBinBAr4M1) minBinBAr4M1=nb;
            if(nb>maxBinBAr4M1) maxBinBAr4M1=nb;
          }
        }
      }
    }
  }
  maxBinBAr4M1/=32;
  minBinBAr4M1/=32;
}

void HMdcLookUpTbSec::makeMod2Plot(Int_t minAm) {
  HMdcLookUpTbMod& fMod=(*this)[1];
  if( !(&fMod) ) return;
  Short_t addm=256;
  Int_t nLayers=0;
  maxBinBAr4M2=-1;
  minBinBAr4M2=sizeBAr4b*32;
  for(Int_t layer=0; layer<6; layer++) {
    Int_t i=6+layer;
    if(nHits[i]==0) continue;
    nLayers++;   // layers with fired wires only
    Short_t add=addm<<layer;
    HMdcLookUpTbLayer& fLayer=fMod[layer];
    for(Int_t n=0; n<nHits[i]; n++) {
      if(hitsDel[i][n]<0) continue;
      HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
      for(Int_t ln=0; ln<fCell.line; ln++) {
        Int_t shift=(fCell.yBinMin+ln) * nBinX;
        Int_t nbL=fCell.xBinMax[ln]+shift;
        for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
          hitPlS1[nb] |= add;
          if( nLayers>=minAm && HMdcBArray::get(plotBArM2,nb)==0 &&
              HMdcBArray::getNSet(hPlMod2[nb<<1])>=minAm ) {
            HMdcBArray::set(plotBArM2,nb);
            if(nb<minBinBAr4M2) minBinBAr4M2=nb;
            if(nb>maxBinBAr4M2) maxBinBAr4M2=nb;
          }
        }
      }
    }
  }
  maxBinBAr4M2/=32;
  minBinBAr4M2/=32;
}

Int_t HMdcLookUpTbSec::findClusters(Int_t *imax){
  nClusters=0;
  if(nHitsTot==0) return 0;
  memset(hPlMod1,0,size*2);
  if(hitPlS2) memset(hPlMod3,0,size*2);
  //isCoilOff - !!!??? ---------------------------------------
  nMods=0;
  for(Int_t mod=0; mod<4; mod++) {
    minAmp[mod]=(mod<nSegments*2) ? imax[mod]:0;
    if(minAmp[mod]>0) nMods++;
  }
  if(typeClFinder==1) {
    // independing clust. finding
    for(Int_t m=0; m<2; m++) if(minAmp[m]>0) findClusInMod(m,minAmp[m],hitPlS1);
    for(Int_t m=2; m<4; m++) if(minAmp[m]>0) findClusInMod(m,minAmp[m],hitPlS2);
  } else {
    // combyned clust. finding
    nModSeg[0]=nModSeg[1]=0;
    for(Int_t m=0;m<4;m++) if(minAmp[m]>0) nModSeg[m/2] |= m%2+1;
    if(nModSeg[0]>0 && nModSeg[1]>0)
        findClusInSec(minAmp[0]+minAmp[1]+minAmp[2]+minAmp[3]);
    else {
      if(nModSeg[0]==3) findClusInSeg1();
        // findClusInSeg(0,minAmp[0]+minAmp[1],hitPlS1);
      else if(nModSeg[0]>0)
        findClusInMod(nModSeg[0]-1,minAmp[nModSeg[0]-1],hitPlS1);
      if(nModSeg[1]==3) findClusInSeg(1,minAmp[2]+minAmp[3],hitPlS2);
      else if(nModSeg[1]>0)
        findClusInMod(nModSeg[1]+1,minAmp[nModSeg[1]+1],hitPlS2);
    }
  }
  return nClusters;
}

void HMdcLookUpTbSec::getClusterSlot(Int_t seg, HMdcList12GroupCells& list) {
  locClus[1]=seg;
  locClus[2]=counter[seg];
  fClus = (HMdcClus*)fClusCat->getSlot(locClus);
  if(!fClus) {
    Warning("getClusterSlot","Sec.%i Segment %i No slot HMdcClus available",
        sector+1,seg+1);
    return;
  }
  if(isGeant) fClus=(HMdcClus*)(new(fClus) HMdcClusSim(list));
  else fClus=new(fClus) HMdcClus(list);
  fClus->setAddress(locClus);
  counter[seg]++;
}

void HMdcLookUpTbSec::fillWiresList(Int_t mod,  HMdcCluster& cls, 
    HMdcList12GroupCells& list) {
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
      if( cls.nLMin > yBinMax ) continue;
      Short_t yBinMin=fCell.yBinMin;
//        if( cls.nLMax < yBinMin ) break;            Hits not sorted !
      Short_t lMax=(yBinMax < cls.nLMax) ? yBinMax : cls.nLMax;
      Short_t lMin=(yBinMin > cls.nLMin) ? yBinMin : cls.nLMin;
      for (Short_t nl=lMin; nl<=lMax; nl++) {
        if(fCell.xBinMin[nl-yBinMin] < cls.xMinCl[nl]) {
          if(cls.xMinCl[nl] > fCell.xBinMax[nl-yBinMin]) continue;
          //???? Chto delat' s udalenniymi vremenami ????
          list.setTime(clLay,cell,hitsNTime[iLayer][i]);
	  break;
        } else {
          if(cls.xMaxCl[nl] < fCell.xBinMin[nl-yBinMin]) continue;
          //???? Chto delat' s udalenniymi vremenami ????
          list.setTime(clLay,cell,hitsNTime[iLayer][i]);
	  break;
        }
      }
    }
  }
}

void HMdcLookUpTbSec::findClusInSeg(Int_t seg, Short_t minAm, Short_t *hPlot){
  makeSegPlot(seg,minAm);
  if(minAm<3) return;
  UChar_t *hPlModF=(UChar_t*) hPlot;
  Int_t maxClS=minAm-1;
  nClsArr=0;
  Int_t nmo=(seg+1)*2;
  //---Cluster finding-------------------------------------
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBArSc4b[n4]==0) continue;
    UChar_t *b1=plotBArSc+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      stack->init();
      HMdcCluster& cls=clusArr[nClsArr];
      cls.clear();
      stack->push(nBin4+nLBin);
      Int_t nBinSt;
      while ((nBinSt=stack->pop()) >= 0) {
        Int_t nx=nBinSt%nBinX;
        Int_t ny=nBinSt/nBinX;
        Double_t wt=(Double_t)(HMdcBArray::getNSet2B(hPlModF+2*nBinSt)-maxClS);
        cls.addBin(xBinsPos[nx],yBinsPos[ny],wt,nx,ny);
        // Testing nearby bins and adding bins to stack
        for(Int_t nTs=0; nTs<8; nTs++) {
          Int_t ncTs=nBinSt+nearbyBins[nTs];          
          if(HMdcBArray::testAndUnset(plotBArSc,ncTs)) stack->push(ncTs);
        }
        stack->checkSize(); // If stack overflowed one will increased on 50%:
      }
      //-Filling of cluster---------------------------------------------------
      cls.calcXY();
      for(Int_t mod=seg*2; mod<nmo; mod++) fillWiresList(mod,cls,cls.lCells1);
      nClsArr++;
      if(nClsArr >= 500) break;
    }
    if(nClsArr >= 500) {
      Warning("findClusInSeg"," Num. of clusters in sector %i > 500\n",sector);
      memset(plotBArSc,0,sizeBAr);
      break;
    }
  }
      
  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInSeg(seg,nClsArr,clusArr);
    fillClusCat(-2,seg);
  }
}

void HMdcLookUpTbSec::findClusInSeg1(void){
  makeMod1Plot(minAmp[0]);
  makeMod2Plot(minAmp[1]);
  Short_t minAm=minAmp[0]+minAmp[1];
  if(minAm<3) return;

  minBinBAr4Sc=(minBinBAr4M1>=minBinBAr4M2) ? minBinBAr4M1:minBinBAr4M2;
  maxBinBAr4Sc=(maxBinBAr4M1<=maxBinBAr4M2) ? maxBinBAr4M1:maxBinBAr4M2;
  for(Int_t n4=minBinBAr4Sc; n4<=maxBinBAr4Sc; n4++)
      plotBArSc4b[n4]=plotBAr4bM1[n4] & plotBAr4bM2[n4];
  
  scanPlotInMod(0,hPlMod1,plotBArM1,clusArrM1,nClsArrM1,
      minBinBAr4M1,maxBinBAr4M1); 
  if(nClsArrM1>1) mergeClusInMod(0,nClsArrM1,clusArrM1);
  
  scanPlotInMod(1,hPlMod2,plotBArM2,clusArrM2,nClsArrM2,
      minBinBAr4M2,maxBinBAr4M2);
  if(nClsArrM2>1) mergeClusInMod(1,nClsArrM2,clusArrM2);

  scanPlotInSeg(0,hPlMod1,plotBArSc,clusArr,nClsArr);

  for(Int_t nCl=0; nCl<nClsArr; nCl++) {
    HMdcCluster& cls=clusArr[nCl];
    if(!cls.status) continue;
    for(Int_t nClM1=0; nClM1<nClsArrM1; nClM1++) {
      if(!cls.hasOverlap(clusArrM1[nClM1])) continue;
      cls.clusMod1=&(clusArrM1[nClM1]);
      clusArrM1[nClM1].numSegCl++;
    }
    for(Int_t nClM2=0; nClM2<nClsArrM2; nClM2++) {
      if(!cls.hasOverlap(clusArrM2[nClM2])) continue;
      cls.clusMod2=&(clusArrM2[nClM2]);
      clusArrM2[nClM2].numSegCl++;
    }
  }

  for(Int_t nCl=0; nCl<nClsArrM1; nCl++)
    if(clusArrM1[nCl].status) clusArrM1[nCl].calcClusParam(xHStep2,yHStep2);
  for(Int_t nCl=0; nCl<nClsArrM2; nCl++)
    if(clusArrM2[nCl].status) clusArrM2[nCl].calcClusParam(xHStep2,yHStep2);
  
  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInSeg(0,nClsArr,clusArr);
    fillClusCat(-2,0);
  }
}

void HMdcLookUpTbSec::scanPlotInMod(Int_t mod, UChar_t* hPlMod,
    UChar_t* plotBAr, HMdcCluster* clArr, Int_t& nClArr, Int_t minBin, Int_t maxBin) {
  Int_t maxClS=minAmp[mod]-1;
  nClArr=0;
  Int_t* plotBAr4b=(Int_t*) plotBAr;
  for(Int_t n4=maxBin; n4>=minBin; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=plotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      stack->init();
      HMdcCluster& cls=clArr[nClArr];
      cls.clear();
      stack->push(nBin4+nLBin);
      Int_t nBinSt;
      while ((nBinSt=stack->pop()) >= 0) {
        Int_t nx=nBinSt%nBinX;
        Int_t ny=nBinSt/nBinX;
        Double_t wt=(Double_t)(HMdcBArray::getNSet(hPlMod[2*nBinSt])-maxClS);
        cls.addBin(xBinsPos[nx],yBinsPos[ny],wt,nx,ny);
        // Testing nearby bins:
        for(Int_t nTs=0; nTs<8; nTs++) {
          Int_t ncTs=nBinSt+nearbyBins[nTs];          
          if(HMdcBArray::testAndUnset(plotBAr,ncTs)) stack->push(ncTs);
        }
        stack->checkSize(); // If stack overflowed one will increased on 50%:
      }
      //-Filling of cluster---------------------------------------------------
      cls.calcXY();
      fillWiresList(mod,cls,cls.lCells1);
      nClArr++;
      if(nClArr >= 500) break;
    }
    if(nClArr >= 500) {
      Warning("scanClusInMod"," Num. of clusters in sector %i > 500\n",sector);
      memset(plotBAr,0,sizeBAr);
      break;
    }
  }
}


void HMdcLookUpTbSec::scanPlotInSeg(Int_t seg, UChar_t* hPlSeg,
    UChar_t* plotBAr, HMdcCluster* clArr, Int_t& nClArr) {
  Int_t m1=seg*2;
  Int_t m2=m1+1;
  Int_t maxClS=minAmp[m1]+minAmp[m2]-1;
  nClArr=0;
  Int_t* plotBAr4b=(Int_t*) plotBAr;
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=plotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      stack->init();
      HMdcCluster& cls=clArr[nClArr];
      cls.clear();
      stack->push(nBin4+nLBin);
      Int_t nBinSt;
      while ((nBinSt=stack->pop()) >= 0) {
        Int_t nx=nBinSt%nBinX;
        Int_t ny=nBinSt/nBinX;
        Double_t wt=(Double_t)(HMdcBArray::getNSet2B(hPlSeg+2*nBinSt)-maxClS);
        cls.addBin(xBinsPos[nx],yBinsPos[ny],wt,nx,ny);
        // Testing nearby bins:
        for(Int_t nTs=0; nTs<8; nTs++) {
          Int_t ncTs=nBinSt+nearbyBins[nTs];          
          if(HMdcBArray::testAndUnset(plotBAr,ncTs)) stack->push(ncTs);
        }
        stack->checkSize(); // If stack overflowed one will increased on 50%:
      }
      //-Filling of cluster---------------------------------------------------
      cls.calcXY();
      for(Int_t mod=m1;mod<=m2;mod++) fillWiresList(mod,cls,cls.lCells1);
      nClArr++;
      if(nClArr >= 500) break;
    }
    if(nClArr >= 500) {
      Warning("scanClusInSeg"," Num. of clusters in sector %i > 500\n",sector);
      memset(plotBAr,0,sizeBAr);
      break;
    }
  }
}

void HMdcLookUpTbSec::findClusInSec(Short_t minAm){
  makeSecPlot(minAm);
  if(minAm<3) return;
  Int_t maxClS=minAm-1;
  nClsArr=0;
  //---Cluster finding-------------------------------------
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBArSc4b[n4]==0) continue;
    UChar_t *b1=plotBArSc+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0) {
      //---New cluster---------------------------------------
      stack->init();
      HMdcCluster& cls=clusArr[nClsArr];
      cls.clear();
      
      stack->push(nBin4+nLBin);
      Int_t nBinSt;
      while ((nBinSt=stack->pop()) >= 0) {
        Int_t nx=nBinSt%nBinX;
        Int_t ny=nBinSt/nBinX;
        Int_t nBinSt1=2*nBinSt;
        Double_t wt=(Double_t)(HMdcBArray::getNSet2B(hPlMod1+nBinSt1)+
                               HMdcBArray::getNSet2B(hPlMod3+nBinSt1)-maxClS);
        cls.addBin(xBinsPos[nx],yBinsPos[ny],wt,nx,ny);
        // Testing nearby bins and puting bins to stack
        for(Int_t nTs=0; nTs<8; nTs++) {
          Int_t ncTs=nBinSt+nearbyBins[nTs];
          if(HMdcBArray::testAndUnset(plotBArSc,ncTs)) stack->push(ncTs);
        }
        stack->checkSize(); // Stack overflowed
      }
      //-End cluster------------------------------------------------------
      cls.calcXY();
      for(Int_t mod=0;mod<4;mod++) {
        if(minAmp[mod]==0) continue;
        if(mod<2) fillWiresList(mod,cls,cls.lCells1);
        else  fillWiresList(mod,cls,cls.lCells2);
      }
      nClsArr++;
      if(nClsArr >= 500) break;
    }
    if(nClsArr >= 500) {
      Warning("findClusInSec"," Num. of clusters in sector %i > 500\n",sector);
      memset(plotBArSc,0,sizeBAr);
      break;
    }
  }
  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInSec(nClsArr, clusArr);
    fillClusCat(-nMods,-1);
  }
}

void HMdcLookUpTbSec::mergeClusInMod(Int_t mod, Int_t nClArr, 
    HMdcCluster* clArr) {
  Int_t nClus=nClArr;
  Int_t fLay=(mod==0 || mod==2) ? 0:6;
  Int_t lLay=fLay+5;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClArr-1; cl1++) {
      HMdcCluster& cls1=clArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=(mod<2) ? cls1.lCells1:cls1.lCells2;
      for(Int_t cl2=cl1+1; cl2<nClArr; cl2++) {
        HMdcCluster& cls2=clArr[cl2];
        if(!cls2.status) continue;
        HMdcList12GroupCells& listCells2=(mod<2) ? cls2.lCells1:cls2.lCells2;
        Float_t dY=cls1.y-cls2.y;
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 10.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 40.) continue;        // 30. mm !???
        if(listCells1.compare(&listCells2,fLay,lLay)<4) continue;
        cls1.addClus(cls2,kFALSE);
        nomerg=kFALSE;
        nClus--;
      }
      while(!clArr[nClArr-1].status) nClArr--;
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::mergeClusInSeg(Int_t seg, Int_t nClArr, 
    HMdcCluster* clArr) {
  Int_t nClus=nClArr;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClArr-1; cl1++) {
      HMdcCluster& cls1=clArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=(seg==0) ? cls1.lCells1:cls1.lCells2;
      for(Int_t cl2=cl1+1; cl2<nClArr; cl2++) {
        HMdcCluster& cls2=clArr[cl2];
        if(!cls2.status) continue;
        
        if(cls1.clusMod1 != cls2.clusMod1) continue;
        if(cls1.clusMod2 != cls2.clusMod2) continue;
            
        HMdcList12GroupCells& listCells2=(seg==0) ? cls2.lCells1:cls2.lCells2;
        Float_t dY=cls1.y-cls2.y;
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 10.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 40.) continue;        // 30. mm !???
        if(listCells1.compare(&listCells2,0, 5)<4) continue;
        if(listCells1.compare(&listCells2,6,11)<4) continue;
        cls1.addClus(cls2,kFALSE);
        nomerg=kFALSE;
        nClus--;
      }
      while(!clArr[nClArr-1].status) nClArr--;
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::mergeClusInSec(Int_t nClArr, HMdcCluster* clArr) {
  Int_t nClus=nClArr;
  Int_t m1=minAmp[0];
  Int_t m2=minAmp[1];
  Int_t m3=minAmp[2];
  Int_t m4=minAmp[3];
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClArr-1; cl1++) {
      HMdcCluster& cls1=clArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      HMdcList12GroupCells& listCells2=cls1.lCells2;
      for(Int_t cl2=cl1+1; cl2<nClArr; cl2++) {
        HMdcCluster& cls2=clArr[cl2];
        if(!cls2.status) continue;
        HMdcList12GroupCells* listCells1s=&(cls2.lCells1);
        HMdcList12GroupCells* listCells2s=&(cls2.lCells2);
        Float_t dY=cls1.y-cls2.y;
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 10.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 40.) continue;        // 30. mm !???
        if(m1>0 && listCells1.compare(listCells1s,0, 5)<4) continue;
        if(m2>0 && listCells1.compare(listCells1s,6,11)<4) continue;
        if(m3>0 && listCells2.compare(listCells2s,0, 5)<4) continue;
        if(m4>0 && listCells2.compare(listCells2s,6,11)<4) continue;
        cls1.addClus(cls2,kTRUE);
        nomerg=kFALSE;
        nClus--;
      }
      while(!clArr[nClArr-1].status) nClArr--;
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::fillClusCat(Int_t mod, Int_t segp) {
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
  for(Int_t cl=0; cl<nClsArr; cl++) {
    HMdcCluster& cls=clusArr[cl];
    if(!cls.status) continue;
    getClusterSlot(seg,cls.lCells1);
    if(!fClus) return;
    cls.calcClusParam(xHStep2,yHStep2);
    nClusters++;
    fClus->setMod(mod);
    fClus->setTypeClFinder(typeClFinder);
    fClus->setMinCl(m1,m2);
    fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
    fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],target[2],eTarg[2]);
    cls.fillClus(fClus,0);
    if(fClus->isGeant()) ((HMdcClusSim*)fClus)->calcTrList();
    if(segp<0) {
      HMdcClus* fClus0=fClus;
      getClusterSlot(1,cls.lCells2);
      if(!fClus) return;
      fClus->setMod(mod);
      fClus->setTypeClFinder(typeClFinder);
      fClus->setMinCl(m3,m4);
      fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
      fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],target[2],eTarg[2]);
      cls.fillClus(fClus,1);
      fClus->setIndexParent(fClus0->getIndex());
      Int_t ind=fClus->getIndex();
      fClus0->setIndexChilds(ind,ind);
      if(fClus->isGeant()) ((HMdcClusSim*)fClus)->getNTracks();
    }
  }
}

void HMdcLookUpTbSec::findClusInMod(Int_t mod, Short_t minAm, Short_t *hPlot){
  makeModPlot(mod,minAm,hPlot);
  if(minAm<4) return;
  UChar_t *hPlMod=(UChar_t*) hPlot;
  if(mod==1 || mod==3) hPlMod++;
  scanPlotInMod(mod,hPlMod,plotBArSc,clusArr,nClsArr,minBinBAr4Sc,maxBinBAr4Sc);
  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInMod(mod,nClsArr,clusArr);
    fillClusCat(mod,mod/2);
  }
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
  setPar(319, kFALSE);
  fMdcClusCat = HMdcGetContainers::getCatMdcClus(kTRUE);
  targLenInc[0]=targLenInc[1]=0.;
}

void HMdcLookUpTb::setPar(Int_t inBinX, Bool_t isCOff) {
  nBinX=(inBinX%2 == 0) ? inBinX:inBinX+1;
  if(nBinX<100) nBinX=320;
  nBinY=(nBinX*278)/100;
  if(nBinY%2 == 0) nBinY++;
  isCoilOff=isCOff;
}

Bool_t HMdcLookUpTb::initContainer() {
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
        Int_t nSegs=(fMdcDet->getModule(sec,0)||fMdcDet->getModule(sec,1)) ?1:0;
        if(isCoilOff)
          nSegs=(fMdcDet->getModule(sec,2) || fMdcDet->getModule(sec,3)) ?2:1;
        else if(nSegs==0) continue;
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

HMdcLookUpTb* HMdcLookUpTb::getExObject() {
  return fMdcLookUpTb;
}

HMdcLookUpTb* HMdcLookUpTb::getObject() {
  if(!fMdcLookUpTb) fMdcLookUpTb=new HMdcLookUpTb();
  return fMdcLookUpTb;
}

void HMdcLookUpTb::deleteCont() {
  if(fMdcLookUpTb) {
    delete fMdcLookUpTb;
    fMdcLookUpTb=0;
  }
}

HMdcLookUpTb::~HMdcLookUpTb() {
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

void HMdcLookUpTb::clear() {
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
    Int_t binig){
  if(!hist) {
    plBining=(binig) ? 2:1;
    hist=new TH2C(name,title,nBinX/plBining,xLow,xUp,nBinY/plBining,yLow,yUp);
  }
  else {
    hist->Reset();
    hist->SetName(name);
    hist->SetTitle(title);
  }
  hist->SetMaximum(6.);
  hist->SetMinimum(0.);
  hist->Fill(0.,0.,0);
  if(nHitsTot==0) return hist;
  Short_t binS1=0;
  Short_t binS2=0;
  UChar_t* bin1S1=(UChar_t*)&binS1;
  UChar_t* bin1S2=(UChar_t*)&binS2;
  for (Int_t nx=0; nx<nBinX; nx+=plBining) {
    for (Int_t ny=0; ny<nBinY; ny+=plBining) {
      Int_t nBin=ny*nBinX+nx;
      binS1=hitPlS1[nBin];
      if(plBining==2)  binS1 |=
        hitPlS1[nBin+1] | hitPlS1[nBin+nBinX] | hitPlS1[nBin+nBinX+1];
      if( hitPlS2 ) {
        binS2=hitPlS2[nBin];
        if(plBining==2) binS2 |=
          hitPlS2[nBin+1] | hitPlS2[nBin+nBinX] | hitPlS2[nBin+nBinX+1];
      }
      if( binS1==0 && binS2==0 ) continue;
      Int_t color=0;
      if(type==1) {
        color=HMdcBArray::getNSet(bin1S1[1]);
        if(color) color+=6;
        color+=HMdcBArray::getNSet(bin1S1[0]);
        if(hitPlS2) {
          Int_t cl=HMdcBArray::getNSet(bin1S2[1]);
          if(cl) cl+=cl+6;
          cl+=HMdcBArray::getNSet(bin1S2[0]);
          if(cl) color+=12;
        }
      } else if(type==2) {
        if(bin1S1[1]) color=2;
        else if(bin1S1[0]) color=1;
        if(hitPlS2) {
          if(bin1S2[1]) color=4;
          else if(bin1S2[0]) color=3;
        }
      } else {
        color=HMdcBArray::getNSet2B(bin1S1); // tablBiN[binS1];
        if(hitPlS2) color+=HMdcBArray::getNSet2B(bin1S2); //tablBiN[binS2];
      }
      hist->Fill((Float_t)nx*xStep+xLowHSt,
                 (Float_t)ny*yStep+yLowHSt, color);
    }
  }
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
        if(yBinMinT<1 || yBinMinT>fsec.nBinY-2) {
          Warning("calcLookUpTb","S%iM%iL%iC%i yBinMinT=%i isn't in 1 - %i !",
              sec+1,mod+1,layer+1,cell+1,yBinMinT,fsec.nBinY-2);
          yBinMinT=(yBinMinT<1) ? 1:fsec.nBinY-2;
        }
        if(yBinMaxT<1 || yBinMaxT>fsec.nBinY-1) {
          Warning("calcLookUpTb","S%iM%iL%iC%i  yBinMaxT=%i isn't in 1 - %i !",
              sec+1,mod+1,layer+1,cell+1,yBinMaxT,fsec.nBinY-2);
          yBinMaxT=(yBinMaxT<1) ? 1:fsec.nBinY-2;
        }
        fLUpTbCell.init(yBinMinT,yBinMaxT);
        Int_t nx1t,nx2t;
        nx1t=+1000000000;
        nx2t=-1000000000;
        for(Int_t ny=yBinMinT; ny<=yBinMaxT; ny++) {
          Double_t y=(ny+1)*fsec.yStep+fsec.yLow;
          Double_t x1,x2;
          calcX(cellProj, y, x1, x2);	
          Int_t nx1i=(Int_t)((x1-fsec.xLow)/fsec.xStep);
	  Int_t nx2i=(Int_t)((x2-fsec.xLow)/fsec.xStep);
	  if(nx1i < nx1t) nx1t=nx1i;
	  if(nx2i > nx2t) nx2t=nx2i;
          if(nx1i<1 || nx1i>fsec.nBinX-2) {
            Warning("calcLookUpTb","S%iM%iL%iC%i  nx1i=%i isn't in 1 - %i !",
              sec+1,mod+1,layer+1,cell+1,nx1i,fsec.nBinX-2);
            nx1i=(nx1i<1) ? 1:fsec.nBinX-2;
          }
          if(nx2i<1 || nx2i>fsec.nBinX-2) {
            Warning("calcLookUpTb","S%iM%iL%iC%i  nx2i=%i isn't in 1 - %i !",
              sec+1,mod+1,layer+1,cell+1,nx2i,fsec.nBinX-2);
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
  //Sectors:
  //  Calc. of projection plane:
  //  It's the plane between MDC0 and MDC1
  //  par=0.0 -> plane=layer 6, MDC1
  //  par=1.0 -> plane=layer 1, MDC2

  HGeomTransform coordSys;
  Double_t par=-100.;
  //  Double_t par=1.0;   // <<<===================== use MDC1
  Int_t nmod=0;
  Int_t firstMod=-1;
  Int_t lastMod=0;
  Int_t nMaxMods=(isCoilOff) ? 4:2;
  for(Int_t m=0; m<nMaxMods; m++) {
    if( fMdcDet->getModule(sec,m) ) {
      nmod++;
      if(firstMod<0) firstMod=m;
      lastMod=m;
    }
  }
  if(nmod==0) return kFALSE;
  Int_t iPar=-1;
  if(nmod==1) iPar=firstMod;
  else if(firstMod==0) par=0.575;   // determ. of par !!!
  else iPar=firstMod;                 // for beem time nov2000 with mdc 3
  Double_t transNew[3];
  if(iPar<0) {
    Double_t tanTheta1 = 0.0;
    Double_t tanTheta2=0.0;
    Double_t zMdc1=0.0;
    Double_t zMdc2=0.0;
    Double_t tmp;
    coordSys=*((*fSizesCells)[sec][firstMod][5].getSecTrans());
    tmp=(coordSys.getRotMatrix())(8);
    tmp=1./(tmp*tmp);
    tanTheta1=sqrt(tmp-1.);
    zMdc1=(coordSys.getTransVector()).getZ()*tmp;

    coordSys=*((*fSizesCells)[sec][lastMod][0].getSecTrans());
    tmp=(coordSys.getRotMatrix())(8);  tmp=1./(tmp*tmp);
    tanTheta2=sqrt(tmp-1.);
    zMdc2=(coordSys.getTransVector()).getZ()*tmp;

    if(zMdc1==0.0 && zMdc2==0.0) {
      Error("calcMdc12Proj","No MDC volume found.");
      return kFALSE;
    }
    printf(
    "\n===> Sec.%i: Using plane between MDC%i and MDC%i (p=%f) as projection plane\n",
        sec+1,firstMod+1,lastMod+1,par);

    Double_t tanTheta=tanTheta1*(1.-par)+tanTheta2*par;
    Double_t cosTheta=1/sqrt(tanTheta*tanTheta+1.);
    Double_t sinTheta=tanTheta*cosTheta;
    Double_t newRot[9];
    newRot[0]=1;
    newRot[1]=newRot[2]=newRot[3]=newRot[6]=0;
    newRot[4]=newRot[8]=cosTheta;                           // cos(Theta)
    newRot[5]=sinTheta;                                     // sin(Theta)
    newRot[7]=-sinTheta;                                    // -sin(Theta)
    HGeomRotation rotMatProj(newRot);
    transNew[0]=0;
    transNew[2]=(zMdc1*(1.-par)+zMdc2*par)*cosTheta*cosTheta; // mm
    transNew[1]=transNew[2]*tanTheta;

    coordSys.setRotMatrix(newRot);
    coordSys.setTransVector(transNew);
  }
  else {
    coordSys=*((*fSizesCells)[sec][iPar].getSecTrans());
    printf("\n===> Sec.%i: Using middle plane of MDC%i as projection plane\n",
                              sec+1,iPar+1);
  }
  ((*this)[sec]).prPlane.setPrPlane(coordSys);
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcTarget(Int_t sec){
  //Geting target parameters
  Int_t nT=fSpecGeomPar->getNumTargets()-1;
  if( nT < 0 ) {
    Error("calcTarget","Number of targets = %i!",nT+1);
    return kFALSE;
  }
  HMdcLookUpTbSec& fsec=(*this)[sec];
  fsec.targVc[0]=(fSpecGeomPar->getTarget( 0)->getTransform()).getTransVector();
  fsec.targVc[0].setZ( fsec.targVc[0].getZ() +
      fSpecGeomPar->getTarget( 0)->getPoint(0)->getZ() - targLenInc[0]);
  fsec.targVc[1]=(fSpecGeomPar->getTarget(nT)->getTransform()).getTransVector();
  fsec.targVc[1].setZ( fsec.targVc[1].getZ() +
      fSpecGeomPar->getTarget(nT)->getPoint(2)->getZ() + targLenInc[1]);
  const HGeomTransform* trans=(*fSizesCells)[sec].getLabTrans();
  if(&trans == 0) return kFALSE;
  fsec.targVc[0]=trans->transTo(fsec.targVc[0]);
  fsec.targVc[1]=trans->transTo(fsec.targVc[1]);
  HGeomVector tar(fsec.targVc[0]+fsec.targVc[1]);
  tar*=0.5;
  HGeomVector errTar(fsec.targVc[0]-fsec.targVc[1]);
  errTar*=0.5;
  errTar=errTar.abs();
  for(Int_t i=0;i<3;i++) {
    fsec.target[i]=tar(i);
    fsec.eTarg[i]=errTar(i);
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
  fsec.xHStep=fsec.xStep/2;
  fsec.yHStep=fsec.yStep/2;
  fsec.xHStep2=fsec.xHStep*fsec.xHStep;
  fsec.yHStep2=fsec.yHStep*fsec.yHStep;
  fsec.xLowHSt=fsec.xLow+fsec.xHStep;
  fsec.yLowHSt=fsec.yLow+fsec.yHStep;
  for(Int_t n=0; n<fsec.nBinX; n++) {
    fsec.xBinsPos[n]=((Double_t)n)*fsec.xStep+fsec.xLowHSt;
  }
  for(Int_t n=0; n<fsec.nBinY; n++) {
    fsec.yBinsPos[n]=((Double_t)n)*fsec.yStep+fsec.yLowHSt;
  }
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
