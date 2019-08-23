//*-- Author : Anar Rustamov 
//*-- Last modified : 10/08/2005 by Ilse Koenig

// In case of experiments where there exists no start hit and the TOF of the
// particles has to be recalculated, the tof variable(getTof()) of all classes derived from
// HBaseTrack will be overwritten by a reconstructor which recalculates the tof.
// To mark those objects as changed one can ask with getFlag(). The result should be
// 1 or 2 depending on the method which has been used and 0 if it has not been changed.
// Anyhow the original tof can be retrieved direct from the HMdcTofHit referenced by
// tofindex (getTofHitInd()). The flag itself is decoded in the tofindex, but will be
// hidden from the user by the getTofHitInd() function.
//

#include "hbasetrack.h"
#include <iostream>
ClassImp(HBaseTrack)
using namespace std;

HBaseTrack::HBaseTrack() 
{
  tofHitInd=showerHitInd=-1;
  tof=-1.;
  beta=-1.;
  mass2=-1.;
  metaEloss=-1.;
  polarity=-100;
  p=-1.;
  cov.Clear();
  tarDist=-1000.;
  tofdist=-1;
  qIOMatch=-1.;
}

HBaseTrack::HBaseTrack(HBaseTrack &track)
{
  z=track.z;
  r=track.r;
  theta=track.theta;
  phi=track.phi;
  tofHitInd=track.tofHitInd;
  showerHitInd=track.showerHitInd;
  p=track.p;
  beta=track.beta;
  mass2=track.mass2;
  tof=track.tof;
  polarity=track.polarity;
  cov=track.cov;
  sector=track.sector;
  metaEloss=track.metaEloss;
  cov.setCov(track.cov);
  tarDist=track.getTarDist();
  tofdist=track.getTofDist();
  qIOMatch=track.getIOMatch();
}

void HBaseTrack::Streamer(TBuffer &R__b)
{

  UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> z;
      R__b >> r;
      R__b >> theta;
      R__b >> phi;
      R__b >> tofHitInd;
      R__b >> showerHitInd;
      R__b >> p;
      R__b >> beta;
      R__b >> mass2;
      R__b >> tof;
      R__b >> polarity;
      R__b >> sector;
      cov.Streamer(R__b);
      R__b >> metaEloss;
      if(R__v>3){
      R__b>>tofdist;
      R__b>>tarDist;
      R__b>>qIOMatch;
      }
      
      else if(R__v>2) {
        qIOMatch=-1.; 
        R__b>>tofdist;
	R__b>>tarDist;
      }
      else if(R__v>1) 
	{
	  qIOMatch=-1.;
          tofdist=-1000.;
          R__b>>tarDist;
	}
      else
	tarDist=-1000.;
      

      R__b.CheckByteCount(R__s, R__c, HBaseTrack::IsA());
   } else {
      R__c = R__b.WriteVersion(HBaseTrack::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << z;
      R__b << r;
      R__b << theta;
      R__b << phi;
      R__b << tofHitInd;
      R__b << showerHitInd;
      R__b << p;
      R__b << beta;
      R__b << mass2;
      R__b << tof;
      R__b << polarity;
      R__b << sector;
      cov.Streamer(R__b);
      R__b << metaEloss;
      R__b << tarDist;
      R__b << tofdist;
      R__b << qIOMatch;
      R__b.SetByteCount(R__c, kTRUE);
   }


}
