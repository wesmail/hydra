#ifndef HMDCTRACKFITPAR_H
#define HMDCTRACKFITPAR_H
#include "TArrayF.h"
#include "TArrayD.h"
#include "TArrayI.h"
#include "hparcond.h"

class HParamList;

class HMdcTrackFitPar : public HParCond {
protected:
    TArrayF driftVelocityMDC;  // Drift Velocity MDC1
    TArrayI maxIteration;      // Maximal number of iterations
    TArrayD step1;             // step
    TArrayD step2;             // step
    TArrayD step3;             // step
    TArrayD step4;             // step
    TArrayD step5;             // step
    TArrayD step6;             // step
    TArrayD weight1;           // weight
    TArrayD weight2;           // weight
    TArrayD weight3;           // weight
    TArrayD weight4;           // weight
    TArrayD weight5;           // weight
    TArrayD weight6;           // weight
    TArrayD stepNorm;          // Initial value of step common for all parameters.
    TArrayD tukeyConstant;     // Tukey constant

public:
    HMdcTrackFitPar(const char* name ="MdcTrackFitPar",
	      const char* title  ="parameters for Dubna track fitter",
	      const char* context="MdcTrackFitParProduction");
    ~HMdcTrackFitPar();
    Float_t  getDriftVelocityMDC(Int_t s,Int_t m)          {return driftVelocityMDC[s*4+m];}
    Int_t    getMaxIteration(Int_t s,Int_t m){return maxIteration[s*4+m];}
    Double_t getStep1(Int_t s,Int_t m){return step1[s*4+m];}
    Double_t getStep2(Int_t s,Int_t m){return step2[s*4+m];}
    Double_t getStep3(Int_t s,Int_t m){return step3[s*4+m];}
    Double_t getStep4(Int_t s,Int_t m){return step4[s*4+m];}
    Double_t getStep5(Int_t s,Int_t m){return step5[s*4+m];}
    Double_t getStep6(Int_t s,Int_t m){return step6[s*4+m];}
    void     getSteps(Int_t s,Int_t m,Double_t* steps){
	steps[0]=step1[s*4+m];
	steps[1]=step2[s*4+m];
	steps[2]=step3[s*4+m];
	steps[3]=step4[s*4+m];
	steps[4]=step5[s*4+m];
	steps[5]=step6[s*4+m];
    }

    Double_t getWeight1(Int_t s,Int_t m){return weight1[s*4+m];}
    Double_t getWeight2(Int_t s,Int_t m){return weight2[s*4+m];}
    Double_t getWeight3(Int_t s,Int_t m){return weight3[s*4+m];}
    Double_t getWeight4(Int_t s,Int_t m){return weight4[s*4+m];}
    Double_t getWeight5(Int_t s,Int_t m){return weight5[s*4+m];}
    Double_t getWeight6(Int_t s,Int_t m){return weight6[s*4+m];}
    void     getWeights(Int_t s,Int_t m,Double_t* weights){
	weights[0]=weight1[s*4+m];
	weights[1]=weight2[s*4+m];
	weights[2]=weight3[s*4+m];
	weights[3]=weight4[s*4+m];
	weights[4]=weight5[s*4+m];
	weights[5]=weight6[s*4+m];
    }

    Double_t getStepNorm(Int_t s,Int_t m)     {return stepNorm[s*4+m];}
    Double_t getTukeyConstant(Int_t s,Int_t m){return tukeyConstant[s*4+m];}


    void     setDriftVelocityMDC(Int_t s,Int_t m,Float_t v){driftVelocityMDC[s*4+m]=v;}
    void     setMaxIteration(Int_t nIt){maxIteration=nIt;}

    void     setStep1(Int_t s,Int_t m,Double_t st){step1[s*4+m]=st;}
    void     setStep2(Int_t s,Int_t m,Double_t st){step2[s*4+m]=st;}
    void     setStep3(Int_t s,Int_t m,Double_t st){step3[s*4+m]=st;}
    void     setStep4(Int_t s,Int_t m,Double_t st){step4[s*4+m]=st;}
    void     setStep5(Int_t s,Int_t m,Double_t st){step5[s*4+m]=st;}
    void     setStep6(Int_t s,Int_t m,Double_t st){step6[s*4+m]=st;}

