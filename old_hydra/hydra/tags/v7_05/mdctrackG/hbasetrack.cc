//*-- Author : Anar Rustamov 
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
}
