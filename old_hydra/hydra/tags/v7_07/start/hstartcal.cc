// File: hstartcal.cc
// Last update: 29/02/2000 I.Koenig
// created 24/1/00 RMS
//

#include "hstartcal.h"

ClassImp(HStartCal)

void HStartCal::fill(const Float_t t, const Int_t m,const Int_t s, Int_t n) {
	module=m;
	strip=s;
	switch (n){
		case TYPE_RIGHT:
			timeR=t;
			break;
		case TYPE_LEFT:
			timeL=t;
			break;
		case TYPE_MEANTIMER:
			timeM=t;
			break;
		default:
			break;
	}
}

void HStartCal::Streamer(TBuffer &R__b)
{
   // Stream an object of class HStartCal.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
			if(R__v==1){
				R__b >> time;
				R__b >> module;
				R__b >> strip;
			}	
			else{
				R__b >> time;
				R__b >> timeR;
				R__b >> timeL;
				R__b >> timeM;
				R__b >> fTof;
				R__b >> fPos;
				R__b >> nType;
				R__b.ReadStaticArray(nMult);
				R__b >> module;
				R__b >> strip;
			}
		R__b.CheckByteCount(R__s, R__c, HStartCal::IsA());
	} else {
			R__c = R__b.WriteVersion(HStartCal::IsA(), kTRUE);
			TObject::Streamer(R__b);
			R__b << time;
			R__b << timeR;
			R__b << timeL;
			R__b << timeM;
			R__b << fTof;
			R__b << fPos;
			R__b << nType;
			R__b.WriteArray(nMult, 6);
			R__b << module;
			R__b << strip;
			R__b.SetByteCount(R__c, kTRUE);
}
}






