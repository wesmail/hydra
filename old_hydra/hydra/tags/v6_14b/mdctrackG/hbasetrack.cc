//*-- Author : Anar Rustamov 
#include "hbasetrack.h"
#include <iostream>
ClassImp(HBaseTrack)
using namespace std;

HBaseTrack::HBaseTrack() 
{
  OuterInd=-1;
  system=-1;
  tof=-1;
  beta=-1;
  mass2=-1;
}
HBaseTrack::HBaseTrack(HBaseTrack &track)
{
  z=track.z;
  r=track.r;
  theta=track.theta;
  phi=track.phi;
//  Ind1=track.Ind1;
//  Ind2=track.Ind2;
  OuterInd=track.OuterInd;
  p=track.p;
  beta=track.beta;
  mass2=track.mass2;
  tof=track.tof;
  polarity=track.polarity;
  cov=track.cov;
  sector=track.sector;
  system=track.system;
  
}
