#include "hkickmatch.h"
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

#include "htofhitsim2.h"

//*-- Author : M. Sanchez

//////////////////////////////////////////////////////////////////////
//  HKickMatch
//
//   This class takes the output of the tracking in the MDC system (inner
// and outer) and matches those tracks to hits in TOF and Shower/Tofino. 
// The momentum comes from the MDC, while KickMatch adds velocity and 
// mass calculations.
/////////////////////////////////////////////////////////////////////

HKickMatch::HKickMatch(Text_t name[], Text_t title[]) : 
  HReconstructor(name,title) {
 fMdcCat = 0; 
 fTofCat = 0;
 fShowerCat = 0;
 fTrackCat = 0;
 fMetaPoints = new TClonesArray("HKickMetaPoint",50); // Array of HGeomVector
 fSecLoc.set(1,0);
}

HKickMatch::~HKickMatch(void) {

}

Bool_t HKickMatch::init(void) {
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    HEvent *event = gHades->getCurrentEvent();
    HSpectrometer *spec = gHades->getSetup(); 
    if (event && rtdb) {
      
      fTofCat = event->getCategory(catTofHit);
      if (!fTofCat) {
	Error("init","Tof Hit category not found!");
	return kFALSE;
      }
      fTofIter = dynamic_cast<HIterator *>(fTofCat->MakeIterator());

      fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");
      if (!fTofGeometry) {
	fTofGeometry=(HTofGeomPar *)spec->createDetGeomPar("Tof");
      }

      fMdcCat = event->getCategory(catKickMdcTrk);
      if (!fMdcCat) {
	Error("init","KickMdcTrk category not found!");
	return kFALSE;
      }
      fMdcIter = dynamic_cast<HIterator *>(fMdcCat->MakeIterator());

      fShowerCat = event->getCategory(catShowerHitTof);
      if (!fShowerCat) {
	Error("init","Shower/Tofino hit category not found!");
	return kFALSE;
      }
      fShowerIter = dynamic_cast<HIterator *>(fShowerCat->MakeIterator());

      fShowerGeometry= (HShowerGeometry *)rtdb->getContainer("ShowerGeometry");
      if (!fShowerGeometry) {
	fShowerGeometry = new HShowerGeometry;
	if (!fShowerGeometry) return kFALSE;
	rtdb->addContainer(fShowerGeometry);
      }

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
      if (!fSpecGeometry) {
	fSpecGeometry = (HSpecGeomPar *)spec->createSpecGeomPar();
      }

      fTrackCat = event->getCategory(catTrack);
      if (!fTrackCat) {
	Int_t ssize[2]={6,100};
	fTrackCat = new HMatrixCategory("HKickTrack",2,ssize,0.5);
	if (fTrackCat) event->addCategory(catKickTrack,
					  fTrackCat,
					  "Tracks");
      }
    } else {
      Error("init","! rtdb && event && spec");
      return kFALSE;
    }
  } else {
    Error("init","gHades does not exist");
    return kFALSE;
  }

  fOut = new TFile("match.root","RECREATE");
  fControl = new TNtuple("cont","control","xf:yf:zf:d:dx:dy:valid");
  return kTRUE;
}

Int_t HKickMatch::execute(void) {
  HKickMdcTrk *track=0;
  HKickMetaPoint *point=0;

  fMaxDist = 140.;
  for (fSecLoc[0] = 0; fSecLoc[0] < 6; fSecLoc[0]++) {
    readMetaPoints();
    
    fMdcIter->Reset();
    fMdcIter->gotoLocation(fSecLoc);
    
    while ( (track = (HKickMdcTrk *)fMdcIter->Next()) != 0) {
      point = findClosestPoint(track);
      if (point != 0) {
	fillData(track,point);
      }
    }
  }
  return 0;
}

