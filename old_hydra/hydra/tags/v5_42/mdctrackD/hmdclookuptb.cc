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

//*-- AUTHOR : Pechenov Vladimir
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
// Container class for Mdc LookUpTable data
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
  if(array) array->Delete();
}

Int_t HMdcLookUpTbLayer::getSize() {
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
  if(array) array->Delete();
}

Int_t HMdcLookUpTbMod::getSize() {
  // returns the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//----------Sector------------------------------------

Int_t* HMdcLookUpTbSec::stack=0;
Int_t  HMdcLookUpTbSec::stackSize;

HMdcLookUpTbSec::HMdcLookUpTbSec(Int_t sec, Int_t nSegs,
                                 Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  array = new TObjArray(4);
  HMdcDetector* fMdcDet= HMdcGetContainers::getObject()->getMdcDetector();
  for (Int_t mod = 0; mod < nSegs*2; mod++)
    if(fMdcDet->getModule(sec,mod)) (*array)[mod]=new HMdcLookUpTbMod(sec,mod);
  nSegments=nSegs;
  nBinX=(inBinX%2 == 0) ? inBinX:inBinX+1;
  nBinY=(inBinY%2 == 0) ? inBinY:inBinY+1;
  size=nBinX*nBinY;
  size4=size/2;
  if((size%2) > 0) size4++;
  hitplot=new Short_t [size4*2];
  hitplot4=(Int_t*) hitplot;
  hist=0;
  if(nSegs==2) {
    hitplotS2=new Short_t [size4*2];
    hitplot4S2=(Int_t*) hitplotS2;
  } else {
    hitplotS2=0;
    hitplot4S2=0;
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
}

HMdcLookUpTbSec::~HMdcLookUpTbSec() {
  // destructor
  array->Delete();
  delete [] hitplot;
  if(stack) delete [] stack;
  stack=0;
  if(hitplotS2) delete [] hitplotS2;
  hitplotS2=0;
  if(hist && hist->IsOnHeap()) delete hist;
  hist=0;
  //====================
//  delete [] bMboard;
}

Int_t HMdcLookUpTbSec::getSize() {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//May be numhits[i]=0 ... in another sub. ?????????????????
void HMdcLookUpTbSec::clearwk() {
  for(Int_t i=0; i<24; i++) nHits[i]=0;
  nHitsTot=0;
  setLUpTb=kFALSE;
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
     if(hitsNTime[ind1][ind2]==1 || nTime==0 ) hitsDel[ind1][ind2]=-1;
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
       if(fLUpTbLayer.ncellbin[nb] == 0) hitplot[nb]--;
     }
   }
 }
*/
}

void HMdcLookUpTbSec::doPlot() {
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
  for(Int_t i=0; i<size4; i++) hitplot4[i]=0;
  if(hitplotS2) for(Int_t i=0; i<size4; i++) hitplot4S2[i]=0;
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
            hitplot[nb] |= iByte;
          }
        }
      }
    }
  }
  // === Segment 2 (mdc3, mdc4) ==========================
  if(!hitplotS2) return;
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
          hitplotS2[nb]=hitplotS2[nb] | iByte;
        }
      }
    }
  }
}

Int_t HMdcLookUpTbSec::findClusters(Int_t *imax){
//TStopwatch timer;
//timer.Start();
  clusters.clear();
  nClusters=0;
  doPlot();
  //isCoilOff - !!!??? ---------------------------------------
  if(nHitsTot==0) return 0;
  nMods=0;
  for(Int_t mod=0; mod<4; mod++) {
    maxClust[mod]=(mod<nSegments*2) ? imax[mod]:0;
    if(maxClust[mod]>0) nMods++;
  }
  if(typeClFinder==1) {
    // independing clust. finding
    for(Int_t m=0; m<2; m++)
      if(maxClust[m]>0) findClusInMod(m,maxClust[m],hitplot);
    for(Int_t m=2; m<4; m++)
      if(maxClust[m]>0) findClusInMod(m,maxClust[m],hitplotS2);
  } else {
    Int_t nSeg=0;
    if(maxClust[0]>0 || maxClust[1]>0) nSeg += 1;
    if((maxClust[2]>0 || maxClust[3]>0) && hitplotS2) nSeg += 2;
    if(nSeg==3) findClusInSec(maxClust[0]+maxClust[1]+maxClust[2]+maxClust[3]);
    else if(nSeg==1) findClusInSeg(0,maxClust[0]+maxClust[1],hitplot);
    else if(nSeg==2) findClusInSeg(1,maxClust[2]+maxClust[3],hitplotS2);
  }

  calcQualClust();
//-------------------------------
//timer.Stop();
//Printf("findClusters: Cpu time for Q.+rest+sort: %f",timer.CpuTime());
  return nClusters;
}

