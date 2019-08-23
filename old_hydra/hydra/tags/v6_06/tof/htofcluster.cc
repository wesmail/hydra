#include "htofcluster.h"

//*-- Author : D.Zovinec
//*-- Modified: 14/02/2002 D.Zovinec
/////////////////////////////
//HTofCluster
//
// Class with a TOF Cluster's data, i.e. clustered Hit's data.
//
/////////////////////////////

void HTofCluster::Streamer(TBuffer &R__b)
{
  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    HTofHit::Streamer(R__b);
    R__b >> clustSize;
    if(R__v <= 1){
      clustProbAll = 0.;
      clustProbLep = 0.;
    } else {
      R__b >> clustProbAll;
      R__b >> clustProbLep;
    }
    R__b.CheckByteCount(R__s, R__c, HTofCluster::IsA());
  } else {
    R__c = R__b.WriteVersion(HTofCluster::IsA(), kTRUE);
    HTofHit::Streamer(R__b);
    R__b << clustSize;
    R__b << clustProbAll;
    R__b << clustProbLep;
    R__b.SetByteCount(R__c, kTRUE);
  }
}

ClassImp(HTofCluster)
