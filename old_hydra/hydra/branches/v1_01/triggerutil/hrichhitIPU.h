#ifndef HRICHHITIPU_H
#define HRICHHITIPU_H

#include <TObject.h>
#include <TArrayI.h>

class HRichHitIPU: public TObject {

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

 HRichHitIPU();
 virtual ~HRichHitIPU();

 HRichHitIPU(const HRichHitIPU& source);
 HRichHitIPU& operator=(const HRichHitIPU& source);

 void Reset();


 Int_t getX(void) {return iRingX;}
 Int_t getY(void) {return iRingY;}
 Int_t getQuality(void) {return iRingQuality;}
 Int_t getVetoQuality(void) {return iVetoQuality;}
 Int_t getSector(void) {return nSector;}
 Int_t getNPads(void) {return nPads;}
 Float_t getTheta(void) {return fTheta;}
 Float_t getPhi(void) {return fPhi;}


 void setXY(Int_t px, Int_t py) { iRingX = px; iRingY = py; }
 void setSector(Int_t s) {nSector = s;}
 void setQuality(Int_t q) { iRingQuality = q; }
 void setVetoQuality(Int_t v) { iVetoQuality = v; }
 void setTheta(Float_t th) { fTheta = th; }
 void setPhi(Float_t ph) { fPhi = ph; }



 ClassDef(HRichHitIPU,1)
};


#endif // HRICHHITIPU_H













