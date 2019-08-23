///////////////////////////////////////////////////////////////////////////
//  HGeantRich
//
//  GEANT RICH event data testtttttt
//
//  last modified on 29/1/99 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTRICH_H
#define HGEANTRICH_H

#include "hlocateddataobject.h"

class HGeantRichPhoton : public HLocatedDataObject
{
private:
  Int_t parentTrack;     // GEANT track number of parent particle
  Float_t xHit;          // x of hit  (in cm)
  Float_t yHit;          // y of hit  (in cm)
  Float_t eHit;          // energy of photon  (in eV)
  Char_t sector;         // sector number  (0-5)
public:
  HGeantRichPhoton(void);
  HGeantRichPhoton(HGeantRichPhoton &aRich);
  ~HGeantRichPhoton(void);
  inline void setTrack(Int_t aTrack) {parentTrack = aTrack;}
  void setHit(Float_t ax, Float_t ay, Float_t ae);
  inline void setAddress (Char_t s) {sector = s;}
  inline Int_t getTrack() {return parentTrack;}
  void getHit(Float_t &ax, Float_t &ay, Float_t &ae);
  inline Char_t getSector(void) {return sector;}
  inline Int_t getNLocationIndex(void) {return 1;}
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantRichPhoton,1) // Geant RICH event data class for photon hits
};


class HGeantRichDirect : public HLocatedDataObject
{
private:
  Int_t trackNumber;     // GEANT track number of particle
  Float_t xHit;          // x of hit  (in cm)
  Float_t yHit;          // y of hit  (in cm)
  Float_t zHit;          // z of hit  (in cm)
  Int_t particleID;      // GEANT particle ID
  Float_t momentum;      // total momentum of particle  (in MeV/c)
  Float_t thetaHit;      // theta of incidence  (0-180 deg)
  Float_t phiHit;        // azimuthal angle  (0-360 deg)
  Float_t eLoss;         // energy loss  (in MeV)
  Float_t trackLength;      // track length in gas detector  (in cm)
  Char_t sector;            // sector number  (0-5)
public:
  HGeantRichDirect(void);
  HGeantRichDirect(HGeantRichDirect &aRich);
  ~HGeantRichDirect(void);
  void setTrack(Int_t aTrack, Int_t aID);
  void setHit(Float_t ax, Float_t ay, Float_t az);
  void setMomentum(Float_t aMom, Float_t aTheta, Float_t aPhi);
  void setELoss(Float_t ae, Float_t alen);
  inline void setAddress (Char_t s) {sector = s;};
  void getTrack(Int_t &aTrack, Int_t &aID);
  void getHit(Float_t &ax, Float_t &ay, Float_t &az);
  void getMomentum(Float_t &aMom, Float_t &aTheta, Float_t &aPhi);
  void getELoss(Float_t &ade, Float_t &alen);
  inline Char_t getSector(void) {return sector;}
  inline Int_t getNLocationIndex(void) {return 1;}
  inline Int_t getLocationIndex(Int_t i); 

  ClassDef(HGeantRichDirect,1) // Geant RICH event data class for direct hits
};

#endif  /*! HGEANTRICH_H */









