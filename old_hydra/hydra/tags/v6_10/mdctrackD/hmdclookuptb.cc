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

//----------Sector------------------------------------

Int_t* HMdcLookUpTbSec::stack=0;
Int_t  HMdcLookUpTbSec::stackSize;
HMdcList12GroupCells HMdcLookUpTbSec::cLCells1[500];
HMdcList12GroupCells HMdcLookUpTbSec::cLCells2[500];
Bool_t HMdcLookUpTbSec::cStatus[500];
Short_t HMdcLookUpTbSec::cNMergedClus[500];
Double_t HMdcLookUpTbSec::cSumWt[500];
Int_t HMdcLookUpTbSec::cNBins[500];
Double_t HMdcLookUpTbSec::cX[500];
Double_t HMdcLookUpTbSec::cY[500];
Double_t HMdcLookUpTbSec::cMeanX[500];
Double_t HMdcLookUpTbSec::cMeanY[500];
Double_t HMdcLookUpTbSec::cMeanXX[500];
Double_t HMdcLookUpTbSec::cMeanYY[500];
Double_t HMdcLookUpTbSec::cMeanYX[500];
Double_t HMdcLookUpTbSec::cMeanXWt[500];
Double_t HMdcLookUpTbSec::cMeanYWt[500];
Double_t HMdcLookUpTbSec::cMeanXXWt[500];
Double_t HMdcLookUpTbSec::cMeanYYWt[500];

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
  size=nBinX*nBinY;
  size4=size/2;
  if((size%2) > 0) size4++;
  hitPlS1=new Short_t [size4*2];
  hitPl4S1=(Int_t*) hitPlS1;
  hist=0;
  if(nSegs==2) {
    hitPlS2=new Short_t [size4*2];
    hitPl4S2=(Int_t*) hitPlS2;
  } else {
    hitPlS2=0;
    hitPl4S2=0;
  }
  typeClFinder=0;
//=========================beamtime !!!!!!!!!!===========
//  bMboard=new Char_t [size];
//  for(Int_t i=0; i<size; i++) bMboard[i]=0;
//  indFill=0;
//=======================================================
  setLUpTb=kFALSE;
  Short_t one=1;
  for(Int_t i=0; i<4096; i++) {
    tablBiN[i]=0;
    Int_t ind4p4[2];
    ind4p4[0]=ind4p4[1]=0;
    for(Int_t j=0; j<12; j++) {
      if(((one<<j)&i) > 0) {
        tablBiN[i]++;
        ind4p4[j/6]++;
      }
    }
  }
  nearbyBins[0]=-1;
  nearbyBins[1]=+1;
  nearbyBins[2]=-nBinX;
  nearbyBins[3]=+nBinX;
  nearbyBins[4]=-1-nBinX;
  nearbyBins[5]=-1+nBinX;
  nearbyBins[6]=1-nBinX;
  nearbyBins[7]=1+nBinX;
  if(stack==0) {
    stackSize=(Int_t)(4950.*(Double_t(nBinX)/315.)*(Double_t(nBinX)/315.));
    stack=new Int_t [stackSize+50];
  }
  isGeant = HMdcGetContainers::isGeant();
  locClus.set(3,sec,0,0);
  radToDeg=180./acos(Double_t(-1.));
}

HMdcLookUpTbSec::~HMdcLookUpTbSec() {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  delete [] hitPlS1;
  if(stack) delete [] stack;
  stack=0;
  if(hitPlS2) delete [] hitPlS2;
  hitPlS2=0;
  if(hist && hist->IsOnHeap()) delete hist;
  hist=0;
  //====================
  //  delete [] bMboard;
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
 UChar_t nc=(UChar_t) nCell;
 const UChar_t one=1;
 for(Int_t l=0; l<fCell.line; l++) {
   for(Int_t nb=fCell.xBinMin[l]; nb<=fCell.xBinMax[l]; nb++) {
     UChar_t bit=(one<<(nc-fLUpTbLayer.nfcell[nb]));
     if(fLUpTbLayer.ncellbin[nb]&bit != 0) {
       fLUpTbLayer.ncellbin[nb]=fLUpTbLayer.ncellbin[nb]&(~bit);
       if(fLUpTbLayer.ncellbin[nb] == 0) hitPlS1[nb]--;
     }
   }
 }
*/
}

