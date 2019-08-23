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

////////////////////////////////////////////////////
//HKickCandidateEvaluator
//
//   Evaluates a candidate from a KickImpact and a TOF
//or Shower hit using the kickplane.
/////////////////////////////////////////////////////
//---------------------- HKickCandidateEvaluator --------------------
void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
				       HTofHit *hit,
				       HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the TOF. It internaly uses the generic evaluation function but adds specific
  //code for TOF hits (mass calculation)
  Float_t tof;
  Double_t v,dist,C=299792458,mass;
  transform(hit,fFarPos);
  evaluate(imp,out);
  //TOF specific stuff
  tof=hit->getTof();

  dist = imp->position().length(); //FIXME: Asuming emission from target
  dist += fDistanceToFarHit;     //distance from imp to tof hit
  dist /= 1000.;                 //In meters
  v = (dist/tof) * 1e9;          //TOF in ns

  if (v > C) { //It is an electron
    mass=-1;
  } else {
    mass = out.getMomentum() * C * TMath::Sqrt(1 - (v/C) * (v/C)) / v;
  }

  out.setMass(mass);
}

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
			      HMdcSeg *hit,HKickCandidate &out) {
  HGeomVector r,alpha;
  HGeomTransform &trans=fMdcGeometry->getModule(0,
						2)->getLabTransform(); 
  //FIXME: Use HMdcHit
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;


  theta=hit->getTheta();
  phi=hit->getPhi();
  r.setZ(hit->getZ());
  r.setX(hit->getR()*TMath::Cos(phi+pi2));
  r.setY(hit->getR()*TMath::Sin(phi+pi2));
  alpha.setZ(TMath::Cos(theta));
  alpha.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alpha.setX(TMath::Sin(theta)*TMath::Cos(phi));

  Double_t by = alpha.getY() / alpha.getZ();
  Double_t bx = alpha.getX() / alpha.getZ();

  fFarPlaneSlope=trans.getRotMatrix()(2*3+1) / trans.getRotMatrix()(1*3+1);
  fFarPlaneOffset=trans.getTransVector().getZ() - fFarPlaneSlope * 
    trans.getTransVector().getY();
  fFarPos.setZ( (by * r.getZ() - r.getY() + fFarPlaneOffset) /
		(by - fFarPlaneSlope) );
  fFarPos.setX( r.getX() + bx * (fFarPos.getZ() - r.getZ()) );
  fFarPos.setY( r.getY() + by * (fFarPos.getZ() - r.getZ()) );
  fFarPosErr.setXYZ(.16,.08,0.); //FIXME: transform error to LAB system.
  //  cout << fFarPlaneSlope << ":" << fFarPlaneOffset << endl;

  evaluate(imp,out);
}

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,HShowerHit *hit,
				       HKickCandidate &out) {
  //Evaluates a candidate formed by a hit in the kick plane and another one in
  //the Shower. It internaly uses the generic evaluation function.
  Float_t x,y,z;
  Float_t ex=30./sqrt(12.),ey=30./sqrt(12.),ez=0.; //FIXME: Use shower pad size.
  hit->getLabXYZ(&x,&y,&z);
  //hit->getCentroid(x,y,z);
  //hit->getErrCentroid(ex,ey,ez);
  HGeomTransform &trans=fSpecGeometry->getSector(hit->getSector())
    ->getTransform();
  HGeomVector r;
  r.setX(x); 
  r.setY(y);
  r.setZ(z);
  fFarPos=trans.transTo(r);
  fFarPosErr.setX(ex);
  fFarPosErr.setY(ey);
  fFarPosErr.setZ(ez);

  //FIXME: Get this from the corresponding container.
  fFarPlaneSlope = -0.480989 / 0.876727;
  fFarPlaneOffset = 1981.402 - fFarPlaneSlope * 1087.035;

  evaluate(imp,out);

}

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
				       HKickCandidate &out) {
  //Generic evaluation of a candidate
  Double_t deltaPhi=0,errPtGhi,errPtPhi,theta1,itanphi,deltaX=1000.;
  Double_t pPhi,pGhi,errPPhi,errPGhi,D,Dx,Dy,errP;
  Double_t ptPhi;
  Int_t polarity;

  fFarDir=fFarPos-imp->position();
  fDistanceToFarHit=fFarDir.length();
  fFarDir/=fDistanceToFarHit;

  theta1 = TMath::ACos( imp->direction().getZ() / imp->direction().length());
  fGhi1=TMath::ATan2(imp->direction().getZ(),imp->direction().getY()); 
  fGhi2=TMath::ATan2(fFarDir.getZ(),fFarDir.getY());
  fPhi1=TMath::ATan2(imp->direction().getY(),imp->direction().getX());
  fPhi2=TMath::ATan2(fFarDir.getY(),fFarDir.getX());
  fSinGhi=sin((fGhi2-fGhi1)/2.);
  fSinPhi=sin((fPhi2-fPhi1)/2.);

  polarity = (fSinGhi >= 0.) ? 1 : -1;
  pGhi=fKickPlane->getPt(imp->position(),errPtGhi,polarity) / (2 * fSinGhi);
  ptPhi = fKickPlane->getPtPhi(imp->position(),errPtPhi,polarity);
  pPhi = ptPhi / (2 * fSinPhi);
  
  //Error in Pghi
  D = ((fFarPos.getY() - imp->position().getY()) * (fFarPos.getY() - imp->position().getY()) +
       (fFarPos.getZ() - imp->position().getZ()) * (fFarPos.getZ() - imp->position().getZ()));
  errPGhi = (TMath::Cos((fGhi2 - fGhi1) / 2.) * 
	     ( imp->position().getZ() - fFarPlaneSlope * imp->position().getY() - 
	       fFarPlaneOffset) * fFarPosErr.getY() ) / (2 * fSinGhi * D);
  errPGhi = TMath::Abs(pGhi) * TMath::Sqrt( errPtGhi * errPtGhi + errPGhi * errPGhi );
  
  //Error in Pphi
  Dx = (fFarPos.getX() - imp->position().getX()) * (fFarPos.getX() - imp->position().getX());
  Dy = (fFarPos.getY() - imp->position().getY()) * (fFarPos.getY() - imp->position().getY());
  D = Dx + Dy;
  errPPhi = TMath::Cos( (fPhi2 - fPhi1)/2. ) / (2 * fSinPhi * D);
  errPPhi = TMath::Abs(pPhi) * TMath::Sqrt( errPtPhi * errPtPhi + 
					    errPPhi * errPPhi * (Dx * fFarPosErr.getY() * 
								 fFarPosErr.getY() +
								 Dy * fFarPosErr.getX() * 
								 fFarPosErr.getX()));
  
  //Calculate momentum
  fP = pGhi;
  errP = errPGhi;
  //fP = (pPhi * errPGhi + pGhi * errPPhi) / (errPPhi + errPGhi);
  //errP = TMath::Sqrt(2.) * ( errPGhi * errPPhi) / ( errPGhi + errPPhi);
  
  if (fP>0.) { //Expected fPredictedSinPhi
    fPredictedSinPhi = ptPhi / ( 2*pGhi); //Momentum calculated from polar deflection
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

  //Set values
  out.setMomentum(fP); 
  out.setDeltaPhi(deltaPhi);
  out.setXPull(deltaX / fFarPosErr.getX());
  //  cout << deltaX << ":" << fFarPosErr.getX() << ":" << out.getXPull() << endl; 
  out.setDeltaTheta(fGhi2-fGhi1);
  out.setPolarity(polarity);
  out.setMomentumError(errP); 
  out.setPhi1(fPhi1);
  out.setGhi1(fGhi1);
  out.setSector(imp->getSector());
  out.setPosition1(imp->position().getX(),
		   imp->position().getY(),
		   imp->position().getZ()) ;
  out.setTheta1(theta1);

}

void HKickCandidateEvaluator::transform(HTofHit *data,HGeomVector &r) {
  //Transforms a TOF hit from the local coordinate system to the LAB
  //coordinate system and smears the data acording to the resolution.
  HGeomVector rLocal,errLocal;
  Float_t x,y,rodHeight;
  const Float_t tofXResol=15.5; // TOF resolution in mm
  HModGeomPar *module=fTofGeometry->getModule(0, //Sector
					      data->getModule());
  HGeomTransform &modTrans=module->getLabTransform();
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();
  const HGeomRotation &modRot=modTrans.getRotMatrix();
  rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - rodVol->getPoint(1)->getY());
  
  x = data->getXpos();
  
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
  r=modTrans.transFrom(rLocal); //Module to LAB system
  fFarPosErr=modRot*errLocal;          //Errors in x,y,z in LAB system
  fFarPlaneSlope=modRot(2*3+1)/modRot(1*3+1);                 //Slope of ROD 
  fFarPlaneOffset=modTrans.getTransVector().getZ() - fFarPlaneSlope * 
    modTrans.getTransVector().getY();                   //Rod's offset
}

//  void HKickCandidateEvaluator::transform(HTofHit *data,HGeomVector &r) {
//    //Transforms a TOF hit from the local coordinate system to the LAB
//    //coordinate system and smears the data acording to the resolution.
//    HGeomVector rLocal;
//    Double_t x,y,rodHeight;
//    if (!fTofGeometry) Fatal("transform","Tof geometry not set");
//    HModGeomPar *module=fTofGeometry->getModule(0,//data->getSector(),
//  					      data->getModule());
//    HGeomTransform &modTrans=module->getLabTransform();
//    HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
//    HGeomTransform &rodTrans=rodVol->getTransform();
//    rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - 
//  		       rodVol->getPoint(1)->getY());
  
//    x=data->getXpos();
//    if (isTofSimulated()) {
//      y = ((HTofHitSim *)data)->getY();
//    } else
//      y=0.;//gRandom->Uniform(rodHeight)-rodHeight/2.; // Smear y

//    r.setX(x);
//    r.setY(y);
//    r.setZ(0.0);
//    rLocal=rodTrans.transFrom(r);
//    r=modTrans.transFrom(rLocal);
//  }


ClassImp(HKickCandidateEvaluator)
