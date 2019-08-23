#ifndef HMDCTRACKFITTERA_H
#define HMDCTRACKFITTERA_H

#include "hmdctrackfitter.h"

class HMdcTrackFitterA : public HMdcTrackFitter {
  private:
    Double_t grad[6];          // Gradient of downhill direction
    Double_t agrad;            // Abs value of gradient
    Double_t cosgrad[6];       // Cosines of downhill direction
    Double_t scalePar[6];      // Scaling factors for gradient downhill
    Double_t grad2[10][10];    // Matrixes of 2-nd derivatives
    Float_t invMatrix[4][4];   // Inverse matrixes 4 parameters !!!
    Double_t dTdA[10][250];    // [][Array Size!!!] Test: dT_k/da
    Double_t stepFit;          // Step for gradient dowhill
    Int_t minimizationMethod;
    Bool_t useTukey;
    Bool_t flDstTime;
    HSymMat4 matrH;
    
    // Fit parameters:
    Double_t funCt1;           // if functional < funCt1 stepD1 will used
    Double_t stepD1;
    Double_t funCt2;           // else if functional < funCt2 stepD2 will used
    Double_t stepD2;
    Double_t stepD3;           // else stepD3 will used
    
  public:
    HMdcTrackFitterA(HMdcTrackFitInOut* fIO);
    virtual ~HMdcTrackFitterA(void);
    virtual Int_t execute(Double_t threshold, Int_t iter=0);
    virtual Bool_t finalize(void);
    virtual Double_t getFunctional(void);

    Int_t getMethod() {return minimizationMethod;}

  private:
    void setParam(void);
    void fillOutput(Double_t* par);
    void printParam(Char_t* title, Double_t *par);
    void printResult(Char_t* status, Double_t *par);
    void printStep(char* cond, Double_t fun, Double_t step);
    void printTime(char* title, Int_t time, Int_t iflag,Double_t aTm);
    Double_t valueOfFunctional(Double_t* par, Int_t iflag=2);
    Double_t valueOfFunctional(Double_t* par, Int_t iflag,
        Int_t iPar1,Double_t step1,Int_t iPar2=0,Double_t step2=0.);
    void derivativesOfFunctional(Double_t fun0, Double_t* par, Int_t iflag=0);
    void secondDerivOfFunct(void);
    void solutionOfLinearEquationsSystem(Double_t* par, Int_t nmOfPar);
    Double_t calcMdcTime(Int_t time, Double_t x1, Double_t y1, Double_t z1,
                                     Double_t x2, Double_t y2, Double_t z2);
    void downhillOnGradient(Double_t* parIn, Double_t* parOut);
    void filterOfHits(void);
    void calculationOfErrors(void);
    void invertMatr(Int_t nmOfPar);

    ClassDef(HMdcTrackFitterA,0)
};

#endif
