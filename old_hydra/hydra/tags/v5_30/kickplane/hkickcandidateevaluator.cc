//*-- Author : M. Sanchez
//*-- Modified : 22.08.2000
#include "hkickcandidateevaluator.h"
#include "htofgeompar.h"
#include "hspecgeompar.h"
#include "hmdcgeompar.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "hshowerhit.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "htofhitsim2.h" //SIM
#include "hshowerhittof.h"
#include "hshowergeometry.h"
#include "hkicktrack.h"
#include "hmdchit.h"


////////////////////////////////////////////////////
//HKickCandidateEvaluator
//
//   Evaluates a candidate from a KickImpact and a TOF
//or Shower hit using the kickplane.
/////////////////////////////////////////////////////

//---------------------- HKickCandidateEvaluator --------------------

///////////////////////////////////////////////////////////////////////////
//                        TOF Evaluation
///////////////////////////////////////////////////////////////////////////
void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
				       HTofHit *hit,
				       HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the TOF. It internaly uses the generic evaluation function but adds specific
  //cout << "TOF ";
  Float_t tof;
  const HGeomVector &targetPos = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
  Double_t v,dist,C = 299792458,mass;
  //code for TOF hits (mass calculation)
  transform(hit,fFarPos);
  evaluate(imp,out);
  //TOF specific stuff
  tof = hit->getTof() - fTimeOffset;
  dist = (imp->position() - targetPos).length(); 
  dist += fDistanceToFarHit;     //distance from imp to tof hit
  dist /= 1000.;                 //In meters
  v = (dist/tof) * 1e9;          //TOF in ns

  if (v > C) { //It is an electron
    mass=0.;
  } else {
    mass = out.getMomentum() * C * TMath::Sqrt(1 - (v/C) * (v/C)) / v;
  }

  out.setMass(mass);
  out.setTof(tof);
  out.setBeta( v / C );
  out.setSystem(1 + 10 * (hit->getModule()*8+hit->getCell()) );
  fMassFitter.fitMomentum(out.getMomentum(),out.getMomentumError(),
			  v / C, (v/C) * TMath::Sqrt( (.01/dist)*(.01/dist) + 
					     (.1/tof)*(.1/tof)),
			  out.getPolarity());
  if (fMassFitter.getMomentum() > 0.) {
    Double_t p = 0.;
    Double_t s1 = 1 / out.getMomentumError(); s1 *= s1;
    Double_t s2 = 1 / fMassFitter.getMomentumErr(); s2 *= s2;

    p = (s1 * out.getMomentum() + s2 * fMassFitter.getMomentum()) /
      (s1 + s2);

    out.setPTof(p);
    out.setPTofErr(1./TMath::Sqrt(s1 + s2));
    out.setPID(fMassFitter.getPID());
  }
}

void HKickCandidateEvaluator::transform(HTofHit *data,HGeomVector &r) {
  //Transforms a TOF hit from the local coordinate system to the LAB
  //coordinate system and smears the data acording to the resolution.
  HGeomVector rLocal,errLocal;
  Float_t x,y,z,rodHeight,rodLength;
  //  const Float_t tofZeroRes=.100; //in ns 
#ifdef NOV00
  #warning "USing resolution for real data"
  const Float_t tofXResol=28;//15.5; // TOF resolution in mm
#else
  const Float_t tofXResol=50;//15.5; // TOF resolution in mm
#endif
  HModGeomPar *module=fTofGeometry->getModule(0, //Sector
					      data->getModule());
  HGeomTransform &modTrans=module->getLabTransform();
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();
  const HGeomRotation &modRot=modTrans.getRotMatrix();
  rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - rodVol->getPoint(1)->getY());
  rodLength = 2. * TMath::Abs(rodVol->getPoint(0)->getX());

  x = data->getXpos();
  z = 0;

#ifndef NOV00
  //Old digitizer gives stuff in mm
  #warning "TOF: converting cm to mm"
  x *= 10;
#endif
  

#ifdef NOV00
  #warning "TOF misalignment"
  Double_t zmis[6]={109, 24.5, 21.2, 16, 16.5, 20.4};
  Double_t ymis[8]={3, 2, 1, 0, 0, -3, -6, -9};
  z = zmis[data->getSector()];
#else
  z=0;
#endif
  

  if (!isTofSimulated()) {
    y=0;
    errLocal.setX(tofXResol);
    errLocal.setY(rodHeight/(TMath::Sqrt(12.)));
    errLocal.setZ(0.);
  } else {
    y = ((HTofHitSim2 *)data)->getY();
    errLocal.setX(0.);
    errLocal.setY(0.);
    errLocal.setZ(0.); 
  }

  r.setX(x);
  r.setY(y);
  r.setZ(0.);
  
  rLocal=rodTrans.transFrom(r); //Rod to modue system
