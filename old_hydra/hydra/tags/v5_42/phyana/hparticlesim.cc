//*-- Author : Dan Magestro
//*-- Created: 29/05/2001
/////////////////////////////////////////////////////////////////////////////////////////////////////
//  HParticleSim
//
//  Data container for simulated identified particles and their 4-momenta.
//  Currently, HParticleSim is simply HParticle with trackNumber from HGEANT
//
//  This class derives from HParticle, which in turn derives from
//  TLorentzVector, so that all of the functions available to
//  TLorentzVector can be performed on HParticle objects:
//
//      HParticleSim *part1 = new HParticleSim(50.,50.,50.,938.);
//
//      HParticleSim *part2 = new HParticleSim(30.,30.,30.,938.);
//
//      Float_t pxSum = part1.Px() + part2.Px()
//      .....
//
//  HParticleSim's can be initialized as follows:
//      part = new HParticleSim();
//      part = new HParticleSim(Double_t px, Double_t py, Double_t pz);
//      part = new HParticleSim(Double_t px, Double_t py, Double_t pz, Double_t m);
//      part = new HParticleSim(Int_t Pid, Double_t px, Double_t py, Double_t pz);
//      part = new HParticleSim(Int_t trackNumber, Int_t Pid, Double_t px, Double_t py, Double_t pz);
//
//  Note: In keeping with getter's and setter's for TLorentzVector, all HParticle getters
//        and setters begin with a capital letter ('GetCharge' instead of 'getCharge').
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hparticlesim.h"

ClassImp(HParticleSim)

HParticleSim::HParticleSim(Double_t px, Double_t py, Double_t pz, Double_t m):
  HParticle(px,py,pz,m) {
// Px, Py, Pz (MeV/c), mass (MeV/c**2)
	index = 0;
	weight = 0;
	pid = 0;
	trackNumber = 0;
  }

HParticleSim::HParticleSim(int Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
// Particle id, Px, Py, Pz (MeV/c)
	index = 0;
	weight = 0;
	pid = Pid;
	trackNumber = 0;
  }

HParticleSim::HParticleSim(int tracknum, int Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
// GEANT track number, Particle id, Px, Py, Pz (MeV/c)
	index = 0;
	weight = 0;
	pid = Pid;
	trackNumber = tracknum;
  }

HParticleSim::HParticleSim(const TVector3 &p, Double_t e):
  HParticle(p,e) {}

HParticleSim::HParticleSim(const HParticleSim & p):
  HParticle() {
// copy constructor
        weight = p.weight;
	index = p.index;
	pid = p.pid;
	trackNumber = p.trackNumber;
}

HParticleSim::HParticleSim(const HParticleSim * p):
  HParticle(  ) {
// copy constructor
}

HParticleSim & HParticleSim::operator = ( const HParticleSim &p ) {
  // assignment
  SetVect4(p.Vect4());
  SetTrackNumber(p.trackNumber);
  SetIndex(p.index);
  SetWeight(p.weight);
  SetPid(p.pid);
  return *this;
}


