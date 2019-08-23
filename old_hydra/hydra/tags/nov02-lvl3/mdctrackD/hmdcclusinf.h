#ifndef HMDCCLUSINF_H
#define HMDCCLUSINF_H

#include "TObject.h"

class HMdcClusInf : public TObject {
protected:
  Char_t fitStatus;       // 0 - HMdcHit filled by cluster, 1 - by fit
  Char_t levelClFinding;  // level of cl.finding (minimal num. of layers)

  Char_t nModsClus;       // number of modules in cluster
  Char_t nModsFit;        // number of modules in fit

  Short_t clusSize;       // number of bins in cluster
  Short_t nMergClus;      // number of merged clusters
  Short_t nDrTimes;       // number of drift times in cluster
  Short_t indClus;        // index of HMdcClus container
  Short_t indClusFit;     // index of HMdcClusFit container
  Float_t sigma1;         // sigma1,sigma2,alpha - cluster shape
  Float_t sigma2;         // sigma1 - long axis, sigma2 - short axis
  Float_t alpha;          // alpha - angle between long axis of cluster and X
public:
  HMdcClusInf(void) : clusSize(0), nMergClus(0), nDrTimes(0), indClus(-1),
    indClusFit(-1) {;}
  ~HMdcClusInf(void) {;}
  void clear(void) {
    clusSize=nMergClus=nDrTimes=0;
    indClus=indClusFit=-1;
  }
  void setIsFitted(void) {fitStatus=1;}
  void setIsNotFitted(void) {fitStatus=0;}
  void setFitStat(const Char_t st) {fitStatus=st;}
  void setLevelClFinding(const Char_t lv) {levelClFinding=lv;}
  void setNModInCl(const Char_t nm) {nModsClus=nm;}
  void setNModInFit(const Char_t nm) {nModsFit=nm;}
  void setClusSize(const Short_t sz) {clusSize=sz;}
  void setNMergClust(const Short_t nmc) {nMergClus=nmc;}
  void setNDrTimes(const Short_t ndt) {nDrTimes=ndt;}
  void setClusIndex(const Short_t ind) {indClus=ind;}
  void setClusFitIndex(const Short_t ind) {indClusFit=ind;}
  void setSigma1(const Float_t par){sigma1=par;}
  void setSigma2(const Float_t par){sigma2=par;}
  void setAlpha(const Float_t al) {alpha=al;}
  void setShape(const Float_t s1,const Float_t s2,const Float_t al)
    {sigma1=s1; sigma2=s2;alpha=al;}

  Char_t  getFitStat(void) {return fitStatus;}
  Char_t  getLevelClFinding(void) {return levelClFinding;}
  Char_t  getNModInCl(void) {return nModsClus;}
  Char_t  getNModInFit(void) {return nModsFit;}
  Int_t   getClusSize(void) {return clusSize;}
  Short_t getNMergClust(void) {return nMergClus;}
  Short_t getNDrTimes(void) {return nDrTimes;}
  Short_t getClusIndex(void) {return indClus;}
  Short_t getClusFitIndex(void) {return indClusFit;}
  Float_t getSigma1(void) {return sigma1;}
  Float_t getSigma2(void) {return sigma2;}
  Float_t getAlpha(void) {return alpha;}
  void getShape(Float_t& s1,Float_t& s2,Float_t& al)
    {s1=sigma1; s2=sigma2; al=alpha;}
  Float_t getRatio(void) {return (sigma1>0.) ? sigma2/sigma1 : 0.;}

  void print(void);

  ClassDef(HMdcClusInf,1) // hit in a MDC chamber
};

#endif  /* HMDCCLUSINF_H */