void HMdcLookUpTbSec::findClusInSeg(Int_t seg, Short_t maxCl, Short_t *hPlot){
  if(maxCl<3) return;
  Int_t stackT=0;
  Int_t stackB=0;

  Int_t nmo=(seg+1)*2;
//  if(nmo>2) nmo=2;
  Int_t nClustFirst=nClusters;

  Int_t nci,nx,ny,iLayer;
  Int_t yBinMax,yBinMin;
  Short_t cell;
  Double_t xmid,ymid,dXmid,dYmid,binsCl;
  Int_t xEmin,xEmax,yEmin,yEmax;
  Double_t xTmp,yTmp;
  Double_t shX2=sh_x*0.5;
  Double_t shY2=sh_y*0.5;

  Int_t mod1=seg*2;
  Int_t mod2=mod1+1;
//  Int_t maxLocal;
  //---Cluster finding-------------------------------------
  for(Int_t nc=size-1; nc>=0; nc--) {
    if( hPlot[nc]<=0 ) continue;
    if( tablBiN[hPlot[nc]]<maxCl ) continue;
    if( tablBiN[hPlot[nc]&63] < maxClust[mod1] ) continue;
    if( tablBiN[hPlot[nc]&4032] < maxClust[mod2] ) continue;
//    maxLocal=imax-tablBiN[bMboard[nc]];
//    if(maxLocal<4) maxLocal=4;
//    maxCl=maxLocal;
    //---New cluster---------------------------------------
    hPlot[nc]=-hPlot[nc];
    xmid=ymid=dXmid=dYmid=binsCl=0.0;
    xEmax=yEmax=-100000000;
    xEmin=yEmin=+100000000;
    stack[stackB++]=nc;

    nClusters=clusters.addCluster(maxClust);
    HMdcCluster& clst=clusters[nClusters-1];
    Short_t xMaxCl[500],xMinCl[500];
    Int_t nLmax=nc/nBinX;  // First line mast by upest!
    Int_t nLmin=nLmax;
    for(Int_t i=0; i<500; i++) {
      xMinCl[i]=size;
      xMaxCl[i]=0;
    }
    Int_t nBins=0;
    while (stackT != stackB) {
      nBins++;
      nci=stack[stackT++];
      nx=nci%nBinX;
      ny=nci/nBinX;

      // Finding of min.Xbin & max.Xbin for each line (y) in cluster
      Int_t nLind=nLmax-ny;
      if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
      if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
      if(ny < nLmin) nLmin=ny;

      Double_t wt=(Double_t)(tablBiN[-hPlot[nci]]-maxCl+1);
      binsCl+=wt;
      if(xEmin > nx) xEmin=nx;
      if(yEmin > ny) yEmin=ny;
      if(xEmax < nx) xEmax=nx;
      if(yEmax < ny) yEmax=ny;
      xTmp=(Double_t)(nx)*sh_x+xLow+shX2;
      yTmp=(Double_t)(ny)*sh_y+yLow+shY2;
      xmid+=xTmp*wt;
      ymid+=yTmp*wt;
      dXmid+=xTmp*xTmp*wt;
      dYmid+=yTmp*yTmp*wt;

      // Testing nearby bins and puting bins to stack
      for(Int_t nTs=0; nTs<8; nTs++) {
        Int_t ncTs=nci+nearbyBins[nTs];
        if( hPlot[ncTs]<=0 || tablBiN[hPlot[ncTs]]<maxCl ) continue;
        if( tablBiN[hPlot[ncTs]&63] < maxClust[mod1] ) continue;
        if( tablBiN[hPlot[ncTs]&4032] < maxClust[mod2] ) continue;
        stack[stackB++]=ncTs;
        hPlot[ncTs]*=-1;
      }
      // If stack overflowed one will increased on 50%:
      if(stackB>stackSize) increaseStack(stackB);
    }
    //-End cluster------------------------------------------------------

    clst.setNMods(nMods);
    clst.setBinsWt(binsCl);
    clst.setNBinsCl(nBins);
    xmid=xmid/binsCl;
    ymid=ymid/binsCl;
    clst.setX(xmid);
    clst.setY(ymid);
    clst.setZ(prPlane.getZOnPlane(xmid,ymid));
    clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
    clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
    clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
    clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
    clst.setZup( prPlane.getZOnPlane(0.,clst.getYup()) );
    clst.setZdown( prPlane.getZOnPlane(0.,clst.getYdown()) );
    dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
    dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
    clst.setErrX(dXmid);
    clst.setErrY(dYmid);
    clst.setErrZ( prPlane.B()*dYmid );

    Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
    Short_t numhitsC[24];
    for(Int_t n=0; n<24; n++) numhitsC[n]=0; //!!!
    Short_t numTimesC[24][40];
    for(Int_t mod=seg*2; mod<nmo; mod++) {
      HMdcLookUpTbMod& fMod=(*this)[mod];
      if( !(&fMod) ) continue;
      for(Int_t layer=0; layer<6; layer++) {
        HMdcLookUpTbLayer& fLayer=fMod[layer];
        iLayer=mod*6+layer;
        numhitsC[iLayer]=0;
        for(Int_t i=0; i<nHits[iLayer]; i++) {
          if(hitsDel[iLayer][i]<0) continue;
          cell=hits[iLayer][i];
          HMdcLookUpTbCell& fCell=fLayer[cell];
          yBinMax=fCell.yBinMax;
          if( nLmin > yBinMax ) continue;
          yBinMin=fCell.yBinMin;
//          if( nLmax < yBinMin ) break;            Hits not sorted !

          Int_t lMin, lMax;
          yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
          yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
          Bool_t indSetCell=kFALSE;
          for (Int_t nl=lMin; nl<=lMax; nl++) {
           Int_t nLind=nLmax-nl;
           if(fCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
              if(xMinCl[nLind] > fCell.xBinMax[nl-yBinMin]) continue;
	      indSetCell=kTRUE;
	      break;
            } else {
              if(xMaxCl[nLind] < fCell.xBinMin[nl-yBinMin]) continue;
	      indSetCell=kTRUE;
	      break;
            }
          }
          //This cell have a part in cluster:
          //???? Chto delat' s udalenniymi vremenami ????	
          if(indSetCell) {
            if(numhitsC[iLayer]<39) {
              Short_t ml=numhitsC[iLayer];
	      numhitsC[iLayer]++;
	      hitsC[iLayer][ml]=cell;
 	      numTimesC[iLayer][ml]=hitsNTime[iLayer][i];
	    }
	    else Warning("findClusInSeg","Too many hits (>max).");
          }
        }
      }
    }
    clst.setCluster(numhitsC,  hitsC, numTimesC);
  }
  for(Int_t i=0; i<stackT; i++) hPlot[stack[i]]=-hPlot[stack[i]];
  testClust(nClustFirst, nClusters);

//timer.Stop();
//Printf("findClusters: Cpu time for Q.+rest+sort: %f",timer.CpuTime());
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

//  Int_t nmo=(seg+1)*2;
//  if(nmo>2) nmo=2;
  Int_t nClustFirst=nClusters;

  Int_t nci,nx,ny,iLayer;
  Int_t yBinMax,yBinMin;
  Short_t cell;
  Double_t xmid,ymid,dXmid,dYmid,binsCl;
  Int_t xEmin,xEmax,yEmin,yEmax;
  Double_t xTmp,yTmp;
  Double_t shX2=sh_x*0.5;
  Double_t shY2=sh_y*0.5;

  //Int_t maxLocal;
  //---Cluster finding-------------------------------------
  for(Int_t nc=size-1; nc>=0; nc--) {
    if( hitplot[nc]<=0 ) continue;
    if( (tablBiN[hitplot[nc]]+tablBiN[hitplotS2[nc]])<maxCl ) continue;
    if( tablBiN[hitplot[nc]&63] < maxClust[0] ) continue;
    if( tablBiN[hitplot[nc]&4032] < maxClust[1] ) continue;
    if( tablBiN[hitplotS2[nc]&63] < maxClust[2] ) continue;
    if( tablBiN[hitplotS2[nc]&4032] < maxClust[3] ) continue;
//    maxLocal=imax-tablBiN[bMboard[nc]];
//    if(maxLocal<4) maxLocal=4;
//    maxCl=maxLocal;
    //---New cluster---------------------------------------
    hitplot[nc]=-hitplot[nc];
    xmid=ymid=dXmid=dYmid=binsCl=0.0;
    xEmax=yEmax=-100000000;
    xEmin=yEmin=+100000000;
    stack[stackB++]=nc;

    nClusters=clusters.addCluster(maxClust);
    HMdcCluster& clst=clusters[nClusters-1];
    Short_t xMaxCl[500],xMinCl[500];
    Int_t nLmax=nc/nBinX;  // First line mast by upest!
    Int_t nLmin=nLmax;
    for(Int_t i=0; i<500; i++) {
      xMinCl[i]=size;
      xMaxCl[i]=0;
    }
    Int_t nBins=0;
    while (stackT != stackB) {
      nBins++;
      nci=stack[stackT++];
      nx=nci%nBinX;
      ny=nci/nBinX;

      // Finding of min.Xbin & max.Xbin for each line (y) in cluster
      Int_t nLind=nLmax-ny;
      if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
      if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
      if(ny < nLmin) nLmin=ny;

      Double_t wt=(Double_t)(tablBiN[-hitplot[nci]]+
                             tablBiN[hitplotS2[nci]]-maxCl+1);
      binsCl+=wt;
      if(xEmin > nx) xEmin=nx;
      if(yEmin > ny) yEmin=ny;
      if(xEmax < nx) xEmax=nx;
      if(yEmax < ny) yEmax=ny;
      xTmp=(Double_t)(nx)*sh_x+xLow+shX2;
      yTmp=(Double_t)(ny)*sh_y+yLow+shY2;
      xmid+=xTmp*wt;
      ymid+=yTmp*wt;
      dXmid+=xTmp*xTmp*wt;
      dYmid+=yTmp*yTmp*wt;

      // Testing nearby bins and puting bins to stack
      for(Int_t nTs=0; nTs<8; nTs++) {
        Int_t ncTs=nci+nearbyBins[nTs];
        if( hitplot[ncTs]<=0 ) continue;
        if( (tablBiN[hitplot[ncTs]]+tablBiN[hitplotS2[ncTs]])<maxCl ) continue;
        if( tablBiN[hitplot[ncTs]&63] < maxClust[0] ) continue;
        if( tablBiN[hitplot[ncTs]&4032] < maxClust[1] ) continue;
        if( tablBiN[hitplotS2[ncTs]&63] < maxClust[2] ) continue;
        if( tablBiN[hitplotS2[ncTs]&4032] < maxClust[3] ) continue;
        stack[stackB++]=ncTs;
        hitplot[ncTs]*=-1;
      }
      // Stack overflowed:
      if(stackB>stackSize) increaseStack(stackB);
    }
    //-End cluster------------------------------------------------------

    clst.setNMods(nMods);
    clst.setBinsWt(binsCl);
    clst.setNBinsCl(nBins);
    xmid=xmid/binsCl;
    ymid=ymid/binsCl;
    clst.setX(xmid);
    clst.setY(ymid);
//    clst.setZ(Dp-Ap*xmid-Bp*ymid);
    clst.setZ(prPlane.getZOnPlane(xmid,ymid));
    clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
    clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
    clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
    clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
//    clst.setZup( Dp-Bp*clst.getYup() );
//    clst.setZdown( Dp-Bp*clst.getYdown() );
    clst.setZup( prPlane.getZOnPlane(0.,clst.getYup()) );
    clst.setZdown( prPlane.getZOnPlane(0.,clst.getYdown()) );
    dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
    dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
    clst.setErrX(dXmid);
    clst.setErrY(dYmid);
//    clst.setErrZ( Bp*dYmid );
    clst.setErrZ( prPlane.B()*dYmid );

    Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
    Short_t numhitsC[24];
    for(Int_t n=0; n<24; n++) numhitsC[n]=0; //!!!
    Short_t numTimesC[24][40];
    for(Int_t mod=0; mod<4; mod++) {
      for(Int_t layer=0; layer<6; layer++) {
        iLayer=mod*6+layer;
        if(nHits[iLayer]<=0) continue;
        HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
        numhitsC[iLayer]=0;
        for(Int_t i=0; i<nHits[iLayer]; i++) {
          if(hitsDel[iLayer][i]<0) continue;
          cell=hits[iLayer][i];
          HMdcLookUpTbCell& fCell=fLayer[cell];
          yBinMax=fCell.yBinMax;
          if( nLmin > yBinMax ) continue;
          yBinMin=fCell.yBinMin;
 //         if( nLmax < yBinMin ) break;            Hits not sorted !

          Int_t lMin, lMax;
          yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
          yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
          Bool_t indSetCell=kFALSE;
          for (Int_t nl=lMin; nl<=lMax; nl++) {
            Int_t nLind=nLmax-nl;
            if(fCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
              if(xMinCl[nLind] > fCell.xBinMax[nl-yBinMin]) continue;
	      indSetCell=kTRUE;
	      break;
            } else {
              if(xMaxCl[nLind] < fCell.xBinMin[nl-yBinMin]) continue;
	      indSetCell=kTRUE;
	      break;
            }
          }
          //This cell have a part in cluster:
          //???? Chto delat' s udalenniymi vremenami ????	
          if(!indSetCell) continue;
          if(numhitsC[iLayer]<39) {
            Short_t ml=numhitsC[iLayer];
	    numhitsC[iLayer]++;
	    hitsC[iLayer][ml]=cell;
 	    numTimesC[iLayer][ml]=hitsNTime[iLayer][i];
	  }
	  else Warning("findClusInSec","Too many hits (>max).");
        }
      }
    }
    clst.setCluster(numhitsC,  hitsC, numTimesC);
    if(stackB>stackSize) increaseStack(stackB);
  }
  for(Int_t i=0; i<stackT; i++) hitplot[stack[i]]=-hitplot[stack[i]];
  testClust(nClustFirst, nClusters);
}

