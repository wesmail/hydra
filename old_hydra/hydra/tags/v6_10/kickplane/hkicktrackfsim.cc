#include "hkicktrackfsim.h"
#include "hkickimpact.h"
#include "hkicktracksim.h"
#include "hiterator.h"
#include "hmdchit.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
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
  fShowerCatId = catShowerHitTof;
  delete fImpacts;
  fImpacts = new TClonesArray("HKickImpact",20);
}

Bool_t HKickTrackFSim::init(void) {
  fKineCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  if (!fKineCat) return kFALSE;

  if (!HKickTrackF::init()) return kFALSE;

  if (gHades->getOutputFile()) {
    fData=new TNtuple("trackfsim_data","data",
		      "p:pc:valid:mass:xPull:qf:ptof:pid:s");
    fControl=new TNtuple("trackfsim_con","Control","p:preal:errP:valid:mass:xPull:x:y:z:qf:ptof:errptof:pid:den");
  }
  return kTRUE;
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
  HKickTrackSim *r=0;
  Float_t valid=0.;
  Int_t totalTimes = 0;
  HGeantKine *kine = 0;
  Int_t trackN=0,pid=-1;
  Float_t p = 0.;
 
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


  //Fill summary ntuple
  for (int i=0; i<seg->getNTracks(); i++) totalTimes+=seg->getNTimes(i);
  if (candidate.getFlag()<0) {
    valid = 0.;
  } else if (candidate.getFlag()>=100) {
    valid = 10 + float(seg->getNTimes(candidate.getFlag()-100)) / 
      float(totalTimes);
    trackN = seg->getTrack(candidate.getFlag()-100);
  } else {
    valid = float(seg->getNTimes(candidate.getFlag())) / float(totalTimes);
    trackN = seg->getTrack(candidate.getFlag());
  }

  if (trackN>0) {
    kine = (HGeantKine*)fKineCat->getObject(trackN-1);
    kine->getParticle(trackN,pid);
    p = kine->getTotalMomentum();
  }

  fData->Fill(p,
	      candidate.getMomentum(),
	      valid,
	      candidate.getMass(),
	      candidate.getXPull(),
	      candidate.isAssigned()?1:0,
	      candidate.getPTof(),
	      candidate.getPID(),
	      candidate.getSystem());

  return r;
}

void HKickTrackFSim::fillControl(HKickCandidate &candidate, HKickImpact *imp) {
  //Fills the control ntuple
  HGeantKine *kine = 0;
  Int_t trackN=0,pid=-1;
  Int_t totalTimes = 0;
  Float_t valid=0.,p=0.;

  HMdcSegSim *seg = dynamic_cast<HMdcSegSim *>(imp->getOriginalSegment());
  if (!seg) {
    Error("fillData","Internal Error 2.Please report bug");
    return;
  }

  //candidate.getFlag() is the id of the common track btw seg and meta
  //with the maximum amount of contributing wires in the segment.
  for (int i=0; i<seg->getNTracks(); i++) totalTimes+=seg->getNTimes(i);
  if (candidate.getFlag()<0) {
    valid = 0.;
  } else if (candidate.getFlag()>=100) {
    valid = 10 + float(seg->getNTimes(candidate.getFlag()-100)) / 
      float(totalTimes);
    trackN = seg->getTrack(candidate.getFlag()-100);
  } else {
    valid = float(seg->getNTimes(candidate.getFlag())) / float(totalTimes);
    trackN = seg->getTrack(candidate.getFlag());
  }

  if (trackN>0) {
    kine = (HGeantKine*)fKineCat->getObject(trackN-1);
    kine->getParticle(trackN,pid);
    p = kine->getTotalMomentum();
  }

  fControl->Fill(candidate.getMomentum(),p,
		 candidate.getMomentumError(),
		 valid,
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
    eval = new HKickCandidateEvaluatorSim(fKickPlane,fMatchPar,fKineCat);
  else if (fTofHits->getClass() == HTofHitSim2::Class())
    eval = new HKickCandidateEvaluatorSim2(fKickPlane,fMatchPar,fKineCat);
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
