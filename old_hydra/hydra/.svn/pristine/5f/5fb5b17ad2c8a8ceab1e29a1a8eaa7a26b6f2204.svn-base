//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 20/05/2000 by V.Pechenov
//*-- Modified : 26/10/99 by V.Pechenov
//*-- Modified : 25/02/99


////////////////////////////////////////////////////////////////
// HMdc12Clusters
//
// Container class keep cluster information.
//
////////////////////////////////////////////////////////////////

#include "hmdc12clusters.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>
#include "TMath.h"
#include "hmdcgeomobj.h"

ClassImp(HMdcCluster)
ClassImp(HMdc12Clusters)

HMdc12Clusters::HMdc12Clusters() {
 clusters=new TObjArray(10);
 nClusters=0;
}

HMdc12Clusters::~HMdc12Clusters() {
 clusters->Delete();
}

Int_t HMdc12Clusters::addCluster(Int_t *maxCl) {
 clusters->AddLast(new HMdcCluster(maxCl));
 nClusters=clusters->GetEntries();
 return  nClusters;
}

void HMdc12Clusters::clear() {
 clusters->Delete();
 nClusters=clusters->GetEntries();
}

Int_t HMdc12Clusters::deleteCluster(Int_t nClust) {
// Output - number of clusters after delet.
 delete (*clusters)[nClust]; 
 clusters->RemoveAt(nClust); 
 clusters->Compress();
 nClusters=clusters->GetEntries();
 return nClusters;
}

void HMdc12Clusters::calcxy(){ 
 for(Int_t ncl=0; ncl<nClusters; ncl++) {
  HMdcCluster& cls=(*this)[ncl];
//  Double_t xmid=cls.getX()/cls.getNBins();
//  Double_t ymid=cls.getY()/cls.getNBins();
//  Double_t dXmid=sqrt(cls.getErrX()/cls.getNBins()-xmid*xmid);
//  Double_t dYmid=sqrt(cls.getErrY()/cls.getNBins()-ymid*ymid); 
//??????  HGeomVector point(xmid,ymid,0.0);
//  point.setX(xmid);
//  point.setY(ymid);
//  point.setZ(0.0);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  
//????  point=system.transFrom(point);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  
//????  cls.setX(point.getX());
//????  cls.setY(point.getY());
//????  cls.setZ(point.getZ());
/*  
  point.setX(xmid+dXmid);
  point.setY(ymid+dYmid);
  point.setZ(0.0);
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  
  point=system.transFrom(point);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   
  cls.setErrX(point.getX()-cls.getX());
  cls.setErrY(point.getY()-cls.getY());
Printf("CalcTr: dXmid=%e-%e=%e",point.getX(),cls.getX(),point.getX()-cls.getX());  
Printf("CalcTr: dYmid=%e-%e=%e",point.getY(),cls.getY(),point.getY()-cls.getY());    
*/
  cls.calcQuality();
 }
}


void HMdcCluster::calcQuality(){ 
  Short_t ni,n[4];
  Int_t ind[40];
  Short_t tmpC[40],tmpT[40];
  for(Int_t nm=0; nm<4; nm++) {
   n[0]=n[1]=n[2]=n[3]=0;
   for(Int_t nli=0; nli<6; nli++) {
    Int_t nl=nm*6+nli;
    if(numhits[nl]==0) continue;
    ni=0;
    TMath::Sort(numhits[nl],hits[nl],ind,kFALSE);
    for(Int_t nh=0; nh<numhits[nl]; nh++) {
     tmpC[nh]=hits[nl][ind[nh]];
     tmpT[nh]=numTimes[nl][ind[nh]];
     ni+=numTimes[nl][nh];
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  if(numTimes[nl][nh]==3) ni=ni-2;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!     
    }
    for(Int_t nh=0; nh<numhits[nl]; nh++) {
      hits[nl][nh]=tmpC[nh];
      numTimes[nl][nh]=tmpT[nh];
    }
    if(ni>4) ni=4;
    for(Int_t i=0; i<ni; i++) n[i]++;
    qualityClusters[nm]=1;
    if(n[1]>=4) qualityClusters[nm]=2; 
    if(n[2]>=4) qualityClusters[nm]=3;
    if(n[3]>=4) qualityClusters[nm]=4;
   }
  }
}

