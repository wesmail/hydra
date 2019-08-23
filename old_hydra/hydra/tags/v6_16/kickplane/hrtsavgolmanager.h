#ifndef HRTSAVGOLMANAGER_H
#define HRTSAVGOLMANAGER_H

#include "hsavitzkygolay.h"
#include "hrtmatrix.h"

class HRtSavGolManager : public TObject {
  public:
    HRtSavGolManager(Int_t nLeft,Int_t nRight,Int_t ld,Int_t m);
    ~HRtSavGolManager(void);
    const HRtVector *getCoefficients(Int_t nl,Int_t nr) { 
      if ( !(nl<fMaxLeft && nr<fMaxRight) ) return 0;
      return fTable[nl*fMaxRight+nr];
    }
    void print(void);
  protected:
    Int_t fSize;
    HRtVector **fTable; //[fSize]
    Int_t fMaxLeft,fMaxRight,fM;
    HSavitzkyGolay fSavGol;
  public:
    ClassDef(HRtSavGolManager,1) //Manager for Savitzky-Golay coefficients
};

#endif
