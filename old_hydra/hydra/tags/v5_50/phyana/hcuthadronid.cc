//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

/////////////////////////////////////////////////////////////////////////
//
// HCutHadronId - cuts to identify hadrons
//
//  This class includes a set of TCutG data members for making two-
//  dimensional cuts for purposes of identifying hadrons.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////

#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <TMath.h>
#include "hcuthadronid.h"
#include "hphysicsconstants.h"
#include "hkicktrack.h"

HCutHadronId::HCutHadronId() : HCut() {
// Default constructor

  piPlusCut = 0;
  piPlusCut = 0;
  piMinusCut = 0;
  pionCut = 0;
  kPlusCut = 0;
  kMinusCut = 0;
  kaonCut = 0;
  protonCut = 0;
  deuteronCut = 0;

  reset();
}

HCutHadronId::HCutHadronId(const Text_t *name, const Text_t *title)
         : HCut(name, title) {
// Default constructor
  piPlusCut = 0;
  piPlusCut = 0;
  piMinusCut = 0;
  pionCut = 0;
  kPlusCut = 0;
  kMinusCut = 0;
  kaonCut = 0;
  protonCut = 0;
  deuteronCut = 0;

  reset();
}

void HCutHadronId::reset() {
// Disables all cuts by resetting to kFALSE
  kFlagTofinoTof = kFALSE;
  kFlagTofTof = kFALSE;
  kFlagP = kFALSE;
  return;
}

void HCutHadronId::setStandardCuts() {
// Not implemented yet
  return;
}

void HCutHadronId::printCutList() {
  // Prints list of defined cuts
  printf("\n--------------------------------------------\n");
  printf("               HCutHadronId cuts               \n");
  printf("               ------------------               \n");

  if(kFlagP)
     printf("fMinP         = %5.1f   fMaxP         = %5.1f\n",fMinP,fMaxP);
  if(kFlagTofinoTof)
     printf("fMinTofinoTof = %4.1f  fMaxTofinoTof = %4.1f\n",fMinTofinoTof,fMaxTofinoTof);
  if(kFlagTofTof)
     printf("fMinTofTof    = %4.1f  fMaxTofTof    = %4.1f\n",fMinTofTof,fMaxTofTof);

  printf("--------------------------------------------\n\n");
  return;
}

void HCutHadronId::setCutWindow(Int_t pid, TCutG *cut) {
  //
   if( pid == HPhysicsConstants::pid("p") )
      protonCut = cut;
   if( pid == HPhysicsConstants::pid("pi+") )
      piPlusCut = cut;
   if( pid == HPhysicsConstants::pid("pi-") )
      piMinusCut = cut;
   if( pid == HPhysicsConstants::pid("K+") )
      kPlusCut = cut;
   if( pid == HPhysicsConstants::pid("K-") )
       kMinusCut = cut;
   if( pid == HPhysicsConstants::pid("d") )
      deuteronCut = cut;
   if( pid == HPhysicsConstants::pid("pion") )
      pionCut = cut;
   if( pid == HPhysicsConstants::pid("kaon") )
      kaonCut = cut;
}

void HCutHadronId::setCutWindow(Char_t *pid, TCutG *cut) {
   setCutWindow(HPhysicsConstants::pid(pid),cut);
   return;
}

Int_t HCutHadronId::getPid(HKickTrack *track) {

  Float_t p,tof;
  Int_t charge;
  Float_t pCharge;

  p = track->getP();
  tof = track->getTof();
  charge = track->getCharge();
  pCharge = (float) p*charge;

  if(pionCut && pionCut->IsInside(tof,p) ) {
     if(charge == -1) return HPhysicsConstants::pid("pi-");
     else if(charge == 1) return HPhysicsConstants::pid("pi+");
     else return HPhysicsConstants::pid("pion");
  }
  if(piMinusCut && piMinusCut->IsInside(tof,pCharge)) return HPhysicsConstants::pid("pi-");
  if(piPlusCut && piPlusCut->IsInside(tof,pCharge)) return HPhysicsConstants::pid("pi+");

  if(kaonCut && kaonCut->IsInside(tof,p)) {
     if(charge == -1) return HPhysicsConstants::pid("K-");
     else if(charge == 1) return HPhysicsConstants::pid("K+");
     else return HPhysicsConstants::pid("kaon");
  }
  if(kMinusCut && kMinusCut->IsInside(tof,pCharge)) return HPhysicsConstants::pid("K-");
  if(kPlusCut && kPlusCut->IsInside(tof,pCharge)) return HPhysicsConstants::pid("K+");

  if(protonCut && protonCut->IsInside(tof,p)) return HPhysicsConstants::pid("p");
  if(deuteronCut && deuteronCut->IsInside(tof,pCharge)) return HPhysicsConstants::pid("d");

  return 0;
}

void HCutHadronId::Draw() {
   if(piPlusCut) piPlusCut->Draw();
   if(piMinusCut) piMinusCut->Draw();
   if(pionCut) pionCut->Draw();
   if(kPlusCut) kPlusCut->Draw();
   if(kMinusCut) kMinusCut->Draw();
   if(kaonCut) kaonCut->Draw();
   if(protonCut) protonCut->Draw();
   if(deuteronCut) deuteronCut->Draw();
}

Int_t HCutHadronId::check(HKickTrack *track) {
  fNCheck++;

  // Time-of-flight check on track
  Float_t tof = track->getTof();

  if( kFlagP && ( track->getP() > fMaxP || track->getP() < fMinP) ) {
    fnP++;
    return kFALSE;
  }

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

  fNGood++;
  return kTRUE;
}

ClassImp(HCutHadronId)
