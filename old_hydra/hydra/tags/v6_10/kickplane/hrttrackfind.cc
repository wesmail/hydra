//*-- Author : M.Sanchez & D. Magestro (21/06/2001)
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

///////////////////////////////////////////////////
//HRtTrackFind
//
// Uses the kickplane to do the matching of segments before and after
//the magnet finding track candidates. Each track candidate is then
//fitted using reference trajectories the track parameters
////////////////////////////////////////////////////

HRtMdcTrackIo::HRtMdcTrackIo (Bool_t mode)
{
  fDispY = fDispZ = 0.;
  fMdc3Mode = mode;
  fMaxChi2 = 1e6;
}

HRtMdcTrackIo::~HRtMdcTrackIo(void) {
}

Int_t HRtMdcTrackIo::readOuterSegments(TClonesArray &segArray) {
  return (fMdc3Mode)?readOuterHits(segArray):readOuterSegs(segArray);
}

void HRtMdcTrackIo::acceptTrack (HRtVector & p, HRtMatrix & cov, Float_t chi2,
				 HMdcSeg * innerSeg, HRtSegmentRef * outerSeg,
				 HRtTrackEvaluator *evaluator){
  // Fills the track parameters in the output category and some statistics.
  HRtMdcTrk *track = 0;
  TObject *slot = 0;
  
  slot = fCatTrk->getNewSlot (fLocSector);
  if (slot) {
    track = new (slot) HRtMdcTrk;
    //FIXME: The covariance matrix needs to be transformed from 1/p to p
    track->setP (1 / p (0), cov (0, 0));
    track->setZ (p (1), cov (1, 1));
    track->setR (p (2), cov (2, 2));
    track->setPhi (p (3), cov (3, 3));
    track->setTheta (p (4), cov (4, 4));
    track->setOuterPos (outerSeg->getR());
    track->setOuterDir (evaluator->getDir());	//outerSeg->getAlpha());
    track->setLength (evaluator->calcEvaluatedLength());
    track->setCharge (evaluator->getCharge());
    track->setQuality (evaluator->getDistanceKick(),evaluator->getDistance());
    track->setLeverArm (evaluator->getLeverArm());
  }
}

Int_t HRtMdcTrackIo::readOuterHits (TClonesArray &segments) {
  // Reads hits from the input category and which correspond to
  //chamber 3. The hits are stored transformed into
  //HRtSegmentRef objects and stored in fSegmentRefs
  //
  // The number of hits read is returned.  
  Int_t nHits = 0;

  if (fOutHitIter) {
    HMdcHit *hit = 0;
    HGeomVector rLocal, alphaLocal;
    HRtSegmentRef *ref = 0;
    
    fLocOuter[1] = 2;	//Location for MDC3
    segments.Clear ();
    fOutHitIter->Reset ();
    fOutHitIter->gotoLocation (fLocOuter);
    
    while ((hit = (HMdcHit *) fOutHitIter->Next ()) != 0) {
      if (hit->getChi2 () > fMaxChi2)
	continue;
      ref = new (segments[nHits]) HRtSegmentRef;
      if (!ref) Error ("readOuterSegments","Memory exhausted");
      nHits++;
      
      HGeomVector & r = ref->getR ();
      HGeomVector & alpha = ref->getAlpha ();
      
      //Coordinate transformation to sector system
      HModGeomPar *mod =
	fMdcGeometry->getModule (hit->getSector (),
				 hit->getModule ());
      if (mod) {
	HGeomTransform & transMod = mod->getLabTransform ();
	HGeomTransform & transSec = fSpecGeometry->getSector(hit->
							     getSector())->
	  getTransform ();
	HGeomTransform trans (transMod);
	trans.transTo (transSec);
	
	rLocal.setX (hit->getX ());
	rLocal.setY (hit->getY ());
	rLocal.setZ (0.);
	alphaLocal.setX (hit->getXDir ());
	alphaLocal.setY (hit->getYDir ());
	alphaLocal.setZ (TMath::Sqrt (1. - hit->getXDir () * hit->getXDir () -
				      hit->getYDir () * hit->getYDir ()));

	r = trans.transFrom (rLocal);
	r.setY (r.getY () + fDispY);
	r.setZ (r.getZ () + fDispZ);
	alpha = trans.getRotMatrix () * alphaLocal;
      }
      else
	Warning ("readOuterHits", "Module %i unknown",
		 hit->getModule ());
      
      ref->setSegment (hit);
    }
    
  } else {
    Warning ("readOuterHits",
	     "Tried to read MDC3 hits but catMdcHit is not there!");
  }
  
  return nHits;
}

