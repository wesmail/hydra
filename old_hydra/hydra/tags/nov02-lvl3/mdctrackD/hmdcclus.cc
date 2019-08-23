//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 05/06/2002 by V.Pechenov
//*-- Modified : 17/01/2002 by V.Pechenov
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
//   indCh1,indCh2 - region of indexes of childs (for Seg.2 = -1,-2)
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
////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcclus.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>
#include <stdlib.h>

ClassImp(HMdcClus)

void HMdcClus::clear(void) {
  HMdcList12GroupCells::clear();
  status=1;
  indexPar=indCh1=-1;
  indCh2=-2;
  typeClFinder=0;
  mod=-1;
  nMergedClus=1;
}

void HMdcClus::print(void) const {
  printPos();
  printCont();
}

void HMdcClus::printPos(void) const {
  printf("Cluster: Sec.%i Seg.%i Index=%i.",sec+1,seg+1,index);
  if(indexPar>=0) printf("  Index of parent=%i\n",indexPar);
  else if(indCh1>=0 && indCh2>=0)
    printf("  Indexes of childs = %i - %i\n",indCh1,indCh2);
  else printf("\n");
  printf("  Level %i:%i, %i bins/cluster, %i hits/cluster, nMergedClus=%i\n",
      minCl1,minCl2,nBins,getNCells(),nMergedClus);
  printf("  <x>=%g+/-%g <y>=%g+/-%g <z>=%g\n",x,errX,y,errY,getZ());
  printf("  xTarg=%g+/-%g yTarg=%g+/-%g zTarg=%g+/-%g\n",
      xTarg,errXTarg,yTarg,errYTarg,zTarg,errZTarg);
  printf("  Project plane equation: %g*x%+g*y+z=%g\n",parA,parB,parD);
}

void HMdcClus::printCont(void) const {
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

void HMdcClus::calcIntersection(const HGeomVector &r, const HGeomVector &dir,
                                   HGeomVector &out) const {
  // Calculating of a cross of line r,dir (point and direction)
  // with project plane parA*x+parB*y+z=parD
  Double_t x1=r(0);
  Double_t y1=r(1);
  Double_t z1=r(2);
  Double_t dX=dir(0);
  Double_t dY=dir(1);
  Double_t dZ=dir(2);
  Double_t del=1/(parA*dX+parB*dY+dZ);
  Double_t x=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del;
  Double_t y=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del;
  out.setXYZ(x,y,-parA*x-parB*y+parD);
}
