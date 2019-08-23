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

  HParticleSim(Char_t *Pid, Double_t px = 0., Double_t py = 0., Double_t pz = 0.);
  // Particle name, Px, Py, Pz (MeV/c)

  HParticleSim(const TVector3 &, Double_t);
  // 3-momentum vector (MeV/c), energy

  HParticleSim(const HParticleSim &);
  // copy constructor

  HParticleSim(const HParticleSim *);
  // copy constructor

  virtual ~HParticleSim() {}
  // destructor

  Float_t getWeight() const {return weight;}
  void    setWeight(Float_t sWeight=0) {weight = sWeight;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void setVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HParticleSim & operator = ( const HParticleSim &);

protected:

  Float_t  weight;      // Geant weight

  ClassDef(HParticleSim,1)  // Data container for simulated identified particles
};


#endif /* !HPARTICLESIM_H */
