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
#define  SCALER_MAX 8
class HTBoxChan : public TObject {
 
private:

    Int_t chanNum; // channel number
	Int_t scaler[SCALER_MAX]; //scalers data

public:
    HTBoxChan(void);
    ~HTBoxChan(void) {}
    void clear(void);
    void set(Int_t i0,Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t nScalerNr=0);
    void setChannelNumber(Int_t i){chanNum=i;}
    void setScaler1Data(Int_t i){scaler[0]=i;}
    void setScaler2Data(Int_t i) {scaler[1]=i;}
    void setScaler3Data(Int_t i){scaler[2]=i;}
    void setScaler4Data(Int_t i) {scaler[3]=i;}
    void setScaler(Int_t s, Int_t i) {scaler[s]=i;}
    void get(Int_t& i0,Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4);
    void getChannelNumber(Int_t& i) {i=chanNum;}
    void getScaler1Data(Int_t& i) {i=scaler[0];}
    void getScaler2Data(Int_t& i) {i=scaler[1];}
    void getScaler3Data(Int_t& i) {i=scaler[2];}
    void getScaler4Data(Int_t& i) {i=scaler[3];}
    void getScalerData(Int_t nScalerNum, Int_t& i) {i=scaler[nScalerNum];}
    void print();

ClassDef(HTBoxChan,1)  // Trigger Box data
};

#endif /* ! HTBoxChan_H */