void HMdc12Clusters::print(){ 
 for(Int_t ncl=0; ncl<nClusters; ncl++) {
  HMdcCluster& cls=(*this)[ncl];
  printf("-Cluster %i: ",ncl+1);
  cls.print();
 }
}

Bool_t HMdcCluster::Test(HMdcCluster& clst, HMdcPrPlane& prPlane, 
      Double_t shX2, Double_t shY2) {
  if(mod != clst.mod) return kFALSE;
  Int_t nIdent[4];
  nIdent[0]=nIdent[1]=nIdent[2]=nIdent[3]=0;
  Char_t imax[4];
  Char_t newMax[4];
  for(Int_t nm=0; nm<4; nm++) {
    if(clst.maxClus[nm] < maxClus[nm]) {
      imax[nm]=clst.maxClus[nm];
      newMax[nm]=maxClus[nm];
    } else {
      imax[nm]=maxClus[nm];
      newMax[nm]=clst.maxClus[nm];
    }
  }
  Int_t maxCr=0;
  Int_t m1=0;
  Int_t m2=3;
  if(mod>=0) m1=m2=mod;
  for(Int_t nm=m1; nm<=m2; nm++) {
    for(Int_t nl=m1*6; nl<(m2+1)*6; nl++) {    // for 1 MDC we need in ...!!!!
      Int_t isId=0;
      for(Int_t nh1=0; nh1<numhits[nl]; nh1++){
        for(Int_t nh2=0; nh2<clst.numhits[nl]; nh2++){
          if(hits[nl][nh1]==clst.hits[nl][nh2]) isId++;
        }
      }
      if(isId>0) nIdent[nm]++;
      if(nIdent[nm] > 3 || (nIdent[nm]>0 && nIdent[nm]>=imax[nm]) ) {
        maxCr++;
        break;
      }
    }
  }
//  if( (maxClus<=6 && maxCr >=1) || (maxCr == 2) ) {
  if( maxCr == nMods ) {
    for(Int_t nl=0; nl<24; nl++) {    // for 1 MDC we need in ...!!!!
      for(Int_t nh=0; nh<numhits[nl]; nh++){
        clst.setHitToCluster(nl,hits[nl][nh],numTimes[nl][nh]);
      }
    }
    Double_t tmpX,tmpY;
    Double_t nEvWt=clst.nBinsInCluster+nBinsInCluster;
    tmpX=(clst.dXmid*clst.dXmid - shX2*shX2 +
          clst.xmid*clst.xmid)*clst.nBinsInCluster;
    tmpX+=(dXmid*dXmid + xmid*xmid)*nBinsInCluster;
    tmpY=(clst.dYmid*clst.dYmid + clst.ymid*clst.ymid)*clst.nBinsInCluster;
    tmpY+=(dYmid*dYmid - shY2*shY2 + 
           ymid*ymid)*nBinsInCluster;
    clst.xmid=(clst.xmid*clst.nBinsInCluster+xmid*nBinsInCluster)/nEvWt; 
    clst.ymid=(clst.ymid*clst.nBinsInCluster+ymid*nBinsInCluster)/nEvWt;
    clst.zmid=prPlane.getZOnPlane(xmid,ymid);
    clst.dXmid=sqrt(tmpX/nEvWt - clst.xmid*clst.xmid + shX2*shX2);
    clst.dYmid=sqrt(tmpY/nEvWt - clst.ymid*clst.ymid + shY2*shY2);
    clst.dZmid=prPlane.B()*dYmid;
    clst.xRight > xRight ? clst.xRight=xRight : clst.xRight;
    clst.xLeft < xLeft ? clst.xLeft=xLeft : clst.xLeft;
    clst.yDown > yDown ? clst.yDown=yDown : clst.yDown;
    clst.yUp < yUp ? clst.yUp=yUp : clst.yUp; 
    clst.zUp=prPlane.getZOnPlane(0.,clst.yUp);
    clst.zDown=prPlane.getZOnPlane(0.,clst.yDown);
    clst.nBinsInCluster=nEvWt;
    for(Int_t m=0; m<4; m++) clst.maxClus[m]=newMax[m];
    return kTRUE;
  }
  return kFALSE;
}


