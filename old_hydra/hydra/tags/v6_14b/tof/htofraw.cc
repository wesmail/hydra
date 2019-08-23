#pragma implementation
#include "htofraw.h"
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

ClassImp(HTofRaw)


  void  HTofRaw::doit(void) {
    TTree *tree=(TTree *)gDirectory->Get("T");
    tree->Draw("HTofRaw.fData.fLeftT>>newhist");
}

HTofRaw::HTofRaw(void) {
     nHit=0;
     rightT= leftT=0.0;
     rightQ= leftQ=0.0;
}

HTofRaw::~HTofRaw(void) {
}

inline Float_t HTofRaw::getRightTime(void) {
  return  rightT;
}

inline Float_t HTofRaw::getLeftTime(void) {
  return  leftT;
}

inline Float_t HTofRaw::getRightCharge(void) {
  return  rightQ;
}

inline Float_t HTofRaw::getLeftCharge(void) {
  return  leftQ;
}

inline Short_t HTofRaw::getNHit(void) {
  return  nHit;
}

inline void HTofRaw::setRightTime(Float_t arightT) {
  rightT=arightT;
}
  
inline void HTofRaw::setLeftTime(Float_t aleftT) {
   leftT=aleftT;
}

inline void HTofRaw::setRightCharge(Float_t arightQ){
   rightQ=arightQ;
}

inline void HTofRaw::setLeftCharge(Float_t aleftQ) {
   leftQ=aleftQ;
}

inline void HTofRaw::setNHit(Short_t anHit){
   nHit=anHit;
}

inline Int_t HTofRaw::getNLocationIndex(void) {
  return 3;
}

inline Int_t HTofRaw::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getModule(); break;
  case 2 : return getCell(); break;
  }
  return -1;
}

