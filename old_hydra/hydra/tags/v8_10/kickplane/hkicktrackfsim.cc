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
#include "htofclustersim.h"
#include "showertofinodef.h"
#include <TNtuple.h>
#include "hkickcandidateevaluatorsim.h"
#include "hkickcandidateevaluatorsim2.h"
#include "hevent.h"
#include "hgeantkine.h"
#include "kickdef.h"
#include "hmatrixcategory.h"
#include "hkicktrackfpar.h"
#include "hgeantmdc.h"

HKickTrackFSim::HKickTrackFSim(Text_t name[],Text_t title[]) 
  : HKickTrackF(name,title) {
  //Default constructor.
  fData=0; fControl=0;
  fShowerCatId = catShowerHitTofTrack;
  delete fImpacts;
  fImpacts = new TClonesArray("HKickImpact",20);
  fDebug = kFALSE;
  fUseSimulatedTof = kFALSE;
}

Bool_t HKickTrackFSim::init(void) {
  fKineCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
  if (!fKineCat) return kFALSE;

  if (!HKickTrackF::init()) return kFALSE;

  if (gHades->getOutputFile() && fDebug) {
    fData=new TNtuple("trackfsim_data","data",
		      "p:pc:valid:mass:xPull:qf:ptof:pid:s");
    fControl=new TNtuple("trackfsim_con","Control","p:preal:errP:valid:mass:xPull:x:y:z:qf:theta:phi:pid:den:s:target:ver");
  }
  return kTRUE;
}

Bool_t HKickTrackFSim::finalize(void) {
  if (fDebug && gHades->getOutputFile() && fData && fControl) {
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
  Float_t p = -1;
 
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
  } else {
    //Specific stuff for TOF
    if(fTofClusterMode){
      HTofClusterSim *oHit = 0;
      oHit = static_cast<HTofClusterSim *>(candidate.getOuterHit());
      if (!oHit) {
        Error("fillData","Internal error 4(%s). Please report bug!",candidate.getOuterHit()->IsA()->GetName());
      } else {
        r->setMetaTracks(oHit);
      }
    } else {
      HTofHitSim *oHit = 0;
      oHit = static_cast<HTofHitSim *>(candidate.getOuterHit());
      if (!oHit) {
        Error("fillData","Internal error 4(%s). Please report bug!",candidate.getOuterHit()->IsA()->GetName());
      } else {
        r->setMetaTracks(oHit);
      }
    }
  }


  //Fill summary ntuple
  if (fDebug) {
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
      HGeantMdc *geMdc=0;
      kine = (HGeantKine*)fKineCat->getObject(trackN-1);
      kine->getParticle(trackN,pid);
      kine->resetMdcIter();
      while ( (geMdc=(HGeantMdc*)kine->nextMdcHit()) != 0) {
	if (geMdc->getModule() == 0 && geMdc->getLayer()==6) {
	  Float_t mx,my,mtof;
	  geMdc->getHit(mx,my,mtof,p);
	}
      }
      if (p<0) p = kine->getTotalMomentum();
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
  }

  return r;
}