void HMdcLookUpTbSec::doPlot(void) {
  Short_t cell,iByte;
  const Short_t one=1;
//============================================
/*  if(indFill==0) {
    Int_t mmm[17][3]={
    {0,0,4}, {0,56,67}, {0,76,83}, {0,92,95},
    {1,0,4}, {1,72,72}, {2,0,4}, {2,32,35}, {2,48,55}, {2,64,71}, {2,76,103},
    {4,0,4}, {4,48,59}, {4,68,75}, {4,84,120}, {5,129,129}, {5,159,159}};
    for(Int_t i=0; i<17; i++) {
      iByte=one<<mmm[i][0];
      HMdcLookUpTbLayer& fLayer=(*this)[1][mmm[i][0]];  //mod 1; layer ...
      for(Int_t cell=mmm[i][1]; cell<=mmm[i][2]; cell++) {
        HMdcLookUpTbCell& fcell=fLayer[cell];
        if(fcell.line>0) {
          for(Int_t ln=0; ln<fCell.line; ln++) {
            for(Int_t nb=fCell.xBinMin[ln]; nb<=fCell.xBinMax[ln]; nb++) {
            bMboard[nb]=bMboard[nb] | iByte;
          }
        }
      }
    }
   }
   indFill=1;
  } */
//============================================
  for(Int_t i=0; i<size4; i++) hitPl4S1[i]=0;
  if(hitPlS2) for(Int_t i=0; i<size4; i++) hitPl4S2[i]=0;
  if(nHitsTot==0) return;
  for(Int_t mod=0; mod<2; mod++) {
    HMdcLookUpTbMod& fMod=(*this)[mod];
    if( !(&fMod) ) continue;
    Int_t nlay=mod*6;
    for(Int_t layer=0; layer<6; layer++) {
      Int_t i=nlay+layer;
      iByte=one<<i;
      HMdcLookUpTbLayer& fLayer=fMod[layer];
      for(Int_t n=0; n<nHits[i]; n++) {
        if(hitsDel[i][n]<0) continue;
        HMdcLookUpTbCell& fCell=fLayer[hits[i][n]];
        for(Int_t ln=0; ln<fCell.line; ln++) {
          Int_t shift=(fCell.yBinMin+ln) * nBinX;
          Int_t nbL=fCell.xBinMax[ln]+shift;
          for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
            hitPlS1[nb] |= iByte;
          }
        }
      }
    }
  }
  // === Segment 2 (mdc3, mdc4) ==========================
  if(!hitPlS2) return;
  for(Int_t i=12; i<24; i++) {
    if(nHits[i]<=0) continue;
    iByte=one<<(i-12);
    Int_t mod=i/6;
    Int_t layer=i%6;
    HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
    for(Int_t n=0; n<nHits[i]; n++) {
      if(hitsDel[i][n]<0) continue;
      cell=hits[i][n];
      HMdcLookUpTbCell& fCell=fLayer[cell];
      for(Int_t ln=0; ln<fCell.line; ln++) {
        Int_t shift=(fCell.yBinMin+ln) * nBinX;
        Int_t nbL=fCell.xBinMax[ln]+shift;
        for(Int_t nb=fCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
          hitPlS2[nb] |= iByte;
        }
      }
    }
  }
}

Int_t HMdcLookUpTbSec::findClusters(Int_t *imax){
  nClusters=0;
  doPlot();
  //isCoilOff - !!!??? ---------------------------------------
  if(nHitsTot==0) return 0;
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
      if(nModSeg[0]==3) findClusInSeg(0,minAmp[0]+minAmp[1],hitPlS1);
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
      if( nLmin > yBinMax ) continue;
      Short_t yBinMin=fCell.yBinMin;
//        if( nLmax < yBinMin ) break;            Hits not sorted !
      Short_t lMax=(yBinMax < nLmax) ? yBinMax : nLmax;
      Short_t lMin=(yBinMin > nLmin) ? yBinMin : nLmin;
      for (Short_t nl=lMin; nl<=lMax; nl++) {
        Short_t nLind=nLmax-nl;
        if(fCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
          if(xMinCl[nLind] > fCell.xBinMax[nl-yBinMin]) continue;
          //???? Chto delat' s udalenniymi vremenami ????
          list.setTime(clLay,cell,hitsNTime[iLayer][i]);
	  break;
        } else {
          if(xMaxCl[nLind] < fCell.xBinMin[nl-yBinMin]) continue;
          //???? Chto delat' s udalenniymi vremenami ????
          list.setTime(clLay,cell,hitsNTime[iLayer][i]);
	  break;
        }
      }
    }
  }
}

