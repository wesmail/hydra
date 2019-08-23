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
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"

//------------------------------- HRtTrackEvaluator ---------------------
Bool_t HRtTrackEvaluator::init(void) {
  HRuntimeDb *rtdb = Hades::instance()->getRuntimeDb();
  HKickTask::setKickParIo (rtdb);
  fKickPlane = (HKickPlane2 *) rtdb->getContainer ("KickPlane2MDC3");
  fMatchPar = (HKickMatchPar *) rtdb->getContainer ("KickMatchParMDC3");
  fSpecGeomPar = (HSpecGeomPar *) rtdb->getContainer ("SpecGeomPar");

  fMdcMatchingPar = (HRtMatchingPar *)rtdb->getContainer("RtMatchingParMDC3");
  fMdcGeom = (HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");

  return kTRUE;
}

Bool_t HRtTrackEvaluator::reinit(void) {
  if (fMdcGeom->hasChanged()) {
    for (int i=0;i<4;i++) {
      fMdcModule[i].setTransform(fMdcGeom->getRefVolume(i)->getTransform());
      fMdcModule[i].invert();
    }
  }
  return kTRUE;
}


void HRtTrackEvaluator::setInnerSeg(HMdcSeg *seg) {
  Float_t theta,phi;
  Float_t pi2 = TMath::Pi() / 2.;

  theta = seg->getTheta ();
  phi = seg->getPhi ();
  fInnerPos.setZ (seg->getZ ());
  fInnerPos.setX (seg->getR () * TMath::Cos (phi + pi2));
  fInnerPos.setY (seg->getR () * TMath::Sin (phi + pi2));
  fInnerAlpha.setZ (TMath::Cos (theta));
  fInnerAlpha.setY (TMath::Sin (theta) * TMath::Sin (phi));
  fInnerAlpha.setX (TMath::Sin (theta) * TMath::Cos (phi));
  fInnerSeg = seg;
}

void HRtTrackEvaluator::setOuterSeg(HRtSegmentRef *seg) {
  fOuterSeg = seg;
}

void 
HRtTrackEvaluator::extractMeasurements(HRtVector &m,HRtMatrix &W) {
  HGeomVector localR,localDir;
  int i;
  register int mod;

  for (i=0;i<2;i++) {
    localR = fMdcModule[i].transFrom(fInnerPos);
    localDir = fMdcModule[i].getRotMatrix()*fInnerAlpha;
    localR.setX(localR.getX() + 
	localDir.getX()/localDir.getZ() * (0-localR.getZ()));
    localR.setY(localR.getY() + 
	localDir.getY()/localDir.getZ() * (0-localR.getZ()));
    mod = i<<1;
    m(mod) = localR.getX();
    mod |= 0x01;
    m(mod) = localR.getY();

   
  }

  for (i=2;i<4;i++) {
    localR = fMdcModule[i].transFrom(fOuterSeg->getR());
    localDir = fMdcModule[i].getRotMatrix()*fOuterSeg->getAlpha();
    localR.setX(localR.getX() + 
	localDir.getX()/localDir.getZ() * (0-localR.getZ()));
    localR.setY(localR.getY() + 
	localDir.getY()/localDir.getZ() * (0-localR.getZ()));
    mod = i<<1;
    m(mod) = localR.getX();
    mod |= 0x01;
    m(mod) = localR.getY();
  }
}


void HRtTrackEvaluator::evaluate(Bool_t onlyMdc3) {
  // evaluates a combination of two segments (inner, outer). That means
  //filling variables that later can be used to check if this combination
  //corresponds to a real particle or not.
  HGeomVector & r2 = fOuterSeg->getR ();
  HGeomVector & alpha2 = fOuterSeg->getAlpha ();
  HGeomVector rKick1, rKick2;
  Float_t g1, g2, deltaG;
  Float_t f1, f2;
  Float_t predictedSinPhi;
  Float_t err;

  // Intersection with the kickplane
  fKickPlane->calcIntersection (fInnerPos, fInnerAlpha, rKick1);
  fKickPlane->calcIntersection (r2, alpha2, rKick2);

  // Point of closest approach between tracks
  fVertexFitter.reset ();
  fVertexFitter.addLine (fInnerPos, fInnerAlpha);
  fVertexFitter.addLine (r2, alpha2);
  fVertexFitter.getVertex (fRClosest);

  // Kickplane stuff  
  if (onlyMdc3) {
      fDir = r2 - rKick1;
      fLeverArm = fDir.length ();
      fDir /= fLeverArm;
      fAngle = TMath::ACos (fInnerAlpha.scalarProduct (fDir));
  } else {
    fDir = alpha2; //FIXME: Optimize this!!
    fLeverArm = (r2-rKick1).length();  //Use dist btw MDCs or MDC width
    fAngle = TMath::ACos(fInnerAlpha.scalarProduct(alpha2));
  }
  //FIXME: "Chequear si calculo de polaridad es correcto"
  g1 = TMath::ATan2 (fInnerAlpha.getZ (), fInnerAlpha.getY ());
  g2 = TMath::ATan2 (alpha2.getZ (), alpha2.getY ());
  deltaG = g2 - g1;
  fPolarity = (deltaG > 0) ? 1 : -1;
  fP = fKickPlane->getP (rKick1, 2 * sin ((fAngle) / 2.), fPolarity);

  // Calculate control variables
//  Float_t itanphi = 0.;
  fDistance =
    TMath::Abs ((fInnerPos - r2).
		scalarProduct (fInnerAlpha.vectorProduct (alpha2)));
  fDistanceKick = fKickPlane->distanceTo (fRClosest);
  f1 = TMath::ATan2 (fInnerAlpha.getX (), fInnerAlpha.getZ ());
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
  p(0) = 1 / fP;
  p(1) = fInnerSeg->getZ();
  p(2) = fInnerSeg->getR();
  p(3) = fInnerSeg->getPhi();
  p(4) = fInnerSeg->getTheta();
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
