#include "hwallhit.h"

// Created 18.06.00 by M.Golubeva
// Modified 11.8.2005 F. Krizek



//void HWallHit::Streamer(TBuffer &R__b)
//{
  // Stream an object of class HWallHit.
  //  if (R__b.IsReading()) {
  //  Version_t R__v = R__b.ReadVersion();
  //  TObject::Streamer(R__b);
    //R__b >> index;
//  R__b >> timeCal;
//  R__b >> cell;
//  if(R__v <= 1){
      //edep = 0.;
//   chargeCal = 0.;
      // flagAdc = 0;
//  } else { 
      //R__b >> edep;
//   R__b >> chargeCal;
      //R__b >> flagAdc;
//  }
//} else {
//  R__b.WriteVersion(HWallHit::IsA());
//  TObject::Streamer(R__b);
    //R__b << index;
//  R__b << timeCal;
    //R__b << edep;
//  R__b << chargeCal;    
//  R__b << cell;
    //R__b << flagAdc;
//}
//}


ClassImp(HWallHit)

    HWallHit::HWallHit(void) { clear(); }

    void HWallHit::setAddress(const Int_t c) {      
       cell=c;
    }

// void HWallHit::fill(const Float_t t, const Float_t a, const Int_t c, Int_t i) {
//     timeCal=t;
//     chargeCal=a;     
//     cell=c;
//       index=i;
//    }

void HWallHit::fill(const Float_t t, const Float_t a, const Int_t c) {
       timeCal=t;
       chargeCal=a;     
       cell=c;    
    }
    void HWallHit::getAddress(Int_t& c) {     
       c=cell;
    }

    void HWallHit::clear(void) {
       timeCal=-200.;
       chargeCal=-200.;      
       cell=-1;
       //index=-1;
    }
