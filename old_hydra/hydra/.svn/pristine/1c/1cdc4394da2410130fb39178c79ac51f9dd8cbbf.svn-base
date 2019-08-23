#ifndef HMDCTRACKFITTERB_H
#define HMDCTRACKFITTERB_H

#include "hmdctrackfittera.h"

class HMdcTrackFitterB : public HMdcTrackFitterA {
  private:

  public:
    HMdcTrackFitterB(HMdcTrackFitInOut* fIO);
    virtual ~HMdcTrackFitterB(void);
    virtual Int_t minimize(Double_t threshold, Int_t iter=0);

  protected:
    void targetScan(void);

    ClassDef(HMdcTrackFitterB,0)  // Dubna track piece fitter
};

#endif
