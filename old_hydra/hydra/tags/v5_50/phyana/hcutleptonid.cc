//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

/////////////////////////////////////////////////////////////////////////
//
//  HCutLeptonId - cuts on track/rich or particle/rich combinations
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <TMath.h>
#include "phyanadef.h"
#include "hcutleptonid.h"
#include "hcut.h"
#include "hkicktrack.h"
#include "kickdef.h"
#include "hparticle.h"
#include "hrichhit.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HCutLeptonId::HCutLeptonId() : HCut() {
  // Default constructor
  kFlagTofinoTof = kFALSE;
  kFlagTofTof = kFALSE;
  kFlagShowerSum = kFALSE;
  kFlagThetaDiff = kFALSE;
  kFlagPhiDiff = kFALSE;
  kFlagRingPatMat = kFALSE;
  kFlagRingHouTra = kFALSE;
}

HCutLeptonId::HCutLeptonId(const Text_t *name, const Text_t *title)
         : HCut(name, title) {
  kFlagTofinoTof = kFALSE;
  kFlagTofTof = kFALSE;
  kFlagShowerSum = kFALSE;
  kFlagThetaDiff = kFALSE;
  kFlagPhiDiff = kFALSE;
  kFlagRingPatMat = kFALSE;
  kFlagRingHouTra = kFALSE;
}

void HCutLeptonId::reset() {
// Disables all cuts by resetting to kFALSE
  kFlagTofinoTof = kFALSE;
  kFlagTofTof = kFALSE;
  kFlagShowerSum = kFALSE;
  kFlagThetaDiff = kFALSE;
  kFlagPhiDiff = kFALSE;
  kFlagRingPatMat = kFALSE;
  kFlagRingHouTra = kFALSE;
  return;
}

void HCutLeptonId::setStandardCuts() {
// Not implemented yet
  return;
}

void HCutLeptonId::printCutList() {
// Prints a table of enabled cuts to stdio
  printf("\n--------------------------------------------\n");
  printf("               HCutLeptonId cuts               \n");
  printf("               ------------------               \n");

  if(kFlagTofinoTof)
     printf("fMinTofinoTof = %4.1f  fMaxTofinoTof = %4.1f\n",fMinTofinoTof,fMaxTofinoTof);
  if(kFlagTofTof)
     printf("fMinTofTof    = %4.1f  fMaxTofTof    = %4.1f\n",fMinTofTof,fMaxTofTof);
  if(kFlagShowerSum)
     printf("fShowerSum10  = %4.1f  fShowerSum20  = %4.1f\n",fShowerSum10,fShowerSum20);
  if(kFlagThetaDiff)
     printf("fMinThetaDiff = %4.1f  fMaxThetaDiff = %4.1f\n",fMinThetaDiff,fMaxThetaDiff);
  if(kFlagPhiDiff)
     printf("fMinPhiDiff   = %4.1f  fMaxPhiDiff   = %4.1f\n",fMinPhiDiff,fMaxPhiDiff);
  if(kFlagRingPatMat)
     printf("fMinRingPatMat= %4.1f  fMaxRingPatMat= %4.1f\n",fMinRingPatMat,fMaxRingPatMat);
  if(kFlagRingHouTra)
     printf("fMinRingHouTra= %4.1f  fMaxRingHouTra= %4.1f\n",fMinRingHouTra,fMaxRingHouTra);

  printf("--------------------------------------------\n\n");
  return;
}

