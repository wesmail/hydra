////////////////////////////////////
//HMdcCal
//
////////////////////////////////////

#include "hmdccal.h"

void HMdcCal::clear() {
  sector = module = layer = cell = nrOfHits = time1 = time2 = 0;
}

void HMdcCal::setTime(Float_t time) {
    switch (nrOfHits) {
    case 0:
        time1 = time;
        nrOfHits++;
        break;
    case 1:
        time2 = time;
        nrOfHits++;
        break;
    }
}

Float_t HMdcCal::getTime(const Int_t hit = 0) const {
    if (hit > nrOfHits)
        return 0;
    switch (hit) {
    case 0:
        return time1;
    case 1:
        return time2;
    }
}

ClassImp(HMdcCal)