Int_t HRtMdcTrackIo::readOuterSegs(TClonesArray &segments) {
  // Reads segments from the input category and which correspond to
  //chambers 3 and 4. The segments are stored transformed into
  //HRtSegmentRef objects and stored in fSegmentRefs
  //
  // The number of segments read is returned.
  Int_t nSegments = 0;
  HMdcSeg *seg = 0;
  HGeomVector rLocal, alphaLocal;
  HRtSegmentRef *ref = 0;
  Float_t theta, phi;
  Double_t pi2 = TMath::Pi () / 2.;
  
  fLocOuter[1] = 1;
  segments.Clear ();
  fOutSegIter->Reset ();
  fOutSegIter->gotoLocation (fLocOuter);
  while ((seg = (HMdcSeg *) fOutSegIter->Next ()) != 0) {
    ref = new (segments[nSegments]) HRtSegmentRef;
    if (!ref) Error ("readOuterSegments", "Memory exhausted");
    nSegments++;
    
    HGeomVector & r = ref->getR ();
    HGeomVector & alpha = ref->getAlpha ();
    
    theta = seg->getTheta ();
    phi = seg->getPhi ();
    
    // Obtain normal representation of the straight line.
    r.setZ (seg->getZ ());
    r.setX (seg->getR () * TMath::Cos (phi + pi2));
    r.setY (seg->getR () * TMath::Sin (phi + pi2));
    
    alpha.setZ (TMath::Cos (theta));
    alpha.setY (TMath::Sin (theta) * TMath::Sin (phi));
    alpha.setX (TMath::Sin (theta) * TMath::Cos (phi));
    
    ref->setSegment (seg);
  }
  
  return nSegments;
}

Bool_t
HRtMdcTrackIo::init (HEvent * ev, HRuntimeDb * rtdb)
{
  fCatSeg = ev->getCategory (catMdcSeg);
  if (!fCatSeg)
    {
      Error ("init", "catMdcSeg not found");
      return kFALSE;
    }
  fOutSegIter = dynamic_cast < HIterator * >(fCatSeg->MakeIterator ());

  fCatHit = ev->getCategory (catMdcHit);
  if (fCatHit)
    fOutHitIter =dynamic_cast <HIterator * >(fCatHit->MakeIterator ());
  else
    {
      if (fMdc3Mode)
	{
	  Error ("init",
		 "cannot work in MDC3 mode without catMdcHit");
	  return kFALSE;
	}
    }

  fCatTrk = ev->getCategory (catMdcTrack);
  if (!fCatTrk)
    {
      Int_t sizes[2] = { 6, 100 };
      fCatTrk = new HMatrixCategory ("HRtMdcTrk", 2, sizes, .3);
      if (fCatTrk)
	ev->addCategory (catMdcTrack, fCatTrk, "Mdc");
      else
	{
	  Error ("init", "Not possible to define catMdcTrack");
	  return kFALSE;
	}
    }
  else
    {			//Check class name
      if (!fCatTrk->getClass ()->InheritsFrom ("HRtMdcTrk"))
	{
	  Error ("init",
		 "Reft needs the catMdcTrack category to "
		 "contain HRtMdcTrk objects");
	  return kFALSE;
	}
    }

  fMdcGeometry = (HMdcGeomPar *) rtdb->getContainer ("MdcGeomPar");
  fSpecGeometry = (HSpecGeomPar *) rtdb->getContainer ("SpecGeomPar");

  fLocOuter.set (2, 0, 1);
  fLocSector.set (1, 0);
  return kTRUE;
}


/*******************************************************************/

