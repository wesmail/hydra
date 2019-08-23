#ifndef HLUDECOMPOSER_H
#define HLUDECOMPOSER_H

#include "TObject.h"
#include "hrtmatrix.h"
#include "TArrayI.h"

class HLuDecomposer : public TObject {
public:
  HLuDecomposer(void);
  ~HLuDecomposer(void);
  void decompose(HRtMatrix &a);
  void backSubstitute(HRtVector &b);
  void backSubstitute(HRtMatrix &b);
protected:
  TArrayI fPermutations; //[fDimension]
  Int_t fPermutationParity;
  HRtMatrix *fA; //!Processed matrix
public:
  ClassDef(HLuDecomposer,1) //LU decomposition for ROOT matrix
};


#endif
