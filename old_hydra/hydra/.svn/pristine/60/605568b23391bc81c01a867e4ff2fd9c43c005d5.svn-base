/////////////////////////////////////////////////////////////////////////
//
//  HMdcSeg this is the SEGment class of MDC
//
//  R.Schicker 20-May-00
//
/////////////////////////////////////////////////////////////////////////
#ifndef HMDCSEG_H
#define HMDCSEG_H

#include "hsymmat.h"
#include "hmdccellgroup.h"

class HMdcSeg : public HMdcCellGroup12 {

protected: 
  Float_t z;    // z-cordinate
  Float_t r;    // r-coordinate
  Float_t theta;  // polar angle theta
  Float_t phi;  // azimuthal angle phi
  Float_t chi2; // chi square per degree of freedom
  Int_t flag; // user flag
  HSymMat4 cov; // covariance matrix 4x4 in sector system		      
  UInt_t ind;   // index for sector/inner-outer segment/hit index

 public:
  HMdcSeg(void); 
  ~HMdcSeg(void);
  
  void Clear(void);
  
  void setZ(const Float_t zl,const Float_t err) { z=zl; cov.setErr(0,err);}
  void setR(const Float_t rl,const Float_t err) { r=rl; cov.setErr(1,err);}
  void setTheta(const Float_t th,const Float_t err) 
    {theta=th; cov.setErr(2,err);}
  void setPhi(const Float_t ph,const Float_t err) {phi=ph; cov.setErr(3,err);}

  void setZR(const Float_t zl,const Float_t erz,const Float_t rl, const Float_t err) { z=zl; cov.setErr(0,erz); r=rl; cov.setErr(1,err);}
  void setThetaPhi(const Float_t th,const Float_t ert,
		 const Float_t ph, const Float_t erp) { 
    theta=th; cov.setErr(2,ert); phi=ph; cov.setErr(3,erp);
  }

  void setPar(const Float_t zl, const Float_t ezl, const Float_t rl, 
	      const Float_t erl, const Float_t th, const Float_t eth, 
	      const Float_t ph, const Float_t eph) {
    z=zl; cov.setErr(0,ezl); r=rl; cov.setErr(1,erl); theta=th; 
    cov.setErr(2,eth); phi=ph; cov.setErr(3,eph);
  }

  void setChi2(const Float_t chl) { chi2=chl;}  
  void setFlag(const Int_t fl) { flag=fl;}

  void setSec(Int_t sec) { ind = ((sec & 0x7) << 29) | (ind & 0x1FFFFFFF); }
  void setIOSeg(Int_t seg) { ind = ((seg & 0x1) << 28) | (ind & 0xEFFFFFFF); }
  void setPol(Int_t pol) { ind = ((pol & 0x3) << 26) | (ind & 0xF3FFFFFF); }
  void setHitInd(Int_t i,Int_t idx) {
    ind = ((idx & 0x1FFF) << (i*13)) | (ind & (~(0x1FFF << (i*13)))); 
  }

  Float_t getZ(void) {return z;}
  Float_t getR(void) {return r;}
  Float_t getTheta(void) {return theta;}
  Float_t getPhi(void) {return phi;}

  void getZ(Float_t& zl,Float_t& err) { zl=z; err=cov.getErr(0);}
  void getR(Float_t& rl,Float_t& err) { rl=r; err=cov.getErr(1);}
  void getTheta(Float_t& th,Float_t& err) { th=theta; err=cov.getErr(2);}
  void getPhi(Float_t& ph,Float_t& err) { ph=phi; err=cov.getErr(3);}
  void getZR(Float_t& zl,Float_t& erz,Float_t& rl,Float_t& err) { 
    zl=z; erz=cov.getErr(0); rl=r; err=cov.getErr(1);}
  void getThetaPhi(Float_t& th,Float_t& ert,Float_t& ph,Float_t& erp) { 
    th=theta; ert=cov.getErr(2); ph=phi; erp=cov.getErr(3);}
  void getPar(Float_t& p1,Float_t& p2,Float_t& p3,Float_t& p4) {
    p1=z; p2=r; p3=theta; p4=phi;}
  void getErrPar(Float_t& p1,Float_t& p2,Float_t& p3,Float_t& p4){
    p1=cov.getErr(0); p2=cov.getErr(1); p3=cov.getErr(2); p4=cov.getErr(3);}
  Float_t getChi2(void) { return chi2;}
  Int_t getFlag(void) { return flag;}

  Int_t getSec(void) {return (ind >> 29) & 0x7; }   // sector number
  Int_t getIOSeg(void) { return (ind >> 28) & 0x1; } // inner seg=0, outer=1 
  Int_t getPol(void) {return (ind >> 26) & 0x3; }  // not defined = 0, positive = 1, negative = 2
  Int_t getHitInd(Int_t i); // Hit index (-1 --> No hit)

  
  ClassDef(HMdcSeg,1) // MDC segment
};

inline Int_t HMdcSeg::getHitInd(Int_t i) {
  Int_t r=((ind >> (i*13)) & 0x1FFF);
  return (r == 0x1FFF)?-1:r;
}

#endif  /* HMdcSeg_H */


