void HKickMatch::fillData(HKickMdcTrk *track, HKickMetaPoint *point) {
  //Fills output data in the output category
  HKickTrack *c=0;
  TObject *slot=0;
  Double_t v=0,dist,C = 299792458,mass=0,mass2=0;
  Float_t segR, segZ, segTheta, segPhi;
  Double_t tof;
  HGeomVector r;
  HGeomVector alpha = track->getKickPos() - track->getZeroPos();
  const HGeomVector &zpos = track->getZeroPos();
  HGeomVector r0;

  dist = alpha.length();
  alpha /= dist;

  // Calculate position at z=0
  r0.setX( zpos.getX() - (alpha.getX() / alpha.getZ()) * zpos.getZ() );
  r0.setY( zpos.getY() - (alpha.getY() / alpha.getZ()) * zpos.getZ() );
  r0.setZ( 0. );

  // Calculate HMdcSeg coordinates
  segTheta = TMath::ACos( alpha.getZ() );
  segPhi = TMath::ATan2( alpha.getY(), alpha.getX() );
  segR = r0.getY() * TMath::Cos( segPhi ) - r0.getX() * TMath::Sin( segPhi );
  segZ = ( -( r0.getY() * TMath::Sin(segPhi) + r0.getX() * TMath::Cos(segPhi)) *
	   alpha.getZ() ) / TMath::Sin(segTheta);


  tof = point->getTof();
  r.setX(point->getX());
  r.setY(point->getY());
  r.setZ(point->getZ()); 
  if (tof > 0.) {
#warning "Add target position: not for Dubna"
    dist += (r - track->getKickPos()).length();
    dist /=  1000;
    v = (dist/tof) * 1e9;          //TOF in ns
    
#warning "Falta correccion por posicion del tofino para el shower"
    if (v > C) { //It is an electron
      mass=0.;
    } else {
      mass = track->getMomentum() * C * TMath::Sqrt(1 - (v/C) * (v/C)) / v;
    }
    mass2 = track->getMomentum() * track->getMomentum() *
	(1 - (v/C)*(v/C)) / ( (v/C)*(v/C) );
  } else mass=0.;
  //  cout << dist << endl;

//   fMassFitter.fitMomentum(track->getMomentum(),out.getMomentumError(),
// 			  v / C, (v/C) * TMath::Sqrt( (.01/dist)*(.01/dist) + 
// 					     (.1/tof)*(.1/tof)),
// 			  out.getPolarity());
//   if (fMassFitter.getMomentum() > 0.) {
//     Double_t p = 0.;
//     Double_t s1 = 1 / out.getMomentumError(); s1 *= s1;
//     Double_t s2 = 1 / fMassFitter.getMomentumErr(); s2 *= s2;

//     p = (s1 * out.getMomentum() + s2 * fMassFitter.getMomentum()) /
//       (s1 + s2);

//     out.setPTof(p);
//     out.setPTofErr(1./TMath::Sqrt(s1 + s2));
//     out.setPID(fMassFitter.getPID());
//   }

  

  slot = fTrackCat->getNewSlot(fSecLoc);
  if (slot) {
    c = new (slot) HKickTrack;

    c->setZ(segZ ,0.);
    c->setR(segR ,0.);
    c->setTheta(segTheta ,0.);
    c->setPhi(segPhi, 0.);
    c->setP(track->getMomentum(), 0.);
    c->setCharge(track->getCharge());
    c->setMass(mass2, 100. ); 
    c->setQuality(0);
    c->setPTof(0. );
    c->setErrPTof(0. );
    c->setPID(0);
    c->setSystem(point->getSystem());
    c->setTof(point->getTof());
    c->setBeta( v / C );
    c->setSector( track->getSector() );
    c->pull = fLatestDistance ; //FIXME: There are better ways than a global.
  }
}

HKickMetaPoint *HKickMatch::findClosestPoint(HKickMdcTrk *track) {
  // Finds closest hit in TOF/Shower to a given track within a distance limit.
  Int_t n = fMetaPoints->GetEntriesFast();
  Float_t dmin = fMaxDist;
  HKickMetaPoint *pMin = 0;
  Float_t d,dX,dY,bX,bY;
  HGeomVector r;
  HGeomVector alpha,r0;

  for (Int_t i = 0; i < n; i++) {
    HKickMetaPoint &point = *((HKickMetaPoint *)fMetaPoints->UncheckedAt(i));

    r.setX(point.getX());
    r.setY(point.getY());
    r.setZ(point.getZ());
    
    r0 = track->getKickPos();
    alpha = track->getOuterAlpha();

    d = (r0 - r).vectorProduct(alpha).length();
  
    bX = alpha.getX() / alpha.getZ();
    bY = alpha.getY() / alpha.getZ();
    dX = r0.getX() + bX * (r.getZ() - r0.getZ());
    dX -= r.getX();

    dY = r0.getY() + bY * (r.getZ() - r0.getZ());
    dY -= r.getY();

    if (d < dmin) {
	dmin = d;
	pMin = (HKickMetaPoint *)fMetaPoints->UncheckedAt(i);
    }

    fControl->Fill(r0.getX(),r0.getY(),r0.getZ(),
		   d,dX,dY,(pMin == 0)?0:1);
  }
  fLatestDistance = dmin; //FIXME: there are better ways.
  return pMin;
}

void HKickMatch::readMetaPoints(void) {
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

void HKickMatch::addPoint(HTofHit *data) {
  // Transforms and initializes one point
  HGeomVector rLocal,errLocal;
  //  HKickMetaPoint &r = createMetaPoint();
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
  HKickMetaPoint &metaPoint = createMetaPoint();
  metaPoint.setX(r.getX());
  metaPoint.setY(r.getY());
  metaPoint.setZ(r.getZ());
  metaPoint.setTof(data->getTof());
  metaPoint.setSystem(1);
  #warning "Flag for valid checking unset"
  metaPoint.setFlag(0); // ((HTofHitSim2 *)data)->getTrack() );
}

void HKickMatch::addPoint(HShowerHitTof *data) {
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
  HKickMetaPoint &metaPoint = createMetaPoint();
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

HKickMetaPoint &HKickMatch::createMetaPoint(void) {
  Int_t n = fMetaPoints->GetEntriesFast();
  HKickMetaPoint *p = new (fMetaPoints->operator[](n)) HKickMetaPoint;
  return *p;
}

Bool_t HKickMatch::finalize(void) {
  fOut->cd();
  fControl->Write();
  fOut->Close();             
  return kTRUE;
}

ClassImp(HKickMetaPoint)
ClassImp(HKickMatch)
