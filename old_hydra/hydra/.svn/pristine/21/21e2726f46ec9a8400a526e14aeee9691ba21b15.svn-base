#include "hrtmetamatch.h"
#include "hcategory.h"
#include "hlocation.h"
#include "htofgeompar.h"
#include "hiterator.h"
#include "hshowergeometry.h"
#include "hruntimedb.h"
#include "hades.h"
#include "hevent.h"
#include "hmatrixcategory.h"
#include "hspectrometer.h"
#include "hspecgeompar.h"
#include "hkicktrack.h"
#include "hkickmdctrk.h"
#include "tofdef.h"
#include "kickdef.h"
#include "showertofinodef.h"
#include "haddef.h"
#include "htofhit.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hshowerhittof.h"
#include "hmdcdef.h"
#include "hrtmetatrack.h"
#include "hrtmdctrk.h"
#include "hkickplane2.h"

#include "htofhitsim2.h"

//*-- Author : M. Sanchez

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
 fTofCat = 0;
 fShowerCat = 0;
 fTrackCat = 0;
 fMetaPoints = new TClonesArray("HRtMetaPoint",50); // Array of HGeomVector
 fSecLoc.set(1,0);
 fHitLoc.set(2,0,2);
}

HRtMetaMatch::~HRtMetaMatch(void) {

}

Bool_t HRtMetaMatch::init(void) {
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    HEvent *event = gHades->getCurrentEvent();
//    HSpectrometer *spec = gHades->getSetup(); 
    if (event && rtdb) {
      
      fTofCat = event->getCategory(catTofHit);
      if (!fTofCat) {
	Error("init","Tof Hit category not found!");
	return kFALSE;
      }
      fTofIter = dynamic_cast<HIterator *>(fTofCat->MakeIterator());

      fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");

      fMdcCat = event->getCategory(catMdcTrack);
      if (!fMdcCat) {
	Error("init","KickMdcTrk category not found!");
	return kFALSE;
      }
      fMdcIter = dynamic_cast<HIterator *>(fMdcCat->MakeIterator());

      fMdcGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
      
      fShowerCat = event->getCategory(catShowerHitTof);
      if (!fShowerCat) {
	Error("init","Shower/Tofino hit category not found!");
	return kFALSE;
      }
      fShowerIter = dynamic_cast<HIterator *>(fShowerCat->MakeIterator());

      fShowerGeometry= (HShowerGeometry *)rtdb->getContainer("ShowerGeometry");

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

      fTrackCat = event->getCategory(catKickMdcTrk);
      if (!fTrackCat) {
	Int_t ssize[2]={6,100};
	fTrackCat = new HMatrixCategory("HRtMetaTrack",2,ssize,0.5);
	if (fTrackCat) event->addCategory(catKickMdcTrk,
					  fTrackCat,
					  "Tracks");
      }

      fKickplane = (HKickPlane2 *)(rtdb->getContainer("KickPlane2Meta"));
    } else {
      Error("init","! rtdb && event && spec");
      return kFALSE;
    }
  } else {
    Error("init","gHades does not exist");
    return kFALSE;
  }

  fOut = new TFile("match.root","RECREATE");
  fControl = new TNtuple("cont","control","xf:yf:zf:d:dx:dy:valid:sys:xm:ym:zm:xk:yk:zk");
  return kTRUE;
}

Int_t HRtMetaMatch::execute(void) {
  HRtMdcTrk *track=0;
  HRtMetaPoint *point=0;

  fMaxDist = 1400.;
  fHitLoc[1] = 2; // Look at MDC3
  fHitLoc[0] = 0;
  for (fSecLoc[0] = 0; fSecLoc[0] < 6; fSecLoc[0]++) {
    fHitLoc[0] = fSecLoc[0];
    readMetaPoints();
    
    fMdcIter->Reset();
    fMdcIter->gotoLocation(fSecLoc);
    
    while ( (track = (HRtMdcTrk *)fMdcIter->Next()) != 0) {
      point = findClosestPoint(track->getOuterPos(),track->getOuterDir(),
			       track->getLeverArm());
      if (point != 0) {
	fillData(track,point);
      }
    }
  }
  return 0;
}

