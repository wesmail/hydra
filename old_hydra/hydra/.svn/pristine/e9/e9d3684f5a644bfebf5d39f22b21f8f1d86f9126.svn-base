#ifndef HMDCTRACKFITPAR_H
#define HMDCTRACKFITPAR_H

#include "hparasciifileio.h"
#include "hparrootfileio.h"
#include "fstream.h"
#include "hparset.h"

class HMdcTrackFitPar : public  HParSet {

 protected:
  Float_t driftVelocityMDC1;      // Drift Velocity MDC1
  Float_t driftVelocityMDC2;      // Drift Velocity MDC2
  Float_t driftVelocityMDC3;      // Drift Velocity MDC3
  Float_t driftVelocityMDC4;      // Drift Velocity MDC4
  Int_t maxNMdcTimes;         // Maximal number of times in cluster 
  Int_t maxIteration;         // Maximal number of iterations
  Double_t step[6];           // Initial step sizes for  parameters
  Double_t weight[6];         // Weights for parameters
  Double_t stepNorm;          // Initial value of step common for all parameters.
  Double_t xVertex,  
    yVertex,  zVertex;        // Vertex coordinates  
  Double_t particleVelocity;  // Tukey constant  
  Double_t tukeyConstant;     // Tukey constant  
    
 public:

  HMdcTrackFitPar(); 
  ~HMdcTrackFitPar();
  Bool_t init(HParAsciiFileIo* inp,Text_t* fname, Text_t* status="in" );
  Bool_t init(HParRootFileIo* inp,Text_t* fname, Text_t* status="in" );
  void setDriftVelocityMDC1(Float_t v){driftVelocityMDC1=v;}
  void setDriftVelocityMDC2(Float_t v){driftVelocityMDC2=v;}
  void setDriftVelocityMDC3(Float_t v){driftVelocityMDC3=v;}
  void setDriftVelocityMDC4(Float_t v){driftVelocityMDC4=v;}
  void setMaxNMdcTimes(Int_t nt){maxNMdcTimes=nt;}
  void setMaxIteration(Int_t nIt){maxIteration=nIt;}
  void setStep(Int_t i, Double_t s){step[i]=s;}
  void setWeight(Int_t i, Double_t w){weight[i]=w;}
  void setXVertex(Double_t x){xVertex=x;}
  void setYVertex(Double_t y){yVertex=y;}
  void setZVertex(Double_t z){zVertex=z;}
  void setParticleVelocity(Double_t v){particleVelocity=v;}
  void setTukeyConstant(Double_t tc){tukeyConstant=tc;}

  Float_t getDriftVelocityMDC1(void) {return driftVelocityMDC1;}
  Float_t getDriftVelocityMDC2(void) {return driftVelocityMDC2;}
  Float_t getDriftVelocityMDC3(void) {return driftVelocityMDC3;}
  Float_t getDriftVelocityMDC4(void) {return driftVelocityMDC4;}
  Int_t getMaxNMdcTimes(void) {return maxNMdcTimes;}
  Int_t getMaxIteration(void){return maxIteration;}
  Double_t* getStep(){return step;}
  Double_t getStep(Int_t i){return step[i];}
  Double_t* getWeight(){return weight;}
  Double_t getWeight(Int_t i){return weight[i];}
  Double_t getStepNorm(void){return stepNorm;}
  Double_t getXVertex(void){return xVertex;}
  Double_t getYVertex(void){return yVertex;}
  Double_t getZVertex(void){return zVertex;}
  Double_t get(void){return zVertex;}
  Double_t getParticleVelocity(void){return particleVelocity;}
  Double_t getTukeyConstant(void){return tukeyConstant;}

  ClassDef(HMdcTrackFitPar,1) // Parameters for fitting procedure
};

#endif  /*!HMDCTRACKFITPAR*/