#ifdef NOV00
  #warning "TOf inner missalignment"
  rLocal.setY(rLocal.getY() + ymis[data->getModule()]);
#endif

  r=modTrans.transFrom(rLocal); //Module to LAB system
  r.setZ( r.getZ() + z );
  fFarPosErr=modRot*errLocal;          //Errors in x,y,z in LAB system
  fFarPlaneSlope=modRot(2*3+1)/modRot(1*3+1);                 //Slope of ROD 
  fFarPlaneOffset=modTrans.getTransVector().getZ() - fFarPlaneSlope * 
    modTrans.getTransVector().getY();                   //Rod's offset
}

//////////////////////////////////////////////////////////////////
//                      MDC Evaluation
//////////////////////////////////////////////////////////////////
void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
			      HMdcSeg *hit,HKickCandidate &out) {
  HGeomVector r,alpha;
  HGeomTransform &trans=fMdcGeometry->getModule(0,2)->getLabTransform(); 
  //FIXME: Use HMdcHit
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;

  //Transform from HMdcSeg coordinates to r, alpha straigth line repres.
  theta=hit->getTheta();
  phi=hit->getPhi();
  r.setZ(hit->getZ());
  r.setX(hit->getR()*TMath::Cos(phi+pi2));
  r.setY(hit->getR()*TMath::Sin(phi+pi2));
  alpha.setZ(TMath::Cos(theta));
  alpha.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alpha.setX(TMath::Sin(theta)*TMath::Cos(phi));

  //Relative slopes of y,x versus z for the straight line
  Double_t by = alpha.getY() / alpha.getZ();
  Double_t bx = alpha.getX() / alpha.getZ();

  //Calc. parameters for eq. of plane of MDC3 
  fFarPlaneSlope=trans.getRotMatrix()(2*3+1) / trans.getRotMatrix()(1*3+1);
  fFarPlaneOffset=trans.getTransVector().getZ() - fFarPlaneSlope * 
    trans.getTransVector().getY();

  //Calc. intersectio of track with MDC3 (measured point)
  fFarPos.setZ( (by * r.getZ() - r.getY() + fFarPlaneOffset) /
		(by - fFarPlaneSlope) );
  fFarPos.setX( r.getX() + bx * (fFarPos.getZ() - r.getZ()) );
  fFarPos.setY( r.getY() + by * (fFarPos.getZ() - r.getZ()) );

  //Set measurement errors in MDC3
  fFarPosErr.setXYZ(.16,.08,0.); //FIXME: transform error to LAB system.

  //---------------------------------------------------------------//
  HGeomVector &alpha0 = imp->direction();
  HGeomVector &kpos = imp->position();
  Double_t errPt = 0.;

  Double_t g1 = TMath::ATan2(alpha0.getZ(), alpha0.getY());
  Double_t g2 = TMath::ATan2(alpha.getZ(), alpha.getY());

  Double_t st = 2. * TMath::Sin( (g2 - g1) / 2. ); 

  Double_t pt = fKickPlane->getPt(imp->position(),errPt,1);
  
  //...............................................................//
  //out.setPosition1(fFarPos.getX(),fFarPos.getY(),fFarPos.getZ());
  out.setPosition1(kpos.getX(), kpos.getY(), kpos.getZ());
  out.setDenominator(st);
  out.setMomentum(pt / st);
  //---------------------------------------------------------------//
//   evaluate(imp,out);
  HGeomVector n = imp->direction().vectorProduct(alpha); n /= n.length();
  out.setDist( TMath::Abs((imp->position() - r).scalarProduct(n)) );
}

