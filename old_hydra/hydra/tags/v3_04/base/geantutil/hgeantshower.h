///////////////////////////////////////////////////////////////////////////
//  HGeantShower
//
//  GEANT SHOWER event data
//
//  last modified on 29/1/99 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTSHOWER_H
#define HGEANTSHOWER_H

#include "hlocateddataobject.h"

class HGeantShower : public HLocatedDataObject
{
private:
  Int_t trackNumber;        // GEANT track number
  Float_t eHit;             // energy deposited  (in MeV)
  Float_t xHit;             // x of hit  (in cm)
  Float_t yHit;             // y of hit  (in cm)
  Float_t thetaHit;         // angle of incidence  (0-180 deg)
  Float_t phiHit;           // azimuthal angle  (0-360 deg)
  Float_t betaHit;           // beta particle (momentum/energy) 
  Char_t sector;            // sector number  (0-5)
  Char_t module;            // module number  (0-2)
public:
  HGeantShower(void);
  HGeantShower(HGeantShower &aShower);
  ~HGeantShower(void);
  inline void setTrack(Int_t aTrack) {trackNumber = aTrack;}
  void setHit(Float_t ae, Float_t ax, Float_t ay, Float_t abeta);
  void setIncidence(Float_t ath, Float_t aph);
  void setAddress (Char_t s, Char_t m);
  inline Int_t getTrack(void) {return trackNumber;}
  void getHit(Float_t &ae, Float_t &ax, Float_t &ay, Float_t &abeta);
  void getIncidence(Float_t &ath, Float_t &aph);
  inline Char_t getSector(void) {return sector;}
  inline Char_t getModule(void) {return module;}
  inline Int_t getNLocationIndex(void) {return 2;}
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantShower,1) // Geant SHOWER event data class
};

#endif  /*! HGEANTSHOWER_H */
