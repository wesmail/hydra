//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
// HCutParticle - cuts on HParticles
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////
using namespace std;
#include "TRandom.h"
#include <time.h>
#include <iostream> 
#include <iomanip>
#include <math.h>
#include "TMath.h"
#include "phyanadef.h"
#include "hcutparticle.h"
#include "hcut.h"
#include "hparticle.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HCutParticle::HCutParticle() : HCut() {
  // Default constructor
  fPartCat = 0;
  iterPart=0;

  reset();
}

HCutParticle::HCutParticle(const Text_t *name, const Text_t *title)
         : HCut(name, title) {
  fPartCat = 0;
  iterPart = 0;

  reset();
}

void HCutParticle::setStandardCuts() {
// Not implemented yet
  return;
  }

void HCutParticle::reset() {
// Disables all cuts by resetting to kFALSE
  kFlagCharge = kFALSE;
  kFlagP = kFALSE;
  kFlagPt = kFALSE;
  kFlagTheta = kFALSE;
  kFlagPhi = kFALSE;
  kFlagRapidity = kFALSE;
  return;
  }

void HCutParticle::printCutList() {
// Prints a table of enabled cuts to stdio
  printf("\n--------------------------------------------\n");
  printf("               HCutParticle cuts              \n");
  printf("               -----------------              \n");

  if(kFlagCharge)
     printf("fCharge = %5i   \n",fCharge);
  if(kFlagP)
     printf("fMinP          = %5.1f   fMaxP          = %5.1f\n",fMinP,fMaxP);
  if(kFlagPt)
     printf("fMinPt         = %5.1f   fMaxPt         = %5.1f\n",fMinPt,fMaxPt);
  if(kFlagRapidity)
     printf("fMinRapidity   = %5.1f   fMaxRapidity   = %5.1f\n",fMinRapidity,fMaxRapidity);
  if(kFlagTheta)
     printf("fMinTheta      = %5.1f   fMaxTheta      = %5.1f\n",fMinTheta,fMaxTheta);
  if(kFlagPhi)
     printf("fMinPhi        = %5.1f   fMaxPhi        = %5.1f\n",fMinPhi,fMaxPhi);

  printf("--------------------------------------------\n\n");
  return;
  }

Int_t HCutParticle::check(HParticle *part) {
// Checks one HParticle and returns 1 if particle passes cuts
  Float_t rad2deg = 57.29578;
  fNCheck++;

  if( kFlagCharge && (part->getCharge() != fCharge) ) {
    if(fVerbose) printf("Particle not accepted (Charge cut).\n");
    fnCharge++;
    return kFALSE;
  }

  if( kFlagP && ( part->P() > fMaxP || part->P() < fMinP) ) {
    if(fVerbose) printf("Particle not accepted (P cut).\n");
    fnP++;
    return kFALSE;
  }

  if( kFlagPt && ( part->Pt() > fMaxP || part->Pt() < fMinP ) ) {
    if(fVerbose) printf("Particle not accepted (Pt cut).\n");
    fnPt++;
    return kFALSE;
  }

//  if( kFlagCMRapidity && () ) {
//    if(fVerbose) printf("Particle not accepted (CMRapidity cut).\n");
//    fnCMRapidity++;
//    return kFALSE;
//  }

  if( kFlagTheta && ( part->Theta()*rad2deg > fMaxTheta || part->Theta()*rad2deg < fMinTheta ) ) {
    if(fVerbose) printf("Particle not accepted (Theta cut).\n");
    fnTheta++;
    return kFALSE;
  }

  if( kFlagPhi && ( part->Phi()*rad2deg > fMaxPhi || part->Phi()*rad2deg < fMinPhi ) ) {
    if(fVerbose) printf("Particle not accepted (Phi cut).\n");
    fnPhi++;
    return kFALSE;
  }

  fNGood++;
  return kTRUE;
}

TObjArray* HCutParticle::getAcceptedParticles(TObjArray *partList) {
// Not implemented yet
  partList = 0;
  return partList;
}

ClassImp(HCutParticle)