    void     setWeight1(Int_t s,Int_t m,Double_t w){weight1[s*4+m]=w;}
    void     setWeight2(Int_t s,Int_t m,Double_t w){weight2[s*4+m]=w;}
    void     setWeight3(Int_t s,Int_t m,Double_t w){weight3[s*4+m]=w;}
    void     setWeight4(Int_t s,Int_t m,Double_t w){weight4[s*4+m]=w;}
    void     setWeight5(Int_t s,Int_t m,Double_t w){weight5[s*4+m]=w;}
    void     setWeight6(Int_t s,Int_t m,Double_t w){weight6[s*4+m]=w;}

    void     setStepNorm(Int_t s,Int_t m,Double_t N){stepNorm[s*4+m]=N;}
    void     setTukeyConstant(Int_t s,Int_t m,Double_t tc){tukeyConstant[s*4+m]=tc;}

    void getStepsSeg1(Int_t s,Double_t* steps){
    (*this).getSteps(s,0,steps);
    steps[0]+=(*this).getStep1(s,1);
    steps[1]+=(*this).getStep2(s,1);
    steps[2]+=(*this).getStep3(s,1);
    steps[3]+=(*this).getStep4(s,1);
    steps[4]+=(*this).getStep5(s,1);
    steps[5]+=(*this).getStep6(s,1);
    for(Int_t i=0;i<6;i++) steps[i]*=0.5; // ???
  }
  void getStepsSeg2(Int_t s,Double_t* steps){
    (*this).getSteps(s,2,steps);
    steps[0]+=(*this).getStep1(s,3);
    steps[1]+=(*this).getStep2(s,3);
    steps[2]+=(*this).getStep3(s,3);
    steps[3]+=(*this).getStep4(s,3);
    steps[4]+=(*this).getStep5(s,3);
    steps[5]+=(*this).getStep6(s,3);
    for(Int_t i=0;i<6;i++) steps[i]*=0.5; // ???
  }
  void getWeightsSeg1(Int_t s,Double_t* weights){
    (*this).getWeights(s,0,weights);
    weights[0]+=(*this).getWeight1(s,1);
    weights[1]+=(*this).getWeight2(s,1);
    weights[2]+=(*this).getWeight3(s,1);
    weights[3]+=(*this).getWeight4(s,1);
    weights[4]+=(*this).getWeight5(s,1);
    weights[5]+=(*this).getWeight6(s,1);
    for(Int_t i=0;i<6;i++) weights[i]*=0.5; // ???
  }
  void getWeightsSeg2(Int_t s,Double_t* weights){
    (*this).getWeights(s,2,weights);
    weights[0]+=(*this).getWeight1(s,3);
    weights[1]+=(*this).getWeight2(s,3);
    weights[2]+=(*this).getWeight3(s,3);
    weights[3]+=(*this).getWeight4(s,3);
    weights[4]+=(*this).getWeight5(s,3);
    weights[5]+=(*this).getWeight6(s,3);
    for(Int_t i=0;i<6;i++) weights[i]*=0.5; // ???
  }
  Int_t getMaxIterationSeg1(Int_t s){
    return ((*this).getMaxIteration(s,0)+(*this).getMaxIteration(s,1))/2;
  }
  Int_t getMaxIterationSeg2(Int_t s){
    return ((*this).getMaxIteration(s,2)+(*this).getMaxIteration(s,3))/2;
  }
  Double_t getStepNormSeg1(Int_t s){
    return ((*this).getStepNorm(s,0)+(*this).getStepNorm(s,1))*0.5;
  }
  Double_t getStepNormSeg2(Int_t s){
    return ((*this).getStepNorm(s,2)+(*this).getStepNorm(s,3))*0.5;
  }
  Double_t getTukeyConstantSeg1(Int_t s){
    return ((*this).getTukeyConstant(s,0)+(*this).getTukeyConstant(s,1))*0.5;
  }
  Double_t getTukeyConstantSeg2(Int_t s){
    return ((*this).getTukeyConstant(s,2)+(*this).getTukeyConstant(s,3))*0.5;
  }

    Bool_t   init(HParIo*, Int_t*);
    Int_t    write(HParIo*);
    void     putParams(HParamList*);
    Bool_t   getParams(HParamList*);
    void     clear();
    void     printParam(void);
    ClassDef(HMdcTrackFitPar,1) // Container for the MDC Digitizer parameters
};
#endif  /*!HMDCTRACKFITPAR_H*/
