#ifndef HSAVITZKYGOLAY_H
#define HSAVITZKYGOLAY_H

#include <TObject.h>
#include "hrtmatrix.h"
#include <TMatrix.h>

class HSavitzkyGolay : public TObject {
public:
  HSavitzkyGolay(void);
  ~HSavitzkyGolay(void);
  void calcCoefficients(HRtVector &out,Int_t nl,Int_t nr,Int_t ld,Int_t m);
protected:
public:
  ClassDef(HSavitzkyGolay,1)
};


#endif
