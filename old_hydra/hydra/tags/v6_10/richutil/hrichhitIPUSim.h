#ifndef HRICHHITIPUSIM_H
#define HRICHHITIPUSIM_H

#include <TObject.h>
#include <TArrayI.h>

class HRichHitIPUSim: public TObject {

public:

 Int_t iRingX;
 Int_t iRingY;
 Int_t iRingQuality; 
 Int_t iVetoQuality; 
 Int_t nPads;
 Float_t fTheta;
 Float_t fPhi;

 Int_t nSector;

public:

 HRichHitIPUSim();
 virtual ~HRichHitIPUSim();

 HRichHitIPUSim(const HRichHitIPUSim& source);
 HRichHitIPUSim& operator=(const HRichHitIPUSim& source);

 void Reset();


 Int_t getX(void) {return iRingX;}
 Int_t getY(void) {return iRingY;}
 Int_t getQuality(void) {return iRingQuality;}
 Int_t getVetoQuality(void) {return iVetoQuality;}
 Int_t getSector(void) {return nSector;}
 Int_t getNPads(void) {return nPads;}
 Float_t getTheta(void) {return fTheta;}
 Float_t getPhi(void) {return fPhi;}


 void setSector(Int_t s) {nSector = s;}
 void setQuality(Int_t q) { iRingQuality = q; }
 void setVetoQuality(Int_t v) { iVetoQuality = v; }
 void setXY(Int_t px, Int_t py) { iRingX = px; iRingY = py; }
 void setTheta(Float_t th) { fTheta = th; }
 void setPhi(Float_t ph) { fPhi = ph; }

 ClassDef(HRichHitIPUSim,1)
};


#endif // HRICHHITIPUSIM_H













