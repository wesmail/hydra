#ifndef HMDCDRIFTTIMEPAR_H
#define HMDCDRIFTTIMEPAR_H

#include "TObjArray.h"

class HMdcCal2ParSim;
class HMdcCal2ParSecSim;
class HMdcCal2ParModSim;
class HMdcCal2ParAngleSim;

class HMdcDriftTimeParBin : public TObject {
  protected:
    Double_t c0;     // Parametrization on drift time versus distance
    Double_t c1;     // and impact angle (alpha):
    Double_t c2;     // driftTime = c0 + c1*alpha + c2*dist + c3*alpha*dist
    Double_t c3;
    Double_t c0err;  // Parametrization on drift time error versus distance
    Double_t c1err;  // and impact angle (alpha):
    Double_t c2err;  // driftTimeErr =
    Double_t c3err;  //    c0err + c1err*alpha + c2err*dist + c3err*alpha*dist
  public:
    HMdcDriftTimeParBin(void);
    ~HMdcDriftTimeParBin(void) {}
    void setParam(Double_t k0,Double_t k1,Double_t k2,Double_t k3);
    void setC0(Double_t k0) {c0=k0;}
    void setC1(Double_t k1) {c1=k1;}
    void setC2(Double_t k2) {c2=k2;}
    void setC3(Double_t k3) {c3=k3;}
    void setC0Err(Double_t k0) {c0err=k0;}
    void setC1Err(Double_t k1) {c1err=k1;}
    void setC2Err(Double_t k2) {c2err=k2;}
    void setC3Err(Double_t k3) {c3err=k3;}

    inline Double_t calcTime(Double_t a,Double_t d);
    inline Double_t calcTimeErr(Double_t a,Double_t d);
    inline void coeffForDer(Double_t a,Double_t d, 
        Double_t& coeffDAlpha,Double_t& coeffDDist,Double_t& coeffDAlphaDDist);
    inline Double_t coeffDAlphaDPar(Double_t d);
    inline Double_t coeffDDistDPar(Double_t a);
    void print(void) const;
    void printTable(void) const;

  ClassDef(HMdcDriftTimeParBin,0) // parametrization of one bin
};

class HMdcDriftTimeParAngle : public TObjArray {
  protected:
  public:
    HMdcDriftTimeParAngle(Int_t nb=100);
    ~HMdcDriftTimeParAngle(void);
    HMdcDriftTimeParBin* at(Int_t b) const {return (HMdcDriftTimeParBin*)At(b);}
  private:

  ClassDef(HMdcDriftTimeParAngle,0) // array of HMdcDriftTimeParBin objects
};

class HMdcDriftTimeParMod : public TObjArray {
  protected:
    Int_t    nAngleBins;    // number of angle bins
    Int_t    lAngleBin;     // index of last bin in angle array (= nAngleBins-1)
    Double_t angleBinSize;  // angle bin size
    Int_t    nDistBins;     // number of distance bins
    Int_t    lDistBin;      // index of last bin in dist. array (= nDistBins-1)
    Double_t distBinSize;   // distance bin size
    Double_t lastBinDist;   // distance for last bin (=lDistBin*distBinSize)
    Double_t slopeOutside;  // slope for outside of table tracks
  public:
    HMdcDriftTimeParMod(Int_t nAnBins=18, Int_t nDsBins=100);
    ~HMdcDriftTimeParMod(void);
    Int_t    getNAngleBins(void) const   {return nAngleBins;}
    Double_t getAngleBinSize(void) const {return angleBinSize;}
    Int_t    getNDistBins(void) const    {return nDistBins;}
    Double_t getDistBinSize(void) const  {return distBinSize;}
    inline HMdcDriftTimeParBin* getBin(Double_t a, Double_t d);
    inline Int_t    calcAngleBinInd(Double_t a) const;
    inline Int_t    calcDistBinInd(Double_t d) const;
    inline Double_t calcTime(Double_t a,Double_t d);
    inline Double_t calcTimeErr(Double_t a,Double_t d);
    Bool_t testSizes(Int_t anSz, Int_t dsSz) const;
    void setDistBinSize(Double_t dBSize);
    HMdcDriftTimeParAngle* at(Int_t a) const {
        return (HMdcDriftTimeParAngle*)At(a);}
    Bool_t initContainer(HMdcCal2ParModSim& fC2PModS,Double_t slOut);
    void print(void);
  private:
    void fillDriftTime(HMdcCal2ParAngleSim& rAng1,HMdcCal2ParAngleSim& rAng2,
        HMdcDriftTimeParBin* bin,Int_t anBin, Int_t dsBin);
    void fillDriftTimeErr(HMdcCal2ParAngleSim& rAng1,HMdcCal2ParAngleSim& rAng2,
        HMdcDriftTimeParBin* bin,Int_t anBin, Int_t dsBin);
    
  ClassDef(HMdcDriftTimeParMod,0) // array of HMdcDriftTimeParAngle objects
};

class HMdcDriftTimeParSec : public TObjArray {
  // array of pointers of type HMdcDriftTimeParMod
  protected:
    Int_t sector;
  public:
    HMdcDriftTimeParSec(void);
    ~HMdcDriftTimeParSec(void);
    inline HMdcDriftTimeParBin* getBin(Int_t m, Double_t a, Double_t d);
    Bool_t initContainer(HMdcCal2ParSecSim& fC2PSecS,Int_t sec,Double_t slOut);
    HMdcDriftTimeParMod* at(Int_t m) const {return (HMdcDriftTimeParMod*)At(m);}
    inline Double_t calcTime(Int_t m,Double_t a,Double_t d);
    inline Double_t calcTimeErr(Int_t m,Double_t a,Double_t d);
    void print(void);
  private:

