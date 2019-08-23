// File: htboxchansum.h
//
// Author: Rainer Schicker
// created: 2/3/2000
// Modified: 21/11/2002 D.Zovinec
// sums an htboxchan object over many events 

#ifndef HTBOXCHANSUM_H
#define HTBOXCHANSUM_H

#include "Rtypes.h"
#include "TObject.h"

class HTBoxChanSum : public TObject {
 
private:
    Int_t chanNum;   // channel number
    Int_t scaler1; // trigger box data set 1
    Int_t scaler2; // trigger box data set 2
    Int_t scaler3; // trigger box data set 3
    Int_t scaler4; // trigger box data set 4

public:
    HTBoxChanSum(void) : chanNum(0), scaler1(0), scaler2(0), scaler3(0), scaler4(0) {;}
    HTBoxChanSum(Int_t i) {chanNum=i; scaler1=0; scaler2=0; scaler3=0; scaler4=0;}
    ~HTBoxChanSum(void) {}
    void clear(void) {scaler1=0; scaler2=0; scaler3=0; scaler4=0;}
    void add(Int_t i0,Int_t i1,Int_t i2,Int_t i3,Int_t i4) {
      if(i0 != chanNum){
	 printf(" Error HTBoxChanSum i0= %3i chanNum = %3i \n",i0,chanNum);
      } else {
	scaler1+=i1; scaler2+=i2; scaler3+=i3; scaler4+=i4;
      }
    }
    void set(Int_t i0,Int_t i1,Int_t i2,Int_t i3,Int_t i4) {
      chanNum=i0; scaler1=i1; scaler2=i2; scaler3=i3; scaler4=i4;
    }
    void setChanNum(Int_t i){chanNum=i;}
    void setScaler1Data(Int_t i){scaler1=i;}
    void setScaler2Data(Int_t i){scaler2=i;}
    void setScaler3Data(Int_t i){scaler3=i;}
    void setScaler4Chan(Int_t i){scaler4=i;}
    void get(Int_t& i0,Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4) {
      i0=chanNum; i1=scaler1; i2=scaler2; i3=scaler3; i4=scaler4;
    }
    void getChanNum(Int_t& i) {i=chanNum;}
    void getScaler1Data(Int_t& i) {i=scaler1;}
    void getScaler2Data(Int_t& i) {i=scaler2;}
    void getScaler3Data(Int_t& i) {i=scaler3;}
    void getScaler4Data(Int_t& i) {i=scaler4;}
    void print(void) {
      printf(" TBoxChanSum: %3i    %8.1i  %8.1i  %8.1i  %8.1i\n",
               chanNum,scaler1,scaler2,scaler3,scaler4);
    }

ClassDef(HTBoxChanSum,1)  // TBoxChanSum data
};

#endif /* ! HTBoxChanSum_H */






