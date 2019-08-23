#ifndef HMDCCLUS_H
#define HMDCCLUS_H

#include "hmdclistcells.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include <TMath.h>

class HMdcClus : public HMdcList12GroupCells {
protected:
  UChar_t sec;
  UChar_t seg;
  Int_t typeClFinder; // =0 - finding in all mdc in sec.
                      // =1 - finding in each mdc indep.
  Short_t mod;        //=-1 if typeClFinder=0
  Short_t index;     // =loc[2]
  Float_t x;         // x-coordinate of cluster
  Float_t y;         // y-coordinate of cluster
  Float_t xTarg;     // x-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t yTarg;     // y-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t zTarg;     // z-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t errX;      // Error of x-coordinate of cluster
  Float_t errY;      // Error of y-coordinate of cluster
  Float_t errXTarg;  // Error of x-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t errYTarg;  // Error of y-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t errZTarg;  // Error of z-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t sumWt;     // Sum. of bins' weights in cluster
  Int_t nBins;       // Num. of bins in cluster
  UChar_t minCl1;    // minCl1 mdc 1or3, minCl2 mdc 2or4 (depending on seg.)
  UChar_t minCl2;    // clusters with bin content >= maxCl were searched
  Short_t nMergedClus; // Number of merged clusters
  Int_t indexPar;      // index(loc[2]-in cat.!) of parent (for Seg.1 =-1)
  Int_t indCh1;        // region of indexes of childs (for Seg.2 = -1)
  Int_t indCh2;        //
  Float_t parA;        // Project plane: parA*x+parB*y+z=parD
  Float_t parB;        //
  Float_t parD;        //
  Float_t sigma1;      // cluster shape:
  Float_t sigma2;      // sigma1 - long axis, sigma2 - short axis
  Float_t alpha;       // angle (deg.) of long axis and X axis
  Short_t status;      // user var.
//  Float_t probDubl;   //???????!!!!!!!!!!

public:
  HMdcClus(void) {
    status=1;
    indexPar=indCh1=-1;
    indCh2=-2;
    typeClFinder=0;
    mod=-1;
    nMergedClus=1;
  }
  HMdcClus(HMdcList12GroupCells& lcells) : HMdcList12GroupCells(lcells) {
    status=0;
    indexPar=indCh1=-1;
    indCh2=-2;
    typeClFinder=0;
    mod=-1;
    nMergedClus=1;
  }
  ~HMdcClus(void){;}
  void clear(void);
  virtual Bool_t isGeant() {return kFALSE;}
  void setSec(Int_t sc) {sec=sc;}
  void setMod(Int_t m) {mod=m;}
  void setIOSeg(Int_t sg) {seg=sg;}
  void setIndex(Int_t ind) {index=ind;}
  void setAddress(Int_t sc, Int_t sg, Int_t ind) {sec=sc; seg=sg; index=ind;}
  void setAddress(HLocation &loc) {sec=loc[0]; seg=loc[1]; index=loc[2];}
  void setMinCl(Int_t m1or3, Int_t m2or4) {minCl1=m1or3; minCl2=m2or4;}
  void setSumWt(Float_t swt) {sumWt=swt;}
  void setNBins(Int_t nBn) {nBins=nBn;}
  void setIndexParent(Int_t lp) {indexPar=lp;}
  void setIndexChilds(Int_t l1, Int_t l2) {indCh1=l1; indCh2=l2;}
  void setTypeClFinder(Int_t type) {typeClFinder=type;}
  void setNMergClust(Short_t nmrc) {nMergedClus=nmrc;}
  void setXY(Float_t xl, Float_t errx, Float_t yl, Float_t erry) {
      x=xl; errX=errx; y=yl; errY=erry;}
  void setXTarg(Float_t xl, Float_t err) {xTarg=xl; errXTarg=err;}
  void setYTarg(Float_t yl, Float_t err) {yTarg=yl; errYTarg=err;}
  void setZTarg(Float_t zl, Float_t err) {yTarg=zl; errZTarg=err;}
  void setTarg(const HGeomVector& v) {xTarg=v.getX();yTarg=v.getY();
                                      zTarg=v.getZ();}
  void setErrTarg(const HGeomVector& v) {
      errXTarg=v.getX();errYTarg=v.getY();errZTarg=v.getZ();}
  void setTarget(Float_t xl, Float_t errx, Float_t yl, Float_t erry,
      Float_t zl, Float_t errz) {xTarg=xl; errXTarg=errx;
      yTarg=yl; errYTarg=erry; zTarg=zl; errZTarg=errz;}
  void setShape(Float_t s1,Float_t s2,Float_t al) {
      sigma1=s1; sigma2=s2; alpha=al;}