HRtTrackFind::HRtTrackFind (Text_t name[], Text_t title[]):
  HReconstructor (name, title), fSegmentRefs ("HRtSegmentRef", 200)
{
  fEvaluator = 0;
  fMode = Mdc3Pos;

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

Bool_t
HRtTrackFind::init (void)
{
  // Initialization
  if (!fEvaluator) fEvaluator = makeEvaluator ();

  switch (fMode) {
  case Mdc3Pos:
    fTrackIo = new HRtMdcTrackIo (kTRUE);
    break;
  case Mdc34:
    fTrackIo = new HRtMdcTrackIo (kFALSE);
    break;
  case Meta: 
    fTrackIo = new HRtMetaTrackIo;
    break;
  default:
    Error ("init", "Invalid I/O mode");
  };

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
      
      HKickTask::setKickParIo (rtdb);
      fKickPlane = (HKickPlane2 *) rtdb->getContainer ("KickPlane2MDC3");
      fMatchPar = (HKickMatchPar *) rtdb->getContainer ("KickMatchParMDC3");
      fSpecGeometry = (HSpecGeomPar *) rtdb->getContainer ("SpecGeomPar");
      
      //FIXME: Read tracking parameters from rtdb
      fParam = new HRtTrackingPar;
      fParam->setParams (40, 700, 0.6);
      
      fEvaluator->setKickPlane (fKickPlane);
      fEvaluator->setMatchPar (fMatchPar);
      fEvaluator->setTrackingPar (fParam);
      fEvaluator->setSpecGeomPar (fSpecGeometry);
    }
  }
  
  fControl =new TNtuple ("RtMatching", "matching",
			 "d:dKick:angle:dPhi:valid:pc:pr:xc:yc:zc");
  
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
  Int_t nOuter;
  HRtVector p (kRtParameterDim);
  HRtMatrix cov (kRtParameterDim, kRtParameterDim);
  HRtVector m (2 * kRtDetectionPlanes);
  HRtMatrix W (2 * kRtDetectionPlanes, 2 * kRtDetectionPlanes);	//FIXME: use the fact that this matrix is block diagonal.
  Float_t chi2 = 0.;

  for (Int_t sector = 0; sector < fMdc->getMaxSectors (); sector++) {
    fTrackIo->setSector(sector);
    fLocInner[0] =  sector;
    
    nOuter = fTrackIo->readOuterSegments(fSegmentRefs);

    fInSegIter->Reset();
    fInSegIter->gotoLocation(fLocInner);

    while ((innerSeg = (HMdcSeg *) fInSegIter->Next ()) != 0) {
      for (Int_t outerSegN = 0; outerSegN < nOuter; outerSegN++) {
	outerSeg = (HRtSegmentRef *) fSegmentRefs.At (outerSegN);
	
	fEvaluator->evaluate(innerSeg, outerSeg, fMode==Mdc3Pos);
	fEvaluator->fillControl (fControl, fMode==Mdc3Pos);
	if (fEvaluator->isGoodCombination ()) {
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
  
//------------------------------- HRtTrackEvaluator ---------------------
void
HRtTrackEvaluator::evaluate (HMdcSeg * inner, HRtSegmentRef * outer,
			     Bool_t onlyMdc3)
{
  // evaluates a combination of two segments (inner, outer). That means
  //filling variables that later can be used to check if this combination
  //corresponds to a real particle or not.
  Float_t pi2 = TMath::Pi () / 2.;
  HGeomVector r1, alpha1;
  HGeomVector & r2 = outer->getR ();
  HGeomVector & alpha2 = outer->getAlpha ();
  HGeomVector rKick1, rKick2;
  Float_t g1, g2, deltaG;
  Float_t f1, f2;
  Float_t theta, phi;
  Float_t predictedSinPhi;
  Float_t err;

  // Obtain normal representation of the straight line for inner segment
  theta = inner->getTheta ();
  phi = inner->getPhi ();
  r1.setZ (inner->getZ ());
  r1.setX (inner->getR () * TMath::Cos (phi + pi2));
  r1.setY (inner->getR () * TMath::Sin (phi + pi2));
  alpha1.setZ (TMath::Cos (theta));
  alpha1.setY (TMath::Sin (theta) * TMath::Sin (phi));
  alpha1.setX (TMath::Sin (theta) * TMath::Cos (phi));

  // Intersection with the kickplane
  fKickPlane->calcIntersection (r1, alpha1, rKick1);
  fKickPlane->calcIntersection (r2, alpha2, rKick2);

  // Point of closest approach between tracks
  fVertexFitter.reset ();
  fVertexFitter.addLine (r1, alpha1);
  fVertexFitter.addLine (r2, alpha2);
  fVertexFitter.getVertex (fRClosest);

  // Kickplane stuff  
  if (onlyMdc3)
    {
      fDir = r2 - rKick1;
      fLeverArm = fDir.length ();
      fDir /= fLeverArm;
      fAngle = TMath::ACos (alpha1.scalarProduct (fDir));
    }
  else
    {
#warning "Temporal!!!!!!"
      fDir = r2 - rKick1;
      fLeverArm = fDir.length ();
      fDir /= fLeverArm;
      fAngle = TMath::ACos (alpha1.scalarProduct (fDir));
      //     fDir = alpha2; //FIXME: Optimize this!!
      //     fLeverArm = (r2-rKick1).length();  //Use dist btw MDCs or MDC width
      //     fAngle = TMath::ACos(alpha1.scalarProduct(alpha2));
    }
#warning "Chequear si calculo de polaridad es correcto"
  g1 = TMath::ATan2 (alpha1.getZ (), alpha1.getY ());
  g2 = TMath::ATan2 (alpha2.getZ (), alpha2.getY ());
  deltaG = g2 - g1;
  fPolarity = (deltaG > 0) ? 1 : -1;
  fP = fKickPlane->getP (rKick1, 2 * sin ((fAngle) / 2.), fPolarity);

  // Calculate control variables
  fDistance =
    TMath::Abs ((r1 - r2).
		scalarProduct (alpha1.vectorProduct (alpha2)));
  fDistanceKick = fKickPlane->distanceTo (fRClosest);
  f1 = TMath::ATan2 (alpha1.getX (), alpha1.getZ ());
  f2 = TMath::ATan2 (fDir.getX (), fDir.getZ ());
  predictedSinPhi =
    fMatchPar->getDeflection (rKick1, fP, err, fPolarity);
  fDeltaPhi = sin ((f2 - f1) / 2.) - predictedSinPhi;
  //  fAngle = deltaG; //FIXME: Check if sin(g2-g1) is better

  // Remember used segments
  fInnerSeg = inner;
  fOuterSeg = outer;
  fRClosest = r2;
  //  fRClosest.setZ( rKick1.getZ() ); //(fRClosest - rKick1).length() );
  //   fRClosest.setX( rKick1.getX() );
  //   fRClosest.setY( rKick1.getY() );

  //  fRClosest.setY(fRClosest.getY() - rKick1.getY());
}

Float_t
HRtTrackEvaluator::calcEvaluatedLength (void)
{
  //FIXME: Do average over targets
  Float_t length;
  const HGeomVector & target =
    fSpecGeomPar->getTarget (0)->getTransform ().
    getTransVector ();
  length = (fRClosest - target).length ();
  length += (fOuterSeg->getR () - fRClosest).length ();
  return length;
}

Bool_t
HRtTrackEvaluator::isGoodCombination (void)
{
  // Returns true or false depending on the latest evaluated combination
  //of segments being good or not.
  Bool_t r = kFALSE;

  r = (fDistanceKick < fParam->getMaxDistance ()) &&
    (fDeltaPhi < fParam->getMaxDeltaPhi ()) &&
    (fDistance < fParam->getMaxChi2 ());

  return r;
}

void HRtTrackEvaluator::fillTrackParameters (HRtVector & p) {
  // Makes a first estimation of the track parameters for the latest 
  //evaluated segment combination. The result is stored in the vector p
  p (0) = 1 / fP;
  p (1) = fInnerSeg->getZ ();
  p (2) = fInnerSeg->getR ();
  p (3) = fInnerSeg->getPhi ();
  p (4) = fInnerSeg->getTheta ();
}

void
HRtTrackEvaluator::fillControl (TNtuple * control, Bool_t onlyMdc3)
{
  // Fills ntuple with control variables
  if (onlyMdc3)
    {
      control->Fill (fDistance, fDistanceKick, fAngle, fDeltaPhi, 0,
		     fP, 0, fRClosest.getX (), fRClosest.getY (),
		     fRClosest.getZ ());
    }
  else
    {
      control->Fill (fDistance, fDistanceKick, fDeltaPhi, 0, fP, 0,
		     fRClosest.getX (), fRClosest.getY (),
		     fRClosest.getZ ());
    }
}

ClassImp(HRtMetaTrackIo)
ClassImp(HRtMdcTrackIo)
ClassImp(HRtTrackIo)
ClassImp(HRtTrackFind) 
ClassImp(HRtTrackEvaluator)
