#ifndef HRTMEASUREMENT_H
#define HRTMEASUREMENT_H

#include "hrttypes.h"
#include "hrtmatrix.h"


//FIXME: Add custom streamer and remove ClassDef. Memory overhead
class HRtMeasurement {
  public:
    HRtMeasurement(void) {}
    virtual ~HRtMeasurement(void) {}
    Float_t &operator[](int i) { return fData[i]; }
    inline void copyTo(HRtVector &v);
    inline void setCoordinates(Int_t i,Float_t x,Float_t y);
    void print(void);
  protected:
    Float_t fData[kRtMeasurementDim];
  public:
    ClassDef(HRtMeasurement,1)
};

void HRtMeasurement::copyTo(HRtVector &v) {
  //FIXME: Don't copy data but only the pointer
  for (int i=0;i<kRtMeasurementDim;i++) v(i) = fData[i];
}

void HRtMeasurement::setCoordinates(Int_t i,Float_t x, Float_t y) {
  fData[i << 1] = x;
  fData[(i << 1) | 0x01] = y;
}

#endif
