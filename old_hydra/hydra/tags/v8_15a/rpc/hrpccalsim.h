#ifndef HRPCCALSIM_H
#define HRPCCALSIM_H

#include "hrpccal.h"

class HRpcCalSim : public HRpcCal {
protected:
  Int_t nTrackL;   
  Int_t nTrackR;   

public:
  HRpcCalSim(void) : nTrackL(-1), nTrackR(-1) {}
  ~HRpcCalSim(void) {}
  void clear(void);
  
  inline void  setNTrackL(const Int_t n) {nTrackL=n;}
  inline void  setNTrackR(const Int_t n) {nTrackR=n;}
  inline Int_t getNTrackL(void) const {return nTrackL;}
  inline Int_t getNTrackR(void) const {return nTrackR;}

  ClassDef(HRpcCalSim,1)
};

#endif  /* HRPCCALSIM_H */

