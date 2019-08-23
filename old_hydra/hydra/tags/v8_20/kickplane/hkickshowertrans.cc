using namespace std;
#include "hkickshowertrans.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerdetector.h"
#include "showerdef.h"
#include "hgeantdef.h"
#include "TObject.h"
#include "hgeantshower.h"
#include "hshowerhittoftrack.h"
#include "hlinearcategory.h"
#include "showertofinodef.h"
#include "hevent.h"
#include <iostream> 
#include <iomanip>
#include "TRandom.h"
#include "hgeantkine.h"
#include "hgeanttof.h"

//_HADES_CLASS_DESCRIPTION 
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
  HShowerHitTofTrack *hittrack =0;
  HShowerHitTof *hit =0;
  TObject *slot = 0;

  fInIter->Reset();
  while ( (geHit = (HGeantShower *)fInIter->Next()) != 0 ) {
    Float_t e,x,y,beta;
    geHit->getHit(e,x,y,beta);
    Int_t row=0,col=0;
    Float_t tof=0.;
    Float_t xm,ym;

    geHit->getHit(e,x,y,beta);
    xm = x + gRandom->Gaus(0,fResolutionX);
    ym = y + gRandom->Gaus(0,fResolutionY);

    //Get TOF
    HGeantKine *geKine=(HGeantKine *)fCatKine->getObject(
	geHit->getTrack()-1);
    HGeantTof *geTof=0;
    geKine->resetTofIter();
    while ( (geTof=(HGeantTof*)geKine->nextTofHit()) != 0) {
      Float_t te,tx,ty,tmom,tlen;
      if (geTof->getModule()>21)
	geTof->getHit(te,tx,ty,tof,tmom,tlen);
    }

    //Store output
    slot = fShoOutput->getNewSlot(fLoc);
    if (slot) {
      hittrack = new (slot) HShowerHitTofTrack;
     
      hittrack->setTrack(geHit->getTrack());
      hittrack->setXY(xm,ym);
      hittrack->setSector(geHit->getSector());
      hittrack->setModule(geHit->getModule());
      hittrack->setRow(row);
      hittrack->setCol(col);
      hittrack->setTof(tof);
      hittrack->setTofinoMult(1);
    }
    slot = fShoOutput2->getNewSlot(fLoc);
    if (slot) {
      hit = new (slot) HShowerHitTof;
 	
      hit->setXY(xm,ym);
      hit->setSector(geHit->getSector());
      hit->setModule(geHit->getModule());
      hit->setRow(row);
      hit->setCol(col);
      hit->setTof(tof);
      hit->setTofinoMult(1);
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

      fShoOutput = ev->getCategory(catShowerHitTofTrack);
      if (!fShoOutput) {
	fShoOutput = new HLinearCategory("HShowerHitTofTrack",1000);
	ev->addCategory(catShowerHitTofTrack,fShoOutput,"Shower");
      }
      fShoOutput2 = ev->getCategory(catShowerHitTof);
      if (!fShoOutput2) {
	fShoOutput2 = new HLinearCategory("HShowerHitTof",1000);
	ev->addCategory(catShowerHitTof,fShoOutput2,"Shower");
      }

      fCatKine = ev->getCategory(catGeantKine);
      if (!fCatKine) return kFALSE;
    }
  }
  return kTRUE;
}

Bool_t HKickShowerTrans::finalize(void) {
  return kTRUE;
}

ClassImp(HKickShowerTrans)
