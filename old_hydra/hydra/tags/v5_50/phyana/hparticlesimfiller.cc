//*-- AUTHOR: D.Magestro
//*-- Created: 28/05/01
//*-- Modified: 07/11/01
/////////////////////////////////////////////////////////////////////////
//
//  HParticleSimFiller - Filler for simulated particles
//
//  This task fills HParticleSim with the same method as used in
//  HParticleFiller. In addition, HParticleSrc is filled if HGeantKine
//  category exists in the input.
//
//  By calling the task with skip='kTRUE', events without an HParticleSim
//  are skipped.  Currently, tracks from HKickTrack which do not have a
//  valid pid are kept and assigned pid=0 (as in HKickTrack).
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <math.h>
#include <time.h>
#include <TMath.h>
#include "hades.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hevent.h"
#include "hiterator.h"
#include "hkicktrack.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hpartialevent.h"
#include "hparticlesim.h"
#include "hparticlesimfiller.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hrichhitsim.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "kickdef.h"
#include "phyanadef.h"
#include "richdef.h"
#include "hcuttrack.h"
#include "hcutleptonid.h"
#include "hcuthadronid.h"
#include "hparticlesrc.h"
#include "hgeantkine.h"

HParticleSimFiller::HParticleSimFiller(void) {
  // Default constructor
  fGeantKineCat=0;
  fPartSrcCat=0;
  iterGeant=0;
}

HParticleSimFiller::HParticleSimFiller(Text_t *name,Text_t *title,Bool_t skip) :
               HParticleFiller(name,title,skip) {
  fGeantKineCat=0;
  fPartSrcCat=0;
  iterGeant=0;
}

Bool_t HParticleSimFiller::init(void) {
  // Creates the KickTrack and Particle categories and adds them to
  // the current event.

  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

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
    fPartCat = new HLinearCategory("HParticleSim",1000);
    if (fPartCat) gHades->getCurrentEvent()->addCategory(catParticle,
          fPartCat,"PhyAna");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  }

  // Set up HGeantKine and HParticleSrc classes (if HGeantKine exists)
  fGeantKineCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  if (fGeantKineCat) {
    iterGeant = (HIterator *)fGeantKineCat->MakeIterator("native");
    fPartSrcCat=gHades->getCurrentEvent()->getCategory(catParticleSrc);
    if (!fPartSrcCat) {
      fPartSrcCat = new HLinearCategory("HParticleSrc",1000);
    if (fPartSrcCat) gHades->getCurrentEvent()->addCategory(catParticleSrc,
          fPartSrcCat,"PhyAna");
      else {
        Error("init","Unable to setup output");
        return kFALSE;
      }
    }
  }

  return kTRUE;
}

