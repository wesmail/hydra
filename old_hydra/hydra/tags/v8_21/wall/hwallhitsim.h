#ifndef HWALLHITSIM_H
#define HWALLHITSIM_H

#include "TObject.h"
#include "hwallhit.h"

//F. Krizek 11.8.2005

class HWallHitSim : public TObject {
//class HWallHitSim : public HWallHit {
protected:
    Float_t timeCal;    // tdc time
    Float_t chargeCal;  // adc charge  
    Int_t cell;      // paddle number
    Float_t theta,phi,d;//d is distance 
    Float_t xlab, ylab, zlab;//coordinats in lab system
    // Int_t index;
  Int_t nTrack1;   // number of Track for first hit
  Int_t nTrack2;   // number of Track for second hit
public:
    HWallHitSim(void);
    ~HWallHitSim(void) {;}

  inline void setNTrack1(const Int_t n) {nTrack1=n;} //set trk nb of hit 1
  inline void setNTrack2(const Int_t n) {nTrack2=n;} //set trk nb of hit 2
  inline Int_t getNTrack1(void) const {return nTrack1;} //get trk nb of hit 1
  inline Int_t getNTrack2(void) const {return nTrack2;} //get trk nb of hit 2 

    void setTime(const Float_t v)   { timeCal=v; }
    void setCharge(const Float_t v) { chargeCal=v; }
    void setCell(const Int_t n)     { cell=n; }
    void setAddress(const Int_t c);
    //void setIndex(Int_t i)    { index=i; } //FK//
    void setTheta(const Float_t t) {theta=t;}
    void setPhi(const Float_t p) {phi=p;}
    void setDistance(const Float_t dis) {d=dis;}
    void setXYZLab(Float_t xx, Float_t yy, Float_t zz) 
           {xlab=xx;ylab=yy;zlab=zz;};

    void clear(void);
    //void fill(const Float_t t, const Float_t a, const Int_t c, Int_t i);
    void fill(const Float_t t, const Float_t a, const Int_t c);

    Float_t getTime(void) const { return timeCal; }
    Float_t getCharge(void) const { return chargeCal; }
    Int_t getCell(void) const { return cell; }
    void getAddress(Int_t& c);
    //Int_t getIndex(void){ return index; } //FK//
    Float_t getTheta(void) {return theta;}
    Float_t getPhi(void) {return phi;}
    Float_t getDistance(void) {return d;}    
    void getXYZLab(Float_t &xx, Float_t &yy, Float_t &zz) { xx = xlab; yy = ylab; zz = zlab; }


    ClassDef(HWallHitSim,1)  // WALL detector calibrated data
};

#endif /* ! HWALLHITSIM_H */
