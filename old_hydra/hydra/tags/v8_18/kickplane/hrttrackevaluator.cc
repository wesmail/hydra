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

#undef USE_EXTERNAL_PAR_FILE

//------------------------------- HRtTrackEvaluator ---------------------
Bool_t HRtTrackEvaluator::init(void) {
  HRuntimeDb *rtdb = Hades::instance()->getRuntimeDb();
  HKickTask::setKickParIo (rtdb);
  fKickPlane = (HKickPlane2 *) rtdb->getContainer ("KickPlane2MDC3");
  fMatchPar = (HKickMatchPar *) rtdb->getContainer ("KickMatchParMDC3");
  fSpecGeomPar = (HSpecGeomPar *) rtdb->getContainer ("SpecGeomPar");

#ifdef USE_EXTERNAL_PAR_FILE
  Warning("init","Using hardcoded parameter file!!!!!");
  TFile f("params.root");
  fMdcMatchingPar = (HRtMatchingPar *)f.Get("RtMatchingParMDC3");
#else
  fMdcMatchingPar = (HRtMatchingPar *)rtdb->getContainer("RtMatchingParMDC3");
#endif

  fMdcGeom = (HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");

  return kTRUE;
}

Bool_t HRtTrackEvaluator::reinit(void) {
  if (fMdcGeom->hasChanged()) {
    for (Int_t i=0;i<4;i++) {
      HGeomVolume *vol=fMdcGeom->getRefVolume(i);
      if (vol) {
        fMdcModule[i].setTransform(vol->getTransform());
        fMdcModule[i].invert();
      } else {
	Warning("reinit","Could not initialize reference geometry. This is a problem if you are intending to use Reference Trajectories. Bug M.Sanchez@gsi.de if that is the case and you are seeing this message");
      }
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
  Int_t i;
  register Int_t mod;

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

void HRtTrackEvaluator::computeDistance(Float_t &dist, Float_t &errDist) {
  Float_t sx1 = fInnerAlpha.getX() / fInnerAlpha.getZ();
  Float_t sy1 = fInnerAlpha.getY() / fInnerAlpha.getZ();
  Float_t x1 = fInnerPos.getX() - fInnerPos.getZ() * sx1;
  Float_t y1 = fInnerPos.getY() - fInnerPos.getZ() * sy1;
  Float_t sx1_2 = sx1*sx1;
  Float_t sy1_2 = sy1*sy1;
  Float_t transMatrix1[4*4] = { 1, 0, -fInnerPos.getZ(),                 0, 
                                0, 1,                 0, -fInnerPos.getZ(),
 				0, 0,                 1,                 0,
                                0, 0,                 0,                 1};
  HSymMat4 cov1; //covariance matrix for inner segment (x,y,x',y')
  cov1.transform(fInnerSeg->getCovariance(),transMatrix1); //Propagate cov. matrix to z=0
  
  HGeomVector &outerPos = fOuterSeg->getR();
  HGeomVector &outerAlpha = fOuterSeg->getAlpha();
  Double_t sx2 = outerAlpha.getX() / outerAlpha.getZ();
  Double_t sy2 = outerAlpha.getY() / outerAlpha.getZ();
  Double_t x2 = outerPos.getX() - outerPos.getZ() * sx2;
  Double_t y2 = outerPos.getY() - outerPos.getZ() * sy2;
  Double_t sx2_2 = sx2*sx2;
  Double_t sy2_2 = sy2*sy2;
  HSymMat &cov2 = fOuterSeg->getCovariance(); //cov. matrix for outer segment: x,y,x',y'

  Double_t deltaX = x1-x2;
  Double_t deltaY = y1-y2;

  Double_t L_2 = sx1_2 + sx2_2 + sy1_2 + sy2_2 + sx1_2*sy2_2 + sx2_2*sy1_2 -
    2*sx1*sx2-2*sy1*sy2-2*sx1*sx2*sy1*sy2;
  //printf("%g --\n",L_2);
  Double_t L = sqrt(L_2);
  Double_t N = deltaX*(sy1-sy2) + deltaY*(sx2-sx1);
  Double_t d = N/L;

  Double_t d_d_x1 = (sy1-sy2)/L;
  Double_t d_d_y1 = (sx2-sx1)/L;
  Double_t d_d_x2 = (sy2-sy1)/L;
  Double_t d_d_y2 = (sx1-sx2)/L;

  Double_t d_d_sx1 = (-deltaY*L - d*(sx1 + sx1*sy2_2 - sx2 - sy1*sx2*sy2)) 
    / L_2;
  Double_t d_d_sy1 = ( deltaX*L - d*(sy1 + sy1*sx2_2 - sy2 - sx1*sx2*sy2))
    / L_2;
  Double_t d_d_sx2 = ( deltaY*L - d*(sx2 + sy1_2*sx2 - sx1 - sx1*sy1*sy2))
    / L_2;
  Double_t d_d_sy2 = (-deltaX*L - d*(sy2 + sx1_2*sy2 - sy1 - sx1*sy1*sx2))
    / L_2;

  Float_t vd1[] = {d_d_x1, d_d_y1, d_d_sx1, d_d_sy1};
  Float_t vd2[] = {d_d_x2, d_d_y2, d_d_sx2, d_d_sy2};

  /*printf("vd1: ");
  for (Int_t i=0;i<4;i++) printf("%11.4g ",vd1[i]);
  printf("\n");
  printf("vd2: ");
  for (Int_t i=0;i<4;i++) printf("%11.4g ",vd2[i]);
  printf("\n");*/


  Float_t sigma = cov1.convolution(vd1,vd1);
  sigma += cov2.convolution(vd2,vd2);
  sigma = sqrt(sigma);
  //printf("%f\n",sigma);

  // Set output
  dist = d;
  errDist = sigma;
}

void HRtTrackEvaluator::evaluate(Bool_t onlyMdc3) {
  // evaluates a combination of two segments (inner, outer). That means
  //filling variables that later can be used to check if this combination
  //corresponds to a real particle or not.
  HGeomVector & r2 = fOuterSeg->getR ();
  HGeomVector & alpha2 = fOuterSeg->getAlpha ();
  HGeomVector rKick2;
  Float_t g1, g2, deltaG;
  Float_t f1, f2;
  Float_t predictedSinPhi;
  Float_t err;

  // Intersection with the kickplane
  fKickPlane->calcIntersection (fInnerPos, fInnerAlpha, fRKick1);
  fKickPlane->calcIntersection (r2, alpha2, rKick2);

  // Point of closest approach between tracks
  fVertexFitter.reset ();
  fVertexFitter.addLine (fInnerPos, fInnerAlpha);
  fVertexFitter.addLine (r2, alpha2);
  fVertexFitter.getVertex (fRClosest);

  // Kickplane stuff  
  if (onlyMdc3) {
      fDir = r2 - fRKick1;
      fLeverArm = fDir.length ();
      fDir /= fLeverArm;
      fAngle = TMath::ACos (fInnerAlpha.scalarProduct (fDir));
  } else {
    fDir = alpha2; //FIXME: Optimize this!!
    fLeverArm = (r2-fRKick1).length();  //Use dist btw MDCs or MDC width
    fAngle = TMath::ACos(fInnerAlpha.scalarProduct(alpha2));
  }
  //FIXME: "Chequear si calculo de polaridad es correcto"
  g1 = TMath::ATan2 (fInnerAlpha.getZ (), fInnerAlpha.getY ());
  g2 = TMath::ATan2 (alpha2.getZ (), alpha2.getY ());
  deltaG = g2 - g1;
  fPolarity = (deltaG > 0) ? 1 : -1;
  fP = fKickPlane->getP (fRKick1, 2 * sin ((fAngle) / 2.), fPolarity);

  // Calculate control variables
//  Float_t itanphi = 0.;
  Float_t dist, errDist;
  computeDistance(dist, errDist);
  fDistance = dist/errDist;
  //fDistance =
    // ((fInnerPos - r2).
//		scalarProduct (fInnerAlpha.vectorProduct (alpha2)));
    //TMath::Abs ((fInnerPos - r2).
//		scalarProduct (fInnerAlpha.vectorProduct (alpha2)));
  fDistanceKick = fKickPlane->distanceTo (fRClosest);
  f1 = TMath::ATan2 (fInnerAlpha.getX (), fInnerAlpha.getZ ());
  f2 = TMath::ATan2 (fDir.getX (), fDir.getZ ());
  predictedSinPhi =
    fMatchPar->getDeflection (fRKick1, fP, err, fPolarity);
  if (fabs(predictedSinPhi)<=1.) {
    fDeltaPhi = sin ((f2 - f1) / 2.) - predictedSinPhi;
  } else {
    fDeltaPhi = 0.9; //FIXME: not sure this is a proper value.
  }

  ///TEST: Use difference in slope rather than distances
  //These 2 lines overwrite the previous computation of distance
  fDistance = fDir.getX()/fDir.getZ() - alpha2.getX()/alpha2.getZ();
  fDistanceKick = fDir.getY()/fDir.getZ() - alpha2.getY()/alpha2.getZ();

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
  length = (fRKick1 - target).length ();
  length += (fOuterSeg->getR () - fRKick1).length ();
  return length;
}

Bool_t
HRtTrackEvaluator::isGoodCombination (void)
{
  // Returns true or false depending on the latest evaluated combination
  //of segments being good or not.
  //

  //Hardcoded matching. Remove after history in CVS
  //Bool_t r = (abs(fDistanceKick)<.7 && abs(fDistance)<.7 && abs(fDeltaPhi)<.7);
  //return r;
  
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
		     fRClosest.getZ (),fInnerSeg->getTheta(),fInnerSeg->getPhi(),0,fPolarity);
    }
  else
    {
      control->Fill (fDistance, fDistanceKick, fAngle,fDeltaPhi, fP, 0,
		     fRClosest.getX (), fRClosest.getY (),
		     fRClosest.getZ ());
    }
}

ClassImp(HRtTrackEvaluator)
