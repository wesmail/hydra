#pragma implementation
#include "htofinoraw.h"
#include <TDirectory.h>
#include <TTree.h>

//*-- Author : W.Koenig / W. Schoen / D. Vassiliev
//*-- Modified : 05/06/98 by Manuel Sanchez
//*-- Modified : 12/12/97 by Walter Karig


////////////////////////////
//HTofRaw
//
// This class contains TOF raw data
//
// adapted from /u/halo/packages/abase/new by Manuel Sanchez (17/06/98)
////////////////////////////

ClassImp(HTofinoRaw)


HTofinoRaw::HTofinoRaw(void) {
     nHit=0;
     tdcTofino = adcTofino = 0.0;
}

HTofinoRaw::~HTofinoRaw(void) {
}

inline Float_t HTofinoRaw::getTdcT(void) {
  return  tdcTofino;
}

inline Float_t HTofinoRaw::getAdcT(void) {
  return  adcTofino;
}

inline Short_t HTofinoRaw::getNHit(void) {
  return  nHit;
}

inline void HTofinoRaw::setTdcT(Float_t atdcTofino) {
  tdcTofino=atdcTofino;
}
  
inline void HTofinoRaw::setAdcT(Float_t aadcTofino) {
   adcTofino=aadcTofino;
}

inline void HTofinoRaw::setNHit(Short_t anHit){
   nHit=anHit;
}

inline Int_t HTofinoRaw::getNLocationIndex(void) {
  return 3;
}

inline Int_t HTofinoRaw::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getModule(); break;
  case 2 : return getCell(); break;
  }
  return -1;
}
