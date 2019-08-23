//*-- AUTHOR : D.Magestro
//*-- Created : 18/06/2001
//*-- Modified: 07/11/2001
//////////////////////////////////////////////////////////////////////////
//
//  HDileptonFiller
//
//  This is a simple task which fills the HDilepton container by iterating
//  over HParticle leptons.
//
//  All combinations of leptons are used to build dileptons.  The charge
//  data member of HDilepton is used to distinguish e+e+ (charge=2), e+e-
//  (charge=0), and e-e- (charge=-2).
//
//  HPhysicsEvent is used to access the HParticle category's leptons
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
//////////////////////////////////////////////////////////////////////////
using namespace std;
#include <TRandom.h>
#include <time.h>
#include <iostream> 
#include <iomanip>
#include <math.h>
#include <TMath.h>
#include "phyanadef.h"
#include "hdileptonfiller.h"
#include "hparticle.h"
#include "hphysicsconstants.h"
#include "hphysicsevent.h"
#include "hdilepton.h"
#include "hades.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hrecevent.h"
#include "hlocation.h"

HDileptonFiller::HDileptonFiller(void) {
  // Default constructor
  fPartCat=0;
  fDileptCat=0;
  phyEvent=0;
  kSkip = kFALSE;
}

HDileptonFiller::HDileptonFiller(Text_t *name,Text_t *title,Bool_t skip) :
               HReconstructor(name,title) {
  fPartCat=0;
  fDileptCat=0;
  phyEvent=0;
  kSkip = skip;
}

Bool_t HDileptonFiller::init(void) {
  // Creates the Dilepton and Particle categories and adds them to
  // the current event.

  fPartCat=gHades->getCurrentEvent()->getCategory(catParticle);
  if (!fPartCat)  {
    Error("init","catParticle doesn't exist in input");
    return kFALSE;
  }
  phyEvent = new HPhysicsEvent(fPartCat);

  fDileptCat=gHades->getCurrentEvent()->getCategory(catDilepton);
  if (!fDileptCat) {
    fDileptCat = new HLinearCategory("HDilepton",1000);
    if (fDileptCat) gHades->getCurrentEvent()->addCategory(catDilepton,
          fDileptCat,"PhyAna");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  }
  return kTRUE;
}

Int_t HDileptonFiller::execute(void) {
// Fills the HDilepton container using input from HParticle. All lepton combinations
// are used to build dileptons.

  HParticle  *part1, *part2;

  TObjArray *electrons = 0, *positrons = 0;

  Int_t i,k,nElec,nPosi,iElec,iPosi;
  Int_t nDileptons = 0;
  nElec = nPosi = 0;

  electrons = phyEvent->getParticles("e-");
  positrons = phyEvent->getParticles("e+");

  nElec = electrons->GetEntriesFast();
  nPosi = positrons->GetEntriesFast();

  if (nElec+nPosi<1) return 0;

  // Build dileptons of opposite sign
  for(iElec = 0;iElec<nElec; iElec++) {
    for(iPosi = 0;iPosi<nPosi; iPosi++) {

      part1 = (HParticle*) electrons->At(iElec);
      part2 = (HParticle*) positrons->At(iPosi);

      makeDilepton(part1,part2);
      nDileptons++;
    }
  }

  // Build dileptons of 2 electrons
  for(i=0; i<nElec; i++) {
    for(k = i+1; k<nElec; k++) {

      part1 = (HParticle*) electrons->At(i);
      part2 = (HParticle*) electrons->At(k);

      makeDilepton(part1,part2);
      nDileptons++;
    }
  }

  // Build dileptons of 2 positrons
  for(i=0; i<nPosi; i++) {
    for(k = i+1; k<nPosi; k++) {

      part1 = (HParticle*) positrons->At(i);
      part2 = (HParticle*) positrons->At(k);

      makeDilepton(part1,part2);
      nDileptons++;
    }
  }

  if(nDileptons == 0 && kSkip == kTRUE) return kSkipEvent;
  return 0;
}

void HDileptonFiller::makeDilepton(HParticle *part1, HParticle *part2) {
// This function takes two HParticle pointers and builds an HDilepton.
// For now, the invariant mass is set explicitly using formula for Inv Mass
// (instead of using dilepton's inherited TLorentzVector::M() function).

  HDilepton *dilept = 0;
  HLocation loc;
  Double_t invMass,angle;

  dilept = (HDilepton*) fDileptCat->getNewSlot(loc);
  dilept = new(dilept) HDilepton;

  dilept->setPart1(part1);
  dilept->setPart2(part2);

  dilept->setPart1Id(part1->getIndex());
  dilept->setPart2Id(part2->getIndex());

  dilept->setCharge( part1->getCharge() + part2->getCharge() );

  angle = part1->Angle(part2);
  invMass = 2.*sin(angle/2.)*sqrt( part1->P()*part2->P() );
  dilept->setInvMass(invMass);
}



ClassImp(HDileptonFiller)
