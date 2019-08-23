// $Id: hrichphotonhitheader.h,v 1.3 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 18:14:35
//
#ifndef HRICHPHOTONHITHEADER_H
#define HRICHPHOTONHITHEADER_H

#include "TObject.h"
#include "hcategory.h"

class HRichPhotonHitHeader: public TObject {

private:

    Int_t nRingX;//x coord of a ring for which more than 3 photons were found 
    Int_t nRingY;//y coord of a ring for which more than 3 photons were found
    Float_t fRingPhi;
    Float_t fRingTheta;
    Int_t nSector;
    Int_t nPhotonsFound;//number of recognized photons for ring
public:

 HRichPhotonHitHeader();
 virtual ~HRichPhotonHitHeader();

 HRichPhotonHitHeader(const HRichPhotonHitHeader& source);
 HRichPhotonHitHeader& operator=(const HRichPhotonHitHeader& source);

 void Reset();

 Int_t getSector(void) {return nSector;};
 void setSector(Int_t s) {nSector=s;};
 void setRingPhi(Int_t x){fRingPhi=x;};
 void setRingTheta(Int_t y){fRingTheta=y;};
 Float_t getRingPhi(){return fRingPhi;};
 Float_t getRingTheta(){return fRingTheta;};
 void setRingX(Int_t x){nRingX=x;};
 void setRingY(Int_t y){nRingY=y;};
 Int_t getRingX(){return nRingX;};
 Int_t getRingY(){return nRingY;};
 void setNbPhotons(Int_t nb){nPhotonsFound=nb;};
 Int_t getNbPhotons(){return nPhotonsFound;};
 ClassDef(HRichPhotonHitHeader,1)
};


#endif // HRICHPHOTONHITHEADER_H
