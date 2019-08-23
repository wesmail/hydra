#ifndef HRTMEASUREMENT_H
#define HRTMEASUREMENT_H

#include "hrttypes.h"
#include "hrtmatrix.h"


//FIXME: Add custom streamer and remove ClassDef. Memory overhead
class HRtMeasurement {
  public:
    HRtMeasurement(void) {}
    virtual ~HRtMeasurement(void) {}
    Float_t &operator[](Int_t i) { return fData[i]; }
    inline void copyTo(HRtVector &v);
    inline void setCoordinates(Int_t i,Float_t x,Float_t y);
    void print(void);
  protected:
    Float_t fData[kRtMeasurementDim];
  public:
    ClassDef(HRtMeasurement,1)
};

inline void HRtMeasurement::copyTo(HRtVector &v) {
  //FIXME: Don't copy data but only the pointer
  for (Int_t i=0;i<kRtMeasurementDim;i++) v(i) = fData[i];
}

inline void HRtMeasurement::setCoordinates(Int_t i,Float_t x, Float_t y) {
  fData[i << 1] = x;
  fData[(i << 1) | 0x01] = y;
}

#endif
