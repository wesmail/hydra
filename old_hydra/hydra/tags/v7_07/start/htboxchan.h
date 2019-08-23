// File: htboxchan.h
//
// Author: Rainer Schicker
// created: 2/3/00
// Modified: 21/11/2001 D.Zovinec
// Modified: 03/09/2004 T.Wojcik
//
#ifndef HTBOXCHAN_H
#define HTBOXCHAN_H

#include "Rtypes.h"
#include <TObject.h>
#define  SCALER_MAX 8
class HTBoxChan : public TObject {
 
private:

    Int_t chanNum; // channel number
	UInt_t scaler[SCALER_MAX]; //scalers data

public:
    HTBoxChan(void);
    ~HTBoxChan(void) {}
    void clear(void);
    void set(Int_t i0,UInt_t i1,UInt_t i2,UInt_t i3,UInt_t i4,Int_t nScalerNr=0);
    void setChannelNumber(Int_t i){chanNum=i;}
    void setScaler1Data(UInt_t i){scaler[0]=i;}
    void setScaler2Data(UInt_t i) {scaler[1]=i;}
    void setScaler3Data(UInt_t i){scaler[2]=i;}
    void setScaler4Data(UInt_t i) {scaler[3]=i;}
    void setScaler(Int_t s, UInt_t i) {scaler[s]=i;}
    void get(Int_t& i0,Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4);
    void get(Int_t& i0,UInt_t& i1,UInt_t& i2,UInt_t& i3,UInt_t& i4, Int_t nScalerNr=0);
    void getChannelNumber(Int_t& i) {i=chanNum;}
    void getScaler1Data(UInt_t& i) {i=scaler[0];}
    void getScaler2Data(UInt_t& i) {i=scaler[1];}
    void getScaler3Data(UInt_t& i) {i=scaler[2];}
    void getScaler4Data(UInt_t& i) {i=scaler[3];}
    void getScalerData(Int_t nScalerNum, UInt_t& i) {i=scaler[nScalerNum];}
    void print();

ClassDef(HTBoxChan,2)  // Trigger Box data
};

#endif /* ! HTBoxChan_H */






