#ifndef HMDCHITCLUS_H
#define HMDCHITCLUS_H

#include "hmdchitsim.h"

class HMdcClusInf : public TObject {
protected:
  Bool_t  isHitFitted;  // =kFALSE - hit filled by cluster
  Int_t   clusSize;     // number of bins in cluster
  Short_t nMergedClus;  // number of merged clusters
  Short_t nDrTimes;     // number of drift times in cluster
  Float_t shapePar1;    // ???
  Float_t shapePar2;    // ???
public:
  HMdcClusInf(void) : clusSize(0), nMergedClus(0), nDrTimes(0) {;}
  ~HMdcClusInf(void) {;}
  void clear(void) {clusSize=nMergedClus=nDrTimes=0;}

  void setIsFitted(void) {isHitFitted=kTRUE;}
  void setIsNotFitted(void) {isHitFitted=kFALSE;}
  void setFitStatus(const Bool_t st) {isHitFitted=st;}
  void setClustSize(const Int_t sz) {clusSize=sz;}
  void setNMergClust(const Short_t nmc) {nMergedClus=nmc;}
  void setNDrTimes(const Short_t ndt) {nDrTimes=ndt;}
  void setShapePar1(const Float_t par){shapePar1=par;}
  void setShapePar2(const Float_t par){shapePar2=par;}
  void setShapePar(const Float_t p1,const Float_t p2)
    {shapePar1=p1; shapePar2=p2;}

  Bool_t  isFitted(void) {return isHitFitted;}
  Int_t   getClustSize(void) {return clusSize;}
  Short_t getNMergClust(void) {return nMergedClus;}
  Short_t getNDrTimes(void) {return nDrTimes;}
  Float_t getShapePar1(void) {return shapePar1;}
  Float_t getShapePar2(void) {return shapePar2;}

  void print(void);

  ClassDef(HMdcClusInf,1) // hit in a MDC chamber
};

class HMdcHitClus : public HMdcHit {
protected:
  HMdcClusInf clus;     // information about cluster
public:
  HMdcHitClus(void) {;}
  ~HMdcHitClus(void) {;}
  void clear(void) {clus.clear(); HMdcHit::Clear();}
  void print(void) {HMdcHit::print(); clus.print();}
  HMdcClusInf* getClusInf(void) {return &clus;}
  
  ClassDef(HMdcHitClus,1) // hit in a MDC chamber
};

class HMdcHitSimClus : public HMdcHitSim {
protected:
  HMdcClusInf clus;     // information about cluster
public:
  HMdcHitSimClus(void) {;}
  ~HMdcHitSimClus(void) {;}
  void clear(void) {clus.clear(); HMdcHitSim::clear();}
  void print(void) {HMdcHitSim::print(); clus.print();}
  HMdcClusInf* getClusInf(void) {return &clus;}

  ClassDef(HMdcHitSimClus,1) // hit in a MDC chamber
};
  
#endif  /* HMdcHitClus_H */









