//*-- Author : A.Sadovski (using Anar's Rustamov code as an example)
//*-- Last modified : 10/08/2005 by Ilse Koenig


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// HKickTrack123B 
//
//   the output of middle(MDC123M) and high(MDC1234) resolution kicktrack in a new BaseTrack scheme
//
//
// Inline functions (except the ones from HBaseTrack) are:
//
//   void setMetaEloss(Float_t t)      - Sets energy loss in META detector
//   void setOuterHitId( Int_t i)      - Sets Id of the hit in meta detector (original KickPlane feature)
//   void setPMeta(Float_t pm)         - Sets momentum estimate from Time-of-Flight measurement
//
//   Float_t getMetaEloss(void)        - energy loss in META detector
//   Int_t   getOuterHitId( void)      - Id of the hit in meta detector (original KickPlane feature)
//   Float_t getPMeta(void)            - momentum estimate from Time-of-Flight measurement
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hkicktrack123B.h"
using namespace std;
#include <iostream>


HKickTrack123B::HKickTrack123B() 
{
    metaeloss  =0.;
    outherHitId=-1; 
    pMeta      =0.;
}
HKickTrack123B::~HKickTrack123B()
{
  ;
}

//HKickTrackB::HKickTrackB(HKickTrackB &aTrack){
//  cout << "copy" <<endl;
//}

void HKickTrack123B::Streamer(TBuffer &R__b)
{
   // Stream an object of class HKickTrack123B.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HBaseTrack::Streamer(R__b);
      R__b >> metaeloss;
      R__b >> outherHitId;
      if(R__v<2) {
        R__b>>tofdist;
      }
      R__b >> pMeta;
      R__b.CheckByteCount(R__s, R__c, HKickTrack123B::IsA());
   } else {
      R__c = R__b.WriteVersion(HKickTrack123B::IsA(), kTRUE);
      HBaseTrack::Streamer(R__b);
      R__b << metaeloss;
      R__b << outherHitId;
      R__b << pMeta;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(HKickTrack123B)
