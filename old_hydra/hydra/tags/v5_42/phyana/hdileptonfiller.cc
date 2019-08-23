//*-- AUTHOR : D.Magestro
//*-- Created : 18/06/2001
//////////////////////////////////////////////////////////////////////////
//
//  HDileptonFiller
//
//  This is a simple task which fills the HDilepton container by iterating
//  over HParticle leptons.
//
//  This task should be used as an example for developing new and adapting
//  existing tasks.
//
//  All combinations of leptons are used to build dileptons.  The charge
//  data member of HDilepton is used to distinguish e+e+ (charge=2), e+e-
//  (charge=0), and e-e- (charge=-2).
//
//////////////////////////////////////////////////////////////////////////

#include <TRandom.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <TMath.h>
#include "phyanadef.h"
#include "hdileptonfiller.h"
#include "hparticle.h"
#include "hdilepton.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
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
  iterPart=0;
  kSkip = kFALSE;
}

HDileptonFiller::HDileptonFiller(Text_t *name,Text_t *title,Bool_t skip) :
               HReconstructor(name,title) {
  fPartCat=0;
  fDileptCat=0;
  iterPart=0;
  kSkip = skip;
}

Bool_t HDileptonFiller::init(void) {
  // Creates the Dilepton and Particle categories and adds them to
  // the current event.

  fPartCat = gHades->getCurrentEvent()->getCategory(catParticle);
  if (!fPartCat) return kFALSE;

  iterPart = (HIterator *)fPartCat->MakeIterator("native");

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


  HParticle  *part = 0;
  HDilepton *dilept = 0;
  HLocation loc;

  iterPart->Reset();
  Int_t nElec,nPosi,iElec,iPosi;
  Int_t nDileptons = 0;
  Int_t i,k;
  Double_t invMass,angle;
  nElec = 0;
  nPosi = 0;

  HParticle partElec[20];     // FIXME: Change to TObjArray...
  HParticle partPosi[20];
  Int_t iPartElec[20];
  Int_t iPartPosi[20];

  // Find leptons using Pid and fill arrays of e+ and e-
  while ((part=(HParticle*) iterPart->Next())!=0) {

    if(part->GetPid() == 3) {     // electron
       partElec[nElec] = part;
       iPartElec[nElec] = nElec;  // Index to be used for selecting leptons
       nElec++;
    }
    if(part->GetPid() == 2) {     // positron
       partPosi[nPosi] = part;
       iPartElec[nPosi] = nElec+nPosi; // Index to be used for selecting leptons
       nPosi++;
    }
  }  // Particle iterator

  // Build dileptons of opposite sign
  for(iElec = 0;iElec<nElec; iElec++) {
    for(iPosi = 0;iPosi<nPosi; iPosi++) {
      dilept = (HDilepton*) fDileptCat->getNewSlot(loc);
      dilept = new(dilept) HDilepton;

      dilept->SetPart1(partElec[iElec]);
      dilept->SetPart2(partPosi[iPosi]);

      dilept->SetPart1Index(iPartElec[iElec]);
      dilept->SetPart2Index(iPartPosi[iPosi]);

      dilept->SetCharge(0);
      nDileptons++;

      angle = partElec[iElec].Angle(partPosi[iPosi]);
      invMass = 2.*sin(angle/2.)*sqrt( partElec[iElec].P()*partPosi[iPosi].P() );
      dilept->SetInvMass(invMass);

    }
  }

  // Build dileptons of 2 electrons
  for(i=0; i<nElec; i++) {
    for(k = i+1; k<nElec; k++) {
      dilept = (HDilepton*) fDileptCat->getNewSlot(loc);
      dilept = new(dilept) HDilepton;

      dilept->SetPart1(partElec[i]);
      dilept->SetPart2(partElec[k]);

      dilept->SetPart1Index(iPartElec[i]);
      dilept->SetPart2Index(iPartElec[k]);

      dilept->SetCharge(-2);
      nDileptons++;

      angle = partElec[i].Angle(partElec[k]);
      invMass = 2.*sin(angle/2.)*sqrt( partElec[i].P()*partElec[k].P() );
      dilept->SetInvMass(invMass);
    }
  }

  // Build dileptons of 2 positrons
  for(i = 0; i<nPosi; i++) {
    for(k = i+1; k<nPosi; k++) {
      dilept = (HDilepton*) fDileptCat->getNewSlot(loc);
      dilept = new(dilept) HDilepton;

      dilept->SetPart1(partPosi[i]);
      dilept->SetPart2(partPosi[k]);

      dilept->SetPart1Index(iPartPosi[i]);
      dilept->SetPart2Index(iPartPosi[k]);

      dilept->SetCharge(2);
      nDileptons++;

      angle = partPosi[i].Angle(partPosi[k]);
      invMass = 2.*sin(angle/2.)*sqrt( partPosi[i].P()*partPosi[k].P() );
      dilept->SetInvMass(invMass);
    }
  }

  if(nDileptons == 0 && kSkip == kTRUE) return kSkipEvent;
  return 0;
}

ClassImp(HDileptonFiller)
