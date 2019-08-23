//*-- AUTHOR: D.Magestro
//*-- Created: 28/05/01
//*-- Modified: 04/09/01
//*-- Modified: 06/10/01

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HParticleFiller - Identifies particles using HCut classes
//
//  This task fills the HParticle container by iterating over HKickTrack
//  and HRichHit and identifying leptons with an angular correlation.  This
//  class is still evolving... further pid and matching tools (showers,
//  p.c. analysis, etc.) are being added.
//
//  An HParticle is assigned its four-momentum compoments and other
//  characteristics from HKickTrack (and rotated into cave coordinate
//  system).  If an HParticle has an angular correlation with an HRichHit,
//  the particle is assigned to be an electron (positron) if the track's
//  charge in HKickTrack was -1 (+1).  2-d cuts are then applied to assign
//  pid to hadrons.
//
//  By calling the task with skip='kTRUE', events without an HParticle
//  are skipped.  Currently, tracks from HKickTrack which do not have a
//  valid pid are kept and assigned pid=0 (as in HKickTrack).
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////
using namespace std;
#include <iostream> 
#include <iomanip>
#include <math.h>
#include <time.h>
#include "TMath.h"
#include "hades.h"
#include "hcategory.h"
#include "hcutleptonid.h"
#include "hcuthadronid.h"
#include "hcuttrack.h"
#include "hdebug.h"
#include "hevent.h"
#include "hgeomvector.h"
#include "hgeomvolume.h"
#include "hgeomtransform.h"
#include "hiterator.h"
#include "hkicktrack.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hpartialevent.h"
#include "hparticle.h"
#include "hparticlefiller.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hrichhit.h"
#include "hruntimedb.h"
#include "hspecgeompar.h"
 #include "hspectrometer.h"
#include "kickdef.h"
#include "phyanadef.h"
#include "richdef.h"

HParticleFiller::HParticleFiller(void) {
// Default constructor
  fPartCat=0;
  fKickTrackCat=0;
  fRichHitCat=0;
  iterTracks=0;
  iterRings=0;
  kSkip = kFALSE;
  trackCuts = 0;
  leptonCuts = 0;
  hadronIdCuts = 0;
}

HParticleFiller::HParticleFiller(Text_t *name,Text_t *title,Bool_t skip) :
               HReconstructor(name,title) {
// Consructor: Name, title, Skip event
  fPartCat=0;
  fKickTrackCat=0;
  fRichHitCat=0;
  iterTracks=0;
  iterRings=0;
  fSpecGeometry=0;
  kSkip = skip;
  trackCuts = 0;
  leptonCuts = 0;
  hadronIdCuts = 0;
}

Bool_t HParticleFiller::init(void) {
  // Creates/gets the KickTrack, RichHit and Particle categories in
  // the current event

  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

  fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
  if (!fKickTrackCat) {
    Error("init","KickTrack not found in input file");
    return kFALSE;
  }
  iterTracks = (HIterator *)fKickTrackCat->MakeIterator("native");

  fRichHitCat = gHades->getCurrentEvent()->getCategory(catRichHit);
  if (!fRichHitCat) {
    Error("init","RichHit not found in input file");
    return kFALSE;
  }
  iterRings = (HIterator *)fRichHitCat->MakeIterator("native");

  fPartCat=gHades->getCurrentEvent()->getCategory(catParticle);
  if (!fPartCat) {
    fPartCat = new HLinearCategory("HParticle",1000);
    if (fPartCat) gHades->getCurrentEvent()->addCategory(catParticle,
          fPartCat,"PhyAna");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  }
  return kTRUE;
}

