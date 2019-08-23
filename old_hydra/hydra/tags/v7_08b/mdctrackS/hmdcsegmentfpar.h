#ifndef HMDCSEGMENTFPAR_H
#define HMDCSEGMENTFPAR_H
using namespace std;
# include <fstream> 
# include "hparset.h"

class TArrayF;
class HParIo;

class HMdcSegmentFPar : public HParSet{

 private:
  Short_t fNSectors, fNLeverArms;   //number of sectors and lever arms in setup

  TArrayF* fCorrX;                // correlation between diff x - diff xslope
  TArrayF* fCorrY;                // correlation between diff y - diff yslope
  TArrayF* fErrDiffX;             // error in x(MDC1)-x(MDC2)
  TArrayF* fErrDiffXSlope;        // error in xslope(MDC1)-xslope(MDC2)
  TArrayF* fErrDiffY;             // error in y(MDC1)-y(MDC2)
  TArrayF* fErrDiffYSlope;        // error in yslope(MDC1)-yslope(MDC2)
  TArrayF* fAlpha;                // number of standard deviations

  inline Int_t getIndex(Int_t nSect, Int_t nLever){return nSect*fNLeverArms + nLever;}

 public:

  HMdcSegmentFPar(const char* name="MdcSegmentFPar",
          const char* title="parameter container for segment finding and fitting",
          const char* context="");
  ~HMdcSegmentFPar(void);

  Bool_t init(HParIo* inp, Int_t* set);
  Int_t write(HParIo* output);

  Bool_t read(fstream* pFile, Int_t* set, Int_t inputNumber);
  Int_t writeFile(fstream *pFile);

  Short_t getNSectors(void){return fNSectors;}
  Short_t getNLeverArms(void){return fNLeverArms;}

  inline Float_t getCorrX(Int_t sect, Int_t lever);
  inline Float_t getCorrY(Int_t sect, Int_t lever);
  inline Float_t getErrDiffX(Int_t sect, Int_t lever);
  inline Float_t getErrDiffXSlope(Int_t sect, Int_t lever);
  inline Float_t getErrDiffY(Int_t sect, Int_t lever);
  inline Float_t getErrDiffYSlope(Int_t sect, Int_t lever);
  inline Float_t getAlpha(Int_t sect, Int_t lever);

  Bool_t setCorrX(Int_t sect, Int_t lever, Float_t corr);
  Bool_t setCorrY(Int_t sect, Int_t lever, Float_t corr);
  Bool_t setErrDiffX(Int_t sect, Int_t lever, Float_t error);
  Bool_t setErrDiffY(Int_t sect, Int_t lever, Float_t error);
  Bool_t setErrDiffXSlope(Int_t sect, Int_t lever, Float_t error);
  Bool_t setErrDiffYSlope(Int_t sect, Int_t lever, Float_t error);
  Bool_t setAlpha(Int_t sect, Int_t lever, Float_t value);

ClassDef(HMdcSegmentFPar,1)
};

inline Float_t HMdcSegmentFPar :: getCorrX(Int_t sect, Int_t lever){
  return fCorrX->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getCorrY(Int_t sect, Int_t lever){
  return fCorrY->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffX(Int_t sect, Int_t lever){
  return fErrDiffX->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffXSlope(Int_t sect, Int_t lever){
  return fErrDiffXSlope->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffY(Int_t sect, Int_t lever){
  return fErrDiffY->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffYSlope(Int_t sect, Int_t lever){
  return fErrDiffYSlope->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getAlpha(Int_t sect, Int_t lever){
  return fAlpha->At(getIndex(sect,lever));
}



# endif /* if !HMDCSEGMENTFPAR_H*/
