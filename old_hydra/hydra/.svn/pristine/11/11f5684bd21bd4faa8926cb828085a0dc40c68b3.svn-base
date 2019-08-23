// Author: Dan Magestro
// Written: 22.05.01

// HDilepton Class Header

#ifndef HDILEPTON_H
#define HDILEPTON_H

class HParticle;

#include <TLorentzVector.h>

class HDilepton: public TLorentzVector {

public:
  HDilepton(Double_t px = 0., Double_t py = 0., Double_t pz = 0., Double_t m=0.);
  // Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HDilepton(const HParticle &p1, const HParticle &p2);
  // Two HParticles; dilepton's quantities are calculated by constructor

  HDilepton(const HDilepton &);
  // copy constructor

  HDilepton(const HDilepton *);
  // copy constructor

  virtual ~HDilepton() {}
  // destructor

  void  SetInvMass(Float_t sInvMass=0) {invMass = sInvMass;}

  Int_t GetCharge() const {return charge;}
  void  SetCharge(Int_t sCharge=0) {charge = sCharge;}

  Int_t GetPart1Index() const {return part1Index;}
  void  SetPart1Index(Int_t sPart1Index=0) {part1Index = sPart1Index;}

  Float_t GetOpeningAngle() const {return openingAngle;}

  Int_t GetPart2Index() const {return part2Index;}
  void  SetPart2Index(Int_t sPart2Index=0) {part2Index = sPart2Index;}

  void SetPart1(const HParticle &p);
  const HParticle& GetPart1() const {return part1;}

  void SetPart2(const HParticle &p);
  const HParticle& GetPart2() const {return part2;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void SetVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HDilepton & operator = ( const HDilepton &);

protected:

  HParticle part1;       // First HParticle
  HParticle part2;       // Second HParticle
  Int_t part1Index;      // Index of 1st HParticle
  Int_t part2Index;      // Index of 2nd HParticle
  Float_t invMass;       // added as data member to allow easier access through Draw() commands
  Float_t openingAngle;  // This is filled when an HParticle is set  ( SetPart1(part1)...)
  Int_t charge;          // "Charge" of dilepton; -2 = e-e-, 0=e+e-, +2=e+e+

  ClassDef(HDilepton,1)  // Data container for identified particles
};

#endif /* !HDILEPTON_H */
