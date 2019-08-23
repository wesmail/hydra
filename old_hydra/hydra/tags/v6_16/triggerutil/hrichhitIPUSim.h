#ifndef HRICHHITIPUSIM_H
#define HRICHHITIPUSIM_H

#include <TObject.h>
#include <TArrayI.h>
#include "hrichhitIPU.h"

class HRichHitIPUSim: public HRichHitIPU {

public:
 HRichHitIPUSim();
 virtual ~HRichHitIPUSim();

 short iRingPadNTrack1[256];//!
 short iRingPadNTrack2[256];//!
 short iRingTrack[256];//!
 char iRingFlag[256];//!
 Int_t track1, track2, track3;
 Int_t weigTrack1, weigTrack2, weigTrack3;
 Int_t flag1,flag2,flag3;

 HRichHitIPUSim& operator=(const HRichHitIPUSim& source){
   HRichHitIPU::operator=(source);
   return *this;
 }
 HRichHitIPUSim& operator=(const HRichHitIPU& source){
   HRichHitIPU::operator=(source);
   return *this;
 }

 ClassDef(HRichHitIPUSim,1)
};
#endif // HRICHHITIPUSIM_H













