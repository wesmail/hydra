//*-- Author : Anar Rustamov
//HSplineTrack
//The output of HSplineTrackF task, like HKickTrack;
//There is no matching with META yet, there is the 
//matched MDC tracks corresponding momentum defined 
//from spline fitting;
#include "hsplinetrack.h"
using namespace std;
HSplineTrack::HSplineTrack() 
{
    tofdist=-1;
    qSpline=-1;
}
HSplineTrack::~HSplineTrack()
{
  ;
}

void HSplineTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class HSplineTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HBaseTrack::Streamer(R__b);
      R__b >> tofdist;
      if(R__v>1) {
     R__b >> qSpline;
      }
      else qSpline=0;
      R__b.CheckByteCount(R__s, R__c, HSplineTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(HSplineTrack::IsA(), kTRUE);
      HBaseTrack::Streamer(R__b);
      R__b << tofdist;
      R__b << qSpline;
      R__b.SetByteCount(R__c, kTRUE);
   }
}


ClassImp(HSplineTrack)
