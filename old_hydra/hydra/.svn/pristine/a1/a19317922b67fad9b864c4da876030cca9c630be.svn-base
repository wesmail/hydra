#ifndef HMDCTIMECUT_H
#define HMDCTIMECUT_H

#include "hparset.h"
#include "hmdccal1.h"

class HMdcTimeCut : public HParSet {
private:
  Float_t cutT1L;       // Cut 1:  cutT1L<time1<cutT1R
  Float_t cutT1R;       // cutT1L>=cutT1R - no cut
  Float_t cutT2L;       // Cut 2:  cutT2L<time2<cutT2R
  Float_t cutT2R;       // cutT2L>=cutT2R - no cut
  Float_t cutLeft;      // Cut 3: cutLeft<(time2-time1)<cutRight
  Float_t cutRight;     // cutLeft>=cutRight - no cut
  Bool_t isContainer;  
  void setNameTitle();
public:
  HMdcTimeCut();
  HMdcTimeCut(Float_t cT1L, Float_t cT1R, Float_t cT2L, Float_t cT2R,
              Float_t cLeft, Float_t cRight);
  static HMdcTimeCut* getExObject();  // if HMdcTimeCut not existed = 0 
  static HMdcTimeCut* getObject();    // if HMdcTimeCut not existed it will cr.
  Bool_t cut(HMdcCal1* cal);  // if =kTRUE don't use this hit
  Bool_t cutComStop(HMdcCal1* cal) {
     return (cal->getTime1()>=cutT1R) ? kTRUE:kFALSE;}
  Bool_t cutTime1(HMdcCal1* cal);
  Bool_t cutTime2(HMdcCal1* cal);
  Bool_t cutTimesDif(HMdcCal1* cal);  
  void setCut(Float_t cT1L, Float_t cT1R, Float_t cT2L, Float_t cT2R,
              Float_t cLeft, Float_t cRight);
  void setCutTime1(Float_t cT1L, Float_t cT1R);
  void setCutTime2(Float_t cT2L, Float_t cT2R);
  void setCutDTime21(Float_t cLeft, Float_t cRight);
  void getCut(Float_t &cT1L, Float_t &cT1R, Float_t &cT2L, Float_t &cT2R,
              Float_t &cLeft, Float_t &cRight);
  Float_t  getCutT1L() {return cutT1L;}
  Float_t  getCutT1R() {return cutT1R;}
  Float_t  getCutT2L() {return cutT2L;}
  Float_t  getCutT2R() {return cutT2R;}
  Float_t  getCutDTime21L() {return cutLeft;}
  Float_t  getCutDTime21R() {return cutRight;} 
  Bool_t init(HParIo* input,Int_t* set) {return isContainer;} 
  
  ClassDef(HMdcTimeCut,0) 
};

#endif  /*!HMDCTIMECUT_H*/ 
