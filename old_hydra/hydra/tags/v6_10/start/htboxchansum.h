// File: htboxchansum.h
//
// Author: Rainer Schicker
// created: 2/3/2000
// sums an htboxchan object over many events 

#ifndef HTBOXCHANSUM_H
#define HTBOXCHANSUM_H

#include "Rtypes.h"
#include <TObject.h>

class HTBoxChanSum : public TObject {
 
private:
    Int_t ch;   // channel number
    Int_t in;   // raw = trigger input
    Int_t dt;   // inhibited = with deadtime
    Int_t red;  // reduced  

public:
    HTBoxChanSum(void) : ch(0), in(0), dt(0), red(0) {;}
    HTBoxChanSum(Int_t i) {ch=i; in=0; dt=0; red=0;}
    ~HTBoxChanSum(void) {}
    void clear(void) {in=0; dt=0; red=0;}
    void add(Int_t i1,Int_t i2,Int_t i3,Int_t i4) {
      if(i1 != ch){
	 printf(" Error HTBoxChanSum i1 = %3i ch = %3i \n",i1,ch);
      } else {
	in+=i2; dt+=i3; red+=i4;
      }
    }
    void set(Int_t i1,Int_t i2,Int_t i3,Int_t i4) {
      ch=i1; in=i2; dt=i3; red=i4;
    }
    void setCh(Int_t i1){ch=i1;}
    void setIn(Int_t i1){in=i1;}
    void setDt(Int_t i1) {dt=i1;}
    void setRed(Int_t i1) {red=i1;}
    void get(Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4) {
      i1=ch; i2=in; i3=dt; i4=red;
    }
    void getCh(Int_t& i1) {i1=ch;}
    void getIn(Int_t& i1) {i1=in;}
    void getDt(Int_t& i1) {i1=dt;}
    void getRed(Int_t& i1) {i1=red;}
    void print(void) { 
      printf(" TBoxChanSum: %3i  Raw:%8.1i Inhibited:%8.1i Reduced:%8.1i \n",
               ch,in,dt,red);
    }

ClassDef(HTBoxChanSum,1)  // TBoxChanSum data
};

#endif /* ! HTBoxChanSum_H */






