#ifndef HMUSCALERS_H
#define HMUSCALERS_H

#include <TObject.h>

class HMUScalers : public TObject {
protected:

  Int_t scaler0;
  Int_t scaler1;
  Int_t scaler2;
  Int_t scaler3;
  Int_t scaler4;
  Int_t scaler5;
  Int_t scaler6;
  Int_t scaler7;
  Int_t scaler8;
  Int_t scaler9;
  Int_t scaler10;
  Int_t scaler11;
  Int_t scaler12;
  Float_t scaler13;
 

public:
  HMUScalers(void);
  ~HMUScalers(void);

  void setScaler0(UInt_t s) {scaler0 = s;}
  void setScaler1(UInt_t s) {scaler1 = s;}
  void setScaler2(UInt_t s) {scaler2 = s;}
  void setScaler3(UInt_t s) {scaler3 = s;}
  void setScaler4(UInt_t s) {scaler4 = s;}
  void setScaler5(UInt_t s) {scaler5 = s;}
  void setScaler6(UInt_t s) {scaler6 = s;}
  void setScaler7(UInt_t s) {scaler7 = s;}
  void setScaler8(UInt_t s) {scaler8 = s;}
  void setScaler9(UInt_t s) {scaler9 = s;}
  void setScaler10(UInt_t s) {scaler10 = s;}
  void setScaler11(UInt_t s) {scaler11 = s;}
  void setScaler12(UInt_t s) {scaler12 = s;}
  void setScaler13(Float_t s) {scaler13 = s;}
 
  Int_t getScaler0(void) {return scaler0;}  
  Int_t getScaler1(void) {return scaler1;}
  Int_t getScaler2(void) {return scaler2;}
  Int_t getScaler3(void) {return scaler3;}
  Int_t getScaler4(void) {return scaler4;}
  Int_t getScaler5(void) {return scaler5;}
  Int_t getScaler6(void) {return scaler6;}
  Int_t getScaler7(void) {return scaler7;}
  Int_t getScaler8(void) {return scaler8;}
  Int_t getScaler9(void) {return scaler9;}
  Int_t getScaler10(void) {return scaler10;}
  Int_t getScaler11(void) {return scaler11;}
  Int_t getScaler12(void) {return scaler12;}
  Float_t getScaler13(void) {return scaler13;}

  ClassDef(HMUScalers,1)
};

#endif



