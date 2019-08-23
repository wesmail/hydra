//*-- Author : A.Sadovski (using Anar's Rustamov code as an example)
//HKickTrackB - the output of low resolution kicktrack in a new BaseTrack scheme
//in fuure one has to put here also middle resolution and high resolution kickplane
#include "hkicktrack123B.h"
using namespace std;
#include <iostream>

HKickTrack123B::HKickTrack123B() 
{
    metaeloss  =0.;
    outherHitId=-1; 
    tofdist    =0.;
    pMeta      =0.;
}
HKickTrack123B::~HKickTrack123B()
{
  ;
}

//HKickTrackB::HKickTrackB(HKickTrackB &aTrack){
//  cout << "copy" <<endl;
//}

ClassImp(HKickTrack123B)
