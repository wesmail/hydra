//*-- AUTHOR : Ilse Koenig
//*-- Modified : 01/03/99 by Ilse Koenig

///////////////////////////////////////
//HMdcCal
//
// Data object for calibrated Mdc cell
///////////////////////////////////////

#include "hmdccal.h"

void HMdcCal::clear() {
  // clears the object
  sector = module = layer = cell = nrOfHits = status  = 0;
  time1 = 0.F;
  time2 = -1.F;
}

void HMdcCal::setTime(Float_t time) {
  // sets the drift time and increments the number of the hits
  switch (nrOfHits) {
    case 0:
      time1 = time;
      nrOfHits++;
      break;
    case 1:
      time2 = time;
      nrOfHits++;
      break;
    default:
      printf("number of hits >2\n");
  }
}

Float_t HMdcCal::getTime(const Int_t hit = 0) const {
  // returns the drift time of given hit (0 or 1)
    if (hit > nrOfHits)
        return 0;
  switch (hit) {
    case 0: return time1;
    case 1: return time2;
    default: return -1.F;
  }
}

ClassImp(HMdcCal)

