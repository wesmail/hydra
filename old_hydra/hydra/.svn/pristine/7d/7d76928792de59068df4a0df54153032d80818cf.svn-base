#ifndef HMDCCLUS_H
#define HMDCCLUS_H

#include "hmdclistcells.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "TMath.h"

class HMdcClus : public HMdcList12GroupCells {
protected:
  UChar_t sec;
  UChar_t seg;
  Int_t typeClFinder;  // =0 - finding in all mdc in sec.
                       // =1 - finding in each mdc indep.
  Short_t mod;         //=-1 if typeClFinder=0
  Short_t index;       // =loc[2]
  Float_t x;           // x-coordinate of cluster
  Float_t y;           // y-coordinate of cluster
  Float_t xTarg;       // x-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t yTarg;       // y-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t zTarg;       // z-target(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t errX;        // Error of x-coordinate of cluster
  Float_t errY;        // Error of y-coordinate of cluster
  Float_t errXTarg;    // Error of xTarg(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t errYTarg;    // Error of yTarg(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t errZTarg;    // Error of zTarg(Mdc1-2) or point on kick plane (Mdc3-4)
  Float_t sumWt;       // Sum. of bins' weights in cluster
  Int_t nBins;         // Num. of bins in cluster
  UChar_t minCl1;      // minCl1 mdc 1or3, minCl2 mdc 2or4 (depending on seg.)
  UChar_t minCl2;      // clusters with bin content >= maxCl were searched
  Short_t nMergedClus; // Number of merged clusters
  Int_t indexPar;      // index of parent cluster in catMdcClus (def.: =-1)
  Int_t indCh1;        // indexes region of childs in catMdcClus (def.: = -1)
  Int_t indCh2;        //                                        (def.: = -2)
  Float_t parA;        // Project plane: parA*x+parB*y+z=parD
  Float_t parB;        //
  Float_t parD;        //
  Short_t status;      // user var.
  
  Short_t clusSizeM1;  // number of bins in mod.1 cluster
  Short_t nDrTimesM1;  // number of drift times in cluster 
  Short_t nMergClusM1; // number of merged clusters 
  Float_t sigma1M1;    // cluster shape mod.1:
  Float_t sigma2M1;    // sigma1 - long axis, sigma2 - short axis
  Float_t alphaM1;     // angle (deg.) of long axis and X axis
  
  Short_t clusSizeM2;  // Num. of bins in mod.2 cluster
  Short_t nDrTimesM2;  // number of drift times in cluster 
  Short_t nMergClusM2; // number of merged clusters
  Float_t sigma1M2;    // cluster shape mod.2:
  Float_t sigma2M2;    // sigma1 - long axis, sigma2 - short axis
  Float_t alphaM2;     // angle (deg.) of long axis and X axis
    

public:
  HMdcClus(void) {
    status=1;
    indexPar=indCh1=-1;
    indCh2=-2;
    typeClFinder=0;
    mod=-1;
    nMergedClus=nMergClusM1=nMergClusM2=1;
    nDrTimesM1=nDrTimesM2=0;
  }
  HMdcClus(HMdcList12GroupCells& lcells) : HMdcList12GroupCells(lcells) {
    status=1;
    indexPar=indCh1=-1;
    indCh2=-2;
    typeClFinder=0;
    mod=-1;
    nMergedClus=nMergClusM1=nMergClusM2=1;
    nDrTimesM1=nDrTimesM2=0;
  }
  ~HMdcClus(void) {;}
  void clear(void);
  void clearMod1Par(void) { nMergClusM1=nDrTimesM1=clusSizeM1=0; 
    sigma1M1=sigma2M1=alphaM1=0; }
  void clearMod2Par(void) { nMergClusM2=nDrTimesM2=clusSizeM2=0; 
    sigma1M2=sigma2M2=alphaM2=0; }
  virtual Bool_t isGeant(void) const {return kFALSE;}
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
  void setPrPlane(const Float_t a, const Float_t b, const Float_t d)
      {parA=a; parB=b; parD=d;}
  
  void setClusSizeM1(Short_t clSz) {clusSizeM1=clSz;}
  void setNDrTimesM1(Short_t nDrTm) {nDrTimesM1=nDrTm;}
  void setNMergClustM1(Short_t nmrc) {nMergClusM1=nmrc;}
  void setShapeM1(Float_t s1,Float_t s2,Float_t al) {
      sigma1M1=s1; sigma2M1=s2; alphaM1=al;}

  void setClusSizeM2(Short_t clSz) {clusSizeM2=clSz;}
  void setNDrTimesM2(Short_t nDrTm) {nDrTimesM2=nDrTm;}
  void setNMergClustM2(Short_t nmrc) {nMergClusM2=nmrc;}
  void setShapeM2(Float_t s1,Float_t s2,Float_t al) {
      sigma1M2=s1; sigma2M2=s2; alphaM2=al;}

