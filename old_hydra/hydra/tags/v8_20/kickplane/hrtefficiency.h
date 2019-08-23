#ifndef HRTEFFICIENCY_H
#define HRTEFFICIENCY_H

#include "TObject.h"
#include "TString.h"
#include <math.h>
#include "TNtuple.h"
#include "TGraphErrors.h"

class HRtMatchingPar;

//Manages efficiency in function of p,theta,phi

class HRtEfficiencyMatrix : public TObject {
public:
  HRtEfficiencyMatrix(void);
  ~HRtEfficiencyMatrix(void);
  HRtEfficiencyMatrix(const HRtEfficiencyMatrix &m);
  void setDimension(UInt_t binsP,UInt_t binsTheta,UInt_t binsPhi);
  void setPRange(Float_t minP,Float_t maxP);
  void setThetaRange(Float_t minTheta, Float_t maxTheta);
  void setPhiRange(Float_t minPhi,Float_t maxPhi);
  Float_t &binContent(Float_t p, Float_t theta, Float_t phi);
  Float_t &bin(UInt_t i, UInt_t j, UInt_t k);
  Float_t &lastBin(void) { return fValue[fBinsTotal]; }
  Float_t &errBinContent(Float_t p, Float_t theta, Float_t phi);
  Float_t &errBin(UInt_t i, UInt_t j, UInt_t k); 
  Float_t &errLastBin(void) { return fError[fBinsTotal]; }
  void multiply(Float_t num);
  void add(Float_t num);
  void computePoissonErrors(void);
  void divide(HRtEfficiencyMatrix &num,HRtEfficiencyMatrix &den);
  void multiply(HRtEfficiencyMatrix &num,HRtEfficiencyMatrix &den);
  Bool_t isCompatible(HRtEfficiencyMatrix &m);
  Bool_t zero(void);
  Float_t sum(void);
  Float_t integral(void);
  UInt_t getBinsTotal(void) const { return fBinsTotal; }
  UInt_t getBinsTheta(void) const { return fBinsTheta; }
  UInt_t getBinsPhi(void) const { return fBinsPhi; }
  UInt_t getBinsP(void) const { return fBinsP; }
  inline Float_t getP(UInt_t binP) const;
  inline Float_t getTheta(UInt_t binTheta) const;
  inline Float_t getPhi(UInt_t binPhi) const;
  inline Int_t getBinP(Float_t p) const;
  inline Int_t getBinTheta(Float_t theta) const;
  inline Int_t getBinPhi(Float_t phi) const;
  inline Float_t getMinP(void) const { return fMinP; }
  inline Float_t getMaxP(void) const { return fMaxP; }
  inline Float_t getMinTheta(void) const { return fMinTheta; }
  inline Float_t getMaxTheta(void) const { return fMaxTheta; }
  inline Float_t getMinPhi(void) const { return fMinPhi; }
  inline Float_t getMaxPhi(void) const { return fMaxPhi; }
protected:
  UInt_t fSize;
  Float_t *fValue; //[fSize]
  Float_t *fError; //[fSize]
  UInt_t fBinsP, fBinsTheta, fBinsPhi, fBinsTotal;
  Float_t fMinP, fMaxP, fBinSizeP;
  Float_t fMinTheta, fMaxTheta, fBinSizeTheta;
  Float_t fMinPhi, fMaxPhi, fBinSizePhi;
 public:
  ClassDef(HRtEfficiencyMatrix,1)
};


class HRtEfficiencyKit : public TObject {
public:
  ~HRtEfficiencyKit(void) {}
  TGraph *plotEff(TString &var);
  TGraph *plotCor(TString &var);
  TGraph *plotNoise(TString &var);
  TGraph *plotEff(TString &var, Float_t var2, Float_t var3);
  TGraph *plotCor(TString &var, Float_t var2, Float_t var3);
  TGraph *plotNoise(TString &var, Float_t var2, Float_t var3);
  Float_t getAverageEfficiency(void);
  Float_t getEfficiency(void);
  Float_t getAbsoluteEfficiency(void);
  Float_t getNoiseLevel(void);
  Float_t getAverageNoiseLevel(void);
  Float_t getAbsoluteNoiseLevel(void);
  void setMatrixDimension(UInt_t binsP,UInt_t binsTheta, UInt_t binsPhi);
  void setPRange(Float_t minP,Float_t maxP);
  void setThetaRange(Float_t minTheta, Float_t maxTheta);
  void setPhiRange(Float_t minPhi,Float_t maxPhi);
  Bool_t processFile(Text_t fileName[], Stat_t maxEntries=kMaxInt);
  void setMinValid(Float_t v) { fMinValid = v; }
  void setMaxValid(Float_t v) { fMaxValid = v; }
  void printReport(void);
protected:
  TGraph *plot(HRtEfficiencyMatrix &matrix, TString &var, 
	       Float_t var2,Float_t var3);
  TGraph *plot(HRtEfficiencyMatrix &m, TString &var);
  virtual void preRead(TNtuple *) {}
  virtual void postRead(void) {} //Llamado para asignar isReconstructed
  TString bnGeantMomentum; //Branch name
  TString bnGeantTheta;   
  TString bnGeantPhi;
  TString bnValidity;
  TString bnRecMomentum;
  TString bnRecTheta;
  TString bnRecPhi;
  TString stNtuple; //string ntuple name

