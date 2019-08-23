#include "hrttrackevaluator.h"
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
#include "hades.h"
#include "hruntimedb.h"
#include "hrtmatchingpar.h"

//------------------------------- HRtTrackEvaluator ---------------------
Bool_t HRtTrackEvaluator::init(void) {
  HRuntimeDb *rtdb = Hades::instance()->getRuntimeDb();
  HKickTask::setKickParIo (rtdb);
  fKickPlane = (HKickPlane2 *) rtdb->getContainer ("KickPlane2MDC3");
  fMatchPar = (HKickMatchPar *) rtdb->getContainer ("KickMatchParMDC3");
  fSpecGeomPar = (HSpecGeomPar *) rtdb->getContainer ("SpecGeomPar");

  fMdcMatchingPar = (HRtMatchingPar *)rtdb->getContainer("RtMatchingParMDC3");

  return kTRUE;
}


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
  if (onlyMdc3) {
      fDir = r2 - rKick1;
      fLeverArm = fDir.length ();
      fDir /= fLeverArm;
      fAngle = TMath::ACos (alpha1.scalarProduct (fDir));
  } else {
    fDir = alpha2; //FIXME: Optimize this!!
    fLeverArm = (r2-rKick1).length();  //Use dist btw MDCs or MDC width
    fAngle = TMath::ACos(alpha1.scalarProduct(alpha2));
  }
#warning "Chequear si calculo de polaridad es correcto"
  g1 = TMath::ATan2 (alpha1.getZ (), alpha1.getY ());
  g2 = TMath::ATan2 (alpha2.getZ (), alpha2.getY ());
  deltaG = g2 - g1;
  fPolarity = (deltaG > 0) ? 1 : -1;
  fP = fKickPlane->getP (rKick1, 2 * sin ((fAngle) / 2.), fPolarity);

  // Calculate control variables
//  Float_t itanphi = 0.;
  fDistance =
    TMath::Abs ((r1 - r2).
		scalarProduct (alpha1.vectorProduct (alpha2)));
  fDistanceKick = fKickPlane->distanceTo (fRClosest);
  f1 = TMath::ATan2 (alpha1.getX (), alpha1.getZ ());
  f2 = TMath::ATan2 (fDir.getX (), fDir.getZ ());
  predictedSinPhi =
    fMatchPar->getDeflection (rKick1, fP, err, fPolarity);
  if (fabs(predictedSinPhi)<=1.) {
    fDeltaPhi = sin ((f2 - f1) / 2.) - predictedSinPhi;
  } else {
    fDeltaPhi = 0.9; //FIXME: not sure this is a proper value.
  }

//   itanphi = TMath::Tan(2*TMath::ASin(predictedSinPhi)+f1);

//   if (TMath::Abs(itanphi) > 0.) {
//     fDeltaPhi = r2.getX()-rKick1.getX() - itanphi*(r2.getZ() - rKick1.getZ());
//   } else {
//     fDeltaPhi = 1000.;
//   }

  //  fAngle = deltaG; //FIXME: Check if sin(g2-g1) is better

  // Remember used segments
  fInnerSeg = inner;
  fOuterSeg = outer;
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
  Float_t var[3];
  var[0] = fDistance;
  var[1] = fDistanceKick;
  var[2] = fabs(fDeltaPhi);
  

/* Old/obsolete algorithm. Remove after history in CVS
  r = (fDistanceKick < fParam->getMaxDistance ()) &&
    (TMath::Abs(fDeltaPhi) < fParam->getMaxDeltaPhi ()) &&
    (fDistance < fParam->getMaxChi2 ());
*/

  return fMdcMatchingPar->bin(var);
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
HRtTrackEvaluator::fillControl(TNtuple * control, Bool_t onlyMdc3)
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
      control->Fill (fDistance, fDistanceKick, fAngle,fDeltaPhi, fP, 0,
		     fRClosest.getX (), fRClosest.getY (),
		     fRClosest.getZ ());
    }
}

ClassImp(HRtTrackEvaluator)
