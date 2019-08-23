#include "hmdclookuptb.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgetcontainers.h"
#include <iostream.h>
#include "hmdcgeomobj.h"
#include "hmdcgeomstruct.h"
#include "TStopwatch.h"

//*-- AUTHOR : Pechenov Vladimir
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
ClassImp(HMdcCalcLookUpTb)


HMdcLookUpTbCell::HMdcLookUpTbCell(Short_t yBinMinT, Short_t yBinMaxT) {
 nLines=yBinMaxT-yBinMinT+1;
 yBinMin=yBinMinT;
 yBinMax=yBinMaxT;
 line=0;
 xBinMin=new Short_t [nLines];
 xBinMax=new Short_t [nLines];
}

HMdcLookUpTbCell::~HMdcLookUpTbCell() {
 delete [] xBinMin;
 delete [] xBinMax;
}

void HMdcLookUpTbCell::addLine(Short_t nc1, Short_t nc2){
 if(line < nLines) {
  xBinMin[line]=nc1;
  xBinMax[line]=nc2;
  line++;
 }
 else printf("Error in HMdcLookUpTbCell::addLine(): nb. of lines > %i\n", line);
}

//----------Layer-----------------------------

HMdcLookUpTbLayer::HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer) {
// Geting of pointers to par. cont.
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return;
  HMdcGeomStruct* pMdc=parCont->getMdcGeomStruct();
  if( !pMdc ) return;
  Int_t nCells=((*pMdc)[sec][mod])[layer];
  array = new TObjArray(nCells);
}

HMdcLookUpTbLayer::~HMdcLookUpTbLayer() {
  if(array) array->Delete();
}

