// File: hstartraw.cc
// Last update: 15/09/2003 T.Wojcik
// Last update: 29/02/2000 I.Koenig
// Last update: 8/12/99 r. holzmann
// Modified 12/99 by Denis Bertini <D.Bertini@gsi.de>
// modified 24/1/00 RMS
//

#include "hstartraw.h"

ClassImp(HStartRaw)

void HStartRaw::Streamer(TBuffer &R__b) {
// Stream an object of class HStartRaw.

    UInt_t R__s, R__c;
    if (R__b.IsReading()) {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
        TObject::Streamer(R__b);
        if(R__v==1){
            R__b >> time;
            R__b >> module;
            R__b >> strip;
        }
        else {
            R__b >> time;
            R__b >> timeR;
            R__b >> timeL;
            R__b >> timeM;
            R__b >> module;
            R__b >> strip;
            R__b >> nType;
        }
        R__b.CheckByteCount(R__s, R__c, HStartRaw::IsA());
    } else {
        R__c = R__b.WriteVersion(HStartRaw::IsA(), kTRUE);
        TObject::Streamer(R__b);
        R__b << time;
        R__b << module;
        R__b << strip;
        R__b << nType;
        R__b.SetByteCount(R__c, kTRUE);
    }
}