HMdcCluster::HMdcCluster() {
 xmid=ymid=dXmid=dYmid=0.;
 maxClus[0]=maxClus[1]=maxClus[2]=maxClus[3]=0;
 nBinsInCluster=0;
 nMods=2;
 mod=-1;
 for(Int_t i=0; i<24; i++) numhits[i]=0;
}

HMdcCluster::HMdcCluster(Int_t *maxCl) {
 xmid=ymid=dXmid=dYmid=0.;
 for(Int_t m=0; m<4; m++) maxClus[m]=maxCl[m];
 nBinsInCluster=0;
 nMods=2;
 mod=-1;
 for(Int_t i=0; i<24; i++) numhits[i]=0;
}

void HMdcCluster::print(){
  printf("nMods=%i, ",nMods);
  if(mod >= 0) printf("Module %i, ",mod+1);
  printf("<x>=%.1f+/-%.1f <y>=%.1f+/-%.1f z=%.1f+/-%.1f\n",
  xmid,dXmid,ymid,dYmid,zmid,dZmid);
  printf("Quality[mod.]=%i-%i-%i-%i; Max.[mod.]=%i-%i-%i-%i\n",
    qualityClusters[0],qualityClusters[1],qualityClusters[2],qualityClusters[3],
    maxClus[0],maxClus[1],maxClus[2],maxClus[3]);
  for(Int_t nl=0; nl<24; nl++) {
    Int_t m=nl/6; Int_t l=nl%6;
    if(numhits[nl]>0) {
      printf("Mod.%i Layer%i  %icells:",m+1,l+1,numhits[nl]);
      for(Int_t h=0; h<numhits[nl]; h++)
        printf(" %i(%i)",hits[nl][h]+1,numTimes[nl][h]);
      printf("\n");
    }
  }
}

void HMdcCluster::setHitToCluster(Int_t nl, Short_t ncell, Short_t nTm){
 Int_t ind=1;
 Int_t nhi=numhits[nl];
 for(Int_t i=0; i<nhi; i++) {
  if(hits[nl][i]==ncell) ind=0;
 }
 if(ind==1) {
  if(nhi<39) {                        //  19) {
   hits[nl][nhi]=ncell;
   numTimes[nl][nhi]=nTm;
   numhits[nl]++;
  }
  else printf("Error! setHitsToCluster: very mach hits (>max)!!!\n");
 }
}

void HMdcCluster::setCluster(Short_t numhitsI[],  Short_t hitsI[][40], Short_t numTimesI[][40]){
 for(Int_t i=0; i<24; i++) {
  numhits[i]=numhitsI[i];
  for(Int_t nH=0; nH<((Int_t)numhits[i]); nH++) {
   hits[i][nH]=hitsI[i][nH];
   numTimes[i][nH]=numTimesI[i][nH];
  }
 }
}


Int_t HMdcCluster::Compare(TObject *obj){
 Int_t q1=qualityClusters[0]+qualityClusters[1];
 Int_t q2=((HMdcCluster*)obj)->qualityClusters[0]+
          ((HMdcCluster*)obj)->qualityClusters[1];
  if (q1 > q2) return 1;
  else if (q1 < q2) return -1;
  else {
   if(ymid < (((HMdcCluster*)obj)->ymid)) return 1;
   else return -1;
  }
}
