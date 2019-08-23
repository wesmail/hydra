//*-- Author : M. Sanchez
//*-- Modified : 22.08.2000
//*-- Modified : 06.03.2003 by D.Zovinec eloss for TOFINO added
#include "hkickcandidateevaluator.h"
#include "htofgeompar.h"
#include "hspecgeompar.h"
#include "hmdcgeompar.h"
#include "hkickimpact.h"
#include "htofhit.h"
#include "htofcluster.h"
#include "hshowerhit.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "htofhitsim2.h" //SIM
#include "hshowerhittof.h"
#include "hshowergeometry.h"
#include "hkicktrack.h"
#include "hmdchit.h"
#include "htofinogeompar.h"


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

  Float_t path,ldz,scal,lvec,zprojvec_lab,edep=-1.0,eloss;
  HGeomVector metanorm;
  Float_t tof;
  const HGeomVector &targetPos = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
  Double_t v,dist,C = 299792458;

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

  Double_t mass2=out.getMomentum()*out.getMomentum();
  mass2*= ( 1 - (v/C)*(v/C) );
  mass2/= (v/C)*(v/C);
  out.setMass(mass2);
  out.setTof(tof);
  // direction from kickplane to META
  out.setMETAdir(fFarDir);
  //eloss
  metanorm=fMetaNormVec;
  zprojvec_lab=fFarDir.scalarProduct(metanorm);
  metanorm*=zprojvec_lab;
  scal=fFarDir.scalarProduct(metanorm);
  ldz=metanorm.length();
  lvec=fFarDir.length();
  path=((ldz*lvec)/scal);
  if(hit->getAdcFlag() != 0){
    edep = hit->getEdep();
    if(path >= 1.0){
      eloss = edep/path;
    } else {
      Warning("evaluate","particle crossed TOF wall with path < 1.\n");
      eloss = -1.0;
    }
  } else {
    eloss = -1.0;
  }
  out.setMetaEloss(eloss);
  out.setOuterHit(hit);
  out.setInnerHit(imp);
  out.setBeta( v / C );
  out.setSystem(1);
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

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
				       HTofCluster *cluster,
				       HKickCandidate &out) {
  // Evaluates a candidate formed by a ToF cluster in the kick plane and
  // another one in the TOF. It internaly uses the generic evaluation
  // function but adds specific

  Float_t path,ldz,scal,lvec,zprojvec_lab,edep=-1.0,eloss;
  HGeomVector metanorm;
  Float_t tof;
  const HGeomVector &targetPos = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
  Double_t v,dist,C = 299792458;

  //code for TOF clusters (mass calculation)
  transform(cluster,fFarPos);

  evaluate(imp,out);

  //TOF specific stuff
  tof = cluster->getTof();
  if (tof<0) out.setMomentum(0);
  dist = (imp->position() - targetPos).length();
  dist += fDistanceToFarHit;     //distance from imp to tof hit
  dist /= 1000.;                 //In meters
  v = (dist/tof) * 1e9;          //TOF in ns

  Double_t mass2=out.getMomentum()*out.getMomentum();
  mass2*= ( 1 - (v/C)*(v/C) );
  mass2/= (v/C)*(v/C);
  out.setMass(mass2);
  out.setTof(tof);
  // direction from kickplane to META
  out.setMETAdir(fFarDir);
  //eloss
  metanorm=fMetaNormVec;
  zprojvec_lab=fFarDir.scalarProduct(metanorm);
  metanorm*=zprojvec_lab; // dz_lab is unit vector multiplied by length of vec in z_lab direction
  scal=fFarDir.scalarProduct(metanorm);
  ldz=metanorm.length();
  lvec=fFarDir.length();
  path=((ldz*lvec)/scal);

  if(cluster->getAdcFlag() != 0){
    edep = cluster->getEdep();
    if(path >= 1.0){
      eloss = edep/path;
    } else {
      Warning("evaluate","particle crossed TOF wall with path < 1.\n");
      eloss = -1.0;
    }
  } else {
    eloss = -1.0;
  }
  out.setMetaEloss(eloss);
  out.setOuterHit(cluster);
  out.setInnerHit(imp);
  out.setBeta( v / C );
  out.setSystem(1);
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
  HGeomVector r0_mod, rz_mod, r0_lab, rz_lab, dz_lab;

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

  // for eloss calculation
  r0_mod.setXYZ(0.0,0.0,0.0);
  rz_mod.setXYZ(0.0,0.0,1.0);
  r0_lab=modTrans.transFrom(r0_mod);
  rz_lab=modTrans.transFrom(rz_mod);
  fMetaNormVec=rz_lab-r0_lab;

  fFarPosErr=modRot*errLocal;          //Errors in x,y,z in LAB system
  fFarPlaneSlope=modRot(2*3+1)/modRot(1*3+1);                 //Slope of ROD
  fFarPlaneOffset=modTrans.getTransVector().getZ() - fFarPlaneSlope *
    modTrans.getTransVector().getY();                   //Rod's offset
}

