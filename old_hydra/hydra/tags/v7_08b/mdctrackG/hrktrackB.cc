//*-- Author : A.Sadovsky
//*-- Last modified : 10/08/2005 by Ilse Koenig

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  HRKTrackB 
//
//  data container holding all information from Runge-Kutta tracking
//  See HBaseTrack for functions and data members which are not listed here...
// 
//  void setChiq(Double_t Chiq)      - Sets #chi^{2}_{RK}
//  void setMETAdx(Float_t x)        - Sets x-component of vector of difference between Runge-Kutta track intersection on the surface of the META subdetector and the coordinate of hit/cluster on META detector
//  void setMETAdy(Float_t y)        - Sets y-component of vector of difference between Runge-Kutta track intersection on the surface of the META subdetector and the coordinate of hit/cluster on META detector
//  void setMETAdz(Float_t z)        - Sets z-component of vector of difference between Runge-Kutta track intersection on the surface of the META subdetector and the coordinate of hit/cluster on META detector
//
//  Float_t getChiq(void)    - #chi^{2} of Runge Kutta tracking from x,y points (fitted minus measured) on each of the MDCs
//  Float_t getMETAdx(void)  - x-deviation of track trajectory of Runge Kutta propagation from the hit/cluster on the META detector
//  Float_t getMETAdy(void)  - y-deviation of track trajectory of Runge Kutta propagation from the hit/cluster on the META detector
//  Float_t getMETAdz(void)  - z-deviation of track trajectory of Runge Kutta propagation from the hit/cluster on the META detector
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hrktrackB.h"
using namespace std;
#include <iostream>

ClassImp(HRKTrackB)

HRKTrackB::HRKTrackB(){
    chiq        = -1.0;
    dxRkMeta    = -1000.0;
    dyRkMeta    = -1000.0;
    dzRkMeta    = -1000.0;
    zSeg1RK     = -1000.;
    rSeg1RK     = -1000;
    thetaSeg1RK = 0.;
    phiSeg1RK   = 0.;
    zSeg2RK     = -1000.;
    rSeg2RK     = -1000;
    thetaSeg2RK = 0.;
    phiSeg2RK   = 0.;
}

void HRKTrackB::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRKTrackB.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HBaseTrack::Streamer(R__b);
      if (R__v>1) {
         R__b >> chiq;
         R__b >> zSeg1RK;
         R__b >> rSeg1RK;
         R__b >> thetaSeg1RK;
         R__b >> phiSeg1RK;
         R__b >> zSeg2RK;
         R__b >> rSeg2RK;
         R__b >> thetaSeg2RK;
         R__b >> phiSeg2RK;
         R__b >> dxRkMeta;
         R__b >> dyRkMeta;
         R__b >> dzRkMeta;
      } else {
         R__b >> chiq;
         Float_t d;
         R__b >> d;
         tofdist=(Double_t)d;
         R__b >> dxRkMeta;
         R__b >> dyRkMeta;
         R__b >> dzRkMeta;
         zSeg1RK     = -1000.;
         rSeg1RK     = -1000;
         thetaSeg1RK = 0.;
         phiSeg1RK   = 0.;
         zSeg2RK     = -1000.;
         rSeg2RK     = -1000;
         thetaSeg2RK = 0.;
         phiSeg2RK   = 0.;
      }
      R__b.CheckByteCount(R__s, R__c, HRKTrackB::IsA());
   } else {
      R__c = R__b.WriteVersion(HRKTrackB::IsA(), kTRUE);
      HBaseTrack::Streamer(R__b);
      R__b << chiq;
      R__b << zSeg1RK;
      R__b << rSeg1RK;
      R__b << thetaSeg1RK;
      R__b << phiSeg1RK;
      R__b << zSeg2RK;
      R__b << rSeg2RK;
      R__b << thetaSeg2RK;
      R__b << phiSeg2RK;
      R__b << dxRkMeta;
      R__b << dyRkMeta;
      R__b << dzRkMeta;
      R__b.SetByteCount(R__c, kTRUE);
   }
}
