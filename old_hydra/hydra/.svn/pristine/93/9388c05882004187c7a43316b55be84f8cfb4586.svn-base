//*-- Author : Anar Rustamov 
#include "hbasetrack.h"
#include <iostream>
ClassImp(HBaseTrack)
using namespace std;

HBaseTrack::HBaseTrack() 
{
  tofHitInd=showerHitInd=-1;
  tof=0.;
  beta=0.;
  mass2=0.;
  metaEloss=0.;
  polarity=0;
  p=0.;
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