void HKickTrackFSim::fillControl(HKickCandidate &candidate, HKickImpact *imp) {
  //Fills the control ntuple
  HGeantKine *kine = 0;
  Int_t trackN=0,pid=-1;
  Int_t totalTimes = 0;
  Float_t valid=0.,p=0.;
  Float_t target_x,target_y,target_z;
  Float_t realTarget,calculatedTarget,targetFlag=-999;
  Float_t takingVertex=-999;
  Float_t vars[17];

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

  Float_t theta=0,phi=0,px,py,pz,pmdc;
  Int_t parentTrack=-999;
  if (trackN>0) {
    HGeantMdc *geMdc=0;
    kine = (HGeantKine*)fKineCat->getObject(trackN-1);
    kine->getParticle(trackN,pid);
    parentTrack = kine->getParentTrack();
    kine->resetMdcIter();
    while ( (geMdc=(HGeantMdc*)kine->nextMdcHit()) != 0) {
      if (geMdc->getModule() == 0 && geMdc->getLayer()==6) {
	Float_t mx,my,mtof;
	geMdc->getHit(mx,my,mtof,pmdc);
      }
    }
    //if (p<0) p = kine->getTotalMomentum();
    p=kine->getTotalMomentum();
    kine->getMomentum(px,py,pz);
    theta = acos(pz/p);
    phi = atan2(px,py);
    
    //Determine if the target computation was good
    kine->getVertex(target_x,target_y,target_z);
    if (target_z <= -30.29) 
	    realTarget = 2;
     else 
	    realTarget = 1;
     
     HVertex &event_vertex = gHades->getCurrentEvent()->getHeader()->getVertex();
     HGeomVector &vertex = event_vertex.getPos();
     Bool_t VertexAlgorithm = kTRUE;

    if (VertexAlgorithm && event_vertex.getIterations()>0) {
      takingVertex = 1;
      if (vertex(2) <= -32.29) 
	      calculatedTarget = 2;
       else if (vertex(2) > -28.29)
	      calculatedTarget = 1;
       else calculatedTarget = 0; 
    } else {
            takingVertex = 0;
	    //calculatedTarget = (seg->getZ()<-30.29)?2:1;
      if (seg->getZ() <= -32.29) 
	      calculatedTarget = 2;
       else if (seg->getZ() > -28.29)
	      calculatedTarget = 1;
       else calculatedTarget = 0; 

    }

    if (realTarget==calculatedTarget)
	    targetFlag = calculatedTarget;
      else
	    targetFlag = -calculatedTarget;
    
  }  

  
  vars[0] = candidate.getMomentum();
  vars[1] = p;
  vars[2] = candidate.getMomentumError();
  vars[3] = valid;
  vars[4] = candidate.getMass();
  vars[5] = candidate.getXPull();
  vars[6] = parentTrack; //candidate.getPos1X();
  vars[7] = pmdc; //candidate.getPos1Y();
  vars[8] = candidate.getPos1Z();
  vars[9] = candidate.isAssigned()?1:0;
  vars[10] = theta;
  vars[11] = phi;
  vars[12] = pid;
  vars[13] = candidate.getDenominator();
  vars[14] = candidate.getSystem();
  vars[15] = targetFlag;
  vars[16] = takingVertex;
  fControl->Fill(vars);
}

HKickCandidateEvaluator *HKickTrackFSim::makeEvaluator(void) {
  //Creates a candidate evaluator. Factory method
  HKickCandidateEvaluator *eval = 0;
  if(fTofClusterMode){
    if (fTofClusters->getClass() == HTofClusterSim::Class())
      eval = new HKickCandidateEvaluatorSim(fKickPlane,fMatchPar,fKineCat);
    else {
      Warning("makeEvaluator","Unknown TOF data class. Using code for non simulated data");
      eval = new HKickCandidateEvaluator(fKickPlane,fMatchPar);
    }
  } else {
    if (fTofHits->getClass() == HTofHitSim::Class())
      eval = new HKickCandidateEvaluatorSim(fKickPlane,fMatchPar,fKineCat);
    else if (fTofHits->getClass() == HTofHitSim2::Class())
      eval = new HKickCandidateEvaluatorSim2(fKickPlane,fMatchPar,fKineCat);
    else {
      Warning("makeEvaluator","Unknown TOF data class. Using code for non simulated data");
      eval = new HKickCandidateEvaluator(fKickPlane,fMatchPar);
    }
  }
  if (fUseSimulatedTof) eval->setSimulatedTof();
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
  fCandidateOut=event->getCategory(catKickTrack);
  if (!fCandidateOut) {
    Int_t ssize[2]={6,100};
    fCandidateOut = new HMatrixCategory("HKickTrackSim",2,ssize,0.5);
    if (fCandidateOut) event->addCategory(catKickTrack,
					  fCandidateOut,
					  "Tracks");
    else {
      Error("init","Unable to setup output");
      return 0;
    }
  }

  return fCandidateOut;
}