  ClassDef(HMdcDriftTimeParSec,0) // array of HMdcDriftTimeParMod objects
};

class HMdcDriftTimePar : public TObjArray {
  // array of pointers of type HMdcDriftTimeParSec
  protected:
    HMdcCal2ParSim* pCal2ParSim;   // pointer to the HMdcCal2ParSim parameters
  public:
    HMdcDriftTimePar(void);
    ~HMdcDriftTimePar(void);
    inline HMdcDriftTimeParBin* getBin(Int_t s,Int_t m,Double_t a,Double_t d);
    Bool_t initContainer(void);
    HMdcDriftTimeParSec* at(Int_t s) const {return (HMdcDriftTimeParSec*)At(s);}
    inline Double_t calcTime(Int_t s,Int_t m,Double_t a,Double_t d);
    inline Double_t calcTimeErr(Int_t s,Int_t m,Double_t a,Double_t d);
    void print(void);
  private:

  ClassDef(HMdcDriftTimePar,0) // array of HMdcDriftTimeParSec objects
};

//==================================================

inline Double_t HMdcDriftTimeParBin::calcTime(Double_t a,Double_t d) {
  if(a<0.) a=-a;
  if(d<0.) d=-d;
  return c0+c1*a+c2*d+c3*a*d;
}

inline void HMdcDriftTimeParBin::coeffForDer(Double_t a,Double_t d,
    Double_t& coeffDAlpha,Double_t& coeffDDist,Double_t& coeffDAlphaDDist) {
  // For calcul. of analytical derivative
  // dDrTime/dPar=coeffDAlpha*dAlpha/dPar + coeffDDist*dDist/dPar
  coeffDAlpha=c1+c3*d;
  coeffDDist=c2+c3*a;
  coeffDAlphaDDist=c3;  // =coeffDDistDAlpha
}

inline Double_t HMdcDriftTimeParBin::coeffDAlphaDPar(Double_t d) {
  // return coeff=dDrTime/dDist
  return c1+c3*d;
}

inline Double_t HMdcDriftTimeParBin::coeffDDistDPar(Double_t a) {
  // return coeff=dDrTime/dDist
  return c2+c3*a;
}

inline Double_t HMdcDriftTimeParMod::calcTime(Double_t a,Double_t d) {
  return getBin(a,d)->calcTime(a,d);
}

inline Double_t HMdcDriftTimeParSec::calcTime(Int_t m, Double_t a, Double_t d) {
  return getBin(m,a,d)->calcTime(a,d);
}

inline Double_t HMdcDriftTimePar::calcTime(Int_t s, Int_t m,
    Double_t a,Double_t d) {
  return getBin(s,m,a,d)->calcTime(a,d);
}


inline Double_t HMdcDriftTimeParBin::calcTimeErr(Double_t a,Double_t d) {
  if(a<0.) a=-a;
  if(d<0.) d=-d;
  return c0err+c1err*a+c2err*d+c3err*a*d;
}

inline Double_t HMdcDriftTimeParMod::calcTimeErr(Double_t a,Double_t d) {
  return getBin(a,d)->calcTimeErr(a,d);
}

inline Double_t HMdcDriftTimeParSec::calcTimeErr(Int_t m,
    Double_t a,Double_t d) {
  return getBin(m,a,d)->calcTimeErr(a,d);
}

inline Double_t HMdcDriftTimePar::calcTimeErr(Int_t s, Int_t m,
    Double_t a,Double_t d) {
  return getBin(s,m,a,d)->calcTimeErr(a,d);
}

inline Int_t HMdcDriftTimeParMod::calcAngleBinInd(Double_t a) const {
  a=90.-a;
  if(a<0.) a=-a;
  Int_t binInd=(a<=lAngleBin*angleBinSize) ? Int_t(a/angleBinSize):lAngleBin;
  return (binInd<nAngleBins) ? binInd:lAngleBin;
}

inline Int_t HMdcDriftTimeParMod::calcDistBinInd(Double_t d) const {
  if(d<0.) d=-d;
  Int_t binInd=(d<=lastBinDist) ? Int_t(d/distBinSize):lDistBin;
  return (binInd<nDistBins) ? binInd:lDistBin;
}

inline HMdcDriftTimeParBin* HMdcDriftTimeParMod::getBin(Double_t a,Double_t d) {
  HMdcDriftTimeParAngle* pAngl=at(calcAngleBinInd(a));
  if(pAngl) return pAngl->at(calcDistBinInd(d));
  return 0;
}

inline HMdcDriftTimeParBin* HMdcDriftTimeParSec::getBin(Int_t m,
    Double_t a, Double_t d) {
  HMdcDriftTimeParMod* pMod=at(m);
  if(pMod) return pMod->getBin(a,d);
  return 0;
}

inline HMdcDriftTimeParBin* HMdcDriftTimePar::getBin(Int_t s, Int_t m,
    Double_t a, Double_t d) {
  HMdcDriftTimeParSec* pSec=at(s);
  if(pSec) return pSec->getBin(m,a,d);
  return 0;
}

#endif  /* HMDCDRIFTTIMEPAR_H */

