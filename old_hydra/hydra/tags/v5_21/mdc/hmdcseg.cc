//*-- Author : R.Schicker
//*-- Modified : M. Sanchez (8.06.2000)

#include "hmdcseg.h"

HMdcSeg::HMdcSeg(void) {
  Clear();
}

HMdcSeg::~HMdcSeg(void) {
} 

void HMdcSeg::Clear(void) {
    z=0.;
    r=0.;
    theta=0.;
    phi=0.;
    chi2=0.;
    flag=0;
    ind=0;         //Nb of hits = 0
    cov.Clear();
}

ClassImp(HMdcSeg)












