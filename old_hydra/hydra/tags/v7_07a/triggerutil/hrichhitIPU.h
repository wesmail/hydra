//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichhitIPU.h,v 1.4 2005-07-15 09:02:50 wuestenf Exp $

#ifndef HRICHHITIPU_H
#define HRICHHITIPU_H

#include <TObject.h>

class HRichHitIPU: public TObject
{

protected:
  Int_t iRingX;        // x coordinate of the ring
  Int_t iRingY;        // y coordinate of the ring
  Int_t iRingQuality;  // quality factor of the ring
  Int_t iVetoQuality;  // quality of the veto region
  Int_t nPads;         // Number of pads that contribute to the ring
  Float_t fTheta;      // Theta of the ring
  Float_t fPhi;        // Phi of the ring
  Int_t DataWord;      // ???
  Int_t nSector;       // Sector of the ring [0..5]

public:
  HRichHitIPU(void);
  virtual ~HRichHitIPU(void){};

  HRichHitIPU(const HRichHitIPU& source);
  HRichHitIPU& operator=(const HRichHitIPU& source);

  void Reset(void);

  Int_t getX(void) {return iRingX;}
  Int_t getY(void) {return iRingY;}
  Int_t getQuality(void) {return iRingQuality;}
  Int_t getVetoQuality(void) {return iVetoQuality;}
  Int_t getDataWord(void) {return DataWord;}
  Int_t getSector(void) {return nSector;}
  Int_t getNPads(void) {return nPads;}
  Float_t getTheta(void) {return fTheta;}
  Float_t getPhi(void) {return fPhi;}

  void setX(Int_t px) { iRingX = px; }
  void setY(Int_t py) { iRingY = py; }
  void setDataWord(Int_t dw) {DataWord = dw;}
  void setSector(Int_t s) {nSector = s;}
  void setQuality(Int_t q) { iRingQuality = q; }
  void setVetoQuality(Int_t v) { iVetoQuality = v; }
  void setTheta(Float_t th) { fTheta = th; }
  void setPhi(Float_t ph) { fPhi = ph; }
  void setNPads(Int_t pd){nPads = pd;};

  ClassDef(HRichHitIPU,1)  // RICH IPU hit information
};
#endif // HRICHHITIPU_H
