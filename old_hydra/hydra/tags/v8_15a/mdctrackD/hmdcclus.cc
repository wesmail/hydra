//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 05/06/2002 by V.Pechenov
//*-- Modified : 17/01/2002 by V.Pechenov
//*-- Modified : 05/06/2001 by V.Pechenov
//*-- Modified : 21/08/2000 by V.Pechenov
//*-- Modified : 20/05/2000 by V.Pechenov
//*-- Modified : 26/10/99 by V.Pechenov
//*-- Modified : 25/02/99


//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcClust
//
//  Container class keep cluster information:
//   x,y - position of cluster on the project plane HMdcPlane
//   xTag,yTag,zTarg - target, or point on the kick plane
//   indexPar - index of parent in catMdcClus for Seg.1 = -1)
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
  setDefValues();
}

void HMdcClus::setDefValues(void) {
  status       = 1;
  indexPar     = -1;
  indCh1       = -1;
  indCh2       = -2;
  typeClFinder = 0;
  mod          = -1;
  nMergedClus  = 1;
  nMergClusM1  = 1;
  nMergClusM2  = 1;
  nDrTimesM1   = 0;
  nDrTimesM2   = 0;
  clusSizeM1   = 0;
  clusSizeM2   = 0;
}


void HMdcClus::print(Bool_t fl) const {
  printPos();
  printCont(fl);
}

void HMdcClus::printPos(void) const {
  Int_t modp=(mod>=0) ? mod+1:mod;
  printf("Cluster: Sec.%i Seg.%i Mod.%i Clust.type=%i %sLevel Index=%i.",
      sec+1,seg+1,modp,typeClFinder&127,isFixedLevel() ? "Fixed":"Float",
      index);
  if(indexPar>=0) printf("  Index of parent=%i\n",indexPar);
  else if(indCh1>=0 && indCh2>=0)
    printf("  Indexes of childs = %i - %i\n",indCh1,indCh2);
  else printf("\n");
  printf("  Initial level:");
  if(seg==0 || (typeClFinder&127)!=2) printf(" %i:%i,",minCl1,minCl2);
  else printf(" %i,",minCl1+minCl2);
  printf("  Real level: %i,",realLevel);
  printf(" %i bins/cluster, %i hits/cluster, nMergedClus=%i\n",
      nBins,getNCells(),nMergedClus);
  printf("  <x>=%g+/-%g <y>=%g+/-%g <z>=%g\n",x,errX,y,errY,getZ());
  printf("  xTarg=%g+/-%g yTarg=%g+/-%g zTarg=%g+/-%g\n",
      xTarg,errXTarg,yTarg,errYTarg,zTarg,errZTarg);
  printf("  Project plane equation: %g*x%+g*y+z=%g\n",parA,parB,parD);
  if(clusSizeM1>0) printf(
    "  Mod%i: level=%i, %3i bins, %2i wires, nMergedClus=%i\n",
    seg*2+1,clFnLevelM1,clusSizeM1,nDrTimesM1,nMergClusM1);
  if(clusSizeM2>0) printf(
    "  Mod%i: level=%i, %3i bins, %2i wires, nMergedClus=%i\n",
    seg*2+2,clFnLevelM2,clusSizeM2,nDrTimesM2,nMergClusM2);
}

