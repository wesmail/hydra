// File: hrichhit.h
//
// modified by Laura Fabbietti: 22/02/2000
//

#ifndef HRICHHIT_H
#define HRICHHIT_H

#include "TObject.h"
#include "TArrayI.h"
#include "hdataobject.h"
#include "TVector.h"

class HRichHit: public HDataObject {
public:
 Int_t iRingX;
 Int_t iRingY;
 Int_t iRingQuality;
 Int_t iRingPadNr;
 Int_t iRingAmplitude;
 Int_t iRingLocalMax4;
 Int_t iRingLocalMax8;
 Int_t iRingClusterNr;
 TArrayI iRingClusterSize;
 TArrayI iRingClusterLMax4;
 TArrayI iRingClusterLMax8;
 Int_t iRingAlgorithmIndex;
 static Int_t iRingImageSize; // this must be == iRingMaskSize
 TArrayI iRingImage; 
 Int_t iRingFreeParam; //!
  
 Float_t fLabPositionX;
 Float_t fLabPositionY;
 Float_t fLabPositionZ;
 Float_t fLocalPositionX;
 Float_t fLocalPositionY;
 Float_t fThetha;
 Float_t fPhi;

 Int_t nSector;
 Int_t lEventNr;
public:
 HRichHit();
 virtual ~HRichHit();

 HRichHit(const HRichHit& source);
 HRichHit& operator=(const HRichHit& source);

 void Reset();

 Int_t getSector(void) {return nSector;}
 void setSector(Int_t s) {nSector = s;}
 Int_t getEventNr() {return lEventNr;}
 void setEventNr(Int_t lNr) {lEventNr = lNr;}
 void setTheta(TVector *theta) {fThetha = (*theta)( iRingX*100 + iRingY);}

 void getLocalXY(Float_t *px, Float_t *py) { *px = fLocalPositionX; 
                                             *py = fLocalPositionY; }
 void getLabXYZ(Float_t *px, Float_t *py, Float_t *pz) { *px = fLabPositionX;
                                                         *py = fLabPositionY;
                                                         *pz = fLabPositionZ; }
 void setLocalXY(Float_t px, Float_t py) { fLocalPositionX = px; 
                                           fLocalPositionY = py; }
 void setLabXYZ(Float_t px, Float_t py, Float_t pz) { fLabPositionX = px;
                                                      fLabPositionY = py;
                                                      fLabPositionZ = pz; }

 ClassDef(HRichHit,1) // RICH hit class
};


#endif // HRICHHIT_H




