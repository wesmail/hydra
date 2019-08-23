// Author: Dan Magestro
// Written: 29.05.01

// HParticleSim Class Header

#ifndef HPARTICLESIM_H
#define HPARTICLESIM_H

#include "hparticle.h"

class HParticleSim : public HParticle {

public:
  HParticleSim(Double_t px=0., Double_t py=0., Double_t pz=0., Double_t m=0.);
  // Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticleSim(Int_t, Double_t px=0., Double_t py=0., Double_t pz=0.);
  // Particle id, Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticleSim(Int_t, Int_t, Double_t px=0., Double_t py=0., Double_t pz=0.);
  // GEANT track number, Particle id, Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticleSim(const TVector3 &, Double_t);
  // 3-momentum vector (GeV/c), energy

  HParticleSim(const HParticleSim &);
  // copy constructor

  HParticleSim(const HParticleSim *);
  // copy constructor

  virtual ~HParticleSim() {}
  // destructor

  Int_t GetTrackNumber() const {return trackNumber;}
  void  SetTrackNumber(Int_t sTrackNumber=0) {trackNumber = sTrackNumber;}

  Float_t GetWeight() const {return weight;}
  void  SetWeight(Float_t sWeight=0) {weight = sWeight;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void SetVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HParticleSim & operator = ( const HParticleSim &);

protected:

  Float_t    weight;      // Geant weight
  Int_t    trackNumber; // Geant track number

  ClassDef(HParticleSim,1)  // Data container for simulated identified particles
};


#endif /* !HPARTICLESIM_H */
