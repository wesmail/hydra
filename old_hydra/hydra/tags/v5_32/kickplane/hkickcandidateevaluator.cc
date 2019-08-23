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
  tof = hit->getTof();
  if (tof<0) out.setMomentum(0);
  dist = (imp->position() - targetPos).length(); 
  dist += fDistanceToFarHit;     //distance from imp to tof hit
  dist /= 1000.;                 //In meters
  v = (dist/tof) * 1e9;          //TOF in ns

  if (v > C) { //It is an electron
    mass=0.;
  } else {
    mass = out.getMomentum() * C * TMath::Sqrt(1 - (v/C) * (v/C)) / v;
  }

  Double_t mass2=out.getMomentum()*out.getMomentum();
  mass2*= ( 1 - (v/C)*(v/C) );
  mass2/= (v/C)*(v/C);
  out.setMass(mass2);
  out.setTof(tof);
  out.setOuterHit(hit);
  out.setBeta( v / C );
  out.setSystem(1);// + 10 * (hit->getModule()*8+hit->getCell()) );
  fMassFitter.fitMomentum(out.getMomentum(),out.getMomentumError(),
			  v / C, (v/C) * TMath::Sqrt( (.01/dist)*(.01/dist) + 
					     (.1/tof)*(.1/tof)),
			  int(out.getPolarity()));
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
  Float_t x,y,rodHeight,rodLength;

  HModGeomPar *module=fTofGeometry->getModule(data->getSector(), //Sector
					      data->getModule());
  HGeomTransform &transMod = module->getLabTransform();
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())
    ->getTransform();

  HGeomTransform modTrans(transMod);
  modTrans.transTo(transSec);
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();
  const HGeomRotation &modRot=modTrans.getRotMatrix();
  rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - rodVol->getPoint(1)->getY());
  rodLength = 2. * TMath::Abs(rodVol->getPoint(0)->getX());

  x = data->getXpos();


  if (!isTofSimulated()) {
    y=0;
    errLocal.setX(fTofXResolution);
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

  r=modTrans.transFrom(rLocal); //Module to LAB system
  r.setZ( r.getZ() );
  fFarPosErr=modRot*errLocal;          //Errors in x,y,z in LAB system
  fFarPlaneSlope=modRot(2*3+1)/modRot(1*3+1);                 //Slope of ROD
  fFarPlaneOffset=modTrans.getTransVector().getZ() - fFarPlaneSlope *
    modTrans.getTransVector().getY();                   //Rod's offset
}

///////////////////////////////////////////////////////////////////////////
//                   SHOWER evaluation
///////////////////////////////////////////////////////////////////////////
void HKickCandidateEvaluator::transform(HShowerHit *data) {
  //converts a Shower hit into a standard form for evaluation.
  Float_t x,y,x0,x1,x2,x3,y0,y1;
  Double_t sq12 = TMath::Sqrt(12.);
  data->getXY(&x,&y);

  //Get transformation from local coord. system to LAB coord system
  const HGeomTransform &transMod=fShoGeometry->getTransform(data->getSector(),
							    data->getModule());
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
  HGeomTransform trans(transMod);
  trans.transTo(transSec);

  
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

  HGeomVector r;
  r.setX(x); 
  r.setY(y);
  r.setZ(0.);
  fFarPos=trans.transFrom(r);
 
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
  out.setOuterHit(hit);
  out.setSystem(0);

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
    tof = ((HShowerHitTof *)hit)->getTof(); //FIXME: no cast
    dist = (imp->position() - targetPos).length(); 
    dist += (posTofino - imp->position()).length();
    dist /= 1000.;                 //In meters
    v = (dist/tof) * 1e9;          //TOF in ns
    
    if (v > C) { //It is an electron
      mass = 0.;
    } else {
      mass = out.getMomentum() * C * TMath::Sqrt(1 - (v/C) * (v/C)) / v;
    }

    Double_t mass2=out.getMomentum()*out.getMomentum();
    mass2*= ( 1 - (v/C)*(v/C) );
    mass2/= (v/C)*(v/C); 
    out.setMass(mass2);
    out.setTof(tof);
    out.setBeta( v / C );
    fMassFitter.fitMomentum(out.getMomentum(),out.getMomentumError(),
			    v / C, v/C * TMath::Sqrt( (.01/dist)*(.01/dist) + 
						      (.2/tof)*(.2/tof)),
			    int(out.getPolarity()));
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
