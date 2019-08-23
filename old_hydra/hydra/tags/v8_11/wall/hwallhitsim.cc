#include "hwallhitsim.h"

// Created 18.06.00 by M.Golubeva
// Modified 11.8.2005 F. Krizek


ClassImp(HWallHitSim)

    HWallHitSim::HWallHitSim(void) { clear(); }

    void HWallHitSim::setAddress(const Int_t c) {      
       cell=c;
    }

void HWallHitSim::fill(const Float_t t, const Float_t a, const Int_t c) {
       timeCal=t;
       chargeCal=a;     
       cell=c;    
    }
    void HWallHitSim::getAddress(Int_t& c) {     
       c=cell;
    }

    void HWallHitSim::clear(void) {
       timeCal=-200.;
       chargeCal=-200.;      
       cell=-1;      
  nTrack1 = nTrack2 = -1;
    }
