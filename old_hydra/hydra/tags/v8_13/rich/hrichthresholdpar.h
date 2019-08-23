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

  Int_t getCalParVers(Int_t pSector) {return fCalParVers[pSector];} 
  Float_t getShift(Int_t pSector) {return fShift[pSector];} 
  Float_t getMultiply_offset(Int_t pSector) {return fMultiply_offset[pSector];} 
  Float_t getMultiply_sigma(Int_t pSector) {return fMultiply_sigma[pSector];} 

  void setCalParVers(Int_t pSector, Int_t pCalParVers);
  void setShift(Int_t pSector, Float_t pShift);
  void setMultiply_offset(Int_t pSector, Float_t pMultiply_offset);
  void setMultiply_sigma(Int_t pSector, Float_t pMultiply_sigma);
  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t defaultInit();

  void printThresholdPar();

  ClassDef(HRichThresholdPar,1) //ROOT extension

private:
  Int_t   fNSectors;
  Int_t   fActiveSectors[6];
  Int_t   fCalParVers[6];
  Float_t fShift[6];
  Float_t fMultiply_offset[6];
  Float_t fMultiply_sigma[6];

};

#endif // HRICHTHRESHOLDPAR_H
