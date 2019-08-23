// File: hmdcraw.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/12/04 12:39:27
//

#include <string.h>
#include "hmdcraw.h"
#include "hdebug.h"

void HMdcRaw::clear() {
    sector = module = mbo = tdc = nrOfHits =
	time1 = time2 = time3 = time4 = time5 = time6 = 0;
}

void HMdcRaw::setTime(const int time) {
    switch (nrOfHits) {
    case 0:
	time1 = time;
	nrOfHits++;
	break;
    case 1:
	time2 = time;
	nrOfHits++;
	break;
    case 2:
	time3 = time;
	nrOfHits++;
	break;
    case 3:
	time4 = time;
	nrOfHits++;
	break;
    case 4:
	time5 = time;
	nrOfHits++;
	break;
    case 5:
	time6 = time;
	nrOfHits++;
	break;
    }
}

int HMdcRaw::getTime(const int hit = 0) const {
    if (hit > nrOfHits)
	return 0;
    switch (hit) {
    case 0:
	return time1;
    case 1:
	return time2;
    case 2:
	return time3;
    case 3:
	return time4;
    case 4:
	return time5;
    case 5:
	return time6;
    }
    return 0;
}

ClassImp(HMdcRaw)