void HMdcLookUpTbSec::testClust(Int_t nFirstClus, Int_t nLastClus) {
//Test clusters-----------
//  nClusters=clusters.Test(Ap,Bp,Dp);
  for(Int_t nClF=nFirstClus; nClF<nLastClus-1; nClF++) {
    HMdcCluster& pClF=clusters[nClF];
    for(Int_t nClS=nClF+1; nClS<nLastClus; nClS++) {
      HMdcCluster& pClS=clusters[nClS];
//      if(pClF.getY()-pClS.getY() > 50.) break;            // 50. mm !???
      if(fabs(pClF.getY()-pClS.getY()) >= 10.) continue;  // 10. mm !???
      if(fabs(pClF.getX()-pClS.getX()) >= 40.) continue;  // 30. mm !???
      if(pClS.Test(pClF,prPlane,shX2,shY2)) {
        nClusters=clusters.deleteCluster(nClS);
        nLastClus--;
        for(Int_t nClT=nClF-1; nClT>=0; nClT--){
          HMdcCluster& pClT=clusters[nClT];
//          if(pClT.getY()-pClF.getY() > 50.) break;            // 50. mm !???
          if(fabs(pClT.getY()-pClF.getY()) >= 10.) continue;  // 10. mm !???
          if(fabs(pClT.getX()-pClF.getX()) >= 40.) continue;  // 30. mm !???
          if(pClT.Test(pClF,prPlane,shX2,shY2)) {
            nClusters=clusters.deleteCluster(nClT);
            nLastClus--;
            nClF--;
            nClT=nClF;
          }
        }
        nClS=nClF;
      }
    }
  }
}

