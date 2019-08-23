#ifndef HRICHHIT_H
#define HRICHHIT_H

#include "TObject.h"
#include "TArrayI.h"
#include "hdataobject.h"

class HRichHit: public HDataObject {

public:

 Int_t iRingX;
 Int_t iRingY;
 Int_t iRingQuality; //!
 Int_t iRingPatMat;
 Int_t iRingHouTra;
 short iRingAmplitude;
 short iRingPadNr;
 short iRingLocalMax4;
 short iRingLocalMax8;
 short iRingClusterNr;
 short iRingAlgorithmIndex;
 Float_t iRingClusterSize;
 Float_t iRingClusterLMax4; //mean size of clusters which can touch on diagonal pads
 Float_t iRingClusterLMax8; //mean size of clusters which do not touch
 Float_t centroid; //do not use to calulate ring center
 Int_t iCentX; //centroid coordinates in pad units
 Int_t iCentY;
 Float_t radius;
 static Int_t iRingImageSize; //! this must be == iRingMaskSize
 short iRingImage[256]; //!
 Int_t iRingFreeParam; //!

 Float_t fLabX;
 Float_t fLabY;
 Float_t fLabZ;

 Float_t fX;
 Float_t fY;
 Float_t fTheta;
 Float_t fPhi;

 Int_t nSector;
// Int_t lEventNr;

public:

 HRichHit();
 virtual ~HRichHit();

 HRichHit(const HRichHit& source);
 HRichHit& operator=(const HRichHit& source);

 void Reset();

 Int_t getSector(void) {return nSector;}
 void setSector(Int_t s) {nSector = s;}
// Int_t getEventNr(){return lEventNr;}
// void setEventNr(Int_t lNr){lEventNr = lNr;}

 void getXY(Float_t *px, Float_t *py) { *px = fX; *py = fY; }
 void setXY(Float_t px, Float_t py) { fX = px; fY = py; }
 void getLabXYZ(Float_t *px, Float_t *py, Float_t *pz) { *px = fLabX;
                                                         *py = fLabY;
                                                         *pz = fLabZ; }
 void setLabXYZ(Float_t px, Float_t py, Float_t pz) { fLabX = px;
                                                      fLabY = py;
                                                      fLabZ = pz; }
 void setTheta(const Float_t th) { fTheta = th; }
 void setPhi(const Float_t phi) { fPhi = phi; }
 Float_t getTheta() { return fTheta; }
 Float_t getPhi() { return fPhi; }

 ClassDef(HRichHit,1)
};


#endif // HRICHHIT_H
