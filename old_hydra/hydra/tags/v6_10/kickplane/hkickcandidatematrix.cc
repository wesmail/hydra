#include "hkickcandidatematrix.h"
#include "hkickcandidate.h"

//------------------------- HKickCandidateMatrix --------------------

static HKickCandidate gNullCandidate;

HKickCandidateMatrix::~HKickCandidateMatrix(void) {
  if (fData) { delete[] fData; fData=0; }
}

void HKickCandidateMatrix::resize(Int_t dx,Int_t dy) {
  //Resizes the candidate matrix to dx x dy
  if (fData) { delete[] fData; fData=0; }
  fData=new HKickCandidate[dx*dy];
  fDimX=dx; fDimY=dy;
  fSize=dx*dy;
}


HKickCandidate &HKickCandidateMatrix::operator()(Int_t i,Int_t j) {
  //Gives back the candidate in position i,j of the matrix
  Int_t linAddr=i*fDimY+j;
  if (fData) {
    return fData[ (linAddr<fSize) ? linAddr : 0];
  } 
  return gNullCandidate;
}

ostream &HKickCandidateMatrix::print(ostream &out) {
  //Prints the candidate matrix to the stream "out" for inspection.
  for (int i=0; i<fDimX; i++) {
    for (int j=0; j<fDimY; j++) {
      out << fData[i*fDimY+j].getMomentum() << "/";
      out << fData[i*fDimY+j].getXPull() << "\t";
    }
    out << endl;
  }
  return out;
}
