/////////////////////////////////////////////////////////////////////////
//
//  HMdcHit this is the HIT class of MDC
//
//  R.Schicker 20-May-00
//
/////////////////////////////////////////////////////////////////////////
 
#ifndef HMDCHIT_H
#define HMDCHIT_H

#include "hdataobject.h" 
#include "hsymmat.h" 
#include <TMath.h>

class HMdcHit : public HDataObject {

protected: 
  static UChar_t nCellsLookUp[256];         //!
  static UChar_t cellPosLookUp[4][256];     //!
  static Bool_t lookUpsInitialized;         //!
  Float_t x;         // x-coordinate of hit
  Float_t y;         // y-coordinate of hit
  Float_t xDir;      // x-direction of hit
  Float_t yDir;      // y-direction of hit
  Float_t offset;    // offset
  Float_t chi2;      // chi square per degree of freedom
  Float_t flag;      // user flag
  HSymMat5 cov;      // covariance matrix of parameters 
  UShort_t iLayer[6];// array which contains wire number/cal2 hit index
  Int_t sector;      // sector
  Int_t module;      // module			  

public:
  HMdcHit(void); 
  ~HMdcHit(void);
 
  void Clear(void);
  void initLookups(void);

  void setX(const Float_t xl,const Float_t err) {
    x=xl; cov.setErr(0,err);}

  void setY(const Float_t yl,const Float_t err) {
    y=yl; cov.setErr(1,err);}

  void setXDir(const Float_t xl,const Float_t err) {
    x=xl; cov.setErr(2,err);}

  void setYDir(const Float_t yl,const Float_t err) {
    y=yl; cov.setErr(3,err);}

  void setOff(const Float_t off,const Float_t err) {
    offset=off; cov.setErr(4,err);}

  void setXY(const Float_t xl,const Float_t errx,const Float_t yl,const Float_t erry) {
    x=xl; cov.setErr(0,errx); y=yl; cov.setErr(1,erry);}

  void setXYDir(const Float_t xl,const Float_t errx,const Float_t yl,const 
   Float_t erry) {xDir=xl; cov.setErr(2,errx); yDir=yl; cov.setErr(3,erry);}

  void setPar(const Float_t p1,const Float_t p2,const Float_t p3,const Float_t p4,const Float_t p5) {
    x=p1; y=p2; xDir=p3; yDir=p4; offset=p5;}

  void setErrPar(const Float_t p1,const Float_t p2,const Float_t p3,const 
     Float_t p4,const Float_t p5) {cov.setErr(0,p1); cov.setErr(1,p2); 
     cov.setErr(2,p3); cov.setErr(3,p4); cov.setErr(4,p5); }

  void setChi2(const Float_t chl) {chi2=chl;}  
  void setFlag(const Float_t fl) {flag = fl;}


  void setSecMod(const Int_t is,const Int_t im) {sector=is; module=im;}

  Float_t getX(void) { return x; }
  Float_t getY(void) { return y; }
  Float_t getXDir(void) { return xDir; }
  Float_t getYDir(void) { return yDir; }
  Float_t getOffset(void) { return offset; }
  Float_t getErrX(void) { return cov.getErr(0); }
  Float_t getErrY(void) { return cov.getErr(1); }
  Float_t getErrXDir(void) { return cov.getErr(2); }
  Float_t getErrYDir(void) { return cov.getErr(3); }
  Float_t getErrOffset(void) { return cov.getErr(4); }
  Float_t getTheta(void) { 
    return TMath::ACos(TMath::Sqrt(1-xDir*xDir-yDir*yDir)); }
  Float_t getPhi(void) { return TMath::ATan(yDir/xDir); }
  HSymMat &getCovariance(void) { return cov; }
  inline Int_t getNCells(Int_t layer);
  inline Int_t getCell(Int_t layer,Int_t idx);
  inline Int_t getSignId(Int_t layer,Int_t idx);
  inline void setSignId(Int_t layer,Int_t cell,
		       Int_t t1,Int_t t2=0,Int_t t3=0,Int_t t4=0);

  void getX(Float_t& xl,Float_t& errx) {xl=x; errx=cov.getErr(0);}
  void getY(Float_t& yl,Float_t& erry) {yl=y; erry=cov.getErr(1);}
  void getXDir(Float_t& xl,Float_t& errx) {xl=xDir; errx=cov.getErr(2);}
  void getYDir(Float_t& yl,Float_t& erry) {yl=yDir; erry=cov.getErr(3);}

  void getXY(Float_t& xl,Float_t& errx,Float_t& yl,Float_t erry) {
    xl=x; errx=cov.getErr(0); yl=y; erry=cov.getErr(1);}

  void getXYDir(Float_t& xl,Float_t& errx,Float_t& yl,Float_t erry) {
    xl=xDir; errx=cov.getErr(2); yl=yDir; erry=cov.getErr(3);}

  void getOff(Float_t& off,Float_t& err) { off=offset; err=cov.getErr(4);}

  void getPar(Float_t& p1,Float_t& p2,Float_t& p3,Float_t& p4,Float_t& p5) {
    p1=x; p2=y; p3=xDir; p4=yDir; p5=offset;}
  
  void getErrPar(Float_t& p1,Float_t& p2,Float_t& p3,Float_t& p4,Float_t& p5) {
    p1=cov.getErr(0); p2=cov.getErr(1); p3=cov.getErr(2); p4=cov.getErr(3);
    p5=cov.getErr(4);}

  void getChi2(Float_t& chl) {chl = chi2;}
  void getFlag(Float_t& fl) {fl=flag;}

  void getSecMod(Int_t& isec,Int_t& imod) {isec=sector; imod=module;}

  void print(void){
    printf("x=%f y=%f xDir=%f yDir=%f offset=%f flag=%f \n",x,y,xDir,yDir,offset,flag);}

   ClassDef(HMdcHit,1) // hit in a MDC chamber 
};

Int_t HMdcHit::getNCells(Int_t layer) {
  return nCellsLookUp[iLayer[layer] & 0xFF];
}

Int_t HMdcHit::getCell(Int_t layer,Int_t idx) {
  return cellPosLookUp[idx][iLayer[layer] & 0xFF];
}

Int_t HMdcHit::getSignId(Int_t layer,Int_t idx) {
  //Returns 0 for time1 and 1 for time2 in cell basecell+idx; starting with 
  //idx=0
  Int_t shift=getCell(layer,idx)*2;
  return (iLayer[layer] >> shift) & 0x3;
}

void HMdcHit::setSignId(Int_t layer,Int_t cell,
		        Int_t t1,Int_t t2,Int_t t3,Int_t t4) {
  //Sets the signal ids and base cell number for layer "layer"
  //Input:
  //  layer --> Number of layer to set
  //  cell  --> Number of the first cell fired in this layer
  //  t1 - t4 --> SignalId for cells: "cell"+0, "cell"+1 ... "cell"+4
  //              Possible values are:
  //                0 --> This cell has not fired
  //                1 --> First signal (time/distance) was used
  //                2 --> Second signal (time/distance) was used
  iLayer[layer]=((cell & 0xFF) << 8) | ((t4 & 0x3)<<6) | 
    ((t3 & 0x3)<<4) | ((t2 & 0x3)<<2) | ((t1 & 0x3));
}

#endif  /* HMdcHit_H */









