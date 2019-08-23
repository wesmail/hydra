#include "hrtmetamatch.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hades.h"
#include "hevent.h"
#include "hmatrixcategory.h"
#include "hspectrometer.h"
#include "hkicktrack.h"
#include "kickdef.h"
#include "haddef.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdcdef.h"
#include "hrtmetatrack.h"
#include "hrtmdctrk.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "hrtmatchingpar.h"

#undef LOCAL_PARAM_FILE

//*-- Author : M. Sanchez

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
//  HRtMetaMatch
//
//   This class takes the output of the tracking in the MDC system (inner
// and outer) and matches those tracks to hits in TOF and Shower/Tofino. 
// The momentum comes from the MDC, while KickMatch adds velocity and 
// mass calculations.
/////////////////////////////////////////////////////////////////////

HRtMetaMatch::HRtMetaMatch(Text_t name[], Text_t title[]) : 
  HReconstructor(name,title) {
 fMdcCat = 0; 
 fTrackCat = 0;
 fHitLoc.set(2,0,2);
 fSecLoc.set(1,0);
}

HRtMetaMatch::~HRtMetaMatch(void) {

}

Bool_t HRtMetaMatch::init(void) {
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    HEvent *event = gHades->getCurrentEvent();
//    HSpectrometer *spec = gHades->getSetup(); 
    if (event && rtdb) {
      fCorrelator.init(event,rtdb);

      fMdcCat = event->getCategory(catMdcTrack);
      if (!fMdcCat) {
	Error("init","KickMdcTrk category not found!");
	return kFALSE;
      }
      fMdcIter = dynamic_cast<HIterator *>(fMdcCat->MakeIterator());

      fMdcGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
      
      fTrackCat = event->getCategory(catKickMdcTrk);
      if (!fTrackCat) {
	Int_t ssize[2]={6,100};
	fTrackCat = new HMatrixCategory("HRtMetaTrack",2,ssize,0.5);
	if (fTrackCat) event->addCategory(catKickMdcTrk,
					  fTrackCat,
					  "Tracks");
      }

      fKickplane = (HKickPlane2 *)(rtdb->getContainer("KickPlane2Meta"));
      fMatchpar = (HKickMatchPar *)rtdb->getContainer("KickMatchParMeta");

#ifndef LOCAL_PARAM_FILE
      fMdcMetaMatchPar = (HRtMatchingPar *)
	rtdb->getContainer("RtMatchingParMeta");
#else
      Warning("init","Using hardcoded parameter file!!");
      TFile f("params.root");
      fMdcMetaMatchPar = (HRtMatchingPar *)gFile->Get("RtMatchingParMeta");
#endif

      if (fDebug) {
	Hades::instance()->getOutputFile()->cd();
	fControl = new TNtuple("metamatch_con","metamatch_con","pmeta:pm3:dPnorm:xPull:massMeta:massM3:dsx:dsy:dphi:theta:phi:tof:charge:sector:sys");
      }

    } else {
      Error("init","! rtdb && event && spec");
      return kFALSE;
    }
  } else {
    Error("init","gHades does not exist");
    return kFALSE;
  }


  return kTRUE;
}

Int_t HRtMetaMatch::execute(void) {
  HRtMdcTrk *track=0;
  
  fHitLoc[1] = 2; // Look at MDC3
  fHitLoc[0] = 0;
  for (fSecLoc[0] = 0; fSecLoc[0] < 6; fSecLoc[0]++) {
    fHitLoc[0] = fSecLoc[0];
    fCorrelator.setSector(fSecLoc[0]);
    fCorrelator.readMetaPoints();
    
    fMdcIter->Reset();
    fMdcIter->gotoLocation(fSecLoc);
    
    while ( (track = (HRtMdcTrk *)fMdcIter->Next()) != 0) {
      for (Int_t i=0;i<fCorrelator.getNMetaPoints();i++) {
	HRtMetaPoint &point = fCorrelator.getMetaPoint(i);
	evaluate(track,point);
      }
    }
  }
  return 0;
}