void HMdcLookUpTbSec::findClusInSeg(Int_t seg, Short_t maxCl, Short_t *hPlot){
  if(maxCl<3) return;
  Int_t stackT=0;
  Int_t stackB=0;
  nClsArr=0;
  Int_t nmo=(seg+1)*2;
  Int_t nci,nx,ny;
  Int_t mod1=seg*2;
  Int_t mod2=mod1+1;
//  Int_t maxLocal;
  //---Cluster finding-------------------------------------
  for(Int_t nc=size-1; nc>=0; nc--) {
    if( hPlot[nc]<=0 ) continue;
    if( tablBiN[hPlot[nc]]<maxCl ) continue;
    if( tablBiN[hPlot[nc]&63] < minAmp[mod1] ) continue;
    if( tablBiN[hPlot[nc]&4032] < minAmp[mod2] ) continue;
//    maxLocal=imax-tablBiN[bMboard[nc]];
//    if(maxLocal<4) maxLocal=4;
//    maxCl=maxLocal;
    //---New cluster---------------------------------------
    Int_t& cNBinsI=cNBins[nClsArr];
    Double_t& cMeanXI=cMeanX[nClsArr];
    Double_t& cMeanYI=cMeanY[nClsArr];
    Double_t& cMeanXXI=cMeanXX[nClsArr];
    Double_t& cMeanYYI=cMeanYY[nClsArr];
    Double_t& cMeanYXI=cMeanYX[nClsArr];
    Double_t& cSumWtI=cSumWt[nClsArr];
    Double_t& cMeanXWtI=cMeanXWt[nClsArr];
    Double_t& cMeanYWtI=cMeanYWt[nClsArr];
    Double_t& cMeanXXWtI=cMeanXXWt[nClsArr];
    Double_t& cMeanYYWtI=cMeanYYWt[nClsArr];
    cStatus[nClsArr]=kTRUE;
    cNMergedClus[nClsArr]=1;
    cLCells1[nClsArr].clear();
    cNBinsI=0;
    cMeanXI=cMeanYI=cMeanXXI=cMeanYYI=cMeanYXI=0.;
    cSumWtI=cMeanXWtI=cMeanYWtI=cMeanXXWtI=cMeanYYWtI=0.;

    hPlot[nc]=-hPlot[nc];
    stack[stackB++]=nc;
    nLmax=nc/nBinX;  // First line mast by upest!
    nLmin=nLmax;
    for(Int_t i=0; i<500; i++) {
      xMinCl[i]=size;
      xMaxCl[i]=0;
    }
    while (stackT != stackB) {
      nci=stack[stackT++];
      nx=nci%nBinX;
      ny=nci/nBinX;

      // Finding of min.Xbin & max.Xbin for each line (y) in cluster
      Int_t nLind=nLmax-ny;
      if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
      if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
      if(ny < nLmin) nLmin=ny;

      Double_t x=((Double_t)nx)*xStep+xLowHSt;
      Double_t y=((Double_t)ny)*yStep+yLowHSt;
      Double_t wt=(Double_t)(tablBiN[-hPlot[nci]]-maxCl+1);
      cSumWtI += wt;
      cMeanXWtI += x*wt;
      cMeanYWtI += y*wt;
      cMeanXXWtI += x*x*wt;
      cMeanYYWtI += y*y*wt;
      cNBinsI++;
      cMeanXI += x;
      cMeanYI += y;
      cMeanXXI += x*x;
      cMeanYYI += y*y;
      cMeanYXI += y*x;

      // Testing nearby bins and puting bins to stack
      for(Int_t nTs=0; nTs<8; nTs++) {
        Int_t ncTs=nci+nearbyBins[nTs];
        if( hPlot[ncTs]<=0 || tablBiN[hPlot[ncTs]]<maxCl ) continue;
        if( tablBiN[hPlot[ncTs]&63] < minAmp[mod1] ) continue;
        if( tablBiN[hPlot[ncTs]&4032] < minAmp[mod2] ) continue;
        stack[stackB++]=ncTs;
        hPlot[ncTs]*=-1;
      }
      // If stack overflowed one will increased on 50%:
      if(stackB>stackSize) increaseStack(stackB);
    }
    //-Filling of cluster---------------------------------------------------
    if(nClsArr<499) {
      cX[nClsArr]=cMeanXWtI/cSumWtI;
      cY[nClsArr]=cMeanYWtI/cSumWtI;
      for(Int_t mod=seg*2; mod<nmo; mod++) fillWiresList(mod,cLCells1[nClsArr]);
    } else {
      Warning("findClusInSeg"," Number of clusters >499 skip cluster.");
      nClsArr--;
    }
    nClsArr++;
  }
  if(nClsArr>0) fillClusCut(-2,seg);
  for(Int_t i=0; i<stackT; i++) hPlot[stack[i]]=-hPlot[stack[i]];
}

