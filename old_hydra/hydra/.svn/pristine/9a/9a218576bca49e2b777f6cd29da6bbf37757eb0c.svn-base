#include "htofinocal.h"

// Created 18.06.00 by M.Golubeva
//

ClassImp(HTofinoCal)

    HTofinoCal::HTofinoCal(void) { clear(); }

    void HTofinoCal::setAddress(const Int_t s, const Int_t m, const Int_t c) {
       sector=s;
       module=m;
       cell=c;
    }

    void HTofinoCal::fill(const Float_t t, const Float_t a,
                 const Int_t s, const Int_t m, const Int_t c) {
       timeCal=t;
       chargeCal=a;
       sector=s;
       module=m;
       cell=c;
    }

    void HTofinoCal::getAddress(Int_t& s, Int_t& m, Int_t& c) {
       s=sector;
       m=module;
       c=cell;
    }

    void HTofinoCal::clear(void) {
       timeCal=-1;
       chargeCal=-1;
       sector=-1;
       module=-1;
       cell=-1;
    }

 