void HMdcLookUpTbLayer::initCell(Int_t nce, Int_t yBinMinT, Int_t yBinMaxT){
  if((*array)[nce]) {
    delete (*array)[nce];
    printf("Delete old and create new HMdcLookUpTbCell: n.cell=%i\n",nce);
  }
  (*array)[nce] = new HMdcLookUpTbCell(yBinMinT, yBinMaxT);
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
  for (Int_t layer = 0; layer < 6; layer++) {
    (*array)[layer] = new HMdcLookUpTbLayer(sec, mod, layer);
  }
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
HMdcLookUpTbSec::HMdcLookUpTbSec(Int_t sec, Int_t nSegs,
                                 Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  array = new TObjArray(4);
  for (Int_t mod = 0; mod < nSegs*2; mod++) {
    (*array)[mod] = new HMdcLookUpTbMod(sec,mod);
  }
  nBinX=inBinX;
  nBinY=inBinY;
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
//   bMboard=new Char_t [size];
//   for(Int_t i=0; i<size; i++) bMboard[i]=0;
//   indFill=0;
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
}

HMdcLookUpTbSec::~HMdcLookUpTbSec() {
  // destructor
  array->Delete();
  delete [] hitplot;
  if(hitplotS2) delete [] hitplotS2;
  //====================
//  delete [] bMboard;

//---------  if (fDigitGeomPar) gHades->getRuntimeDb()->removeContainer("MdcDigitGeomPar");
//  delete breakPoints;
//  delete ntuple;
}

void HMdcLookUpTbSec::setParPlane(const HGeomTransform &sys){
  planeCoord.setTransform(sys);
  const HGeomRotation& mt=planeCoord.getRotMatrix();
  const HGeomVector& tr=planeCoord.getTransVector();
  Double_t Cp=(mt(0)*mt(4)-mt(3)*mt(1));
  Ap=(mt(3)*mt(7)-mt(6)*mt(4))/Cp;
  Bp=(mt(6)*mt(1)-mt(0)*mt(7))/Cp;
  Dp=Ap*tr(0)+Bp*tr(1)+tr(2);
}

Int_t HMdcLookUpTbSec::getSize() {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

void HMdcLookUpTbSec::setEdgesPlot(Double_t ixLow, Double_t ixUp,
                               Double_t iyLow, Double_t iyUp) {
  xLow=ixLow; xUp=ixUp; sh_x=(xUp-xLow)/(Double_t)(nBinX);
  yLow=iyLow; yUp=iyUp; sh_y=(yUp-yLow)/(Double_t)(nBinY);
}

//May be numhits[i]=0 ... in another sub. ?????????????????
void HMdcLookUpTbSec::clearwk() {
  for(Int_t i=0; i<24; i++) nHits[i]=0;
  nHitsTot=0;
  setLUpTb=kFALSE;
}


void HMdcLookUpTbSec::setCell(Short_t mod, Short_t layer,
                              Short_t cell, Short_t nTimes){
 if(mod<4) {
   HMdcLookUpTbLayer& pLayer=(*this)[mod][layer];
   if(pLayer.isCell(cell)) {
     Short_t ind1=mod*6+layer;
     Short_t ind2=nHits[ind1];
     if (ind2<250) {
       hits[ind1][ind2]=cell;
       hitsNTime[ind1][ind2]=nTimes;
       hitsDel[ind1][ind2]=0;
       nHits[ind1]++;
       nHitsTot++;
     }
     else Printf("Error: Too many hits in layer! It's impossible!\n");
   }
//   else Printf("Cell %i doesn't exist!\n",cell);
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
   else Printf("Error: Hit %iM%iL%iC is absent!\n",mod,layer,cell);
 }
/*
 HMdcLookUpTbLayer& pLayer=(*this)[mod][layer];
 HMdcLookUpTbCell& pCell=pLayer[cell];
 UChar_t nc=(UChar_t) nCell;
 const UChar_t one=1;
 for(Int_t l=0; l<pCell.line; l++) {
   for(Int_t nb=pCell.xBinMin[l]; nb<=pCell.xBinMax[l]; nb++) {
     UChar_t bit=(one<<(nc-pLayer.nfcell[nb]));
     if(pLayer.ncellbin[nb]&bit != 0) {
       pLayer.ncellbin[nb]=pLayer.ncellbin[nb]&(~bit);
       if(pLayer.ncellbin[nb] == 0) hitplot[nb]--;
     }
   }
 }
*/
}

void HMdcLookUpTbSec::doPlot() {
 Short_t cell,iByte;
 const Short_t one=1;
//============================================
/* if(indFill==0) {
   Int_t mmm[17][3]={
   {0,0,4}, {0,56,67}, {0,76,83}, {0,92,95},
   {1,0,4}, {1,72,72}, {2,0,4}, {2,32,35}, {2,48,55}, {2,64,71}, {2,76,103},
   {4,0,4}, {4,48,59}, {4,68,75}, {4,84,120}, {5,129,129}, {5,159,159}};
   for(Int_t i=0; i<17; i++) {
     iByte=one<<mmm[i][0];
     HMdcLookUpTbLayer& fLayer=(*this)[1][mmm[i][0]];  //mod 1; layer ...
     for(Int_t cell=mmm[i][1]; cell<=mmm[i][2]; cell++) {
       if(fLayer.isCell(cell)) {
         HMdcLookUpTbCell& pCell=fLayer[cell];
         for(Int_t ln=0; ln<pCell.line; ln++) {
           for(Int_t nb=pCell.xBinMin[ln]; nb<=pCell.xBinMax[ln]; nb++) {
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
  for(Int_t i=0; i<12; i++) {
    iByte=one<<i;
    Int_t mod=i/6;
    Int_t layer=i%6;
    HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
    for(Int_t n=0; n<nHits[i]; n++) {
      if(hitsDel[i][n]>-1) {
        cell=hits[i][n];
        if(fLayer.isCell(cell)) {
          HMdcLookUpTbCell& pCell=fLayer[cell];
          for(Int_t ln=0; ln<pCell.line; ln++) {
            Int_t shift=(pCell.yBinMin+ln) * nBinX;
            Int_t nbL=pCell.xBinMax[ln]+shift;
            for(Int_t nb=pCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
              hitplot[nb] |= iByte;
            }
          }
        }
      }
    }
  }
  // === Segment 2 (mdc3, mdc4) ==========================
  if(!hitplotS2) return;
  for(Int_t i=12; i<24; i++) {
    iByte=one<<(i-12);
    Int_t mod=i/6;
    Int_t layer=i%6;
    HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
    for(Int_t n=0; n<nHits[i]; n++) {
      if(hitsDel[i][n]>-1) {
        cell=hits[i][n];
        if(fLayer.isCell(cell)) {
          HMdcLookUpTbCell& pCell=fLayer[cell];
          for(Int_t ln=0; ln<pCell.line; ln++) {
            Int_t shift=(pCell.yBinMin+ln) * nBinX;
            Int_t nbL=pCell.xBinMax[ln]+shift;
            for(Int_t nb=pCell.xBinMin[ln]+shift; nb<=nbL; nb++) {
              hitplotS2[nb]=hitplotS2[nb] | iByte;
            }
          }
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
    maxClust[mod]=imax[mod];
    if(maxClust[mod]>0) nMods++;
  }
  if(typeClFinder==1) {
    // independing clust. finding
    for(Int_t m=0; m<2; m++) if(imax[m]>0) findClusInMod(m,imax[m],hitplot);
    for(Int_t m=2; m<4; m++) if(imax[m]>0) findClusInMod(m,imax[m],hitplotS2);
  } else {
    Int_t nSeg=0;
    if(maxClust[0]>0 || maxClust[1]>0) nSeg += 1;
    if((maxClust[2]>0 || maxClust[3]>0) && hitplotS2) nSeg += 2;
    if(nSeg==3) findClusInSec(imax[0]+imax[1]+imax[2]+imax[3]); //track in 2 seg., coil is off
    else if(nSeg==1) findClusInSeg(0,imax[0]+imax[1],hitplot);
    else if(nSeg==2) findClusInSeg(1,imax[2]+imax[3],hitplotS2);
  }
  
  calcQualClust();
//-------------------------------
//timer.Stop();
//Printf("findClusters: Cpu time for doPlot: %f",timer.CpuTime());
//timer.Start();
 
 
// Char_t maxCl=(imax>12||imax<1) ? 12:imax;
//??? Short_t maxCl=maxClust[0]+maxClust[1]+maxClust[2]+maxClust[3];
//??? if(maxCl<3) return 0;
 
//timer.Stop();
//Printf("findClusters: Cpu time for Q.+rest+sort: %f",timer.CpuTime());
 return nClusters;
}

/*  Old version !!!
Int_t HMdcLookUpTbSec::findClusters(Int_t *imax){
//TStopwatch timer;
//timer.Start();
 clusters.clear();
 Int_t stackSize=(Int_t)(9950.*(Double_t(nBinX)/315.)*(Double_t(nBinX)/315.));
 Int_t stack[(stackSize*105)/100];   //?!!
 Int_t stackT,stackB;
 stackB=stackT=0;
//timer.Stop();
//Printf("findClusters: Cpu time for Clear+initstack: %f",timer.CpuTime());
//timer.Start();
 nMods=0;
 for(Int_t mod=0; mod<4; mod++) {
   maxClust[mod]=imax[mod];
   if(maxClust[mod]>0) nMods++;
 }
 doPlot();
 if(nHitsTot==0) return 0;
//timer.Stop();
//Printf("findClusters: Cpu time for doPlot: %f",timer.CpuTime());
//timer.Start();
 
 
// Char_t maxCl=(imax>12||imax<1) ? 12:imax;
 Short_t maxCl=maxClust[0]+maxClust[1]+maxClust[2]+maxClust[3];
 if(maxCl<3) return 0;
 
 Int_t nmo=getSize();
 if(nmo>2) nmo=2;
 Int_t nla[2];
 if(nmo>=0) nla[0]=(*this)[0].getSize();
 if(nmo>0)  nla[1]=(*this)[1].getSize();

 Int_t nci,nx,ny,iLayer;
 Int_t yBinMax,yBinMin;
 Short_t cell;
 Double_t xmid,ymid,dXmid,dYmid,binsCl;
 Int_t xEmin,xEmax,yEmin,yEmax;
 Double_t xTmp,yTmp;
 nClusters=0;
 Double_t shX2=sh_x*0.5;
 Double_t shY2=sh_y*0.5;

//Int_t maxLocal;
 //---Cluster finding-------------------------------------
 for(Int_t nc=size-1; nc>=0; nc--) {
   if(hitplot[nc]>0 && tablBiN[hitplot[nc]]>=maxCl) {
//  maxLocal=imax-tablBiN[bMboard[nc]];
//  if(maxLocal<4) maxLocal=4;
//  maxCl=maxLocal;
  //---New cluster---------------------------------------
   hitplot[nc]=-hitplot[nc];
   xmid=ymid=dXmid=dYmid=binsCl=0.0;
   xEmax=yEmax=-100000000;
   xEmin=yEmin=+100000000;
   stack[stackB++]=nc;

   nClusters=clusters.addCluster(maxCl);
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

// Determining of min.Xbin & max.Xbin for each line (y) in cluster
    Int_t nLind=nLmax-ny;
    if(nx<xMinCl[nLind]) xMinCl[nLind]=nx;
    if(nx>xMaxCl[nLind]) xMaxCl[nLind]=nx;
    if(ny < nLmin) nLmin=ny;

    Double_t wt=(Double_t)(tablBiN[-hitplot[nci]]-maxCl+1);
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
     if(hitplot[ncTs]>0) {
       if(tablBiN[hitplot[ncTs]]>=maxCl) {
         stack[stackB++]=ncTs;
         hitplot[ncTs]*=-1;
       }
     }
    }
   }
   //-End cluster------------------------------------------------------

   clst.setNMods(nMods);
   clst.setBinsWt(binsCl);
   clst.setNBinsCl(nBins);
   xmid=xmid/binsCl;
   ymid=ymid/binsCl;
   clst.setX(xmid);
   clst.setY(ymid);
   clst.setZ(Dp-Ap*xmid-Bp*ymid);
   clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
   clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
   clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
   clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
   clst.setZup( Dp-Bp*clst.getYup() );
   clst.setZdown( Dp-Bp*clst.getYdown() );
   dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
   dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
   clst.setErrX(dXmid);
   clst.setErrY(dYmid);
   clst.setErrZ( Bp*dYmid );

   Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
   Short_t numhitsC[24];
   for(Int_t n=12; n<24; n++) numhitsC[n]=0; //!!!
   Short_t numTimesC[24][40];
   for(Int_t mod=0; mod<nmo; mod++) {
    for(Int_t layer=0; layer<nla[mod]; layer++) {
     HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
     iLayer=mod*6+layer;
     numhitsC[iLayer]=0;
     for(Int_t i=0; i<nHits[iLayer]; i++) {
      if(hitsDel[iLayer][i]>-1) {
       cell=hits[iLayer][i];
       HMdcLookUpTbCell& pCell=fLayer[cell];
       yBinMax=pCell.getYBinMax();
       if( nLmin > yBinMax ) continue;
       yBinMin=pCell.getYBinMin();
//       if( nLmax < yBinMin ) break;            Hits not sorted !

       Int_t lMin, lMax;
       yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
       yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
       Bool_t indSetCell=kFALSE;
       for (Int_t nl=lMin; nl<=lMax; nl++) {
        Int_t nLind=nLmax-nl;
        if(pCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
         if(xMinCl[nLind] > pCell.xBinMax[nl-yBinMin]) continue;
	 indSetCell=kTRUE;
	 break;
        } else {
         if(xMaxCl[nLind] < pCell.xBinMin[nl-yBinMin]) continue;
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
	else printf("Error in setHitsToCluster: too many hits (>max)!\n");
       }
      }
     }
    }
   }
   clst.setCluster(numhitsC,  hitsC, numTimesC);
   if(stackB>stackSize) {
    Printf("Finding clusters: Error stack overflow!\n");
    break;
   }
  }
 }
//timer.Stop();
//Printf("findClusters: Cpu time for finder: %f",timer.CpuTime());
//timer.Start();
 for(Int_t i=0; i<stackT; i++) hitplot[stack[i]]=-hitplot[stack[i]];
 testClust(0, nClusters);

//timer.Stop();
//Printf("findClusters: Cpu time for Q.+rest+sort: %f",timer.CpuTime());
 return nClusters;
}


Int_t HMdcLookUpTbSec::testClust(Int_t nClFirst, Int_t nClusters) {
  //Test clusters-----------
// nClusters=clusters.Test(Ap,Bp,Dp);
  for(Int_t nClF=nClFirst; nClF<nClusters-1; nClF++) {
  HMdcCluster& pClF=clusters[nClF];
  for(Int_t nClS=nClF+1; nClS<nClusters; nClS++) {
   HMdcCluster& pClS=clusters[nClS];
//   if(pClF.getY()-pClS.getY() > 50.) break;            // 50. mm !???
   if(fabs(pClF.getY()-pClS.getY()) >= 10.) continue;  // 10. mm !???
   if(fabs(pClF.getX()-pClS.getX()) >= 40. continue;  // 40. mm !???
   if(pClS.Test(pClF,Ap,Bp,Dp,shX2,shY2)) {
    nClusters=clusters.deleteCluster(nClS);
    for(Int_t nClT=nClF-1; nClT>=0; nClT--){
     HMdcCluster& pClT=clusters[nClT];
//     if(pClT.getY()-pClF.getY() > 50.) break;            // 50. mm !???
//!!!!!!!!!!!!!!!!
     if(fabs(pClT.getY()-pClF.getY()) >= 10.) continue;  // 10. mm !???
     if(fabs(pClT.getX()-pClF.getX()) >= 40.) continue;  // 40. mm !???
     if(pClT.Test(pClF,Ap,Bp,Dp,shX2,shY2)) {
      nClusters=clusters.deleteCluster(nClT);
      nClF--;
      nClT=nClF;
     }
    }
    nClS=nClF;
   }
  }
 }
 clusters.calcxy(); //???? delaetsya sortirovka !!!

// clusters.Sort(); // Onli after calcxy!!!
 // ------------------------
 Short_t hits[24][250];
 for(Int_t l=0; l<24; l++) for(Int_t c=0; c<250; c++) hits[l][c]=0;
 for(Int_t nCl=0; nCl<nClusters; nCl++) {
  HMdcCluster& pCl=clusters[nCl];
  for(Int_t l=0; l<24; l++) {
   Int_t nWires=pCl.getNumHits(l);   // num. fired wires in this clusters for loc
   for(Int_t nW=0; nW<nWires; nW++) {
    Int_t nCell=pCl.getHit(l,nW);         // num. fired wire
    hits[l][nCell]++;
   }
  }
 }
 for(Int_t nCl=0; nCl<nClusters; nCl++) {
  Int_t nTotCell=0;
  Int_t nCellDb=0;
  HMdcCluster& pCl=clusters[nCl];
  for(Int_t l=nClFirst; l<24; l++) {
   Int_t nWires=pCl.getNumHits(l);   // num. fired wires in this clusters for loc
   for(Int_t nW=0; nW<nWires; nW++) {
    Int_t nCell=pCl.getHit(l,nW);         // num. fired wire
    nTotCell++;
    if(hits[l][nCell]>1) nCellDb++;
   }
  }
  Float_t prDb=((Float_t) nCellDb)/((Float_t) nTotCell);
  pCl.setPrDb(prDb);
 }
  return nClusters;
}
*/

void HMdcLookUpTbSec::findClusInSeg(Int_t seg, Short_t maxCl, Short_t *hPlot){
 if(maxCl<3) return;
 Int_t stackSize=(Int_t)(9950.*(Double_t(nBinX)/315.)*(Double_t(nBinX)/315.));
 Int_t stack[(stackSize*105)/100];   //?!!
 Int_t stackT,stackB;
 stackB=stackT=0;
 
 Int_t nmo=(seg+1)*2;
// if(nmo>2) nmo=2;
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
//Int_t maxLocal;
 //---Cluster finding-------------------------------------
 for(Int_t nc=size-1; nc>=0; nc--) {
   if(hPlot[nc]>0 && tablBiN[hPlot[nc]]>=maxCl) {
     if( tablBiN[hPlot[nc]&63] < maxClust[mod1] ) continue;
     if( tablBiN[hPlot[nc]&4032] < maxClust[mod2] ) continue;
//  maxLocal=imax-tablBiN[bMboard[nc]];
//  if(maxLocal<4) maxLocal=4;
//  maxCl=maxLocal;
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

// Determining of min.Xbin & max.Xbin for each line (y) in cluster
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
      if(hPlot[ncTs]>0) {
        if(tablBiN[hPlot[ncTs]]>=maxCl) {
          if( tablBiN[hPlot[ncTs]&63] < maxClust[mod1] ) continue;
          if( tablBiN[hPlot[ncTs]&4032] < maxClust[mod2] ) continue;
          stack[stackB++]=ncTs;
          hPlot[ncTs]*=-1;
        }
      }
    }
   }
   //-End cluster------------------------------------------------------

   clst.setNMods(nMods);
   clst.setBinsWt(binsCl);
   clst.setNBinsCl(nBins);
   xmid=xmid/binsCl;
   ymid=ymid/binsCl;
   clst.setX(xmid);
   clst.setY(ymid);
   clst.setZ(Dp-Ap*xmid-Bp*ymid);
   clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
   clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
   clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
   clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
   clst.setZup( Dp-Bp*clst.getYup() );
   clst.setZdown( Dp-Bp*clst.getYdown() );
   dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
   dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
   clst.setErrX(dXmid);
   clst.setErrY(dYmid);
   clst.setErrZ( Bp*dYmid );

   Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
   Short_t numhitsC[24];
   for(Int_t n=0; n<24; n++) numhitsC[n]=0; //!!!
   Short_t numTimesC[24][40];
   for(Int_t mod=seg*2; mod<nmo; mod++) {
    for(Int_t layer=0; layer<6; layer++) {
     HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
     iLayer=mod*6+layer;
     numhitsC[iLayer]=0;
     for(Int_t i=0; i<nHits[iLayer]; i++) {
      if(hitsDel[iLayer][i]>-1) {
       cell=hits[iLayer][i];
       HMdcLookUpTbCell& pCell=fLayer[cell];
       yBinMax=pCell.getYBinMax();
       if( nLmin > yBinMax ) continue;
       yBinMin=pCell.getYBinMin();
//       if( nLmax < yBinMin ) break;            Hits not sorted !

       Int_t lMin, lMax;
       yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
       yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
       Bool_t indSetCell=kFALSE;
       for (Int_t nl=lMin; nl<=lMax; nl++) {
        Int_t nLind=nLmax-nl;
        if(pCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
         if(xMinCl[nLind] > pCell.xBinMax[nl-yBinMin]) continue;
	 indSetCell=kTRUE;
	 break;
        } else {
         if(xMaxCl[nLind] < pCell.xBinMin[nl-yBinMin]) continue;
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
	else printf("Error in setHitsToCluster: too many hits (>max)!\n");
       }
      }
     }
    }
   }
   clst.setCluster(numhitsC,  hitsC, numTimesC);
   if(stackB>stackSize) {
    Printf("Finding clusters: Error stack overflow!\n");
    break;
   }
  }
 }
 for(Int_t i=0; i<stackT; i++) hPlot[stack[i]]=-hPlot[stack[i]];
 testClust(nClustFirst, nClusters);

//timer.Stop();
//Printf("findClusters: Cpu time for Q.+rest+sort: %f",timer.CpuTime());
}

void HMdcLookUpTbSec::findClusInSec(Short_t maxCl){
 if(maxCl<3) return;
 Int_t stackSize=(Int_t)(9950.*(Double_t(nBinX)/315.)*(Double_t(nBinX)/315.));
 Int_t stack[(stackSize*105)/100];   //?!!
 Int_t stackT,stackB;
 stackB=stackT=0;
 
// Int_t nmo=(seg+1)*2;
// if(nmo>2) nmo=2;
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
   if(hitplot[nc]>0 && (tablBiN[hitplot[nc]]+tablBiN[hitplotS2[nc]])>=maxCl) {
     if( tablBiN[hitplot[nc]&63] < maxClust[0] ) continue;
     if( tablBiN[hitplot[nc]&4032] < maxClust[1] ) continue;
     if( tablBiN[hitplotS2[nc]&63] < maxClust[2] ) continue;
     if( tablBiN[hitplotS2[nc]&4032] < maxClust[3] ) continue;
//  maxLocal=imax-tablBiN[bMboard[nc]];
//  if(maxLocal<4) maxLocal=4;
//  maxCl=maxLocal;
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

// Determining of min.Xbin & max.Xbin for each line (y) in cluster
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
     if(hitplot[ncTs]>0) {
       if((tablBiN[hitplot[ncTs]]+tablBiN[hitplotS2[ncTs]]) >= maxCl) {
         if( tablBiN[hitplot[ncTs]&63] < maxClust[0] ) continue;
         if( tablBiN[hitplot[ncTs]&4032] < maxClust[1] ) continue;
         if( tablBiN[hitplotS2[ncTs]&63] < maxClust[2] ) continue;
         if( tablBiN[hitplotS2[ncTs]&4032] < maxClust[3] ) continue;
         stack[stackB++]=ncTs;
         hitplot[ncTs]*=-1;
       }
     }
    }
   }
   //-End cluster------------------------------------------------------

   clst.setNMods(nMods);
   clst.setBinsWt(binsCl);
   clst.setNBinsCl(nBins);
   xmid=xmid/binsCl;
   ymid=ymid/binsCl;
   clst.setX(xmid);
   clst.setY(ymid);
   clst.setZ(Dp-Ap*xmid-Bp*ymid);
   clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
   clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
   clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
   clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
   clst.setZup( Dp-Bp*clst.getYup() );
   clst.setZdown( Dp-Bp*clst.getYdown() );
   dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
   dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
   clst.setErrX(dXmid);
   clst.setErrY(dYmid);
   clst.setErrZ( Bp*dYmid );

   Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
   Short_t numhitsC[24];
   for(Int_t n=0; n<24; n++) numhitsC[n]=0; //!!!
   Short_t numTimesC[24][40];
   for(Int_t mod=0; mod<4; mod++) {
    for(Int_t layer=0; layer<6; layer++) {
     HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
     iLayer=mod*6+layer;
     numhitsC[iLayer]=0;
     for(Int_t i=0; i<nHits[iLayer]; i++) {
      if(hitsDel[iLayer][i]>-1) {
       cell=hits[iLayer][i];
       HMdcLookUpTbCell& pCell=fLayer[cell];
       yBinMax=pCell.getYBinMax();
       if( nLmin > yBinMax ) continue;
       yBinMin=pCell.getYBinMin();
//       if( nLmax < yBinMin ) break;            Hits not sorted !

       Int_t lMin, lMax;
       yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
       yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
       Bool_t indSetCell=kFALSE;
       for (Int_t nl=lMin; nl<=lMax; nl++) {
        Int_t nLind=nLmax-nl;
        if(pCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
         if(xMinCl[nLind] > pCell.xBinMax[nl-yBinMin]) continue;
	 indSetCell=kTRUE;
	 break;
        } else {
         if(xMaxCl[nLind] < pCell.xBinMin[nl-yBinMin]) continue;
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
	else printf("Error in setHitsToCluster: too many hits (>max)!\n");
       }
      }
     }
    }
   }
   clst.setCluster(numhitsC,  hitsC, numTimesC);
   if(stackB>stackSize) {
    Printf("Finding clusters: Error stack overflow!\n");
    break;
   }
  }
 }
 for(Int_t i=0; i<stackT; i++) hitplot[stack[i]]=-hitplot[stack[i]];
 testClust(nClustFirst, nClusters);

}