void HMdcLookUpTbSec::increaseStack(Int_t stackB) {
  Warning("increaseStack","Stack increased on 50\%");
  stackSize += stackSize/2;
  Int_t* stackNew=new Int_t [stackSize+50];
  for(Int_t n=0; n<=stackB; n++) stackNew[n]=stack[n];
  delete [] stack;
  stack=stackNew;
}

void HMdcLookUpTbSec::findClusInSec(Short_t maxCl){
  if(maxCl<3) return;
  Int_t stackT,stackB;
  stackB=stackT=0;
  nClsArr=0;
  Int_t nci,nx,ny;
  //Int_t maxLocal;
  //---Cluster finding-------------------------------------
  for(Int_t nc=size-1; nc>=0; nc--) {
    if( hitPlS1[nc]<=0 ) continue;
    if( (tablBiN[hitPlS1[nc]]+tablBiN[hitPlS2[nc]])<maxCl ) continue;
    if( tablBiN[hitPlS1[nc]&  63] < minAmp[0] ) continue;
    if( tablBiN[hitPlS1[nc]&4032] < minAmp[1] ) continue;
    if( tablBiN[hitPlS2[nc]&  63] < minAmp[2] ) continue;
    if( tablBiN[hitPlS2[nc]&4032] < minAmp[3] ) continue;
//    maxLocal=imax-tablBiN[bMboard[nc]];
//    if(maxLocal<4) maxLocal=4;
//    maxCl=maxLocal;
    //---New cluster---------------------------------------
    Int_t& cNBinsI=cNBins[nClsArr];
    Double_t& cMeanXI=cMeanX[nClsArr];
    Double_t& cMeanYI=cMeanY[nClsArr];
    Double_t& cMeanXXI=cMeanXX[nClsArr];
    Double_t& cMeanYYI=cMeanYY[nClsArr];
    Double_t& cMeanYXI=cMeanYX[nClsArr];
    Double_t& cSumWtI=cSumWt[nClsArr];
    Double_t& cMeanXWtI=cMeanXWt[nClsArr];
    Double_t& cMeanYWtI=cMeanYWt[nClsArr];
    Double_t& cMeanXXWtI=cMeanXXWt[nClsArr];
    Double_t& cMeanYYWtI=cMeanYYWt[nClsArr];
    cStatus[nClsArr]=kTRUE;
    cNMergedClus[nClsArr]=1;
    cLCells1[nClsArr].clear();
    cLCells2[nClsArr].clear();
    cNBinsI=0;
    cMeanXI=cMeanYI=cMeanXXI=cMeanYYI=cMeanYXI=0.;
    cSumWtI=cMeanXWtI=cMeanYWtI=cMeanXXWtI=cMeanYYWtI=0.;

    hitPlS1[nc]=-hitPlS1[nc];
    stack[stackB++]=nc;
    nLmax=nc/nBinX;  // First line mast by upest!
    nLmin=nLmax;
    for(Int_t i=0; i<500; i++) {
      xMinCl[i]=size;
      xMaxCl[i]=0;
    }
    while (stackT != stackB) {
      nci=stack[stackT++];
      nx=nci%nBinX;
      ny=nci/nBinX;

      // Finding of min.Xbin & max.Xbin for each line (y) in cluster
      Int_t nLind=nLmax-ny;
      if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
      if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
      if(ny < nLmin) nLmin=ny;

      Double_t x=((Double_t)nx)*xStep+xLowHSt;
      Double_t y=((Double_t)ny)*yStep+yLowHSt;
      Double_t wt=(Double_t)(tablBiN[-hitPlS1[nci]]+
                             tablBiN[ hitPlS2[nci]]-maxCl+1);
      cSumWtI += wt;
      cMeanXWtI += x*wt;
      cMeanYWtI += y*wt;
      cMeanXXWtI += x*x*wt;
      cMeanYYWtI += y*y*wt;
      cNBinsI++;
      cMeanXI += x;
      cMeanYI += y;
      cMeanXXI += x*x;
      cMeanYYI += y*y;
      cMeanYXI += y*x;
      // Testing nearby bins and puting bins to stack
      for(Int_t nTs=0; nTs<8; nTs++) {
        Int_t ncTs=nci+nearbyBins[nTs];
        if( hitPlS1[ncTs]<=0 ) continue;
        if( (tablBiN[hitPlS1[ncTs]]+tablBiN[hitPlS2[ncTs]])<maxCl ) continue;
        if( tablBiN[hitPlS1[ncTs]&  63] < minAmp[0] ) continue;
        if( tablBiN[hitPlS1[ncTs]&4032] < minAmp[1] ) continue;
        if( tablBiN[hitPlS2[ncTs]&  63] < minAmp[2] ) continue;
        if( tablBiN[hitPlS2[ncTs]&4032] < minAmp[3] ) continue;
        stack[stackB++]=ncTs;
        hitPlS1[ncTs]*=-1;
      }
      // Stack overflowed:
      if(stackB>stackSize) increaseStack(stackB);
    }
    //-End cluster------------------------------------------------------
    if(nClsArr<499) {
      cX[nClsArr]=cMeanXWtI/cSumWtI;
      cY[nClsArr]=cMeanYWtI/cSumWtI;
      for(Int_t mod=0;mod<4;mod++) {
        if(minAmp[mod]==0) continue;
        if(mod<2) fillWiresList(mod,cLCells1[nClsArr]);
        else  fillWiresList(mod,cLCells2[nClsArr]);
      }
    } else {
      Warning("findClusInSec"," Number of clusters >499 skip cluster.");
      nClsArr--;
    }
    nClsArr++;
  }
  if(nClsArr>0) fillClusCut(-nMods,-1);
  for(Int_t i=0; i<stackT; i++) hitPlS1[stack[i]]=-hitPlS1[stack[i]];
}

