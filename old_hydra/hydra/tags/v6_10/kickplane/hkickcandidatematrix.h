#ifndef HKICKCANDIDATEMATRIX_H
#define HKICKCANDIDATEMATRIX_H

#include <iostream.h>
#include <TObject.h>
class HKickCandidate;

class HKickCandidateMatrix { //FIXME: Generalize
protected:
  HKickCandidate *fData;
  Int_t fDimX,fDimY,fSize;
public:
  HKickCandidateMatrix(void) : fData(0), fDimX(0), fDimY(0), fSize(0) {}
  HKickCandidateMatrix(Int_t dx,Int_t dy) : fData(0) 
    { resize(dx,dy); }
  ~HKickCandidateMatrix();
  void resize(Int_t dx,Int_t dy);
  HKickCandidate &operator()(Int_t i,Int_t j);
  Int_t getDimX(void) const { return fDimX; }
  Int_t getDimY(void) const { return fDimY; }
  ostream &print(ostream &out);  
};

#endif