void HMdcLookUpTbSec::calcQualClust(void) {
  clusters.calcxy(); //???? delaetsya sortirovka !!!
//  clusters.Sort(); // Onli after calcxy!!!
  Short_t hits[24][250];
  for(Int_t l=0; l<24; l++) for(Int_t c=0; c<250; c++) hits[l][c]=0;
  for(Int_t nCl=0; nCl<nClusters; nCl++) {
    HMdcCluster& pCl=clusters[nCl];
    for(Int_t l=0; l<24; l++) {
      Int_t nWires=pCl.getNumHits(l);   // num. fired wires in this clusters for loc
      for(Int_t nW=0; nW<nWires; nW++) {
        Int_t nCell=pCl.getHit(l,nW);   // num. fired wire
        hits[l][nCell]++;
      }
    }
  }
  for(Int_t nCl=0; nCl<nClusters; nCl++) {
    Int_t nTotCell=0;
    Int_t nCellDb=0;
    HMdcCluster& pCl=clusters[nCl];
    for(Int_t l=0; l<24; l++) {
      Int_t nWires=pCl.getNumHits(l);   // num. fired wires in this clusters for loc
      for(Int_t nW=0; nW<nWires; nW++) {
        Int_t nCell=pCl.getHit(l,nW);   // num. fired wire
        nTotCell++;
        if(hits[l][nCell]>1) nCellDb++;
      }
    }
    Float_t prDb=((Float_t) nCellDb)/((Float_t) nTotCell);
    pCl.setPrDb(prDb);
  }
}

