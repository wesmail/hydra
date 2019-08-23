//*-- AUTHOR : D.Magestro
//*-- Created : 28/05/2001

/////////////////////////////////////////////////////////////////////////
//
// HParticleFiller
//
//  This is a very simple detector matching task which fills the HParticle
//  container by iterating over HKickTrack and HRichHit.
//
//  This task should be used as an example for developing new and adapting
//  existing matching tasks.
//
//  An HParticle is assigned its four-vector and other characteristics from
//  HKickTrack (and rotated into cave coordinate system).  If an HParticle
//  has an angular correlation with an HRichHit, the particle is assigned to
//  be an electron (positron) if the track's charge in HKickTrack was -1 (+1),
//  and its mass is assigned the mass of an electron. Geant particle codes
//  are used: 2 = positron, 3 = electron.
//
//  By calling the task with skip='kTRUE', events without an assigned
//  lepton are skipped.
//
/////////////////////////////////////////////////////////////////////////

#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <TMath.h>
#include "richdef.h"
#include "kickdef.h"
#include "phyanadef.h"
#include "hparticlefiller.h"
#include "hparticle.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hspectrometer.h"
#include "hlinearcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "hrichhit.h"
#include "hkicktrack.h"
#include "hgeomvector.h"
#include "hgeomvolume.h"
#include "hgeomtransform.h"
#include "hspecgeompar.h"
#include "hruntimedb.h"

HParticleFiller::HParticleFiller(void) {
  // Default constructor
  fPartCat=0;
  fKickTrackCat=0;
  fRichHitCat=0;
  iterTracks=0;
  iterRings=0;
  kSkip = kFALSE;
}

HParticleFiller::HParticleFiller(Text_t *name,Text_t *title,Bool_t skip) :
               HReconstructor(name,title) {
  fPartCat=0;
  fKickTrackCat=0;
  fRichHitCat=0;
  iterTracks=0;
  iterRings=0;
  fSpecGeometry=0;
  kSkip = skip;
}

Bool_t HParticleFiller::init(void) {
  // Creates the KickTrack and Particle categories and adds them to
  // the current event.

  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
  if (!fSpecGeometry) fSpecGeometry=(HSpecGeomPar*)gHades->getSetup()->createSpecGeomPar();

  fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
  if (!fKickTrackCat) {
    Error("init","KickTrack not found in input file");
    return kFALSE;
  }

  iterTracks = (HIterator *)fKickTrackCat->MakeIterator("native");

  fRichHitCat = gHades->getCurrentEvent()->getCategory(catRichHit);
  if (!fRichHitCat) return kFALSE;
  iterRings = (HIterator *)fRichHitCat->MakeIterator("native");

  fPartCat=gHades->getCurrentEvent()->getCategory(catParticle);
  if (!fPartCat) {
    fPartCat = new HLinearCategory("HParticle",1000);
//    if (fPartCat) gHades->getCurrentEvent()->addCategory(catParticle,
//          fPartCat,"PhyAna");
    if (fPartCat) gHades->getCurrentEvent()->addCategory(catParticle,
          fPartCat,"Tracks");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  }
  return kTRUE;
}

Int_t HParticleFiller::execute(void) {
  // Fills the Particle container using input from the Kick Plane and Rich Hits.
  // A simple transformation from the sector to the cave coordinate
  // system is also done.


  Float_t px,py,pz,en;
  Float_t ringTheta,ringPhi,trackTheta,trackPhi,trackTof,tofMax;

  HParticle  *part = 0;
  HKickTrack *track = 0;
  HRichHit   *ring = 0;
  HLocation loc;

  iterTracks->Reset();
  Int_t nLeptons = 0;
  Int_t nPart = 0;

  HGeomVector alpha;
  HGeomVector alphaLocal;

  while ((track=(HKickTrack*) iterTracks->Next())!=0) {

    part = (HParticle*) fPartCat->getNewSlot(loc);
    if(part != 0) {
      part = new(part) HParticle;

      HGeomTransform &transSec = fSpecGeometry->getSector( track->getSector() )->getTransform();

      alphaLocal.setX(sin(track->getTheta())*cos(track->getPhi()));
      alphaLocal.setY(sin(track->getTheta())*sin(track->getPhi()));
      alphaLocal.setZ(cos(track->getTheta()));

      alpha = transSec.getRotMatrix() * alphaLocal;

      px = alpha.getX() * track->getP();
      py = alpha.getY() * track->getP();
      pz = alpha.getZ() * track->getP();
      en = sqrt( track->getMass() + px*px + py*py + pz*pz  );

      part->SetPxPyPzE(px,py,pz,en);

      part->SetIndex(nPart);
      part->SetPid(track->getPID());

      // Iterate over Rich Hits... if a correlation exists, change PID
      // FIXME: Remove hard-wired particle codes for e+ and e-

      iterRings->Reset();
      while ((ring=(HRichHit*) iterRings->Next())!=0) {
        trackTheta = part->Theta()*57.296;
        trackPhi = part->Phi()*57.296 + (part->Phi()<0.)*360.;
        ringTheta = ring->getTheta();
        ringPhi = ring->getPhi();

        trackTof = track->getTof();
        if(track->getSystem()==0) tofMax = 10.5;   // TOFINO+Shower (cuts provided
        else tofMax = 8.0;                         // TOF            by W.Koenig)

        if( TMath::Abs(ringTheta-trackTheta) < 3. && TMath::Abs(ringPhi-trackPhi) < 5. &&
                trackTof > 3.0 && trackTof < tofMax) {
          if(track->getCharge() == -1) part->SetPid(3); // electron geant code
          if(track->getCharge() == 1)  part->SetPid(2); // positron geant code
          if(part->GetPid() == 2 || part->GetPid() == 3) {
               nLeptons++;
               part->SetE(sqrt(px*px+py*py+pz*pz+0.511*0.511));  // 4th coordinate of TLorentzVector
          }
        }

      } // Ring iterator

      nPart++;       // Used for partIndex in HParticle

    } // Check to make sure slot exists
  } // Track iterator

  // Skip events without a lepton if kSkip is kTRUE in the task initialization
  if(nLeptons == 0 && kSkip == kTRUE) return kSkipEvent;
  return 0;
}

ClassImp(HParticleFiller)