void HMdcClus::printCont(Bool_t fl) const {
  if(!fl) return;
  for(Int_t lay=0; lay<12; lay++) {
    Int_t modw=seg*2+lay/6;
    Int_t nCells=getNCells(lay);
    if(!nCells) continue;
    printf("M.%i L.%i  %i cells:",modw+1,lay%6+1,nCells);
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
  Double_t del=1./(parA*dX+parB*dY+dZ);
  Double_t x=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del;
  Double_t y=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del;
  out.setXYZ(x,y,-parA*x-parB*y+parD);
}

void HMdcClus::calcIntersection(const HGeomVector &p1, const HGeomVector &p2,
                                   Float_t& x, Float_t& y) const {
  // Calculating of a cross of line p1 -> p2 (two points on the line)
  // with project plane parA*x+parB*y+z=parD
  Double_t x1=p1(0);
  Double_t y1=p1(1);
  Double_t z1=p1(2);
  Double_t dX=p2(0)-x1;
  Double_t dY=p2(1)-y1;
  Double_t dZ=p2(2)-z1;
  Double_t del=1./(parA*dX+parB*dY+dZ);
  x=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del;
  y=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del;
}

void HMdcClus::calcIntersection(const HGeomVector &p, 
                                   Float_t& x, Float_t& y) const {
  // Calculating of a cross of line target -> p (two points on the line)
  // with project plane parA*x+parB*y+z=parD
  Double_t dX=p(0)-xTarg;
  Double_t dY=p(1)-yTarg;
  Double_t dZ=p(2)-zTarg;
  Double_t del=1./(parA*dX+parB*dY+dZ);
  x=(dX*(parD-zTarg-parB*yTarg)+xTarg*(parB*dY+dZ))*del;
  y=(dY*(parD-zTarg-parA*xTarg)+yTarg*(parA*dX+dZ))*del;
}


void HMdcClus::fillErrMatClus(Bool_t isCoilOff, HSymMat4& errMatClus) {
  errMatClus.Clear();
  errMatClus.setErr(2,errX);
  errMatClus.setErr(3,errY);
  if(seg==0 || isCoilOff) {  // errXTarg+errYTarg==0. ???
    errMatClus.setErr(0,errZTarg);
  } else {
    errMatClus.setErr(0,errXTarg);
    errMatClus.setErr(1,errYTarg);
  }
}

void HMdcClus::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcClus.
   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HMdcList12GroupCells::Streamer(R__b);
      R__b >> sec;
      R__b >> seg;
      R__b >> typeClFinder;
      R__b >> mod;
      R__b >> index;
      R__b >> x;
      R__b >> y;
      R__b >> xTarg;
      R__b >> yTarg;
      R__b >> zTarg;
      R__b >> errX;
      R__b >> errY;
      R__b >> errXTarg;
      R__b >> errYTarg;
      R__b >> errZTarg;
      R__b >> sumWt;
      R__b >> nBins;
      R__b >> minCl1;
      R__b >> minCl2;
      R__b >> nMergedClus;
      R__b >> indexPar;
      R__b >> indCh1;
      R__b >> indCh2;
      if(R__v < 2) realLevel = minCl1+minCl2;
      R__b >> parA;
      R__b >> parB;
      R__b >> parD;
      R__b >> status;
      if(R__v < 2) clFnLevelM1 = minCl1;
      R__b >> clusSizeM1;
      R__b >> nDrTimesM1;
      R__b >> nMergClusM1;
      R__b >> sigma1M1;
      R__b >> sigma2M1;
      R__b >> alphaM1;
      if(R__v < 2) clFnLevelM2 = minCl2;
      R__b >> clusSizeM2;
      R__b >> nDrTimesM2;
      R__b >> nMergClusM2;
      R__b >> sigma1M2;
      R__b >> sigma2M2;
      R__b >> alphaM2;
      R__b.CheckByteCount(R__s, R__c, HMdcClus::IsA());
   } else {
      R__c = R__b.WriteVersion(HMdcClus::IsA(), kTRUE);
      HMdcList12GroupCells::Streamer(R__b);
      R__b << sec;
      R__b << seg;
      R__b << typeClFinder;
      R__b << mod;
      R__b << index;
      R__b << x;
      R__b << y;
      R__b << xTarg;
      R__b << yTarg;
      R__b << zTarg;
      R__b << errX;
      R__b << errY;
      R__b << errXTarg;
      R__b << errYTarg;
      R__b << errZTarg;
      R__b << sumWt;
      R__b << nBins;
      R__b << minCl1;
      R__b << minCl2;
      R__b << realLevel;
      R__b << nMergedClus;
      R__b << indexPar;
      R__b << indCh1;
      R__b << indCh2;
      R__b << parA;
      R__b << parB;
      R__b << parD;
      R__b << status;
      R__b << clFnLevelM1;
      R__b << clusSizeM1;
      R__b << nDrTimesM1;
      R__b << nMergClusM1;
      R__b << sigma1M1;
      R__b << sigma2M1;
      R__b << alphaM1;
      R__b << clFnLevelM2;
      R__b << clusSizeM2;
      R__b << nDrTimesM2;
      R__b << nMergClusM2;
      R__b << sigma1M2;
      R__b << sigma2M2;
      R__b << alphaM2;
      R__b.SetByteCount(R__c, kTRUE);
   }
}
