//*-- Author : Dan Magestro
//*-- Created: 10/10/01
//*-- Last modified: 07/11/01
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  HParticleSrc - Data container for Geant tracks at particle level
//
//  The purpose of HParticleSrc is to store particle parameters before
//  they have been digitized. See the documentation (link below).
//
//  This class derives from HParticle, which in turn derives from
//  TLorentzVector, so that all of the functions available to
//  TLorentzVector can be performed on HParticle objects:
//
//      HParticleSrc *part1 = new HParticleSrc(50.,50.,50.,938.);
//
//      HParticleSrc *part2 = new HParticleSrc(30.,30.,30.,938.);
//
//      Float_t pxSum = part1.Px() + part2.Px()
//      .....
//
//  HParticleSrc's can be initialized as follows:
//      part = new HParticleSrc();
//      part = new HParticleSrc(Double_t px, Double_t py, Double_t pz);
//      part = new HParticleSrc(Double_t px, Double_t py, Double_t pz, Double_t m);
//      part = new HParticleSrc(Int_t Pid, Double_t px, Double_t py, Double_t pz);
//      part = new HParticleSrc(Int_t trackNumber, Int_t Pid, Double_t px, Double_t py, Double_t pz);
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hparticlesrc.h"

ClassImp(HParticleSrc)

HParticleSrc::HParticleSrc(Double_t px, Double_t py, Double_t pz, Double_t m):
  HParticle(px,py,pz,m) {
// Px, Py, Pz (MeV/c), mass (MeV/c**2)
	weight = 0;
	trackNumber = 0;
  }

HParticleSrc::HParticleSrc(int Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
// Particle id, Px, Py, Pz (MeV/c)
	weight = 0;
	trackNumber = 0;
  }

HParticleSrc::HParticleSrc(Char_t *Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
	weight = 0;
        trackNumber = 0;
  }

HParticleSrc::HParticleSrc(int tracknum, int Pid, Double_t px, Double_t py, Double_t pz):
  HParticle(Pid,px,py,pz) {
// GEANT track number, Particle id, Px, Py, Pz (MeV/c)
	index = 0;
	weight = 0;
	trackNumber = tracknum;
  }

HParticleSrc::HParticleSrc(const TVector3 &p, Double_t e):
  HParticle(p,e) {}

HParticleSrc::HParticleSrc(const HParticleSrc & p):
  HParticle() {
// copy constructor
        weight = p.weight;
	index = p.index;
	pid = p.pid;
	trackNumber = p.trackNumber;
}

HParticleSrc::HParticleSrc(const HParticleSrc * p):
  HParticle(  ) {
// copy constructor
}

HParticleSrc & HParticleSrc::operator = ( const HParticleSrc &p ) {
  // assignment
  setVect4(p.Vect4());
  setTrackNumber(p.trackNumber);
  setIndex(p.index);
  setWeight(p.weight);
  setPid(p.pid);
  return *this;
}


