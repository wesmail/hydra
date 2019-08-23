//*-- Author : Dan Magestro
//*-- Created: 26/05/01
//*-- Last modified: 07/11/01
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////////////////
//
//  HParticle - Data container for identified particles and their 4-momenta
//
//  This class derives from TLorentzVector, so that all of the functions
//  available to TLorentzVector can be performed on HParticle objects.
//  Some extra care needs to be taken regarding Hydra vs. Root coding
//  conventions ('set' vs. 'Set'), for example
//
//      HParticle *part1 = new HParticle();
//      part1->SetPxPyPzE(1.,1.,1.,10.);     <-- uses TLorentzVector function
//      part1->setPid("e-");                 <-- uses HParticle function
//
//  HParticle's can be initialized as follows:
//
//      part = new HParticle();
//      part = new HParticle(Double_t px, Double_t py, Double_t pz, Double_t m);
//      part = new HParticle(TVector3 p,Double_t E);
//      part = new HParticle(Int_t Pid, Double_t px, Double_t py, Double_t pz);
//      part = new HParticle(Char_t *Pid, Double_t px, Double_t py, Double_t pz);
//
//  For example:
//      HParticle part1("e+",30.,-50.,100.); <-- constructed with pid and momentum
//      HParticle part2("e-",30.,50.,100.);  <-- constructed with pid and momentum
//
//      Float_t openAngle = part1.Angle(part2);  <-- Opening angle between 2 HParticles
//
//      theta = part2->Theta();
//      Float_t pxSum = part1.Px() + part2.Px();
//      .....
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "hparticle.h"
#include "hades.h"
#include "hcategory.h"
#include "kickdef.h"
#include "richdef.h"
#include "hevent.h"

ClassImp(HParticle)

HParticle::HParticle(Double_t px, Double_t py, Double_t pz, Double_t m):
  TLorentzVector(px,py,pz, sqrt(px*px+py*py+pz*pz+ m*m ) ) {
// Px, Py, Pz (MeV/c), mass (MeV/c**2)
	pid = 0;
        r = z = 0;
	index = 0;
        trackId = -1;
        ringId = -1;
        fKickTrackCat = 0;
        fRichHitCat = 0;
  }

HParticle::HParticle(Int_t Pid, Double_t px, Double_t py, Double_t pz):
  TLorentzVector(px,py,pz,0.) {
// Particle id, Px, Py, Pz (MeV/c)
        r = z = 0;
	index = 0;
        trackId = -1;
        ringId = -1;
        fKickTrackCat = 0;
        fRichHitCat = 0;
	pid = Pid;
        Float_t mass = HPhysicsConstants::mass(pid);
        SetE( sqrt(px*px + py*py + pz*pz + mass*mass) );
  }

HParticle::HParticle(Char_t *Pid, Double_t px, Double_t py, Double_t pz):
  TLorentzVector(px,py,pz,0.) {
// Particle name, Px, Py, Pz (MeV/c)
	index = 0;
	pid = HPhysicsConstants::pid(Pid);
        Float_t mass = HPhysicsConstants::mass(Pid);
        SetE( sqrt(px*px + py*py + pz*pz + mass*mass) );
        fKickTrackCat = 0;
        fRichHitCat = 0;
        trackId = -1;
        ringId = -1;
  }


HParticle::HParticle(const TVector3 &p, Double_t e):
  TLorentzVector(p,e) {}

HParticle::HParticle(const HParticle & p):
  TLorentzVector( p.Vect4() ) {
// copy constructor
	setIndex(p.index);
	setPid(p.pid);
        setR(p.r);
        setZ(p.z);
        setTrackId(p.trackId);
        setRingId(p.ringId);
}

HParticle::HParticle(const HParticle * p):
  TLorentzVector( p->Vect4() ) {
// copy constructor
	setIndex( p->getIndex() );
	setPid( p->getPid() );
        setR(p->getR() );
        setZ(p->getZ() );
        setTrackId(p->getTrackId() );
        setRingId(p->getRingId() );
}

HParticle & HParticle::operator = ( const HParticle &p ) {
// assignment operator
  setVect4(p.Vect4());
  setIndex(p.index);
  setPid(p.pid);
  setR(p.r);
  setZ(p.z);
  setTrackId(p.trackId);
  setRingId(p.ringId);
  return *this;
}

HKickTrack* HParticle::getKickTrack() {
// Returns the HKickTrack object corresponding to the particle.  As of now,
// this only works when using Hades object... it won't work in a macro
// which reads a tree.
  HKickTrack *track = 0;
  if(!fKickTrackCat) {
    if(gHades) {
      fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
      if(!fKickTrackCat) return track;  // Returns empty object if no category
    }
// This code doesn't work in its current form
//    else {
//     fKickTrackCat = (HCategory*) gDirectory->Get("HKickTrack");
//    }
  }

  if(trackId < 0) return track;  // Returns empty object if no trackId

  track = (HKickTrack*) fKickTrackCat->getObject(trackId);
  return track;
}

HRichHit* HParticle::getRichHit() {
// Returns the HRichHit object corresponding to the particle. As of now,
// this only works when using Hades object... it won't work in a macro
// which reads a tree.
  HRichHit *ring = 0;
  if(!fRichHitCat) {
    fRichHitCat = gHades->getCurrentEvent()->getCategory(catRichHit);
    if(!fRichHitCat) return ring;  // Returns empty object if no category
  }

  if(ringId < 0) return ring;  // Returns empty object if no trackId

  ring = (HRichHit*) fRichHitCat->getObject(ringId);
  return ring;
}
