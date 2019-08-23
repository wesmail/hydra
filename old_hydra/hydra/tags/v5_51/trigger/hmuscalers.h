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
  Int_t scaler13;
  Int_t scaler14;
  Int_t scaler15;
 

public:
  HMUScalers(void);
  ~HMUScalers(void);

  void setScaler0(Int_t s) {scaler0 = s;}
  void setScaler1(Int_t s) {scaler1 = s;}
  void setScaler2(Int_t s) {scaler2 = s;}
  void setScaler3(Int_t s) {scaler3 = s;}
  void setScaler4(Int_t s) {scaler4 = s;}
  void setScaler5(Int_t s) {scaler5 = s;}
  void setScaler6(Int_t s) {scaler6 = s;}
  void setScaler7(Int_t s) {scaler7 = s;}
  void setScaler8(Int_t s) {scaler8 = s;}
  void setScaler9(Int_t s) {scaler9 = s;}
  void setScaler10(Int_t s) {scaler10 = s;}
  void setScaler11(Int_t s) {scaler11 = s;}
  void setScaler12(Int_t s) {scaler12 = s;}
  void setScaler13(Int_t s) {scaler13 = s;}
  void setScaler14(Int_t s) {scaler14 = s;}
  void setScaler15(Int_t s) {scaler15 = s;}
 
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
  Int_t getScaler13(void) {return scaler13;}
  Int_t getScaler14(void) {return scaler14;}
  Int_t getScaler15(void) {return scaler15;}

  ClassDef(HMUScalers,1)
};

#endif



