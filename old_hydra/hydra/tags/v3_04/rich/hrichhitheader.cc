// File: HRichHitHeader.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#include "hrichhitheader.h" 

ClassImp(HRichHitHeader)


HRichHitHeader::HRichHitHeader() {
 setSector(-1);

 iEventNr = 0;
 iPadFiredNr = 0;
 iPadCleanedNr = 0;
 iClusterCleanedNr = 0;
 iRingNr = 0;

 iFakeLocalMax4 = 0;
 iFakeLocalMax8 = 0;

}

HRichHitHeader::~HRichHitHeader() {
}



