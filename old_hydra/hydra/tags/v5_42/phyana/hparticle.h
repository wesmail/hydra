// Author: Dan Magestro
// Written: 22.05.01

// HParticle Class Header

#ifndef HPARTICLE_H
#define HPARTICLE_H

#include <TLorentzVector.h>

class HParticle: public TLorentzVector {

public:
  HParticle(Double_t px = 0., Double_t py = 0., Double_t pz = 0., Double_t m=0.);
  // Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticle(Int_t, Double_t px = 0., Double_t py = 0., Double_t pz = 0.);
  // Particle id, Px, Py, Pz (MeV/c)

  HParticle(const TVector3 &, Double_t);
  // 3-momentum vector (GeV/c), energy

  HParticle(const HParticle &);
  // copy constructor

  HParticle(const HParticle *);
  // copy constructor

  virtual ~HParticle() {}
  // destructor

  inline Double_t Angle(const HParticle &p) const;
  inline Double_t AngleD(const HParticle &p) const;

  Double_t P() const {return Vect().Mag();}

  Int_t GetPid() const {return pid;}
  void  SetPid(Int_t sPid) {pid = sPid;}

  Double_t GetR() const {return r;}
  void    SetR(Double_t sR) {r = sR;}

  Double_t GetZ() const {return z;}
  void    SetZ(Double_t sZ) {z = sZ;}

  Int_t GetIndex() const {return index;}
  void  SetIndex(Int_t sInd) {index = sInd;}

  Int_t GetVertexIndex() const {return vertexIndex;}
  void  SetVertexIndex(Int_t sInd) {vertexIndex = sInd;}

//  Int_t GetCharge() const {return charge;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void SetVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HParticle & operator = ( const HParticle &);

protected:

  Double_t r;         // polar coordinate of track's closest approach to beam axis
  Double_t z;         // z coordinate of track's closest approach to beam axis
  Int_t pid;          // Particle id
  Int_t index;        // Index of particle in event
  Int_t detIndex[3];  // Index of detector, track components (not implemented yet)
  Int_t vertexIndex;  // Index of vertex in HVertex

  ClassDef(HParticle,1)  // Data container for identified particles
};

inline Double_t HParticle::Angle(const HParticle &p) const {
// Function to get angle between two HParticles in radians
  TVector3 v1(Px(),Py(),Pz());
  TVector3 v2(p.Px(),p.Py(),p.Pz());
  return v1.Angle(v2);
}

inline Double_t HParticle::AngleD(const HParticle &p) const {
// Function to get angle between two HParticles in degrees
  TVector3 v1(Px(),Py(),Pz());
  TVector3 v2(p.Px(),p.Py(),p.Pz());
  return (v1.Angle(v2))*57.29578;
}

#endif /* !HPARTICLE_H */