void HMdcLookUpTbSec::findClusInMod(Int_t mod, Short_t maxCl, Short_t *hPlot){
  if(maxCl<4) return;
  Int_t stackT=0;
  Int_t stackB=0;

  Short_t mask=63 << ((mod & 1)*6); // = 000000111111b or 111111000000b
  Int_t nClustFirst=nClusters;

  Int_t nci,nx,ny,iLayer;
  Int_t yBinMax,yBinMin;
  Short_t cell;
  Double_t xmid,ymid,dXmid,dYmid,binsCl;
  Int_t xEmin,xEmax,yEmin,yEmax;
  Double_t xTmp,yTmp;
  Double_t shX2=sh_x*0.5;
  Double_t shY2=sh_y*0.5;

//  Int_t maxLocal;
  //---Cluster finding-------------------------------------
  for(Int_t nc=size-1; nc>=0; nc--) {
    if(hPlot[nc]<=0 || tablBiN[hPlot[nc]&mask]<maxCl) continue;
//    maxLocal=imax-tablBiN[bMboard[nc]];
//    if(maxLocal<4) maxLocal=4;
//    maxCl=maxLocal;
    //---New cluster---------------------------------------
    hPlot[nc]=-hPlot[nc];
    xmid=ymid=dXmid=dYmid=binsCl=0.0;
    xEmax=yEmax=-100000000;
    xEmin=yEmin=+100000000;
    stack[stackB++]=nc;

    nClusters=clusters.addCluster(maxClust);
    HMdcCluster& clst=clusters[nClusters-1];
    Short_t xMaxCl[500],xMinCl[500];
    Int_t nLmax=nc/nBinX;  // First line mast by upest!
    Int_t nLmin=nLmax;
    for(Int_t i=0; i<500; i++) {
      xMinCl[i]=size;
      xMaxCl[i]=0;
    }
    Int_t nBins=0;
    while (stackT != stackB) {
      nBins++;
      nci=stack[stackT++];
      nx=nci%nBinX;
      ny=nci/nBinX;

      // finding of min.Xbin & max.Xbin for each line (y) in cluster
      Int_t nLind=nLmax-ny;
      if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
      if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
      if(ny < nLmin) nLmin=ny;

      Double_t wt=(Double_t)(tablBiN[(-hPlot[nci])&mask]-maxCl+1);
      binsCl+=wt;
      if(xEmin > nx) xEmin=nx;
      if(yEmin > ny) yEmin=ny;
      if(xEmax < nx) xEmax=nx;
      if(yEmax < ny) yEmax=ny;
      xTmp=(Double_t)(nx)*sh_x+xLow+shX2;
      yTmp=(Double_t)(ny)*sh_y+yLow+shY2;
      xmid+=xTmp*wt;
      ymid+=yTmp*wt;
      dXmid+=xTmp*xTmp*wt;
      dYmid+=yTmp*yTmp*wt;

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
    //-End cluster------------------------------------------------------

    clst.setNMods(1);
    clst.setMod(mod);
    clst.setBinsWt(binsCl);
    clst.setNBinsCl(nBins);
    xmid=xmid/binsCl;
    ymid=ymid/binsCl;
    clst.setX(xmid);
    clst.setY(ymid);
//    clst.setZ(Dp-Ap*xmid-Bp*ymid);
    clst.setZ(prPlane.getZOnPlane(xmid,ymid));
    clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
    clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
    clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
    clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
//    clst.setZup( Dp-Bp*clst.getYup() );
//    clst.setZdown( Dp-Bp*clst.getYdown() );
    clst.setZup( prPlane.getZOnPlane(0.,clst.getYup()) );
    clst.setZdown( prPlane.getZOnPlane(0.,clst.getYdown()) );
    dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
    dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
    clst.setErrX(dXmid);
    clst.setErrY(dYmid);
//    clst.setErrZ( Bp*dYmid );
    clst.setErrZ( prPlane.B()*dYmid );

    Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
    Short_t numhitsC[24];
    for(Int_t n=0; n<24; n++) numhitsC[n]=0; //!!!
    Short_t numTimesC[24][40];
    for(Int_t layer=0; layer<6; layer++) {
      iLayer=mod*6+layer;
      if(nHits[iLayer]<=0) continue;
      HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
      numhitsC[iLayer]=0;
      for(Int_t i=0; i<nHits[iLayer]; i++) {
        if(hitsDel[iLayer][i]<0) continue;
        cell=hits[iLayer][i];
        HMdcLookUpTbCell& fCell=fLayer[cell];
        yBinMax=fCell.yBinMax;
        if( nLmin > yBinMax ) continue;
        yBinMin=fCell.yBinMin;
//        if( nLmax < yBinMin ) break;            Hits not sorted !

        Int_t lMin, lMax;
        yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
        yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
        Bool_t indSetCell=kFALSE;
        for (Int_t nl=lMin; nl<=lMax; nl++) {
          Int_t nLind=nLmax-nl;
          if(fCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
            if(xMinCl[nLind] > fCell.xBinMax[nl-yBinMin]) continue;
	    indSetCell=kTRUE;
	    break;
          } else {
            if(xMaxCl[nLind] < fCell.xBinMin[nl-yBinMin]) continue;
	    indSetCell=kTRUE;
	    break;
          }
        }
        //This cell have a part in cluster:
        //???? Chto delat' s udalenniymi vremenami ????	
        if(indSetCell) {
          if(numhitsC[iLayer]<39) {
            Short_t ml=numhitsC[iLayer];
	    numhitsC[iLayer]++;
	    hitsC[iLayer][ml]=cell;
 	    numTimesC[iLayer][ml]=hitsNTime[iLayer][i];
	  }
	  else Warning("findClusInMod","Too many hits in cluster(>max).");
        }
      }
    }
    clst.setCluster(numhitsC,  hitsC, numTimesC);
    if(stackB>stackSize) increaseStack(stackB);
  }
  for(Int_t i=0; i<stackT; i++) hPlot[stack[i]]=-hPlot[stack[i]];
  testClust(nClustFirst, nClusters);
}

void HMdcLookUpTbSec::printClusters(){
  clusters.print();
}


//---------------------------------------------------------
HMdcLookUpTb* HMdcLookUpTb::fMdcLookUpTb=0;

HMdcLookUpTb::HMdcLookUpTb() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcLookUpTbSec
  // The container name is set to "MdcLookUpTb"
  fName="MdcLookUpTb";
  fTitle="Look Up Table for Dubna track finding";
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
}