void HRtMetaMatch::fillData(HRtMdcTrk *track, HRtMetaPoint *point) {
  //Fills output data in the output category
  HRtMetaTrack *c=0;
  TObject *slot=0;
  Double_t dist,beta,m2;
  Double_t tof;
  Double_t C = 299792458.0;
  HGeomVector rOut;
  Float_t p,pMdc3,pMeta;
  Float_t m2Meta,m2Mdc3;
  Float_t scaling;

  pMdc3 = track->getP();
  pMeta = calcMomentum(track,point);
  p = .5*pMeta +.5*(pMdc3);  //FIXME: use average of pMdc3 and pMeta

  tof = point->getTof();
  rOut.setX(point->getX());
  rOut.setY(point->getY());
  rOut.setZ(point->getZ()); 
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
    c->setSystem(point->getSystem());
    c->setPull(fLatestDistance); //FIXME: There are better ways than a global.
    c->setResXY(fResX,fResY);
    c->setPMdc3(pMdc3);
    c->setPMeta(pMeta);
    c->setMass(m2Meta,m2Mdc3);
    c->setDirection(track->getTheta(),track->getPhi());
    c->setDistances(track->getDKick(),track->getD());
    c->setTargetPos(track->getR(),track->getZ());
    c->setScaling(scaling);
  }
}

Float_t HRtMetaMatch::calcMomentum(HRtMdcTrk *track, HRtMetaPoint *point) {
  HGeomVector r,alpha,inter;
  HGeomVector dir(point->getX(),point->getY(),point->getZ());
  Float_t theta,phi;
  Float_t pi2 = TMath::Pi() / 2.;
  Float_t angle; //deflection angle
  Float_t p=-1.;
  Float_t g2,g1;
  Int_t pol;

  //Calculate normalized representation of a straigth line
  theta = track->getTheta();
  phi = track->getPhi();
  r.setX(track->getR() * TMath::Cos(phi + pi2));
  r.setY(track->getR() * TMath::Sin(phi + pi2));
  r.setZ(track->getZ());
  
  alpha.setX(TMath::Sin(theta) * TMath::Cos(phi));
  alpha.setY(TMath::Sin(theta) * TMath::Sin(phi));
  alpha.setZ(TMath::Cos(theta));

  //Calculate intersection with kickplane
  fKickplane->calcIntersection(r,alpha,inter);

  //Calculate deflection angle and momentum.
  dir -= inter;
  dir /= dir.length();
  g1 = TMath::ATan2(alpha.getZ(),alpha.getY());
  g2 = TMath::ATan2(dir.getZ(),dir.getY());
  angle = g2 - g1;
  pol = ((angle)>=0)?1:-1;
  p = fKickplane->getP(inter,2*TMath::Sin(angle/2.),pol);

  //Exit
  return p;
}

HRtMetaPoint *HRtMetaMatch::findClosestPoint(HGeomVector &r0, 
					     HGeomVector &alpha,
					     Float_t leverArm) {
  // Finds closest hit in TOF/Shower to a given track within a distance limit.
  Int_t n = fMetaPoints->GetEntriesFast();
  Float_t dmin = fMaxDist;
  HRtMetaPoint *pMin = 0;
  Float_t d,dX,dY,bX,bY;
  HGeomVector r;

  for (Int_t i = 0; i < n; i++) {
    HRtMetaPoint &point = *((HRtMetaPoint *)fMetaPoints->UncheckedAt(i));

    r.setX(point.getX());
    r.setY(point.getY());
    r.setZ(point.getZ());
    
    d = (r0 - r).vectorProduct(alpha).length();
  
    bX = alpha.getX() / alpha.getZ();
    bY = alpha.getY() / alpha.getZ();
    dX = r0.getX() + bX * (r.getZ() - r0.getZ());
    dX -= r.getX();

    dY = r0.getY() + bY * (r.getZ() - r0.getZ());
    dY -= r.getY();

    if (d < dmin) {
	dmin = d;
	pMin = (HRtMetaPoint *)fMetaPoints->UncheckedAt(i);
    }

    fResX = dX;
    fResY = dY;
    fControl->Fill(r.getX(),r.getY(),r.getZ(),
		   d,dX,dY,(pMin == 0)?0:1,point.getSystem(),
		   r0.getX(),r0.getY(),r0.getZ(),
		   r0.getX() - leverArm * alpha.getX(),
		   r0.getY() - leverArm * alpha.getY(),
		   r0.getZ() - leverArm * alpha.getZ());
  }
  fLatestDistance = dmin; //FIXME: there are better ways.
  return pMin;
}