Int_t HCutLeptonId::check(HRichHit *ring, HParticle *part) {
  // Checks one particle and ring and returns 1 if cuts are satisfied
  Float_t r2d = 57.29578;
  fNCheck++;

  // Get HKickTrack object from HParticle trackId
  HCategory *trackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
  HKickTrack *track = (HKickTrack*) trackCat->getObject( part->getTrackId() );

  // Time-of-flight check on track
  Float_t tof = track->getTof();

  if(kFlagTofinoTof) {
    if( track->getSystem() == 0 &&
         (tof>fMaxTofinoTof || tof<fMinTofinoTof) ) {
      fnTofinoTof++;
      return kFALSE;
    }
  }

  if(kFlagTofTof) {
    if( track->getSystem() == 1 &&
         (tof>fMaxTofTof || tof<fMinTofTof) ) {
      fnTofTof++;
      return kFALSE;
    }
  }

  // Ring quality check
  if(kFlagRingHouTra) {
    if( ring->iRingHouTra < fMinRingHouTra ||
        ring->iRingHouTra > fMaxRingHouTra ) {
      fnRingHouTra++;
      return kFALSE;
    }
  }

  if(kFlagRingPatMat) {
    if( ring->iRingPatMat < fMinRingPatMat ||
        ring->iRingPatMat > fMaxRingPatMat ) {
      fnRingPatMat++;
      return kFALSE;
    }
  }

  // Shower multiplication factor check on track
  if(kFlagShowerSum) {
    if( track->getSystem() == 0 &&
         ( track->getShowerSum10(1.) < fShowerSum10 ||
           track->getShowerSum20(1.) < fShowerSum20 ) ) {
      fnShowerSum++;
      return kFALSE;
    }
  }

  // Angular correlation check on part - ring
  if(kFlagThetaDiff) {
    Float_t ringTheta = ring->getTheta();
    Float_t partTheta = part->Theta()*r2d;

    if( ringTheta-partTheta < fMinThetaDiff ||
        ringTheta-partTheta > fMaxThetaDiff ) {
      fnThetaDiff++;
      return kFALSE;
    }
  }

  if(kFlagPhiDiff) {
    Float_t ringPhi = ring->getPhi();
    Float_t partPhi = part->Phi()*r2d + (part->Phi()<0.)*360.;

    if( ringPhi-partPhi < fMinPhiDiff ||
        ringPhi-partPhi > fMaxPhiDiff ) {
      fnPhiDiff++;
      return kFALSE;
    }
  }

  fNGood++;
  return kTRUE;
}

Int_t HCutLeptonId::check(HRichHit *ring, HKickTrack *track) {
  // Checks one track and ring and returns 1 if cuts are satisfied
  // Note: tracks are rotated into the cave system simply by rotating
  // phi by multiples of 60.  The correct way (used in HParticleFiller,
  // for example) is to use HSpecGeomPar for the rotation.
  Float_t r2d = 57.29578;
  fNCheck++;

  // Time-of-flight check on track
  Float_t tof = track->getTof();

  if(kFlagTofinoTof) {
    if( track->getSystem() == 0 &&
         (tof>fMaxTofinoTof || tof<fMinTofinoTof) ) {
      fnTofinoTof++;
      return kFALSE;
    }
  }

  if(kFlagTofTof) {
    if( track->getSystem() == 1 &&
         (tof>fMaxTofTof || tof<fMinTofTof) ) {
      fnTofTof++;
      return kFALSE;
    }
  }

  // Ring quality check
  if(kFlagRingHouTra) {
    if( ring->iRingHouTra < fMinRingHouTra ||
        ring->iRingHouTra > fMaxRingHouTra ) {
      fnRingHouTra++;
      return kFALSE;
    }
  }

  if(kFlagRingPatMat) {
    if( ring->iRingPatMat < fMinRingPatMat ||
        ring->iRingPatMat > fMaxRingPatMat ) {
      fnRingPatMat++;
      return kFALSE;
    }
  }

  // Shower multiplication factor check on track
  if(kFlagShowerSum) {
    if( track->getSystem() == 0 &&
         ( track->getShowerSum10(1.) < fShowerSum10 ||
           track->getShowerSum20(1.) < fShowerSum20 ) ) {
      fnShowerSum++;
      return kFALSE;
    }
  }

  // Angular correlation check on track - ring
  if(kFlagThetaDiff) {
    Float_t ringTheta = ring->getTheta();
    Float_t trackTheta = track->getTheta()*r2d;

    if( ringTheta-trackTheta < fMinThetaDiff ||
        ringTheta-trackTheta > fMaxThetaDiff ) {
      fnThetaDiff++;
      return kFALSE;
    }
  }

  if(kFlagPhiDiff) {
    Float_t ringPhi = ring->getPhi();
    Float_t trackPhi = track->getPhi()*r2d + (track->getSector()*60.);
    if(trackPhi > 360.) trackPhi-= 360.;

    if( ringPhi-trackPhi < fMinPhiDiff ||
        ringPhi-trackPhi > fMaxPhiDiff ) {
      fnPhiDiff++;
      return kFALSE;
    }
  }

  fNGood++;
  return kTRUE;
}

ClassImp(HCutLeptonId)
