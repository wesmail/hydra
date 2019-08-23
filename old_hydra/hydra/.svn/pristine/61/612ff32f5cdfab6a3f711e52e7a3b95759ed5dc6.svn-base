#include "hrttracksiminspect.h"
#include "hades.h"
#include "hevent.h"
#include "hrtmdctrksim.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hmdcdef.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "TNtuple.h"

HRtTrackSimInspect::HRtTrackSimInspect(Text_t name[], Text_t title[]) :
	HReconstructor(name,title) {
  fCatTrack = 0;
  fCatKine = 0;
  fTrackIter = 0;
}

HRtTrackSimInspect::~HRtTrackSimInspect(void) {

}

Bool_t HRtTrackSimInspect::init(void) {
  HEvent *ev = Hades::instance()->getCurrentEvent();
  Bool_t r=kTRUE;
  
  if (ev) {
    fCatTrack = ev->getCategory(catMdcTrack);
    if (!fCatTrack) r=kFALSE;
    else {
      fTrackIter = (HIterator *)fCatTrack->MakeIterator();
    }

    fCatKine = ev->getCategory(catGeantKine);
    if (!fCatKine) r=kFALSE;
  }

  if (Hades::instance()->getOutputFile()) {
    Hades::instance()->getOutputFile()->cd();
    fOutput = new TNtuple("TrInspect","TrInspect",
		"pc:r:z:theta:phi:pmdc:con");
  }

  return r;
}

Bool_t HRtTrackSimInspect::finalize(void) {
  if (Hades::instance()->getOutputFile() && fOutput) {
    Hades::instance()->getOutputFile()->cd();
    fOutput->Write();
  }
  return kTRUE;
}

Int_t HRtTrackSimInspect::execute(void) {
  HRtMdcTrkSim *track=0;

  fTrackIter->Reset();
  while ( (track = (HRtMdcTrkSim *)fTrackIter->Next()) != 0) {
    for (Int_t i=0;i<track->getNTracks();i++) {
      if ( track->getTrack(i) == 0) 
	Error("execute","invalid track %i of %i",i,track->getNTracks());
      HGeantKine *kine = (HGeantKine *)
	fCatKine->getObject(track->getTrack(i)-1);
      HGeantMdc *geMdc=0;
      Float_t mx,my,mtof,mp;

      kine->resetMdcIter();
      mp=0;
      while ( (geMdc = (HGeantMdc *)kine->nextMdcHit()) != 0) {
	if (geMdc->getModule()==0 && geMdc->getLayer()==6) {
	  geMdc->getHit(mx,my,mtof,mp);
	}
      }

      fOutput->Fill(track->getP(),track->getR(),track->getZ(),
	  track->getTheta(),track->getPhi(),
	  mp,track->getFitResult());
    }
  }

  return 0;
}

ClassImp(HRtTrackSimInspect)
