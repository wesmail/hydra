#include "hrtmetamatcheff.h"
#include "hades.h"
#include "hevent.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hmdcdef.h"
#include "hiterator.h"
#include "hrtmdctrk.h"
#include "hmdcgeompar.h"
#include "hlocation.h"
#include <TNtuple.h>
#include "hshowerhittoftrack.h"
#include "htofhitsim.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hspecgeompar.h"
#include "hgeomvolume.h"

#undef USE_EXTERNAL_PAR_FILE

HRtMetaMatchEff::HRtMetaMatchEff(Text_t name[], Text_t title[]) : 
  HReconstructor(name,title) {
  fControl=0;
  fMdcCat=0;
  fMdcIter=0;
  fMdcGeometry=0;
  fSecLoc.set(1,0);
  fHitLoc.set(2,0,0);

#ifdef USE_EXTERNAL_PAR_FILE
  FILE *fp=fopen("resol_pm3_pmeta_shower.txt","r");
  fscanf(fp,"%f %f",&fResolPar[0][0],&fResolPar[0][1]);
  fclose(fp);
  
  fp = fopen("resol_pm3_pmeta_tof.txt","r");
  fscanf(fp,"%f %f",&fResolPar[1][0],&fResolPar[1][1]);
  fclose(fp);
#else
  fResolPar[0][0] = 0.006; //Shower
  fResolPar[0][1] = 9e-5;

  fResolPar[1][0] = 0.01;
  fResolPar[1][1] = 1.4e-4;
#endif
}

HRtMetaMatchEff::~HRtMetaMatchEff(void) {

}

Bool_t 
HRtMetaMatchEff::init(void) {
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    HEvent *event = gHades->getCurrentEvent();

    if (event && rtdb) {
      fCorrelator.init(event,rtdb);

      fMdcCat = event->getCategory(catMdcTrack);
      if (!fMdcCat) {
	Error("init","KickMdcTrk category not found!");
	return kFALSE;
      }
      fMdcIter = dynamic_cast<HIterator *>(fMdcCat->MakeIterator());

      fMdcGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
      fSpecGeometry=(HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

      fKineCat = event->getCategory(catGeantKine);
      if (!fKineCat) Error("init","Geant info not found!");

      fKickplane = (HKickPlane2 *)rtdb->getContainer("KickPlane2Meta");
      fMatchpar = (HKickMatchPar *)rtdb->getContainer("KickMatchParMeta");
    } else {
      Error("init","! rtdb && event && spec");
      return kFALSE;
    }
  } else {
    Error("init","gHades does not exist");
    return kFALSE;
  }

  gHades->getOutputFile()->cd();
  fControl = new TNtuple("metamatcheff","metamatcheff",
			 "valid:p:pgemdc:pm3:pmeta:xPull:sys:dsx:dsy:dphi:dPnorm:geTheta:gePhi:prec:theta:phi:massMeta:massM3:pid:tof");

  return kTRUE;
}

Int_t HRtMetaMatchEff::execute(void) {
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
      for (int i=0;i<fCorrelator.getNMetaPoints();i++) {
	HRtMetaPoint &point = fCorrelator.getMetaPoint(i);
	fillControl(track,point);
      }
//       point = fCorrelator.findClosestPoint(track->getOuterPos(),
// 					   track->getOuterDir(),
// 					   track->getLeverArm());
//       fControl->Fill(calcValidity(track,point),
// 		     dX,dY,fCorrelator.getDistance());
    }
  }
  return 0;
}

