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
    Double_t stepFit;          // Step for gradient dowhill
    Int_t minimizationMethod;
    Bool_t useTukey;
    Bool_t flDstTime;
  public:
    HMdcTrackFitterA(void);
    virtual ~HMdcTrackFitterA(void);
    virtual Bool_t init(void);
    virtual Bool_t reinit(void);
    virtual Int_t execute(Double_t threshold, Int_t iter=0);
    virtual Bool_t finalize(void);
    virtual Double_t getFunctional(void);

    Int_t getMethod() {return minimizationMethod;}

  private:
    void setParam(void);
    void fillOutput(Double_t* par);
    void printParam(Char_t* title, Double_t *par);
    void printResult(Char_t* status);
    void printStep(char* cond, Double_t fun, Double_t step);
    void printTime(char* title, Int_t time, Int_t iflag,Double_t aTm);
    Double_t valueOfFunctional(Double_t* par, Int_t iflag=2);
    void derivativesOfFunctional(Double_t fun0, Double_t* par, Int_t iflag=0);
    void solutionOfLinearEquationsSystem(Double_t* x);
    Double_t calcMdcTime(Int_t time,
        Double_t xVertex, Double_t yVertex, Double_t zVertex,
        Double_t xPlane, Double_t yPlane, Double_t zPlane);
    void downhillOnGradient(Double_t* parIn, Double_t* parOut);
    Int_t filterOfHits(Double_t* parOut, Int_t& numberOfHits);

    ClassDef(HMdcTrackFitterA,0)
};

#endif