void HRtMetaMatch::evaluate(HRtMdcTrk *track, HRtMetaPoint &point) {
  // Proceeds by obtaining momentum from meta and correlation between
  //azimuthal and polar deflection. These two variables are added to
  //the 3 coming from track to perform a 5D cut
  HGeomVector r,r0,alpha0,inter;
  Float_t theta,phi;
  Float_t pi2 = TMath::Pi() / 2.;
  Float_t g1,g2,pmeta,angle;
  Int_t pol;
  Float_t et1,et2;
  Float_t errPtPhi;
  Float_t itanphi,deltaX;
  Float_t predictedSinPhi;
  HGeomVector dir;
  Int_t system=point.getSystem();
  Float_t matchVar[5]; //The variables used for matching

  r.setXYZ(point.getX(), point.getY(), point.getZ());

  //Calculate momentum from Meta.
  //   1.- Calculate normalized representation of impining straigth line
  theta = track->getTheta();
  phi = track->getPhi();
  r0.setX(track->getR() * TMath::Cos(phi + pi2));
  r0.setY(track->getR() * TMath::Sin(phi + pi2));
  r0.setZ(track->getZ());
  
  alpha0.setX(TMath::Sin(theta) * TMath::Cos(phi));
  alpha0.setY(TMath::Sin(theta) * TMath::Sin(phi));
  alpha0.setZ(TMath::Cos(theta));  

  //  2.- Calculate intersection with kickplane
  fKickplane->calcIntersection(r0,alpha0,inter);

  //  3.- Calculate deflection angle and momentum.from Meta
  dir = r - inter;
  dir /= dir.length();
  g1 = TMath::ATan2(alpha0.getZ(),alpha0.getY());
  g2 = TMath::ATan2(dir.getZ(),dir.getY());
  angle = g2 - g1;
  pol = ((angle)>=0)?1:-1;
  pmeta = fKickplane->getP(inter,2*TMath::Sin(angle/2.),pol);

  //Compare MDC3 and Meta momenta. Obtain normalized difference
  Float_t dPnorm = track->getP()*(1/pmeta - 1/track->getP());
  /*Float_t dPnorm = 1 - track->getP() / pmeta;
  if (system==0) { //Shower
    dPnorm /= 0.006 + 9e-5 * track->getP();
  } else { //Tof
    dPnorm /= 0.01 + 1.4e-4 * track->getP();
  }*/

  //Obtain azimuthal deflection from meta
  et1 = TMath::ATan2(alpha0.getX(),alpha0.getZ());
  et2 = TMath::ATan2(dir.getX(),dir.getZ());
  predictedSinPhi = fMatchpar->getDeflection(inter,pmeta,errPtPhi,pol);
  if (predictedSinPhi>1) { //Error calculating deflection
    deltaX = 29.;
  } else {
    itanphi = TMath::Tan(2*TMath::ASin(predictedSinPhi)+et1);
    if (TMath::Abs(itanphi) > 0.) {
      deltaX = r.getX() - inter.getX() - itanphi * (r.getZ() - inter.getZ());
    } else deltaX = 1000;
    deltaX /= point.getErrors().getX();  
  }

  //Check cuts
  /*
  matchVar[0] = fabs(deltaX);
  matchVar[1] = fabs(dPnorm);
  matchVar[2] = fabs(track->getDKick());
  matchVar[3] = fabs(track->getDPhi());
  */
  matchVar[0] = track->getD();
  matchVar[1] = track->getDKick();
  matchVar[2] = track->getDPhi();
  matchVar[3] = deltaX;
  matchVar[4] = dPnorm;

  if (fDebug) {
    Double_t dist,beta,m2;
    Double_t tof;
    Double_t C = 299792458.0;
    HGeomVector rOut;
    Float_t pMdc3;
    Float_t m2Meta,m2Mdc3;
    Float_t scaling;

    pMdc3 = track->getP();

    tof = point.getTof();
    rOut.setX(point.getX());
    rOut.setY(point.getY());
    rOut.setZ(point.getZ()); 
    dist = (rOut - track->getOuterPos()).length();
    scaling = track->getLeverArm() / (dist + track->getLeverArm()); 
    dist += track->getLength();
    dist /=  1000.;
    if (tof>.1) { //Sometimes we don't have TOF
      beta = (dist / tof) * (1.0e9/C); 
      m2 = ( 1/(beta*beta) - 1);
      m2Meta = pmeta * pmeta * m2;
      m2Mdc3 = pMdc3 * pMdc3 * m2;
    } else {
      m2Meta = -1000;
      m2Mdc3 = -1000;
    }
    fControl->Fill(pmeta,track->getP(),dPnorm,deltaX,m2Meta,m2Mdc3,track->getD(),track->getDKick(),track->getDPhi(),theta,phi,tof,pol,track->getSector(),system);
  }

  if (fIgnoreCuts || fMdcMetaMatchPar->bin(matchVar) != 0) {
    fillData(track,point,pmeta);
  }
}

void HRtMetaMatch::fillData(HRtMdcTrk *track, HRtMetaPoint &point, 
			    Float_t pMeta) {
  //Fills output data in the output category
  HRtMetaTrack *c=0;
  TObject *slot=0;
  Double_t dist,beta=-1.,m2;
  Double_t tof;
  Double_t C = 299792458.0;
  HGeomVector rOut;
  Float_t p,pMdc3;
  Float_t m2Meta,m2Mdc3;
  Float_t scaling;

  pMdc3 = track->getP();
  p = .5*pMeta +.5*(pMdc3);  //FIXME: use average of pMdc3 and pMeta

  tof = point.getTof();
  rOut.setX(point.getX());
  rOut.setY(point.getY());
  rOut.setZ(point.getZ()); 
  dist = (rOut - track->getOuterPos()).length();
  scaling = track->getLeverArm() / (dist + track->getLeverArm()); 
  dist += track->getLength();
  dist /=  1000.;
  if (tof>.1) { //Sometimes we don't have TOF
    beta = (dist / tof) * (1.0e9/C); 
    m2 = ( 1/(beta*beta) - 1);
    m2Meta = pMeta * pMeta * m2;
    m2Mdc3 = pMdc3 * pMdc3 * m2;
    m2 = p * p * m2;
  } else {
    m2Meta = -1000;
    m2Mdc3 = -1000;
    m2 = -1000;
  }

  slot = fTrackCat->getNewSlot(fSecLoc);
  if (slot) {
    c = new (slot) HRtMetaTrack;
    c->set(p, m2, tof,dist);
    c->setSystem(point.getSystem());
    c->setPull(fCorrelator.getDistance()); 
    c->setResXY(fCorrelator.getResX(),fCorrelator.getResY());
    c->setPMdc3(pMdc3);
    c->setPMeta(pMeta);
    c->setMass(m2Meta,m2Mdc3);
    c->setDirection(track->getTheta(),track->getPhi());
    c->setDistances(track->getDKick(),track->getD());
    c->setTargetPos(track->getR(),track->getZ());
    c->setScaling(scaling);
    c->setBeta(beta);
    c->setSector(track->getSector());
    c->setPolarity(track->getCharge());
  }
}

Bool_t HRtMetaMatch::finalize(void) {
  fCorrelator.finalize();
  if (fDebug) {
    Hades::instance()->getOutputFile()->cd();
    fControl->Write();
  }
  return kTRUE;
}

ClassImp(HRtMetaMatch)