void HMdcLookUpTb::setPar(Int_t inBinX, Bool_t isCOff) {
  nBinX=(inBinX%2 == 0) ? inBinX:inBinX+1;
  if(nBinX<100) nBinX=320;
  nBinY=(nBinX*278)/100;
  if(nBinY%2 == 0) nBinY++;
  isCoilOff=isCOff;
}

Bool_t HMdcLookUpTb::initContainer() {
  // Is called from "reinit" of reconstractor!
  if( !fMdcDet || !HMdcGetContainers::isInited(fMdcGeomPar) ||
      !HMdcGetContainers::isInited(fSpecGeomPar) ||
      !HMdcGetContainers::isInited(fLayerGeomPar) ||
      !fSizesCells->initContainer()) return kFALSE;
  
  if( !status && (fSizesCells->hasChanged() || fSpecGeomPar->hasChanged() || 
      fMdcGeomPar->hasChanged() || fLayerGeomPar->hasChanged()) ) {
    changed=kTRUE;   
    for (Int_t sec = 0; sec < 6; sec++) {
      if(!fMdcDet->isSectorActive(sec))  continue;
      if( !(*array)[sec] ) {
        Int_t nSegs=(fMdcDet->getModule(sec,0)||fMdcDet->getModule(sec,1)) ?1:0;
        if(isCoilOff)
          nSegs=(fMdcDet->getModule(sec,2) || fMdcDet->getModule(sec,3)) ?2:1;
        else if(nSegs==0) continue;
        (*array)[sec] = new HMdcLookUpTbSec(sec,nSegs,nBinX,nBinY);
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
  array->Delete();
}

Int_t HMdcLookUpTb::getSize() {
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

void HMdcLookUpTb::printClusters(Int_t sec=-1){
  Int_t s1=0;
  Int_t s2=5;
  if( sec>=0 && sec<=5 ) s1=s2=sec;
  for(Int_t s=s1;s<=s2;s++) {
    if( (*array)[s] ) {
      printf("-Sector %i  ----Clusters:\n",s);
      HMdcLookUpTbSec& sec=(*this)[s];
      sec.printClusters();
    }
  }
}

Int_t  HMdcLookUpTb::findClusters(Int_t *imax){
  Int_t ntot=0;
  for(Int_t s=0;s<6;s++) {
    if( (*array)[s] ) {
      HMdcLookUpTbSec& sec=(*this)[s];
      ntot+=sec.findClusters(imax);
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
      binS1=hitplot[nBin];
      if(plBining==2)  binS1 |=
        hitplot[nBin+1] | hitplot[nBin+nBinX] | hitplot[nBin+nBinX+1];
      if( hitplotS2 ) {
        binS2=hitplotS2[nBin];
        if(plBining==2) binS2 |=
          hitplotS2[nBin+1] | hitplotS2[nBin+nBinX] | hitplotS2[nBin+nBinX+1];
      }
      if( binS1==0 && binS2==0 ) continue;
      Int_t color=0;
      if(type==1) {
        color=tablBiN[binS1&4032]; // 4032=111111000000b
        if(color) color+=6;
        color+=tablBiN[binS1&63];        //   63=000000111111b
        if(hitplotS2) {
          Int_t cl=tablBiN[binS2&4032]; // 4032=111111000000b
          if(cl) cl+=cl+6;
          cl+=tablBiN[binS2&63];        //   63=000000111111b
          if(cl) color+=12;
        }
      } else if(type==2) {
        if(binS1&4032) color=2;
        else if(binS1&63) color=1;
        if(hitplotS2) {
          if(binS2&4032) color=4;
          else if(binS2&63) color=3;
        }
      } else {
        color=tablBiN[binS1];
        if(hitplotS2) color+=tablBiN[binS2];
      }
      hist->Fill((Float_t)nx*sh_x+sh_x*0.5+xLow,
                 (Float_t)ny*sh_y+sh_y*0.5+yLow, color);
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
        yBinMinT=(Int_t)((cellProj[yBinMinT].gety()-fsec.yLow)/fsec.sh_y);
        yBinMaxT=(Int_t)((cellProj[yBinMaxT].gety()-fsec.yLow)/fsec.sh_y);
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
          Double_t y=(ny+1)*fsec.sh_y+fsec.yLow;
          Double_t x1,x2;
          calcX(cellProj, y, x1, x2);	
          Int_t nx1i=(Int_t)((x1-fsec.xLow)/fsec.sh_x);
	  Int_t nx2i=(Int_t)((x2-fsec.xLow)/fsec.sh_x);
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

  //  Calc. of plane for projection:
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
//    if( !fGetCont->getSecTransLayer(coordSys,sec,firstMod,5) ) return kFALSE;
    tmp=(coordSys.getRotMatrix())(8);
    tmp=1./(tmp*tmp);
    tanTheta1=sqrt(tmp-1.);
    zMdc1=(coordSys.getTransVector()).getZ()*tmp;

    coordSys=*((*fSizesCells)[sec][lastMod][0].getSecTrans());
//    if( !fGetCont->getSecTransLayer(coordSys,sec,lastMod,0) ) return kFALSE;
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
//    if( !fGetCont->getSecTransMod(coordSys,sec,iPar) ) return kFALSE;
    printf("\n===> Sec.%i: Using middle plane of MDC%i as projection plane\n",
                              sec+1,iPar+1);
  }
  ((*this)[sec]).prPlane.setPrPlane(coordSys);
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcTarget(Int_t sec){
  //Geting size of target
  Int_t nT=fSpecGeomPar->getNumTargets()-1;
  if( nT < 0 ) {
    Error("calcTarget","Number of targets = %i!",nT+1);
    return kFALSE;
  }
  HMdcLookUpTbSec& fsec=(*this)[sec];
  fsec.target[0]=(fSpecGeomPar->getTarget( 0)->getTransform()).getTransVector();
  fsec.target[0].setZ( fsec.target[0].getZ() +
      fSpecGeomPar->getTarget( 0)->getPoint(0)->getZ());
  fsec.target[1]=(fSpecGeomPar->getTarget(nT)->getTransform()).getTransVector();
  fsec.target[1].setZ( fsec.target[1].getZ() +
      fSpecGeomPar->getTarget(nT)->getPoint(2)->getZ());
//  const HGeomTransform& trans=fGetCont->getLabTransSec(sec);
  const HGeomTransform* trans=(*fSizesCells)[sec].getLabTrans();
  if(&trans == 0) return kFALSE;
  fsec.target[0]=trans->transTo(fsec.target[0]);
  fsec.target[1]=trans->transTo(fsec.target[1]);
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
          fsec.prPlane.calcIntersection(fsec.target[targ],
                       senVol-fsec.target[targ],cross);
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
  fsec.sh_x=(fsec.xUp-fsec.xLow)/(Double_t)(fsec.nBinX-3);
  fsec.sh_y=(fsec.yUp-fsec.yLow)/(Double_t)(fsec.nBinY-3);
  fsec.yLow -= fsec.sh_y*1.5;                  //mast be bin=0 at limits of plot
  fsec.yUp += fsec.sh_y*1.5;
  fsec.xLow -= fsec.sh_x*1.5;
  fsec.xUp += fsec.sh_x*1.5;
  fsec.sh_x=(fsec.xUp-fsec.xLow)/(Double_t)(fsec.nBinX);
  fsec.sh_y=(fsec.yUp-fsec.yLow)/(Double_t)(fsec.nBinY);
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcCellProj(Int_t sec,HMdcTrap& cellSize,
    HMdcTrapPlane& cellProj) {
  // Calculation of cell projection on the proj. plane.
  // (For sector coordinat system only !!!)
  HGeomVector pProj[16];
  HMdcLookUpTbSec& fsec=(*this)[sec];
  for (Int_t i=0; i<8; i++) {
    for (Int_t j=0; j<2; j++) fsec.prPlane.calcIntersection(fsec.target[j],
      cellSize[i]-fsec.target[j],pProj[j*8+i]);
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
