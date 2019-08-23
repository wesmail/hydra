#include "hrttrackfindsim.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hrtsegmentref.h"
#include "hcategory.h"
#include "hgeantkine.h"
#include "hades.h"
#include "hevent.h"
#include "hgeantmdc.h"
#include "hrtmetasegsim.h"
#include <TNtuple.h>
#include <assert.h>
#include "hgeomvolume.h"
#include "hrtmdctrackiosim.h"
#include "hrtmetatrackio.h"

void HRtTrackEvaluatorSim::fillControl(TNtuple *control, Bool_t onlyMdc3) {
  Float_t valid;
  Int_t mostCommonTrack=0, mostCommonTrackTimes=0;
  Int_t totalTimes=0;
  Int_t innerTimes = 0, outerTimes = 0;
  Int_t parentTrack = 0;
  HGeantKine *geKine = 0;
  HMdcSegSim *innerSeg = dynamic_cast<HMdcSegSim *>(fInnerSeg);
  Float_t geThetaSec = 0;
  Float_t gePhiSec = 0;
  Int_t pid,tmp;

  if (onlyMdc3) {
    HMdcHitSim *outerSeg = dynamic_cast<HMdcHitSim *>(fOuterSeg->getSegment());

    if (innerSeg && outerSeg) {

      // Find common tracks
      mostCommonTrackTimes = 0;
      totalTimes = 0;
      for (Int_t i=0;i<innerSeg->getNTracks();i++) { 
	for (Int_t j=0;j<outerSeg->getNTracks();j++) 
	  if (innerSeg->getTrack(i) == outerSeg->getTrack(j)) {

	    //Common track found!
	    innerTimes = innerSeg->getNTimes(i);
	    outerTimes = outerSeg->getNTimes(j);
	    if (innerTimes + outerTimes > mostCommonTrackTimes) {
	      mostCommonTrack = innerSeg->getTrack(i);
	      mostCommonTrackTimes = innerTimes + outerTimes;
	    }
	  }
      }

      // If no common track found. Look for particles decaying between
      // inner and outer segments  ----> correlated noise
      for (Int_t i=0;i<outerSeg->getNTracks();i++) {
	geKine = (HGeantKine *)fCatKine->getObject(outerSeg->getTrack(i)-1);
	geKine->getParticle(tmp,pid);
	parentTrack = geKine->getParentTrack();
	for (Int_t j=0; j<innerSeg->getNTracks(); j++) {
	  if (innerSeg->getTrack(j) == parentTrack) {

	    //Correlated noise found
	    innerTimes = innerSeg->getNTimes(j);
	    outerTimes = outerSeg->getNTimes(i);
	    if (innerTimes + outerTimes > mostCommonTrackTimes) {
	      mostCommonTrack = innerSeg->getTrack(i);
	      mostCommonTrackTimes = 10 * (innerTimes + outerTimes);
	    }
	  }
	}
      }
      
      //Compute total times 
      for (Int_t i=0;i<innerSeg->getNTracks();i++)
	totalTimes += innerSeg->getNTimes(i);
      for (Int_t i=0;i<outerSeg->getNTracks();i++)
	totalTimes += outerSeg->getNTimes(i);

      // valid = 0               ---> bad combination.
      // valid > 0 && valid < 1  ---> good combination.
      // valid > 1               ---> correlated noise with valid/10 validity
      valid = float(mostCommonTrackTimes) / float(totalTimes);

      // Geant track information
      Int_t track = outerSeg->getTrack(0);
      geKine = 0;
      if (track<1) { 
	Error("fillControl","Invalid track %i %p",track,outerSeg); 
	outerSeg->Dump(); 
      }
      geKine = (HGeantKine *)fCatKine->getObject(track-1);

      // Get theta,phi from Geant Track in sector coordinates
      if (geKine) {
	HGeomVector vecGeMomentum;
	HGeomVector geAlpha0Sec;
	HGeomTransform &transSec = fSpecGeomPar->getSector(innerSeg->getSec())->getTransform();
	geKine->getMomentum(vecGeMomentum);
	vecGeMomentum /= vecGeMomentum.length();
	geAlpha0Sec = transSec.transTo(vecGeMomentum);
	geThetaSec = acos(geAlpha0Sec.getZ());
	gePhiSec = TMath::ATan2(geAlpha0Sec.getY(),geAlpha0Sec.getX());
      }

      //Obtain info of resolution of outer segment
      HGeantMdc *geMdc = 0;
      Float_t geX,geY,geTheta,gePhi,geTof,geMom;
      Float_t reX,reY,reTheta,rePhi;

      geKine->resetMdcIter();
      while ( (geMdc = (HGeantMdc *)geKine->nextMdcHit()) != 0) {
	if (geMdc->getLayer() == 6 && geMdc->getModule()==2) {
	  geMdc->getHit(geX,geY,geTof,geMom); //theta phi are placeholders.
	  geMdc->getIncidence(geTheta,gePhi);
	  if (gePhi>180) gePhi -= 360.;
	}
      }
      reX = outerSeg->getX();
      reY = outerSeg->getY();
      reTheta = outerSeg->getTheta();
      rePhi = outerSeg->getPhi();

      //Uncomment for debugging
      //cout << innerSeg->getNTracks() << endl;
      //cout << "-   " << innerSeg->getTrack(0) << endl;
      //cout << "--  " << outerSeg->getNTracks() << endl;
      //cout << "--- " << outerSeg->getTrack(0) << endl;

      control->Fill(fDistance,fDistanceKick,fAngle,fDeltaPhi,valid,fP,
		    geKine->getTotalMomentum(), geMom,
		    geThetaSec, gePhiSec, 
		    innerSeg->getTheta(), innerSeg->getPhi(),pid);

      fOutRes->Fill(valid,fDistanceKick,geX,geY,geTheta,gePhi,reX,reY,
		    reTheta,rePhi,geKine->getTotalMomentum(),geMom,fP);
    }
  } else {
    HMdcSegSim *outerSeg = dynamic_cast<HMdcSegSim *>(fOuterSeg->getSegment());

    if (innerSeg && outerSeg) {

      // Find common tracks
      mostCommonTrackTimes = 0;
      totalTimes = 0;
      for (Int_t i=0;i<innerSeg->getNTracks();i++) { 
	for (Int_t j=0;j<outerSeg->getNTracks();j++) 
	  if (innerSeg->getTrack(i) == outerSeg->getTrack(j)) {

	    //Common track found!
	    innerTimes = innerSeg->getNTimes(i);
	    outerTimes = outerSeg->getNTimes(j);
	    if (innerTimes + outerTimes > mostCommonTrackTimes) {
	      mostCommonTrack = innerSeg->getTrack(i);
	      mostCommonTrackTimes = innerTimes + outerTimes;
	    }
	  }
      }

      // If no common track found. Look for particles decaying between
      // inner and outer segments  ----> correlated noise
      for (Int_t i=0;i<outerSeg->getNTracks();i++) {
	geKine = (HGeantKine *)fCatKine->getObject(outerSeg->getTrack(i)-1);
	parentTrack = geKine->getParentTrack();
	for (Int_t j=0; j<innerSeg->getNTracks(); j++) {
	  if (innerSeg->getTrack(j) == parentTrack) {

	    //Correlated noise found
	    innerTimes = innerSeg->getNTimes(j);
	    outerTimes = outerSeg->getNTimes(i);
	    if (innerTimes + outerTimes > mostCommonTrackTimes) {
	      mostCommonTrack = innerSeg->getTrack(i);
	      mostCommonTrackTimes = 10 * (innerTimes + outerTimes);
	    }
	  }
	}
      }
      
      //Compute total times 
      for (Int_t i=0;i<innerSeg->getNTracks();i++)
	totalTimes += innerSeg->getNTimes(i);
      for (Int_t i=0;i<outerSeg->getNTracks();i++)
	totalTimes += outerSeg->getNTimes(i);

      // valid = 0               ---> bad combination.
      // valid > 0 && valid < 1  ---> good combination.
      // valid > 1               ---> correlated noise with valid/10 validity
      valid = float(mostCommonTrackTimes) / float(totalTimes);

      // Geant track information
      Int_t track = outerSeg->getTrack(0);
      geKine = 0;
      if (track<1) { 
	Error("fillControl","Invalid track %i %p",track,outerSeg); 
	outerSeg->Dump(); 
      }
      geKine = (HGeantKine *)fCatKine->getObject(track-1);

      // Get theta,phi from Geant Track in sector coordinates
      if (geKine) {
	HGeomVector vecGeMomentum;
	HGeomVector geAlpha0Sec;
	HGeomTransform &transSec = fSpecGeomPar->getSector(innerSeg->getSec())->getTransform();
	geKine->getMomentum(vecGeMomentum);
	vecGeMomentum /= vecGeMomentum.length();
	geAlpha0Sec = transSec.transTo(vecGeMomentum);
	geThetaSec = acos(geAlpha0Sec.getZ());
	gePhiSec = TMath::ATan2(geAlpha0Sec.getY(),geAlpha0Sec.getX());
      }

      //Obtain info of resolution of outer segment
      HGeantMdc *geMdc = 0;
      Float_t geX,geY,geTheta,gePhi,geTof,geMom;
      Float_t reTheta,rePhi;

      geKine->resetMdcIter();
      while ( (geMdc = (HGeantMdc *)geKine->nextMdcHit()) != 0) {
	if (geMdc->getLayer() == 6 && geMdc->getModule()==0) {
	  geMdc->getHit(geX,geY,geTof,geMom); //theta phi are placeholders.
	  geMdc->getIncidence(geTheta,gePhi);
	  if (gePhi>180) gePhi -= 360.;
	}
      }
      reTheta = outerSeg->getTheta();
      rePhi = outerSeg->getPhi();

      //Uncomment for debugging
      //cout << innerSeg->getNTracks() << endl;
      //cout << "-   " << innerSeg->getTrack(0) << endl;
      //cout << "--  " << outerSeg->getNTracks() << endl;
      //cout << "--- " << outerSeg->getTrack(0) << endl;
      //cout << "\n\nmom: " << 1/geMom << " = " << geMom << endl;
      control->Fill(fDistance,fDistanceKick,fAngle,fDeltaPhi,valid,fP,
		    geKine->getTotalMomentum(), geMom,
		    geThetaSec, gePhiSec, 
		    innerSeg->getTheta(), innerSeg->getPhi());
    }
  }
}

