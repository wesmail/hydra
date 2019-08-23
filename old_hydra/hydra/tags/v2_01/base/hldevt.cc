// File: hldevt.cc

//////////////////////////////////////////////////////
//HldEvent
//
//  Class used for reading LMD events from file
//
/////////////////////////////////////////////////////

#include <iomanip.h>
#include "hldevt.h"
#include "hldsubevt.h"

Int_t HldEvt::appendSubEvtIdx(HldUnpack& unpack) {
  subEvtTable[lastSubEvtIdx].id = unpack.getSubEvtId();
  subEvtTable[lastSubEvtIdx].p = unpack.getpSubEvt();
  if (lastSubEvtIdx == maxSubEvtIdx - 1)
    return 0;
  else
    return ++lastSubEvtIdx;
}


void HldEvt::dumpHdr() const {
  cout.fill('0');
  cout << "Event "
       << "SeqNr: " << getSeqNr()
       << "\tId: " << getId()
       << "\tSize: " << getSize()
       << "\tDate: "
       << setw(2) << getYear() << '/'
       << setw(2) << getMonth() << '/'
       << setw(2) << getDay() << ' '
       << setw(2) << getHour() << ':'
       << setw(2) << getMinute() << ':'
       << setw(2) << getSecond() << endl;
}

//ClassImp(HldEvt)  