void HMdcLookUpTbSec::fillClusCut(Int_t mod, Int_t segp) {
  //Test clusters-----------
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
  Int_t nClus=nClsArr;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      if(!cStatus[cl1]) continue;
      HMdcList12GroupCells& cLCSeg1=cLCells1[cl1];
      HMdcList12GroupCells& cLCSeg2=cLCells2[cl1];
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        if(!cStatus[cl2]) continue;
        HMdcList12GroupCells* cLCSeg1s=&(cLCells1[cl2]);
        Float_t dY=cY[cl1]-cY[cl2];
        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 10.) continue;                     // 10. mm !???
        if(fabs(cX[cl1]-cX[cl2]) > 40.) continue;        // 30. mm !???
        if(m1>0 && cLCSeg1.compare(cLCSeg1s,0, 5)<4) continue;
        if(m2>0 && cLCSeg1.compare(cLCSeg1s,6,11)<4) continue;
        if(segp==-1) {
          HMdcList12GroupCells* cLCSeg2s=&(cLCells2[cl2]);
          if(m3>0 && cLCSeg2.compare(cLCSeg2s,0, 5)<4) continue;
          if(m4>0 && cLCSeg2.compare(cLCSeg2s,6,11)<4) continue;
          cLCSeg2.add(cLCSeg2s);
        }
        cLCSeg1.add(cLCSeg1s);
        cStatus[cl2]=kFALSE;
        cSumWt[cl1] += cSumWt[cl2];
        cNBins[cl1] += cNBins[cl2];
        cMeanX[cl1] += cMeanX[cl2];
        cMeanY[cl1] += cMeanY[cl2];
        cMeanXX[cl1] += cMeanXX[cl2];
        cMeanYY[cl1] += cMeanYY[cl2];
        cMeanYX[cl1] += cMeanYX[cl2];
        cMeanXWt[cl1] += cMeanXWt[cl2];
        cMeanYWt[cl1] += cMeanYWt[cl2];
        cMeanXXWt[cl1] += cMeanXXWt[cl2];
        cMeanYYWt[cl1] += cMeanYYWt[cl2];
        cX[cl1]=cMeanXWt[cl1]/cSumWt[cl1];
        cY[cl1]=cMeanYWt[cl1]/cSumWt[cl1];
        cNMergedClus[cl1] += cNMergedClus[cl2];
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
  // Filling containers:
  for(Int_t cl=0; cl<nClsArr; cl++) {
    if(!cStatus[cl]) continue;
    Float_t errX=sqrt(cMeanXXWt[cl]/cSumWt[cl]-cX[cl]*cX[cl] + xHStep2);
    Float_t errY=sqrt(cMeanYYWt[cl]/cSumWt[cl]-cY[cl]*cY[cl] + yHStep2);
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
    Double_t sigma1,sigma2,alpha;
    if(cNBins[cl]<2) sigma1=sigma2=alpha=0.;
    else {
      Double_t nBn=cNBins[cl];
      Double_t eXX = cMeanXX[cl]*nBn- cMeanX[cl]*cMeanX[cl];
      Double_t eYX = cMeanYX[cl]*nBn- cMeanY[cl]*cMeanX[cl];
      Double_t eYY = cMeanYY[cl]*nBn- cMeanY[cl]*cMeanY[cl];
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

    getClusterSlot(seg,cLCells1[cl]);
    if(!fClus) return;
    nClusters++;
    fClus->setMod(mod);
    fClus->setTypeClFinder(typeClFinder);
    fClus->setMinCl(m1,m2);
    fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
    fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],target[2],eTarg[2]);
    fClus->setSumWt(cSumWt[cl]);
    fClus->setNBins(cNBins[cl]);
    fClus->setXY(cX[cl],errX,cY[cl],errY);
    fClus->setNMergClust(cNMergedClus[cl]);
    fClus->setShape(sigma1,sigma2,alpha);
    if(fClus->isGeant()) ((HMdcClusSim*)fClus)->getNTracks();
    if(segp<0) {
      HMdcClus* fClus0=fClus;
      getClusterSlot(1,cLCells2[cl]);
      if(!fClus) return;
      fClus->setMod(mod);
      fClus->setTypeClFinder(typeClFinder);
      fClus->setMinCl(m3,m4);
      fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
      fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],target[2],eTarg[2]);
      fClus->setSumWt(cSumWt[cl]);
      fClus->setNBins(cNBins[cl]);
      fClus->setXY(cX[cl],errX,cY[cl],errY);
      fClus->setNMergClust(cNMergedClus[cl]);
      fClus->setShape(sigma1,sigma2,alpha);
      fClus->setIndexParent(fClus0->getIndex());
      Int_t ind=fClus->getIndex();
      fClus0->setIndexChilds(ind,ind);
      if(fClus->isGeant()) ((HMdcClusSim*)fClus)->getNTracks();
    }
  }
}

