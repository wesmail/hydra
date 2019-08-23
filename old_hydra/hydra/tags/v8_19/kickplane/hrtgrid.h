#ifndef HRTGRID_H
#define HRTGRID_H

#include "TObject.h"
#include "hrtmatrix.h"
#include "hrttypes.h"
#include "hrtmeasurement.h"

class HRtGrid : public TObject { //FIXME: Inherint from HParSet
 public:
  enum EState {kOk,kOutOfBounds};
  HRtGrid(void);
  ~HRtGrid(void);
  void setDimension(Int_t d, const UInt_t siz[]);
  void setRange(UInt_t i, Float_t min, Float_t max);
  Float_t getMinimum(UInt_t i) { return fMin[i]; }
  Float_t getMaximum(UInt_t i) { return fMax[i]; }
  Float_t getBinCenter(UInt_t var, UInt_t bin);
  Float_t getBinSize(UInt_t var) { return fStep[var]; }
  Float_t getMidValue(UInt_t var) { return (fMax[var]-fMin[var])/2.; }
  UInt_t getBins(UInt_t i) { return fBins[i]; }
  HRtMeasurement &bin(UInt_t point[]);
  HRtMeasurement &bin(Float_t point[]);
  HRtMeasurement &bin(const HRtVector &v);
  UInt_t *getCurrentBin(void) { return fIdx; }
  UInt_t getCurrentAddress(void) { return fCurrentAddress; }
  void fillCurrentBinCoord(HRtVector &out);
  Int_t getDimension(void) { return fDimension; }
  EState getStatus(void) { return fState; }
  void print(Int_t v,UInt_t point[]) { printf("%f\n",bin(point)[v]); }
 protected:
  UInt_t fDimension;
  Int_t fSize;
  HRtMeasurement *fCells; //[fSize]
  UInt_t *fBins; //[fDimension]
  UInt_t *fIdx; //[fDimension]
  Float_t *fMin; //[fDimension]
  Float_t *fFirstBin; //[fDimension]
  Float_t *fMax; //[fDimension]
  Float_t *fStep; //[fDimension]
  UInt_t fCurrentAddress;
  Int_t fTotalBins;
  void freeMemory(void);
  enum EState fState;
 public:
  ClassDef(HRtGrid,1) //Reference trajectories
};

#endif
