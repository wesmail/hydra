// Author: Dan Magestro
// Written: 29.05.01

// HParticleSrc Class Header

#ifndef HPARTICLESRC_H
#define HPARTICLESRC_H

#include "hparticle.h"

class HParticleSrc : public HParticle {

public:
  HParticleSrc(Double_t px=0., Double_t py=0., Double_t pz=0., Double_t m=0.);
  // Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticleSrc(Int_t, Double_t px=0., Double_t py=0., Double_t pz=0.);
  // Particle id, Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticleSrc(Char_t *Pid, Double_t px = 0., Double_t py = 0., Double_t pz = 0.);
  // Particle name, Px, Py, Pz (MeV/c)

  HParticleSrc(Int_t, Int_t, Double_t px=0., Double_t py=0., Double_t pz=0.);
  // GEANT track number, Particle id, Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticleSrc(const TVector3 &, Double_t);
  // 3-momentum vector (MeV/c), energy

  HParticleSrc(const HParticleSrc &);
  // copy constructor

  HParticleSrc(const HParticleSrc *);
  // copy constructor

  virtual ~HParticleSrc() {}
  // destructor

  Int_t getTrackNumber() const {return trackNumber;}
  void  setTrackNumber(Int_t sTrackNumber=0) {trackNumber = sTrackNumber;}

  Float_t getWeight() const {return weight;}
  void    setWeight(Float_t sWeight=0) {weight = sWeight;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void setVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HParticleSrc & operator = ( const HParticleSrc &);

protected:

  Float_t  weight;      // Geant weight
  Int_t    trackNumber; // Geant track number

  ClassDef(HParticleSrc,1)  // Data container for simulated identified particles
};


#endif /* !HPARTICLESRC_H */