Int_t HParticleSimFiller::execute(void) {
  // Fills the Particle container using input from the Kick Plane and Rich Hits.
  // A simple transformation from the sector to the cave coordinate
  // system is also done to HKickTrack.

  HParticleSim  *part = 0;
  HKickTrack *track = 0;
  HRichHitSim *ring = 0;
  HLocation loc;

  Int_t nPart = 0;
  Int_t nLeptons = 0;

  iterTracks->Reset();
  while ((track=(HKickTrack*) iterTracks->Next())!=0) {

    // STEP 1: Apply global cuts to tracks (p, tof, geometrical, etc.)
    if(trackCuts && !trackCuts->check(track)) continue;

    part = (HParticleSim*) fPartCat->getNewSlot(loc);
    if(part != 0) {

      // Make HParticle from track, fill everything but pid
      part = new(part) HParticleSim;

      part->setTrackId( fKickTrackCat->getIndex(track) );
      part->setIndex(nPart);

      fillMomenta(track,part);
      fillVertex(track,part);

      // STEP 2: Check for leptons
      // Identify leptons by iterating over Rich Hits... change PID,
      // recalculate E.  For now, make ALL leptons which pass cuts.
      iterRings->Reset();
      while ((ring=(HRichHitSim*) iterRings->Next())!=0) {

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

  // If HGeantKine exists in input, fill HParticleSrc
  if(fGeantKineCat) fillParticleSrc();

  // Skip events without an HParticle if kSkip is kTRUE in the task initialization
  if(nPart == 0 && kSkip == kTRUE) return kSkipEvent;

  return 0;
}

void HParticleSimFiller::fillParticleSrc(void) {
// If HGeantKine exists in the input, this function fills HParticleSrc with
// particle properties from HGeantKine. Currently, source particles (parent
// track = 0) and leptons from dalitz and pi-0 conversion processes are kept.

  Int_t nPart;
  Int_t iParent,iMedium,iMech,iTrack,iPid,iTrackParent,iPidParent;
  Int_t iParentParent,iMediumParent,iMechParent;
  Bool_t isFromSource,isDetectable;
  Float_t px,py,pz,en,mass;
  Float_t x,y,z,r,theta,phi;
  nPart = 0;
  HGeantKine *kine = 0;
  HGeantKine *kineParent = 0;
  HParticleSrc *partSrc = 0;
  HLocation loc;

  iterGeant->Reset();
  while ((kine=(HGeantKine*) iterGeant->Next())!=0) {

    isFromSource = isDetectable = kFALSE;

    kine->getParticle(iTrack,iPid);
    kine->getCreator(iParent,iMedium,iMech);

    if(iParent == 0) isFromSource = kTRUE;

    if(iPid != HPhysicsConstants::pid("g")
          && iPid != HPhysicsConstants::pid("n") )
      isDetectable = kTRUE;

    if(iPid == HPhysicsConstants::pid("e-")
          || iPid == HPhysicsConstants::pid("e+") ) {

      // If lepton's parent is eta or pi-0, assign it as being from 'source'
      // The '-1' in the getObject() comes from the convention that track
      //    numbering begins with 1 (due to GEANT being written in Fortran?)
      kineParent =(HGeantKine*) fGeantKineCat->getObject(iParent-1);
      kineParent->getParticle(iTrackParent,iPidParent);
      kineParent->getCreator(iParentParent,iMediumParent,iMechParent);

      if(iPidParent == HPhysicsConstants::pid("eta")
          || iPidParent == HPhysicsConstants::pid("pi0") )
        isFromSource = kTRUE;

      // If lepton's parent is gamma and gamma's parent is pi-0,
      //    assign it as being from 'source'
      // The '-1' in the getObject() comes from the convention that track
      //    numbering begins with 1 (due to GEANT being written in Fortran?)
      if(iPidParent == HPhysicsConstants::pid("g") && iParentParent!= 0) {
        kineParent =(HGeantKine*) fGeantKineCat->getObject(iParentParent-1);
        kineParent->getParticle(iTrackParent,iPidParent);

        if(iPidParent == HPhysicsConstants::pid("pi0") ) {
          isFromSource = kTRUE;
        }
      }
    }

    if(isFromSource && isDetectable) {

      partSrc = (HParticleSrc*) fPartSrcCat->getNewSlot(loc);
      if(partSrc != 0) {

        partSrc = new(partSrc) HParticleSrc;

        partSrc->setIndex(nPart);
        partSrc->setPid(iPid);
        partSrc->setTrackNumber(iTrack);

        // Fill momentum coordinates
        kine->getMomentum(px,py,pz);
        mass = HPhysicsConstants::mass(iPid);
        en = sqrt( px*px + py*py + pz*pz + mass*mass);
        partSrc->SetPxPyPzE(px,py,pz,en);

        // Fill vertex coordinates (equations from MDC data levels doc.)
        kine->getVertex(x,y,z);
        theta = partSrc->Theta();
        phi = partSrc->Phi();
        r = y*cos(phi) - x*sin(phi);
        z = - ( x*cos(phi) + y*sin(phi) ) * cos(theta)/sin(theta) ;
        partSrc->setR(r);
        partSrc->setZ(z);

        nPart++; // Used for partIndex in HParticleSim
      }
    }
  }
  return;
}

ClassImp(HParticleSimFiller)
