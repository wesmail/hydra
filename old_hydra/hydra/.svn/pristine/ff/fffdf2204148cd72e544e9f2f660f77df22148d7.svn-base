//*-- Author : M.Sanchez (21/06/2001)
//*-- Modified : M. Sanchez (22/06/2001) 
#include "hrttrackfind.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hiterator.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "hrttrackingpar.h"
#include "hmdcseg.h"
#include "hrtsegmentref.h"
#include "hmdcdetector.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hmdcdef.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrtmdctrk.h"
#include "hmdchit.h"
#include "hgeomvector.h"
#include "hgeomtransform.h"
#include "hmdcgeompar.h"
#include "hspecgeompar.h"
#include "hgeomvolume.h"
#include "hkicktask.h"
#include <TMath.h>
#include "kickdef.h"
#include "hrtmetatrackio.h"
#include "hrtmdctrackio.h"

///////////////////////////////////////////////////
//HRtTrackFind
//
// Uses the kickplane to do the matching of segments before and after
//the magnet finding track candidates. Each track candidate is then
//fitted using reference trajectories the track parameters
////////////////////////////////////////////////////





HRtTrackFind::HRtTrackFind (Text_t name[], Text_t title[]):
  HReconstructor (name, title), fSegmentRefs ("HRtSegmentRef", 200)
{
  fEvaluator = 0;
  fMode = Mdc3Pos;
  fTrackIo = 0;
  fIgnoreCuts = kFALSE;
}

HRtTrackFind::~HRtTrackFind (void)
{
  delete fEvaluator;
}

HRtTrackEvaluator *
HRtTrackFind::makeEvaluator (void)
{
  return new HRtTrackEvaluator;
}

HRtTrackIo *HRtTrackFind::makeTrackIo(void) {
  HRtTrackIo *io=0;
  switch (fMode) {
  case Mdc3Pos:
    io = new HRtMdcTrackIo (kTRUE);
    break;
  case Mdc34:
    io = new HRtMdcTrackIo (kFALSE);
    break;
  case Meta: 
    io = new HRtMetaTrackIo;
    break;
  default:
    Error ("init", "Invalid I/O mode");
  }; 
  return io;
}

Bool_t HRtTrackFind::init (void) {
  // Initialization
  if (!fEvaluator) fEvaluator = makeEvaluator ();
  if (!fTrackIo) fTrackIo = makeTrackIo();

  if (gHades) {
    HEvent *ev = gHades->getCurrentEvent ();
    HRuntimeDb *rtdb = gHades->getRuntimeDb ();
    fTrackIo->init (ev, rtdb);
    fMdc = gHades->getSetup ()->getDetector ("Mdc");
    if (fMdc) {
      fCatSeg = ev->getCategory (catMdcSeg);
      if (!fCatSeg) {
	Error ("init", "catMdcSeg not found");
	return kFALSE;
      }
      fInSegIter = dynamic_cast <HIterator * >(fCatSeg->MakeIterator ());

      fEvaluator->init();
      
      //FIXME: Read tracking parameters from rtdb
      fParam = new HRtTrackingPar;
      fParam->setParams (400, 500, .8);
    }
  }
  
  fControl =new TNtuple ("RtMatching", "matching",
			 "d:dKick:angle:dPhi:valid:pc:pr:pgemdc:geTheta:gePhi:recTheta:recPhi");
  
  fLocInner.set (2, 0, 0);
  
  return kTRUE;
}

Bool_t
HRtTrackFind::finalize (void)
{
  // Writes the control ntuple if any
  if (fControl)
    {
      TFile *file = gHades->getOutputFile ();
      if (file)
	file->cd ();
      fControl->Write ();
    }

  return kTRUE;
}





void
HRtTrackFind::extractMeasurements (HMdcSeg * inner, HRtSegmentRef * outer,
				   HRtVector & m, HRtMatrix & w)
{
  // Extracts the relevant measurements from segments inner and outer.
  // This information is the x,y coordinates of the track on each detection
  //plane.
  //
  // The measurements are stored in the vector m and the inverse of the
  //covariance matrix is stored in w

}



Int_t HRtTrackFind::execute (void) {
  // All combinations of segments before and after the magnet are performed
  //
  // For each combination some control variables are evaluated; if 
  //those control variables pass some cuts then we say we have found a 
  //track candidate.
  //
  // Last step the candidate is fitted and an additional cut on Chi2 of the
  //fit is imposed.
  HMdcSeg *innerSeg;
  HRtSegmentRef *outerSeg;
  Int_t nOuter,nInner=0;
  HRtVector p (kRtParameterDim);
  HRtMatrix cov (kRtParameterDim, kRtParameterDim);
  HRtVector m (2 * kRtDetectionPlanes);
  HRtMatrix W (2 * kRtDetectionPlanes, 2 * kRtDetectionPlanes);	//FIXME: use the fact that this matrix is block diagonal.
  Float_t chi2 = 0.;

  for (Int_t sector = 0; sector < fMdc->getMaxSectors (); sector++) {
    fTrackIo->setSector(sector);
    fLocInner[0] =  sector;
    
    nOuter = fTrackIo->readOuterSegments(fSegmentRefs);
    nInner = 0;

    fInSegIter->Reset();
    fInSegIter->gotoLocation(fLocInner);
 
    while ((innerSeg = (HMdcSeg *) fInSegIter->Next ()) != 0) {
       nInner++;
      for (Int_t outerSegN = 0; outerSegN < nOuter; outerSegN++) {
	outerSeg = (HRtSegmentRef *) fSegmentRefs.At (outerSegN);
	
	fEvaluator->evaluate(innerSeg, outerSeg, fMode==Mdc3Pos);
	fEvaluator->fillControl (fControl, fMode==Mdc3Pos);
	if (fEvaluator->isGoodCombination () || fIgnoreCuts) {
	  fEvaluator->fillTrackParameters (p);	//First estimation

	  //Call the fitting      
	  //        fFitter.setInitialValue(p);
	  //        extractMeasurements(innerSeg,outerSeg,m,W);
	  //        fFitter.fit(m,W,p,cov,chi2); 
	  
	  if (chi2 < fParam->getMaxChi2 ()) {
	    //Track is finally accepted.
	    fTrackIo->acceptTrack (p, cov, chi2, innerSeg,outerSeg,fEvaluator);
	  }
	}
	  
      }	// Loop on outer segments
    }		// Loop on inner segments
  }			//Loop on sectors
  return 0;
}
  


ClassImp(HRtTrackFind) 

