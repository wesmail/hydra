#include "hstarthit.h"

//*-- Author :D. Zovinec

/////////////////////////////////////////////////////////////
//  HStartHit
//
//  This class contains START hit data
//
/////////////////////////////////////////////////////////////


ClassImp(HStartHit)


void HStartHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class HStartHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> time;
      R__b >> module;
      R__b >> strip;
      if(R__v>1)R__b >> startMult;
      R__b >> flag;
      R__b.CheckByteCount(R__s, R__c, HStartHit::IsA());
   } else {
      R__c = R__b.WriteVersion(HStartHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << time;
      R__b << module;
      R__b << strip;
      R__b << startMult;
      R__b << flag;
      R__b.SetByteCount(R__c, kTRUE);
   }
}






