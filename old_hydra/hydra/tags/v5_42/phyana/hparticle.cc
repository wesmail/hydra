//*-- Author : Dan Magestro
//*-- Created: 26/05/2001
///////////////////////////////////////////////////////////////////////////////////////////
//  HParticle
//
//  Data container for identified particles and their 4-momenta.
//
//  This class derives from TLorentzVector, so that all of the functions
//  available to TLorentzVector can be performed on HParticle objects:
//
//      HParticle *part1 = new HParticle;
//      part1->SetPxPyPzE(1.,1.,1.,10.);     <-- uses TLorentzVector function
//
//      part2 = new HParticle(2.,3.,5.,.51); <-- constructed with momentum and mass
//
//      TLorentzVector *sum = part1 + part2;               <-- Returns a TLorentzVector
//      part4 = part1.Angle(part2);          <-- TLorentzVector function
//
//      theta = part2->Theta()
//
//      Float_t pxSum = part1.Px() + part2.Px()
//      .....
//
//  HParticle's can be initialized as follows:
//      part = new HParticle();
//      part = new HParticle(TVector3 p,Double_t E);
//      part = new HParticle(Double_t px, Double_t py, Double_t pz, Double_t m);
//      part = new HParticle(Int_t Pid, Double_t px, Double_t py, Double_t pz);
//
//  Note: In keeping with getter's and setter's for TLorentzVector, all HParticle getters
//        and setters begin with a capital letter ('GetCharge' instead of 'getCharge').
//        This will lead to fewer problems when using both inherited and member functions.
//
//  Note #2: The 'wt' and 'detIndex' data members are not implemented yet but will be.
//           The detIndex[] array will be useful for tracing an HParticle's history.
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "hparticle.h"
#include "TLorentzVector.h"

ClassImp(HParticle)

HParticle::HParticle(Double_t px, Double_t py, Double_t pz, Double_t m):
  TLorentzVector(px,py,pz, sqrt(px*px+py*py+pz*pz+ m*m ) ) {
// Px, Py, Pz (MeV/c), mass (MeV/c**2)
	index = 0;
	pid = 0;
  }

HParticle::HParticle(int Pid, Double_t px, Double_t py, Double_t pz):
  TLorentzVector(px,py,pz,0.) {
// Particle id, Px, Py, Pz (MeV/c)
	index = 0;
	pid = Pid;
  }

HParticle::HParticle(const TVector3 &p, Double_t e):
  TLorentzVector(p,e) {}

HParticle::HParticle(const HParticle & p):
  TLorentzVector( p.Vect4() ) {
// copy constructor
	SetIndex(p.index);
	SetPid(p.pid);
}

HParticle::HParticle(const HParticle * p):
  TLorentzVector( p->Vect4() ) {
	SetIndex( p->GetIndex() );
	SetPid( p->GetPid() );
}
// copy constructor

HParticle & HParticle::operator = ( const HParticle &p ) {
  // assignment
  SetVect4(p.Vect4());
  SetIndex(p.index);
  SetPid(p.pid);
  return *this;
}

