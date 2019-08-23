//*-- Author : Dan Magestro
//*-- Created: 26/05/01
//*-- Last modified: 07/11/01
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  HParticleSim - Data container for simulated identified particles
//
//  Currently, HParticleSim is simply an HParticle. Geant track number is
//  not propagated due to possibility of multiple Geant tracks contributing
//  to one particle. Indices to hits/tracks must be used to access Geant info.
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
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hparticlesim.h"

ClassImp(HParticleSim)

HParticleSim::HParticleSim(Double_t px, Double_t py, Double_t pz, Double_t m):
  HParticle(px,py,pz,m) {
// Px, Py, Pz (MeV/c), mass (MeV/c**2)
	weight = 0;
  }

HParticleSim::HParticleSim(Int_t Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
// Particle id, Px, Py, Pz (MeV/c)
	weight = 0;
  }

HParticleSim::HParticleSim(Char_t *Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
	weight = 0;
  }

HParticleSim::HParticleSim(const TVector3 &p, Double_t e):
  HParticle(p,e) {}

HParticleSim::HParticleSim(const HParticleSim & p):
  HParticle() {
// copy constructor
        weight = p.weight;
	index = p.index;
	pid = p.pid;
}

HParticleSim::HParticleSim(const HParticleSim * p):
  HParticle(  ) {
// copy constructor
}

HParticleSim & HParticleSim::operator = ( const HParticleSim &p ) {
  // assignment
  setVect4(p.Vect4());
  setIndex(p.index);
  setWeight(p.weight);
  setPid(p.pid);
  return *this;
}


