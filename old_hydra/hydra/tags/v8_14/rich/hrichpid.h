#ifndef HRICHPID_H
#define HRICHPID_H

#include "TObject.h"
#include "TArrayI.h"

class HRichPID: public TObject {

public:

 Float_t fX;
 Float_t fY;
 Float_t fTheta;
 Float_t fPhi;

 Int_t iRingQuality;
 Int_t iRingAmplitude;
 Int_t iRingPadNr;

 Int_t nSector;
// Int_t lEventNr;

public:

 HRichPID();
 virtual ~HRichPID();

 HRichPID(const HRichPID& source);
 HRichPID& operator=(const HRichPID& source);

 void Reset();

 Int_t getSector(void) {return nSector;}
 void setSector(const Int_t s) {nSector = s;}
// Int_t getEventNr(){return lEventNr;}
// void setEventNr(const Int_t lNr){lEventNr = lNr;}

 Int_t getRingQuality() {return iRingQuality;}
 Int_t getRingAmplitude() {return iRingAmplitude;}
 Int_t getRingPadNr() {return iRingPadNr;}
 void setRingQuality(const Int_t rq) {iRingQuality = rq;}
 void setRingAmplitude(const Int_t ampl) {iRingAmplitude = ampl;}
 void setRingPadNr(const Int_t nr) {iRingPadNr = nr;}

 void getXY(Float_t *px, Float_t *py) { *px = fX; *py = fY; }
 void setXY(const Float_t px, const Float_t py) { fX = px; fY = py; }
 void setTheta(const Float_t th) { fTheta = th; }
 void setPhi(const Float_t phi) { fPhi = phi; }
 Float_t getTheta() { return fTheta; }
 Float_t getPhi() { return fPhi; }

 ClassDef(HRichPID,1)
};


#endif // HRICHPID_H
