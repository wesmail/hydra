//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 05/06/2001 by V.Pechenov
//*-- Modified : 21/08/2000 by V.Pechenov
//*-- Modified : 20/05/2000 by V.Pechenov
//*-- Modified : 26/10/99 by V.Pechenov
//*-- Modified : 25/02/99


////////////////////////////////////////////////////////////////
// HMdcClust
//
//  Container class keep cluster information:
//   x,y - position of cluster on the project plane HMdcPrPlane
//   xTag,yTag,zTarg - target, or point on the kick plane
//   indexPar - index(loc[2]-in cat.!) of parent (for Seg.1 = -1)
//   indexChild[2] - region of indexes of childs (for Seg.2 = -1,-2)
//
//  Int_t getIndexParent(void) {return indexPar;}  
//  void  getIndexRegChilds(Int_t& first, Int_t& last)
//  Int_t getNextIndexChild(Int_t ind)
//    Example:
//      Int_t ind=-1;
//      while(ind=getNextIndexChild(ind)>0) {
//        ...
//      }
//
//  HMdcClusFilter class used for filtering of HMdcClus category.
//   The flag "status" in HMdcClus is used for filtering.
//   If  status=kFALSE the object will removed via category->filter(...)
//   Check this status (getStatus()) at the geting of container!
//
////////////////////////////////////////////////////////////////

#include "hmdcclus.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>
#include "TMath.h"

ClassImp(HMdcClus)
ClassImp(HMdcClusFilter)

void HMdcClus::clear() {
  HMdcList12GroupCells::clear();
  status=kTRUE;
  indexPar=indexChild[0]=-1;
  indexChild[1]=-2;
  typeClFinder=0;
  mod=-1;
}

void HMdcClus::print() {
  printPos();
  printCont();
}

void HMdcClus::printPos() {
  printf("Cluster: Sec.%i Seg.%i Index=%i.",sec+1,seg+1,index);
  if(indexPar>=0) Printf("  Index of parent=%i",indexPar);
  else if(indexChild[0]>=0 && indexChild[1]>=0)
    Printf("  Indexes of childs = %i - %i",indexChild[0],indexChild[1]);
  else printf("\n");
  Printf("  Level %i:%i, %i bins/cluster, %i hits/cluster",
      minCl[0],minCl[1],nBins,getNCells());
  Printf("  <x>=%g+/-%g <y>=%g+/-%g <z>=%g",x,errX,y,errY,getZ());
  Printf("  xTarg=%g+/-%g yTarg=%g+/-%g zTarg=%g+/-%g",
      xTarg,errXTarg,yTarg,errYTarg,zTarg,errZTarg);
  if(!status) Printf("  Cluster was deleted !.");
}

void HMdcClus::printCont() {
  for(Int_t lay=0; lay<12; lay++) {
    Int_t mod=seg*2+lay/6;
    Int_t nCells=getNCells(lay);
    if(!nCells) continue;
    printf("M.%i L.%i  %i cells:",mod+1,lay%6+1,nCells);
    Int_t cell=-1;
    while((cell=next(lay,cell)) >= 0) {
      printf(" %i(%i)",cell+1,HMdcBArray::getNSet(getTime(lay,cell)));
    }
    printf("\n");
  }
}

void HMdcClus::associate(HMdcClus *clus) {
  HMdcList12GroupCells::add((HMdcList12GroupCells*)clus);
  Float_t sWt=sumWt + clus->sumWt;
  Float_t newX=( x * sumWt + clus->x * clus->sumWt)/sWt;
  Float_t newY=( y * sumWt + clus->y * clus->sumWt)/sWt;
  Float_t a;
  a=(errX*errX+x*x)*sumWt+(clus->errX*clus->errX+clus->x*clus->x)*clus->sumWt;
  errX=sqrt(a/sWt-newX*newX);
  a=(errY*errY+y*y)*sumWt+(clus->errY*clus->errY+clus->y*clus->y)*clus->sumWt;
  errY=sqrt(a/sWt-newY*newY);
  x=newX;
  y=newY;
  sumWt=sWt;
  nBins+=clus->nBins;
  if(minCl[0]<clus->minCl[0]) minCl[0]=clus->minCl[0];
  if(minCl[1]<clus->minCl[1]) minCl[1]=clus->minCl[1];
  clus->remove();
}

/*void HMdcClus::calcQuality(){
  Short_t ni,n[4];
  Int_t ind[40];
  Short_t tmpC[40],tmpT[40];
  for(Int_t nm=0; nm<2; nm++) {
   n[0]=n[1]=n[2]=n[3]=0;
   for(Int_t nli=0; nli<6; nli++) {
    Int_t nl=nm*6+nli;
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
*/

/*Int_t HMdcClus::Compare(TObject *obj){
 Int_t q1=qualityClusters[0]+qualityClusters[1];
 Int_t q2=((HMdcClus*)obj)->qualityClusters[0]+
          ((HMdcClus*)obj)->qualityClusters[1];
  if (q1 > q2) return 1;
  else if (q1 < q2) return -1;
  else {
   if(ymid < (((HMdcClus*)obj)->ymid)) return 1;
   else return -1;
  }
}*/
