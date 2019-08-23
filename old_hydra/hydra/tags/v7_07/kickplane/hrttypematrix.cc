#include "hrttypematrix.h"
#include <math.h>

/************************************************************/
/*                    HRTFLOATMATRIX                        */
/************************************************************/
HRtFloatMatrix::HRtFloatMatrix(void) {
  fBins = fIdx = 0;
  fMin = fMax = fStep = 0;
  fFirstBin = 0;
  fCells = 0;
  fState = kOk;
}

void HRtFloatMatrix::setDimension(Int_t d,const UInt_t siz[]) {
  freeMemory();
  fDimension = d;
  fTotalBins = 1;
  fBins = new UInt_t[d];
  fMin = new Float_t[d];
  fFirstBin = new Float_t[d];
  fMax = new Float_t[d];
  fStep = new Float_t[d];
  fIdx = new UInt_t[d];

  for (int i=0;i<d;i++) {
    fBins[i] = siz[i];
    fTotalBins *= siz[i];
  }
  fSize = fTotalBins + 1;
  fCells = new Float_t[fSize];
}

HRtFloatMatrix::~HRtFloatMatrix(void) {
  freeMemory();
}

Float_t HRtFloatMatrix::interpol2D(Float_t v[]) {
  Float_t y1 = bin(v);
  fIdx[0]++;
  Float_t y2 = bin(fIdx);
  fIdx[1]++;
  Float_t y3 = bin(fIdx);
  fIdx[0]--;
  Float_t y4 = bin(fIdx);
  fIdx[1]--;

  Float_t t = (v[0] - fMin[0]-fIdx[0]*fStep[0]) / fStep[0];
  Float_t u = (v[1] - fMin[1]-fIdx[1]*fStep[1]) / fStep[1];
  return (1-t)*(1-u)*y1 + t*(1-u)*y2 + t*u*y3 + (1-t)*u*y4;
}

void HRtFloatMatrix::fillCurrentBinCoord(HRtVector &v) {
  for (unsigned int i=0;i<fDimension;i++) {
    v(i) = fFirstBin[i] + fStep[i]*fIdx[i];
  }
}

void HRtFloatMatrix::freeMemory(void) {
  if (fBins) { delete[] fBins; fBins=0; }
  if (fMin) { delete[] fMin; fMin=0; }
  if (fFirstBin) { delete[] fFirstBin; fFirstBin=0; }
  if (fMax) { delete[] fMax; fMax=0; }
  if (fCells) { delete[] fCells; fCells=0; }
  if (fIdx) { delete[] fIdx; fIdx=0; }
  if (fStep) { delete[] fStep; fStep=0; }
}

Float_t HRtFloatMatrix::getBinCenter(UInt_t var, UInt_t bin) {
  Float_t r;
  r = fFirstBin[var] + bin*fStep[var];
  return r;
}
  
void HRtFloatMatrix::setRange(UInt_t i, Float_t min, Float_t max) {
  if (i<fDimension) {
    fStep[i] = (max - min) / fBins[i];
    fFirstBin[i] = min + fStep[i] / 2.;
    fMax[i] = max;
    fMin[i] = min;
  }
}

Float_t &HRtFloatMatrix::bin(UInt_t idx[]){
  UInt_t fCurrentAddress = idx[0];
  fState = kOk;

  if (idx[0]<fBins[0]) {
    for (unsigned int i=1;i<fDimension;i++) {
      if (!(idx[i]<fBins[i])) {
        //printf("out of bonds at %i. !(%i<%i)\n",i,idx[i],fBins[i]);
	fCurrentAddress = fTotalBins;
	fState = kOutOfBounds;
	break;
      }
      fCurrentAddress = fCurrentAddress*fBins[i] + idx[i];
    }
  } else {
    fState = kOutOfBounds;
    fCurrentAddress = fTotalBins;
  }

  return fCells[fCurrentAddress];
}

Float_t &HRtFloatMatrix::bin(Float_t var[]){
  for (unsigned int i=0;i<fDimension;i++) {
    if (var[i]<fMin[i]) {
      fState = kOutOfBounds;
      fCurrentAddress = fTotalBins;
      return fCells[fTotalBins];
    }
    fIdx[i] = int(rint( (var[i] - fFirstBin[i]) / fStep[i] ));
  }
  fState = kOk;
  return bin(fIdx);
}

Float_t &HRtFloatMatrix::bin(const HRtVector &v) {
  for (unsigned int i=0;i<fDimension;i++) {
    if (v.at(i)<fMin[i]) {
      //printf("out of bonds at %i. !(%f>%f)\n",i,v.at(i),fMin[i]);
      fState = kOutOfBounds;
      fCurrentAddress = fTotalBins;
      return fCells[fTotalBins];
    }
    fIdx[i] = int(rint( (v.at(i) - fFirstBin[i]) / fStep[i] ));
  }
  return bin(fIdx);
}

ClassImp(HRtFloatMatrix)