void HKickCandidateEvaluator::transform(HTofCluster *data,HGeomVector &r) {
  // Transforms a TOF cluster from lab coordinate system into the local
  // coordinate system. Then takes it into the sector
  // coordinate system and smears the data acording to the resolution.
  // Sector coordinate system is mentioned as the LAB coordinate
  // system transformed into the sector 0.

  HGeomVector rLocal,errLocal;
  Float_t rodHeight,rodLength;
  Float_t xr, yr, zr;
  HGeomVector r0_mod, rz_mod, r0_lab, rz_lab, dz_lab;

  data->getXYZLab(xr,yr,zr);

  r.setX(xr);
  r.setY(yr);
  r.setZ(zr);

  HGeomVolume *tv=fSpecGeometry->getTarget(0);
  if (tv) r += tv->getTransform().getTransVector();   // correct for target position

  HModGeomPar *module=fTofGeometry->getModule(data->getSector(), //Sector
					      data->getModule());
  HGeomTransform &transMod = module->getLabTransform();
  HGeomVolume *rodVol1=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans1=rodVol1->getTransform();

  rLocal=transMod.transTo(r); //transform from LAB to module system
  r=rodTrans1.transTo(rLocal); //transform from module to local coordinate system

  HGeomTransform &transSec2 = fSpecGeometry->getSector(data->getSector())
    ->getTransform();

  HGeomTransform modTrans2(transMod);
  modTrans2.transTo(transSec2);
  HGeomVolume *rodVol2=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans2=rodVol2->getTransform();
  const HGeomRotation &modRot2=modTrans2.getRotMatrix();
  rodHeight=TMath::Abs(rodVol2->getPoint(0)->getY() - rodVol2->getPoint(1)->getY());
  rodLength = 2. * TMath::Abs(rodVol2->getPoint(0)->getX());

  if (!isTofSimulated()) {
    errLocal.setX(fTofXResolution);
    errLocal.setY(rodHeight/(TMath::Sqrt(12.)));
    errLocal.setZ(0.);
  }

  rLocal=rodTrans2.transFrom(r); //Rod to module system
  r=modTrans2.transFrom(rLocal); //Module to LAB system

  // for eloss calculation
  r0_mod.setXYZ(0.0,0.0,0.0);
  rz_mod.setXYZ(0.0,0.0,1.0);
  r0_lab=modTrans2.transFrom(r0_mod);
  rz_lab=modTrans2.transFrom(rz_mod);
  fMetaNormVec=rz_lab-r0_lab;

  fFarPosErr=modRot2*errLocal;          //Errors in x,y,z in LAB system
  fFarPlaneSlope=modRot2(2*3+1)/modRot2(1*3+1);                 //Slope of ROD
  fFarPlaneOffset=modTrans2.getTransVector().getZ() - fFarPlaneSlope *
    modTrans2.getTransVector().getY();                   //Rod's offset
}