void HRtMetaMatch::readMetaPoints(void) {
  // Read points from TOF/Shower
  HTofHit *tofHit = 0;
  HShowerHitTof *shoHit = 0;

  fMetaPoints->Clear();
  
  fTofIter->Reset();
  while ( (tofHit = (HTofHit *)fTofIter->Next()) != 0) {
    if (tofHit->getSector() == fSecLoc[0] && tofHit->getModule()<8)
      if (tofHit->getTof()>0 && tofHit->getTof()<60)
	addPoint(tofHit);
  }

  fShowerIter->Reset();
  while ( (shoHit = (HShowerHitTof *)fShowerIter->Next()) != 0) {
    if (shoHit->getSector() == fSecLoc[0] && shoHit->getModule()==0)
      addPoint(shoHit);
  }
}

void HRtMetaMatch::addPoint(HTofHit *data) {
  // Transforms and initializes one point
  HGeomVector rLocal,errLocal;
  //  HRtMetaPoint &r = createMetaPoint();
  HGeomVector r;
  Float_t x,y,rodHeight,rodLength;
  //  const Float_t tofXResol=15.5; // TOF resolution in mm
  HModGeomPar *module=fTofGeometry->getModule(data->getSector(), //Sector
					      data->getModule());
  HGeomTransform &transMod = module->getLabTransform();
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())
    ->getTransform();

  HGeomTransform modTrans(transMod);
  modTrans.transTo(transSec);

  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();
  //  const HGeomRotation &modRot=modTrans.getRotMatrix();
  rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - rodVol->getPoint(1)->getY());
  rodLength = 2. * TMath::Abs(rodVol->getPoint(0)->getX());

  x = data->getXpos();

  y = 0.;


  r.setX(x);
  r.setY(y);
  r.setZ(0.);
  rLocal=rodTrans.transFrom(r); //Rod to modue system

  r=modTrans.transFrom(rLocal); //Module to LAB system
  r.setZ( r.getZ() );
  //r.setTof(data->getTof());
  //r.setSystem(2);
  HRtMetaPoint &metaPoint = createMetaPoint();
  metaPoint.setX(r.getX());
  metaPoint.setY(r.getY());
  metaPoint.setZ(r.getZ());
  metaPoint.setTof(data->getTof());
  metaPoint.setSystem(1+data->getModule()*8+data->getCell());
  #warning "Flag for valid checking unset"
  metaPoint.setFlag(0); // ((HTofHitSim2 *)data)->getTrack() );
}


void HRtMetaMatch::addPoint(HShowerHitTof *data) {
  // Transforms and initializes one point from the Shower detector
  Float_t x,y,x0,x1,x2,x3,y0,y1;
  HGeomVector rLocal;
  data->getXY(&x,&y);
  
  //Get transformation from local coord. system to LAB coord system
  const HGeomTransform &transMod=fShowerGeometry->getTransform(data->getSector(),
							       data->getModule());
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
  HGeomTransform trans(transMod);
  trans.transTo(transSec);

  //Get pad geometry
  HShowerPad *pad = fShowerGeometry->getPadParam(0)->getPad(data->getRow(),
							 data->getCol());
  //  const HGeomRotation &rot = trans.getRotMatrix();

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
  HRtMetaPoint &metaPoint = createMetaPoint();
  rLocal.setX(x); 
  rLocal.setY(y);
  rLocal.setZ(0.);
  r = trans.transFrom(rLocal);
  metaPoint.setX(r.getX());
  metaPoint.setY(r.getY());
  metaPoint.setZ(r.getZ());
  if (data->getTofinoMult() == 1)
    metaPoint.setTof(data->getTof());
  else 
    metaPoint.setTof(0.);
  metaPoint.setSystem(0);
  metaPoint.setFlag( 0 ); //data->getTrack() );
}

HRtMetaPoint &HRtMetaMatch::createMetaPoint(void) {
  Int_t n = fMetaPoints->GetEntriesFast();
  HRtMetaPoint *p = new (fMetaPoints->operator[](n)) HRtMetaPoint;
  return *p;
}

Bool_t HRtMetaMatch::finalize(void) {
  fOut->cd();
  fControl->Write();
  fOut->Close();             
  return kTRUE;
}

ClassImp(HRtMetaPoint)
ClassImp(HRtMetaMatch)