void HMdcLookUpTbSec::findClusInMod(Int_t mod, Short_t maxCl, Short_t *hPlot){
  if(maxCl<4) return;
  Int_t stackT=0;
  Int_t stackB=0;
  Int_t seg=mod/2;
  Short_t mask=63 << ((mod & 1)*6); // = 000000111111b or 111111000000b
  nClsArr=0;
  Int_t nci,nx,ny;
//  Int_t maxLocal;
  //---Cluster finding-------------------------------------
  for(Int_t nc=size-1; nc>=0; nc--) {
    if(hPlot[nc]<=0 || tablBiN[hPlot[nc]&mask]<maxCl) continue;
//    maxLocal=imax-tablBiN[bMboard[nc]];
//    if(maxLocal<4) maxLocal=4;
//    maxCl=maxLocal;
    //---New cluster---------------------------------------
    Int_t& cNBinsI=cNBins[nClsArr];
    Double_t& cMeanXI=cMeanX[nClsArr];
    Double_t& cMeanYI=cMeanY[nClsArr];
    Double_t& cMeanXXI=cMeanXX[nClsArr];
    Double_t& cMeanYYI=cMeanYY[nClsArr];
    Double_t& cMeanYXI=cMeanYX[nClsArr];
    Double_t& cSumWtI=cSumWt[nClsArr];
    Double_t& cMeanXWtI=cMeanXWt[nClsArr];
    Double_t& cMeanYWtI=cMeanYWt[nClsArr];
    Double_t& cMeanXXWtI=cMeanXXWt[nClsArr];
    Double_t& cMeanYYWtI=cMeanYYWt[nClsArr];
    cStatus[nClsArr]=kTRUE;
    cNMergedClus[nClsArr]=1;
    cLCells1[nClsArr].clear();
    cNBinsI=0;
    cMeanXI=cMeanYI=cMeanXXI=cMeanYYI=cMeanYXI=0.;
    cSumWtI=cMeanXWtI=cMeanYWtI=cMeanXXWtI=cMeanYYWtI=0.;

    hPlot[nc]=-hPlot[nc];
    stack[stackB++]=nc;
    nLmax=nc/nBinX;  // First line mast by upest!
    nLmin=nLmax;
    for(Int_t i=0; i<500; i++) {
      xMinCl[i]=size;
      xMaxCl[i]=0;
    }
    while (stackT != stackB) {
      nci=stack[stackT++];
      nx=nci%nBinX;
      ny=nci/nBinX;

      // finding of min.Xbin & max.Xbin for each line (y) in cluster
      Int_t nLind=nLmax-ny;
      if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
      if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
      if(ny < nLmin) nLmin=ny;

      Double_t x=((Double_t)nx)*xStep+xLowHSt;
      Double_t y=((Double_t)ny)*yStep+yLowHSt;
      Double_t wt=(Double_t)(tablBiN[(-hPlot[nci])&mask]-maxCl+1);
      cSumWtI += wt;
      cMeanXWtI += x*wt;
      cMeanYWtI += y*wt;
      cMeanXXWtI += x*x*wt;
      cMeanYYWtI += y*y*wt;
      cNBinsI++;
      cMeanXI += x;
      cMeanYI += y;
      cMeanXXI += x*x;
      cMeanYYI += y*y;
      cMeanYXI += y*x;

      // Testing nearby bins and puting bins to stack
      for(Int_t nTs=0; nTs<8; nTs++) {
        Int_t ncTs=nci+nearbyBins[nTs];
        if( hPlot[ncTs]<=0 || tablBiN[hPlot[ncTs]&mask]<maxCl ) continue;
        stack[stackB++]=ncTs;
        hPlot[ncTs]*=-1;
      }
      // Stack overflowed:
      if(stackB>stackSize) increaseStack(stackB);
    }
    //-Filling of cluster---------------------------------------------------
    if(nClsArr<499) {
      cX[nClsArr]=cMeanXWtI/cSumWtI;
      cY[nClsArr]=cMeanYWtI/cSumWtI;
      fillWiresList(mod,cLCells1[nClsArr]);
    } else {
      Warning("findClusInSeg"," Number of clusters >499 skip cluster.");
      nClsArr--;
    }  
    nClsArr++;
  }
  if(nClsArr>0) fillClusCut(mod,seg);
  for(Int_t i=0; i<stackT; i++) hPlot[stack[i]]=-hPlot[stack[i]];
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
      ntot+=fSec.findClusters(imax);
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
  Short_t binS1=0;
  Short_t binS2=0;
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
        color=tablBiN[binS1&4032]; // 4032=111111000000b
        if(color) color+=6;
        color+=tablBiN[binS1&63];        //   63=000000111111b
        if(hitPlS2) {
          Int_t cl=tablBiN[binS2&4032]; // 4032=111111000000b
          if(cl) cl+=cl+6;
          cl+=tablBiN[binS2&63];        //   63=000000111111b
          if(cl) color+=12;
        }
      } else if(type==2) {
        if(binS1&4032) color=2;
        else if(binS1&63) color=1;
        if(hitPlS2) {
          if(binS2&4032) color=4;
          else if(binS2&63) color=3;
        }
      } else {
        color=tablBiN[binS1];
        if(hitPlS2) color+=tablBiN[binS2];
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
