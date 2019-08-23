#ifndef HMDCTRACKFITPAR_H
#define HMDCTRACKFITPAR_H

#include "hparset.h"
#include "hmdcdetector.h"

#include <stdio.h>
#include <TObjArray.h>


class HMdcTrackFitParMod : public TObject {
 protected:
  Float_t driftVelocityMDC;      // Drift Velocity MDC1
  Int_t maxIteration;         // Maximal number of iterations
  Double_t step1,step2,step3,step4,step5,step6;
  Double_t weight1,weight2,weight3,weight4,weight5,weight6;
  Double_t stepNorm;          // Initial value of step common for all parameters.
  Double_t tukeyConstant;     // Tukey constant




public:
  HMdcTrackFitParMod() { clear(); }
  HMdcTrackFitParMod(HMdcTrackFitParMod &tc) {fill(tc);}
  ~HMdcTrackFitParMod() {}

  void setDriftVelocityMDC(Float_t v){driftVelocityMDC=v;}

  void setMaxIteration(Int_t nIt){maxIteration=nIt;}

  void setStep1(Double_t s){step1=s;}
  void setStep2(Double_t s){step2=s;}
  void setStep3(Double_t s){step3=s;}
  void setStep4(Double_t s){step4=s;}
  void setStep5(Double_t s){step5=s;}
  void setStep6(Double_t s){step6=s;}

  void setWeight1(Double_t w){weight1=w;}
  void setWeight2(Double_t w){weight2=w;}
  void setWeight3(Double_t w){weight3=w;}
  void setWeight4(Double_t w){weight4=w;}
  void setWeight5(Double_t w){weight5=w;}
  void setWeight6(Double_t w){weight6=w;}

  void setStepNorm(Double_t N){stepNorm=N;}
  void setTukeyConstant(Double_t tc){tukeyConstant=tc;}

  Float_t getDriftVelocityMDC(void) {return driftVelocityMDC;}

  Int_t getMaxIteration(void){return maxIteration;}

  Double_t getStep1(){return step1;}
  Double_t getStep2(){return step2;}
  Double_t getStep3(){return step3;}
  Double_t getStep4(){return step4;}
  Double_t getStep5(){return step5;}
  Double_t getStep6(){return step6;}
  void getSteps(Double_t* steps){
    steps[0]=step1;
    steps[1]=step2;
    steps[2]=step3;
    steps[3]=step4;
    steps[4]=step5;
    steps[5]=step6;
  }

  Double_t getWeight1(){return weight1;}
  Double_t getWeight2(){return weight2;}
  Double_t getWeight3(){return weight3;}
  Double_t getWeight4(){return weight4;}
  Double_t getWeight5(){return weight5;}
  Double_t getWeight6(){return weight6;}
  void getWeights(Double_t* weights){
    weights[0]=weight1;
    weights[1]=weight2;
    weights[2]=weight3;
    weights[3]=weight4;
    weights[4]=weight5;
    weights[5]=weight6;
  }

  Double_t getStepNorm(void){return stepNorm;}
  Double_t getTukeyConstant(void){return tukeyConstant;}



  void fill(HMdcTrackFitParMod &tc) {

    driftVelocityMDC=tc.driftVelocityMDC;

    maxIteration=tc.maxIteration;      

    step1=tc.step1;
    step2=tc.step2;
    step3=tc.step3;
    step4=tc.step4;
    step5=tc.step5;
    step6=tc.step6;

    weight1=tc.weight1;
    weight2=tc.weight2;
    weight3=tc.weight3;
    weight4=tc.weight4;
    weight5=tc.weight5;
    weight6=tc.weight6;

    stepNorm=tc.stepNorm;

    tukeyConstant=tc.tukeyConstant;

  }

  void fill(Float_t dVMDC,
	    Int_t maxIt,
            Double_t stp1,Double_t stp2,Double_t stp3,Double_t stp4,Double_t stp5,Double_t stp6,
            Double_t wt1,Double_t wt2,Double_t wt3,Double_t wt4,Double_t wt5,Double_t wt6,
	    Double_t stN,
            Double_t tukey)
  {
      driftVelocityMDC=dVMDC;

      maxIteration=maxIt;

      step1=stp1;
      step2=stp2;
      step3=stp3;
      step4=stp4;
      step5=stp5;
      step6=stp6;

      weight1=wt1;
      weight2=wt2;
      weight3=wt3;
      weight4=wt4;
      weight5=wt5;
      weight6=wt6;

      stepNorm=stN;
      tukeyConstant=tukey;     // Tukey constant
  }
  void clear(void) {
      fill(0.,
	   0,
           0., 0., 0., 0.,0.,0.,
           0., 0., 0., 0.,0.,0.,
           0.,0.
	  );
  }
  void print(void) {

      printf("Dift Velocity: (%f,)\n  (%i)\n,",
	     driftVelocityMDC,maxIteration);
      printf("step : (%f, %f, %f, %f, %f, %f,)\n",
	     step1,step2,step3,step4,step5,step6);
      printf("weight : (%f, %f, %f, %f, %f, %f,)\n",
	     weight1,weight2,weight3,weight4,weight5,weight6);
      printf("stepNorm: (%f), tuckey:(%f)\n",
	     stepNorm,tukeyConstant);

  }
  ClassDef(HMdcTrackFitParMod,1)
};

