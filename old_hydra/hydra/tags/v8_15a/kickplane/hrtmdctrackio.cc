#include "hrtmdctrackio.h"
#include "hrtmdctrk.h"
#include "hiterator.h"
#include "hgeomvolume.h"
#include "hmdcdef.h"
#include "hevent.h"
#include "hruntimedb.h"
#include "hmatrixcategory.h"



// I/O from MDC
HRtMdcTrackIo::HRtMdcTrackIo (Bool_t mode)
{
  fDispY = fDispZ = 0.;
  fMdc3Mode = mode;
  fMaxChi2 = 1e6;
  fOutputClassName = "HRtMdcTrk";
}

HRtMdcTrackIo::~HRtMdcTrackIo(void) {
}

Int_t HRtMdcTrackIo::readOuterSegments(TClonesArray &segArray) {
  return (fMdc3Mode)?readOuterHits(segArray):readOuterSegs(segArray);
}

void HRtMdcTrackIo::acceptTrack (HRtVector & p, HRtMatrix & cov, Float_t chi2,
				 HMdcSeg * innerSeg, HRtSegmentRef * outerSeg,
				 HRtTrackEvaluator *evaluator,
				 HRtFitter::EFitResult fitResult){
  // Fills the track parameters in the output category and some statistics.
  HRtMdcTrk *track = 0;
  TObject *slot = 0;
  slot = fCatTrk->getNewSlot (fLocSector);
  if (slot) {
    track = new (slot) HRtMdcTrk;
    //FIXME: The covariance matrix needs to be transformed from 1/p to p
    track->setSector(fLocSector[0]);
    track->setP (1 / p (0), cov (0, 0));
    track->setZ (p (1), cov (1, 1));
    track->setR (p (2), cov (2, 2));
    track->setPhi (p (3), cov (3, 3));
    track->setTheta (p (4), cov (4, 4));
    track->setOuterPos (outerSeg->getR());
    track->setOuterDir (evaluator->getDir());	//outerSeg->getAlpha());
    track->setLength (evaluator->calcEvaluatedLength());
    track->setCharge (evaluator->getCharge());
    track->setQuality (evaluator->getDistanceKick(),
		       evaluator->getDistance(),
		       evaluator->getDeltaPhi());
    track->setLeverArm (evaluator->getLeverArm());
    track->setFitResult((fitResult));
  }
}

// void HRtMdcTrackIoSim::acceptTrack(HRtVector & p, HRtMatrix & cov, 
// 				   Float_t chi2, HMdcSeg * innerSeg, 
// 				   HRtSegmentRef * outerSeg,
// 				   HRtTrackEvaluator *evaluator){
//   // Fills the track parameters in the output category and some statistics.
//   HRtMdcTrk *track = 0;
//   TObject *slot = 0;
  
//   slot = fCatTrk->getNewSlot (fLocSector);
//   if (slot) {
//     track = new (slot) HRtMdcTrk;
//     //FIXME: The covariance matrix needs to be transformed from 1/p to p
//     track->setP (1 / p (0), cov (0, 0));
//     track->setZ (p (1), cov (1, 1));
//     track->setR (p (2), cov (2, 2));
//     track->setPhi (p (3), cov (3, 3));
//     track->setTheta (p (4), cov (4, 4));
//     track->setOuterPos (outerSeg->getR());
//     track->setOuterDir (evaluator->getDir());	//outerSeg->getAlpha());
//     track->setLength (evaluator->calcEvaluatedLength());
//     track->setCharge (evaluator->getCharge());
//     track->setQuality (evaluator->getDistanceKick(),evaluator->getDistance());
//     track->setLeverArm (evaluator->getLeverArm());
//   }  
// }

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

	//Transform covariance matrix according to coord. transformation
	HSymMat &hitCov = hit->getCovariance(); //5x5 covariance matrix
		//with variables x,y,xdir,ydir,offset
	HSymMat &cov = ref->getCovariance(); //Destination covariance matrix

	Float_t sx = alphaLocal.getX() / alphaLocal.getZ();
	Float_t sy = alphaLocal.getY() / alphaLocal.getZ();
	//Float_t x0 = r.getX() - r.getZ() * sx;
	//Float_t y0 = r.getY() - r.getZ() * sy;
	//const HGeomRotation &rot = trans.getRotMatrix();
	HGeomRotation rot;
	Float_t d_x0_x = rot.getElement(0,0) - sx * rot.getElement(2,0);
	Float_t d_x0_y = rot.getElement(0,1) - sx * rot.getElement(2,1);
	Float_t d_y0_x = rot.getElement(1,0) - sy * rot.getElement(2,0);
	Float_t d_y0_y = rot.getElement(1,1) - sy * rot.getElement(2,1);
	Float_t d_sx_x = 0;
	Float_t d_sx_y = 0;
	Float_t d_sy_x = 0;
	Float_t d_sy_y = 0;
	Float_t d_az_ax = rot.getElement(2,0) - sx * rot.getElement(2,2);
	Float_t d_az_ay = rot.getElement(2,1) - sy * rot.getElement(2,2);
	Float_t d_ax_ax = rot.getElement(0,0) - sx * rot.getElement(0,2);
	Float_t d_ax_ay = rot.getElement(0,1) - sy * rot.getElement(0,2);
	Float_t d_ay_ax = rot.getElement(1,0) - sx * rot.getElement(1,2);
	Float_t d_ay_ay = rot.getElement(1,1) - sy * rot.getElement(1,2);
	Float_t az_2 = alpha.getZ() * alpha.getZ();
	Float_t d_sx_ax = (d_ax_ax*alpha.getZ() - d_az_ax*alpha.getX()) / az_2;
	Float_t d_sx_ay = (d_ax_ay*alpha.getZ() - d_az_ay*alpha.getX()) / az_2;
	Float_t d_sy_ax = (d_ay_ax*alpha.getZ() - d_az_ax*alpha.getY()) / az_2;
	Float_t d_sy_ay = (d_ay_ay*alpha.getZ() - alpha.getY()*d_az_ay) / az_2;
	Float_t d_x0_ax = -r.getZ() * d_sx_ax;
	Float_t d_x0_ay = -r.getZ() * d_sx_ay;
	Float_t d_y0_ax = -r.getZ() * d_sy_ax;
	Float_t d_y0_ay = -r.getZ() * d_sy_ay;
	Float_t derivatives[4*5] = { d_x0_x, d_x0_y, d_x0_ax, d_x0_ay, 0,
	                             d_y0_x, d_y0_y, d_y0_ax, d_y0_ay, 0,
				     d_sx_x, d_sx_y, d_sx_ax, d_sx_ay, 0,
				     d_sy_x, d_sy_y, d_sy_ax, d_sy_ay, 0};
	//printf("-- Derivatives matrix\n");
	//for (int ii=0;ii<4;ii++) {
	  //for (int jj=0;jj<5;jj++) derivatives[ii*5+jj]*=0.00001; //printf("%11.4g ",derivatives[ii*5+jj]);
	  //printf("\n");
	//}
	cov.transform(hitCov,derivatives);
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
  // Reads segments from the input category and which 
  // correspond to chambers 3 and 4. The segments are stored 
  // transformed into HRtSegmentRef objects and stored in 
  // fSegmentRefs
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

#warning "readOuterSegments Covariance matrix not generated");
    
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
      fCatTrk = new HMatrixCategory (const_cast<char *>(fOutputClassName.Data()), 2, sizes, .3);
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
      if (!fCatTrk->getClass ()->InheritsFrom (fOutputClassName.Data()))
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

ClassImp(HRtMdcTrackIo)
