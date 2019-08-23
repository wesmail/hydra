//*-- AUTHOR : D.Magestro
//*-- Created : 10/06/2001

/////////////////////////////////////////////////////////////////////
//
// HParticleSimFiller
//
// This is a temporary task which fills the HParticleSim container
// by iterating over HKickTrack and (eventually) HRichHits.  This
// placeholder class should be replaced by a more advanced algorithm.
//
/////////////////////////////////////////////////////////////////////

#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include "kickdef.h"
#include "phyanadef.h"
#include "hparticlesimfiller.h"
#include "hparticlesim.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "hkicktrack.h"
#include "hgeomvector.h"
#include "hgeomvolume.h"
#include "hgeomtransform.h"
#include "hspecgeompar.h"
#include "hruntimedb.h"

HParticleSimFiller::HParticleSimFiller(void) {
  // Default constructor
  fPartCat=0;
  fKickTrackCat=0;
  iter=0;
}

HParticleSimFiller::HParticleSimFiller(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {
  fPartCat=0;
  fKickTrackCat=0;
  fSpecGeometry=0;
  iter=0;
}

Bool_t HParticleSimFiller::init(void) {
  // Creates the KickTrack and Particle categories and adds them to
  // the current event.

  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

  fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
  if (!fKickTrackCat) return kFALSE;

  iter = (HIterator *)fKickTrackCat->MakeIterator("native");

  fPartCat=gHades->getCurrentEvent()->getCategory(catParticle);
  if (!fPartCat) {
    fPartCat = new HLinearCategory("HParticleSim",1000);
    if (fPartCat) gHades->getCurrentEvent()->addCategory(catParticle,
          fPartCat,"Particles");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  }
  return kTRUE;
}

Int_t HParticleSimFiller::execute(void) {
  // Fills the Particle container using input from the Kick Plane.
  // A simple transformation from the <b>sector</b> to the <b>cave</b> coordinate
  // system is also done.

  Float_t px,py,pz,en;

  HParticleSim  *part = 0;
  HKickTrack *track = 0;
  HLocation loc;

  iter->Reset();

  while ((track=(HKickTrack*) iter->Next())!=0) {

    part = (HParticleSim*) fPartCat->getNewSlot(loc);
    part = new(part) HParticleSim;

    HGeomVector alpha;
    HGeomVector alphaLocal;
    HGeomTransform &transSec = fSpecGeometry->getSector( track->getSector() )->getTransform();

    alphaLocal.setX(sin(track->getTheta())*cos(track->getPhi()));
    alphaLocal.setY(sin(track->getTheta())*sin(track->getPhi()));
    alphaLocal.setZ(cos(track->getTheta()));

    alpha = transSec.getRotMatrix() * alphaLocal;

    px = alpha.getX() * track->getP();
    py = alpha.getY() * track->getP();
    pz = alpha.getZ() * track->getP();
    en = sqrt( track->getMass() + px*px+py*py+pz*pz  );

//  px = track->getP() * sin(track->getTheta()) * cos(track->getPhi());
//  py = track->getP() * sin(track->getTheta()) * sin(track->getPhi());
//  pz = track->getP() * cos(track->getTheta());

      part->SetPxPyPzE(px,py,pz,en);

      part->SetCharge(track->getCharge());
      part->SetPid(track->getPID());
  }
  return 0;
}

ClassImp(HParticleSimFiller)
