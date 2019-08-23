#ifndef HMDCTIMECUTD_H
#define HMDCTIMECUTD_H

#include "hparset.h"
#include "hmdccal1.h"

class HMdcTimeCutD : public HParSet {
private:
  static HMdcTimeCutD* fMdcTimeCutD;
  Float_t cutT1L[4];       // Cut 1:  cutT1L<time1<cutT1R
  Float_t cutT1R[4];       // cutT1L>=cutT1R - no cut
  Float_t cutT2L[4];       // Cut 2:  cutT2L<time2<cutT2R
  Float_t cutT2R[4];       // cutT2L>=cutT2R - no cut
  Float_t cutLeft[4];      // Cut 3: cutLeft<(time2-time1)<cutRight
  Float_t cutRight[4];     // cutLeft>=cutRight - no cut
  Bool_t isContainer;
  
  HMdcTimeCutD();
  ~HMdcTimeCutD(){;}
public:
  static HMdcTimeCutD* getExObject(); // if HMdcTimeCutD not existed = 0 
  static HMdcTimeCutD* getObject();   // if HMdcTimeCutD not existed it will be cr.
  static void deleteCont();
  Bool_t cut(HMdcCal1* cal);  // if =kTRUE don't use this hit
  Bool_t cutComStop(HMdcCal1* cal);
  Bool_t cutTime1(HMdcCal1* cal);
  Bool_t cutTime2(HMdcCal1* cal);
  Bool_t cutTimesDif(HMdcCal1* cal);  
  void setCut(Float_t cT1L, Float_t cT1R, Float_t cT2L, Float_t cT2R,
              Float_t cLeft, Float_t cRight);
  void setCut(Float_t* cT1L, Float_t* cT1R, Float_t* cT2L, Float_t* cT2R,
              Float_t* cLeft, Float_t* cRight);
  void setCutTime1(Float_t cT1L, Float_t cT1R);
  void setCutTime2(Float_t cT2L, Float_t cT2R);
  void setCutDTime21(Float_t cLeft, Float_t cRight);
  void setCut(Int_t mod, Float_t cT1L, Float_t cT1R, 
      Float_t cT2L, Float_t cT2R, Float_t cLeft, Float_t cRight);
  void setCutTime1(Int_t mod, Float_t cT1L, Float_t cT1R);
  void setCutTime2(Int_t mod, Float_t cT2L, Float_t cT2R);
  void setCutDTime21(Int_t mod, Float_t cLeft, Float_t cRight);
  void getCut(Int_t mod, Float_t &cT1L, Float_t &cT1R, 
      Float_t &cT2L, Float_t &cT2R, Float_t &cLeft, Float_t &cRight);
  Float_t  getCutT1L(Int_t mod) {return cutT1L[mod];}
  Float_t  getCutT1R(Int_t mod) {return cutT1R[mod];}
  Float_t  getCutT2L(Int_t mod) {return cutT2L[mod];}
  Float_t  getCutT2R(Int_t mod) {return cutT2R[mod];}
  Float_t  getCutDTime21L(Int_t mod) {return cutLeft[mod];}
  Float_t  getCutDTime21R(Int_t mod) {return cutRight[mod];} 
  Bool_t init(HParIo* input,Int_t* set) {return isContainer;} 
  
  ClassDef(HMdcTimeCutD,0) 
};

#endif  /*!HMDCTIMECUTD_H*/ 
