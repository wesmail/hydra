//*-- Author : Dan Magestro
//*-- Created: 26/05/01
//*-- Last modified: 07/11/01

// HDilepton Class Header

#ifndef HDILEPTON_H
#define HDILEPTON_H

class HParticle;

#include "TLorentzVector.h"

class HDilepton: public TLorentzVector {

public:
  HDilepton(Double_t px = 0., Double_t py = 0., Double_t pz = 0., Double_t m=0.);
  // Px, Py, Pz (MeV/c), mass (MeV/c**2)
  // This constructor assigns an opening angle = 0

  HDilepton(HParticle *p1, HParticle *p2);
  // Two HParticles; dilepton's quantities are calculated by constructor

  HDilepton(const HDilepton &);
  // copy constructor

  HDilepton(const HDilepton *);
  // copy constructor

  virtual ~HDilepton() {}
  // destructor

  void  setInvMass(Float_t sInvMass=0) {invMass = sInvMass;}

  Int_t getCharge() const {return charge;}
  void  setCharge(Int_t sCharge=0) {charge = sCharge;}

  Int_t getPart1Id() const {return part1Id;}
  void  setPart1Id(Int_t sPart1Id=0) {part1Id = sPart1Id;}

  Int_t getPart2Id() const {return part2Id;}
  void  setPart2Id(Int_t sPart2Id=0) {part2Id = sPart2Id;}

  Float_t getOpeningAngle() const {return openingAngle;}

  void  setPart1(HParticle *p);
  HParticle* getPart1() const {return part1;}

  void setPart2(HParticle *p);
  HParticle* getPart2() const {return part2;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void setVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HDilepton & operator = ( const HDilepton &);

protected:

  HParticle *part1;     //! Particle 1
  HParticle *part2;     //! Particle 2
  Int_t part1Id;        // Index of 1st HParticle
  Int_t part2Id;        // Index of 2nd HParticle
  Float_t invMass;      // added as data member to allow easier access through Draw() commands
  Float_t openingAngle; // DEGREES.  Filled when an HParticle is set  ( SetPart1(part1)...)
  Int_t charge;         // "Charge" of dilepton; -2 = e-e-, 0=e+e-, +2=e+e+

  ClassDef(HDilepton,1)  // Data container for identified particles
};

#endif /* !HDILEPTON_H */