void HKickTrackFSim::readMetaHits(HLocation &loc) {
  //Reads hits from MDC and TOF detectors
  HLocation hLoc;
  hLoc.set(3,0,0,0);
  HTofHitSim *tofHit=0;
  HShowerHit *shoHit=0;
  HTofClusterSim *tofCluster=0;
  HTofClusterSim *tofClHit=0;
  Int_t i=0;
  if (hasTof()) {
    fTofIter->Reset(); //FIXME: Remove ??
    fTofIter->gotoLocation(loc);
    i=0;
    if(fTofClusterMode){
      HLocation clLoc;
      clLoc.set(3,0,0,0);
      Int_t size_cl=0;
      fTofCIter->Reset();
      fTofCIter->gotoLocation(loc);
      while ((tofCluster=(HTofClusterSim *)fTofCIter->Next()) != 0) {
        if (!hasShower() || tofCluster->getModule() < 8) { //Outer TOF
          if((tofCluster->getTof() > ((HKickTrackFPar *)fKickPar)->getMinTof()) &&
             (tofCluster->getTof() < ((HKickTrackFPar *)fKickPar)->getMaxTof()) &&
             (tofCluster->getClusterSize() > 1)) {
            size_cl=tofCluster->getClusterSize();
            clLoc[0]=tofCluster->getSector();
            clLoc[1]=tofCluster->getModule();
            clLoc[2]=tofCluster->getCell();
            while((tofHit=(HTofHitSim *)fTofIter->Next()) != 0) {
              hLoc[0]=tofHit->getSector();
              hLoc[1]=tofHit->getModule();
              hLoc[2]=tofHit->getCell();
              if((hLoc[0]==clLoc[0]) && (hLoc[1]==clLoc[1]) && (hLoc[2]==clLoc[2])) break;
              if (!hasShower() || tofHit->getModule() < 8) { //Outer TOF
                if((tofHit->getTof() > ((HKickTrackFPar *)fKickPar)->getMinTof()) &&
                   (tofHit->getTof() < ((HKickTrackFPar *)fKickPar)->getMaxTof())) {
                  tofClHit=new HTofClusterSim(tofHit);
                  fOuterHitArray.AddAtAndExpand(tofClHit,i);
                  i++;
                }
              }
            }
            if(size_cl==2){   // to select only clusters with size=2
              fOuterHitArray.AddAtAndExpand(tofCluster,i);
              i++;
              tofClHit=new HTofClusterSim(tofHit);
              fOuterHitArray.AddAtAndExpand(tofClHit,i);
              i++;
            }
            for(Int_t j=0; j<(size_cl-1); j++){
              tofHit=(HTofHitSim *)fTofIter->Next();
              if(size_cl==2){   // to select only clusters with size=2
                tofClHit=new HTofClusterSim(tofHit);
                fOuterHitArray.AddAtAndExpand(tofClHit,i);
                i++;
              }
            }
            size_cl=0;
          }
        }
      }
      while ((tofHit=(HTofHitSim *)fTofIter->Next()) != 0) {
        if (!hasShower() || tofHit->getModule() < 8) { //Outer TOF
          if((tofHit->getTof() > ((HKickTrackFPar *)fKickPar)->getMinTof()) &&
             (tofHit->getTof() < ((HKickTrackFPar *)fKickPar)->getMaxTof())) {
            tofClHit=new HTofClusterSim(tofHit);
            fOuterHitArray.AddAtAndExpand(tofClHit,i);
            i++;
          }
        }
      }
    } else {
      while ( (tofHit=(HTofHitSim *)fTofIter->Next()) != 0) {
        if (!hasShower() || tofHit->getModule() < 8) { //Outer TOF
          if (tofHit->getTof() > ((HKickTrackFPar *)fKickPar)->getMinTof() &&
              tofHit->getTof() < ((HKickTrackFPar *)fKickPar)->getMaxTof() ) {
            fOuterHitArray.AddAtAndExpand(tofHit,i);
            i++;
          }
        }
      }
    }
    fNTofHits=i;

  } else fNTofHits=0;
  if (hasShower()) {
    fShowerIter->Reset(); //FIXME: remove ??
    while ( (shoHit=(HShowerHit *)fShowerIter->Next()) != 0) {
      if (shoHit->getSector()==fSectorLoc[0] && shoHit->getModule()==0) {
	fOuterHitArray.AddAtAndExpand(shoHit,i);
	i++;
      }
    }
    fNShowerHits=i - fNTofHits;
  } else fNShowerHits=0;
  fNTotalHits=fNTofHits + fNShowerHits;
}


ClassImp(HKickTrackFSim)