void HMdcLookUpTbSec::testClust(Int_t nFirstClus, Int_t nLastClus) {
//Test clusters-----------
// nClusters=clusters.Test(Ap,Bp,Dp);
  for(Int_t nClF=nFirstClus; nClF<nLastClus-1; nClF++) {
    HMdcCluster& pClF=clusters[nClF];
    for(Int_t nClS=nClF+1; nClS<nLastClus; nClS++) {
      HMdcCluster& pClS=clusters[nClS];
//      if(pClF.getY()-pClS.getY() > 50.) break;            // 50. mm !???
      if(fabs(pClF.getY()-pClS.getY()) >= 10.) continue;  // 10. mm !???
      if(fabs(pClF.getX()-pClS.getX()) >= 40.) continue;  // 30. mm !???
      if(pClS.Test(pClF,Ap,Bp,Dp,shX2,shY2)) {
        nClusters=clusters.deleteCluster(nClS);
        nLastClus--;
        for(Int_t nClT=nClF-1; nClT>=0; nClT--){
          HMdcCluster& pClT=clusters[nClT];
//          if(pClT.getY()-pClF.getY() > 50.) break;            // 50. mm !???
          if(fabs(pClT.getY()-pClF.getY()) >= 10.) continue;  // 10. mm !???
          if(fabs(pClT.getX()-pClF.getX()) >= 40.) continue;  // 30. mm !???
          if(pClT.Test(pClF,Ap,Bp,Dp,shX2,shY2)) {
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

void HMdcLookUpTbSec::calcQualClust() {
  clusters.calcxy(); //???? delaetsya sortirovka !!!
// clusters.Sort(); // Onli after calcxy!!!
// ------------------------
  Short_t hits[24][250];
  for(Int_t l=0; l<24; l++) for(Int_t c=0; c<250; c++) hits[l][c]=0;
  for(Int_t nCl=0; nCl<nClusters; nCl++) {
    HMdcCluster& pCl=clusters[nCl];
    for(Int_t l=0; l<24; l++) {
      Int_t nWires=pCl.getNumHits(l);   // num. fired wires in this clusters for loc
      for(Int_t nW=0; nW<nWires; nW++) {
        Int_t nCell=pCl.getHit(l,nW);         // num. fired wire
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
        Int_t nCell=pCl.getHit(l,nW);         // num. fired wire
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
 Int_t stackSize=(Int_t)(9950.*(Double_t(nBinX)/315.)*(Double_t(nBinX)/315.));
 Int_t stack[(stackSize*105)/100];   //?!!
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

//Int_t maxLocal;
 //---Cluster finding-------------------------------------
 for(Int_t nc=size-1; nc>=0; nc--) {
   if(hPlot[nc]>0 && tablBiN[hPlot[nc]&mask]>=maxCl) {
//  maxLocal=imax-tablBiN[bMboard[nc]];
//  if(maxLocal<4) maxLocal=4;
//  maxCl=maxLocal;
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

// Determining of min.Xbin & max.Xbin for each line (y) in cluster
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
     if(hPlot[ncTs]>0) {
       if(tablBiN[hPlot[ncTs]&mask]>=maxCl) {
         stack[stackB++]=ncTs;
         hPlot[ncTs]*=-1;
       }
     }
    }
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
   clst.setZ(Dp-Ap*xmid-Bp*ymid);
   clst.setXright( (Double_t)(xEmin)*sh_x+xLow );
   clst.setXleft(  (Double_t)(xEmax)*sh_x+xLow+sh_x );
   clst.setYup( (Double_t)(yEmax)*sh_y+yLow+sh_y );
   clst.setYdown( (Double_t)(yEmin)*sh_y+yLow );
   clst.setZup( Dp-Bp*clst.getYup() );
   clst.setZdown( Dp-Bp*clst.getYdown() );
   dXmid=sqrt(dXmid/binsCl-xmid*xmid + shX2*shX2);
   dYmid=sqrt(dYmid/binsCl-ymid*ymid + shY2*shY2);
   clst.setErrX(dXmid);
   clst.setErrY(dYmid);
   clst.setErrZ( Bp*dYmid );

   Short_t hitsC[24][40]; //!!!!!!!!!!!!!!!! [20];
   Short_t numhitsC[24];
   for(Int_t n=0; n<24; n++) numhitsC[n]=0; //!!!
   Short_t numTimesC[24][40];
    for(Int_t layer=0; layer<6; layer++) {
     HMdcLookUpTbLayer& fLayer=(*this)[mod][layer];
     iLayer=mod*6+layer;
     numhitsC[iLayer]=0;
     for(Int_t i=0; i<nHits[iLayer]; i++) {
      if(hitsDel[iLayer][i]>-1) {
       cell=hits[iLayer][i];
       HMdcLookUpTbCell& pCell=fLayer[cell];
       yBinMax=pCell.getYBinMax();
       if( nLmin > yBinMax ) continue;
       yBinMin=pCell.getYBinMin();
//       if( nLmax < yBinMin ) break;            Hits not sorted !

       Int_t lMin, lMax;
       yBinMax < nLmax ? lMax=yBinMax : lMax=nLmax;
       yBinMin > nLmin ? lMin=yBinMin : lMin=nLmin;
       Bool_t indSetCell=kFALSE;
       for (Int_t nl=lMin; nl<=lMax; nl++) {
        Int_t nLind=nLmax-nl;
        if(pCell.xBinMin[nl-yBinMin] < xMinCl[nLind]) {
         if(xMinCl[nLind] > pCell.xBinMax[nl-yBinMin]) continue;
	 indSetCell=kTRUE;
	 break;
        } else {
         if(xMaxCl[nLind] < pCell.xBinMin[nl-yBinMin]) continue;
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
	else printf("Error in setHitsToCluster: too many hits (>max)!\n");
       }
      }
     }
    }
   clst.setCluster(numhitsC,  hitsC, numTimesC);
   if(stackB>stackSize) {
    Printf("Finding clusters: Error stack overflow!\n");
    break;
   }
  }
 }
 for(Int_t i=0; i<stackT; i++) hPlot[stack[i]]=-hPlot[stack[i]];
 testClust(nClustFirst, nClusters);

}

void HMdcLookUpTbSec::printClusters(){
 clusters.print();
}


//---------------------------------------------------------
HMdcLookUpTb::HMdcLookUpTb(Int_t inBinX, Bool_t isCoilOff) :
                                                            HParSet() {
  // constructor creates an array of pointers of type
  // HMdcLookUpTbSec
  // The container name is set to "MdcLookUpTb"
  Int_t nBinX=inBinX;
  if(nBinX<100) nBinX=319;
  Int_t nBinY=(nBinX*278)/100;
  fName="MdcLookUpTb";
  fTitle="Look Up Table for Mdc1,2 track finding";
  strcpy(detName,"Mdc");
  if(gHades) gHades->getRuntimeDb()->addContainer(this);

  array = new TObjArray(6);
  HMdcCalcLookUpTb* pCalc=new HMdcCalcLookUpTb(isCoilOff);

  for (Int_t nSec = 0; nSec < 6; nSec++) {
    if(pCalc->isCellProj(nSec)) {
      Int_t nSegs=(pCalc->isCellProj(nSec,2)||pCalc->isCellProj(nSec,3)) ? 2:1;
      (*array)[nSec] = new HMdcLookUpTbSec(nSec,nSegs,nBinX,nBinY);
      if(! pCalc->calcLookUpTb(nSec, this)) {
        isContainer=kFALSE;
        delete pCalc;
        return;
      }
    }
  }
  isContainer=kTRUE;
  delete pCalc;
}

HMdcLookUpTb* HMdcLookUpTb::getExObject() {
  HMdcLookUpTb* flookup=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) flookup=(HMdcLookUpTb*)rdb->getContainer("MdcLookUpTb");
    if( flookup && !((HParSet*)flookup)->init() ) return 0;
  }
  return flookup;
}

HMdcLookUpTb* HMdcLookUpTb::getObject() {
  HMdcLookUpTb* flookup=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) flookup=(HMdcLookUpTb*)rdb->getContainer("MdcLookUpTb");
    if( !flookup ) flookup=new HMdcLookUpTb();
    if( !((HParSet*)flookup)->init() ) return 0;
  }
  return flookup;
}

HMdcLookUpTb::~HMdcLookUpTb() {
  // destructor
  array->Delete();
}

Int_t HMdcLookUpTb::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Int_t HMdcLookUpTb::write(HParIo* output) {
  // writes the container to an output ???
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcLookUpTb::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
   if( (*array)[s] ) {
    HMdcLookUpTbSec& sec=(*this)[s];
    sec.clearwk();
   }
  }
  status=kFALSE;
  resetInputVersions();
}

void HMdcLookUpTb::clearwk() {
  for(Int_t s=0;s<6;s++) {
   if( (*array)[s] ) {
    HMdcLookUpTbSec& sec=(*this)[s];
    sec.clearwk();
   }
  }
}

void HMdcLookUpTb::printClusters(Int_t nSec=-1){
  Int_t s1=0;
  Int_t s2=5;
  if( nSec>=0 && nSec<=5 ) s1=s2=nSec;
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

TH2C* HMdcLookUpTbSec::fillTH2C(Char_t* name, Char_t* title, Int_t type=0){
 if(!hist) {
//  TH2C* hist=new TH2C(name,title,nBinX,xLow,xUp,nBinY,yLow,yUp);
   hist=new TH2C(name,title,nBinX,xLow,xUp,nBinY,yLow,yUp);
 }
 else {
   hist->Reset();
   hist->SetName(name);
   hist->SetTitle(title);
 }
 hist->SetMaximum(6.);
 hist->SetMinimum(0.);
 hist->Fill(0.,0.,0);
 for (Int_t nBin=0; nBin<size; nBin++) {
   if( hitplot[nBin]==0 && (!hitplotS2 || hitplotS2[nBin]==0) ) continue;
   Int_t ny=nBin/nBinX;
   Int_t nx=nBin%nBinX;
   Int_t color=0;
   if(type==1) {
     color=tablBiN[hitplot[nBin]&4032]; // 4032=111111000000b
     if(color) color+=6;
     color+=tablBiN[hitplot[nBin]&63];        //   63=000000111111b
     if(hitplotS2) {
       Int_t cl=tablBiN[hitplotS2[nBin]&4032]; // 4032=111111000000b
       if(cl) cl+=cl+6;
       cl+=tablBiN[hitplotS2[nBin]&63];        //   63=000000111111b
       if(cl) color+=12;
     }
   } else if(type==2) {
     if(hitplot[nBin]&4032) color=2;
     else if(hitplot[nBin]&63) color=1;
     if(hitplotS2) {
       if(hitplotS2[nBin]&4032) color=4;
       else if(hitplotS2[nBin]&63) color=3;
     }
   } else {
     color=tablBiN[hitplot[nBin]];
     if(hitplotS2) color+=tablBiN[hitplotS2[nBin]];
   }
   hist->Fill((Float_t)nx*sh_x+sh_x*0.5+xLow,
              (Float_t)ny*sh_y+sh_y*0.5+yLow, color);
 }
 return hist;
}

//----------------------------------------------------------
HMdcCalcLookUpTb::HMdcCalcLookUpTb(Bool_t isCoilOff) {
  cellsProjec=new HMdc12CellProjec(isCoilOff);
}

HMdcCalcLookUpTb::~HMdcCalcLookUpTb() {
  delete cellsProjec;
}

Bool_t HMdcCalcLookUpTb::isCellProj(Int_t sec) {
  if( ! cellsProjec->init() ) return kFALSE;
  if( !&((*cellsProjec)[sec]) ) return kFALSE;
  return kTRUE;
}

Bool_t HMdcCalcLookUpTb::isCellProj(Int_t sec, Int_t mod) {
  if(!isCellProj(sec)) return kFALSE;
  if( !&((*cellsProjec)[sec][mod]) ) return kFALSE;
  return kTRUE;
}

Bool_t HMdcCalcLookUpTb::calcLookUpTb(Int_t nSec, HMdcLookUpTb* fLookUpTb) {

  if ( ! cellsProjec->init() ) return kFALSE;

  HMdcLookUpTbSec& fLUpTbSec=(*fLookUpTb)[nSec];
  Int_t nBinX=fLUpTbSec.getNBinX();
  Int_t nBinY=fLUpTbSec.getNBinY();	
//Sectors:
  Double_t yUp,yLow,xUp,xLow;
  yUp=xUp=-1.0e+30;
  yLow=xLow=1.0e+30;

  HMdc12CellProjecSec& p12CPSec = (*cellsProjec)[nSec];
 //---------------------------------
//Modules:
  for(Int_t nMod=0; nMod<4; nMod++) {
    if( !&(p12CPSec[nMod]) ) continue;
//Layers:
    for(Int_t nLayer=0; nLayer<6; nLayer++) {
      HMdc12CellProjecLayer& p12CPLayer = p12CPSec[nMod][nLayer];
      Int_t nCells=p12CPLayer.getSize();
//Cells:
      for(Int_t nCell=0; nCell<nCells; nCell++) {
        HMdc12CellProjecCell& p12CPCell=p12CPLayer[nCell];
        if(p12CPCell.isCell()) {
          HMdcTrapPlane& cellPro=p12CPCell.cellProjec();
          for(Int_t i=0; i<4; i++) {
   	    if( yLow > cellPro[i].gety() ) yLow=cellPro[i].gety();
   	    if( yUp  < cellPro[i].gety() ) yUp =cellPro[i].gety();
   	    if( xLow > cellPro[i].getx() ) xLow=cellPro[i].getx();
   	    if( xUp  < cellPro[i].getx() ) xUp =cellPro[i].getx();
          }
        }
      }
    }
  }
  Double_t sh_x=(xUp-xLow)/(Double_t)(nBinX-3);
  Double_t sh_y=(yUp-yLow)/(Double_t)(nBinY-3);
  yLow=yLow-sh_y*1.5;                  //mast be bin=0 at limits of plot
  yUp=yUp+sh_y*1.5;
  xLow=xLow-sh_x*1.5;
  xUp=xUp+sh_x*1.5;


  fLUpTbSec.setParPlane(p12CPSec.getSys());
  fLUpTbSec.setTargetF(p12CPSec.getTargetF());
  fLUpTbSec.setTargetL(p12CPSec.getTargetL());
//Init look up table for one sector:
  fLUpTbSec.setEdgesPlot(xLow,xUp,yLow,yUp);
  for(Int_t nMod=0; nMod<4; nMod++) {
    if( !&(p12CPSec[nMod]) ) continue;
    for(Int_t nLayer=0; nLayer<6; nLayer++) {
      HMdc12CellProjecLayer& p12CPLayer = p12CPSec[nMod][nLayer];
      Int_t nCells=p12CPLayer.getSize();
      if(nCells<1) continue;

      HMdcLookUpTbLayer& fLUpTbLayer=fLUpTbSec[nMod][nLayer];
//Cells:
      for(Int_t nCell=0; nCell<nCells; nCell++) {
        HMdc12CellProjecCell& p12CPCell = p12CPLayer[nCell];
        Int_t yBinMinT,yBinMaxT;
        if(p12CPCell.isCell()) {
          HMdcTrapPlane& cellPro=p12CPCell.cellProjec();
//Lines 0,1,2,3 - lines from points 0-1, 1-2, 2-3, 3-0 respectively
          yBinMinT=0;
          yBinMaxT=0;
          for(Int_t i=1; i<4; i++) {
            if(cellPro[yBinMinT].gety() > cellPro[i].gety()) yBinMinT=i;
	    if(cellPro[yBinMaxT].gety() < cellPro[i].gety()) yBinMaxT=i;
          }
          yBinMinT=(Int_t)((cellPro[yBinMinT].gety()-yLow)/sh_y);
          yBinMaxT=(Int_t)((cellPro[yBinMaxT].gety()-yLow)/sh_y);

          fLUpTbLayer.initCell(nCell,yBinMinT,yBinMaxT);
          HMdcLookUpTbCell& fLUpTbCell=fLUpTbLayer[nCell];
          Int_t nx1t,nx2t;
          nx1t=+1000000000;
          nx2t=-1000000000;
          for(Int_t ny=yBinMinT; ny<yBinMaxT; ny++) {

            Double_t y=(ny+1)*sh_y+yLow;
            Double_t x1,x2;
            calcX(cellPro, y, x1, x2);	
            Int_t nx1i=(Int_t)((x1-xLow)/sh_x);
	    Int_t nx2i=(Int_t)((x2-xLow)/sh_x);
	    if(nx1i < nx1t) nx1t=nx1i;
	    if(nx2i > nx2t) nx2t=nx2i;
//	    Int_t nc1i=ny*nBinX + nx1t;
//	    Int_t nc2i=ny*nBinX + nx2t;
//Fill look up table:
//	    fLUpTbCell.addLine(nc1i,nc2i);
            fLUpTbCell.addLine(nx1t,nx2t);
	    if(ny==yBinMaxT-1 && yBinMinT!=yBinMaxT) {
//	      fLUpTbCell.addLine(nc1i+nBinX,nc2i+nBinX);
              fLUpTbCell.addLine(nx1t,nx2t);
            }
	    nx1t=nx1i;
	    nx2t=nx2i;	
          }
        }
      }
    }
  }
  return kTRUE;
}

void HMdcCalcLookUpTb::calcX(HMdcTrapPlane& pr, Double_t y,
                           Double_t &xLow, Double_t &xUp) {
 xLow=1.0e+30;
 xUp=-1.0e+30;
 for(Int_t i=0; i<4; i++){
  Int_t u1=i;
  Int_t u2=(u1<3) ? u1+1:0;
  if(fabs(pr[u1].gety()-pr[u2].gety())>1.e-4){
   if(pr[u1].getx()>pr[u2].getx()) {
    Int_t u=u1;  u1=u2;  u2=u;
   }
   Double_t x=(y-pr[u1].gety())/(pr[u1].gety()-pr[u2].gety())*
             (pr[u1].getx()-pr[u2].getx())+pr[u1].getx();
   if(x>=pr[u1].getx() && x<=pr[u2].getx()) {
    if(x<xLow) xLow=x;
    if(x>xUp) xUp=x;
   }
  }
 }
//Attention pr[0]&pr[1] mast be >= pr[2]&pr[3] (0-3; 1-2)!!!!
 if(xLow<-2000) xLow=(pr[2].getx() < pr[3].getx()) ? pr[2].getx():pr[3].getx();
 if(xUp> 2000) xUp=(pr[0].getx() > pr[1].getx()) ? pr[0].getx():pr[1].getx();
}