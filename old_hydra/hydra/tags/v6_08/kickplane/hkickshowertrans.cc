#include "hkickshowertrans.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerdetector.h"
#include "showerdef.h"
#include "hgeantdef.h"
#include <TObject.h>
#include "hgeantshower.h"
#include "hshowerhittoftrack.h"
#include "hlinearcategory.h"
#include "showertofinodef.h"
#include "hevent.h"
#include <iostream.h>
#include <TRandom.h>

///////////////////////////////////////////////////////////////////
//HKickShowerTrans
//
//  Utility class to convert Geant hits in shower hits. Optionally 
//adding a resolution effect.
///////////////////////////////////////////////////////////////////

HKickShowerTrans::HKickShowerTrans(Text_t name[], Text_t title[]) : 
  HReconstructor(name,title) {
 fShoInput=0;
 fShoOutput=0;
 fInIter=0;
 fResolutionX = fResolutionY = 0.;
 //fLoc.set(0);
}

HKickShowerTrans::~HKickShowerTrans(void) {
}

Int_t HKickShowerTrans::execute(void) {
  HGeantShower *geHit=0;
  HShowerHitTofTrack *hit =0;
  TObject *slot = 0;

  fInIter->Reset();
  while ( (geHit = (HGeantShower *)fInIter->Next()) != 0 ) {
    Float_t e,x,y,beta;
    geHit->getHit(e,x,y,beta);
    
    slot = fShoOutput->getNewSlot(fLoc);
    if (slot) {
      hit = new (slot) HShowerHitTofTrack;

      hit->setXY(x+gRandom->Gaus(0,fResolutionX),
		 y+gRandom->Gaus(0,fResolutionY));
      hit->setSector(geHit->getSector());
      hit->setModule(geHit->getModule());
      hit->setTrack(geHit->getTrack());
      hit->setTof(0.);  //FIXME: Make proper TOF calculation
      hit->setTofinoMult(0); //FIXME: Since TOF is not calculated
    }
  }
  return 0;
}

Bool_t HKickShowerTrans::init(void) {
  if (gHades) {
    HEvent *ev = gHades->getCurrentEvent();
    HSpectrometer *spec = gHades->getSetup();

    if (ev && spec) {
      fShoInput = ev->getCategory(catShowerGeantRaw);
      if (!fShoInput) return kFALSE;
      fInIter = (HIterator *) fShoInput->MakeIterator();

      fShoOutput = ev->getCategory(catShowerHit);
      if (!fShoOutput) {
	fShoOutput = new HLinearCategory("HShowerHitTofTrack",1000);
	ev->addCategory(catShowerHitTofTrack,fShoOutput,"Shower");
      }
    }
  }
  return kTRUE;
}

Bool_t HKickShowerTrans::finalize(void) {
  return kTRUE;
}

ClassImp(HKickShowerTrans)
