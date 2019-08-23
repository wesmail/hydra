#include "hkicktrackfsim.h"
#include "hkickimpactsim.h"
#include "hkicktracksim.h"
#include "hiterator.h"
#include "hmdchit.h"
#include "hmdcseg.h"
#include "hmdcsimseg.h"
#include "hmdcsimhit.h"
#include "hshowerhittoftrack.h"
#include "hades.h"
#include "htofhitsim.h"
#include "htofhitsim2.h"
#include "showertofinodef.h"
#include <TNtuple.h>
#include "hkickcandidateevaluatorsim.h"
#include "hkickcandidateevaluatorsim2.h"
#include "hevent.h"
#include "hgeantkine.h"
#include "kickdef.h"
#include "hmatrixcategory.h"

HKickTrackFSim::HKickTrackFSim(Text_t name[],Text_t title[]) 
  : HKickTrackF(name,title) {
  //Default constructor.
  fData=0; fControl=0;
  fShowerCatId = catShowerHitTofTrack;
  delete fImpacts;
  fImpacts = new TClonesArray("HKickImpactSim",20);
}

Bool_t HKickTrackFSim::init(void) {
  if (!HKickTrackF::init()) return kFALSE;

  fKineCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  if (!fKineCat) return kFALSE;

  if (gHades->getOutputFile()) {
    fData=new TNtuple("trackfsim_data","data","p:pc:valid:mass:xPull:qf:ptof:pid:s");
    fControl=new TNtuple("trackfsim_con","Control","p:preal:errP:valid:mass:xPull:x:y:z:qf:ptof:errptof:pid:den");
  }
  return kTRUE;
}

void HKickTrackFSim::readHits(HLocation &loc) {
  //Loops on the input category reading objects and adding objects to
  //the list of candidates fImpacts.
 
  Int_t geTrack=0;
  HMdcSeg *seg=0;
  HGeomVector r;
  HKickImpactSim *imp=0;
  Int_t mdcSector=0;
  fIter->Reset();
  fIter->gotoLocation(loc);
  while ( (seg=(HMdcSeg *)fIter->Next()) != 0 ) {
    mdcSector=seg->getSec();
    imp=new( (*fImpacts)[fNImpacts++] ) HKickImpactSim;
    transform(seg,r,imp->direction());
    fKickPlane->calcIntersection(r,imp->direction(),imp->position());
    imp->setSector(mdcSector);
    imp->setOriginalSegment(seg);
    if (seg->InheritsFrom(HMdcSimSeg::Class())) {
      imp->setMomentum(((HMdcSimSeg *)seg)->getMomentum());
      geTrack = ((HMdcSimSeg *)seg)->getTrack(0);
      imp->setTrack(geTrack);
    }
  }
}

Bool_t HKickTrackFSim::finalize(void) {
  if (gHades->getOutputFile() && fData && fControl) {
    gHades->getOutputFile()->cd();
    fData->Write();
    fControl->Write();
  }
  return HKickTrackF::finalize();
}

HKickTrack* HKickTrackFSim::fillData(HKickCandidate &candidate,
				     HKickImpact *imp) {
  //Fills output data in the output category
  HKickImpactSim *impact=(HKickImpactSim *)imp;
  HKickTrackSim *r=0;

  fData->Fill(impact->getMomentum(),
	      candidate.getMomentum(),
	      candidate.getFlag(),
	      candidate.getMass(),
	      candidate.getXPull(),
	      candidate.isAssigned()?1:0,
	      candidate.getPTof(),
	      candidate.getPID(),
	      candidate.getSystem());

  r = dynamic_cast<HKickTrackSim *>(HKickTrackF::fillData(candidate,imp));
  if (!r) {
    Error("fillData","Internal Error 1.Please report bug");
    return 0;
  }

  // Add extra information from simulations
  HMdcSegSim *seg = dynamic_cast<HMdcSegSim *>(imp->getOriginalSegment());
  if (!seg) {
    Error("fillData","Internal Error 2.Please report bug");
    return r;
  } else {
    r->setMdcTracks(seg);
  }
      

  if (candidate.getSystem() == 0) { //Specific stuff for Shower
    HShowerHitTofTrack *oHit = 0;
    oHit = dynamic_cast<HShowerHitTofTrack *>(candidate.getOuterHit());
    if (!oHit) {
      Error("fillData","Internal error 3. Please report bug!");
    } else {
      r->setMetaTracks(oHit);
    }
  } else {                          //Specific stuff for TOF
    HTofHitSim *oHit = 0;
    oHit = dynamic_cast<HTofHitSim *>(candidate.getOuterHit());
    if (!oHit) {
      Error("fillData","Internal error 4. Please report bug!");
      } else {
	r->setMetaTracks(oHit);
      }     
  }
  
  return r;
}

void HKickTrackFSim::fillControl(HKickCandidate &candidate, HKickImpact *imp) {
  //Fills the control ntuple
  HKickImpactSim *impact=(HKickImpactSim *)imp;

  HGeantKine *kine = (HGeantKine*)fKineCat->getObject(((HKickImpactSim*)impact)->getTrack()-1);
  Int_t trackN,pid;
  kine->getParticle(trackN,pid);

  fControl->Fill(candidate.getMomentum(),impact->getMomentum(),
		 candidate.getMomentumError(),
		 candidate.getFlag(),
		 candidate.getMass(),
		 candidate.getXPull(),
		 candidate.getPos1X(),
		 candidate.getPos1Y(),
		 candidate.getPos1Z(),
		 candidate.isAssigned()?1:0,
		 candidate.getPTof(),
		 candidate.getPTofErr(),
		 pid,
		 candidate.getDenominator());
}

HKickCandidateEvaluator *HKickTrackFSim::makeEvaluator(void) {
  //Creates a candidate evaluator. Factory method
  HKickCandidateEvaluator *eval = 0;
  if (fTofHits->getClass() == HTofHitSim::Class())
    eval = new HKickCandidateEvaluatorSim(fKickPlane,fMatchPar);
  else if (fTofHits->getClass() == HTofHitSim2::Class())
    eval = new HKickCandidateEvaluatorSim2(fKickPlane,fMatchPar);
  else {
    Warning("makeEvaluator","Unknown TOF data class. Using code for non simulated data");
    eval = new HKickCandidateEvaluator(fKickPlane,fMatchPar);
  }
  return eval;
}

HKickTrack *HKickTrackFSim::makeTrack(void) {
  TObject *slot = 0;
  HKickTrack *track = 0;

  slot = fCandidateOut->getNewSlot(fSectorLoc);
  if (slot) {
    track = new(slot) HKickTrackSim;
  } 

  return track;
}

HCategory *HKickTrackFSim::makeOutputCategory(HEvent *event) {
  fCandidateOut=event->getCategory(catTrack);
  if (!fCandidateOut) {
    Int_t ssize[2]={6,100};
    fCandidateOut = new HMatrixCategory("HKickTrackSim",2,ssize,0.5);
    if (fCandidateOut) event->addCategory(catKickTrack,
					  fCandidateOut,
					  "Tracks");
    else {
      Error("init","Unable to setup output");
      return kFALSE;
    }
  }

  return fCandidateOut;
}

ClassImp(HKickTrackFSim)
