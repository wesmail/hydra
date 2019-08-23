#ifndef HTOFCLUSTERFPAR_H
#define HTOFCLUSTERFPAR_H

#include "hparcond.h"
#include <iostream.h>
#include <fstream.h>

class HParamList;

class HTofClusterFPar : public HParCond {
private:
  Float_t diffTmax;       //Maximal time distance of two hits.
  Float_t diffXmax;       //Maximal x position distance of two hits.
  Float_t mipLimit;       //Time interval limit for probability calculation.
  Float_t mL1all;         //Most probable value of 1st Landau (all).
  Float_t sL1all;         //Sigma of 1st Landau (all).
  Float_t mL2all;         //Most probable value of 2nd Landau (all).
  Float_t sL2all;         //Sigma of 2nd Landau (all).
  Float_t ratCall1;       //Constant ratio of Landau functions (all).
  Float_t mL1lep;         //Most probable value of 1st Landau (lep).
  Float_t sL1lep;         //Sigma of 1st Landau (lep).
  Float_t mL2lep;         //Most probable value of 2nd Landau (lep).
  Float_t sL2lep;         //Sigma of 2nd Landau (lep).
  Float_t ratClep1;       //Constant ratio of Landau functions (lep).
  Float_t diffPmax;       //Maximal momentum difference between tracks (for kick-plane part).
  Float_t lossEmax;       //Maximal energy loss for cluster of size = 2 (for kick-plane part).

public:
  HTofClusterFPar(const char* name="TofClusterFPar",
                  const char* title="Parameter container for the cluster finder",
                  const char* context="TofCluNormalBias");
  virtual ~HTofClusterFPar(void);
  void clear(void);
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  void putParams(HParamList*);
  Bool_t getParams(HParamList*);
  Float_t getMaxTDiff(void)      { return diffTmax; }
  Float_t getMaxXDiff(void)      { return diffXmax; }
  Float_t getMIPLimit(void) { return mipLimit; }
  Float_t getMPV1(char* aset="all") { return (aset=="lep")? mL1lep:mL1all; }
  Float_t getSigma1(char* aset="all") { return (aset=="lep")? sL1lep:sL1all; }
  Float_t getMPV2(char* aset="all") { return (aset=="lep")? mL2lep:mL2all; }
  Float_t getSigma2(char* aset="all") { return (aset=="lep")? sL2lep:sL2all; }
  Float_t getConstRatio(char* aset="all") { return (aset=="lep")? ratClep1:ratCall1; }
  Float_t getMaxPDiff(void)      { return diffPmax; }
  Float_t getMaxELoss(void)      { return lossEmax; }
  void readFrom(const HTofClusterFPar &par);
//  Int_t writeFile(fstream *pFile);
//  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
//  void printTo(ostream &out);
//  void readFrom(istream &in);
  ClassDef(HTofClusterFPar,4)
};

#endif
