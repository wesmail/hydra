#ifndef HRICHTHRESHOLDPAR_H
#define HRICHTHRESHOLDPAR_H


#include "hparset.h"
#include "hrichparset.h"
#include "hobjtable.h"

class HParHadAsciiFileIo;

class HRichThresholdPar : public HRichParSet {

public:

  HRichThresholdPar(const char* name="RichThresholdPar",
                    const char* title="Rich Threshold Parameters",
                    const char* context="Common");
  ~HRichThresholdPar();

  Float_t getOffset() {return fOffset;} 
  Float_t getMultiply_mean() {return fMultiply_mean;} 
  Float_t getMultiply_sigma() {return fMultiply_sigma;} 

  void setOffset(Float_t pOffset);
  void setMultiply_mean(Float_t pMultiply_mean);
  void setMultiply_sigma(Float_t pMultiply_sigma);
  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t defaultInit();

  void printThresholdPar();

  ClassDef(HRichThresholdPar,1) //ROOT extension

private:
  
  Float_t fOffset;
  Float_t fMultiply_mean;
  Float_t fMultiply_sigma;

};

#endif // HRICHTHRESHOLDPAR_H
