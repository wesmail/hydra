#ifndef HMDCCLUS_H
#define HMDCCLUS_H

#include "hmdclistcells.h"
#include "hmdcgeomobj.h"
#include "hfilter.h"
#include "hlocation.h"
#include <TMath.h>

class HMdcClus : public HMdcList12GroupCells, public HMdcPrPlane {
protected:
  UChar_t sec;
  UChar_t seg;
  Int_t typeClFinder; // =0 - finding in all mdc in sec.
                      // =1 - finding in each mdc indep.
  Short_t mod;        //=-1 if typeClFinder=0
//  UChar_t ind;       // Sector(3bits 00011100), segment(2bits 00000011 (may be it will module, not segment))
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
  UChar_t minCl[2];  // minCl[0] mdc 1or3, minCl[1] mdc 2or4 (depending on seg.)
                     // clusters with bin content >= maxCl were searched
  Short_t nMergedClus; // Number of merged clusters
  Bool_t status;       // status=kTRUE - ok., =kFALSE - the obj. will be removed
  Int_t indexPar;      // index(loc[2]-in cat.!) of parent (for Seg.1 = -1)
  Int_t indexChild[2]; // region of indexes of childs (for Seg.2 = -1)
//  Float_t probDubl;   //???????!!!!!!!!!!

public:
  HMdcClus() {clear();}
  ~HMdcClus(){;}
  void clear();
  virtual Bool_t isGeant() {return kFALSE;}
  void setSec(Int_t sc) {sec=sc;}
  void setIOSeg(Int_t sg) {seg=sg;}
  void setIndex(Int_t ind) {index=ind;}
  void setAddress(Int_t sc, Int_t sg, Int_t ind) {sec=sc; seg=sg; index=ind;}
  void setAddress(HLocation &loc) {sec=loc[0]; seg=loc[1]; index=loc[2];}
  void setMinCl(Int_t m1or3, Int_t m2or4) {minCl[0]=m1or3; minCl[1]=m2or4;}
  void setMinCl(Int_t *mb) {minCl[0]=mb[seg*2]; minCl[1]=mb[seg*2+1];} //?!!
  void setX(Float_t xl, Float_t err) {x=xl; errX=err;}
  void setY(Float_t yl, Float_t err) {y=yl; errY=err;}
  void setXTarg(Float_t xl, Float_t err) {xTarg=xl; errXTarg=err;}
  void setYTarg(Float_t yl, Float_t err) {yTarg=yl; errYTarg=err;}
  void setZTarg(Float_t zl, Float_t err) {yTarg=zl; errZTarg=err;}
  void setTarg(const HGeomVector& v) {xTarg=v.getX();yTarg=v.getY();
                                      zTarg=v.getZ();}
  void setErrTarg(const HGeomVector& v) {
      errXTarg=v.getX();errYTarg=v.getY();errZTarg=v.getZ();}
  void setXY(Float_t xl, Float_t errx, Float_t yl, Float_t erry) {
      x=xl; errX=errx; y=yl; errY=erry;}
  void setXYZTarg(Float_t xl, Float_t errx, Float_t yl, Float_t erry,
      Float_t zl, Float_t errz) {xTarg=xl; errXTarg=errx;
      yTarg=yl; errYTarg=erry; zTarg=zl; errZTarg=errz;}
  void setTarg(Float_t dX, Float_t dY, Float_t dZ) {
      xTarg=dX; yTarg=dY; zTarg=dZ;}
  void setSumWt(Float_t swt) {sumWt=swt;}
  void setNBins(Int_t nBn) {nBins=nBn;}
  void setIndexParent(Int_t lp) {indexPar=lp;}
  void setIndexChilds(Int_t l1, Int_t l2) {indexChild[0]=l1; indexChild[1]=l2;}
  void setTypeClFinder(Int_t type) {typeClFinder=type;}
  void setMod(Int_t m) {mod=m; minCl[(m%2) ? 0:1]=0;}
  void setNMergClust(Short_t nmrc) {nMergedClus=nmrc;}

  Short_t getMod() {return mod;}
  Int_t getTypeClFinder() {return typeClFinder;}
  UChar_t getSec() {return sec;}
  UChar_t getIOSeg() {return seg;}
  Short_t getIndex() {return index;}
  void getAddress(Int_t& sc, Int_t& sg, Int_t& in) {sc=sec; sg=seg; in=index;}
  void getAddress(HLocation &loc) {loc[0]=sec; loc[1]=seg; loc[2]=index;}
  Int_t getMinCl() {return Int_t(minCl[0]+minCl[1]);}
  Int_t getMinCl(Int_t mod) {return Int_t(minCl[mod&1]);}
  Float_t getX(void) {return x;}
  Float_t getY(void) {return y;}
  Float_t getZ(void) {return getZOnPlane(x,y);}
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
  void getPoint(HGeomVector& v) {v.setXYZ(x,y,getZOnPlane(x,y));}
  void getTarg(HGeomVector& v) {v.setXYZ(xTarg,yTarg,zTarg);}
  Int_t getIndexParent(void) {return indexPar;}
  void  getIndexRegChilds(Int_t& first, Int_t& last) {first=indexChild[0];
      last=indexChild[1];}
  Int_t getNextIndexChild(Int_t ind) {  //using: ind=-1;ind=getNextIndexChild(ind)
    return (ind<indexChild[0]) ? indexChild[0]:((ind<indexChild[1]) ? ind+1:-1);
  }
  Short_t getNMergClust() {return nMergedClus;}

  void print();
  void printPos();
  virtual void printCont();
  void setStatus(Bool_t st) {status=st;}
  void remove() {status=kFALSE;}
  Bool_t getStatus() {return status;}
  Bool_t check(HMdcClus *clus) {return status;}
  virtual void associate(HMdcClus *clus);

//  void setPrDb(Float_t pD){prDb=pD;}
//  Float_t getPrDb(){return prDb;}
//  Int_t getQuality1(){return qualityClusters[0];}
//  Int_t getQuality2(){return qualityClusters[1];}

//  void calcQuality();

//  Bool_t Test(HMdcClus& clst, Double_t Ap, Double_t Bp, Double_t Dp,
//              Double_t shX2, Double_t shY2);
//  Bool_t IsSortable() const {return kTRUE;}
//  Int_t Compare(TObject *obj);

  ClassDef(HMdcClus,1)
};

class HMdcClusFilter : public HFilter {
public:
  HMdcClusFilter(void){;}
  ~HMdcClusFilter(void){;}
  Bool_t check(TObject *clus) {return ((HMdcClus*)clus)->getStatus();}
  ClassDef(HMdcClusFilter,1)
};

#endif
