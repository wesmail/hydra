//*-- Author : Dan Magestro
//*-- Created: 16/6/2001
///////////////////////////////////////////////////////////////////////////////////////////
//  HDilepton
//
//  Data container for lepton pairs and their 4-momenta.
//
//  This class derives from TLorentzVector, so that all of the functions
//  available to TLorentzVector can be performed on HDilepton objects.
//
//  HDileptons can be e+e- (by default) or like-sign pairs (e+e+ or e-e-).  The type
//  of pair is assigned with the charge data member; a charge of +2 (-2) identifies a
//  pair of positrons (electrons).
//
//  HDilepton's can be initialized as follows:
//      dilept = new HDilepton();
//      dilept = new HDilepton(HParticle &p1, HParticle &p2);
//      dilept = new HDilepton(Double_t px, Double_t py, Double_t pz, Double_t m);
//         (this constructor assigns openingAngle = 0)
//
//  An HDilepton's 4-momenta, openingAngle, and invMass are calculated automatically
//  whenever an HParticle is set or by using the HDilepton constructor with two
//  HParticles.
//
//  Example code:
//
//      HParticle part1(10.,10.,10,0.511);
//      HParticle part2(10.,15.,20,0.511);
//
//      HDilepton dilept(part1,part2);
//      mInv = dilept.M();
//         (and much more....)
//
//  Note: In keeping with getter's and setter's for TLorentzVector, all HDilepton getters
//        and setters begin with a capital letter ('GetCharge' instead of 'getCharge').
//        This will lead to fewer problems when using both inherited and member functions.
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "TLorentzVector.h"
#include "hparticle.h"
#include "hdilepton.h"

ClassImp(HDilepton)

HDilepton::HDilepton(Double_t px, Double_t py, Double_t pz, Double_t m):
  TLorentzVector(px,py,pz, sqrt(px*px+py*py+pz*pz+ m*m ) ) {
// Px, Py, Pz (MeV/c), mass (MeV/c**2)
	charge = 0;
        invMass = M();
        openingAngle = 0;  // Need two HParticles to calculate this
        part1 = 0;
        part2 = 0;
}

HDilepton::HDilepton(const HParticle &p1, const HParticle &p2):
  TLorentzVector() {
// Two HParticles; dilepton's quantities are calculated by constructor
        part1 = p1;
        part2 = p2;
        TLorentzVector partsum;
        partsum = (part1+part2);
        SetVect4(partsum);
        openingAngle = part1.AngleD(part2);
	charge = 0;
        invMass = M();
}

HDilepton::HDilepton(const HDilepton & p):
  TLorentzVector( p.Vect4() ) {
// copy constructor
	SetCharge(p.charge);
	SetPart1Index(p.part1Index);
	SetPart2Index(p.part2Index);
        SetPart1(p.GetPart1());
        SetPart2(p.GetPart2());
        invMass = p.M();
        openingAngle = part1.AngleD(part2);
}

HDilepton::HDilepton(const HDilepton * p):
  TLorentzVector( p->Vect4() ) {
// copy constructor
	SetCharge(p->GetCharge());
	SetPart1Index(p->GetPart1Index());
	SetPart2Index(p->GetPart2Index());
        SetPart1(p->GetPart1());
        SetPart2(p->GetPart2());
        invMass = p->M();
        openingAngle = part1.AngleD(part2);
}


void HDilepton::SetPart1 (const HParticle &p) {
// Each time an HParticle is set, the dilepton's 4-vector and opening angle
// are recalculated
  part1 = p;
  TLorentzVector partsum;
  partsum = part1+part2;
  SetVect4(partsum);
  openingAngle = part1.AngleD(part2);
  invMass = M();
}

void HDilepton::SetPart2 (const HParticle &p) {
// Each time an HParticle is set, the dilepton's 4-vector and opening angle
// are recalculated
  part2 = p;
  TLorentzVector partsum;
  partsum = (part1+part2);
  SetVect4(partsum);
  openingAngle = part1.AngleD(part2);
  invMass = M();
}

HDilepton & HDilepton::operator = ( const HDilepton &p ) {
// Operator needed to correctly return HDilepton
  SetVect4(p.Vect4());
  SetPart1(p.GetPart1());
  SetPart2(p.GetPart2());
  SetCharge(p.charge);
  invMass = p.M();
  openingAngle = part1.AngleD(part2);
  return *this;
}
