// $Id: hrichphotonhit.h,v 1.3 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 18:14:09
//
#ifndef HRICHPHOTONHIT_H
#define HRICHPHOTONHIT_H

#include <TObject.h>
#include "hcategory.h"

class HRichPhotonHit: public TObject {

private:

    Float_t fX;//interpolated x-coord of photon hit in pad units
    Float_t fY; //interpolated y-coord of photon hit in pad units
    Float_t fTheta;//interpolated theta (polar angle) of photon hit in degrees
    Float_t fPhi;//interpolated phi (azimuthal angle) of photon hit in degrees 
    Int_t nSector;// RICH sector [0;5]
    Float_t fCharge;//summed charge induced by by photo electron
    Int_t nRingX;//ring x coord the photon hit belongs to
    Int_t nRingY;//ring y coord the photon hit belongs to
public:

 HRichPhotonHit();
 virtual ~HRichPhotonHit();

 HRichPhotonHit(const HRichPhotonHit& source);
 HRichPhotonHit& operator=(const HRichPhotonHit& source);

 void Reset();

 Int_t getSector(void) {return nSector;};
 void setSector(Int_t s) {nSector = s;};
 void setTheta(const Float_t th) { fTheta = th; };
 void setPhi(const Float_t phi) { fPhi = phi; };
 Float_t getTheta() { return fTheta; };
 Float_t getPhi() { return fPhi; };
 void setX(Float_t x){fX=x;};
 void setY(Float_t y){fY=y;};
 void setCharge(Float_t c){fCharge = c;};
 Float_t getX(){return fX;};
 Float_t getY(){return fY;};
 Float_t getCharge(){return fCharge;}
 void setRingX(Int_t x){nRingX=x;};
 void setRingY(Int_t y){nRingY=y;};
 Int_t getRingX(){return nRingX;};
 Int_t getRingY(){return nRingY;};

 ClassDef(HRichPhotonHit,1)
};


#endif // HRICHHIT_H
