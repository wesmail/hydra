#ifndef HTRACK_H
#define HTRACK_H

#include "hrecobject.h"

class HTrack : public HRecObject
{
private:
  Float_t fP; //Momentum
public:
  HTrack(void);
  HTrack(HTrack &aTrack);
  ~HTrack(void);
  void setMomentum(Float_t aP);
  ClassDef(HTrack,1) //Track on a MDC (test class)
};

#endif /* !HTRACK_H */



