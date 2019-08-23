#ifndef HMDCSEGMENTFPAR_H
#define HMDCSEGMENTFPAR_H

# include <fstream.h>
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

  Float_t getCorrX(Int_t sect, Int_t lever);
  Float_t getCorrY(Int_t sect, Int_t lever);
  Float_t getErrDiffX(Int_t sect, Int_t lever);
  Float_t getErrDiffXSlope(Int_t sect, Int_t lever);
  Float_t getErrDiffY(Int_t sect, Int_t lever);
  Float_t getErrDiffYSlope(Int_t sect, Int_t lever);
  Float_t getAlpha(Int_t sect, Int_t lever);

  Bool_t setCorrX(Int_t sect, Int_t lever, Float_t corr);
  Bool_t setCorrY(Int_t sect, Int_t lever, Float_t corr);
  Bool_t setErrDiffX(Int_t sect, Int_t lever, Float_t error);
  Bool_t setErrDiffY(Int_t sect, Int_t lever, Float_t error);
  Bool_t setErrDiffXSlope(Int_t sect, Int_t lever, Float_t error);
  Bool_t setErrDiffYSlope(Int_t sect, Int_t lever, Float_t error);
  Bool_t setAlpha(Int_t sect, Int_t lever, Float_t value);

ClassDef(HMdcSegmentFPar,1)
};


# endif /* if !HMDCSEGMENTFPAR_H*/
