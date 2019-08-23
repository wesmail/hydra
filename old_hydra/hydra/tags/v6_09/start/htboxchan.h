// File: htboxchan.h
//
// Author: Rainer Schicker
// created: 2/3/00
//

#ifndef HTBOXCHAN_H
#define HTBOXCHAN_H

#include "Rtypes.h"
#include <TObject.h>

class HTBoxChan : public TObject {
 
private:

    Int_t ch; // channel number
    Int_t in;   // trigger input
    Int_t dt;   // after deadtime
    Int_t red;  // after reduction  

public:
    HTBoxChan(void) : in(0), dt(0), red(0) {;}
    ~HTBoxChan(void) {}
    void clear(void) { ch=0; in=0; dt=0; red=0;}
    void set(Int_t i1,Int_t i2,Int_t i3,Int_t i4){ch=i1;in=i2;dt=i3;red=i4;}
    void setch(Int_t i1){ch=i1;}
    void setin(Int_t i1){in=i1;}
    void setdt(Int_t i1) {dt=i1;}
    void setred(Int_t i1) {red=i1;}
    void get(Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4){i1=ch;i2=in;i3=dt;i4=red;}
    void getch(Int_t& i1) {i1=ch;}
    void getin(Int_t& i1) {i1=in;}
    void getdt(Int_t& i1) {i1=dt;}
    void getred(Int_t& i1) {i1=red;}
    void print(){ printf(" TBoxCh %6.1i %6.1i %6.1i %6.1i \n",ch,in,dt,red); }

ClassDef(HTBoxChan,1)  // Trigger Box data
};

#endif /* ! HTBoxChan_H */