HRtTrackEvaluator *HRtTrackFindSim::makeEvaluator(void) {
  return new HRtTrackEvaluatorSim(fCatKine,fOutRes);
}

Bool_t HRtTrackFindSim::init(void) {
  fOutRes = new TNtuple("outres","outres",
			"valid:dkick:gex:gey:gethe:gephi:rex:rey:rethe:rephi:p:pgemdc:prec");
  fCatKine = gHades->getCurrentEvent()->getCategory(catGeantKine);
 
  if (!HRtTrackFind::init()) return kFALSE;

  return (fCatKine!=0);
}

HRtTrackIo *HRtTrackFindSim::makeTrackIo(void) {
  HRtTrackIo *io=0;
  switch (fMode) {
  case Mdc3Pos:
    io = new HRtMdcTrackIoSim(kTRUE);
    break;
  case Mdc34:
    io = new HRtMdcTrackIoSim(kFALSE);
    break;
  case Meta: 
    io = new HRtMetaTrackIo;
    break;
  default:
    Error ("init", "Invalid I/O mode");
  }; 
  return io;
}

Bool_t HRtTrackFindSim::finalize(void) {
  HRtTrackFind::finalize();
  if (fOutRes) {
    fOutRes->Write();
  }
  return kTRUE;
}


ClassImp(HRtTrackFindSim)
ClassImp(HRtTrackEvaluatorSim)
