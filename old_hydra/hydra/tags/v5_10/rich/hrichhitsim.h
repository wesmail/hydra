// File: hrichhit.h
//
// modified by Laura Fabbietti: 22/02/2000
//
#ifndef HRICHHITSIM_H
#define HRICHHITSIM_H
#include "hrichhit.h"
//#include "TArrayI.h"

class HRichHitSim: public HRichHit{
public:
 HRichHitSim();
 virtual ~HRichHitSim(){}
   //LF
 Int_t iRingPadNTrack1[256];//!
 Int_t iRingPadNTrack2[256];//!
    Int_t iRingTrack[256];//!
    Int_t iRingFlag[256];//!
    Int_t track1, track2, track3;//for each ring maximally 3 tracks are stored.
    Int_t weigTrack1, weigTrack2, weigTrack3; 
    Int_t flag1,flag2,flag3;// flag that distinguishes Photons from IP
 HRichHitSim& operator=(const HRichHitSim& source);
    //LF
 ClassDef(HRichHitSim,1)
};

#endif /* !HRICHHITSIM_H */
