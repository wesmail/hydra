#ifndef HRPCCAL_H
#define HRPCCAL_H
#pragma interface

#include "hlocateddataobject.h"

class HRpcCal : public HLocatedDataObject {
protected:
  Float_t  rightT;   //time right
  Float_t  leftT;    //time left
  Float_t  rightTot; //ToT  right
  Float_t  leftTot;  //ToT  left
  Char_t  sector, module, cell; //Location
public:
  HRpcCal(void);
  ~HRpcCal(void);
  inline Float_t getRightTime(void);
  inline Float_t getLeftTime(void);
  inline Float_t getRightTot(void);
  inline Float_t getLeftTot(void);
  Short_t getAddress(void) {return  sector;}
  Char_t getSector(void) {return  sector;}
  Char_t getModule(void) {return  module;}
  Char_t getCell(void) {return  cell;}  
  inline void setRightTime(Float_t arightT);
  inline void setLeftTime(Float_t aleftT);
  inline void setRightTot(Float_t arightTot);
  inline void setLeftTot(Float_t aleftTot);
  inline Int_t getNLocationIndex(void);
  inline Int_t getLocationIndex(Int_t i);
  void setSector(Char_t s) { sector=s;}
  void setModule(Char_t m) { module=m;}
  void setCell(Char_t c) { cell=c;}
  void setAddress(Short_t add) { sector=add;}
  void doit(void);
  ClassDef(HRpcCal,1) //RPC cal data class
};


inline Float_t HRpcCal::getRightTime(void) {
  return  rightT;
}

inline Float_t HRpcCal::getLeftTime(void) {
  return  leftT;
}

inline Float_t HRpcCal::getRightTot(void) {
  return  rightTot;
}

inline Float_t HRpcCal::getLeftTot(void) {
  return  leftTot;
}

inline void HRpcCal::setRightTime(Float_t arightT) {
  rightT=arightT;
}
  
inline void HRpcCal::setLeftTime(Float_t aleftT) {
   leftT=aleftT;
}

inline void HRpcCal::setRightTot(Float_t arightTot){
   rightTot=arightTot;
}

inline void HRpcCal::setLeftTot(Float_t aleftTot) {
   leftTot=aleftTot;
}

inline Int_t HRpcCal::getNLocationIndex(void) {
  return 3;
}

inline Int_t HRpcCal::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getModule(); break;
  case 2 : return getCell(); break;
  }
  return -1;
}

#endif /* !HRPCCAL_H */
