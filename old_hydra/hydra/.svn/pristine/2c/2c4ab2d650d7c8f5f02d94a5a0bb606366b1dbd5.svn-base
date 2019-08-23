//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HCutTrack - cuts on tracks and their hit components
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
#include "hcuttrack.h"
#include "hkicktrack.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HCutTrack::HCutTrack() : HCut() {
// Default constructor
  fTrackCat = 0;
  iterTrack=0;

  reset();
}

HCutTrack::HCutTrack(const Text_t *name, const Text_t *title)
         :HCut(name, title) {
// Default constructor
  fTrackCat = 0;
  iterTrack = 0;

  reset();
}

void HCutTrack::reset() {
// Disables all cuts by resetting to kFALSE
  kFlagTofinoTof = kFALSE;
  kFlagTofTof = kFALSE;
  kFlagP = kFALSE;
  kFlagIdentifiedHadron = kFALSE;
  return;
}

void HCutTrack::setStandardCuts() {
// Not implemented yet
  return;
}

void HCutTrack::printCutList() {
// Prints a table of enabled cuts to stdio
  printf("\n--------------------------------------------\n");
  printf("               HCutTrack cuts               \n");
  printf("               --------------               \n");

  if(kFlagIdentifiedHadron)
     printf("isIdentifiedHadron = TRUE\n");
  if(kFlagP)
     printf("fMinP         = %5.1f   fMaxP         = %5.1f\n",fMinP,fMaxP);
  if(kFlagTofinoTof)
     printf("fMinTofinoTof = %5.1f   fMaxTofinoTof = %5.1f\n",fMinTofinoTof,fMaxTofinoTof);
  if(kFlagTofTof)
     printf("fMinTofTof    = %5.1f   fMaxTofTof    = %5.1f\n",fMinTofTof,fMaxTofTof);

  printf("--------------------------------------------\n\n");
  return;
}

void HCutTrack::printCutStatistics() {
  printf("\n--------------------------------------------------------------\n");
  printf("                        HCutTrack cuts               \n");
  printf("                        --------------               \n");

  if(kFlagIdentifiedHadron)
     printf("isIdentifiedHadron = TRUE\n");
  if(kFlagP)
     printf("fMinP         = %5.1f   fMaxP         = %5.1f  nCut = %6i\n",fMinP,fMaxP,fnP);
  if(kFlagTofinoTof)
     printf("fMinTofinoTof = %5.1f   fMaxTofinoTof = %5.1f  nCut = %6i\n",fMinTofinoTof,fMaxTofinoTof,fnTofinoTof);
  if(kFlagTofTof)
     printf("fMinTofTof    = %5.1f   fMaxTofTof    = %5.1f  nCut = %6i\n",fMinTofTof,fMaxTofTof,fnTofTof);

  printf("----------------------------------------------------------------\n\n");
  return;
}

Int_t HCutTrack::check(HKickTrack *track) {
  // Checks one track and returns 1 if track passes cuts
  fNCheck++;

  if( kFlagIdentifiedHadron &&
             ( track->getPID()<4 || track->getPID()>15) ) {
    fnIdentifiedHadron++;
    return kFALSE;
  }

  if( kFlagP && ( track->getP() > fMaxP || track->getP() < fMinP) ) {
    fnP++;
    return kFALSE;
  }

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

  fNGood++;
  return kTRUE;
}

ClassImp(HCutTrack)
