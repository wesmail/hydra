// File: hgeantrich.h
// ****************************************************************************
//
// HGeantRichPhoton & HGeantRichDirect
//
// Author: Romain Holzmann (r.holzmann@gsi.de)
// Modified: 1999/10/07 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
// Reader for RICH simulated GEANT data written to ROOT file
// ****************************************************************************

#ifndef HGEANTRICH_H
#define HGEANTRICH_H


#include "hlinkeddataobject.h"

// ***************************************************************************

class HGeantRichPhoton : public HLinkedDataObject {

// ***************************************************************************

public:

  HGeantRichPhoton();
  HGeantRichPhoton(const HGeantRichPhoton &source);
  ~HGeantRichPhoton();

  void    setTrack(Int_t aTrack) { parentTrack = aTrack; }
  void    setHit(Float_t ax, Float_t ay, Float_t ae);
  void    setAddress(Char_t s) { sector = s; }

  Int_t   getTrack() { return parentTrack; }
  void    getHit(Float_t &ax, Float_t &ay, Float_t &ae);
  Float_t getEnergy() { return eHit; }
  Float_t getX() { return yHit; /* detector rotation! */ }
  Float_t getY() { return xHit; /* detector rotation! */ }
  Int_t   getSector() { return sector; }
  Int_t   getNLocationIndex() { return 1; }
  Int_t   getLocationIndex(Int_t i); 

private:

  Int_t parentTrack;     // GEANT track number of parent particle
  Float_t xHit;          // x of hit  (in cm)
  Float_t yHit;          // y of hit  (in cm)
  Float_t eHit;          // energy of photon  (in eV)
  Char_t sector;         // sector number  (0-5)


  ClassDef(HGeantRichPhoton,2) // Geant RICH event data class for photon hits
};

//============================================================================


// ***************************************************************************

class HGeantRichDirect : public HLinkedDataObject {

// ***************************************************************************

public:

  HGeantRichDirect();
  HGeantRichDirect(const HGeantRichDirect &source);
  ~HGeantRichDirect();

  void    setTrack(Int_t aTrack, Int_t aID);
  void    setHit(Float_t ax, Float_t ay, Float_t az);
  void    setMomentum(Float_t aMom, Float_t aTheta, Float_t aPhi);
  void    setELoss(Float_t ae, Float_t alen);
  void    setAddress (Char_t s) { sector = s; };

  void    getTrack(Int_t &aTrack, Int_t &aID);
  void    getHit(Float_t &ax, Float_t &ay, Float_t &az);
  void    getMomentum(Float_t &aMom, Float_t &aTheta, Float_t &aPhi);
  void    getELoss(Float_t &ade, Float_t &alen);
  Float_t getX() { return yHit; /* detector rotation! */ }
  Float_t getY() { return xHit; /* detector rotation! */ }
  Float_t getTheta() { return thetaHit; }
  Float_t getPhi() { return phiHit; }
  Float_t getEnergyLoss() { return eLoss; }
  Float_t getTrackLengthInPhotDet() { return trackLength; }
  Int_t   getSector() {return sector;}
  Int_t   getNLocationIndex() { return 1; }
  Int_t   getLocationIndex(Int_t i); 

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


  ClassDef(HGeantRichDirect,2) // Geant RICH event data class for direct hits
};

//============================================================================

#endif  // HGEANTRICH_H