  Bool_t isReconstructed; //Indica si la traza sera reconstruida
  Float_t fMinValid;      //  
  Float_t fMaxValid;      //Max value of valid. To elliminate correlated noise

  HRtEfficiencyMatrix fMatchedGood;   //Reconstructed tracks which are good
  HRtEfficiencyMatrix fGeant;
  HRtEfficiencyMatrix fMatchedTotal;  //p,theta,phi are the reconstructed ones
  HRtEfficiencyMatrix fEfficiency;    
  HRtEfficiencyMatrix fCorrection;
 public:
  ClassDef(HRtEfficiencyKit,1)
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class HRtEfficiencyKitMeta : public HRtEfficiencyKit {
 public:
  HRtEfficiencyKitMeta(void);
  ~HRtEfficiencyKitMeta(void) {}
  void setCutValues(Float_t d,Float_t dkick,Float_t dPhi,Float_t xPull,Float_t dPnorm);
 protected:
  void preRead(TNtuple *);
  void postRead(void); 
  Float_t fMaxD;
  Float_t fMaxDKick;
  Float_t fMaxDPhi;
  Float_t fMaxXPull;
  Float_t fMaxDPnorm;
  Float_t fD,fDKick,fXPull,fDPhi,fDPnorm;
 public:
  ClassDef(HRtEfficiencyKitMeta,1)
};

class HRtEfficiencyKitMeta2 : public HRtEfficiencyKit { 
  public: 
    HRtEfficiencyKitMeta2(void); 
    ~HRtEfficiencyKitMeta2(void) {} 
    void loadParameters(Text_t file[]); 
  protected: 
    void preRead(TNtuple *); 
    void postRead(void);  

    Float_t fD,fDKick,fXPull,fDPhi,fDPnorm; 
    Float_t fMatchVar[4];
    HRtMatchingPar *fMatchingPar;
    TString fParamFile;
  public: 
    ClassDef(HRtEfficiencyKitMeta2,1) 
}; 

class HRtEfficiencyKitMdc : public HRtEfficiencyKit {
 public:
  HRtEfficiencyKitMdc(void);
  ~HRtEfficiencyKitMdc(void) {}
  void setCutValues(Float_t d,Float_t dkick,Float_t dPhi);
 protected:
  void preRead(TNtuple *);
  void postRead(void); 
  Float_t fMaxD;
  Float_t fMaxDKick;
  Float_t fMaxDPhi;
  Float_t fD,fDKick,fDPhi;
 public:
  ClassDef(HRtEfficiencyKitMdc,1)
};

class HRtEfficiencyKitMdc2 : public HRtEfficiencyKit {
 public:
  HRtEfficiencyKitMdc2(void);
  ~HRtEfficiencyKitMdc2(void) {}
  void loadParameters(Text_t file[]);
 protected:
  void preRead(TNtuple *);
  void postRead(void); 
  HRtMatchingPar *fMatchingPar;
  Float_t fPar[3]; //d, dKick, dphi
  TString fParamFile;
 public:
  ClassDef(HRtEfficiencyKitMdc2,1)
};

////// Inlines /////////////////////////////////////////////////////

inline Float_t HRtEfficiencyMatrix::getP(UInt_t binP) const {
  return fMinP + binP*fBinSizeP + fBinSizeP/2.;
}

inline Float_t HRtEfficiencyMatrix::getTheta(UInt_t binTheta) const {
  return fMinTheta + binTheta*fBinSizeTheta + fBinSizeTheta/2.;
}

inline Float_t HRtEfficiencyMatrix::getPhi(UInt_t binPhi) const {
  return fMinPhi + binPhi*fBinSizePhi + fBinSizePhi/2.;
}

inline Int_t HRtEfficiencyMatrix::getBinP(Float_t p) const {
  return int(floor((p - fMinP) /  fBinSizeP)); 
}

inline Int_t HRtEfficiencyMatrix::getBinTheta(Float_t theta) const {
  return int(floor((theta - fMinTheta)) / fBinSizeTheta); 
}

inline Int_t HRtEfficiencyMatrix::getBinPhi(Float_t phi) const {
  return int(floor((phi - fMinPhi)) / fBinSizePhi); 
}
#endif
