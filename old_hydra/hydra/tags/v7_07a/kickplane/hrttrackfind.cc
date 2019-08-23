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
  fDoFit = kFALSE;
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

void HRtTrackFind::setMode(EMode mode) {
  if (fDoFit && mode!=Mdc34)
    Error("setMode","Cannot do fitting for any mode other \
	than Mdc34");
  else {
    fMode = mode;
  }
}

void HRtTrackFind::setFitting(Bool_t f=kTRUE) {
  if (f && fMode!=Mdc34) {
    Error("setFitting","Fitting can only be activated when \
	in Mdc34 mode");
  } else {
    fDoFit = f;
  }
}

Bool_t HRtTrackFind::reinit(void) {
  Bool_t result=kTRUE;

  if (fDoFit) 
    if (!fFitter.reinit()) result = kFALSE;

  if (!fEvaluator->reinit()) result = kFALSE;

  return result;
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

      if (fDoFit) {
	fCatHit = ev->getCategory(catMdcHit);
	if (!fCatHit) {
	  Error("init","catMdcHit not found");
	  return kFALSE;
	}

	if (fMode!=Mdc34) {
	  Error("init",
	      "Fitting can only be used with mode MDC34");
	  return kFALSE;
	}
      }

      fEvaluator->init();
      
      //FIXME: Read tracking parameters from rtdb
      fParam = new HRtTrackingPar;
      fParam->setParams (400, 500, .8);
    }
  }

  if (fDoFit) {
    if (!fFitter.init()) {
      Error("init","Fitter could not be initialized");
      return kFALSE;
    }
  }
  
  fControl =new TNtuple ("RtMatching", "matching",
			 "d:dKick:angle:dPhi:valid:pc:pr:pgemdc:geTheta:gePhi:recTheta:recPhi:pid:charge");
  
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

Int_t HRtTrackFind::execute (void) {
  // All combinations of segments before and after the 
  // magnet are performed
  //
  // For each combination some control variables are 
  // evaluated; if those control variables pass some cuts 
  // then we say we have found a track candidate.
  //
  // Last step the candidate is fitted and an additional cut 
  // on Chi2 of the fit is imposed.
  HMdcSeg *innerSeg;
  HRtSegmentRef *outerSeg;
  Int_t nOuter,nInner=0;
  enum HRtFitter::EFitResult fitResult = HRtFitter::kUnknown;
  Float_t rad2deg = 180./TMath::Pi();
  HRtVector p (kRtParameterDim);
  HRtVector pFit (kRtParameterDim);
  HRtMatrix cov (kRtParameterDim, kRtParameterDim);
  HRtVector m (kRtMeasurementDim);
  HRtMatrix W (kRtMeasurementDim, kRtMeasurementDim);	//FIXME: use the fact that this matrix is block diagonal.
  W.identity();
  for (int i=0;i<kRtMeasurementDim;i++)
    if (i&0x01) W(i,i)=1/(0.08*0.08); else W(i,i)=1/(0.16*0.16);
  Float_t chi2 = 0.;
  
  for (Int_t sector=0;sector<fMdc->getMaxSectors();sector++) {
    fTrackIo->setSector(sector);
    fLocInner[0] =  sector;
    
    nOuter = fTrackIo->readOuterSegments(fSegmentRefs);
    nInner = 0;

    fInSegIter->Reset();
    fInSegIter->gotoLocation(fLocInner);
 
    while ((innerSeg = (HMdcSeg *)fInSegIter->Next()) != 0) {
      fEvaluator->setInnerSeg(innerSeg);
      nInner++;

      for (Int_t outerSegN=0; outerSegN<nOuter; outerSegN++) {
	outerSeg = (HRtSegmentRef *)
	  fSegmentRefs.At(outerSegN);
	fEvaluator->setOuterSeg(outerSeg);
	fEvaluator->evaluate(fMode==Mdc3Pos);
	//cout << "\n\nCombination found!" << endl;
	fEvaluator->fillControl (fControl, fMode==Mdc3Pos);
	if (fEvaluator->isGoodCombination () || fIgnoreCuts) {
	  fEvaluator->fillTrackParameters (p);	
	  //First estimation

	  //Call the fitting      
	  if (fDoFit) {
	    pFit(0)=p(0)*1000.;
	    pFit(1)=p(1);
	    pFit(2)=p(2);
	    pFit(3)=p(3)*rad2deg;
	    pFit(4)=p(4)*rad2deg;
	    
	    //fFitter.setInitialValue(pFit);
	    fEvaluator->extractMeasurements(m,W);
	    fitResult = fFitter.fit(m,W,pFit,fEvaluator->getCharge(),
		cov,chi2); 

	    if (fitResult==HRtFitter::kConverged) {
	      p(0)=pFit(0)/1000.; //To MeV
	      p(1) = pFit(1);
	      p(2) = pFit(2);
	      p(3)=pFit(3)/rad2deg;
	      p(4)=pFit(4)/rad2deg;
	    }
	  }

	  //if (chi2 < fParam->getMaxChi2 ()) {
	    //Track is finally accepted.
	    fTrackIo->acceptTrack (p, cov, chi2, 
		innerSeg,outerSeg,fEvaluator,fitResult);
	  //}
	}
	  
      }	// Loop on outer segments
    }		// Loop on inner segments
  }			//Loop on sectors
  return 0;
}
  


ClassImp(HRtTrackFind) 

