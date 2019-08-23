//*-- Author : Dan Magestro
//*-- Created: 26/05/01
//*-- Last modified: 07/11/01

#ifndef HPARTICLE_H
#define HPARTICLE_H

#include <TLorentzVector.h>
#include "hphysicsconstants.h"
#include "hsymmat.h"

class HCategory;
class HKickTrack;
class HRichHit;

class HParticle: public TLorentzVector {

public:
  HParticle(Double_t px = 0., Double_t py = 0., Double_t pz = 0., Double_t m=0.);
  // Px, Py, Pz (MeV/c), mass (MeV/c**2)

  HParticle(Int_t, Double_t px = 0., Double_t py = 0., Double_t pz = 0.);
  // Particle id, Px, Py, Pz (MeV/c)

  HParticle(Char_t *Pid, Double_t px = 0., Double_t py = 0., Double_t pz = 0.);
  // Particle name, Px, Py, Pz (MeV/c)

  HParticle(const TVector3 &, Double_t);
  // 3-momentum vector (MeV/c), energy

  HParticle(const HParticle &);
  // copy constructor

  HParticle(const HParticle *);
  // copy constructor

  virtual ~HParticle() {}
  // destructor

  inline Double_t Angle(const HParticle &p) const;
  inline Double_t AngleD(const HParticle &p) const;

  Double_t P() const {return Vect().Mag();}

  Int_t getPid() const {return pid;}
  void  setPid(Int_t sPid) {pid = sPid;}
  void  setPid(Char_t *sPid) {setPid( HPhysicsConstants::pid(sPid) );}

  Int_t getCharge() const {return HPhysicsConstants::charge(pid);}

  HKickTrack* getKickTrack();
  HRichHit* getRichHit();

  Double_t getR() const {return r;}
  void     setR(Double_t sR) {r = sR;}

  Double_t getZ() const {return z;}
  void     setZ(Double_t sZ) {z = sZ;}

  Int_t getIndex() const {return index;}
  void  setIndex(Int_t sInd) {index = sInd;}

  Int_t getTrackId() const {return trackId;}
  void  setTrackId(Int_t sInd) {trackId = sInd;}

  Int_t getRingId() const {return ringId;}
  void  setRingId(Int_t sInd) {ringId = sInd;}

  Int_t getVertexId() const {return vertexId;}
  void  setVertexId(Int_t sInd) {vertexId = sInd;}

  TLorentzVector Vect4() const { return TLorentzVector(Vect(),E()); }
  void setVect4(const TLorentzVector &v) {SetPxPyPzE(v[0],v[1],v[2],v[3]);}

  HParticle & operator = ( const HParticle &);

private:
  HCategory *fKickTrackCat;  //! Kick plane category, used for getKickTrack()
  HCategory *fRichHitCat;  //! Rich hit category, used for getRichHit()


protected:

  Double_t r;         // polar coordinate of track's closest approach to beam axis
  Double_t z;         // z coordinate of track's closest approach to beam axis
  Int_t pid;          // Particle id
  Int_t index;        // Index of particle in event
  Int_t trackId;      // Index of track component in track container
  Int_t ringId;       // Index of ring component in HRichHit container (-1 = no ring used)
  Int_t vertexId;     // Index of vertex in HVertex (not used yet)
  HSymMat6 cov;       // Covariant matrix

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
