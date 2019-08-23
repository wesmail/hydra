#pragma implementation
#include "htofinoshowermapcell.h"
#include <TClass.h>

///////////////////////////////////////////////////////////////////////
// HTofinoShowerMapCell
//
// HTofinoShowerMapCell contains information about tofino detector
// attached to one pad in shower detector 
//
//////////////////////////////////////////////////////////////////////

ClassImp(HTofinoShowerMapCell)

void HTofinoShowerMapCell::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTofinoShowerMapCell.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      if(R__v < 2) {
         Version_t R__t = R__b.ReadVersion(); if (R__t) { }
      }
      TObject::Streamer(R__b);
      R__b >> m_nCellNumber;
   } else {
      R__b.WriteVersion(HTofinoShowerMapCell::IsA());
      TObject::Streamer(R__b);
      R__b << m_nCellNumber;
   }
}