class HMdcTrackFitParSec : public TObject {
 protected:
  TObjArray fMod;
 public:
  HMdcTrackFitParSec(Int_t mods=4) : fMod(mods) {
    for (Int_t i=0; i<mods; i++) fMod.AddAt(new HMdcTrackFitParMod,i);
  }
  ~HMdcTrackFitParSec(void) {fMod.Delete();}
  HMdcTrackFitParMod& operator[](Int_t i) {
    return *( static_cast<HMdcTrackFitParMod*>(fMod[i]) );
  }
  Int_t getSize() {return fMod.GetEntries();}
  void getStepsSeg1(Double_t* steps){
    (*this)[0].getSteps(steps);
    steps[0]+=(*this)[1].getStep1();
    steps[1]+=(*this)[1].getStep2();
    steps[2]+=(*this)[1].getStep3();
    steps[3]+=(*this)[1].getStep4();
    steps[4]+=(*this)[1].getStep5();
    steps[5]+=(*this)[1].getStep6();
    for(Int_t i=0;i<6;i++) steps[i]*=0.5; // ???
  }
  void getStepsSeg2(Double_t* steps){
    (*this)[2].getSteps(steps);
    steps[0]+=(*this)[3].getStep1();
    steps[1]+=(*this)[3].getStep2();
    steps[2]+=(*this)[3].getStep3();
    steps[3]+=(*this)[3].getStep4();
    steps[4]+=(*this)[3].getStep5();
    steps[5]+=(*this)[3].getStep6();
    for(Int_t i=0;i<6;i++) steps[i]*=0.5; // ???
  }
  void getWeightsSeg1(Double_t* weights){
    (*this)[0].getWeights(weights);
    weights[0]+=(*this)[1].getWeight1();
    weights[1]+=(*this)[1].getWeight2();
    weights[2]+=(*this)[1].getWeight3();
    weights[3]+=(*this)[1].getWeight4();
    weights[4]+=(*this)[1].getWeight5();
    weights[5]+=(*this)[1].getWeight6();
    for(Int_t i=0;i<6;i++) weights[i]*=0.5; // ???
  }
  void getWeightsSeg2(Double_t* weights){
    (*this)[2].getWeights(weights);
    weights[0]+=(*this)[3].getWeight1();
    weights[1]+=(*this)[3].getWeight2();
    weights[2]+=(*this)[3].getWeight3();
    weights[3]+=(*this)[3].getWeight4();
    weights[4]+=(*this)[3].getWeight5();
    weights[5]+=(*this)[3].getWeight6();
    for(Int_t i=0;i<6;i++) weights[i]*=0.5; // ???
  }
  Int_t getMaxIterationSeg1(void){
    return ((*this)[0].getMaxIteration()+(*this)[1].getMaxIteration())/2;
  }
  Int_t getMaxIterationSeg2(void){
    return ((*this)[2].getMaxIteration()+(*this)[3].getMaxIteration())/2;
  }
  Double_t getStepNormSeg1(void){
    return ((*this)[0].getStepNorm()+(*this)[1].getStepNorm())*0.5;
  }
  Double_t getStepNormSeg2(void){
    return ((*this)[2].getStepNorm()+(*this)[3].getStepNorm())*0.5;
  }
  Double_t getTukeyConstantSeg1(void){
    return ((*this)[0].getTukeyConstant()+(*this)[1].getTukeyConstant())*0.5;
  }
  Double_t getTukeyConstantSeg2(void){
    return ((*this)[2].getTukeyConstant()+(*this)[3].getTukeyConstant())*0.5;
  }
  ClassDef(HMdcTrackFitParSec,1)
};

class HMdcTrackFitPar : public HParSet {
protected:
  TObjArray fSecs; // Sectors containins modules containing cuts
  Bool_t isContainer;  
  void setNameTitle();
  HMdcDetector *fMdc;
  Int_t lineCounter;
  Int_t lineCounterWrite;

public:
  HMdcTrackFitPar(Int_t secs=6,Int_t mods=4);
  ~HMdcTrackFitPar(void) {fSecs.Delete();}

  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output) ;
  void readline(const char* buf, Int_t* set) ;
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t);

  HMdcTrackFitParSec& operator[](Int_t i) {
    return *( static_cast<HMdcTrackFitParSec*>(fSecs[i]) );
  }
  Int_t getSize() {return fSecs.GetEntries();}
  Int_t getNumberOfLines() {return 4;}

  void clear(void);

  void printParam(void) {
    printf ("TimeCut:\n");
    for (Int_t i=0;i<getSize();i++) {
      HMdcTrackFitParSec &sector = (*this)[i];
      for (Int_t m=0; m<sector.getSize(); m++) {
	HMdcTrackFitParMod &mod = sector[m];
	printf("%i %i ",i,m);
	mod.print();
      }
    }
  }

  ClassDef(HMdcTrackFitPar,1) 
};

#endif  /*!HMDCTRACKFITPAR_H*/
