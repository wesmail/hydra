#include "hrtm3metamatchsim.h"
#include "hmatrixcategory.h"
#include "hmdchitsim.h"
#include "htofhitsim.h"
#include "hshowerhittoftrack.h"
#include "hades.h"
#include "hevent.h"
#include "hgeantkine.h"
#include <TFile.h>

Bool_t HRtM3MetaMatchSim::init(void) {
  if (!HRtM3MetaMatch::init()) return kFALSE;

  fCatKine = gHades->getCurrentEvent()->getCategory(catGeantKine);

  if (gHades->getOutputFile()) {
    gHades->getOutputFile()->cd();
    fControl = new TNtuple("m3metamatch","m3metamatch","p:x:y:dx:dy:valid:sys:nvalid");
  } else fControl = 0;
  return kTRUE;
}

HCategory *HRtM3MetaMatchSim::makeOutputCategory(HEvent *ev) {
  HCategory *r;
  Int_t sizes[2]={6,60}; //Maximum 60 tracks per sector
  r = new HMatrixCategory("HRtMetaSegSim",2,sizes,0.5);
  return r;
}


HRtMetaSeg *HRtM3MetaMatchSim::fillData(HMdcHit *hit, HGeomVector &pos, 
				     HGeomVector &alpha, HRtMetaPoint *point) {
  HRtMetaSegSim *c = (HRtMetaSegSim *)HRtM3MetaMatch::fillData(hit,pos,alpha,point);
  if (!c) return 0;

  HMdcHitSim *hitsim = (HMdcHitSim *)hit;
  Int_t lsTracks[5];
  UChar_t lsTimes[5];
  Int_t nTracks = 0,addedTracks=0;
  Int_t mostCommonTrackTimes = 0;
  Int_t totalTimes = 0;
  Float_t valid;

  if (point->getSystem()==0) { //Shower
    HShowerHitTofTrack *track = (HShowerHitTofTrack *)point->getMetaHit();
    lsTracks[0] = track->getTrack();
    lsTimes[0] = 1;
    nTracks = 1;
  } else {
    HTofHitSim *tofhit = (HTofHitSim *)point->getMetaHit();
    lsTracks[0] = tofhit->getNTrack1();
    lsTimes[0] = 1;
    if (tofhit->getNTrack2()) {
      nTracks = 2;
      lsTracks[1] = tofhit->getNTrack2();
      lsTimes[1] = 1;
    } else {
      nTracks = 1;
    }
  }

  totalTimes = nTracks;
  for (int i=0; i<hitsim->getNTracks();i++)
    totalTimes += hitsim->getNTimes(i);

  for (int i=0; i<nTracks; i++) {
    for (int j=0; j<hitsim->getNTracks(); j++) {
      if (lsTracks[i] == hitsim->getTrack(j)) {
	lsTimes[i]+=hitsim->getNTimes(j);
	if (mostCommonTrackTimes<lsTimes[i]) mostCommonTrackTimes = lsTimes[i];
      } else {
	if (nTracks+addedTracks<6) {
	  lsTracks[nTracks+addedTracks] = hitsim->getTrack(j);
	  lsTimes[nTracks+addedTracks] = hitsim->getNTimes(j);
	  addedTracks++;
	}
      }
    }
  }
  nTracks = nTracks + addedTracks;

  valid = float(mostCommonTrackTimes) / float(totalTimes);

  for (int i=0;i<nTracks;i++) {
    c->setTrack(i,lsTracks[i]);
    c->setNTimes(i,lsTimes[i]);
  }
  c->setNTracks(nTracks);
  c->setValidity(valid);

  return c;
}

HRtMetaSeg *HRtM3MetaMatchSim::makeSegment(TObject *s) { 
  return new(s) HRtMetaSegSim;
}

void HRtM3MetaMatchSim::fillControl(HMdcHit *hit, HRtMetaPoint &point,
				    Float_t dx,Float_t dy,Int_t &nValid) {
  HMdcHitSim *hitsim = (HMdcHitSim *)hit;
  Int_t lsTracks[5];
  UChar_t lsTimes[5];
  Int_t nTracks = 0,addedTracks=0;
  Int_t mostCommonTrackTimes = 0;
  Int_t mostCommonTrack = 0;
  Int_t totalTimes = 0;
  Float_t valid;
  HGeantKine *geKine = 0;
  Float_t mom=-1;

  if (point.getSystem()==0) { //Shower
    HShowerHitTofTrack *track = (HShowerHitTofTrack *)point.getMetaHit();
    lsTracks[0] = track->getTrack();
    lsTimes[0] = 1;
    nTracks = 1;
  } else {
    HTofHitSim *tofhit = (HTofHitSim *)point.getMetaHit();
    lsTracks[0] = tofhit->getNTrack1();
    lsTimes[0] = 1;
    if (tofhit->getNTrack2()) {
      nTracks = 2;
      lsTracks[1] = tofhit->getNTrack2();
      lsTimes[1] = 1;
    } else {
      nTracks = 1;
    }
  }

  totalTimes = nTracks;
  for (int i=0; i<hitsim->getNTracks();i++)
    totalTimes += hitsim->getNTimes(i);

  for (int i=0; i<nTracks; i++) {
    for (int j=0; j<hitsim->getNTracks(); j++) {
      if (lsTracks[i] == hitsim->getTrack(j)) {
	lsTimes[i]+=hitsim->getNTimes(j);
	if (mostCommonTrackTimes<lsTimes[i]) {
	  mostCommonTrackTimes = lsTimes[i];
	  mostCommonTrack = lsTracks[i];
	}
      } else {
	if (nTracks+addedTracks<6) {
	  lsTracks[nTracks+addedTracks] = hitsim->getTrack(j);
	  lsTimes[nTracks+addedTracks] = hitsim->getNTimes(j);
	  addedTracks++;
	}
      }
    }
  }
  nTracks = nTracks + addedTracks;

  valid = float(mostCommonTrackTimes) / float(totalTimes);
  if (valid>.98) nValid++;

  if (mostCommonTrack != 0) {
    geKine = (HGeantKine *)fCatKine->getObject(mostCommonTrack-1);
    mom = geKine->getTotalMomentum();
  }

  if (fControl)
    fControl->Fill(mom,hit->getX(),hit->getY(),dx,dy,valid,point.getSystem(),nValid);
}

ClassImp(HRtM3MetaMatchSim)