  Short_t getMod() const {return mod;}
  Int_t getTypeClFinder() const {return typeClFinder;}
  UChar_t getSec() const {return sec;}
  UChar_t getIOSeg() const {return seg;}
  Short_t getIndex() const {return index;}
  void getAddress(Int_t& sc,Int_t& sg,Int_t& in) const {sc=sec;sg=seg;in=index;}
  void getAddress(HLocation &loc) const {loc[0]=sec; loc[1]=seg; loc[2]=index;}
  Int_t getMinCl() const {return Int_t(minCl1+minCl2);}
  UChar_t getMinCl(Int_t mod) const {return ((mod&1)==0) ? minCl1:minCl2;}
  Float_t getX(void) const {return x;}
  Float_t getY(void) const {return y;}
  Float_t getZ(void) const {return parD-parA*x-parB*y;}
  Float_t getXTarg(void) const {return xTarg;}
  Float_t getYTarg(void) const {return yTarg;}
  Float_t getZTarg(void) const {return zTarg;}
  Float_t getErrX(void) const {return errX;}
  Float_t getErrY(void) const {return errY;}
  Float_t getErrXTarg(void) const {return errXTarg;}
  Float_t getErrYTarg(void) const {return errYTarg;}
  Float_t getErrZTarg(void) const {return errZTarg;}
  Float_t getTheta(void) const { return TMath::ATan2(
      TMath::Sqrt((x-xTarg)*(x-xTarg)+(y-yTarg)*(y-yTarg)),getZ()-zTarg);}
  Float_t getPhi(void) const {return TMath::ATan2(y-yTarg,x-xTarg);}
  Float_t getSumWt() const {return sumWt;}
  Int_t getNBins() const {return nBins;}
  void getPoint(HGeomVector& v) const {v.setXYZ(x,y,parD-parA*x-parB*y);}
  void getTarg(HGeomVector& v) const {v.setXYZ(xTarg,yTarg,zTarg);}
  Int_t getIndexParent(void) const {return indexPar;}
  void  getIndexRegChilds(Int_t& first, Int_t& last) const {first=indCh1;
      last=indCh2;}
  Int_t getNextIndexChild(Int_t ind) const {  
    //using: ind=-1;ind=getNextIndexChild(ind)
    return (ind<indCh1) ? indCh1:((ind<indCh2) ? ind+1:-1);
  }
  Short_t getNMergClust() const {return nMergedClus;}
  Float_t A() const {return parA;}
  Float_t B() const {return parB;}
  Float_t C() const {return 1.;}
  Float_t D() const {return parD;}
  Float_t getZOnPrPlane(Float_t x,Float_t y) const {return parD-parA*x-parB*y;}
  Float_t getYOnPrPlane(Float_t x,Float_t z) const
      {return (parB!=0.) ? (parD-parA*x-z)/parB : 0.;}

  Short_t getClusSizeM1(void) const {return clusSizeM1;}
  Short_t getNDrTimesM1(void) const {return nDrTimesM1;} 
  Short_t getNMergClustM1(void) const {return nMergClusM1;}
  void getShapeM1(Float_t s1,Float_t s2,Float_t al) const {
      s1=sigma1M1; s2=sigma2M1; al=alphaM1;}
  Float_t getAlphaM1(void) const {return alphaM1;}
  Float_t getSigma1M1(void) const {return sigma1M1;}
  Float_t getSigma2M1(void) const {return sigma2M1;}
  Float_t getRatioM1(void) const {return (sigma1M1>0.) ? sigma2M1/sigma1M1:0.;}
  
  Short_t getClusSizeM2(void) const {return clusSizeM2;}
  Short_t getNDrTimesM2(void) const {return nDrTimesM2;} 
  Short_t getNMergClustM2(void) const {return nMergClusM2;}
  void getShapeM2(Float_t s1,Float_t s2,Float_t al) const {
      s1=sigma1M2; s2=sigma2M2; al=alphaM2;}
  Float_t getAlphaM2(void) const {return alphaM2;}
  Float_t getSigma1M2(void) const {return sigma1M2;}
  Float_t getSigma2M2(void) const {return sigma2M2;}
  Float_t getRatioM2(void) const {return (sigma1M2>0.) ? sigma2M2/sigma1M2:0.;}

  void print(void) const;
  void printPos(void) const;
  virtual void printCont(void) const;
  void setStatus(Short_t st) {status=st;}
  Short_t getStatus() const {return status;}
  void calcIntersection(const HGeomVector &r, const HGeomVector &dir,
                                   HGeomVector &out) const;
  
  ClassDef(HMdcClus,1)
};

#endif