void HRtMetaMatchEff::fillControl(HRtMdcTrk *track, HRtMetaPoint &point) {
  HRtMdcTrkSim *hitsim = (HRtMdcTrkSim *)track;
  Int_t lsTracks[5];
  UChar_t lsTimes[5];
  Int_t nTracks = 0;
  Int_t mostCommonTrackTimes = 0;
  Int_t mostCommonTrack = 0;
  Int_t totalTimes = 0;
  Int_t parentTrack=0;
  Float_t valid;
  Float_t momentum;
  Float_t mdcMomentum;
  HGeomVector r,r0,alpha0,inter;
  Float_t bX,bY,dX,dY,theta,phi;
  HGeomVector &pos = track->getOuterPos();
  HGeomVector &alpha = track->getOuterDir();
  Float_t pi2 = TMath::Pi() / 2.;
  Float_t g1,g2,pmeta,angle;
  Int_t pol;
  Float_t et1,et2;
  Float_t errPtPhi;
  Float_t itanphi,deltaX;
  Float_t predictedSinPhi;
  HGeomVector dir;
  HGeantKine *kine = 0;
  Float_t system=0.;
  Float_t var[20];
  Float_t geThetaSec=0,gePhiSec=0;
  HGeomVector vecGeMomentum;
  Int_t tmp,pid;

  //Extrapolate to Meta and calculate residuals
  r.setXYZ(point.getX(), point.getY(), point.getZ());
  
  bX = alpha.getX() / alpha.getZ();
  bY = alpha.getY() / alpha.getZ();
  
  dX = pos.getX() + bX * (r.getZ() - pos.getZ());
  dX -= r.getX();	  
  dY = pos.getY() + bY * (r.getZ() - pos.getZ());
  dY -= r.getY();

  //Calculate common tracks
  //   1.- Put tracks from TOF/Shower into a list
  if (point.getSystem()==0) { //Shower
    HShowerHitTofTrack *track = (HShowerHitTofTrack *)point.getMetaHit();
    lsTracks[0] = track->getTrack();
    lsTimes[0] = 1;
    nTracks = 1;
    system = 0.;
  } else {
    HTofHitSim *tofhit = (HTofHitSim *)point.getMetaHit();
    lsTracks[0] = tofhit->getNTrack1();
    lsTimes[0] = 1;
    if (tofhit->getNTrack2()>0) {
      nTracks = 2;
      lsTracks[1] = tofhit->getNTrack2();
      lsTimes[1] = 1;
    } else {
      nTracks = 1;
    }
    system=1.;
  }

  //  2.- Obtain common tracks
  for (int i=0; i<nTracks; i++) {
    for (int j=0; j<hitsim->getNTracks(); j++) {
      if (lsTracks[i] == hitsim->getTrack(j)) {
	lsTimes[i]+=hitsim->getNTimes(j);
	if (mostCommonTrackTimes<lsTimes[i]) {
	  mostCommonTrackTimes = lsTimes[i];
	  mostCommonTrack = lsTracks[i];
	}
      }
//    else {
// 	if (nTracks+addedTracks<6) {
// 	  lsTracks[nTracks+addedTracks] = hitsim->getTrack(j);
// 	  lsTimes[nTracks+addedTracks] = hitsim->getNTimes(j);
// 	  addedTracks++;
// 	}
//       }
    }
  }
  //  nTracks = nTracks + addedTracks;

  //  3.- If no common tracks look to parents
  if (mostCommonTrack == 0) {
    for (int i=0; i<nTracks; i++) {
      kine = (HGeantKine *)fKineCat->getObject(lsTracks[i] - 1);
      parentTrack = kine->getParentTrack();
      for (int j=0; j<hitsim->getNTracks(); j++) {
	if (parentTrack == hitsim->getTrack(j)) {
	  mostCommonTrack = parentTrack;
	  mostCommonTrackTimes = 10*(hitsim->getNTimes(j)+1);
	}
      }
    }
  }
  
  //Compute total number of impacts
  totalTimes = nTracks;
  for (int i=0; i<hitsim->getNTracks();i++)
    totalTimes += hitsim->getNTimes(i);

  //Calculate weight of common track
  valid = float(mostCommonTrackTimes) / float(totalTimes);

  //Obtain momentum from common track, or -1
  if (mostCommonTrack>0) {
    Float_t geX,geY,geTof;
    HGeantMdc *geMdc;
    kine = static_cast<HGeantKine *>(fKineCat->getObject(mostCommonTrack-1));
    kine->getParticle(tmp,pid);
    momentum = kine->getTotalMomentum();
    kine->resetMdcIter();
    while (( geMdc = (HGeantMdc *)kine->nextMdcHit()) != 0) {
      if (geMdc->getModule() == 2 && geMdc->getLayer()==6){
	geMdc->getHit(geX,geY,geTof,mdcMomentum);
      }
    }
  } else {
    momentum=-1;
    mdcMomentum = -1;
  }

  //Obtain sector theta and phi for common track. Sector is taken from HMdcTrk
  if (mostCommonTrack>0) {
	  HGeomVector geAlpha0Sec;
	  HGeomTransform &transSec = fSpecGeometry->getSector(hitsim->getSector())->getTransform();
	  kine = static_cast<HGeantKine *>(fKineCat->getObject(mostCommonTrack - 1));
	  kine->getMomentum(vecGeMomentum);
	  vecGeMomentum /= vecGeMomentum.length();
	  geAlpha0Sec = transSec.transTo(vecGeMomentum);
	  geThetaSec = acos(geAlpha0Sec.getZ());
	  gePhiSec = TMath::ATan2(geAlpha0Sec.getY(),geAlpha0Sec.getX());
  }


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

  //Obtain azimuthal deflection from meta
  et1 = TMath::ATan2(alpha0.getX(),alpha0.getZ());
  et2 = TMath::ATan2(dir.getX(),dir.getZ());
  predictedSinPhi = fMatchpar->getDeflection(inter,pmeta,errPtPhi,pol);
  if (predictedSinPhi>1) {
    deltaX = 29;
  } else {
    itanphi = TMath::Tan(2*TMath::ASin(predictedSinPhi)+et1);
    if (TMath::Abs(itanphi) > 0.) {
      deltaX = r.getX() - inter.getX() - itanphi * 
	(r.getZ() - inter.getZ());
    } else deltaX = 1000;
    deltaX /= point.getErrors().getX(); 
  }

  //Compare MDC3 and Meta momenta. Obtain normalized difference
  Float_t dPnorm = 1 - track->getP() / pmeta;
  if (system==0) { //Shower
    dPnorm /= fResolPar[0][0] + fResolPar[0][1] * track->getP();
  } else { //Tof
    dPnorm /= fResolPar[1][0] + fResolPar[1][1] * track->getP();
  }

  //Compute masses
  Double_t dist,beta,m2;
  Double_t tof;
  Double_t C = 299792458.0;
  HGeomVector rOut;
  Float_t p,pMdc3;
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
    m2 = p * p * m2;
  } else {
    m2Meta = -1000;
    m2Mdc3 = -1000;
    m2 = -1000;
  }

  //Fill data to output
  var[0] = valid;
  var[1] = momentum;
  var[2] = mdcMomentum;
  var[3] = track->getP();
  var[4] = pmeta;
  var[5] = deltaX;
  var[6] = system;
  var[7] = track->getD();
  var[8] = track->getDKick();
  var[9] = track->getDPhi();
  var[10] = dPnorm;
  var[11] = geThetaSec;
  var[12] = gePhiSec;
  var[13] = 0.5*track->getP() + 0.5*pmeta;
  var[14] = track->getTheta(); 
  var[15] = track->getPhi();
  var[16] = m2Meta;
  var[17] = m2Mdc3;
  var[18] = pid;
  var[19] = tof;

  fControl->Fill(var);
}

Bool_t HRtMetaMatchEff::finalize(void) {
  return kTRUE;
}

ClassImp(HRtMetaMatchEff)