///////////////////////////////////////////////////////////////////////////
//                   SHOWER evaluation
///////////////////////////////////////////////////////////////////////////
void HKickCandidateEvaluator::transform(HShowerHit *data) {
  //converts a Shower hit into a standard form for evaluation.
  Float_t x,y,z,x0,x1,x2,x3,y0,y1;
  Double_t sq12 = TMath::Sqrt(12.);
  data->getXY(&x,&y);

  //Get transformation from local coord. system to LAB coord system
  const HGeomTransform &trans=fShoGeometry->getTransform(0,data->getModule());

  //Get pad geometry
  HShowerPad *pad = fShoGeometry->getPadParam(0)->getPad(data->getRow(),
							 data->getCol());
  const HGeomRotation &rot = trans.getRotMatrix();

  #warning "Conversion from cm to mm needed for Shower (25.10.2000)"
  x0 = pad->fXld * 10.; 
  x1 = pad->fXrd * 10.;
  x2 = pad->fXlu * 10.;
  x3 = pad->fXru * 10.;
  y0 = pad->fYld * 10.;
  y1 = pad->fYlu * 10.;

  Double_t N = ((x2 - x0 - x3 + x1) * (y1 + y0)) / 2.0 + (x1 - x0) * (y1 - y0);
  Double_t A = (x2 - x0 - x3 + x1) / (y1 - y0);
  Double_t B = (x1 - x0);

  // Mean y value for a uniform distribution in the Shower pad.
  y = (A * (y1*y1*y1 - y0*y0*y0)) / (3.0 * N) + 
    (B * (y1*y1 - y0*y0)) / ( 2.0 *N); 

#ifdef NOV00
#warning "Shower misalignment"
   Double_t zmis[6]={191, 139, 93, 163.5, 143, 97.5};
   z = zmis[data->getSector()];
#else
    z=0;
#endif

  HGeomVector r;
  r.setX(x); 
  r.setY(y);
  r.setZ(0.);
  fFarPos=trans.transFrom(r);
 
  fFarPos.setZ(fFarPos.getZ() + z);

  //Fill errors. error is size/sqrt(12). Note size is given in cm for Shower.
  r.setX( (10. * (pad->fXru - pad->fXlu)) / sq12);
  r.setY( (10. * (pad->fYlu - pad->fYrd)) / sq12);
  r.setZ(0.);
  fFarPosErr = rot * r;

  fFarPlaneSlope = rot(2 * 3 + 1) / rot(1 * 3 + 1); 
  fFarPlaneOffset = trans.getTransVector().getZ() - fFarPlaneSlope *
    trans.getTransVector().getY();

}

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,HShowerHit *hit,
				       HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the Shower. It internaly uses the generic evaluation function.
  HGeomVector &kpos = imp->position();
  const HGeomVector &targetPos = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
  transform(hit);
  evaluate(imp,out);

  out.setMass(-10.0);

  //Mass Calcualation..
  Double_t a=-1.72162;  // Tofino plane: y = a z + b
  Double_t b=4356.3165;
  HGeomVector alpha = fFarPos - imp->position(); alpha /= alpha.length();
  Double_t bx = alpha.getX() / alpha.getZ();
  Double_t by = alpha.getY() / alpha.getZ();
  HGeomVector posTofino;
  Float_t tof;
  Double_t v,dist,C=299792458,mass;

  posTofino.setZ( (kpos.getY() - by * kpos.getZ() - b) / (a - by) );
  posTofino.setY( kpos.getY() + by * (posTofino.getZ() - kpos.getZ()) );
  posTofino.setX( kpos.getX() + bx * (posTofino.getZ() - kpos.getZ()) );

  Int_t mult = ((HShowerHitTof *)hit)->getTofinoMult(); //FIXME: no cast
  if ( mult == 1) {
    tof = ((HShowerHitTof *)hit)->getTof() - fTimeOffset; //FIXME: no cast
    dist = (imp->position() - targetPos).length(); 
    dist += (posTofino - imp->position()).length();
    dist /= 1000.;                 //In meters
    v = (dist/tof) * 1e9;          //TOF in ns
    
    if (v > C) { //It is an electron
      mass = 0.;
    } else {
      mass = out.getMomentum() * C * TMath::Sqrt(1 - (v/C) * (v/C)) / v;
    }
    out.setMass(mass);
    out.setTof(tof);
    out.setBeta( v / C );
    out.setSystem(0);
    fMassFitter.fitMomentum(out.getMomentum(),out.getMomentumError(),
			    v / C, v/C * TMath::Sqrt( (.01/dist)*(.01/dist) + 
						      (.2/tof)*(.2/tof)),
			    out.getPolarity());
    if (fMassFitter.getMomentum() > 0.) {
      Double_t p = 0.;
      Double_t s1 = 1 / out.getMomentumError(); s1 *= s1;
      Double_t s2 = 1 / fMassFitter.getMomentumErr(); s2 *= s2;
      
      p = (s1 * out.getMomentum() + s2 * fMassFitter.getMomentum()) /
	(s1 + s2);
      
      out.setPTof(p);
      out.setPTofErr(1./TMath::Sqrt(s1 + s2));
      out.setPID(fMassFitter.getPID());
    } 
  } else out.setMass(-2);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                       Common evaluation
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
				       HKickCandidate &out) {
  //Generic evaluation of a candidate
  Double_t deltaPhi=0,errPtGhi,errPtPhi,theta1,itanphi,deltaX=1000.;
  Double_t pPhi,pGhi,errPPhi,errPGhi,D,Dx,Dy,errP;
  Double_t ptPhi;
  Int_t polarity;

  //Set fFarDir to the direction after the magnet
  fFarDir=fFarPos-imp->position();
  fDistanceToFarHit=fFarDir.length();
  fFarDir/=fDistanceToFarHit;

  //Calculate incident and output angles
  theta1 = TMath::ACos( imp->direction().getZ() / imp->direction().length());
  fGhi1=TMath::ATan2(imp->direction().getZ(),imp->direction().getY()); 
  fGhi2=TMath::ATan2(fFarDir.getZ(),fFarDir.getY());
  fPhi1=TMath::ATan2(imp->direction().getY(),imp->direction().getX());
  fPhi2=TMath::ATan2(fFarDir.getY(),fFarDir.getX());

  //Calculate deflection
  fSinGhi=sin((fGhi2-fGhi1)/2.);
  fSinPhi=sin((fPhi2-fPhi1)/2.);

  //Look to the sign of the deflection to determine polarity
  polarity = (fSinGhi >= 0.) ? 1 : -1;

  //Calculate momentum both from deflection in phi and ghi
  pGhi=fKickPlane->getPt(imp->position(),errPtGhi,polarity) / (2 * fSinGhi);
  ptPhi = fKickPlane->getPtPhi(imp->position(),errPtPhi,polarity);
  pPhi = ptPhi / (2 * fSinPhi);
  
//   cout << pGhi << " = ";
//   cout << fKickPlane->getPt(imp->position(),errPtGhi,polarity) << ", ";
//   cout << 2 * fSinGhi << endl;
  //Error in Pghi
  D = ((fFarPos.getY() - imp->position().getY()) * 
       (fFarPos.getY() - imp->position().getY()) +
       (fFarPos.getZ() - imp->position().getZ()) * 
       (fFarPos.getZ() - imp->position().getZ()));

  errPGhi = (TMath::Cos((fGhi2 - fGhi1) / 2.) * 
	     (imp->position().getZ() - fFarPlaneSlope*imp->position().getY() - 
	      fFarPlaneOffset) * fFarPosErr.getY() ) / (2 * fSinGhi * D);

  errPGhi = TMath::Abs(pGhi)*TMath::Sqrt(errPtGhi*errPtGhi + errPGhi*errPGhi);
  

  //Error in Pphi
  Dx = (fFarPos.getX() - imp->position().getX()) * 
    (fFarPos.getX() - imp->position().getX());

  Dy = (fFarPos.getY() - imp->position().getY()) * 
    (fFarPos.getY() - imp->position().getY());

  D = Dx + Dy;

  errPPhi = TMath::Cos( (fPhi2 - fPhi1)/2. ) / (2 * fSinPhi * D);
  errPPhi = TMath::Abs(pPhi) * 
    TMath::Sqrt(errPtPhi*errPtPhi + 
		errPPhi*errPPhi * (Dx * fFarPosErr.getY() * fFarPosErr.getY() +
				   Dy * fFarPosErr.getX() * fFarPosErr.getX())
		);
  
  //*** Calculate momentum **********************
  fP = pGhi;
  errP = errPGhi;
//   Double_t errPPhi2 = errPPhi * errPPhi;
//   Double_t errPGhi2 = errPGhi * errPGhi;
//   fP = ( pGhi / errPGhi2 + pPhi / errPPhi2) / ( 1 / errPGhi2 + 1 / errPPhi2);
//   errP = 1 / TMath::Sqrt( 1 / errPPhi2 + 1 / errPGhi2);
  
  if (fP>0.) { //Expected fPredictedSinPhi
    fPredictedSinPhi = ptPhi / ( 2*pGhi); //Momentum from polar deflection
    deltaPhi = (fPhi2 - fPhi1) - 2*TMath::ASin(fPredictedSinPhi);
    itanphi=TMath::Tan(fPhi1 + 2*TMath::ASin(fPredictedSinPhi));
    if (TMath::Abs(itanphi) > 0.) {
      itanphi = 1. / itanphi;
      deltaX = fFarPos.getX() - imp->position().getX() - itanphi * 
	(fFarPos.getY() - imp->position().getY());
    } else {
      #if DEBUG_LEVEL>1
      Warning("evaluate","Division by 0");      
      #endif
      deltaX = 1000.*fFarPosErr.getX();
    }
  } else {deltaPhi=1000; deltaX=1000.*fFarPosErr.getX(); }

  
  //*** Set output values ******************
  out.setPTof(-1.);
  out.setPTofErr(0.);
  out.setPID(HKickTrack::unknownId);
  out.setMomentum(fP); 
  out.setDenominator(2 * fSinGhi);
  // #warning "Xpull is difference in x"
  out.setXPull(deltaX / fFarPosErr.getX() );
  out.setPolarity(polarity);
  out.setMomentumError(errP); 
  out.setSector(imp->getSector());
  out.setPosition1(imp->position().getX(),
		   imp->position().getY(),
		   imp->position().getZ()) ;
}

ClassImp(HKickCandidateEvaluator)
