// File: htboxchan.h
//
// Author: Rainer Schicker
// created: 2/3/00
// Modified: 21/11/2001 D.Zovinec
//

#ifndef HTBOXCHAN_H
#define HTBOXCHAN_H

#include "Rtypes.h"
#include <TObject.h>

class HTBoxChan : public TObject {
 
private:

    Int_t chanNum; // channel number
    Int_t scaler1;   // scaler 1 data
    Int_t scaler2;   // scaler 2 data
    Int_t scaler3;   // scaler 3 data
    Int_t scaler4;   // scaler 4 data

public:
    HTBoxChan(void) : scaler1(0), scaler2(0), scaler3(0), scaler4(0) {;}
    ~HTBoxChan(void) {}
    void clear(void) { chanNum=0; scaler1=0; scaler2=0; scaler3=0; scaler4=0;}
    void set(Int_t i0,Int_t i1,Int_t i2,Int_t i3,Int_t i4){chanNum=i0;scaler1=i1;scaler2=i2;scaler3=i3;scaler4=i4;}
    void setChannelNumber(Int_t i){chanNum=i;}
    void setScaler1Data(Int_t i){scaler1=i;}
    void setScaler2Data(Int_t i) {scaler2=i;}
    void setScaler3Data(Int_t i){scaler3=i;}
    void setScaler4Data(Int_t i) {scaler4=i;}
    void get(Int_t& i0,Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4){i0=chanNum;i1=scaler1;i2=scaler2;i3=scaler3;i4=scaler4;}
    void getChannelNumber(Int_t& i) {i=chanNum;}
    void getScaler1Data(Int_t& i) {i=scaler1;}
    void getScaler2Data(Int_t& i) {i=scaler2;}
    void getScaler3Data(Int_t& i) {i=scaler3;}
    void getScaler4Data(Int_t& i) {i=scaler4;}
    void print(){ printf(" TBoxCh %6.1i %6.1i %6.1i %6.1i %6.1i \n",chanNum,scaler1,scaler2,scaler3,scaler4); }

ClassDef(HTBoxChan,1)  // Trigger Box data
};

#endif /* ! HTBoxChan_H */






