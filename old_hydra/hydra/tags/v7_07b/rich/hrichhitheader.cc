// File: HRichHitHeader.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Modified : Tue Feb 15 18:50:45 CET 2005 martin.jurkovic@ph.tum.de
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
 iClustersCleaned.Set(0);
 iRingNr = 0;

 iFakePad = 0;
 iFakeLocalMax4 = 0;
 iFakeLocalMax8 = 0;
 detCurrentSec=0;
 iExecutedAnalysis=0;

}

HRichHitHeader::~HRichHitHeader() {
}