  Short_t getMod() {return mod;}
  Int_t getTypeClFinder() {return typeClFinder;}
  UChar_t getSec() {return sec;}
  UChar_t getIOSeg() {return seg;}
  Short_t getIndex() {return index;}
  void getAddress(Int_t& sc, Int_t& sg, Int_t& in) {sc=sec; sg=seg; in=index;}
  void getAddress(HLocation &loc) {loc[0]=sec; loc[1]=seg; loc[2]=index;}
  Int_t getMinCl() {return Int_t(minCl1+minCl2);}
  UChar_t getMinCl(Int_t mod) {return ((mod&1)==0) ? minCl1:minCl2;}
  Float_t getX(void) {return x;}
  Float_t getY(void) {return y;}
  Float_t getZ(void) {return parD-parA*x-parB*y;}
  Float_t getXTarg(void) {return xTarg;}
  Float_t getYTarg(void) {return yTarg;}
  Float_t getZTarg(void) {return zTarg;}
  Float_t getErrX(void) {return errX;}
  Float_t getErrY(void) {return errY;}
  Float_t getErrXTarg(void) {return errXTarg;}
  Float_t getErrYTarg(void) {return errYTarg;}
  Float_t getErrZTarg(void) {return errZTarg;}
  Float_t getTheta(void) { return TMath::ACos((getZ()-zTarg)/TMath::Sqrt(
      (getZ()-zTarg)*(getZ()-zTarg)+(x-xTarg)*(x-xTarg)+(y-yTarg)*(y-yTarg) ));}
  Float_t getPhi(void) {return TMath::ATan2(y-yTarg,x-xTarg);}
  Float_t getSumWt() {return sumWt;}
  Int_t getNBins(){return nBins;}
  void getPoint(HGeomVector& v) {v.setXYZ(x,y,parD-parA*x-parB*y);}
  void getTarg(HGeomVector& v) {v.setXYZ(xTarg,yTarg,zTarg);}
  Int_t getIndexParent(void) {return indexPar;}
  void  getIndexRegChilds(Int_t& first, Int_t& last) {first=indCh1;
      last=indCh2;}
  Int_t getNextIndexChild(Int_t ind) {  //using: ind=-1;ind=getNextIndexChild(ind)
    return (ind<indCh1) ? indCh1:((ind<indCh2) ? ind+1:-1);
  }
  Short_t getNMergClust() {return nMergedClus;}

  void print();
  void printPos();
  virtual void printCont();
  void setStatus(Short_t st) {status=st;}
  Short_t getStatus() {return status;}

  void setPrPlane(const Float_t a, const Float_t b, const Float_t d)
      {parA=a; parB=b; parD=d;}
  Float_t A() const {return parA;}
  Float_t B() const {return parB;}
  Float_t C() const {return 1.;}
  Float_t D() const {return parD;}
  Float_t getZOnPrPlane(Float_t x,Float_t y) const {return parD-parA*x-parB*y;}
  Float_t getYOnPrPlane(Float_t x,Float_t z) const
      {return (parB!=0.) ? (parD-parA*x-z)/parB : 0.;}
  void calcIntersection(const HGeomVector &r, const HGeomVector &dir,
                                   HGeomVector &out) const;
  void getShape(Float_t s1,Float_t s2,Float_t al) {
      s1=sigma1; s2=sigma2; al=alpha;}
  Float_t getAlpha(void) {return alpha;}
  Float_t getSigma1(void) {return sigma1;}
  Float_t getSigma2(void) {return sigma2;}
  Float_t getRatio(void) {return (sigma1>0.) ? sigma2/sigma1 : 0.;}

  ClassDef(HMdcClus,1)
};

#endif