Int_t HParticleFiller::execute(void) {
  // Fills the Particle container using input from HKickTrack and HRichHit.
  HParticle  *part = 0;
  HKickTrack *track = 0;
  HRichHit   *ring = 0;

  HLocation loc;

  Int_t nLeptons = 0;
  Int_t nPart = 0;

  iterTracks->Reset();
  while ((track=(HKickTrack*) iterTracks->Next())!=0) {

    // STEP 1: Apply global cuts to tracks (p, tof, geometrical, etc.)
    if(trackCuts && !trackCuts->check(track)) continue;

    part = (HParticle*) fPartCat->getNewSlot(loc);
    if(part != 0) {

      // Make HParticle from track, fill everything but pid
      part = new(part) HParticle;

      part->setTrackId( fKickTrackCat->getIndex(track) );
      part->setIndex(nPart);

      fillMomenta(track,part);
      fillVertex(track,part);

      // STEP 2: Check for leptons
      // Identify leptons by iterating over Rich Hits... change PID,
      // recalculate E.  For now, make ALL leptons which pass cuts.
      iterRings->Reset();
      while ((ring=(HRichHit*) iterRings->Next())!=0) {

        if(leptonCuts && !leptonCuts->check(ring,part)) continue;
        part->setRingId(fRichHitCat->getIndex(ring));
        makeLepton(ring,part);
        nLeptons++;
      } // Ring iterator

      // STEP 3: Assign hadron PIDs from graphical cuts in p-tof spectra if
      //         particle wasn't identified as a lepton
      if( !part->getPid() ) {
        if(hadronIdCuts) part->setPid( hadronIdCuts->getPid(track) );
        else part->setPid( track->getPID() );
      }

      // Recalculate energy component of HParticle using identified
      //    particle mass (if it's identified)
      fillMomenta(track,part);

      nPart++; // Used for partIndex in HParticle
    }
  } // Track iterator

  // Skip events without an HParticle if kSkip is kTRUE in the task initialization
  if(nPart == 0 && kSkip == kTRUE) return kSkipEvent;
  return 0;
}

void HParticleFiller::fillMomenta(HKickTrack *track, HParticle *part) {
  // Fill components of 4-vector by rotating track into cave and
  // calculating the coordinates.  If the particle has a PID, mass is
  // calculated using particle mass from look-up table.  Otherwise,
  // mass from HKickTrack is used.

  Float_t px,py,pz,en,mass;

  HGeomVector alpha;
  HGeomVector alphaLocal;

  HGeomTransform &transSec = fSpecGeometry->getSector( track->getSector() )->getTransform();

  alphaLocal.setX(sin(track->getTheta())*cos(track->getPhi()));
  alphaLocal.setY(sin(track->getTheta())*sin(track->getPhi()));
  alphaLocal.setZ(cos(track->getTheta()));

  alpha = transSec.getRotMatrix() * alphaLocal;

  // ------ Momentum recalculation for
  // ------ identified hadrons should go here

  px = alpha.getX() * track->getP();
  py = alpha.getY() * track->getP();
  pz = alpha.getZ() * track->getP();

  if(part->getPid()) mass = HPhysicsConstants::mass(part->getPid());
  else mass = sqrt(track->getMass());

  en = sqrt( mass*mass + px*px + py*py + pz*pz );

  part->SetPxPyPzE(px,py,pz,en);
}

void HParticleFiller::fillVertex(HKickTrack *track, HParticle *part) {
  // For now, this function simply fills HParticle's r and z directly
  // from HKickTrack.  Eventually a vertex class could be implemented.
  part->setR(track->getR());
  part->setZ(track->getZ());

}

void HParticleFiller::makeLepton(HRichHit *ring, HParticle *part) {
  // Make a lepton out of an HParticle by reassigning mass and
  // recalculating energy
  Float_t mass;

  HKickTrack *track = (HKickTrack*) fKickTrackCat->getObject(part->getTrackId());
  if(track->getCharge() == 1)  part->setPid("e+");
  if(track->getCharge() == -1) part->setPid("e-");

  mass = HPhysicsConstants::mass("e-");
  part->SetE(sqrt( part->P()*part->P() + mass*mass ));
}

ClassImp(HParticleFiller)