///////////////////////////////////////////////////////////////////////////
//                   SHOWER evaluation
///////////////////////////////////////////////////////////////////////////
void HKickCandidateEvaluator::transform(HShowerHit *data) {
  //converts a Shower hit into a standard form for evaluation.
  Float_t x,y;
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
  Double_t v,dist,C=299792458;
  Float_t tof;
  HGeomVector r0_mod, rz_mod, r0_lab, rz_lab;
  Float_t path,ldz,scal,lvec,zprojvec_lab,edep=-1.0,eloss;
  HGeomVector metanorm;

  HGeomVector &kpos = imp->position();
  const HGeomVector &targetPos = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
  transform(hit);
  evaluate(imp,out);

  out.setMass(-10.0);
  out.setOuterHit(hit);
  out.setInnerHit(imp);
  out.setSystem(0);

  //Perform mass calculation
  //1.- Obtain hit position in Tofino for path lentgh
  //1.1 - Obtain equation of tofino plane y = a z + b
  HGeomVector posTofino;
  const HGeomTransform &transMod=fTofinoGeometry->getModule(hit->getSector(),
							    hit->getModule())
    ->getLabTransform();

  HGeomTransform &transSec = fSpecGeometry->getSector(hit->getSector())
    ->getTransform();

  HGeomTransform trans(transMod);
  trans.transTo(transSec);
  const HGeomRotation &trot = trans.getRotMatrix();

  Double_t a = trot(1*3+1)/trot(2*3+1);  // Tofino plane: y = a z + b
  Double_t b = (trans.getTransVector().getY()) - 
    a * (trans.getTransVector().getZ());

  //1.2 - Calculate intersection of outer segment with Tofino
  HGeomVector &alpha = fFarDir;
  Double_t bx = alpha.getX() / alpha.getZ();
  Double_t by = alpha.getY() / alpha.getZ();
 
  posTofino.setZ( (kpos.getY() - by * kpos.getZ() - b) / (a - by) );
  posTofino.setY( kpos.getY() + by * (posTofino.getZ() - kpos.getZ()) );
  posTofino.setX( kpos.getX() + bx * (posTofino.getZ() - kpos.getZ()) );

  //2.- Calculate mass 
  Int_t mult = ((HShowerHitTof *)hit)->getTofinoMult(); //FIXME: no cast
  tof = ((HShowerHitTof *)hit)->getTof(); //FIXME: no cast
  dist = (imp->position() - targetPos).length(); 
  dist += (posTofino - imp->position()).length();
  dist /= 1000.;                 //In meters
  v = (dist/tof) * 1e9;          //TOF in ns
  
  out.setTof(tof);
  out.setBeta( v / C );  

  if ( mult == 1) {
    Double_t mass2=out.getMomentum()*out.getMomentum();
    mass2*= ( 1 - (v/C)*(v/C) );
    mass2/= (v/C)*(v/C); 
    out.setMass(mass2);
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
  } else out.setMass(-2000);

  // direction from kickplane to META
  out.setMETAdir(fFarDir);
  
  // 3. - Calculate energy loss per path length in tofino
  // 3.1. - calculation of vector normal to tofino paddle
  r0_mod.setXYZ(0.0,0.0,0.0);
  rz_mod.setXYZ(0.0,0.0,1.0);
  r0_lab=trans.transFrom(r0_mod);
  rz_lab=trans.transFrom(rz_mod);
  fMetaNormVec=rz_lab-r0_lab;

  // 3.2. - calculation of path correction in tofino
  metanorm=fMetaNormVec;
  zprojvec_lab=fFarDir.scalarProduct(metanorm);
  metanorm*=zprojvec_lab;
  scal=fFarDir.scalarProduct(metanorm);
  ldz=metanorm.length();
  lvec=fFarDir.length();
  path=((ldz*lvec)/scal);

  // 3.3. - calculation of corrected energy loss
  edep = ((HShowerHitTof *)hit)->getADC(); //FIXME: no cast;
  if(path >= 1.0){
    eloss = edep/path;
  } else {
    Warning("evaluate","particle crossed TOFINO with path < 1.\n");
    eloss = -1.0;
  }
  out.setMetaEloss(eloss);

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                       Common evaluation
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void HKickCandidateEvaluator::evaluate(HKickImpact *imp,
				       HKickCandidate &out) {
  //Generic evaluation of a candidate
  Float_t deltaPhi=0,errPtGhi=0,errPtPhi,itanphi,deltaX=1000.;
  Float_t pGhi,errPGhi,D,errP;
  Int_t polarity;

  //Set fFarDir to the direction after the magnet
  fFarDir=fFarPos-imp->position();
  fDistanceToFarHit=fFarDir.length();
  fFarDir/=fDistanceToFarHit;

  //Calculate incident and output angles
  fGhi1=TMath::ATan2(imp->direction().getZ(),imp->direction().getY()); 
  fGhi2=TMath::ATan2(fFarDir.getZ(),fFarDir.getY());
  Float_t et1 = TMath::ATan2(imp->direction().getX(),imp->direction().getZ());

  //Calculate deflection
  fSinGhi=sin((fGhi2-fGhi1)/2.);

  //Look to the sign of the deflection to determine polarity
  polarity = (fSinGhi >= 0.) ? 1 : -1;

  //Calculate momentum both from deflection in phi and ghi
  pGhi=fKickPlane->getP(imp->position(),2*fSinGhi,polarity);

  //Error in Pghi
  D = ((fFarPos.getY() - imp->position().getY()) * 
       (fFarPos.getY() - imp->position().getY()) +
       (fFarPos.getZ() - imp->position().getZ()) * 
       (fFarPos.getZ() - imp->position().getZ()));

  errPGhi = (TMath::Cos((fGhi2 - fGhi1) / 2.) * 
	     (imp->position().getZ() - fFarPlaneSlope*imp->position().getY() - 
	      fFarPlaneOffset) * fFarPosErr.getY() ) / (2 * fSinGhi * D);
# warning "this calculation has to be checked : errPtGhi is never initialized !!!!!!!!!!!!"
  errPGhi = TMath::Abs(pGhi)*TMath::Sqrt(errPtGhi*errPtGhi + errPGhi*errPGhi);
  
  //*** Calculate momentum **********************
  fP = pGhi;
  errP = errPGhi;
  if (fP>0.) { //Expected fPredictedSinPhi
    fPredictedSinPhi = fMatchPar->getDeflection(imp->position(),fP,
						errPtPhi,polarity);
    if (fPredictedSinPhi<=1.) {
      itanphi = TMath::Tan(2*TMath::ASin(fPredictedSinPhi)+et1);
      
      if (TMath::Abs(itanphi) > 0.) {
	deltaX = fFarPos.getX() - imp->position().getX() - itanphi * 
	  (fFarPos.getZ() - imp->position().getZ());
      } else {
        #if DEBUG_LEVEL>1
	Warning("evaluate","Division by 0");      
        #endif
	deltaX = 1000.*fFarPosErr.getX();
      }
    } else {deltaPhi=1000; deltaX=1000.*fFarPosErr.getX(); }
  } else {deltaPhi=1000; deltaX=1000.*fFarPosErr.getX(); }

  
  //*** Set output values ******************
  out.setPTof(-1.);
  out.setPTofErr(0.);
  out.setPID(HKickTrack::unknownId);
  out.setMomentum(fP); 
  out.setDenominator(2 * fSinGhi);
  out.setXPull(deltaX / fFarPosErr.getX() );
  out.setPolarity(polarity);
  out.setMomentumError(errP); 
  out.setSector(imp->getSector());
  out.setPosition1(fFarPos.getX(),fFarPos.getY(),fFarPos.getZ());
//   out.setPosition1(imp->position().getX(),
// 		   imp->position().getY(),
// 		   imp->position().getZ()) ;
}

ClassImp(HKickCandidateEvaluator)
